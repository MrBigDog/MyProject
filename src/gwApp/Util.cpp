#include <gwApp/Util.h>
#include <gwApp/Application.h>
#include <gwApp/Environment.h>

#include <gwEarth/Root.h>
#include <gwEarth/Render.h>
#include <gwEarth/TerrainSystem.h>
#include <gwEarth/ModelSystem.h>
#include <gwEarth/CameraManipulatorManager.h>

#include <gwBase/CacheMgr.h>
#include <gwBase/EventMgr.h>
#include <gwBase/CommandMgr.h>
#include <gwBase/SubsystemMgr.h>
#include <gwBase/PerformMonitor.h>
#include <gwBase/PropertyInterpolationMgr.h>

#include <osgEarth/MapNode>
#include <osgEarthUtil/Sky>

namespace gwApp
{
	void Util::addSubsystem(gwApp::Application* app, gwBase::Subsystem* sys, const gwBase::SubsystemMgr::GroupType & groupType, double time_sec)
	{
		if (!app) return;
		gwBase::SubsystemMgr* sysMgr = app->getSystemMgr();
		if (!sysMgr) return;

		sysMgr->add(sys->getName().c_str(), sys, groupType, time_sec);
	}


	void Util::executeCommand(gwApp::Application* app, const std::string & cmdKey, const GWPropertyNode& arg, bool isExecuteAsOperation)
	{
		if (!app) return;
		gwBase::CommandMgr* comdMgr = app->getCommandMgr();
		if (!comdMgr) return;

		comdMgr->execute(cmdKey, arg, isExecuteAsOperation);
	}



}
