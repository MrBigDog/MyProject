#include <gwTool/NavigationTool.h>
#include <osgEarth/GeoData>

namespace gwTool
{
	NavigationTool::NavigationTool(osgEarth::MapNode* mapnode, EarthManipulator* earthmanipulator)
		: osgGA::AnimationPathManipulator()
		, mapNode_(mapnode)
		, earthManipulator_(earthmanipulator)
	{
		_printOutTimingInfo = false;
		planetAnimationPath_ = new PlanetAnimationPath;
		planetAnimationPath_->setMapNode(mapnode);
		planetAnimationPath_->setLoopMode(osg::AnimationPath::LOOP);
		this->setAnimationPath(planetAnimationPath_);
		speed_ = 1.0;
		isPlaying_ = false;
	}

	NavigationTool::NavigationTool(osgEarth::MapNode* mapnode, EarthManipulator* earthmanipulator, const std::string& xml_in)
		: osgGA::AnimationPathManipulator()
		, mapNode_(mapnode)
		, earthManipulator_(earthmanipulator)
	{
		_printOutTimingInfo = false;
		planetAnimationPath_ = new PlanetAnimationPath;
		planetAnimationPath_->setMapNode(mapnode);
		planetAnimationPath_->setLoopMode(osg::AnimationPath::LOOP);
		planetAnimationPath_->openAnimationPathByXml(xml_in);

		this->setAnimationPath(planetAnimationPath_);
		speed_ = 1.0;
		isPlaying_ = false;
	}

	void NavigationTool::addControlPoint(const osgEarth::Viewpoint& controlpoint, double time)
	{
		osgEarth::GeoPoint geop = controlpoint.focalPoint().value();
		double heading = controlpoint.getHeading();
		double pitch = controlpoint.getPitch();

		planetAnimationPath_->geospatialPath()->timeTupleMap().insert(
			std::make_pair(time,
				PlanetAnimationPath::Tuple(geop,
					osg::Vec3d(heading, pitch, 0.0),
					osg::Vec3d(1.0, 1.0, 1.0))));
	}

	void NavigationTool::addControlPointToConstantSpeedPathAnimation(const osgEarth::Viewpoint& controlpoint)
	{
		double heading = controlpoint.heading()->as(osgEarth::Units::DEGREES);
		double pitch = controlpoint.pitch()->as(osgEarth::Units::DEGREES);

		osg::Vec3d e, c, u;
		earthManipulator_->getInverseMatrix().getLookAt(e, c, u);

		osgEarth::GeoPoint gp;
		gp.fromWorld(mapNode_->getMapSRS(), e);

		double time = 0;

		PlanetAnimationPath::TimeTupleMap ttm = planetAnimationPath_->geospatialPath()->timeTupleMap();
		if (!ttm.empty())
		{
			PlanetAnimationPath::TimeTupleMap::iterator it = ttm.end();
			it--;

			osgEarth::GeoPoint currentGeop = it->second.position();//osgEarth::GeoPoint(mapNode_->getMapSRS(), it->second.position());

			double dis = gp.distanceTo(currentGeop);

			double interTime = dis / speed_;
			double preTime = it->first;
			time = preTime + interTime;
		}

		planetAnimationPath_->geospatialPath()->timeTupleMap().insert(
			std::make_pair(time,
				PlanetAnimationPath::Tuple(gp,
					osg::Vec3d(heading, pitch, 0.0),
					osg::Vec3d(1.0, 1.0, 1.0))));
	}

	void NavigationTool::openAnimationPathByXml(const std::string& xml_in)
	{
		planetAnimationPath_->openAnimationPathByXml(xml_in);
	}

	void NavigationTool::saveAnimationPathToXml(const std::string& xml_out)
	{
		planetAnimationPath_->saveAnimationPathToXml(xml_out);
	}

	bool NavigationTool::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us)
	{
		if (!earthManipulator_ || earthManipulator_ == NULL)return false;

		if (!valid()) return false;

		if (isPlaying_)
		{
			switch (ea.getEventType())
			{
			case osgGA::GUIEventAdapter::FRAME:
				if (_isPaused)
				{
					handleFrame(_pauseTime);
					earthManipulator_->setByMatrix(_matrix);
				}
				else
				{
					handleFrame(ea.getTime());
					earthManipulator_->setByMatrix(_matrix);
				}
				return false;
			case osgGA::GUIEventAdapter::KEYDOWN:
				if (ea.getKey() == ' ')
				{
					_isPaused = false;

					home(ea, us);
					us.requestRedraw();
					us.requestContinuousUpdate(false);

					return true;
				}
				else if (ea.getKey() == ')')
				{
					double time = _isPaused ? _pauseTime : ea.getTime();
					double animationTime = (time + _timeOffset)*_timeScale;

					_timeScale *= 1.1;
					_timeOffset = animationTime / _timeScale - time;

					return true;
				}
				else if (ea.getKey() == '(')
				{
					double time = _isPaused ? _pauseTime : ea.getTime();
					double animationTime = (time + _timeOffset)*_timeScale;

					_timeScale /= 1.1;
					_timeOffset = animationTime / _timeScale - time;

					return true;
				}
				else if (ea.getKey() == 'p')
				{
					if (_isPaused)
					{
						_isPaused = false;
						_timeOffset -= ea.getTime() - _pauseTime;
					}
					else
					{
						_isPaused = true;
						_pauseTime = ea.getTime();
					}
					us.requestRedraw();
					us.requestContinuousUpdate(false);
					return true;
				}
				else if (ea.getKey() == 'q')
				{
					isPlaying_ = false;
					home(ea, us);
					us.requestRedraw();
					us.requestContinuousUpdate(false);

					return true;
				}

				break;
			default:
				break;
			}
		}
		else
		{
			switch (ea.getEventType())
			{
			case osgGA::GUIEventAdapter::KEYDOWN:
				if (ea.getKey() == 'a')
				{
					earthManipulator_->getMatrix();
					addControlPointToConstantSpeedPathAnimation(earthManipulator_->getViewpoint());
					return true;
				}
				else if (ea.getKey() == 'o')
				{
					saveAnimationPathToXml("navigation_path.xml");
					return true;
				}
				break;
			default:
				break;
			}
		}

		return false;
	}
}