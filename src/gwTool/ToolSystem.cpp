#include <gwTool/ToolSystem.h>

#include <osgEarth/Registry>
#include <osgEarth/StringUtils>

namespace gwTool
{
	//static bool setCommonPickToolFuctionCommand(const MDPropertyNode* arg)
	//{
	//	if (arg == NULL) return false;

	//	ToolSystem* toolsys = (ToolSystem*)EarthUtil::instance()->getSubsystem("tool_system");
	//	if (!toolsys) return false;

	//	CommonPickTool* cpt = toolsys->getCommonPickTool();
	//	if (!cpt) return false;

	//	std::string fuctionType = osgDB::convertToLowerCase(arg->getStringValue("function"));
	//	if (fuctionType == "dig")
	//	{
	//		cpt->setDigTerrainFuction();
	//	}
	//	else if (fuctionType == "distance_measure")
	//	{
	//		cpt->setDistanceMeasureFuction();
	//	}
	//	else if (fuctionType == "height_measure")
	//	{
	//		cpt->setHightMeasureFuction();
	//	}
	//	else if (fuctionType == "area_measure")
	//	{
	//		cpt->setAreaMeasureFuction();
	//	}
	//	else if (fuctionType == "line_sight_analysis")
	//	{
	//		cpt->setLineSightAnalysisFunction();
	//	}
	//	else if (fuctionType == "area_sight_analysis")
	//	{
	//		cpt->setAreaSightAnalysisFunction();
	//	}
	//	else if (fuctionType == "polyhedral_sight_analysis")
	//	{
	//		cpt->setPolyhedralSightAnalysisFunction();
	//	}
	//	else if (fuctionType == "terrain_profile_analysis")
	//	{
	//		cpt->setTerrainProfileAnalysisFunction();
	//	}
	//	else return false;

	//	return true;
	//}

	//static bool setQueryToolCommand(const MDPropertyNode* arg)
	//{
	//	if (arg == NULL) return false;

	//	ToolSystem* toolsys = dynamic_cast<ToolSystem*>(EarthUtil::instance()->getSubsystem("tool_system"));
	//	if (!toolsys) return false;

	//	// 		mdBase::SubsystemMgr* sysMgr = Application::instance()->getSubsystemMgr();
	//	// 		if (!sysMgr) return false;

	//	// 		SceneSystem* scene = dynamic_cast<SceneSystem*>(Application::instance()->getSceneSystem());
	//	// 		if (!scene) return false;

	//	osg::Node* queryNode = NULL;
	//	std::string queryType = arg->getStringValue("query_type");
	//	if (queryType == "common_model")
	//	{
	//		SceneSystem* scene = dynamic_cast<SceneSystem*>(EarthUtil::instance()->getSubsystem("scene_system"));
	//		if (!scene) return false;

	//		CommonModelSystem* cmsys = dynamic_cast<CommonModelSystem*>(scene->get_subsystem("common_model_system"));
	//		if (!cmsys) return false;

	//		queryNode = cmsys->getRoot();
	//	}
	//	else if (queryType == "pipeline")
	//	{

	//	}

	//	if (queryNode == NULL) return false;

	//	toolsys->getQueryTool()->removeAllChildren();
	//	toolsys->getQueryTool()->addChild(queryNode);

	//	return true;
	//}

	//static bool setNavigationCommond(const MDPropertyNode* arg)
	//{
	//	if (arg == NULL) return false;

	//	ToolSystem* toolsys = dynamic_cast<ToolSystem*>(EarthUtil::instance()->getSubsystem("tool_system"));
	//	if (!toolsys) return false;

	//	NavigationTool* navigationtool = toolsys->getNavigationTool();
	//	if (!navigationtool) return false;

	//	std::string animationpath = arg->getStringValue("animation_path");
	//	navigationtool->openAnimationPathByXml(animationpath);
	//	navigationtool->play();

	//	return true;
	//}

	//static bool setViewportCommond(const MDPropertyNode* arg)
	//{
	//	if (arg == NULL) return false;

	//	ToolSystem* toolsys = dynamic_cast<ToolSystem*>(EarthUtil::instance()->getSubsystem("tool_system"));
	//	if (!toolsys) return false;

	//	EarthManipulator* em = toolsys->getEarthManipulator();
	//	if (!em) return false;

	//	std::string vpName = arg->getStringValue("name");
	//	double lon = arg->getDoubleValue("lon");
	//	double lat = arg->getDoubleValue("lat");
	//	double alt = arg->getDoubleValue("alt");
	//	double heading = arg->getDoubleValue("heading");
	//	double pitch = arg->getDoubleValue("pitch");
	//	double range = arg->getDoubleValue("range");
	//	double time = arg->getDoubleValue("time");

	//	osgEarth::Viewpoint vp(vpName.c_str(), lon, lat, alt, heading, pitch, range);
	//	em->setViewpoint(vp, time);

	//	return true;
	//}



	//...........................................................................................................
	ToolSystem::ToolSystem(osgEarth::MapNode* mapNode)
		: mapNode_(mapNode)
	{ }

	ToolSystem::~ToolSystem()
	{
	}

	ToolSystem * ToolSystem::getToolSystem(gwApp::Application * app, const std::string & name)
	{
		if (!app) return 0L;
		gwBase::SubsystemMgr* sysMgr = app->getSystemMgr();
		if (!sysMgr) return 0L;
		return dynamic_cast<ToolSystem*>(sysMgr->get_subsystem(name));
	}

	void ToolSystem::init()
	{
		//commonPickTool_ = new CommonPickTool(mapNode_);
		//commonPickTool_->setDefaultCallback(NULL);
		//EarthUtil::instance()->getRender()->getMainView()->addEventHandler(commonPickTool_);

		////添加高亮功能;
		//SceneSystem* scene = dynamic_cast<SceneSystem*>(EarthUtil::instance()->getSubsystem("scene_system"));
		//if (scene)
		//{
		//	osg::StateSet* sceneStateset = scene->getSceneMapNode()->getOrCreateStateSet();

		//	//添加点选查询功能;
		//	queryTool_ = new QueryTool;
		//	queryTool_->setDefaultCallback(new QueryTool::QueryToolCallback);
		//	queryTool_->installHighlighter(sceneStateset, osgEarth::Registry::objectIndex()->getObjectIDAttribLocation());
		//	EarthUtil::instance()->getRender()->getMainView()->addEventHandler(queryTool_);
		//}
	}

	void ToolSystem::shutdown()
	{
	}

	void ToolSystem::update(double dt)
	{ }


	CommonPickTool* ToolSystem::getCommonPickTool()
	{
		return commonPickTool_;
	}

	QueryTool* ToolSystem::getQueryTool()
	{
		return queryTool_;
	}

	NavigationTool* ToolSystem::getNavigationTool()
	{
		return navigationTool_;
	}
}