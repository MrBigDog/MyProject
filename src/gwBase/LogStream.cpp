#include <gwBase/LogStream.h>
#include <gwBase/Thread.h>
#include <gwBase/Queue.h>
#include <gwBase/Guard.h>
#include <gwBase/Stream.h>
#include <gwBase/Path.h>

#include <boost/foreach.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

#if defined (GW_WINDOWS)
// for AllocConsole, OutputDebugString
#include <windows.h>
#include <fcntl.h>
#include <io.h>
#endif

const char* debugClassToString(gwDebugClass c)
{
	switch (c)
	{
	case GW_NONE:       return "none";
	case GW_TERRAIN:    return "terrain";
	case GW_ASTRO:      return "astro";
	case GW_FLIGHT:     return "flight";
	case GW_INPUT:      return "input";
	case GW_GL:         return "opengl";
	case GW_VIEW:       return "view";
	case GW_COCKPIT:    return "cockpit";
	case GW_GENERAL:    return "general";
	case GW_MATH:       return "math";
	case GW_EVENT:      return "event";
	case GW_AIRCRAFT:   return "aircraft";
	case GW_AUTOPILOT:  return "autopilot";
	case GW_IO:         return "io";
	case GW_CLIPPER:    return "clipper";
	case GW_NETWORK:    return "network";
	case GW_ATC:        return "atc";
	case GW_NASAL:      return "nasal";
	case GW_INSTR:      return "instruments";
	case GW_SYSTEMS:    return "systems";
	case GW_AI:         return "ai";
	case GW_ENVIRONMENT:return "environment";
	case GW_SOUND:      return "sound";
	case GW_NAVAID:     return "navaid";
	case GW_GUI:        return "gui";
	case GW_TERRASYNC:  return "terrasync";
	case GW_PARTICLES:  return "particles";
	default:            return "unknown";
	}
}

//////////////////////////////////////////////////////////////////////////////
namespace gwBase
{
	LogCallback::LogCallback(gwDebugClass c, gwDebugPriority p)
		: m_class(c)
		, m_priority(p)
	{ }

	bool LogCallback::shouldLog(gwDebugClass c, gwDebugPriority p) const
	{
		return ((c & m_class) != 0 && p >= m_priority);
	}

	void LogCallback::setLogLevels(gwDebugClass c, gwDebugPriority p)
	{
		m_priority = p;
		m_class = c;
	}

} // of namespace gwBase

//////////////////////////////////////////////////////////////////////////////
class FileLogCallback : public gwBase::LogCallback
{
public:
	FileLogCallback(const GWPath& aPath, gwDebugClass c, gwDebugPriority p)
		: gwBase::LogCallback(c, p)
	{
		m_file.open(aPath, std::ios_base::out | std::ios_base::trunc);
	}

	virtual void operator()(gwDebugClass c, gwDebugPriority p,
		const char* file, int line, const std::string& message)
	{
		if (!shouldLog(c, p))
		{
			return;
		}
		m_file << debugClassToString(c) << ":" << (int)p << ":" << file << ":" << line << ":" << message << std::endl;
	}
private:
	gw_ofstream m_file;
};

class StderrLogCallback : public gwBase::LogCallback
{
public:
	StderrLogCallback(gwDebugClass c, gwDebugPriority p)
		: gwBase::LogCallback(c, p)
	{ }

#if defined (GW_WINDOWS)
	~StderrLogCallback()
	{
		FreeConsole();
	}
#endif

	virtual void operator()(gwDebugClass c, gwDebugPriority p,
		const char* file, int line, const std::string& aMessage)
	{
		if (!shouldLog(c, p)) return;

		fprintf(stderr, "%s\n", aMessage.c_str());
		//fprintf(stderr, "%s:%d:%s:%d:%s\n", debugClassToString(c), p,
		//    file, line, aMessage.c_str());
		fflush(stderr);
	}
};


#ifdef GW_WINDOWS
class WinDebugLogCallback : public gwBase::LogCallback
{
public:
	WinDebugLogCallback(gwDebugClass c, gwDebugPriority p) :
		gwBase::LogCallback(c, p)
	{ }

	virtual void operator()(gwDebugClass c, gwDebugPriority p, const char* file, int line, const std::string& aMessage)
	{
		if (!shouldLog(c, p)) return;

		std::ostringstream os;
		os << debugClassToString(c) << ":" << aMessage << std::endl;
		OutputDebugStringA(os.str().c_str());
	}
};
#endif

class LogStreamPrivate : public GWThread
{
private:
	/**
	 * storage of a single log entry. This is used to pass log entries from
	 * the various threads to the logging thread, and also to store the startup
	 * entries
	 */
	class LogEntry
	{
	public:
		LogEntry(gwDebugClass c, gwDebugPriority p, const char* f, int l, const std::string& msg)
			: debugClass(c)
			, debugPriority(p)
			, file(f)
			, line(l)
			, message(msg)
		{ }

		const gwDebugClass debugClass;
		const gwDebugPriority debugPriority;
		const char* file;
		const int line;
		const std::string message;
	};

	/**
	 * RAII object to pause the logging thread if it's running, and restart it.
	 * used to safely make configuration changes.
	 */
	class PauseThread
	{
	public:
		PauseThread(LogStreamPrivate* parent)
			: m_parent(parent)
			, m_wasRunning(m_parent->stop())
		{
		}

		~PauseThread()
		{
			if (m_wasRunning)
			{
				m_parent->startLog();
			}
		}
	private:
		LogStreamPrivate* m_parent;
		const bool m_wasRunning;
	};

public:
	LogStreamPrivate()
		: m_logClass(GW_ALL)
		, m_logPriority(GW_ALERT)
	{
#if defined (GW_WINDOWS)
		/*
		 * 2016-09-20(RJH) - Reworked console handling
		 * 1) When started from the console use the console (when no --console)
		 * 2) When started from the GUI (with --console) open a new console window
		 * 3) When started from the GUI (without --console) don't open a new console
		 *    window; stdout/stderr will not appear (except in logfiles as they do now)
		 * 4) When started from the Console (with --console) open a new console window
		 * 5) Ensure that IO redirection still works when started from the console
		 *
		 * Notes:
		 * 1) fgfs needs to be a GUI subsystem app - which it already is
		 * 2) What can't be done is to make the cmd prompt run fgfs synchronously;
		 * this is only something that can be done via "start /wait fgfs".
		 */

		int stderr_handle_type = GetFileType(GetStdHandle(STD_ERROR_HANDLE));
		int stdout_handle_type = GetFileType(GetStdHandle(STD_OUTPUT_HANDLE));
		int stdout_isNull = 0;
		int stderr_isNull = 0;

		m_stderr_isRedirectedAlready = stderr_handle_type == FILE_TYPE_DISK || stderr_handle_type == FILE_TYPE_PIPE || stderr_handle_type == FILE_TYPE_CHAR;
		m_stdout_isRedirectedAlready = stdout_handle_type == FILE_TYPE_DISK || stdout_handle_type == FILE_TYPE_PIPE || stdout_handle_type == FILE_TYPE_CHAR;

		/*
		 * We don't want to attach to the console if either stream has been redirected - so in this case ensure that both streams
		 * are redirected as otherwise something will be lost (as Alloc or Attach Console will cause the handles that were bound
		 * to disappear)
		 */
		if (m_stdout_isRedirectedAlready)
		{
			if (!m_stderr_isRedirectedAlready)
			{
				MessageBox(0, "Redirection only works when you use 2>&1 before using > or |\r\n(e.g. fgfs 2>&1 | more)", "gwBase Error", MB_OK | MB_ICONERROR);
				exit(3);
			}
		}
		else
		{
			/*
			* Attempt to attach to the console process of the parent process; when launched from cmd.exe this should be the console,
			* when launched via the RUN menu explorer, or another GUI app that wasn't started from the console this will fail.
			* When it fails we will redirect to the NUL device. This is to ensure that we have valid streams.
			* Later on in the initialisation sequence the --console option will be processed and this will cause the requestConsole() to
			* always open a new console, except for streams that are redirected. The same rules apply there, if both streams are redirected
			* the console will be opened, and it will contain a message to indicate that no output will be present because the streams are redirected
			*/
			if (AttachConsole(ATTACH_PARENT_PROCESS) == 0)
			{
				/*
				* attach failed - so ensure that the streams are bound to the null device - but only when not already redirected
				*/
				if (!m_stdout_isRedirectedAlready)
				{
					stdout_isNull = true;
					freopen("NUL", "w", stdout);
				}

				if (!m_stderr_isRedirectedAlready)
				{
					stderr_isNull = true;
					freopen("NUL", "w", stderr);
				}
			}
			/*
			* providing that AttachConsole succeeded - we can then either reopen the stream onto the console, or use
			* _fdopen to attached to the currently redirected (and open stream)
			*/
			if (!stdout_isNull)
			{
				if (!m_stdout_isRedirectedAlready)
					freopen("conout$", "w", stdout);
				else
					/*
					* for already redirected streams we need to attach the stream to the OS handle that is open.
					* - this comes from part of the answer http://stackoverflow.com/a/13841522
					*   _open_osfhandle returns an FD for the Win32 Handle, which is then opened using fdopen and
					*   hopefully safely assigned to the stream (although it does look wrong to me it works)
					* Removing this bit will stop pipes and command line redirection (> 2> and 2>&1 from working)
					*/
					*stdout = *_fdopen(_open_osfhandle((intptr_t)GetStdHandle(STD_OUTPUT_HANDLE), _O_WRONLY), "a");
			}

			if (!stderr_isNull)
			{
				if (!m_stderr_isRedirectedAlready)
					freopen("conout$", "w", stderr);
				else
					*stderr = *_fdopen(_open_osfhandle((intptr_t)GetStdHandle(STD_ERROR_HANDLE), _O_WRONLY), "a");
			}
		}
		//http://stackoverflow.com/a/25927081
		//Clear the error state for each of the C++ standard stream objects. 
		std::wcout.clear();
		std::cout.clear();
		std::wcerr.clear();
		std::cerr.clear();
#endif

		m_callbacks.push_back(new StderrLogCallback(m_logClass, m_logPriority));
		m_consoleCallbacks.push_back(m_callbacks.back());
#if defined (GW_WINDOWS) && !defined(NDEBUG)
		m_callbacks.push_back(new WinDebugLogCallback(m_logClass, m_logPriority));
		m_consoleCallbacks.push_back(m_callbacks.back());
#endif
	}

	~LogStreamPrivate()
	{
		for (gwBase::LogCallback* cb : m_callbacks)
		{
			delete cb;
		}
	}

	GWMutex m_lock;
	GWBlockingQueue<LogEntry> m_entries;

	// log entries posted during startup
	std::vector<LogEntry> m_startupEntries;
	bool m_startupLogging = false;

	typedef std::vector<gwBase::LogCallback*> CallbackVec;
	CallbackVec m_callbacks;
	/// subset of callbacks which correspond to stdout / console,
	/// and hence should dynamically reflect console logging settings
	CallbackVec m_consoleCallbacks;

	gwDebugClass m_logClass;
	gwDebugPriority m_logPriority;
	bool m_isRunning = false;
#if defined (GW_WINDOWS)
	// track whether the console was redirected on launch (in the constructor, which is called early on)
	bool m_stderr_isRedirectedAlready = false;
	bool m_stdout_isRedirectedAlready = false;
#endif
	bool m_developerMode = false;

	void startLog()
	{
		GWGuard<GWMutex> g(m_lock);
		if (m_isRunning) return;
		m_isRunning = true;
		start();
	}

	void setStartupLoggingEnabled(bool on)
	{
		if (m_startupLogging == on)
		{
			return;
		}

		m_startupLogging = on;
		m_startupEntries.clear();
	}

	virtual void run()
	{
		while (1)
		{
			LogEntry entry(m_entries.pop());
			// special marker entry detected, terminate the thread since we are
			// making a configuration change or quitting the app
			if ((entry.debugClass == GW_NONE) && !strcmp(entry.file, "done"))
			{
				return;
			}

			if (m_startupLogging)
			{
				// save to the startup list for not-yet-added callbacks to
				// pull down on startup
				m_startupEntries.push_back(entry);
			}

			// submit to each installed callback in turn
			for (gwBase::LogCallback* cb : m_callbacks)
			{
				(*cb)(entry.debugClass, entry.debugPriority,
					entry.file, entry.line, entry.message);
			}
		} // of main thread loop
	}

	bool stop()
	{
		GWGuard<GWMutex> g(m_lock);
		if (!m_isRunning)
		{
			return false;
		}

		// log a special marker value, which will cause the thread to wakeup,
		// and then exit
		log(GW_NONE, GW_ALERT, "done", -1, "");
		join();

		m_isRunning = false;
		return true;
	}

	void addCallback(gwBase::LogCallback* cb)
	{
		PauseThread pause(this);
		m_callbacks.push_back(cb);

		// we clear startup entries not using this, so always safe to run
		// this code, container will simply be empty
		for (auto entry : m_startupEntries)
		{
			(*cb)(entry.debugClass, entry.debugPriority,
				entry.file, entry.line, entry.message);
		}
	}

	void removeCallback(gwBase::LogCallback* cb)
	{
		PauseThread pause(this);
		CallbackVec::iterator it = std::find(m_callbacks.begin(), m_callbacks.end(), cb);
		if (it != m_callbacks.end())
		{
			m_callbacks.erase(it);
		}
	}

	void setLogLevels(gwDebugClass c, gwDebugPriority p)
	{
		PauseThread pause(this);
		m_logPriority = p;
		m_logClass = c;
		BOOST_FOREACH(gwBase::LogCallback* cb, m_consoleCallbacks)
		{
			cb->setLogLevels(c, p);
		}
	}

	bool would_log(gwDebugClass c, gwDebugPriority p) const
	{
		p = translatePriority(p);
		if (p >= GW_INFO) return true;
		return ((c & m_logClass) != 0 && p >= m_logPriority);
	}

	void log(gwDebugClass c, gwDebugPriority p,
		const char* fileName, int line, const std::string& msg)
	{
		p = translatePriority(p);
		LogEntry entry(c, p, fileName, line, msg);
		m_entries.push(entry);
	}

	gwDebugPriority translatePriority(gwDebugPriority in) const
	{
		if (in == GW_DEV_WARN) 
		{
			return m_developerMode ? GW_WARN : GW_DEBUG;
		}

		if (in == GW_DEV_ALERT) 
		{
			return m_developerMode ? GW_POPUP : GW_WARN;
		}

		return in;
	}
};

/////////////////////////////////////////////////////////////////////////////

static logstream* global_logstream = NULL;
static LogStreamPrivate* global_privateLogstream = NULL;
static GWMutex global_logStreamLock;

logstream::logstream()
{
	global_privateLogstream = new LogStreamPrivate;
	global_privateLogstream->startLog();
}

logstream::~logstream()
{
	popup_msgs.clear();
	global_privateLogstream->stop();
	delete global_privateLogstream;
}

void logstream::setLogLevels(gwDebugClass c, gwDebugPriority p)
{
	global_privateLogstream->setLogLevels(c, p);
}

void logstream::setDeveloperMode(bool devMode)
{
	global_privateLogstream->m_developerMode = devMode;
}


void logstream::addCallback(gwBase::LogCallback* cb)
{
	global_privateLogstream->addCallback(cb);
}

void logstream::removeCallback(gwBase::LogCallback* cb)
{
	global_privateLogstream->removeCallback(cb);
}

void logstream::log(gwDebugClass c, gwDebugPriority p,
	const char* fileName, int line, const std::string& msg)
{
	global_privateLogstream->log(c, p, fileName, line, msg);
}

void logstream::popup(const std::string& msg)
{
	popup_msgs.push_back(msg);
}

std::string logstream::get_popup()
{
	std::string rv = "";
	if (!popup_msgs.empty())
	{
		rv = popup_msgs.front();
		popup_msgs.erase(popup_msgs.begin());
	}
	return rv;
}

bool logstream::has_popup()
{
	return (popup_msgs.size() > 0) ? true : false;
}

bool logstream::would_log(gwDebugClass c, gwDebugPriority p) const
{
	return global_privateLogstream->would_log(c, p);
}

gwDebugClass logstream::get_log_classes() const
{
	return global_privateLogstream->m_logClass;
}

gwDebugPriority logstream::get_log_priority() const
{
	return global_privateLogstream->m_logPriority;
}

void logstream::set_log_priority(gwDebugPriority p)
{
	global_privateLogstream->setLogLevels(global_privateLogstream->m_logClass, p);
}

void logstream::set_log_classes(gwDebugClass c)
{
	global_privateLogstream->setLogLevels(c, global_privateLogstream->m_logPriority);
}


logstream& gwlog()
{
	// Force initialization of cerr.
	static std::ios_base::Init initializer;

	// http://www.aristeia.com/Papers/DDJ_Jul_Aug_2004_revised.pdf
	// in the absence of portable memory barrier ops in gwBase,
	// let's keep this correct & safe
	GWGuard<GWMutex> g(global_logStreamLock);

	if (!global_logstream)
		global_logstream = new logstream();
	return *global_logstream;
}

void logstream::logToFile(const GWPath& aPath, gwDebugClass c, gwDebugPriority p)
{
	global_privateLogstream->addCallback(new FileLogCallback(aPath, c, p));
}

void logstream::setStartupLoggingEnabled(bool enabled)
{
	global_privateLogstream->setStartupLoggingEnabled(enabled);
}

namespace gwBase
{
	void requestConsole()
	{
#if defined (GW_WINDOWS)
		/*
		 * 2016-09-20(RJH) - Reworked console handling
		 * This is part of the reworked console handling for Win32. This is for building as a Win32 GUI Subsystem where no
		 * console is allocated on launch. If building as a console app then the startup will ensure that a console is created - but
		 * we don't need to handle that.
		 * The new handling is quite simple:
		 *              1. The constructor will ensure that these streams exists. It will attach to the
		 *                 parent command prompt if started from the command prompt, otherwise the
		 *                 stdout/stderr will be bound to the NUL device.
		 *              2. with --console a window will always appear regardless of where the process was
		 *                 started from. Any non redirected streams will be redirected
		 *              3. You cannot use --console and either redirected stream.
		 *
		 * This is called after the Private Log Stream constructor so we need to undo any console that it has attached to.
		 */

		if (!global_privateLogstream->m_stderr_isRedirectedAlready && !global_privateLogstream->m_stdout_isRedirectedAlready) {
			FreeConsole();
			if (AllocConsole())
			{
				if (!global_privateLogstream->m_stdout_isRedirectedAlready)
					freopen("conout$", "w", stdout);

				if (!global_privateLogstream->m_stderr_isRedirectedAlready)
					freopen("conout$", "w", stderr);

				//http://stackoverflow.com/a/25927081
				//Clear the error state for each of the C++ standard stream objects. 
				std::wcout.clear();
				std::cout.clear();
				std::wcerr.clear();
				std::cerr.clear();
			}
		}
		else
		{
			MessageBox(0, "--console ignored because stdout or stderr redirected with > or 2>", "gwBase Error", MB_OK | MB_ICONERROR);
		}
#endif
	}

	void shutdownLogging()
	{
		GWGuard<GWMutex> g(global_logStreamLock);
		delete global_logstream;
		global_logstream = 0;
	}

} // of namespace gwBase
