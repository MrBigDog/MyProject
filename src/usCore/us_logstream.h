#ifndef _LOGSTREAM_H
#define _LOGSTREAM_H
#include <usCore\Export.h>
#include "DebugTypes.h"

#include <sstream>
namespace uniscope_globe
{
	class USCORE_EXPORT LogCallback
	{
	public:
		virtual ~LogCallback() {}
		virtual void operator()(mdDebugClass c, gwDebugPriority p, const char* file, int line, const std::string& aMessage) = 0;

		void setLogLevels(mdDebugClass c, gwDebugPriority p);
	protected:
		LogCallback(mdDebugClass c, gwDebugPriority p);

		bool shouldLog(mdDebugClass c, gwDebugPriority p) const;
	private:
		mdDebugClass class_;
		gwDebugPriority priority_;
	};

	void requestConsole();



	class USCORE_EXPORT logstream
	{
	public:
		static void initGlobalLogstream();

		void setLogLevels(mdDebugClass c, gwDebugPriority p);

		bool would_log(mdDebugClass c, gwDebugPriority p) const;

		void logToFile(const std::string& aPath, mdDebugClass c, gwDebugPriority p);

		void set_log_priority(gwDebugPriority p);

		void set_log_classes(mdDebugClass c);

		mdDebugClass get_log_classes() const;

		gwDebugPriority get_log_priority() const;

		void log(mdDebugClass c, gwDebugPriority p, const char* fileName, int line, const std::string& msg);

		friend logstream& gwlog();

		void addCallback(uniscope_globe::LogCallback* cb);

		void removeCallback(uniscope_globe::LogCallback* cb);

	private:
		logstream();
	};


	logstream& gwlog();


#ifdef GW_NDEBUG
# define GW_LOG(C,P,M)
#else
# define GW_LOG(C,P,M) do {                     \
	if(gwlog().would_log(C,P)) {                      \
	std::ostringstream os;                             \
	os << M;                                       \
	gwlog().log(C, P, __FILE__, __LINE__, os.str()); \
	} \
	} while(0)
#endif
}
//#define GW_ORIGIN __FILE__ ":" GW_STRINGIZE(__LINE__)
//GW_LOG(MD_GENERAL, MD_ALERT, "command '" << name << "' failed with exception\n"
//	"\tmessage:" << e.getMessage() << " (from " << e.getOrigin() << ")");
#endif // _LOGSTREAM_H

