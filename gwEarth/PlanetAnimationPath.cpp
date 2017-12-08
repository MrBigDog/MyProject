#include <constants.h>
#include <compiler.h>

#include <gwEarth/planetAnimationPath.h>

#include <osgEarth/GeoData>
#include <osgEarth/GeoCommon>
#include <osgEarth/IOTypes>

#include <osg/io_utils>
#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>

#include <fstream>
#include <sstream>

#define LC "[Planet Animation Path] "

#define ANIMATION_PATH "animationpath"
#define GEOSPATIAL_PATH "geospatialpath"
#define CONTROLPOINT_SET "controlpointset"
#define CONTROLPOINT "controlpoint"
#define TIME "time"
#define POSITION_LLH "position-llh"
#define ORIENTATION "orient"
#define SCALE "scale"

namespace gwEarth
{
	osg::Matrixd createRotationXMatrix(double angle, gwCoordSysOrientMode orientationMode)
	{
		osg::Matrixd m;
		double Cosine = cos(angle*RAD_PER_DEG);
		double Sine = sin(angle*RAD_PER_DEG);
		if (orientationMode == GW_RIGHT_HANDED)
		{
			m(0, 0) = 1.0; m(0, 1) = 0.0; m(0, 2) = 0.0; m(0, 3) = 0.0;
			m(1, 0) = 0.0; m(1, 1) = Cosine; m(1, 2) = Sine; m(1, 3) = 0.0;
			m(2, 0) = 0.0; m(2, 1) = -Sine; m(2, 2) = Cosine; m(2, 3) = 0.0;
			m(3, 0) = 0.0; m(3, 1) = 0.0; m(3, 2) = 0.0; m(3, 3) = 1.0;
		}
		else
		{
			m(0, 0) = 1.0; m(0, 1) = 0.0; m(0, 2) = 0.0; m(0, 3) = 0.0;
			m(1, 0) = 0.0; m(1, 1) = Cosine; m(1, 2) = -Sine; m(1, 3) = 0.0;
			m(2, 0) = 0.0; m(2, 1) = Sine; m(2, 2) = Cosine; m(2, 3) = 0.0;
			m(3, 0) = 0.0; m(3, 1) = 0.0; m(3, 2) = 0.0; m(3, 3) = 1.0;
		}
		return m;
	}

	osg::Matrixd createRotationYMatrix(double angle, gwCoordSysOrientMode orientationMode)
	{
		osg::Matrixd m;
		double Cosine = cos(angle*RAD_PER_DEG);
		double Sine = sin(angle*RAD_PER_DEG);
		if (orientationMode == GW_RIGHT_HANDED)
		{
			m(0, 0) = Cosine; m(0, 1) = 0.0; m(0, 2) = -Sine; m(0, 3) = 0.0;
			m(1, 0) = 0.0; m(1, 1) = 1.0; m(1, 2) = 0.0; m(1, 3) = 0.0;
			m(2, 0) = Sine; m(2, 1) = 0.0; m(2, 2) = Cosine; m(2, 3) = 0.0;
			m(3, 0) = 0.0; m(3, 1) = 0.0; m(3, 2) = 0.0; m(3, 3) = 1.0;
		}
		else
		{
			m(0, 0) = Cosine; m(0, 1) = 0.0; m(0, 2) = Sine; m(0, 3) = 0.0;
			m(1, 0) = 0.0; m(1, 1) = 1.0; m(1, 2) = 0.0; m(1, 3) = 0.0;
			m(2, 0) = -Sine; m(2, 1) = 0.0; m(2, 2) = Cosine; m(2, 3) = 0.0;
			m(3, 0) = 0.0; m(3, 1) = 0.0; m(3, 2) = 0.0; m(3, 3) = 1.0;
		}
		return m;
	}


	osg::Matrixd createRotationZMatrix(double angle, gwCoordSysOrientMode orientationMode)
	{
		osg::Matrixd m;
		double Cosine = cos(angle*RAD_PER_DEG);
		double Sine = sin(angle*RAD_PER_DEG);
		if (orientationMode == GW_RIGHT_HANDED)
		{
			m(0, 0) = Cosine; m(0, 1) = Sine; m(0, 2) = 0.0; m(0, 3) = 0.0;
			m(1, 0) = -Sine; m(1, 1) = Cosine; m(1, 2) = 0.0; m(1, 3) = 0.0;
			m(2, 0) = 0.0; m(2, 1) = 0.0; m(2, 2) = 1.0; m(2, 3) = 0.0;
			m(3, 0) = 0.0; m(3, 1) = 0.0; m(3, 2) = 0.0; m(3, 3) = 1.0;
		}
		else
		{
			m(0, 0) = Cosine; m(0, 1) = -Sine; m(0, 2) = 0.0; m(0, 3) = 0.0;
			m(1, 0) = Sine; m(1, 1) = Cosine; m(1, 2) = 0.0; m(1, 3) = 0.0;
			m(2, 0) = 0.0; m(2, 1) = 0.0; m(2, 2) = 1.0; m(2, 3) = 0.0;
			m(3, 0) = 0.0; m(3, 1) = 0.0; m(3, 2) = 0.0; m(3, 3) = 1.0;
		}
		return m;
	}

	osg::Matrixd createRotationMatrix(double angleX, double angleY, double angleZ, gwCoordSysOrientMode orientationMode)
	{
		return (createRotationZMatrix(angleZ, orientationMode)*
			createRotationYMatrix(angleY, orientationMode)*
			createRotationXMatrix(angleX, orientationMode));
	}

	osg::Matrixd createScaleMatrix(double x, double y, double z)
	{
		osg::Matrixd m;
		m(0, 0) = x; m(0, 1) = 0.0; m(0, 2) = 0.0; m(0, 3) = 0.0;
		m(1, 0) = 0.0; m(1, 1) = y; m(1, 2) = 0.0; m(1, 3) = 0.0;
		m(2, 0) = 0.0; m(2, 1) = 0.0; m(2, 2) = z; m(2, 3) = 0.0;
		m(3, 0) = 0.0; m(3, 1) = 0.0; m(3, 2) = 0.0; m(3, 3) = 1.0;
		return m;
	}

	////////////////////////////////////////////////////////////////////////////////////////
	PlanetAnimationPath::PlanetAnimationPath()
	{
		geoSpatialPath_ = new GeospatialPath();
	}

	bool PlanetAnimationPath::openAnimationPathByXml(const std::string& xmlfile)
	{
		osgEarth::ReadResult r = osgEarth::URI(xmlfile).readString();
		if (r.failed())
		{
			OE_DEBUG << LC << "Failed to read path animation file from " << xmlfile << std::endl;
			return false;
		}

		osgEarth::Config conf;
		std::stringstream buf(r.getString());
		conf.fromXML(buf);

		const osgEarth::Config* animationpathconfig = conf.find(ANIMATION_PATH);
		if (!animationpathconfig)
		{
			OE_DEBUG << LC << "Failed to read AnimationPath from " << xmlfile << std::endl;
			return false;
		}

		const osgEarth::Config* pathConfig = animationpathconfig->find(GEOSPATIAL_PATH);
		if (!pathConfig)
		{
			OE_DEBUG << LC << "Failed to read GeospatialPath from " << xmlfile << std::endl;
			return false;
		}

		const osgEarth::Config* coordnateConfig = pathConfig->find(CONTROLPOINT_SET);
		if (!coordnateConfig)
		{
			return false;
		}

		const osgEarth::ConfigSet& coordinateConfigSet = coordnateConfig->children(CONTROLPOINT);
		osgEarth::ConfigSet::const_iterator it;
		for (it = coordinateConfigSet.begin(); it != coordinateConfigSet.end(); ++it)
		{
			const osgEarth::Config& conf = *it;
			double time = conf.value<double>(TIME, 0.0);
			osg::Vec3d pos = conf.value<osg::Vec3d>(POSITION_LLH, osg::Vec3d());
			osg::Vec3d orient = conf.value<osg::Vec3d>(ORIENTATION, osg::Vec3d());
			osg::Vec3d scale = conf.value<osg::Vec3d>(SCALE, osg::Vec3d());

			osgEarth::GeoPoint geop(mapNode_->getMapSRS(), pos);

			geoSpatialPath_->timeTupleMap().insert(std::make_pair(time, Tuple(geop, orient, scale)));
		}

		return true;
	}

	bool PlanetAnimationPath::saveAnimationPathToXml(const std::string& xml_out)
	{
		osg::ref_ptr<osgEarth::XmlDocument> doc = new osgEarth::XmlDocument();
		doc->setName(ANIMATION_PATH);

		osg::ref_ptr<osgEarth::XmlElement> geopath = new osgEarth::XmlElement(GEOSPATIAL_PATH);
		doc->getChildren().push_back(geopath);

		osg::ref_ptr<osgEarth::XmlElement> controlPointSet = new osgEarth::XmlElement(CONTROLPOINT_SET);
		geopath->getChildren().push_back(controlPointSet);

		TimeTupleMap::const_iterator it;
		for (it = geoSpatialPath_->timeTupleMap().begin(); it != geoSpatialPath_->timeTupleMap().end(); ++it)
		{
			osg::ref_ptr<osgEarth::XmlElement> controlPoint = new osgEarth::XmlElement(CONTROLPOINT);
			controlPoint->getAttrs()[TIME] = osgEarth::toString(it->first);
			controlPoint->getAttrs()[POSITION_LLH] = osgEarth::toString(it->second.position().vec3d());
			controlPoint->getAttrs()[ORIENTATION] = osgEarth::toString(it->second.orientation());
			controlPoint->getAttrs()[SCALE] = osgEarth::toString(it->second.scale());
			controlPointSet->getChildren().push_back(controlPoint);
		}

		std::string path = osgDB::getFilePath(xml_out);
		if (!osgDB::fileExists(path) && !osgDB::makeDirectory(path))
		{
			OE_WARN << LC << "Couldn't create path " << std::endl;
		}
		std::ofstream out(xml_out.c_str());
		doc->store(out);

		return true;
	}

	bool PlanetAnimationPath::getInterpolatedControlPoint(double time, osg::AnimationPath::ControlPoint& controlPoint) const
	{
		if (!geoSpatialPath_.valid() || geoSpatialPath_->timeTupleMap().empty()) return false;

		time = adjustTime(time);
		TimeTupleMap::const_iterator second = geoSpatialPath_->timeTupleMap().lower_bound(time);
		if (second == geoSpatialPath_->timeTupleMap().begin())
		{
			osg::Quat q;
			osg::Vec3d pos;
			osg::Matrixd m;

			lsrMatrix(m, second->second);

			m = osg::Matrixd::scale(second->second.scale())*m;
			q.set(m);
			pos.set(m(3, 0), m(3, 1), m(3, 2));
			controlPoint = ControlPoint(pos, q);
		}
		else if (second != geoSpatialPath_->timeTupleMap().end())
		{
			TimeTupleMap::const_iterator first = second;
			--first;

			double delta_time = second->first - first->first;
			if (delta_time == 0.0)
			{
				osg::Quat q;
				osg::Vec3d pos;
				osg::Matrixd m;

				lsrMatrix(m, first->second);
				q.set(m);
				pos.set(m(3, 0), m(3, 1), m(3, 2));
				controlPoint = ControlPoint(pos, q);
			}
			else
			{
				ControlPoint one, two;
				osg::Quat q;
				osg::Vec3d pos;
				osg::Matrixd m;

				lsrMatrix(m, first->second);
				q.set(m);
				pos.set(m(3, 0), m(3, 1), m(3, 2));
				one = ControlPoint(pos, q);

				lsrMatrix(m, second->second);
				q.set(m);
				pos.set(m(3, 0), m(3, 1), m(3, 2));
				two = ControlPoint(pos, q);
				controlPoint.interpolate((time - first->first) / delta_time, one, two);
			}
		}
		else // (second==_timeControlPointMap.end())
		{
			osg::Quat q;
			osg::Vec3d pos;
			osg::Matrixd m;

			lsrMatrix(m, geoSpatialPath_->timeTupleMap().rbegin()->second);
			q.set(m);
			pos.set(m(3, 0), m(3, 1), m(3, 2));
			controlPoint = ControlPoint(pos, q);
		}
		return true;
	}

	double PlanetAnimationPath::adjustTime(double t)const
	{
		double time = t;
		switch (_loopMode)
		{
		case(SWING):
		{
			double modulated_time = (time - geoSpatialPath_->firstTime()) / (geoSpatialPath_->period()*2.0);
			double fraction_part = modulated_time - floor(modulated_time);
			if (fraction_part > 0.5) fraction_part = 1.0 - fraction_part;

			time = geoSpatialPath_->firstTime() + (fraction_part*2.0) * geoSpatialPath_->period();
			break;
		}
		case(LOOP):
		{
			double modulated_time = (time - geoSpatialPath_->firstTime()) / geoSpatialPath_->period();
			double fraction_part = modulated_time - floor(modulated_time);
			time = geoSpatialPath_->firstTime() + fraction_part * geoSpatialPath_->period();
			break;
		}
		case(NO_LOOPING):
			break;
		}

		return time;
	}

	void PlanetAnimationPath::lsrMatrix(osg::Matrixd& result, const Tuple& tuple)const
	{
		osg::Matrixd matrix;
		osgEarth::GeoPoint geop = tuple.position();
		geop.createLocalToWorld(matrix);

		//osg::Matrixd orien = 
		//	createRotationZMatrix(osg::DegreesToRadians(tuple.orientation()[0]), GW_RIGHT_HANDED)*
		//	createRotationXMatrix(osg::DegreesToRadians(tuple.orientation()[1]-90.0), GW_LEFT_HANDED)*
		//	createRotationYMatrix(osg::DegreesToRadians(tuple.orientation()[2]), GW_LEFT_HANDED);

		//osg::Matrixd tempM(orien(0,0), orien(1,0), orien(2,0), 0.0,
		//	orien(0,1), orien(1,1), orien(2,1), 0.0,
		//	orien(0,2), orien(1,2), orien(2,2), 0.0,
		//	0.0, 0.0, 0.0, 1.0);

		osg::Matrixd tempM = osg::Matrixd::rotate(
			osg::DegreesToRadians(tuple.orientation()[2]), osg::Y_AXIS
			, osg::DegreesToRadians(tuple.orientation()[1]) + osg::PI_2, osg::X_AXIS
			, osg::DegreesToRadians(tuple.orientation()[0]) + osg::PI_4, osg::Z_AXIS
		);



		result = osg::Matrixd::scale(tuple.scale())*tempM*matrix;
	}

	bool PlanetAnimationPath::generateWorldCoordinates(PointList& worldPoints)const
	{
		if (geoSpatialPath_.valid()) return false;

		TimeTupleMap::const_iterator iter = geoSpatialPath_->timeTupleMap().begin();
		TimeTupleMap::const_iterator endIter = geoSpatialPath_->timeTupleMap().end();

		while (iter != endIter)
		{
			osg::Vec3d worldPoint;
			osgEarth::GeoPoint geop = iter->second.position();
			geop.toWorld(worldPoint);

			worldPoints.push_back(worldPoint);
			++iter;
		}
		return true;
	}

	bool PlanetAnimationPath::generateModelCoordinates(PointList& modelPoints)const
	{
		TimeTupleMap::const_iterator iter = geoSpatialPath_->timeTupleMap().begin();
		TimeTupleMap::const_iterator endIter = geoSpatialPath_->timeTupleMap().end();
		while (iter != endIter)
		{
			modelPoints.push_back(iter->second.position().vec3d());
			++iter;
		}

		return true;
	}

	bool PlanetAnimationPath::moveToLocationLatLon(const osg::Vec2d& llh)
	{
		PointList worldPoints;
		bool result = false;
		if (generateWorldCoordinates(worldPoints) && (worldPoints.size()))
		{
			osg::Vec3d centerWorld;
			long idx = 0;
			long size = worldPoints.size();
			for (idx = 0; idx < size; ++idx)
			{
				centerWorld += worldPoints[idx];
			}
			centerWorld *= (1.0 / (double)size);

			osgEarth::GeoPoint centerWorldLlh;
			centerWorldLlh.fromWorld(mapNode_->getMapSRS(), centerWorld);

			osgEarth::GeoPoint centerDistinationLlh(mapNode_->getMapSRS(), llh[0], llh[1], centerWorldLlh.alt());

			osg::Matrixd lsrSourceMatrix;
			osg::Matrixd lsrDestinationMatrix;
			centerWorldLlh.createLocalToWorld(lsrSourceMatrix);
			centerDistinationLlh.createLocalToWorld(lsrDestinationMatrix);

			osg::Matrixd sourceInvert(osg::Matrixd::inverse(lsrSourceMatrix));

			for (idx = 0; idx < size; ++idx)
			{
				osg::Vec3d relative = worldPoints[idx] * sourceInvert;
				osg::Vec3d newPoint = relative*lsrDestinationMatrix;
				worldPoints[idx] = newPoint;
			}
			TimeTupleMap::iterator iter = geoSpatialPath_->timeTupleMap().begin();
			osg::Vec3d newModelPoint;
			for (idx = 0; idx < size; ++idx)
			{
				osgEarth::GeoPoint newModelPoint;
				newModelPoint.fromWorld(mapNode_->getMapSRS(), worldPoints[idx]);

				iter->second.setPosition(newModelPoint);
				++iter;
			}
			result = true;
		}
		return result;
	}
}