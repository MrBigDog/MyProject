#ifndef GWEARTH_MODELSYSTEM_H
#define GWEARTH_MODELSYSTEM_H 1

#include <compiler.h>
#include <gwEarth/Export.h>
#include <gwBase/SubsystemMgr.h>
#include <gwBase/Exception.h>
#include <gwBase/LogStream.h>

#include <osgEarth/MapNode>
#include <osgEarth/Registry>
#include <vector>
#include <osgEarth/ModelLayer>

//namespace osgEarth
//{
//	class ModelLayerVector;
//}

namespace gwApp { class Application; };

namespace gwEarth
{
	class GWEARTH_EXPORT ModelSystem : public gwBase::Subsystem
	{
	public:
		class Callback : public osg::Referenced
		{
		public:
			Callback() {}
			virtual ~Callback() {}

			virtual void onInit() {}
			virtual void onLayerAdded(osgEarth::ModelLayer* layer) {}
			virtual void onLayerRemoved(osgEarth::ModelLayer* layer) {}
			virtual void onUpdate(double delta_time) {}
			virtual void onShutdown() {}
		};
		typedef std::vector<osg::ref_ptr<Callback > > CallbackVector;

	public:
		ModelSystem(const std::string& name, gwApp::Application* app);
		virtual ~ModelSystem();

		const osgEarth::UID& getUid() const { return _uid; }

		void init();
		void shutdown();
		void update(double delta_t);

		osgEarth::ModelLayer* getLayer(const std::string& layername) const;
		void getLayers(osgEarth::ModelLayerVector& out_layers) const;

		bool isCanAddLayer(osgEarth::ModelLayer* layer) const;
		bool addLayer(osgEarth::ModelLayer* layer);
		bool addLayer(const osgEarth::Config& conf);
		void addLayers(const osgEarth::ModelLayerVector& layers);

		bool removeLayer(osgEarth::ModelLayer* layer);
		bool removeLayer(const std::string& layername);
		void removeAllLayers();

		void addCallback(Callback* cb);
		void removeCallback(Callback* cb);

		osg::Node* getLayerNode(osgEarth::ModelLayer* layer) const;

		bool isLayerNameRepeat(const std::string layerName) const;
		bool isLayerRepeat(osgEarth::ModelLayer* layer) const;

	private:
		//std::string _name;
		osgEarth::UID _uid;
		osg::ref_ptr<osgEarth::Map> _map;
		osg::ref_ptr<osgEarth::MapNode> _mapNode;
		osgEarth::ModelLayerVector _layers;

		CallbackVector _callbacks;

		osg::ref_ptr<gwApp::Application> _app;
	};

	//type define.
	typedef osg::ref_ptr<ModelSystem > ModelSystemPtr;
	typedef osg::ref_ptr<osgEarth::ModelLayer> ModelLayerPtr;

	//make new.
	static ModelSystem* makeNewModelSystem(const std::string& name, gwApp::Application* app)
	{
		return new ModelSystem(name, app);
	}

	template<typename MODEL_LAYER = osgEarth::ModelLayer>
	static MODEL_LAYER* makeNewModelLayer(const osgEarth::ModelLayerOptions& options)
	{
		return new MODEL_LAYER(options);
	}
}

#endif // GWEARTH_MODELSYSTEM_H
