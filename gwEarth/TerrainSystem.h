#ifndef GWEARTH_MYTERRAINSYSTEM_H
#define GWEARTH_MYTERRAINSYSTEM_H 1

#include <compiler.h>
#include <gwEarth/Export.h>
#include <gwBase/SubsystemMgr.h>
#include <gwApp/Application.h>

#include <osgEarth/MapNode>
#include <osgEarth/Registry>
#include <vector>

#include <osgEarth/ImageLayer>
#include <osgEarth/ElevationLayer>
#include <osgEarth/MaskLayer>

//namespace osgEarth
//{
//	//class ImageLayerVector;
//	class ElevationLayerVector;
//	class MaskLayerVector;
//}

namespace gwEarth
{
	using namespace osgEarth;

	class GWEARTH_EXPORT TerrainSystem :public gwBase::Subsystem
	{
	public:
		class Callback :public osg::Referenced
		{
		public:
			Callback() {}
			virtual ~Callback() {}

			virtual void onInit() {}
			virtual void onImageLayerAdded(osgEarth::ImageLayer* layer) {}
			virtual void onImageLayerRemoved(osgEarth::ImageLayer* layer) {}
			virtual void onElevationLayerAdded(osgEarth::ElevationLayer* layer) {}
			virtual void onElevationLayerRemoved(osgEarth::ElevationLayer* layer) {}
			virtual void onMaskLayerAdded(osgEarth::MaskLayer*layer) {}
			virtual void onMaskLayerRemoved(osgEarth::MaskLayer*layer) {}
			virtual void onUpdate(double delta_time) {}
			virtual void onShutdown() {}
		};
		typedef std::vector<osg::ref_ptr<Callback> > CallbackVector;

	public:
		TerrainSystem(const std::string& name, gwApp::Application* app);
		virtual ~TerrainSystem();

		//-------------------BASE--------------------------------------------
		const osgEarth::UID& getUid() const { return _uid; }

		virtual void init();
		virtual void shutdown();
		virtual void update(double delta_t);

		virtual bool addLayer(const osgEarth::Config& conf);

		//--------------------osgEarth::ImageLayer---------------------------------------------------
		osgEarth::ImageLayer* getImageLayer(const std::string& layername) const;
		void getImageLayers(osgEarth::ImageLayerVector& out_layers) const;
		//bool addImageLayer(const Config& conf);
		bool addImageLayer(osgEarth::ImageLayer* layer);
		bool removeImageLayer(osgEarth::ImageLayer* layer);
		bool removeImageLayer(const std::string& layername);
		void removeAllImageLayers();

		//----------------------osgEarth::ElevationLayer--------------------------------------------------------
		osgEarth::ElevationLayer* getElevationLayer(const std::string& layerName) const;
		void getElevationLayers(osgEarth::ElevationLayerVector& out_layers) const;
		//bool addElevationLayer(const Config& conf);
		bool addElevationLayer(osgEarth::ElevationLayer* layer);
		bool removeElevationLayer(osgEarth::ElevationLayer* layer);
		bool removeElevationLayer(const std::string& layername);
		void removeAllElevationLayers();

		//----------------osgEarth::MaskLayer---------------------------------------------------------------
		osgEarth::MaskLayer* getMaskLayer(const std::string& layerName) const;
		void getMaskLayers(osgEarth::MaskLayerVector& out_layers) const;
		//bool addMaskLayer(const Config& conf);
		bool addMaskLayer(osgEarth::MaskLayer*layer);
		bool removeMaskLayer(osgEarth::MaskLayer* layer);
		bool removeMaskLayer(const std::string& layername);
		void removeAllMaskLayers();

		//-----------------CALL BACK---------------------------------------------------------------
		virtual void addCallback(Callback* cb);
		virtual void removeCallback(Callback* cb);

	protected:
		osgEarth::UID _uid;
		osg::ref_ptr<osgEarth::Map> _map;
		osg::ref_ptr<osgEarth::MapNode> _mapNode;
		std::vector< osg::ref_ptr<osgEarth::ImageLayer> > _imageLayers;
		std::vector< osg::ref_ptr<osgEarth::ElevationLayer> > _elevationLayers;
		std::vector< osg::ref_ptr<osgEarth::MaskLayer> > _maskLayers;

		CallbackVector _callbacks;

		osg::ref_ptr<gwApp::Application> _app;
	};

	//type define.
	typedef osg::ref_ptr<TerrainSystem >TerrainSystemPtr;
	typedef osg::ref_ptr<osgEarth::MaskLayer> MaskLayerPtr;
	typedef osg::ref_ptr<osgEarth::ImageLayer> ImageLayerPtr;
	typedef osg::ref_ptr<osgEarth::ElevationLayer> ElevationLayerPtr;
}

#endif // GWEARTH_MYTERRAINSYSTEM_H
