#ifndef GWEARTH_TERRAINSYSTEM_H
#define GWEARTH_TERRAINSYSTEM_H 1

#include <gwEarth/ITerrainSystem.h>

#include <osgEarth/Map>
#include <osgEarth/ImageLayer>
#include <osgEarth/ElevationLayer>
#include <osgEarth/TileSource>
#include <osgEarth/MaskLayer>

namespace gwEarth
{
	class TerrainSystem : public ITerrainSystem
	{
	public:
		explicit TerrainSystem(gwApp::IApplication* app);
		~TerrainSystem();

		// base system method
		virtual void init();
		virtual void shutdown();
		virtual void update(double dt);

		virtual void initSystemFromConfig(const osgEarth::Config& systemConfig);

		virtual void addImageLayer(const osgEarth::Config& layerconfig);
		virtual void insertImageLayer(const osgEarth::Config& layerconfig);
		virtual void removeImageLayer(const std::string& layerName);
		virtual void removeAllImageLayers() {}
		virtual void moveImageLayer(const std::string& layerName, int newIndex);

		virtual osgEarth::ImageLayer* getImageLayer(const std::string& layerName);
		virtual void getAllImageLayers(osgEarth::ImageLayerVector& imageLayers);

		virtual void setImageLayerOpcity(const std::string& layerName, float opcity);
		virtual float getImageLayerOpcity(const std::string& layerName);

		virtual void setImageLayerVisible(const std::string& layerName, bool visible);
		virtual bool getImageLayerVisible(const std::string& layerName);

		virtual void addImageLayerCallback(const std::string& layerName, osgEarth::ImageLayerCallback* icb);
		virtual void removeImageLayerCallback(const std::string& layerName, osgEarth::ImageLayerCallback* icb);

		virtual void setImageLayerEnable(const std::string& layerName, bool enable);
		virtual bool getImageLayerEnable(const std::string& layerName);

		//elevation
		virtual void addElevationLayer(const osgEarth::Config& layerconfig);
		virtual void moveElevationLayer(const std::string& layerName, int newIndex);
		virtual void removeElevationLayer(const std::string& layerName);
		virtual void removeAllElevationLayers();

		virtual osgEarth::ElevationLayer* getElevationLayer(const std::string& layerName);
		virtual void getAllElevationLayers(osgEarth::ElevationLayerVector& elevationLayers);

		virtual void setElevationLayerVisible(const std::string& layerName, bool visible);
		virtual bool getElevationLayerVisible(const std::string& layerName);

		virtual void addElevationLayerCallback(const std::string& layerName, osgEarth::ElevationLayerCallback* ecb);
		virtual void removeElevationLayerCallback(const std::string& layerName, osgEarth::ElevationLayerCallback* ecb);

		virtual bool getElevationLayerEnable(const std::string& layerName);

		virtual void addMaskLayer(osgEarth::MaskLayer* masklayer);
		virtual void removeMaskLayer(osgEarth::MaskLayer* masklayer);
		virtual osgEarth::MaskLayer* getMaskLayer(const std::string& layername);
		virtual void getMaskLayerVector(osgEarth::MaskLayerVector& mlv);


		virtual osg::Node* getTerrainNode();

		//virtual void addCallback(TerrainSystemCallback* tsc) {};// { _callbackList.push_back(tsc); }
		//virtual void removeCallback(TerrainSystemCallback* tsc) {};
		//OpenThreads::ReadWriteMutex& getRWMutex(){return terrainMutex_;}
		//private:
		//    osgEarth::ElevationLayerVector elevationLayerVector_;
		//    osgEarth::ImageLayerVector imageLayerVector_;
		//friend class LayerOperation;
	public:
		void addImageLayerMethod(const osgEarth::Config& arg);
		void moveImageLayerMethod(const osgEarth::Config& arg);
		void removeImageLayerMethod(const osgEarth::Config& arg);
		void insertImageLayerMethod(const osgEarth::Config& arg);

		void addElevationlayerMethod(const osgEarth::Config& arg);
		void moveElevationLayerMethod(const osgEarth::Config& arg);
		void removeElevationLayerMethod(const osgEarth::Config& arg);

		void addMaskLayerMethod(const osgEarth::Config& arg);
		void removeMaskLayerMethod(const osgEarth::Config& arg);
	};
}
#endif // GWEARTH_TERRAINSYSTEM_H
