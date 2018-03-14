/**@add by Felix »’÷æ¿‡*/
//#include "stdafx.h"
#include "us_logstream.h"
#include "us_queue.h"
#include <usUtil/us_mutex.h>
#include <usUtil/us_singleton.h>
#include <usCore/us_thread_base.h>
#include <usCore/us_thread_valve.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>



#ifdef GW_WINDOWS
#include "windows.h"
#endif

namespace uniscope_globe
{
	const char* debugClassToString(mdDebugClass c)
	{
		switch (c)
		{
		case MD_NONE:       return "none";
		case MD_TERRAIN:    return "terrain";
		case MD_ASTRO:      return "astro";
		case MD_FLIGHT:     return "flight";
		case MD_INPUT:      return "input";
		case MD_GL:         return "opengl";
		case MD_VIEW:       return "view";
		case MD_COCKPIT:    return "cockpit";
		case MD_GENERAL:    return "general";
		case MD_MATH:       return "math";
		case MD_EVENT:      return "event";
		case MD_AIRCRAFT:   return "aircraft";
		case MD_AUTOPILOT:  return "autopilot";
		case MD_IO:         return "io";
		case MD_CLIPPER:    return "clipper";
		case MD_NETWORK:    return "network";
		case MD_ATC:        return "atc";
		case MD_NASAL:      return "nasal";
		case MD_INSTR:      return "instruments";
		case MD_SYSTEMS:    return "systems";
		case MD_AI:         return "ai";
		case MD_ENVIRONMENT:return "environment";
		case MD_SOUND:      return "sound";
		case MD_NAVAID:     return "navaid";
		case MD_GUI:        return "gui";
		case MD_TERRASYNC:  return "terrasync";
		default:            return "unknown";
		}
	}

	//////////////////////////////////////////////////////////////////////////////


	LogCallback::LogCallback(mdDebugClass c, gwDebugPriority p) :
		class_(c),
		priority_(p)
	{
	}

	bool LogCallback::shouldLog(mdDebugClass c, gwDebugPriority p) const
	{
		return ((c & class_) != 0 && p >= priority_);
	}

	void LogCallback::setLogLevels(mdDebugClass c, gwDebugPriority p)
	{
		priority_ = p;
		class_ = c;
	}



	class FileLogCallback : public uniscope_globe::LogCallback
	{
	public:
		FileLogCallback(const std::string& aPath, mdDebugClass c, gwDebugPriority p)
			: uniscope_globe::LogCallback(c, p)
			, m_file(aPath.c_str(), std::ios_base::out | std::ios_base::trunc)
		{
		}

		virtual void operator()(mdDebugClass c, gwDebugPriority p, const char* file, int line, const std::string& message)
		{
			if (!shouldLog(c, p)) return;
			m_file << debugClassToString(c) << ":" << (int)p
				<< ":" << file << ":" << line << ":" << message << std::endl;
		}
	private:
		std::ofstream m_file;
	};

	class StderrLogCallback : public uniscope_globe::LogCallback
	{
	public:
		StderrLogCallback(mdDebugClass c, gwDebugPriority p) :
			uniscope_globe::LogCallback(c, p)
		{
#ifdef GW_WINDOWS
			AllocConsole(); // but only if we want a console
			freopen("conin$", "r", stdin);
			freopen("conout$", "w", stdout);
			freopen("conout$", "w", stderr);
#endif
		}

		virtual void operator()(mdDebugClass c, gwDebugPriority p,
			const char* /*file*/, int /*line*/, const std::string& aMessage)
		{
			if (!shouldLog(c, p)) return;

			fprintf(stderr, "%s\n", aMessage.c_str());
			//fprintf(stderr, "%s:%d:%s:%d:%s\n", debugClassToString(c), p,
			//    file, line, aMessage.c_str());
			fflush(stderr);
		}
	};


#ifdef GW_WINDOWS

	class WinDebugLogCallback : public uniscope_globe::LogCallback
	{
	public:
		WinDebugLogCallback(mdDebugClass c, gwDebugPriority p) :
			uniscope_globe::LogCallback(c, p)
		{
		}

		virtual void operator()(mdDebugClass c, gwDebugPriority p,
			const char* file, int line, const std::string& aMessage)
		{
			if (!shouldLog(c, p)) return;

			std::ostringstream os;
			os << debugClassToString(c) << ":" << aMessage << std::endl;
			OutputDebugStringA(os.str().c_str());
		}
	};

#endif

	class LogStreamPrivate : public uniscope_globe::thread_base
	{
	private:
		class LogEntry
		{
		public:
			LogEntry(mdDebugClass c, gwDebugPriority p, const char* f, int l, const std::string& msg)
				: debugClass(c), debugPriority(p), file(f), line(l), message(msg)
			{
			}

			mdDebugClass debugClass;
			gwDebugPriority debugPriority;
			const char* file;
			int line;
			std::string message;
		};

		class PauseThread
		{
		public:
			PauseThread(LogStreamPrivate* parent) : m_parent(parent)
			{
				m_wasRunning = m_parent->stop();
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
			bool m_wasRunning;
		};
	public:
		LogStreamPrivate()
			: m_logClass(MD_ALL), m_logPriority(MD_ALERT), m_isRunning(false), m_consoleRequested(false)
		{
			thread_base::create();
#if !defined(GW_WINDOWS)
			m_callbacks.push_back(new StderrLogCallback(m_logClass, m_logPriority));
			m_consoleCallbacks.push_back(m_callbacks.back());
			m_consoleRequested = true;
#endif

#if defined (GW_WINDOWS) && !defined(NDEBUG)
			m_callbacks.push_back(new WinDebugLogCallback(m_logClass, m_logPriority));
			m_consoleCallbacks.push_back(m_callbacks.back());
#endif
		}

		US_AUTO_MUTEX
			GWBlockingQueue<LogEntry> m_entries;

		typedef std::vector<uniscope_globe::LogCallback*> CallbackVec;
		CallbackVec m_callbacks;
		/// subset of callbacks which correspond to stdout / console,
		/// and hence should dynamically reflect console logging settings
		CallbackVec m_consoleCallbacks;

		mdDebugClass m_logClass;
		gwDebugPriority m_logPriority;
		bool m_isRunning;
		bool m_consoleRequested;

		void startLog()
		{
			US_LOCK_AUTO_MUTEX
				if (m_isRunning) return;
			m_isRunning = true;
			thread_run();
		}

		bool on_waiting(void)
		{
			int time_sleep = singleton_thread_valve::instance().control_time();
			time_sleep = max(0, time_sleep);
			time_sleep = min(200, time_sleep);

			if (!thread_private_info.m_canceling)
			{
				Sleep(time_sleep);
			}
			return thread_base::on_waiting();
		}

		void terminate()
		{
			thread_base::destroy();
		}

		void on_running()
		{
			LogEntry entry(m_entries.pop());
			// special marker entry detected, terminate the thread since we are
			// making a configuration change or quitting the app
			if ((entry.debugClass == MD_NONE) && !strcmp(entry.file, "done"))
			{
				return;
			}

			// submit to each installed callback in turn
			//std::for_each(uniscope_globe::LogCallback* cb, m_callbacks) 
			for (int index = 0; index < m_callbacks.size(); ++index)
			{
				//uniscope_globe::LogCallback* cb = m_callbacks[index];
				(*m_callbacks[index])(entry.debugClass, entry.debugPriority,
					entry.file, entry.line, entry.message);
			}
		}

		bool stop()
		{
			US_LOCK_AUTO_MUTEX
				if (!m_isRunning)
				{
					return false;
				}

			// log a special marker value, which will cause the thread to wakeup,
			// and then exit
			log(MD_NONE, MD_ALERT, "done", -1, "");
			thread_base::thread_stop();

			m_isRunning = false;
			return true;
		}

		void addCallback(uniscope_globe::LogCallback* cb)
		{
			PauseThread pause(this);
			m_callbacks.push_back(cb);
		}

		void removeCallback(uniscope_globe::LogCallback* cb)
		{
			PauseThread pause(this);
			CallbackVec::iterator it = std::find(m_callbacks.begin(), m_callbacks.end(), cb);
			if (it != m_callbacks.end()) {
				m_callbacks.erase(it);
			}
		}

		void setLogLevels(mdDebugClass c, gwDebugPriority p)
		{
			PauseThread pause(this);
			m_logPriority = p;
			m_logClass = c;
			//for(uniscope_globe::LogCallback* cb; m_consoleCallbacks)
			for (int index = 0; index < m_consoleCallbacks.size(); ++index)
			{
				uniscope_globe::LogCallback* cb = m_consoleCallbacks[index];
				cb->setLogLevels(c, p);
			}
		}

		bool would_log(mdDebugClass c, gwDebugPriority p) const
		{
			if (p >= MD_INFO) return true;
			return ((c & m_logClass) != 0 && p >= m_logPriority);
		}

		void log(mdDebugClass c, gwDebugPriority p,
			const char* fileName, int line, const std::string& msg)
		{
			LogEntry entry(c, p, fileName, line, msg);
			m_entries.push(entry);
		}

		void requestConsole()
		{
			PauseThread pause(this);
			if (m_consoleRequested) {
				return;
			}

			m_consoleRequested = true;
			m_callbacks.push_back(new StderrLogCallback(m_logClass, m_logPriority));
			m_consoleCallbacks.push_back(m_callbacks.back());
		}
	};

	/////////////////////////////////////////////////////////////////////////////
	static logstream* global_logstream = NULL;
	static LogStreamPrivate* global_privateLogstream = NULL;

	logstream::logstream()
	{
		global_privateLogstream = new LogStreamPrivate;
		global_privateLogstream->startLog();
	}

	void logstream::setLogLevels(mdDebugClass c, gwDebugPriority p)
	{
		global_privateLogstream->setLogLevels(c, p);
	}

	void logstream::addCallback(uniscope_globe::LogCallback* cb)
	{
		global_privateLogstream->addCallback(cb);
	}

	void logstream::removeCallback(uniscope_globe::LogCallback* cb)
	{
		global_privateLogstream->removeCallback(cb);
	}

	void logstream::log(mdDebugClass c, gwDebugPriority p,
		const char* fileName, int line, const std::string& msg)
	{
		global_privateLogstream->log(c, p, fileName, line, msg);
	}

	bool logstream::would_log(mdDebugClass c, gwDebugPriority p) const
	{
		return global_privateLogstream->would_log(c, p);
	}

	mdDebugClass logstream::get_log_classes() const
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

	void logstream::set_log_classes(mdDebugClass c)
	{
		global_privateLogstream->setLogLevels(c, global_privateLogstream->m_logPriority);
	}

	logstream& gwlog()
	{
		// Force initialization of cerr.
		static std::ios_base::Init initializer;

		// let's keep this correct & safe
		US_LOCK_MUTEX(global_privateLogstream->mutex)

			if (!global_logstream)
				global_logstream = new logstream();
		return *global_logstream;
	}

	void logstream::logToFile(const std::string& aPath, mdDebugClass c, gwDebugPriority p)
	{
		global_privateLogstream->addCallback(new FileLogCallback(aPath, c, p));
	}




	void requestConsole()
	{
		gwlog();
		global_privateLogstream->requestConsole();
	}

}
