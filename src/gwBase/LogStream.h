#ifndef GWBASE_LOGSTREAM_H
#define GWBASE_LOGSTREAM_H 1

#include <compiler.h>
#include <gwBase/export.h>
#include <sstream>
#include <vector>
#include <string>

typedef enum
{
	GW_NONE = 0x00000000,

	GW_TERRAIN = 0x00000001,
	GW_ASTRO = 0x00000002,
	GW_FLIGHT = 0x00000004,
	GW_INPUT = 0x00000008,
	GW_GL = 0x00000010,
	GW_VIEW = 0x00000020,
	GW_COCKPIT = 0x00000040,
	GW_GENERAL = 0x00000080,
	GW_MATH = 0x00000100,
	GW_EVENT = 0x00000200,
	GW_AIRCRAFT = 0x00000400,
	GW_AUTOPILOT = 0x00000800,
	GW_IO = 0x00001000,
	GW_CLIPPER = 0x00002000,
	GW_NETWORK = 0x00004000,
	GW_ATC = 0x00008000,
	GW_NASAL = 0x00010000,
	GW_INSTR = 0x00020000,
	GW_SYSTEMS = 0x00040000,
	GW_AI = 0x00080000,
	GW_ENVIRONMENT = 0x00100000,
	GW_SOUND = 0x00200000,
	GW_NAVAID = 0x00400000,
	GW_GUI = 0x00800000,
	GW_TERRASYNC = 0x01000000,
	GW_PARTICLES = 0x02000000,
	GW_UNDEFD = 0x04000000, // For range checking

	GW_ALL = 0xFFFFFFFF
} gwDebugClass;


typedef enum
{
	GW_BULK = 1,       // For frequent messages
	GW_DEBUG,          // Less frequent debug type messages
	GW_INFO,           // Informatory messages
	GW_WARN,           // Possible impending problem
	GW_ALERT,          // Very possible impending problem
	GW_POPUP,          // Severe enough to alert using a pop-up window
	//GW_EXIT,        // Problem (no core)
	//GW_ABORT        // Abandon ship (core)
	GW_DEV_WARN,       // Warning for developers, translated to other priority
	GW_DEV_ALERT       // Alert for developers, translated
} gwDebugPriority;

class GWPath;

namespace gwBase
{
	class GW_BASE_EXPORT LogCallback
	{
	public:
		virtual ~LogCallback() {}
		virtual void operator()(gwDebugClass c, gwDebugPriority p, const char* file, int line, const std::string& aMessage) = 0;

		void setLogLevels(gwDebugClass c, gwDebugPriority p);

	protected:
		LogCallback(gwDebugClass c, gwDebugPriority p);

		bool shouldLog(gwDebugClass c, gwDebugPriority p) const;
	private:
		gwDebugClass m_class;
		gwDebugPriority m_priority;
	};

	/**
	 * Helper force a console on platforms where it might optional, when
	 * we need to show a console. This basically means Windows at the
	 * moment - on other plaforms it's a no-op
	 */
	void requestConsole();

	void shutdownLogging();

} // of namespace gwBase

/**
 * Class to manage the debug logging stream.
 */
class GW_BASE_EXPORT logstream
{
public:
	~logstream();

	static void initGlobalLogstream();
	/**
	 * Set the global log class and priority level.
	 * @param c debug class
	 * @param p priority
	 */
	void setLogLevels(gwDebugClass c, gwDebugPriority p);

	bool would_log(gwDebugClass c, gwDebugPriority p) const;

	void logToFile(const GWPath& aPath, gwDebugClass c, gwDebugPriority p);

	void set_log_priority(gwDebugPriority p);

	void set_log_classes(gwDebugClass c);

	gwDebugClass get_log_classes() const;

	gwDebugPriority get_log_priority() const;

	/**
	 * set developer mode on/off. In developer mode, GW_DEV_WARN messags
	 * are treated as warnings. In normal (non-developer) mode they are
	 * treated as GW_DEBUG.
	 */
	void setDeveloperMode(bool devMode);

	/**
	 * the core logging method
	 */
	void log(gwDebugClass c, gwDebugPriority p,
		const char* fileName, int line, const std::string& msg);

	/**
	 * support for the GW_POPUP logging class
	 * set the content of the popup message
	 */
	void popup(const std::string& msg);

	/**
	 * retrieve the contents of the popup message and clear it's internal
	 * content. The return value may be an empty string.
	 */
	std::string get_popup();

	/**
	 * return true if a new popup message is available. false otherwise.
	 */
	bool has_popup();

	/**
	 * \relates logstream
	 * Return the one and only logstream instance.
	 * We use a function instead of a global object so we are assured that cerr
	 * has been initialised.
	 * @return current logstream
	 */
	friend logstream&  gwlog();

	/**
	 * register a logging callback. Note callbacks are run in a
	 * dedicated thread, so callbacks which pass data to other threads
	 * must use appropriate locking.
	 */
	void addCallback(gwBase::LogCallback* cb);

	void removeCallback(gwBase::LogCallback* cb);

	/**
	 * optionally record all entries and submit them to new log callbacks that
	 * are added. This allows simplified logging configuration, but still including
	 * early startup information in all logs.
	 */
	void setStartupLoggingEnabled(bool enabled);
private:
	logstream();

	std::vector<std::string> popup_msgs;
};

logstream& gwlog();


/** \def GW_LOG(C,P,M)
 * Log a message.
 * @param C debug class
 * @param P priority
 * @param M message
 */
# define GW_LOGX(C,P,M) \
    do { if(gwlog().would_log(C,P)) {                         \
        std::ostringstream os; os << M;                  \
        gwlog().log(C, P, __FILE__, __LINE__, os.str()); \
        if ((P) == GW_POPUP) gwlog().popup(os.str());    \
    } } while(0)
#ifdef NDEBUG
# define GW_LOG(C,P,M)	do { if((P) == GW_POPUP) GW_LOGX(C,P,M); } while(0)
#else
# define GW_LOG(C,P,M)	GW_LOGX(C,P,M)
#endif

#define GW_ORIGIN __FILE__ ":" GW_STRINGIZE(__LINE__)

#endif // GWUTIL_LOGSTREAM_H

