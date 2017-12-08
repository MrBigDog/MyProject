#ifndef GWTOOL_TOOLSYSTEM_H
#define GWTOOL_TOOLSYSTEM_H 1

#include <compiler.h>
#include <gwTool/Export.h>
#include <gwBase/SubsystemMgr.h>
#include <gwApp/Application.h>

#include <osgEarth/MapNode>
#include <osgEarth/Registry>
#include <vector>

#include <gwTool/QueryTool.h>
#include <gwTool/CommonPickTool.h>
#include <gwTool/NavigationTool.h>

#include <osgEarth/MapNode>

namespace gwTool
{
	class GWTOOL_EXPORT ToolSystem : public gwBase::Subsystem
	{
	public:
		ToolSystem(osgEarth::MapNode* mapNode);
		~ToolSystem();

		static ToolSystem* getToolSystem(gwApp::Application*app, const std::string& name);

		// base system method
		void init();
		void shutdown();
		void update(double dt);

		CommonPickTool* getCommonPickTool();
		QueryTool* getQueryTool();
		NavigationTool* getNavigationTool();
		EarthManipulator* getEarthManipulator();

		//private:
		//	void installHighlighter(osg::StateSet* stateSet, int attrLocation);

	private:
		//osg::ref_ptr<osgEarth::Map> map_;
		osg::ref_ptr<osgEarth::MapNode> mapNode_;
		osg::ref_ptr<CommonPickTool> commonPickTool_;
		osg::ref_ptr<QueryTool> queryTool_;
		osg::ref_ptr<NavigationTool> navigationTool_;

		//osg::ref_ptr<EarthManipulator> earthManipulator_;
	};
}


#endif // GWEARTH_TOOLSYSTEM_H
