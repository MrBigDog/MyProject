#ifndef GWTOOL_COMMONPICKTOOL_H
#define GWTOOL_COMMONPICKTOOL_H 1

#include <compiler.h>

#include <gwTool/export.h>
#include <osgEarth/GeoData>
#include <osgEarth/MapNode>
#include <osgEarth/ModelLayer>
#include <osgEarthUtil/Common>
#include <osgEarthUtil/LinearLineOfSight>
#include <osgEarthUtil/RadialLineOfSight>
#include <osgEarthUtil/PolyhedralLineOfSight>
#include <osgEarthUtil/TerrainProfile>
#include <osgEarthAnnotation/FeatureNode>
#include <osgGA/GUIEventHandler>

namespace gwTool
{
	class GWTOOL_EXPORT CommonPickTool : public osgGA::GUIEventHandler
	{
	public:
		struct Callback : public osg::Referenced
		{
		public:
			Callback(osgEarth::MapNode* mapNode, CommonPickTool* cpt) :mapNode_(mapNode), cpt_(cpt) {}
			virtual ~Callback() { removeTemptModelLayer(); }

			virtual void mousePick(const osg::Vec3d& point) {}
			virtual void mouseMove(const osg::Vec3d& point) {}
			virtual void finish() {}
			virtual void reset() {}
			virtual void removeTemptModelLayer()
			{
				mapNode_->getMap()->removeModelLayer(temptLayer_);
			}
			virtual void addTemptModelLayer(const std::string& name)
			{
				temptLayer_ = new osgEarth::ModelLayer(name, new osg::Group);
				mapNode_->getMap()->addModelLayer(temptLayer_);
			}

		protected:
			CommonPickTool* cpt_;//避免智能指针循环调用;
			osgEarth::MapNode* mapNode_;
			osg::ref_ptr<osgEarth::ModelLayer> temptLayer_;
		};

	public:
		CommonPickTool(osgEarth::MapNode* mapNode);// {}
		~CommonPickTool();

		void setDefaultCallback(Callback* cb);
		virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

		void setOnlyPickTerrain(bool onlypickterrain) { isOnlyPickTerrain_ = onlypickterrain; }
		void setActiveMouseMoving(bool activemousemoving) { isActiveMouseMoving_ = activemousemoving; }

		void setDigTerrainFuction();
		void setDistanceMeasureFuction();
		void setHightMeasureFuction();
		void setAreaMeasureFuction();
		void setLineSightAnalysisFunction();
		void setAreaSightAnalysisFunction();
		void setPolyhedralSightAnalysisFunction();
		void setTerrainProfileAnalysisFunction();
	private:
		bool getPickPos(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, bool onlyPickTerrain, osg::Vec3d& pos);

	private:
		osg::ref_ptr<osgEarth::MapNode> mapNode_;
		osg::NodePath mapNodePath_;
		bool isCanPick_;
		bool isOnlyPickTerrain_;
		bool isActiveMouseMoving_;

		osg::ref_ptr<Callback> defaultCallback_;
	};

	//挖洞分析;// 这个功能可以使用地形压平功能实现 [11/10/2017 BigDog]
	class GWTOOL_EXPORT digToolCallback : public CommonPickTool::Callback
	{
	public:
		digToolCallback(CommonPickTool*cpt, osgEarth::MapNode* mapNode);
		~digToolCallback();

		virtual void mousePick(const osg::Vec3d &point);
		virtual void finish();
		virtual void reset();

	protected:
		osg::Node* createFeatureNode(osg::Vec3dArray* va, const osgEarth::Color& color);
		osg::ref_ptr<osg::Vec3dArray> va_;
	};

	//两点通视分析;
	class GWTOOL_EXPORT lineSightAnalysisCallback : public CommonPickTool::Callback
	{
	public:
		lineSightAnalysisCallback(CommonPickTool*cpt, osgEarth::MapNode* mapNode);

		virtual void mousePick(const osg::Vec3d& point);
		virtual void mouseMove(const osg::Vec3d& point);
		virtual void finish();
		virtual void reset();
		virtual void addTemptModelLayer(const std::string& name);

	private:
		int pickIndex_;
		osg::ref_ptr<osgEarth::Util::LinearLineOfSightNode> los_;

	};

	//区域通视分析;
	class GWTOOL_EXPORT areaSightAnalysisCallback : public CommonPickTool::Callback
	{
	public:
		areaSightAnalysisCallback(CommonPickTool*cpt, osgEarth::MapNode* mapNode);


		virtual void mousePick(const osg::Vec3d &point);
		virtual void mouseMove(const osg::Vec3d& point);
		virtual void finish() {}
		virtual void reset() {}
		virtual void addTemptModelLayer(const std::string& name);

	private:
		int pickIndex_;
		osg::ref_ptr<osgEarth::Util::RadialLineOfSightNode> rlos_;
	};

	////投影通视分析;
	//class polyhedralSightAnalysisCallback: public CommonPickTool::Callback
	//{
	//public:
	//	polyhedralSightAnalysisCallback(CommonPickTool* cpt, osgEarth::MapNode* mapnode);

	//	virtual void mousePick(const osg::Vec3d &point );
	//	virtual void finish(){}
	//	virtual void reset(){}
	//	virtual void addTemptModelLayer( const std::string& name );

	//private:
	//	int pickIndex_;
	//	osg::ref_ptr<osgEarth::Util::PolyhedralLineOfSightNode> plos_;
	//};


	//地形刨面分析;
	class GWTOOL_EXPORT terrainProfileAnalysisCallback : public CommonPickTool::Callback
	{
	public:
		terrainProfileAnalysisCallback(CommonPickTool*cpt, osgEarth::MapNode* mapNode);

		virtual void mousePick(const osg::Vec3d &point);
		virtual void finish() {}
		virtual void reset() {}
		virtual void addTemptModelLayer(const std::string& name);

	private:
		void compute(const osgEarth::GeoPoint& s, const osgEarth::GeoPoint& e);
	private:
		osg::ref_ptr<osgEarth::Util::TerrainProfileCalculator> tpc_;
		osgEarth::GeoPoint startPos_;
		osgEarth::GeoPoint endPos_;
		osg::ref_ptr<osg::Group> profileRoot_;
		osg::ref_ptr<osg::Group> root_;
		osg::ref_ptr<osgEarth::Annotation::FeatureNode>	featureNode_;
		int pickIndex_;
	};

	//距离测量;
	class GWTOOL_EXPORT distanceMeasureToolCallback : public CommonPickTool::Callback
	{
	public:
		distanceMeasureToolCallback(CommonPickTool*cpt, osgEarth::MapNode* mapNode)
			:CommonPickTool::Callback(mapNode, cpt)
		{ }

		virtual void mousePick(const osg::Vec3d &point) {}
		virtual void finish() {}
		virtual void reset() {}
	};

	//高度测量;
	class GWTOOL_EXPORT heightMeasureToolCallback : public CommonPickTool::Callback
	{
	public:
		heightMeasureToolCallback(CommonPickTool*cpt, osgEarth::MapNode* mapNode)
			:CommonPickTool::Callback(mapNode, cpt)
		{

		}
		virtual void mousePick(const osg::Vec3d &point) {}
		virtual void finish() {}
		virtual void reset() {}
	};

	//面积测量;
	class GWTOOL_EXPORT areaMeasureToolCallback : public CommonPickTool::Callback
	{
	public:
		areaMeasureToolCallback(CommonPickTool*cpt, osgEarth::MapNode* mapNode)
			:CommonPickTool::Callback(mapNode, cpt)
		{

		}
		virtual void mousePick(const osg::Vec3d &point) {}
		virtual void finish() {}
		virtual void reset() {}
	};
}

#endif // CommonPickTool_h__
