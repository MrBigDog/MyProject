#ifndef GWEARTH_PLANETANIMATIONPATH_H
#define GWEARTH_PLANETANIMATIONPATH_H 1

#include <Compiler.h>
#include <gwEarth/export.h>
#include <osgEarth/XmlUtils>
#include <osgEarth/MapNode>

#include <osg/Vec3d>
#include <osg/AnimationPath>

//控制点为经纬度形式的 路径动画;
namespace gwEarth
{
	class GWEARTH_EXPORT PlanetAnimationPath : public osg::AnimationPath
	{
	public:
		class GWEARTH_EXPORT Tuple
		{
		public:
			Tuple(const osgEarth::GeoPoint& pos, const osg::Vec3d& orient, const osg::Vec3d& s = osg::Vec3(1.0, 1.0, 1.0))
				: position_(pos)
				, orientation_(orient)
				, scale_(s)
			{ }

			const osgEarth::GeoPoint& position()const { return position_; }
			const osg::Vec3d& orientation()const { return orientation_; }
			const osg::Vec3d& scale()const { return scale_; }
			void setPosition(const osgEarth::GeoPoint& value) { position_ = value; }
			void setScale(const osg::Vec3d& value) { scale_ = value; }
			void setOrientation(const osg::Vec3d& value) { orientation_ = value; }

		private:
			/*osg::Vec3d*/osgEarth::GeoPoint position_;
			osg::Vec3d orientation_;
			osg::Vec3d scale_;
		};
		typedef std::map<double, Tuple> TimeTupleMap;

		class GWEARTH_EXPORT GeospatialPath : public osg::Referenced
		{
		public:
			GeospatialPath() {}
			bool empty()const { return timeTupleMap_.empty(); }
			TimeTupleMap& timeTupleMap() { return timeTupleMap_; }
			const TimeTupleMap& timeTupleMap()const { return timeTupleMap_; }
			double firstTime() const { if (!timeTupleMap_.empty()) return timeTupleMap_.begin()->first; else return 0.0; }
			double lastTime() const { if (!timeTupleMap_.empty()) return timeTupleMap_.rbegin()->first; else return 0.0; }
			double period() const { return lastTime() - firstTime(); }
		protected:
			TimeTupleMap timeTupleMap_;
		};

		typedef std::vector<osg::Vec3d> PointList;

	public:
		PlanetAnimationPath();
		void setMapNode(osgEarth::MapNode* mn) { mapNode_ = mn; }
		osgEarth::MapNode* getMapNode() { return mapNode_.get(); }
		const osgEarth::MapNode* getMapNode()const { return mapNode_.get(); }

		bool openAnimationPathByXml(const std::string& xml_in);
		bool saveAnimationPathToXml(const std::string& xml_out);

		GeospatialPath* geospatialPath() { return geoSpatialPath_.get(); };
		const GeospatialPath* geospatialPath()const { return geoSpatialPath_.get(); };
		void setGeospatialPath(GeospatialPath* path);
		virtual bool getInterpolatedControlPoint(double time, osg::AnimationPath::ControlPoint& controlPoint) const;

		bool generateWorldCoordinates(PointList& worldPoints)const;
		bool generateModelCoordinates(PointList& modelPoints)const;

		bool moveToLocationLatLon(const osg::Vec2d& llh);

	protected:
		virtual double adjustTime(double time)const;
		void lsrMatrix(osg::Matrixd& result, const Tuple& tuple)const;

		osg::ref_ptr<GeospatialPath> geoSpatialPath_;
		osg::ref_ptr<osgEarth::MapNode> mapNode_;
	};
}

#endif // planetAnimationPath_h__
