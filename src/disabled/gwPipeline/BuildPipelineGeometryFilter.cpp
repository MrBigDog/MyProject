#include <gwPipeline/BuildPipelineGeometryFilter.h>
#include <gwPipeline/PipeLineSymbol.h>
#include <gwPipeline/PipeJointSymbol.h>
#include <gwPipeline/PipeEquipmentSymbol.h>

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

#define LC "[BuildPipelineGeometryFilter] "

#define OE_TEST OE_NULL

//using namespace mdEarth;
using namespace osgEarth;
using namespace osgEarth::Features;
using namespace osgEarth::Symbology;

namespace gwPipeline
{
	osg::Geode* createUnitCylinder(int segmentNum)
	{
		int vertexNum = segmentNum * 2 + 2;

		osg::ref_ptr<osg::Vec3Array> va = new osg::Vec3Array; va->reserve(vertexNum);
		osg::ref_ptr<osg::Vec3Array> na = new osg::Vec3Array; na->reserve(vertexNum);
		osg::ref_ptr<osg::Vec2Array> ta = new osg::Vec2Array; ta->reserve(vertexNum);
		osg::ref_ptr<osg::Vec4Array> ca = new osg::Vec4Array; ca->reserve(1);
		ca->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));

		double deltaAngle = osg::PI*2.0 / ((double)segmentNum);
		for (int i = 0; i <= segmentNum; ++i)
		{
			double vAngle = (double)i * deltaAngle;
			double vX = sin(vAngle);
			double vY = cos(vAngle);

			va->push_back(osg::Vec3d(vX, vY, 0.5));
			va->push_back(osg::Vec3d(vX, vY, -0.5));

			na->push_back(osg::Vec3d(vX, vY, 0.0));
			na->push_back(osg::Vec3d(vX, vY, 0.0));

			float vU = (float)i / (float)segmentNum;
			ta->push_back(osg::Vec2(vU, 0.0));
			ta->push_back(osg::Vec2(vU, 1.0));
		}

		osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
		geom->setVertexArray(va);
		geom->setNormalArray(na);
		geom->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);
		geom->setTexCoordArray(0, ta);
		geom->setColorArray(ca);
		geom->setColorBinding(osg::Geometry::BIND_OVERALL);
		geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLE_STRIP, 0, vertexNum));

		osg::ref_ptr<osg::Geode> geode = new osg::Geode;
		geode->addDrawable(geom);

		//osg::Image* image = osgDB::readImageFile("pipelineys.jpg");
		//if (image && image != NULL)
		//{
		//	osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
		//	texture->setImage(image);

		//	if (texture)
		//	{
		//		geode->getOrCreateStateSet()->setTextureAttributeAndModes(0, texture, osg::StateAttribute::ON);
		//	}
		//}

		return geode.release();
	}

	BuildPipelineGeometryFilter::BuildPipelineGeometryFilter( const Style& style ) 
		: _style        ( style )
		, _maxAngle_deg ( 180.0 )
		, _geoInterp    ( GEOINTERP_RHUMB_LINE )
		, _maxPolyTilingAngle_deg( 45.0f )
		, instanceCache_(false)
	{

		//SceneSystem* scene = (SceneSystem*) Application::instance()->getSubsystemMgr()->get_subsystem("scene_system");
		//if(scene)
		//{
		//	_mapNode=scene->getSceneMapNode();
		//	_eleQuery= new osgEarth::ElevationQuery(_mapNode->getMap());//=osgEarth::ElevationQuery(_mapNode->getMap());
		//}
	}



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

	osg::Group* BuildPipelineGeometryFilter::processLines(FeatureList& features, const InstanceSymbol* symbol, FilterContext& context)
	{
		osg::ref_ptr<osg::Group> pipeGroup = createDelocalizeGroup();

		osgEarth::fast_map<std::string, URI> uriCache;

		std::set< URI > missing;
		StringExpression  uriEx    = *symbol->url();

		bool makeECEF = false;
		const SpatialReference* featureSRS = 0L;
		const SpatialReference* mapSRS = 0L;
		const SpatialReference* targetSRS = 0L;
		// set up referencing information:
		if ( context.isGeoreferenced() )
		{
			makeECEF   = context.getSession()->getMapInfo().isGeocentric();
			featureSRS = context.extent()->getSRS();
			mapSRS     = context.getSession()->getMapInfo().getProfile()->getSRS();

			targetSRS  = context.getSession()->getMapInfo().getSRS();
		}

		std::map< std::pair<URI, float>, osg::ref_ptr<osg::Node> > uniqueModels;

		for( FeatureList::iterator f = features.begin(); f != features.end(); ++f )
		{
			Feature* input = f->get();

			const std::string& st = input->eval(uriEx, &context);
			URI& instanceURI = uriCache[st];
			if(instanceURI.empty())
			{
				instanceURI = URI( st, uriEx.uriContext() );
			}

			osg::ref_ptr<InstanceResource> instance;
			if ( !findResource(instanceURI, symbol, context, missing, instance) )
				continue;

			// how that we have a marker source, create a node for it
			std::pair<URI,float> key( instanceURI, 1.0f ); //use 1.0 for models, since we don't want unique models based on scaling

			osg::ref_ptr<osg::Node>& model = uniqueModels[key];
			if ( !model.valid() )
			{
				//克隆一个新的，这样如果之前场景中有这个模型，改变当前的模型不会影响其他的;
				context.resourceCache()->cloneOrCreateInstanceNode(instance.get(), model, context.getDBOptions());
				//std::cout << "创建一个管段" << std::endl;
			}

			//static osg::ref_ptr<osg::Node> model;
			//if (!model)
			//{
			//	model = /*createUnitCylinder(17)*/osgDB::readNodeFile("E:\\BYL\\dogearth\\osgEarthWfs\\unitPipe.ive");
			//	std::cout << "创建一个管段" << std::endl;
			//}

			if (model.valid())
			{
				GeometryIterator parts( input->getGeometry(), true );
				while( parts.hasMore() )
				{
					Geometry* part = parts.next();

					if ( part->size() < 2 )
						continue;

					if ( !makeECEF && !targetSRS->isEquivalentTo(context.profile()->getSRS()) )
					{
						context.profile()->getSRS()->transform( part->asVector(), targetSRS );
					}

					//得到线的所有顶点;
					osg::ref_ptr<osg::Vec3Array> allPoints = new osg::Vec3Array();
					transformAndLocalize( part->asVector(), featureSRS, allPoints, mapSRS, _world2local, makeECEF );

					for (int i=0;i<(int)allPoints->size();++i)
					{
						if (i+1 < (int)allPoints->size())
						{
							osg::Vec3d loc = _local2world.getTrans();

							//osgEarth::GeoPoint geoP1;
							//geoP1.fromWorld(mapSRS, loc);

							////if (clampToTerrain(geoP1, 1.0))
							////{
							////	osg::Vec3d clampP1;
							////	geoP1.toWorld(clampP1);

							////	osg::Vec3d offset = clampP1 - loc;

							////	allPoints->at(i) += offset;
							////	allPoints->at(i + 1) += offset;
							////}

							//double sHeight = 0;
							//if (_mapNode->getTerrain()->getHeight(mapSRS, geoP1.x(), geoP1.y(), &sHeight))
							//{
							//	geoP1.z() = sHeight;
							//	osg::Vec3d clampP1;
							//	geoP1.toWorld(clampP1);

							//	osg::Vec3d offset = clampP1 - loc;

							//	allPoints->at(i) += offset;
							//	allPoints->at(i + 1) += offset;
							//}

							
							osg::Vec3d v_dir = (*allPoints)[i+1] - (*allPoints)[i];
							float v_length = v_dir.length();
							v_dir.normalize();

							osg::Vec3d v_pos = ((*allPoints)[i+1] + (*allPoints)[i])*0.5;

							osg::Quat v_rot;
							v_rot.makeRotate(osg::Vec3d(0.0, 0.0, 1.0), v_dir);
							osg::Matrixd v_rotMatrix = osg::Matrix::rotate(v_rot);

							osg::Vec3d scaleVec(0.8, 0.8, v_length);
							osg::Matrixd v_scaleMatrix = osg::Matrix::scale( scaleVec );

							osg::Matrixd mat = v_scaleMatrix * v_rotMatrix * osg::Matrixd::translate( v_pos );

							osg::ref_ptr<osg::MatrixTransform> xform = new osg::MatrixTransform();
							xform->setMatrix( mat );
							xform->setDataVariance( osg::Object::DYNAMIC );
							xform->addChild( model/*unitPipe_*/.get() );

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


	osg::Geode* BuildPipelineGeometryFilter::processPoints(FeatureList& features, const InstanceSymbol* symbol, FilterContext& context)
	{
		osg::Geode* geode = new osg::Geode();

		bool makeECEF = false;
		const SpatialReference* featureSRS = 0L;
		const SpatialReference* mapSRS = 0L;

		// set up referencing information:
		if ( context.isGeoreferenced() )
		{
			makeECEF   = context.getSession()->getMapInfo().isGeocentric();
			featureSRS = context.extent()->getSRS();
			mapSRS     = context.getSession()->getMapInfo().getProfile()->getSRS();
		}

		for( FeatureList::iterator f = features.begin(); f != features.end(); ++f )
		{
			Feature* input = f->get();

			GeometryIterator parts( input->getGeometry(), true );
			while( parts.hasMore() )
			{
				Geometry* part = parts.next();

				// extract the required point symbol; bail out if not found.
				const PointSymbol* point =
					input->style().isSet() && input->style()->has<PointSymbol>() ? input->style()->get<PointSymbol>() :
					_style.get<PointSymbol>();

				if ( !point )
					continue;

				// collect all the pre-transformation HAT (Z) values.
				osg::ref_ptr<osg::FloatArray> hats = new osg::FloatArray();
				hats->reserve( part->size() );
				for(Geometry::const_iterator i = part->begin(); i != part->end(); ++i )
					hats->push_back( i->z() );

				// resolve the color:
				osg::Vec4f primaryColor = point->fill()->color();

				osg::ref_ptr<osg::Geometry> osgGeom = new osg::Geometry();
				osgGeom->setUseVertexBufferObjects( true );
				osgGeom->setUseDisplayList( false );

				// embed the feature name if requested. Warning: blocks geometry merge optimization!
				if ( _featureNameExpr.isSet() )
				{
					const std::string& name = input->eval( _featureNameExpr.mutable_value(), &context );
					osgGeom->setName( name );
				}

				// build the geometry:
				osg::Vec3Array* allPoints = new osg::Vec3Array();

				transformAndLocalize( part->asVector(), featureSRS, allPoints, mapSRS, _world2local, makeECEF );

				osgGeom->addPrimitiveSet( new osg::DrawArrays(GL_POINTS, 0, allPoints->getNumElements()) );
				osgGeom->setVertexArray( allPoints );

				if ( input->style().isSet() )
				{
					//TODO: re-evaluate this. does it hinder geometry merging?
					applyPointSymbology( osgGeom->getOrCreateStateSet(), point );
				}

				// assign the primary color (PER_VERTEX required for later optimization)
				osg::Vec4Array* colors = new osg::Vec4Array;
				colors->assign( osgGeom->getVertexArray()->getNumElements(), primaryColor );
				osgGeom->setColorArray( colors );
				osgGeom->setColorBinding( osg::Geometry::BIND_PER_VERTEX );

				geode->addDrawable( osgGeom );

				// record the geometry's primitive set(s) in the index:
				if ( context.featureIndex() )
					context.featureIndex()->tagDrawable( osgGeom, input );

				// install clamping attributes if necessary
				if (_style.has<AltitudeSymbol>() &&
					_style.get<AltitudeSymbol>()->technique() == AltitudeSymbol::TECHNIQUE_GPU)
				{
					Clamping::applyDefaultClampingAttrs( osgGeom, input->getDouble("__oe_verticalOffset", 0.0) );
					Clamping::setHeights( osgGeom, hats.get() );
				}
			}
		}

		return geode;
	}

	// Borrowed from MeshConsolidator.cpp
	template<typename FROM, typename TO>
	osg::PrimitiveSet* copy( FROM* src, unsigned offset )
	{
		TO* newDE = new TO( src->getMode() );
		newDE->reserve( src->size() );
		for( typename FROM::const_iterator i = src->begin(); i != src->end(); ++i )
			newDE->push_back( (*i) + offset );
		return newDE;
	}


	osg::Group* BuildPipelineGeometryFilter::push( FeatureList& input, FilterContext& context )
	{
		if ( !isSupported() )
		{
			OE_WARN << "BuildPipelineGeometryFilter support not enabled" << std::endl;
			return 0L;
		}

		if ( _style.empty() )
		{
			OE_WARN << LC << "Empty style; cannot process features" << std::endl;
			return 0L;
		}

		osg::ref_ptr<const InstanceSymbol> symbol = _style.get<InstanceSymbol>();

		if ( !symbol.valid() )
		{
			OE_WARN << LC << "No appropriate symbol found in stylesheet; aborting." << std::endl;
			return 0L;
		}

		// establish the resource library, if there is one:
		resourceLib_ = 0L;

		const StyleSheet* sheet = context.getSession() ? context.getSession()->styles() : 0L;
		if ( symbol->library().isSet() )
		{
			resourceLib_ = sheet->getResourceLibrary( symbol->library()->expr() );
			if ( !resourceLib_.valid() )
			{
				OE_WARN << LC << "Unable to load resource library '" << symbol->library()->expr() << "'" << "; may not find instance models." << std::endl;
			}
		}

		// Compute localization info:
		FilterContext newContext( context );
		//newContext.extent().value().height() += 2000.0;
		computeLocalizers( context );

		osg::ref_ptr<osg::Group> result = new osg::Group();

		const PipeLineSymbol*      lineSymbol      = _style.get<PipeLineSymbol>();
		const PipeJointSymbol*     jointSymbol     = _style.get<PipeJointSymbol>();
		const PipeEquipmentSymbol* equipmentSymbol = _style.get<PipeEquipmentSymbol>();

		// bin the feautres into polygons, lines, polygonized lines, and points.
		FeatureList polygons;
		FeatureList lines;
		FeatureList joints;
		FeatureList equipments;

		for(FeatureList::iterator i = input.begin(); i != input.end(); ++i)
		{
			Feature* f = i->get();

			// first consider the overall style:
			bool has_linesymbol      = lineSymbol != 0L;
			bool has_jointsymbol     = jointSymbol != 0L;
			bool has_equipmentsymbol = equipmentSymbol!=0L;

			// if the feature has a style set, that overrides:
			if ( f->style().isSet() )
			{
				has_linesymbol      = has_linesymbol      || f->style()->has<PipeLineSymbol>();
				has_jointsymbol     = has_jointsymbol     || f->style()->has<PipeJointSymbol>();
				has_equipmentsymbol = has_equipmentsymbol || f->style()->has<PipeEquipmentSymbol>();
			}

			// if no style is set, use the geometry type:
			if ( !has_linesymbol && !has_jointsymbol && !has_equipmentsymbol && f->getGeometry() )
			{
				switch( f->getGeometry()->getComponentType() )
				{
				default:
				case Geometry::TYPE_LINESTRING:
				case Geometry::TYPE_RING:
				case Geometry::TYPE_POLYGON:
					f->style()->add( new PipeLineSymbol() );
					has_linesymbol = true;
					break;

				case Geometry::TYPE_POINTSET:
					f->style()->add( new PipeJointSymbol() );//如果没有指定是什么类型的管点的话，默认是Joint
					has_jointsymbol = true;
					break;

					//case Geometry::TYPE_POLYGON:
					//    f->style()->add( new PipeLineSymbol() );
					//    has_linesymbol = true;
					//    break;
				}
			}

			if ( has_linesymbol )
				lines.push_back( f );

			if ( has_jointsymbol )
				joints.push_back( f );

			if ( has_equipmentsymbol )
				equipments.push_back( f );
		}

		// process them separately.
		if ( lines.size() > 0 )
		{
			osg::ref_ptr<osg::Group> v_node = processLines(lines, lineSymbol, newContext);
			if (v_node)
			{
				result->addChild(v_node);
			}
		}
		if (joints.size()>0)
		{
			osg::ref_ptr<osg::Group> jointGroup = processPoints(joints, jointSymbol, newContext);
		}
		if (equipments.size()>0)
		{
			//osg::ref_ptr<osg::Group> equipmentGroup = processEquipments(equipments, equipmentSymbol, newContext);
		}

		//去掉这个会导致没有纹理;
		osgEarth::Registry::shaderGenerator().run(result.get(),"mdPipeline.PipelineCompiler" );
		//if ( points.size() > 0 )
		//{
		//    OE_TEST << LC << "Building " << points.size() << " points." << std::endl;
		//    osg::ref_ptr<osg::Geode> geode = processPoints(points, context);
		//    if ( geode->getNumDrawables() > 0 )
		//    {
		//        osgUtil::Optimizer o;
		//        o.optimize( geode.get(), osgUtil::Optimizer::MERGE_GEOMETRY );

		//        applyPointSymbology( geode->getOrCreateStateSet(), point );
		//        result->addChild( geode.get() );
		//    }
		//}

		//// indicate that geometry contains clamping attributes
		//if (_style.has<AltitudeSymbol>() &&
		//    _style.get<AltitudeSymbol>()->technique() == AltitudeSymbol::TECHNIQUE_GPU)
		//{
		//    Clamping::installHasAttrsUniform( result->getOrCreateStateSet() );
		//}    

		//// Prepare buffer objects.
		//AllocateAndMergeBufferObjectsVisitor allocAndMerge;
		//result->accept( allocAndMerge );

		//if ( result->getNumChildren() > 0 )
		//{
		//    // apply the delocalization matrix for no-jitter
		//    return delocalize( result.release() );
		//}
		//else
		//{
		//    return 0L;
		//}

		context = newContext;

		return result.release();
	}

	bool BuildPipelineGeometryFilter::findResource(const URI& uri
		, const InstanceSymbol* symbol
		, FilterContext& context
		, std::set<URI>& missing
		, osg::ref_ptr<InstanceResource>& output )
	{
		// be careful about refptrs here since instanceCache_ is an LRU.
		InstanceCache::Record rec;
		if ( instanceCache_.get(uri, rec) )
		{
			// found it in the cache:
			output = rec.value().get();
		}
		else if ( resourceLib_.valid() )
		{
			// look it up in the resource library:
			output = resourceLib_->getInstance( uri.base(), context.getDBOptions() );
		}
		else
		{
			// create it on the fly:
			output = symbol->createResource();
			output->uri() = uri;
			instanceCache_.insert( uri, output.get() );
		}

		// failed to find the instance.
		if ( !output.valid() )
		{
			if ( missing.find(uri) == missing.end() )
			{
				missing.insert(uri);
				OE_WARN << LC << "Failed to locate resource: " << uri.full() << std::endl;
			}
		}

		return output.valid();
	}


	bool BuildPipelineGeometryFilter::clampToTerrain( osgEarth::GeoPoint& inpoint, float offset)
	{
		osg::Vec3d endpoint;
		inpoint.toWorld(endpoint);

		osg::Vec3d start = osg::Vec3d(0.0, 0.0, 0.0);
		osg::Vec3d unitdir = osg::Vec3d(endpoint);
		unitdir.normalize();
		
		osg::Vec3d end = unitdir*osg::WGS_84_RADIUS_EQUATOR*1.5;

		DPLineSegmentIntersector* i = new DPLineSegmentIntersector( start, end );
		i->setIntersectionLimit( osgUtil::Intersector::LIMIT_NEAREST );
		osgUtil::IntersectionVisitor iv;
		iv.setIntersector( i );
		_mapNode->getTerrain()->accept( iv );
		osgUtil::LineSegmentIntersector::Intersections& results = i->getIntersections();

		if (results.empty())
		{
			return false;
		}
		osg::Vec3d hitpoint = results.begin()->getWorldIntersectPoint();

		osgEarth::GeoPoint mapres;
		mapres.fromWorld(inpoint.getSRS(), hitpoint);

		inpoint.z() = mapres.z() + offset;

		return true;
	}
}
