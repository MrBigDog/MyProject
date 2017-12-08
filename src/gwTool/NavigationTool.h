#ifndef GWTOOL_NAVIGATIONTOOL_H
#define GWTOOL_NAVIGATIONTOOL_H 1

#include <compiler.h>

#include <gwTool/export.h>
#include <gwEarth/planetAnimationPath.h>

#include <osgEarth/MapNode>
#include <osgEarthUtil/EarthManipulator>

#include <osgGA/AnimationPathManipulator>

using namespace gwEarth;
using namespace osgEarth::Util;

namespace gwTool
{
	//功能类似于animationpathcallback，只是这里控制的是漫游器的漫游路径;
	class GWTOOL_EXPORT NavigationTool : public osgGA::AnimationPathManipulator
	{
	public:
		NavigationTool(osgEarth::MapNode* mapnode, EarthManipulator* earthmanipulator);
		NavigationTool(osgEarth::MapNode* mapnode, EarthManipulator* earthmanipulator, const std::string& xml_in);

		virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

		void play() { isPlaying_ = true; }
		void setPause(bool pause) { pause_ = pause; }
		bool getPause() const { return pause_; }

		void setSpeed(double speed) { speed_ = speed; }
		double getSpeed() { return speed_; }

		void addControlPoint(const osgEarth::Viewpoint& controlpoint, double time);
		void addControlPointToConstantSpeedPathAnimation(const osgEarth::Viewpoint& controlpoint);

		void openAnimationPathByXml(const std::string& xml_in);
		void saveAnimationPathToXml(const std::string& xml_out);

	private:
		EarthManipulator* earthManipulator_;
		osgEarth::MapNode* mapNode_;

		osg::ref_ptr<PlanetAnimationPath> planetAnimationPath_;
		bool       pause_;
		double     speed_;

		bool       isPlaying_;
	};
}

#endif // NavigationTool_h__
