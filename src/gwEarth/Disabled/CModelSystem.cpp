#include <gwEarth/CModelSystem.h>
#include <gwEarth/CModelLayer.h>
#include <gwBase/IRender.h>
#include <osgEarth/ModelLayer>
#include <osgEarth/OverlayDecorator>
#include <osgEarthFeatures/FeatureSource>
#include <osgEarthFeatures/FeatureDisplayLayout>

#include <osgEarthDrivers/feature_wfs/WFSFeatureOptions>
#include <osgEarthDrivers/model_feature_geom/FeatureGeomModelOptions>
#include <osgEarthDrivers/model_simple/SimpleModelOptions>

#include <gwDrivers/modelConfig/WjhConfigOptions.h>
#include <gwDrivers/model/WjhModelOptions.h>

typedef void(gwEarth::CModelSystem::*method_f)(const osgEarth::Config& arg);

namespace gwEarth
{
	class LayerOperation :public osg::Operation
	{
	public:
		LayerOperation(CModelSystem*ts, method_f method, const osgEarth::Config& arg)
			: _ts(ts)
			, _method(method)
			, _arg(arg)
		{ }
		~LayerOperation() { }

		virtual void operator()(osg::Object*)
		{
			(_ts->*_method)(_arg);
		}
	private:
		CModelSystem*_ts;
		method_f _method;
		const osgEarth::Config _arg;
	};

	bool getFeatureSourceOptions(const osgEarth::Config& args, osgEarth::Features::FeatureSourceOptions& out_opt)
	{
		std::string type = args.value("type");
		if (type.empty()) return false;

		std::string url = args.value("url");
		if (url.empty()) return false;

		if (type == "wfs")
		{
			std::string type_name = args.value("type_name");
			std::string out_format = args.value("format", std::string("JSON"));

			osgEarth::Drivers::WFSFeatureOptions opt;
			opt.url() = url;
			opt.typeName() = type_name;
			opt.outputFormat() = out_format;

			out_opt = opt;
			return true;
		}
		return false;
	}

	bool getModelSourceOptions(const osgEarth::Config& args, osgEarth::ModelSourceOptions& out_opt)
	{
		std::string type = osgDB::convertToLowerCase(args.value("type"));
		if (type.empty()) return false;

		if (type == "simple")
		{
			std::string url = args.value("url");// getStringValue("url");
			if (url.empty()) return false;

			osgEarth::Drivers::SimpleModelOptions opt;
			opt.url() = url;
			out_opt = opt;
			return true;
		}
		else if (type == "feature_geom")
		{
			osgEarth::Features::FeatureSourceOptions fsoptions;
			if (!getFeatureSourceOptions(args, fsoptions))
			{
				return false;
			}

			std::string style_name = args.value("style_name", std::string("my_type_name"));

			osgEarth::Features::Style style;
			style.setName(style_name);

			std::string symbolType = osgDB::convertToLowerCase(args.value("symbol_type"));
			if (symbolType.empty()) return false;

			if (symbolType == "line")
			{
				float width = args.value("line_width", 3.0f);
				osg::ref_ptr<osgEarth::Features::LineSymbol> linesymbol = style.getOrCreateSymbol<osgEarth::Features::LineSymbol>();
				linesymbol->stroke()->color() = osgEarth::Symbology::Color(1.0, 1.0, 0.0, 1.0);//::Yellow;
				linesymbol->stroke()->width() = width;

				osg::ref_ptr<osgEarth::Symbology::AltitudeSymbol> altisymbol = style.getOrCreateSymbol<osgEarth::Symbology::AltitudeSymbol>();
				altisymbol->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
				altisymbol->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_DRAPE;
			}
			else if (symbolType == "point")
			{
				osgEarth::Features::PointSymbol* pointsymbol = style.getOrCreateSymbol<osgEarth::Features::PointSymbol>();
				pointsymbol->size() = 16.0f;

			}
			else if (symbolType == "polygon")
			{
				osgEarth::Features::PolygonSymbol* polygonsymbol = style.getOrCreateSymbol<osgEarth::Features::PolygonSymbol>();
			}
			else if (symbolType == "model")
			{
				std::string url = args.value("url");
				if (url.empty()) return false;

				osgEarth::Features::ModelSymbol* modelsymbol = style.getOrCreateSymbol<osgEarth::Features::ModelSymbol>();
				modelsymbol->url() = url;
			}
			else if (symbolType == "marker")
			{
				osgEarth::Features::MarkerSymbol* markersymbol = style.getOrCreateSymbol<osgEarth::Features::MarkerSymbol>();
			}
			else if (symbolType == "text")
			{
				std::string contentname = args.value("content");
				osgEarth::Features::TextSymbol* text = style.getOrCreateSymbol<osgEarth::Features::TextSymbol>();
				text->content() = osgEarth::Symbology::StringExpression("[" + contentname + "]");
				text->declutter() = true;
				text->size() = 16.0f;
				text->alignment() = osgEarth::Symbology::TextSymbol::ALIGN_CENTER_CENTER;
				text->fill()->color() = osgEarth::Symbology::Color::Yellow;
				text->halo()->color() = osgEarth::Symbology::Color::DarkGray;
				text->font() = "simhei.ttf";//指定中文字体路径;
				text->encoding() = osgEarth::Symbology::TextSymbol::ENCODING_UTF8;
			}

			float minrange = args.value("min_range", 0.0f);
			float maxrange = args.value("max_range", (float)FLT_MAX);
			float sizefactor = args.value("size_factor", (int)15);
			osgEarth::Features::FeatureDisplayLayout layout;
			layout.tileSizeFactor() = sizefactor;
			layout.addLevel(osgEarth::Features::FeatureLevel(minrange, maxrange, style_name));

			osgEarth::Drivers::FeatureGeomModelOptions opt;
			opt.featureOptions() = fsoptions;
			opt.styles() = new osgEarth::Features::StyleSheet();
			opt.styles()->addStyle(style);
			opt.layout() = layout;
			out_opt = opt;
			return true;
		}
		else if (type == "wjh")
		{
			gwDrivers::WJHConfigOptions configoption;
			configoption.url() = osgEarth::URI(args.value("config_url"));

			osgEarth::Features::FeatureDisplayLayout layout;
			layout.tileSize() = 90.0;
			layout.addLevel(osgEarth::Features::FeatureLevel(0.0f, 1500.0f, "buildings"));

			osgEarth::Style buildingStyle;
			buildingStyle.setName("buildings");

			osgEarth::StyleSheet* styleSheet = new osgEarth::StyleSheet();
			styleSheet->addStyle(buildingStyle);

			osgEarth::Symbology::AltitudeSymbol* alt = buildingStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>();
			alt->clamping() = alt->CLAMP_TO_TERRAIN;
			alt->binding() = alt->BINDING_VERTEX;

			gwDrivers::WJHModelOptions modeloptions;
			modeloptions.modelUrl() = osgEarth::URI(args.value("model_url"));
			modeloptions.featureOptions() = configoption;
			modeloptions.layout() = layout;
			modeloptions.styles() = styleSheet;
			modeloptions.isMemCache() = true;
			modeloptions.memCacheSize() = 2000;
			//std::string modelurl = args.value("model_url");
			out_opt = modeloptions;
			return true;
		}
		//else if (type =="gw_pipeline")
		//{

		//}
		return false;
	}


	//////////////////////////////////////////////////////////////////////////
	CModelSystem::CModelSystem(gwApp::IApplication* app) : ICModelSystem(app)
	{ }

	CModelSystem::~CModelSystem() { }

	// base system method
	void CModelSystem::init() { }
	void CModelSystem::shutdown() { }

	void CModelSystem::update(double dt)
	{
		fireCallback(dt, &ICModelSystemCallback::onUpdate);
	}

	void CModelSystem::initSystemFromConfig(const osgEarth::Config & systemConfig)
	{

	}

	void CModelSystem::addLayer(const osgEarth::Config & layerconfig)
	{
		if (!_map) return;

		osgEarth::ModelSourceOptions opt;
		if (!getModelSourceOptions(layerconfig, opt)) return;

		std::string layername = layerconfig.value("layer_name");
		addLayer(new CModelLayer(layername, opt));
	}

	void CModelSystem::removeLayer(const std::string& layername)
	{
		removeLayer(getLayer(layername));
	}

	void CModelSystem::removeAllLayers()
	{
		ICModelLayerVector layers;
		IModelSystem::getLayers(layers);
		for (ICModelLayerVector::iterator it = layers.begin(); it != layers.end(); ++it)
		{
			removeLayer(*it);
		}
	}

	void CModelSystem::addLayer(ICModelLayer * layer)
	{
		if (IModelSystem::addLayer(layer))
		{
			fireCallback(layer, &ICModelSystemCallback::onLayerAdded);
		}
	}

	void CModelSystem::removeLayer(ICModelLayer * layer)
	{
		if (IModelSystem::removeLayer(layer))
		{
			fireCallback(layer, &ICModelSystemCallback::onLayerRemoved);
		}
	}

	ICModelLayer* CModelSystem::getLayer(const std::string& layername) const
	{
		return IModelSystem::getLayer<ICModelLayer>(layername);
	}

	void CModelSystem::getLayers(ICModelLayerVector& layers) const
	{
		IModelSystem::getLayers(layers);
	}

	osg::Node* CModelSystem::getLayerNode(const std::string& layername) const
	{
		return _mapNode->getModelLayerNode(getLayer(layername));
	}

	void CModelSystem::queryAnalysis()
	{
	}

	//void CModelSystem::addLayerMethod(const osgEarth::Config & arg)
	//{
	//	if (!_app || !_app->getMap()) return;

	//	osgEarth::ModelSourceOptions opt;
	//	if (!getModelSourceOptions(arg, opt))
	//	{
	//		return;
	//	}

	//	std::string layername = arg.value("layer_name");
	//	if (layername.empty()) { /*GW_LOG()*/ }

	//	osg::ref_ptr<ICModelLayer> layer = new CModelLayer(opt);
	//	_app->getMap()->beginUpdate();
	//	_app->getMap()->addModelLayer(layer);
	//	_app->getMap()->endUpdate();

	//	ICModelSystemCallbackVector::const_iterator it;
	//	for (it = _callbacks.begin(); it != _callbacks.end(); ++it)
	//	{
	//		if (!(*it) || (*it) == NULL) continue;
	//		(*it)->onLayerAdded(layer);
	//	}
	//}

	//void CModelSystem::removeLayerMethod(const osgEarth::Config & arg)
	//{
	//	ICModelLayer* layer = getLayer(arg.value("layer_name"));
	//	if (!layer) return;

	//	//osg::Node* layernode = _app->getMapNode()->getModelLayerNode(layer);
	//	//if (_sceneRoot)
	//	//{
	//	//	_sceneRoot->removeChild(layernode);
	//	//}

	//	_app->getMap()->beginUpdate();
	//	_app->getMap()->removeModelLayer(layer);
	//	_app->getMap()->endUpdate();
	//	ICModelSystemCallbackVector::const_iterator it;
	//	for (it = _callbacks.begin(); it != _callbacks.end(); ++it)
	//	{
	//		if (!(*it) || (*it) == NULL) continue;
	//		(*it)->onLayerRemoved(layer);
	//	}
	//}
}
