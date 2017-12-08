#ifndef GWEARTH_PATHANIMATEDMODEL_H
#define GWEARTH_PATHANIMATEDMODEL_H 1

#include <compiler.h>
#include <gwEarth/export.h>
#include <gwEarth/planetAnimationPath.h>
#include <osgEarthUtil/EarthManipulator>
#include <osg/Geode>
#include <osg/MatrixTransform>
#include <osg/Geometry>
#include <osg/LineWidth>
#include <OpenThreads/Mutex>

using namespace osgEarth::Util;

namespace gwEarth
{
	class GWEARTH_EXPORT PathAnimatedModel : public osg::MatrixTransform
	{
	public:
		enum DirtyBit
		{
			NOT_DIRTY = 0,
			COORDINATE_DIRTY = 1,
			COLOR_DIRTY = 2,
			LABEL_DIRTY = 4,
			ICON_DIRTY = 8,
			MATRIX_DIRTY = 16,
			GENERIC_DIRTY = 32,
			ALL_DIRTY = (COORDINATE_DIRTY | COLOR_DIRTY | ICON_DIRTY | LABEL_DIRTY | MATRIX_DIRTY | GENERIC_DIRTY)
		};

		PathAnimatedModel(osgEarth::MapNode* mapnode, EarthManipulator* earthManip = NULL);
		virtual ~PathAnimatedModel();

		void setAnimationPathColor(const osg::Vec4f& value);
		void setAnimationPathLineThickness(double value);
		void setShowPathFlag(bool flag);
		void setEarthCameraManipulator(EarthManipulator* manip);//
		void setTimeScale(double scale);
		void setTimeOffset(double offset);
		virtual void traverse(osg::NodeVisitor& nv);
		virtual void stage();

		PlanetAnimationPath*       animationPath() { return animationPath_.get(); }
		const PlanetAnimationPath* animationPath()const { return animationPath_.get(); }

		void readAnimationPathFromXml(const std::string& xml_in);
		void saveAnimationPathToXml(const std::string& xml_out);

		void setLoopMode(osg::AnimationPath::LoopMode lm);
		void addControlPoint(double t, const osgEarth::GeoPoint& pos, const osg::Vec3d& orient, const osg::Vec3d& scale);
		void play();
		void pause();
		void stop();

		void setDirtyBits(DirtyBit bit)
		{
			OpenThreads::ScopedLock<OpenThreads::Mutex> lock(propertyMutex_);
			dirtyBit_ = bit;
		}
		void setDirtyBit(DirtyBit bit)
		{
			OpenThreads::ScopedLock<OpenThreads::Mutex> lock(propertyMutex_);
			dirtyBit_ |= (unsigned int)bit;
		}
		void clearDirtyBit(DirtyBit bit)
		{
			OpenThreads::ScopedLock<OpenThreads::Mutex> lock(propertyMutex_);
			dirtyBit_ &= (~((unsigned int)bit));
		}
		bool isDirtyBitSet(DirtyBit bit)const
		{
			OpenThreads::ScopedLock<OpenThreads::Mutex> lock(propertyMutex_);
			return (dirtyBit_ & bit);
		}
		void setStagedFlag(bool flag)
		{
			OpenThreads::ScopedLock<OpenThreads::Mutex> lock(propertyMutex_);
			stagedFlag_ = flag;
		}

	protected:
		void updateCoordinates();
		void updateColor();

		class GWEARTH_EXPORT PathCallback : public osg::AnimationPathCallback
		{
		public:
			PathCallback(EarthManipulator* earthManip = NULL);
			PathCallback(const PathCallback& apc, const osg::CopyOp& copyop);
			PathCallback(osg::AnimationPath* ap, double timeOffset = 0.0, double timeMultiplier = 1.0);
			PathCallback(osg::AnimationPath* ap, double timeOffset = 0.0, double timeMultiplier = 1.0, EarthManipulator* earthManip = NULL);

			virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);

		private:
			osg::ref_ptr<EarthManipulator> earthManip_;
		};

		OpenThreads::Mutex                     updateCoordinatesMutex_;
		OpenThreads::Mutex                     updateColorMutex_;
		bool                                   showPathFlag_;
		osg::Vec4f                             animationPathColor_;
		float								   animationPathLineThickness_;
		osg::ref_ptr<EarthManipulator>         earthManipulator_;
		osg::ref_ptr<PlanetAnimationPath>	   animationPath_;

		//»æÖÆÂ·¾¶;
		osg::ref_ptr<osg::MatrixTransform>     pathMatrixTransform_;
		osg::ref_ptr<osg::Vec4Array>           pathColor_;
		osg::ref_ptr<osg::Vec3Array>           pathVertices_;
		osg::ref_ptr<osg::LineWidth>           lineWidth_;
		osg::ref_ptr<osg::Geode>               pathGeode_;
		osg::ref_ptr<osg::Geometry>            pathGeometry_;

		osg::ref_ptr<PathCallback>             animationPathCallback_;

		unsigned int dirtyBit_;
		bool stagedFlag_;
		mutable OpenThreads::Mutex propertyMutex_;
	};
}

#endif // planetAnimatedPointModel_h__
