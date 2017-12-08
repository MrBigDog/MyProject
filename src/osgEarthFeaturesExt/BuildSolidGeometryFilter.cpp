#include "BuildSolidGeometryFilter.h"

#include <osgEarth/Registry>
#include <osgEarth/DrawInstanced>
#include <osgEarth/ScreenSpaceLayout>
#include <osgEarth/Tessellator>
#include <osgEarth/Utils>
#include <osgEarth/ECEF>
#include <osgEarth/Clamping>
#include <osgEarth/Terrain>

#include <osgEarthFeatures/Session>
#include <osgEarthFeatures/FeatureSourceIndexNode>

#include <osgEarthSymbology/MeshSubdivider>
#include <osgEarthSymbology/ResourceCache>
#include <osgEarthSymbology/MeshConsolidator>

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/LineWidth>
#include <osg/LineStipple>
#include <osg/Point>
#include <osg/Depth>
#include <osg/PolygonOffset>
#include <osg/MatrixTransform>
#include <osgText/Text>
#include <osgUtil/Tessellator>
#include <osgUtil/Optimizer>
#include <osgUtil/Simplifier>
#include <osgUtil/SmoothingVisitor>
#include <osgDB/WriteFile>
#include <osg/Version>
#include <osg/ShapeDrawable>
#include <iterator>
#include <assert.h>

#define LC "[BuildSolidGeometryFilter] "

#define OE_TEST OE_NULL

using namespace osgEarth;
using namespace osgEarth::Features;
using namespace osgEarth::Symbology;

namespace oeFeaturesExt
{
	BuildSolidGeometryFilter::BuildSolidGeometryFilter(const Style& style)
		: _style(style)
		, _maxAngle_deg(180.0)
		, _geoInterp(GEOINTERP_RHUMB_LINE)
		, _maxPolyTilingAngle_deg(45.0f)
		, instanceCache_(false)
	{}

	//_mapNode

	////joint的绘制;
	//{
	//	vector_3d_array directions;
	//	directions.push_back(osg::Vec3d(0.39934945106832698, 0.82792213487017408, -0.39378287738732259));
	//	directions.push_back(osg::Vec3d(-0.87383703138129454, 0.040789825066686207, -0.48450493574137649));

	//	double_value_array radius;
	//	radius.push_back(1.6799999475479126);
	//	radius.push_back(1.6799999475479126);

	//	double_value_array widths;
	//	widths.push_back(1.6799999475479126);
	//	widths.push_back(1.6799999475479126);

	//	double_value_array heights;
	//	heights.push_back(1.6799999475479126);
	//	heights.push_back(1.6799999475479126);

	//	int direct_num = 2;

	//	udg_circle_joint_entry* joint_entry = new udg_circle_joint_entry;

	//	us_pipeline_generate_base joint_entry_generator;
	//	joint_entry_generator.check_point_type(joint_entry, directions, radius/*widths, heights*/, direct_num);
	//	joint_entry->m_texture_scl = 0.5;

	//	//joint_entry->m_jointType = udg_circle_joint_entry::SQUARE;

	//	osg::ref_ptr<osg::Group> jointNode = pipeline_joint_factory::generateCircleJointNode(*joint_entry);

	//	osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
	//	texture->setImage(osgDB::readImageFile("pipelineys.jpg"));
	//	texture->setWrap(osg::Texture2D::WRAP_S, osg::Texture2D::REPEAT);
	//	texture->setWrap(osg::Texture2D::WRAP_T, osg::Texture2D::REPEAT);
	//	texture->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR_MIPMAP_LINEAR);
	//	texture->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);
	//	jointNode->getOrCreateStateSet()->setTextureAttributeAndModes(0, texture);


	//	osg::ref_ptr<osgDB::Options>       _dbOptions = new osgDB::Options;
	//	_dbOptions->setObjectCacheHint(osgDB::Options::CACHE_IMAGES);
	//}

	osg::Group* BuildSolidGeometryFilter::processSolidLines(FeatureList& features, const SolidLineSymbol* symbol, FilterContext& context)
	{
		osg::ref_ptr<osg::Group> pipeGroup = createDelocalizeGroup();

		osgEarth::fast_map<std::string, URI> uriCache;

		std::set< URI > missing;
		//StringExpression  uriEx = *symbol->url();

		bool makeECEF = false;
		const SpatialReference* featureSRS = 0L;
		const SpatialReference* mapSRS = 0L;
		const SpatialReference* targetSRS = 0L;
		// set up referencing information:
		if (context.isGeoreferenced())
		{
			makeECEF = context.getSession()->getMapInfo().isGeocentric();
			featureSRS = context.extent()->getSRS();
			mapSRS = context.getSession()->getMapInfo().getProfile()->getSRS();
			targetSRS = context.getSession()->getMapInfo().getSRS();
		}

		osg::ref_ptr<osg::Node> model = symbol->getUnidSolidLineModel();
		if (!model) return 0L;

		//std::map< std::pair<URI, float>, osg::ref_ptr<osg::Node> > uniqueModels;
		for (FeatureList::iterator f = features.begin(); f != features.end(); ++f)
		{
			Feature* input = f->get();

			if (model.valid())
			{
				GeometryIterator parts(input->getGeometry(), true);
				while (parts.hasMore())
				{
					Geometry* part = parts.next();

					if (part->size() < 2)
						continue;

					if (!makeECEF && !targetSRS->isEquivalentTo(context.profile()->getSRS()))
					{
						context.profile()->getSRS()->transform(part->asVector(), targetSRS);
					}

					//得到线的所有顶点;
					osg::ref_ptr<osg::Vec3Array> allPoints = new osg::Vec3Array();
					transformAndLocalize(part->asVector(), featureSRS, allPoints, mapSRS, _world2local, makeECEF);

					for (int i = 0; i < (int)allPoints->size(); ++i)
					{
						if (i + 1 < (int)allPoints->size())
						{
							osg::Vec3d loc = _local2world.getTrans();

							osg::Vec3d v_dir = (*allPoints)[i + 1] - (*allPoints)[i];
							float v_length = v_dir.length();
							v_dir.normalize();

							osg::Vec3d v_pos = ((*allPoints)[i + 1] + (*allPoints)[i])*0.5;

							osg::Quat v_rot;
							v_rot.makeRotate(osg::Vec3d(0.0, 0.0, 1.0), v_dir);
							osg::Matrixd v_rotMatrix = osg::Matrix::rotate(v_rot);

							osg::Vec3d scaleVec(0.8, 0.8, v_length);
							osg::Matrixd v_scaleMatrix = osg::Matrix::scale(scaleVec);

							osg::Matrixd mat = v_scaleMatrix * v_rotMatrix * osg::Matrixd::translate(v_pos);

							osg::ref_ptr<osg::MatrixTransform> xform = new osg::MatrixTransform();
							xform->setMatrix(mat);
							xform->setDataVariance(osg::Object::DYNAMIC);
							xform->addChild(model/*unitPipe_*/.get());

							// name the feature if necessary
							//if ( !_featureNameExpr.empty() )
							//{
							//	const std::string& name = input->eval( _featureNameExpr, &context);
							//	if ( !name.empty() )
							//		xform->setName( name );
							//}
							//if (context.featureIndex())
							//{
							//	context.featureIndex()->tagNode(xform, input);
							//}

							pipeGroup->addChild(xform);
						}
					}
				}
			}
		}
		return pipeGroup.release();
	}

	osg::Geode * BuildSolidGeometryFilter::processSolidLines(FeatureList & input, FilterContext & cx)
	{
		return nullptr;
	}

	osg::Geode * BuildSolidGeometryFilter::processSolidJoints(FeatureList & input, FilterContext & cx)
	{
		return nullptr;
	}

	osg::Group* BuildSolidGeometryFilter::push(FeatureList& input, FilterContext& context)
	{
		if (!isSupported())
		{
			OE_WARN << "BuildSolidGeometryFilter support not enabled" << std::endl;
			return 0L;
		}

		if (_style.empty())
		{
			OE_WARN << LC << "Empty style; cannot process features" << std::endl;
			return 0L;
		}

		const StyleSheet* sheet = context.getSession() ? context.getSession()->styles() : 0L;

		// Compute localization info:
		FilterContext newContext(context);
		//newContext.extent().value().height() += 2000.0;
		computeLocalizers(context);

		osg::ref_ptr<osg::Group> result = new osg::Group();

		const SolidLineSymbol*      lineSymbol = _style.get<SolidLineSymbol>();
		//const SolidJointSymbol*     jointSymbol = _style.get<SolidJointSymbol>();

		// bin the feautres into polygons, lines, polygonized lines, and points.
		//FeatureList polygons;
		FeatureList solidLines;
		FeatureList solidJoints;
		for (FeatureList::iterator i = input.begin(); i != input.end(); ++i)
		{
			Feature* f = i->get();

			// first consider the overall style:
			bool has_linesymbol = lineSymbol != 0L;
			//bool has_jointsymbol = jointSymbol != 0L;

			// if the feature has a style set, that overrides:
			if (f->style().isSet())
			{
				has_linesymbol = has_linesymbol || f->style()->has<SolidLineSymbol>();
				//has_jointsymbol = has_jointsymbol || f->style()->has<SolidJointSymbol>();
			}

			// if no style is set, use the geometry type:
			if (!has_linesymbol && /*!has_jointsymbol &&*/ f->getGeometry())
			{
				switch (f->getGeometry()->getComponentType())
				{
				default:
				case Geometry::TYPE_LINESTRING:
				case Geometry::TYPE_RING:
				case Geometry::TYPE_POLYGON:
					f->style()->add(new SolidLineSymbol());
					has_linesymbol = true;
					break;

				case Geometry::TYPE_POINTSET:
					//f->style()->add(new SolidJointSymbol());//如果没有指定是什么类型的管点的话，默认是Joint
					//has_jointsymbol = true;
					break;
				}
			}

			if (has_linesymbol)
				solidLines.push_back(f);
		}

		// process them separately.
		if (solidLines.size() > 0)
		{
			osg::ref_ptr<osg::Group> v_node = processSolidLines(solidLines, lineSymbol, newContext);
			if (v_node)
			{
				result->addChild(v_node);
			}
		}
		if (solidJoints.size() > 0)
		{
			//osg::ref_ptr<osg::Group> jointGroup = processSolidJoints(solidJoints, jointSymbol, newContext);
		}

		//去掉这个会导致没有纹理;
		osgEarth::Registry::shaderGenerator().run(result.get(), "gwEarth.SolidGeomCompiler");

		context = newContext;

		return result.release();
	}

	bool BuildSolidGeometryFilter::findResource(const URI& uri
		, const InstanceSymbol* symbol
		, FilterContext& context
		, std::set<URI>& missing
		, osg::ref_ptr<InstanceResource>& output)
	{
		// be careful about refptrs here since instanceCache_ is an LRU.
		InstanceCache::Record rec;
		if (instanceCache_.get(uri, rec))
		{
			// found it in the cache:
			output = rec.value().get();
		}
		else if (resourceLib_.valid())
		{
			// look it up in the resource library:
			output = resourceLib_->getInstance(uri.base(), context.getDBOptions());
		}
		else
		{
			// create it on the fly:
			output = symbol->createResource();
			output->uri() = uri;
			instanceCache_.insert(uri, output.get());
		}

		// failed to find the instance.
		if (!output.valid())
		{
			if (missing.find(uri) == missing.end())
			{
				missing.insert(uri);
				OE_WARN << LC << "Failed to locate resource: " << uri.full() << std::endl;
			}
		}

		return output.valid();
	}
}
