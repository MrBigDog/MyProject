/**@brief 日志公共类
* @author Felix
@date 2017/10/11
*/
#ifndef US_LOGCREATOR_INCLUDE
#define US_LOGCREATOR_INCLUDE
#include <usCore\Export.h>
#include <sstream>
#include <WinBase.h>
class LogTrace : public std::stringstream
{
public:
	LogTrace(){}
	~LogTrace(void)
	{
		try
		{
			
			OutputDebugStringA(str().c_str());
		}
		catch (...)
		{

		}
	}
};

#ifdef GW_DEBUG_LOG///debug
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/helpers/loglog.h>
#include <log4cplus/helpers/stringhelper.h>
#include <log4cplus/fileappender.h>

using namespace log4cplus;
using namespace log4cplus::helpers;
#define GWLOG_TRACE_METHOD(logEvent) LOG4CPLUS_TRACE_METHOD(singleton_gw_log::GetGWInstance(), logEvent)
#define GWLOG_DEBUG(logEvent)		 LOG4CPLUS_DEBUG(singleton_gw_log::GetGWInstance(), logEvent)
#define GWLOG_INFO(logEvent)		 LOG4CPLUS_INFO(singleton_gw_log::GetGWInstance(), logEvent)
#define GWLOG_WARN(logEvent)		 LOG4CPLUS_WARN(singleton_gw_log::GetGWInstance(), logEvent)
#define GWLOG_ERROR(logEvent)		 LOG4CPLUS_ERROR(singleton_gw_log::GetGWInstance(), logEvent)
#define GWLOG_FATAL(logEvent)		 LOG4CPLUS_FATAL(singleton_gw_log::GetGWInstance(), logEvent)
//static log4cplus::Logger g_logger;

class singleton_gw_log: public log4cplus::Logger
{
public:
	static log4cplus::Logger GetGWInstance( );
	static void GWDestroy();
protected:
	singleton_gw_log(){};
	~singleton_gw_log(){};
private:
	static log4cplus::Logger _logger;
	static char* _tmp_symbol;
};

#define AUTO_DELETE_LOG singleton_gw_log::GWDestroy();
#else//release
#define GWLOG_OUT(logEvent)			{LogTrace() <<logEvent<<" " << __FILE__ << "("<<__LINE__<< ")"<< std::endl;}
#define GWLOG_TRACE_METHOD(logEvent)	GWLOG_OUT(logEvent)	
#define GWLOG_DEBUG(logEvent)			GWLOG_OUT(logEvent)	
#define GWLOG_INFO(logEvent)			GWLOG_OUT(logEvent)	
#define GWLOG_WARN(logEvent)			GWLOG_OUT(logEvent)	
#define GWLOG_ERROR(logEvent)			GWLOG_OUT(logEvent)	
#define GWLOG_FATAL(logEvent)			GWLOG_OUT(logEvent)	
#endif

#endif //US_LOGCREATOR_INCLUDE
