#ifndef GWAPP_UTIL_H
#define GWAPP_UTIL_H 1

#include <Compiler.h>
#include <gwApp/Export.h>

#include <gwBase/SubsystemMgr.h>
#include <gwBase/CommandMgr.h>

class GWPropertyNode;

namespace gwApp
{
	class Application;

	//template<typename SYSTEM_TYPE>
	//static SYSTEM_TYPE* getSubsystem(gwApp::Application* app, const std::string& key) const
	//{
	//	if (!app) return 0L;
	//	gwBase::SubsystemMgr* sysMgr = app->getSystemMgr();
	//	if (!sysMgr) return 0L;
	//	return sysMgr->get_subsystem<SYSTEM_TYPE>(key);
	//}



	class GW_APP_EXPORT Util
	{
	public:
		static void addSubsystem(gwApp::Application* app, gwBase::Subsystem* sys, const gwBase::SubsystemMgr::GroupType & groupType, double time_sec = 0);
		static void executeCommand(gwApp::Application* app, const std::string & cmdKey, const GWPropertyNode& arg, bool isExecuteAsOperation);
	};


}



#endif // Util_h__
