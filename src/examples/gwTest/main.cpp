#include <compiler.h>
#include <gwApp/Application.h>
#include <gwEarth/Render.h>
#include <gwEarth/CameraManipulatorManager.h>
#include <gwEarth/TerrainSystem.h>
#include <gwEarth/ModelSystem.h>
#include <gwDrivers/modelConfig/WjhConfigOptions.h>
#include <gwDrivers/model/WjhModelOptions.h>

#include <osgEarthUtil/Sky>
#include <osgEarthUtil/EarthManipulator>
#include <osgEarthDrivers/tms/TMSOptions>

#include <osgGA/StateSetManipulator>
#include <osgViewer/Viewer>
#include <osgViewer/CompositeViewer>
#include <osgViewer/ViewerEventHandlers>

#include <gwEarth/ModelSystem.h>
#include <gwEarth/ModelLayer.h>
#include <gwDrivers/model/WjhModelOptions.h>
#include <osgEarthDrivers/feature_ogr/OGRFeatureOptions>
#include <gwDrivers/model_feature_solid_geom/SolidFeatureGeomModelOptions.h>
#include <osgEarthSymbologyExt/SolidLineSymbol.h>
#include <osgEarthSymbologyExt/SolidJointSymbol.h>
#include <osgEarthFeatures/FeatureDisplayLayout>

#include <gwApp/Util.h>
#include <gwEarth/TerrainUtil.h>
#include <gwEarth/TerrainSystem.h>
#include <gwDrivers/model_tiled/TiledModelOptions.h>
#include <gwDrivers/geniusworld_model/GeniusWorldModelOptions.h>

using namespace gwApp;
using namespace gwEarth;

#include <osg/io_utils>
#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>

#define LAYER_NAME "layer_name"
#define DATA_TYPE  "type"
#define URL        "url"

//WMS para
#define LAYERS "layers"
#define FORMAT "format"
#define SRS    "srs"
#define STYLES "styles"

//WFS para
//osgEarth::Config CreateTmsConfig(const std::string& layername, const std::string&url)
//{
//	osgEarth::Config arg;
//	arg.add(LAYER_NAME, layername);
//	arg.add(DATA_TYPE, "tms");
//	arg.add(URL, url);
//	return arg;
//}

osgEarth::Config CreateWmsConfig(const std::string& layername
	, const std::string&url
	, const std::string& layers
	, const std::string& format
	, const std::string& srs
	, const std::string& styles)
{
	osgEarth::Config arg;
	arg.add(LAYER_NAME, layername);
	arg.add(DATA_TYPE, "wms");
	arg.add(URL, url);
	arg.add(LAYERS, layers);
	arg.add(FORMAT, format);
	arg.add(SRS, srs);
	arg.add(STYLES, styles);
	return arg;
}

osgEarth::Config createWJHConfig(const std::string& layername, const std::string& configname, const std::string& modelpath)
{
	osgEarth::Config wjhmodelconfig;
	wjhmodelconfig.add("type", "wjh");
	wjhmodelconfig.add("layer_name", layername);
	wjhmodelconfig.add("config_url", configname);
	wjhmodelconfig.add("model_url", modelpath);
	return wjhmodelconfig;
}

gwDrivers::WJHModelOptions createWJHModelOptions(const osgEarth::Config& args)
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
	return modeloptions;
}

//void addWJHModel(gwApp::IApplication* app)
//{
//	osgEarth::Config wjhmodelconfig = createWJHConfig("wjh_model", "D:/Data/LFSJ/building.xml", "D:/Data/LFSJ");
//	app->getModelSystem("CommonModelSystem")->addLayer(wjhmodelconfig);
//
//	osgEarth::Config wjhgroundconfig = createWJHConfig("wjh_ground", "D:/Data/LFSJ/ground.xml", "D:/Data/LFSJ");
//	app->getModelSystem("CommonModelSystem")->addLayer(wjhgroundconfig);
//
//	osgEarth::Config wjhroadconfig = createWJHConfig("wjh_road", "D:/Data/LFSJ/road.xml", "D:/Data/LFSJ");
//	app->getModelSystem("CommonModelSystem")->addLayer(wjhroadconfig);
//}

class SetViewPortEH :public osgGA::GUIEventHandler
{
public:
	SetViewPortEH(gwApp::Application* app)
		: _app(app)
	{}
	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
	{
		switch (ea.getEventType())
		{
		case osgGA::GUIEventAdapter::KEYDOWN:
		{
			if (ea.getKey() == osgGA::GUIEventAdapter::KEY_1)
			{
				osgGA::CameraManipulator* cm =
					_app->getCameraManipulatorManager()->getCameraManipulator(EARTH);
				osgEarth::Util::EarthManipulator* em = dynamic_cast<osgEarth::Util::EarthManipulator*>(cm);
				if (em)
				{
					em->setViewpoint(osgEarth::Viewpoint("Home", 116.688869, 39.52654758, 0, 24.261, -21.6, 3450.0), 8.0);
				}
			}
			else if (ea.getKey() == osgGA::GUIEventAdapter::KEY_2)
			{
				osgGA::CameraManipulator* cm =
					_app->getCameraManipulatorManager()->getCameraManipulator(EARTH);
				osgEarth::Util::EarthManipulator* em = dynamic_cast<osgEarth::Util::EarthManipulator*>(cm);
				if (em)
				{
					em->setViewpoint(osgEarth::Viewpoint("LL", 119.98884582519531, 35.870365142822266, 0, 24.261, -21.6, 3450.0), 8.0);
				}
			}
			else if (ea.getKey() == osgGA::GUIEventAdapter::KEY_3)
			{
				osgGA::CameraManipulator* cm =
					_app->getCameraManipulatorManager()->getCameraManipulator(EARTH);
				osgEarth::Util::EarthManipulator* em = dynamic_cast<osgEarth::Util::EarthManipulator*>(cm);
				if (em)
				{
					em->setViewpoint(osgEarth::Viewpoint("Tiled", 113.06277, 22.64785, 0, 24.261, -21.6, 3450.0), 8.0);
				}
			}
			else if (ea.getKey() == osgGA::GUIEventAdapter::KEY_4)
			{
				osgGA::CameraManipulator* cm =
					_app->getCameraManipulatorManager()->getCameraManipulator(EARTH);
				osgEarth::Util::EarthManipulator* em = dynamic_cast<osgEarth::Util::EarthManipulator*>(cm);
				if (em)
				{
					em->setViewpoint(osgEarth::Viewpoint("Rainier", -121.7706234748925, 46.84187674081022, -0.002506599761545658, -27.566, -27.566, 38701.6), 8.0);
				}
			}
			else if (ea.getKey() == osgGA::GUIEventAdapter::KEY_5)
			{
				osgGA::CameraManipulator* cm =
					_app->getCameraManipulatorManager()->getCameraManipulator(EARTH);
				osgEarth::Util::EarthManipulator* em = dynamic_cast<osgEarth::Util::EarthManipulator*>(cm);
				if (em)
				{
					em->setViewpoint(osgEarth::Viewpoint("GeniusWorld", 116.66107177734375, 40.042419433593743, 0.00000000000000000, -27.566, -27.566, 38701.6), 8.0);
				}
			}
			else if (ea.getKey() == osgGA::GUIEventAdapter::KEY_5)
			{
				TerrainSystemPtr terrainSys = _app->getOrCreateSubsystem<TerrainSystem>("TerrainSystem", gwBase::SubsystemMgr::EARTH);
				terrainSys->addElevationLayer(TerrainUtil::createFeatureElevationLayer("dd", ""));
			}
			break;
		}
		default:
			break;
		}
		return false;
	}

private:
	gwApp::Application* _app;
};

//class noop_logger : public mongocxx::logger
//{
//public:
//	virtual void operator()(mongocxx::log_level, bsoncxx::stdx::string_view, bsoncxx::stdx::string_view) noexcept {}
//};

#include <osg/ShapeDrawable>

osg::Node* makeCylinder()
{
	osg::ref_ptr<osg::Cylinder> cylinder = new osg::Cylinder();
	osg::ref_ptr<osg::ShapeDrawable> sd = new osg::ShapeDrawable(cylinder);
	//sd->setColor(osg::Vec4(1, 0, 0, 1));

	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	geode->addDrawable(sd);

	osg::ref_ptr<osg::Image> image = osgDB::readImageFile("D:/BYL/gwEarth/gwEarth2017.09.09/data/Images/walnut.jpg");
	if (image && image != NULL)
	{
		osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
		texture->setImage(image);

		if (texture)
		{
			geode->getOrCreateStateSet()->setTextureAttributeAndModes(0, texture, osg::StateAttribute::ON);
		}
	}
	return geode.release();
}



class TiledDirectoryVisitor
{
public:
	TiledDirectoryVisitor(osg::Group* root) :_root(root) {}

	virtual bool handleDir(const std::string& path) { return true; }
	virtual void traverse(const std::string& path)
	{
		osgDB::DirectoryContents files = osgDB::getDirectoryContents(path);
		for (osgDB::DirectoryContents::const_iterator f = files.begin(); f != files.end(); ++f)
		{
			if (f->compare(".") == 0 || f->compare("..") == 0)
				continue;

			std::string filepath = osgDB::concatPaths(path, *f);

			osgDB::DirectoryContents subFiles = osgDB::getDirectoryContents(filepath);
		}
	}

private:
	osg::Group* _root;
};

#include <gwBase/SubsystemMgr.h>
//
std::string readyMapImageUrl = "http://readymap.org/readymap/tiles/1.0.0/22/";/*//"E:/DATA/readymap/readymap_imagery/tms.xml"; "D:/Data/readymap_imagery/tms.xml";*/
std::string elevationUrl = "http://readymap.org/readymap/tiles/1.0.0/116/";/*// "E:/DATA/readymap/readymap_elevation/tms.xml";// "D:/Data/readymap_elevation/readymap_elevation/tms.xml";*/
std::string weihaiImageUrl = "E:/DATA/weihai_images/weihai/tms.xml";
std::string arcgisImage = "http://services.arcgisonline.com/arcgis/rest/services/World_Imagery/MapServer/";
std::string osmImageUrl = "http://[abc].tile.openstreetmap.org/{z}/{x}/{y}.png";
std::string googleImageUrl = "http://mt[0123].google.cn/vt/lyrs=m&amp;hl=zh-CN&amp;gl=cn&amp;x={x}&amp;y={y}&amp;z={z}";
std::string modelUrl = /*"D:/Data/OSGB8CM/Production_3/Data"; //*/ "D:/BYL/ive/ZZZ8CM/Production_3/Data";
std::string gaodeUrl = "http://webrd0[1234].is.autonavi.com/appmaptile?lang=zh_cn&amp;size=1&amp;scale=1&amp;style=7&amp;x={x}&amp;y={y}&amp;z={z}";
std::string baiduUrl = "http://shangetu[012356789].map.bdimg.com/it/u=x={x-Math.floor(Math.pow(2,z)/2.0)};y={y-Math.floor(Math.pow(2,z)/2.0)};z={z+1};v=009;type=sate&amp;fm=46&amp;app=webearth2&amp;v=009&amp;udt=20171031";
std::string tianditu = "http://t3.tianditu.cn/img_c/wmts?service=wmts&request=GetTile&version=1.0.0&LAYER=img&tileMatrixSet=c&TileMatrix={z}&TileRow={y}&TileCol={x}&style=default&format=tiles";

//#include <gwDrivers/geniusworld_model/GeniusWorldModelOptions.h>
std::string qiaofengurl = "http://127.0.0.1:9080/type=1&x={x}&y={y}&z={z}";
std::string qiaofengurl1 = "http://127.0.0.1:9080/type=30&x={x}&y={y}&z={z}";
int main()
{
	gwBase::SubsystemMgr::GroupType earthType = gwBase::SubsystemMgr::EARTH;

	gwApp::ApplicationPtr app = gwApp::Application::instance();

	TerrainSystemPtr terrainSys = app->getOrCreateSubsystem<TerrainSystem>("TerrainSystem", earthType);
	terrainSys->addImageLayer(TerrainUtil::createXYZImageLayer("qiaofeng", qiaofengurl, "global-geodetic", false));
	terrainSys->addImageLayer(TerrainUtil::createXYZImageLayer("qiaofeng1", qiaofengurl1, "global-geodetic", false));

	//terrainSys->addImageLayer(TerrainUtil::createTMSImageLayer("ReadyMapImg", readyMapImageUrl));
	//terrainSys->addImageLayer(TerrainUtil::createGDALImageLayer("WeiHai", "E:/DATA/weihai_images/weihai_wgs84.tif"));
	//terrainSys->addImageLayer(TerrainUtil::createArcgisImageLayer("ArcGisImage", arcgisImage));
	//terrainSys->addImageLayer(TerrainUtil::createXYZImageLayer("OSM", osmImageUrl, "spherical-mercator"));
	//terrainSys->addImageLayer(TerrainUtil::createTMSImageLayer("WeiHai", weihaiImageUrl));
	//terrainSys->addImageLayer(TerrainUtil::createXYZImageLayer("Google", googleImageUrl));
	//terrainSys->addImageLayer(TerrainUtil::createXYZImageLayer("GoaDe", gaodeUrl, "global-mercator", false));
	//terrainSys->addImageLayer(TerrainUtil::createXYZImageLayer("baidu", baiduUrl, "global-mercator", false));
	//terrainSys->addImageLayer(TerrainUtil::createXYZImageLayer("TianDiTu", tianditu, "spherical-mercator", false));
	//terrainSys->addElevationLayer(TerrainUtil::createTMSElevationLayer("ReadyMapEle", elevationUrl));
	//terrainSys->addElevationLayer(TerrainUtil::createFeatureElevationLayer("dd", ""));

	ModelSystemPtr modelSys = app->getOrCreateSubsystem<ModelSystem>("TiledModelSystem", earthType);
	gwDrivers::TiledModelOptions tiledOptions;
	tiledOptions.url() = osgEarth::URI(modelUrl);
	tiledOptions.location() = osg::Vec3d(113.06277, 22.64785, 0.0);
	modelSys->addLayer(new osgEarth::ModelLayer(tiledOptions));

	ModelSystemPtr gwModelSys = app->getOrCreateSubsystem<ModelSystem>("GeniusWorldModelSystem", earthType);
	gwDrivers::GeniusWorldModelOptions gwoption;
	gwModelSys->addLayer(new osgEarth::ModelLayer(gwoption));












	//gwDrivers::WJHModelOptions buildingOptions = createWJHModelOptions(createWJHConfig("wjh_building", "D:/DATA/LFSJ_OSGB_NZ/building.xml", "http://192.168.1.154:8010/wjh_model/"));
	//gwDrivers::WJHModelOptions groundOptions = createWJHModelOptions(createWJHConfig("wjh_ground", "D:/DATA/LFSJ_OSGB_NZ/ground.xml", "http://192.168.1.154:8010/wjh_model/"));
	//gwDrivers::WJHModelOptions roadOptions = createWJHModelOptions(createWJHConfig("wjh_road", "D:/DATA/LFSJ_OSGB_NZ/road.xml", "http://192.168.1.154:8010/wjh_model/"));
	//gwEarth::ModelSystem<>* modelsys = app->getSystem<gwEarth::ModelSystem<> >("model_system");
	//if (modelsys)
	//{
	//	//modelsys->addLayer(new osgEarth::ModelLayer(buildingOptions));
	//	//modelsys->addLayer(new osgEarth::ModelLayer(groundOptions));
	//	//modelsys->addLayer(new osgEarth::ModelLayer(roadOptions));

	//	osgEarth::Drivers::OGRFeatureOptions ogroptions;
	//	ogroptions.name() = "pipeline_test";
	//	ogroptions.url() = osgEarth::URI("D:/Data/SHPE/JN_DLL.shp");//WGS84

	//	osgEarth::Style pipelineStyle;
	//	pipelineStyle.setName("pipeline_test1");

	//	osg::ref_ptr< gwUtil::SolidLineSymbol> solidlinesym = pipelineStyle.getOrCreate<gwUtil::SolidLineSymbol>();
	//	solidlinesym->setUnidSolidLineModel(makeCylinder());

	//	osgEarth::Features::FeatureDisplayLayout layout;
	//	layout.tileSizeFactor() = 3.0f;
	//	layout.maxRange() = 2000.0f;

	//	gwDrivers::SolidFeatureGeomModelOptions solidmodeloptions;
	//	solidmodeloptions.featureOptions() = ogroptions;
	//	solidmodeloptions.layout() = layout;
	//	solidmodeloptions.styles() = new osgEarth::StyleSheet();
	//	solidmodeloptions.styles()->addStyle(pipelineStyle);

	//	modelsys->addLayer(new osgEarth::ModelLayer(solidmodeloptions));
	//}

	//app->attatchSky();
	app->getRender()->setWindowSize(30, 30, 1600, 900);
	app->getRender()->getViewer()->addEventHandler(new SetViewPortEH(app));
	app->getRender()->update();

	return 0;
}


//{
//
//	for (unsigned q = 0; q < 4; ++q)
//	{
//		TileKey child = key.createChildKey(q);
//		//_modelFactory->createTileModel( child, _frame, model[q] );
//		TestTileModelThread *p = new TestTileModelThread(q, this, child, model[q]);
//		DatabaseThreadList.push_back(p);
//		p->startThread();
//	}
//
//	//-------------------------- - TestTileModelThread类实现如下----------------------------------------
//	SingleKeyNodeFactory::TestTileModelThread::TestTileModelThread(int num, osg::ref_ptr<SingleKeyNodeFactory> nodefactory, const TileKey &key, osg::ref_ptr<TileModel> out_model)
//	{
//		m_nodefactory = nodefactory;
//		m_TileKey = key;
//		m_model = out_model;
//		m_q = num;
//	}
//	int SingleKeyNodeFactory::TestTileModelThread::cancel()
//	{
//		OpenThreads::Thread::YieldCurrentThread();
//		return 0;
//	}
//
//
//	void SingleKeyNodeFactory::TestTileModelThread::run() 
//	{
//		//double start = GetTickCount();
//		m_nodefactory->_modelFactory->createTileModel(m_TileKey, m_nodefactory->_frame, m_model);
//		m_nodefactory->_model[m_q] = m_model.release();
//		//double end = GetTickCount();
//		//printf("%s::%d:: key:%s,  time:: %lf, pthreadID: %0x \n", __FILE__, __LINE__, m_TileKey.str().c_str(), end - start, getThreadId());
//	}
//}