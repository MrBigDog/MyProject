#pragma warning(disable: 4996)

#include "SolidFeatureGeomModelOptions.h"
#include <osgEarthFeaturesExt/SolidGeomFeatureNodeFactory.h>
#include <osgEarthFeaturesExt/SolidLineSymbol.h>
#include <osgEarthSymbologyExt/SolidJointSymbol.h>

#include <osgEarth/Map>
#include <osgEarth/Registry>
#include <osgEarth/ModelSource>
#include <osgEarth/GeoTransform>
#include <osgEarthFeatures/GeometryCompiler>

#include <osg/Notify>
#include <osg/MatrixTransform>
#include <osg/ComputeBoundsVisitor>
#include <osg/ShapeDrawable>
#include <osgDB/FileNameUtils>

using namespace osgEarth;
using namespace osgEarth::Features;
using namespace osgEarth::Symbology;
using namespace gwDrivers;

#define LC "[SolidFeatureGeomModelSource] "

namespace
{
	//osg::Node* makeCylinder()
	//{
	//	osg::ref_ptr<osg::Cylinder> cylinder = new osg::Cylinder();
	//	osg::ref_ptr<osg::ShapeDrawable> sd = new osg::ShapeDrawable(cylinder);

	//	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	//	geode->addDrawable(sd);
	//	return geode.release();
	//}

	//class SolidGeometryNodeFactory :public FeatureNodeFactory
	//{
	//public:
	//	SolidGeometryNodeFactory(const SolidFeatureGeomModelOptions& options) : _options(options)
	//	{
	//		_readoptions = new osgDB::Options;
	//		_wsg84Srs = osgEarth::SpatialReference::create("wgs84");
	//		std::string gauss3 = "+proj=tmerc +lat_0=0 +lon_0=117 +k=1 +x_0=500000 +y_0=0 +ellps=krass +units=m +no_defs";//4796
	//		_dataSrs = osgEarth::SpatialReference::create(gauss3);

	//		_unitNode = makeCylinder();
	//	}
	//	~SolidGeometryNodeFactory() {}

	//public:
	//	virtual bool createOrUpdateNode(FeatureCursor* cursor, const Style& style,
	//		const FilterContext& context, osg::ref_ptr<osg::Node>&  node)
	//	{
	//		FeatureList features;
	//		cursor->fill(features);

	//		osg::ref_ptr<osg::Group> result = new osg::Group;
	//		for (FeatureList::const_iterator f = features.begin(); f != features.end(); ++f)
	//		{
	//			Feature* input = f->get();

	//			osgEarth::Geometry* geom = input->getGeometry();
	//			if (geom->empty()) continue;

	//			osg::ref_ptr<osg::Vec3Array> va = new osg::Vec3Array;
	//			for (osgEarth::Geometry::const_iterator it = geom->begin(); it != geom->end(); ++it)
	//			{
	//				va->push_back(*it);
	//			}

	//			//osgEarth::Symbology::Style style = input->style().value();
	//			const gwUtil::SolidLineSymbol* lineSym = style.get<gwUtil::SolidLineSymbol>();
	//			const gwEarth::SolidJointSymbol* jointSym = style.get<gwEarth::SolidJointSymbol>();

	//			osg::ref_ptr<osg::Group> subGroup = new osg::Group;
	//			if (lineSym)
	//			{
	//				if (va->size() < 2) return 0L;
	//				for (unsigned int i = 0; i < va->size() - 1; ++i)
	//				{
	//					osg::Vec3d p0 = va->at(i);
	//					osg::Vec3d p1 = va->at(i + 1);
	//					osg::Vec3d pc = (p0 + p1)*0.5;
	//					pc[2] += 1000.0;

	//					osg::Vec3d dir = p1 - p0;
	//					double length = dir.length();
	//					dir.normalize();

	//					osg::Matrixd scale;
	//					scale.makeScale(1.0, 1.0, length);

	//					osg::Matrixd rot;
	//					rot.makeRotate(osg::Vec3d(0, 0, 1), dir);

	//					osgEarth::GeoPoint geoPos = osgEarth::GeoPoint(_wsg84Srs, pc);
	//					osg::Matrixd local2world;
	//					geoPos.createLocalToWorld(local2world);

	//					osg::ref_ptr<osg::Node> un = lineSym->getConfig().getNonSerializable<osg::Node>("SolidLineSymbol::Node");
	//					//osg::ref_ptr<osg::Node> un1 = lineSym->getUnidSolidLineModel();
	//					//osg::ref_ptr<osg::Node> un2 = lineSym->model().value();


	//					if (_unitNode)
	//					{
	//						osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
	//						mt->addChild(_unitNode);
	//						mt->setMatrix(scale*rot*local2world);
	//						subGroup->addChild(mt);
	//					}
	//				}
	//			}
	//			else if (jointSym)
	//			{

	//			}
	//			if (subGroup->getNumChildren() > 0)
	//			{
	//				result->addChild(subGroup);
	//				osgEarth::Registry::shaderGenerator().run(subGroup, "SolidGeom.ModelNode", Registry::stateSetCache());
	//			}
	//		}
	//		node = result;
	//		return node.valid();
	//	}

	//private:
	//	const SolidFeatureGeomModelOptions _options;
	//	osg::ref_ptr<osgDB::Options> _readoptions;
	//	osg::ref_ptr<osgEarth::SpatialReference> _dataSrs;
	//	osg::ref_ptr<osgEarth::SpatialReference> _wsg84Srs;
	//	osg::ref_ptr<osg::Node> _unitNode;
	//};

	//±ÜÃâ¿çÍßÆ¬Ä£ÐÍ±»¹ýÔç²Ã¼ô;
	//class PagedLodOption : public osgEarth::NodeOperation
	//{
	//public:
	//	PagedLodOption() {}
	//	virtual void operator()(osg::Node* node)
	//	{
	//		osg::PagedLOD* pl = osgEarth::findFirstParentOfType<osg::PagedLOD>(node);
	//		if (pl)
	//		{
	//			osg::BoundingSphere bs = pl->computeBound();
	//			bs.expandBy(node->getBound());

	//			pl->setRadius(bs.radius()*2.0);
	//			pl->setCenter(bs.center());
	//		}
	//	}
	//};

	class MySceneGraphCallback :public osgEarth::SceneGraphCallback
	{
	public:
		virtual void onPostMergeNode(osg::Node* node)
		{
			osg::PagedLOD* pl = osgEarth::findFirstParentOfType<osg::PagedLOD>(node);
			if (pl)
			{
				osg::BoundingSphere bs = pl->computeBound();
				bs.expandBy(node->getBound());

				pl->setRadius(bs.radius()*2.0);
				pl->setCenter(bs.center());
			}
		}
	};

	/** The model source implementation for solid feature geom model */
	class SolidFeatureGeomModelSource : public FeatureModelSource
	{
	public:
		SolidFeatureGeomModelSource(const ModelSourceOptions& options)
			: FeatureModelSource(options)
			, _options(options)
		{
			//addPostMergeOperation(new PagedLodOption);
			getSceneGraphCallbacks()->add(new MySceneGraphCallback);
		}

		const SolidFeatureGeomModelSource& getOptions() const
		{
			return _options;
		}

	public: // FeatureModelSource
		FeatureNodeFactory* createFeatureNodeFactory()
		{
			return new oeFeaturesExt::SolidGeomFeatureNodeFactory(_options);
		}

	private:
		const SolidFeatureGeomModelOptions _options;
	};
}

//------------------------------------------------------------------------
class SolidFeatureGeomModelSourceDriver : public ModelSourceDriver
{
public:
	SolidFeatureGeomModelSourceDriver()
	{
		supportsExtension("osgearth_model_solid_feature_geom", "gwEarth solid geom model plugin");
	}

	virtual const char* className() const
	{
		return "gwEarth solid geometry Model Plugin";
	}

	virtual ReadResult readObject(const std::string& file_name, const Options* options) const
	{
		if (!acceptsExtension(osgDB::getLowerCaseFileExtension(file_name)))
		{
			return ReadResult::FILE_NOT_HANDLED;
		}
		return new SolidFeatureGeomModelSource(getModelSourceOptions(options));
	}
};

REGISTER_OSGPLUGIN(osgearth_model_solid_feature_geom, SolidFeatureGeomModelSourceDriver)