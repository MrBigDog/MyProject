#include <gwEarth/Export.h>
//#include <gwEarth/CModelSystem.h>
#include <gwEarth/ModelSystem.h>
//#include <gwEarth/MyTerrainSystem.h>
//#include <gwEarth/TerrainSystem.h>
#include <gwEarth/ModelLayer.h>
#include <gwApp/IApplication.h>

extern "C" GWEARTH_EXPORT bool Login(gwApp::IApplication* app)
{
	//static gwEarth::TerrainSystem<> terSys("terrain_system", app);
	//terSys.addImageLayer(new osgEarth::ImageLayer(osgEarth::ImageLayerOptions()));
	//terSys.addElevationLayer(new osgEarth::ElevationLayer(osgEarth::ElevationLayerOptions()));
	//app->addSubsystem("TerrainSystem", &terSys, "earth", 0.0);

	//gwEarth::TerrainSystemPtr terptr = gwEarth::makeNewTerrainSystem("test_terrain", app);
	//gwEarth::ModelSystemPtr modelPtr = gwEarth::makeNewModelSystem("test_model", app);

	//gwEarth::ImageLayerPtr imageptr = gwEarth::makeNewImageLayer(osgEarth::ImageLayerOptions());
	//gwEarth::ModelLayerPtr modelptr = gwEarth::makeNewModelLayer(osgEarth::ModelLayerOptions());

	//static gwEarth::TerrainSystem terSys(app);
	//app->addSubsystem("TerrainSystem", &terSys, "earth", 0.0);

	//static gwEarth::CModelSystem cmodelSys(app);
	//cmodelSys.setKey("CommonModelSystem");
	//app->addSubsystem(cmodelSys.getKey(), &cmodelSys, "earth", 0.0);

	{
		gwEarth::ModelSystemPtr modelSys = gwEarth::makeNewModelSystem<>("model_system", app);
		app->addSubsystem(modelSys->getName(), modelSys, "earth", 0.0);



		//static gwEarth::ModelSystem<> modelSys("model_system", app);
		//modelSys.addCallback(new gwEarth::ModelSystem<>::Callback());
		//modelSys.addLayer(new osgEarth::ModelLayer("test", 0L));
		//modelSys.addLayer(new osgEarth::ModelLayer("test", 0L));

		//app->addSubsystem(modelSys.getName(), &modelSys, "earth", 0.0);
	}

	return true;
}