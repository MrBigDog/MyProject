#include <gwTool/CommonPickTool.h>
#include <osgEarth/DrapeableNode>
#include <osgEarth/MapNode>
#include <osgEarth/Terrain>
#include <osgEarth/Registry>
#include <osgEarth/PrimitiveIntersector>
//#include <osgEarth/DPLineSegmentIntersector>
#include <osgEarthFeatures/Feature>
#include <osgEarthAnnotation/FeatureNode>
#include <osgEarthSymbology/Geometry>

#include <osg/Depth>
#include <osgViewer/View>

namespace gwTool
{
	//////////////////////////////////////////////////////////////////////////////////////////////////
	CommonPickTool::CommonPickTool(osgEarth::MapNode* mapNode)
		: mapNode_(mapNode)
		, isCanPick_(false)
		, isActiveMouseMoving_(false)
	{
		setDefaultCallback(NULL);
	}

	CommonPickTool::~CommonPickTool()
	{
	}

	void CommonPickTool::setDefaultCallback(Callback* cb)
	{
		defaultCallback_ = cb;
	}

	void CommonPickTool::setDigTerrainFuction()
	{
		setDefaultCallback(new digToolCallback(this, mapNode_));
	}

	void CommonPickTool::setDistanceMeasureFuction()
	{
		setDefaultCallback(new distanceMeasureToolCallback(this, mapNode_));
	}

	void CommonPickTool::setHightMeasureFuction()
	{
		setDefaultCallback(new heightMeasureToolCallback(this, mapNode_));
	}

	void CommonPickTool::setAreaMeasureFuction()
	{
		setDefaultCallback(new areaMeasureToolCallback(this, mapNode_));
	}

	void CommonPickTool::setLineSightAnalysisFunction()
	{
		setDefaultCallback(new lineSightAnalysisCallback(this, mapNode_));
	}

	void CommonPickTool::setAreaSightAnalysisFunction()
	{
		setDefaultCallback(new areaSightAnalysisCallback(this, mapNode_));
	}

	void CommonPickTool::setPolyhedralSightAnalysisFunction()
	{
		//setDefaultCallback(new polyhedralSightAnalysisCallback(this, mapNode_));
	}
	void CommonPickTool::setTerrainProfileAnalysisFunction()
	{
		setDefaultCallback(new terrainProfileAnalysisCallback(this, mapNode_));
	}

	bool CommonPickTool::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
	{
		if (!defaultCallback_ || defaultCallback_ == NULL)
		{
			return false;
		}

		switch (ea.getEventType())
		{
		case osgGA::GUIEventAdapter::KEYDOWN:
		{
			if (ea.getKey() == osgGA::GUIEventAdapter::KEY_Control_L || ea.getKey() == osgGA::GUIEventAdapter::KEY_Control_R)
				isCanPick_ = true;
			break;
		}
		case osgGA::GUIEventAdapter::KEYUP:
		{
			if (ea.getKey() == osgGA::GUIEventAdapter::KEY_Control_L || ea.getKey() == osgGA::GUIEventAdapter::KEY_Control_R)
				isCanPick_ = false;
			break;
		}
		default:
			break;
		}

		if (!isCanPick_) return false;

		switch (ea.getEventType())
		{
		case osgGA::GUIEventAdapter::PUSH:
		{
			if (ea.getButton() != osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON) break;

			osg::Vec3d world;
			if (getPickPos(ea, aa, isOnlyPickTerrain_, world))
			{
				osgEarth::GeoPoint geopos;
				geopos.fromWorld(mapNode_->getMapSRS(), world);
				defaultCallback_->mousePick(geopos.vec3d());
			}

			break;
		}

		case osgGA::GUIEventAdapter::MOVE:
		{
			if (!isActiveMouseMoving_) break;

			osg::Vec3d world;
			if (getPickPos(ea, aa, isOnlyPickTerrain_, world))
			{
				osgEarth::GeoPoint geopos;
				geopos.fromWorld(mapNode_->getMapSRS(), world);
				defaultCallback_->mouseMove(geopos.vec3d());
			}

			break;
		}

		case osgGA::GUIEventAdapter::RELEASE:
		{
			if (ea.getButton() == osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON)
			{
				defaultCallback_->finish();
			}
			break;
		}
		default:
			break;
		}

		return false;
	}

	bool CommonPickTool::getPickPos(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, bool onlyPickTerrain, osg::Vec3d& pos)
	{
		if (onlyPickTerrain)
		{
			osg::Vec3d world;
			if (mapNode_->getTerrain()->getWorldCoordsUnderMouse(aa.asView(), ea.getX(), ea.getY(), world))
			{
				pos = world;
				return true;
			}
		}
		else
		{
			osgViewer::View* view2 = dynamic_cast<osgViewer::View*>(aa.asView());
			if (!view2)
				return false;

			osgUtil::LineSegmentIntersector::Intersections results;

			osg::NodePath path;
			path.push_back(mapNode_.get());

			if (view2->computeIntersections(ea.getX(), ea.getY(), path, results))
			{
				osgUtil::LineSegmentIntersector::Intersection first = *(results.begin());
				pos = first.getWorldIntersectPoint();

				return true;
			}
		}
		return false;
	}

	//=================================================================================================================
	digToolCallback::digToolCallback(CommonPickTool*cpt, osgEarth::MapNode *mapnode)
		: CommonPickTool::Callback(mapnode, cpt)
		, va_(new osg::Vec3dArray)
	{
		addTemptModelLayer("dig_tempt_layer");
		cpt_->setOnlyPickTerrain(false);
		cpt_->setActiveMouseMoving(false);
	}

	digToolCallback::~digToolCallback()
	{
		removeTemptModelLayer();
	}

	osg::Node* digToolCallback::createFeatureNode(osg::Vec3dArray* va, const osgEarth::Color& color)
	{
		if (va->empty()) return NULL;

		osg::ref_ptr<osgEarth::Features::Feature> feature = new osgEarth::Features::Feature(new osgEarth::Symbology::LineString(), mapNode_->getMapSRS());
		feature->geoInterp() = osgEarth::GEOINTERP_GREAT_CIRCLE;

		osgEarth::Style pathStyle;
		pathStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->color() = osgEarth::Color::Red;
		pathStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->width() = 2.0f;
		//pathStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->tessellationSize() = 75000;
		//pathStyle.getOrCreate<osgEarth::Symbology::PointSymbol>()->size() = 5;
		//pathStyle.getOrCreate<osgEarth::Symbology::PointSymbol>()->fill()->color() = osgEarth::Color::Red;
		pathStyle.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color() = color;



		//if (va->size()==1)
		//{
		//	//画点;
		//	osgEarth::Symbology::PointSymbol* pointsym =
		//		feature->style()->getOrCreate<osgEarth::Symbology::PointSymbol>();
		//	pointsym->size()=20;
		//	pointsym->fill()->color() = color;
		//}
		//else if (va->size()==2)
		//{
		//	//画线;
		//	osgEarth::Symbology::LineSymbol* linesym = 
		//		feature->style()->getOrCreate<osgEarth::Symbology::LineSymbol>();
		//	linesym->stroke()->color() = color;
		//	linesym->stroke()->width() = 3.0f;
		//}
		//else
		//{
		//	//画多边形;
		//	osgEarth::Symbology::PolygonSymbol* ps = feature->style()->getOrCreate<osgEarth::Symbology::PolygonSymbol>();
		//	ps->fill()->color() = color;//::Yellow;
		//}

		for (unsigned int i = 0; i < va->size(); ++i)
		{
			feature->getGeometry()->push_back(va->at(i));
		}

		osg::ref_ptr<osgEarth::Annotation::FeatureNode> fn = new osgEarth::Annotation::FeatureNode(mapNode_, feature.get(), pathStyle);
		fn->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

		return fn.release();
	}

	void digToolCallback::mousePick(const osg::Vec3d& point)
	{
		va_->push_back(point);

		if (!temptLayer_) return;

		osg::Group* group = dynamic_cast<osg::Group*>(mapNode_->getLayerNode(temptLayer_));
		if (!group) return;

		osg::ref_ptr<osgEarth::DrapeableNode> dn = new osgEarth::DrapeableNode;
		dn->addChild(createFeatureNode(va_, osgEarth::Color(1.0f, 1.0f, 0.0f, 0.6f)));

		if (group->getNumChildren() == 0)
		{
			group->addChild(dn);
		}
		else
		{
			group->replaceChild(group->getChild(0), dn);
		}
	}

	void digToolCallback::finish()
	{
		//if (!temptLayer_) return;

		//osg::Group* group = dynamic_cast<osg::Group*>(mapNode_->getModelLayerNode(temptLayer_));
		//if (!group) return;

		//osg::ref_ptr<ErosionDrapeableNode> edn = new ErosionDrapeableNode;
		//edn->addChild(createFeatureNode(va_, osgEarth::Color(1.0f, 1.0f, 1.0f, 1.0f)));

		//group->addChild(edn);

		//va_->clear();

		//osg::ref_ptr<osgEarth::Features::Feature> feature = new osgEarth::Features::Feature(new osgEarth::Symbology::LineString(), mapNode_->getMapSRS());
		//feature->geoInterp() = osgEarth::GEOINTERP_GREAT_CIRCLE;

		//osgEarth::Symbology::PolygonSymbol* ps = feature->style()->getOrCreate<osgEarth::Symbology::PolygonSymbol>();
		//ps->fill()->color() = osgEarth::Color(1.0,1.0,1.0,1.0);//::Yellow;

		//for (unsigned int i=0; i<va_->size(); ++i)
		//{
		//	feature->getGeometry()->push_back(va_->at(i));
		//}
		//va_->clear();

		//osg::ref_ptr<osgEarth::Annotation::FeatureNode> fn = new osgEarth::Annotation::FeatureNode( mapNode_, feature.get() );
		//fn->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

		//osg::ref_ptr<ErosionDrapeableNode> edn = new ErosionDrapeableNode;
		//edn->addChild(fn);

		//if (temptLayer_)
		//{
		//	osg::Group* group = dynamic_cast<osg::Group*>(mapNode_->getModelLayerNode(temptLayer_));
		//	if (group)
		//	{
		//		group->addChild(edn);
		//	}
		//}
	}

	void digToolCallback::reset()
	{
		va_->clear();
		if (temptLayer_)
		{
			osg::Group* group = dynamic_cast<osg::Group*>(mapNode_->getLayerNode(temptLayer_));
			if (group)
			{
				group->removeChildren(0, group->getNumChildren());
			}
		}
	}

	//两点通视分析;===============================================================================================================
	lineSightAnalysisCallback::lineSightAnalysisCallback(CommonPickTool*cpt, osgEarth::MapNode* mapNode)
		:CommonPickTool::Callback(mapNode, cpt)
	{
		addTemptModelLayer("line_sight_analysis_tempt_layer");
		cpt_->setOnlyPickTerrain(false);
		cpt_->setActiveMouseMoving(false);
		pickIndex_ = 0;
	}

	void lineSightAnalysisCallback::mousePick(const osg::Vec3d &point)
	{
		if (pickIndex_ == 0)
		{
			osg::Vec3d sp = point;
			sp[2] += 0.5;//避免由于精度问题而被地面遮挡;
			osgEarth::GeoPoint startPos(mapNode_->getMapSRS()->getGeographicSRS(), sp[0], sp[1], sp[2]);
			los_->setStart(startPos);
			los_->setEnd(startPos);
			pickIndex_++;
			cpt_->setActiveMouseMoving(true);
			return;
		}
		else if (pickIndex_ == 1)
		{
			los_->setEnd(osgEarth::GeoPoint(mapNode_->getMapSRS()->getGeographicSRS(), point[0], point[1], point[2]));
			pickIndex_ = 0;
			cpt_->setActiveMouseMoving(false);
			return;
		}
		else return;
	}

	void lineSightAnalysisCallback::mouseMove(const osg::Vec3d& point)
	{
		if (pickIndex_ == 1)
		{
			los_->setEnd(osgEarth::GeoPoint(mapNode_->getMapSRS()->getGeographicSRS(), point[0], point[1], point[2]));
		}
	}

	void lineSightAnalysisCallback::finish() {}
	void lineSightAnalysisCallback::reset() {}
	void lineSightAnalysisCallback::addTemptModelLayer(const std::string& name)
	{
		los_ = new osgEarth::Util::LinearLineOfSightNode(mapNode_);
		los_->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
		los_->getOrCreateStateSet()->setAttributeAndModes(new osg::Depth(osg::Depth::ALWAYS, 0, 1, false));
		los_->setTerrainOnly(false);

		osg::ref_ptr<osg::Group> losRoot = new osg::Group;
		losRoot->addChild(los_);

		temptLayer_ = new osgEarth::ModelLayer(name, losRoot);
		mapNode_->getMap()->addModelLayer(temptLayer_);
	}

	//==============================================================================================================
	areaSightAnalysisCallback::areaSightAnalysisCallback(CommonPickTool*cpt, osgEarth::MapNode* mapNode)
		:CommonPickTool::Callback(mapNode, cpt)
	{
		addTemptModelLayer("area_sight_analysis_tempt_layer");
		cpt_->setOnlyPickTerrain(false);
		cpt_->setActiveMouseMoving(false);
		pickIndex_ = 0;
	}

	void areaSightAnalysisCallback::addTemptModelLayer(const std::string& name)
	{
		rlos_ = new osgEarth::Util::RadialLineOfSightNode(mapNode_);
		rlos_->setCenter(osgEarth::GeoPoint(mapNode_->getMapSRS()->getGeographicSRS(), 0, 0, 0));
		rlos_->setRadius(1.0);
		rlos_->setNumSpokes(100);
		rlos_->setTerrainOnly(false);

		osg::ref_ptr<osg::Group> losRoot = new osg::Group;
		losRoot->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
		losRoot->getOrCreateStateSet()->setAttributeAndModes(new osg::Depth(osg::Depth::ALWAYS, 0, 1, false));
		losRoot->addChild(rlos_);

		temptLayer_ = new osgEarth::ModelLayer(name, losRoot);
		mapNode_->getMap()->addModelLayer(temptLayer_);
	}

	void areaSightAnalysisCallback::mousePick(const osg::Vec3d &point)
	{
		if (pickIndex_ == 0)
		{
			osg::Vec3d sp = point;
			sp[2] += 0.1;//避免因为精度问题而被遮挡;
			rlos_->setCenter(osgEarth::GeoPoint(mapNode_->getMapSRS()->getGeographicSRS(), point[0], point[1], point[2]));
			rlos_->setRadius(0.1);
			pickIndex_++;
			cpt_->setActiveMouseMoving(true);
			return;
		}
		else if (pickIndex_ == 1)
		{
			double rad = rlos_->getCenter().distanceTo(osgEarth::GeoPoint(mapNode_->getMapSRS()->getGeographicSRS(), point[0], point[1], point[2]));
			rlos_->setRadius(rad);
			pickIndex_ = 0;
			cpt_->setActiveMouseMoving(false);
			return;
		}
		else return;
	}

	void areaSightAnalysisCallback::mouseMove(const osg::Vec3d& point)
	{
		if (pickIndex_ == 1)
		{
			double rad = rlos_->getCenter().distanceTo(osgEarth::GeoPoint(mapNode_->getMapSRS()->getGeographicSRS(), point[0], point[1], point[2]));
			rlos_->setRadius(rad);
		}
	}

	//==============================================================================================================

	//polyhedralSightAnalysisCallback::polyhedralSightAnalysisCallback(CommonPickTool* cpt, osgEarth::MapNode* mapnode)
	//	: CommonPickTool::Callback(mapnode, cpt)
	//{
	//	addTemptModelLayer("polyhedral_sight_analysis_tempt_layer");
	//	cpt_->setOnlyPickTerrain(true);
	//	cpt_->setActiveMouseMoving(false);
	//	pickIndex_ = 0;
	//}

	//void polyhedralSightAnalysisCallback::addTemptModelLayer( const std::string& name )
	//{
	//	plos_ = new osgEarth::Util::PolyhedralLineOfSightNode(mapNode_);
	//	plos_->setPosition(osgEarth::GeoPoint(mapNode_->getMapSRS()->getGeographicSRS(), 0, 0, 0));
	//	plos_->setDistance(1.0);

	//	osg::ref_ptr<osg::Group> posRoot = new osg::Group;
	//	posRoot->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	//	posRoot->getOrCreateStateSet()->setAttributeAndModes(new osg::Depth(osg::Depth::ALWAYS, 0, 1, false));
	//	posRoot->addChild(plos_);

	//	temptLayer_ = new osgEarth::ModelLayer(name, posRoot);
	//	mapNode_->getMap()->addModelLayer(temptLayer_);
	//}

	//void polyhedralSightAnalysisCallback::mousePick(const osg::Vec3d &point )
	//{
	//	if (pickIndex_ == 0)
	//	{
	//		osg::Vec3d sp = point;
	//		sp[2] += 0.1;//避免因为精度问题而被遮挡;
	//		plos_->setPosition(osgEarth::GeoPoint(mapNode_->getMapSRS()->getGeographicSRS(), point[0],point[1],point[2]));
	//		plos_->setDistance(0.1);
	//		pickIndex_++;
	//		return;
	//	}
	//	else if (pickIndex_==1)
	//	{
	//		double rad = plos_->getPosition().distanceTo(osgEarth::GeoPoint(mapNode_->getMapSRS()->getGeographicSRS(), point[0],point[1],point[2]));
	//		plos_->setDistance(rad);
	//		pickIndex_ =0;
	//		return;
	//	}
	//	else return;
	//}


	//============================================================================================================
	osg::Camera* createHud(double width, double height)
	{
		osg::Camera* hud = new osg::Camera;
		hud->setProjectionMatrix(osg::Matrix::ortho2D(0, width, 0, height));
		hud->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
		hud->setViewMatrix(osg::Matrix::identity());
		hud->setClearMask(GL_DEPTH_BUFFER_BIT);
		hud->setRenderOrder(osg::Camera::POST_RENDER);
		hud->setAllowEventFocus(false);
		hud->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE);
		hud->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
		hud->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);

		return hud;
	}

	class TerrainProfileGraph : public osg::Group
	{
	public:
		struct GraphChangedCallback : public osgEarth::Util::TerrainProfileCalculator::ChangedCallback
		{
			GraphChangedCallback(TerrainProfileGraph* graph) :graph_(graph)
			{ }

			virtual void onChanged(const osgEarth::Util::TerrainProfileCalculator* sender)
			{
				graph_->setTerrainProfile(sender->getProfile());
			}

			TerrainProfileGraph* graph_;
		};

		TerrainProfileGraph(osgEarth::Util::TerrainProfileCalculator* profileCalculator
			, double graphWidth = 200
			, double graphHeight = 200)

			: profileCalculator_(profileCalculator)
			, graphWidth_(graphWidth)
			, graphHeight_(graphHeight)
			, color_(1.0f, 1.0f, 0.0f, 1.0f)
			, backcolor_(0.0f, 0.0f, 0.0f, 0.5f)
		{
			graphChangedCallback_ = new GraphChangedCallback(this);
			profileCalculator_->addChangedCallback(graphChangedCallback_.get());

			float textSize = 8;
			osg::ref_ptr< osgText::Font> font = osgEarth::Registry::instance()->getDefaultFont();

			osg::Vec4 textColor = osg::Vec4f(1, 0, 0, 1);

			distanceMinLabel_ = new osgText::Text();
			distanceMinLabel_->setCharacterSize(textSize);
			distanceMinLabel_->setFont(font.get());
			distanceMinLabel_->setAlignment(osgText::TextBase::LEFT_BOTTOM);
			distanceMinLabel_->setColor(textColor);

			distanceMaxLabel_ = new osgText::Text();
			distanceMaxLabel_->setCharacterSize(textSize);
			distanceMaxLabel_->setFont(font.get());
			distanceMaxLabel_->setAlignment(osgText::TextBase::RIGHT_BOTTOM);
			distanceMaxLabel_->setColor(textColor);

			elevationMinLabel_ = new osgText::Text();
			elevationMinLabel_->setCharacterSize(textSize);
			elevationMinLabel_->setFont(font.get());
			elevationMinLabel_->setAlignment(osgText::TextBase::RIGHT_BOTTOM);
			elevationMinLabel_->setColor(textColor);

			elevationMaxLabel_ = new osgText::Text();
			elevationMaxLabel_->setCharacterSize(textSize);
			elevationMaxLabel_->setFont(font.get());
			elevationMaxLabel_->setAlignment(osgText::TextBase::RIGHT_TOP);
			elevationMaxLabel_->setColor(textColor);
		}

		~TerrainProfileGraph()
		{
			profileCalculator_->removeChangedCallback(graphChangedCallback_.get());
		}

		void setTerrainProfile(const osgEarth::Util::TerrainProfile& profile)
		{
			profile_ = profile;
			redraw();
		}

		void redraw()
		{
			removeChildren(0, getNumChildren());

			addChild(createBackground(graphWidth_, graphHeight_, backcolor_));

			osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
			geom->setUseVertexBufferObjects(true);

			osg::ref_ptr<osg::Vec3Array> verts = new osg::Vec3Array();
			verts->reserve(profile_.getNumElevations());
			geom->setVertexArray(verts);
			if (verts->getVertexBufferObject())
				verts->getVertexBufferObject()->setUsage(GL_STATIC_DRAW_ARB);

			osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array();
			colors->push_back(color_);
			geom->setColorArray(colors);
			geom->setColorBinding(osg::Geometry::BIND_OVERALL);

			double minElevation, maxElevation;
			profile_.getElevationRanges(minElevation, maxElevation);
			double elevationRange = maxElevation - minElevation;

			double totalDistance = profile_.getTotalDistance();

			for (unsigned int i = 0; i < profile_.getNumElevations(); i++)
			{
				double distance = profile_.getDistance(i);
				double elevation = profile_.getElevation(i);

				double x = (distance / totalDistance) * graphWidth_;
				double y = ((elevation - minElevation) / elevationRange) * graphHeight_;
				verts->push_back(osg::Vec3(x, y, 0));
			}

			geom->addPrimitiveSet(new osg::DrawArrays(GL_LINE_STRIP, 0, verts->size()));
			osg::ref_ptr<osg::Geode> graphGeode = new osg::Geode;
			graphGeode->addDrawable(geom);
			addChild(graphGeode);

			osg::ref_ptr<osg::Geode> labelGeode = new osg::Geode;
			labelGeode->addDrawable(distanceMinLabel_.get());
			labelGeode->addDrawable(distanceMaxLabel_.get());
			labelGeode->addDrawable(elevationMinLabel_.get());
			labelGeode->addDrawable(elevationMaxLabel_.get());

			distanceMinLabel_->setPosition(osg::Vec3(0, 0, 0));
			distanceMaxLabel_->setPosition(osg::Vec3(graphWidth_ - 15, 0, 0));
			elevationMinLabel_->setPosition(osg::Vec3(graphWidth_ - 5, 10, 0));
			elevationMaxLabel_->setPosition(osg::Vec3(graphWidth_ - 5, graphHeight_, 0));

			distanceMinLabel_->setText("0m");
			distanceMaxLabel_->setText(osgEarth::toString<int>((int)totalDistance) + std::string("m"));
			elevationMinLabel_->setText(osgEarth::toString<int>((int)minElevation) + std::string("m"));
			elevationMaxLabel_->setText(osgEarth::toString<int>((int)maxElevation) + std::string("m"));

			addChild(labelGeode);
		}

		osg::Node* createBackground(double width, double height, const osg::Vec4f& backgroundColor)
		{
			osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();
			geometry->setUseVertexBufferObjects(true);

			osg::ref_ptr<osg::Vec3Array> verts = new osg::Vec3Array();
			verts->reserve(4);
			verts->push_back(osg::Vec3(0, 0, 0));
			verts->push_back(osg::Vec3(width, 0, 0));
			verts->push_back(osg::Vec3(width, height, 0));
			verts->push_back(osg::Vec3(0, height, 0));
			geometry->setVertexArray(verts);
			if (verts->getVertexBufferObject())
				verts->getVertexBufferObject()->setUsage(GL_STATIC_DRAW_ARB);

			osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array();
			colors->push_back(backgroundColor);
			geometry->setColorArray(colors);
			geometry->setColorBinding(osg::Geometry::BIND_OVERALL);
			geometry->addPrimitiveSet(new osg::DrawArrays(GL_QUADS, 0, 4));

			osg::ref_ptr<osg::Geode> geode = new osg::Geode;
			geode->addDrawable(geometry);

			return geode;
		}

		osg::ref_ptr<osgText::Text> distanceMinLabel_;
		osg::ref_ptr<osgText::Text> distanceMaxLabel_;
		osg::ref_ptr<osgText::Text> elevationMinLabel_;
		osg::ref_ptr<osgText::Text> elevationMaxLabel_;

		osg::Vec4f backcolor_;
		osg::Vec4f color_;
		osgEarth::Util::TerrainProfile profile_;
		osg::ref_ptr< osgEarth::Util::TerrainProfileCalculator > profileCalculator_;
		double graphWidth_;
		double graphHeight_;
		osg::ref_ptr< GraphChangedCallback > graphChangedCallback_;
	};

	terrainProfileAnalysisCallback::terrainProfileAnalysisCallback(CommonPickTool*cpt, osgEarth::MapNode* mapNode)
		:CommonPickTool::Callback(mapNode, cpt)
	{
		addTemptModelLayer("terrain_profile_tempt_layer");
		cpt_->setOnlyPickTerrain(true);
		cpt_->setActiveMouseMoving(false);
		pickIndex_ = 0;
	}


	void terrainProfileAnalysisCallback::mousePick(const osg::Vec3d &point)
	{
		osgEarth::GeoPoint geop(mapNode_->getMapSRS()->getGeographicSRS(), point[0], point[1], point[2]);
		if (pickIndex_ == 0)
		{
			startPos_ = geop;
			pickIndex_++;
			return;
		}
		else if (pickIndex_ == 1)
		{
			endPos_ = geop;

			compute(startPos_, endPos_);
			profileRoot_ = new TerrainProfileGraph(tpc_.get(), 200, 100);
			pickIndex_ = 0;
			return;
		}
		else return;
	}

	void terrainProfileAnalysisCallback::addTemptModelLayer(const std::string& name)
	{
		tpc_ = new osgEarth::Util::TerrainProfileCalculator(mapNode_);
		profileRoot_ = new osg::Group;
		osg::ref_ptr<osg::Camera> hudCamera = createHud(500, 500);
		hudCamera->addChild(profileRoot_);

		/*osg::ref_ptr<osg::Group> */root_ = new osg::Group;
		root_->addChild(hudCamera);

		temptLayer_ = new osgEarth::ModelLayer(name, root_);
		mapNode_->getMap()->addModelLayer(temptLayer_);
	}

	void terrainProfileAnalysisCallback::compute(const osgEarth::GeoPoint& s, const osgEarth::GeoPoint& e)
	{
		tpc_->setStartEnd(s, e);

		osg::ref_ptr<osgEarth::LineString> line = new osgEarth::LineString();
		line->push_back(s.vec3d());
		line->push_back(e.vec3d());

		osgEarth::Style style;
		osg::ref_ptr<osgEarth::Symbology::LineSymbol> ls = style.getOrCreateSymbol<osgEarth::Symbology::LineSymbol>();
		ls->stroke()->color() = osgEarth::Color::Yellow;
		ls->stroke()->width() = 2.0f;
		ls->tessellation() = 20;

		style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
		style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_SCENE;

		osg::ref_ptr<osgEarth::Features::Feature> feature = new osgEarth::Features::Feature(line, mapNode_->getMapSRS());
		feature->geoInterp() = osgEarth::GEOINTERP_GREAT_CIRCLE;
		feature->style() = style;

		if (featureNode_.valid())
		{
			root_->removeChild(featureNode_.get());
			featureNode_ = 0;
		}
		featureNode_ = new osgEarth::Annotation::FeatureNode(mapNode_, feature);
		featureNode_->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
		root_->addChild(featureNode_);
	}
}
