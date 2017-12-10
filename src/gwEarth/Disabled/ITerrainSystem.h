#ifndef GWEARTH_ITERRAINSYSTEM_H
#define GWEARTH_ITERRAINSYSTEM_H 1

#include <gwApp/IApplication.h>
#include <gwBase/ISubsystemMgr.h>

#include <osgEarth/MapNode>

#include <vector>

//namespace osgEarth {
//	class Config;
//	class ImageLayer;
//	class MaskLayer;
//	class ElevationLayer;
//	class ElevationLayerVector;
//	class MaskLayerVector;
//	class ImageLayerVector;
//	class ImageLayerCallback;
//	class ElevationLayerCallback;
//}

namespace gwEarth
{
	class ITerrainSystemCallback : public osg::Referenced
	{
	public:
		virtual void onImageLayerAdded(osgEarth::ImageLayer* layer) {}
		virtual void onImageLayerInserted(osgEarth::ImageLayer* layer, unsigned int newIndex) {}
		virtual void onImageLayerMoved(osgEarth::ImageLayer* layer, unsigned int newIndex) {}
		virtual void onImageLayerRemoved(osgEarth::ImageLayer* layer) {}
		virtual void onImageLayerVisibleChanged(osgEarth::ImageLayer* layer) {}
		virtual void onImageLayerOpcityChanged(osgEarth::ImageLayer* layer) {}
		virtual void onImageLayerEnablChanged(osgEarth::ImageLayer* layer) {}

		virtual void onElevationLayerAdded(osgEarth::ElevationLayer* layer) {}
		virtual void onElevationLayerMoved(osgEarth::ElevationLayer* layer, unsigned int newIndex) {}
		virtual void onElevationLayerRemoved(osgEarth::ElevationLayer* layer) {}
		virtual void onElevationLayerVisibleChanged(osgEarth::ElevationLayer* layer) {}
		virtual void onElevationLayerEnablChanged(osgEarth::ElevationLayer* layer) {}

		virtual void onMaskLayerAdded(osgEarth::MaskLayer* layer, unsigned int index) {}
		virtual void onMaskLayerRemoved(osgEarth::MaskLayer* layer, unsigned int index) {}
	};
	typedef std::vector<osg::ref_ptr<ITerrainSystemCallback> > TerrainSystemCallbackVector;


	class ITerrainSystem : public gwBase::ISubsystem
	{
	public:
		ITerrainSystem(gwApp::IApplication* app)
			: _app(app)
		{
			setKey("TERRAIN_SYSTEM");
		}

		virtual ~ITerrainSystem() {}

		void setKey(const std::string& key) { _key = key; }
		std::string getKey() { return _key; }

		//// base system method
		//virtual void init() = 0;
		//virtual void shutdown() = 0;
		//virtual void update(double dt) = 0;

		virtual void initSystemFromConfig(const osgEarth::Config& systemConfig) = 0;

		virtual void addImageLayer(const osgEarth::Config& layerconfig) = 0;
		virtual void insertImageLayer(const osgEarth::Config& layerconfig) = 0;
		virtual void removeImageLayer(const std::string& layerName) = 0;
		virtual void moveImageLayer(const std::string& layerName, int newIndex) = 0;
		virtual void removeAllImageLayers() = 0;

		virtual osgEarth::ImageLayer* getImageLayer(const std::string& layerName) = 0;
		virtual void getAllImageLayers(osgEarth::ImageLayerVector& imageLayers) = 0;

		virtual void setImageLayerOpcity(const std::string& layerName, float opcity) = 0;
		virtual float getImageLayerOpcity(const std::string& layerName) = 0;

		virtual void setImageLayerVisible(const std::string& layerName, bool visible) = 0;
		virtual bool getImageLayerVisible(const std::string& layerName) = 0;

		virtual void addImageLayerCallback(const std::string& layerName, osgEarth::ImageLayerCallback* icb) = 0;
		virtual void removeImageLayerCallback(const std::string& layerName, osgEarth::ImageLayerCallback* icb) = 0;

		virtual void setImageLayerEnable(const std::string& layerName, bool enable) = 0;
		virtual bool getImageLayerEnable(const std::string& layerName) = 0;

		//elevation
		virtual void addElevationLayer(const osgEarth::Config& layerconfig) = 0;
		virtual void removeElevationLayer(const std::string& layerName) = 0;
		virtual void moveElevationLayer(const std::string& layerName, int newIndex) = 0;
		virtual void removeAllElevationLayers() = 0;

		virtual osgEarth::ElevationLayer* getElevationLayer(const std::string& layerName) = 0;
		virtual void getAllElevationLayers(osgEarth::ElevationLayerVector& elevationLayers) = 0;

		virtual void setElevationLayerVisible(const std::string& layerName, bool visible) = 0;
		virtual bool getElevationLayerVisible(const std::string& layerName) = 0;

		virtual void addElevationLayerCallback(const std::string& layerName, osgEarth::ElevationLayerCallback* ecb) = 0;
		virtual void removeElevationLayerCallback(const std::string& layerName, osgEarth::ElevationLayerCallback* ecb) = 0;

		virtual bool getElevationLayerEnable(const std::string& layerName) = 0;

		//
		virtual void addMaskLayer(osgEarth::MaskLayer* masklayer) = 0;
		virtual void removeMaskLayer(osgEarth::MaskLayer* masklayer) = 0;
		virtual osgEarth::MaskLayer* getMaskLayer(const std::string& layername) = 0;
		virtual void getMaskLayerVector(osgEarth::MaskLayerVector& mlv) = 0;

		virtual osg::Node* getTerrainNode() = 0;

		virtual void addCallback(ITerrainSystemCallback* tsc) { _callbacks.push_back(tsc); }
		virtual void removeCallback(ITerrainSystemCallback* tsc)
		{
			TerrainSystemCallbackVector::iterator it;
			it = std::find(_callbacks.begin(), _callbacks.end(), tsc);
			if (it == _callbacks.end()) return;
			_callbacks.erase(it);
		}

	protected:
		std::string _key;

		gwApp::IApplication* _app;

		TerrainSystemCallbackVector _callbacks;
	};
}

#endif // GWEARTH_ITERRAINSYSTEM_H
