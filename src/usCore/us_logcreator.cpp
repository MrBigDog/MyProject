//#include "stdafx.h"

#ifdef GW_DEBUG_LOG///debug
log4cplus::Logger singleton_gw_log::_logger = Logger::getInstance(LOG4CPLUS_TEXT("GeniusWorld"));
char* singleton_gw_log::_tmp_symbol = nullptr;
log4cplus::Logger singleton_gw_log::GetGWInstance( )
{
	if ( _tmp_symbol == nullptr )
	{
		_tmp_symbol = new char('@');

		log4cplus::initialize();
		LogLog::getLogLog()->setInternalDebugging(true);
		_logger = Logger::getInstance(LOG4CPLUS_TEXT("GeniusWorld"));
		SharedAppenderPtr pFileAppender1(new FileAppender( uniscope_globe::singleton_system_environment::instance().m_log_path ));
		auto_ptr<Layout> pPatternLayout(new PatternLayout( LOG4CPLUS_TEXT("%d{%m/%d/%y %H:%M:%S}    - %m [%l]%n"))); 
		///02/07/17 07:14:04    - This is a <Warn> log message... [e:\fxl\project\log4\log4cplus-1.2.0-rc3\tests\ostream_test\main.cxx:87]
		pFileAppender1->setLayout(pPatternLayout); 
		_logger.addAppender(pFileAppender1); 
		_logger.setLogLevel(INFO_LOG_LEVEL);

		return _logger;
	}

	return _logger;
}

void singleton_gw_log::GWDestroy()
{
	AUTO_DELETE(_tmp_symbol)
		log4cplus::Logger::shutdown();
}
#endif
