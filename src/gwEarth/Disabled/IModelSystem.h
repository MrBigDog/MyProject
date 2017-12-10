#ifndef GWEARTH_IMODELSYSTEM_H
#define GWEARTH_IMODELSYSTEM_H 1

#include <gwApp/IApplication.h>
#include <gwBase/ISubsystemMgr.h>

#include <osgEarth/MapNode>
#include <vector>

namespace gwEarth
{
#define META_SYSTEM_CALLBACK(callback_class) \
	virtual void addCallback(callback_class* cb){_callbacks.push_back(cb);}\
	virtual void removeCallback(callback_class* cb)\
	{\
		std::vector<osg::ref_ptr<callback_class> >::iterator it;\
		it = std::find(_callbacks.begin(), _callbacks.end(), cb);\
		if (it == _callbacks.end()) return;\
		_callbacks.erase(it);\
	}

	///
	template<typename T>
	class IModelSystemCallback : public osg::Referenced
	{
	public:
		IModelSystemCallback() {}
		virtual ~IModelSystemCallback() {}

		virtual void onLayerAdded(T* layer) {}
		virtual void onLayerRemoved(T* layer) {}
		virtual void onUpdate(double delta_time) {}
	};

	///
	class IModelSystem : public gwBase::ISubsystem
	{
	public:
		IModelSystem(gwApp::IApplication* app) : _app(app), _map(0L), _mapNode(0L)
		{
			if (_app)
			{
				_map = _app->getMap();
				_mapNode = _app->getMapNode();
			}
		}

		virtual ~IModelSystem() {};

		void setKey(const std::string& key) { _key = key; }
		std::string getKey() const { return _key; }

		// base system method
		virtual void init() = 0;
		virtual void shutdown() = 0;
		virtual void update(double dt) = 0;

		virtual void initSystemFromConfig(const osgEarth::Config& systemConfig) = 0;

		template<class T>
		T* getLayer(const std::string& layername) const
		{
			if (!_map) return 0L;
			return dynamic_cast<T*>(_map->getModelLayerByName(layername));
		}

		template<class T>
		void getLayers(std::vector<osg::ref_ptr<T> >& out_layers) const
		{
			if (!_map) return;
			osgEarth::ModelLayerVector mlv;
			_app->getMap()->getModelLayers(mlv);
			for (osgEarth::ModelLayerVector::iterator it = mlv.begin(); it != mlv.end(); ++it)
			{
				T* layer = dynamic_cast<T*>((*it).get());
				if (!layer) continue;
				out_layers.push_back(layer);
			}
		}

		template<class T>
		bool addLayer(T* layer)
		{
			if (!_map || !layer) return false;
			_map->beginUpdate();
			_map->addModelLayer(layer);
			_map->endUpdate();
			return true;
		}

		template<class T>
		bool removeLayer(T* layer)
		{
			if (!_map || !layer) return false;
			_map->beginUpdate();
			_map->removeModelLayer(layer);
			_map->endUpdate();
			return true;
		}

		template<class T>
		bool removeLayer(const std::string& layername)
		{
			T*layer = getLayer(layername);
			return removeLayer(layer);
		}

		virtual void addLayer(const osgEarth::Config& layerconfig) = 0;

		virtual void queryAnalysis() = 0;

		virtual osg::Node* getLayerNode(const std::string& layername) const = 0;

		//osg::Group* getRoot() const { return _sceneRoot; }
	//protected:
	//	META_SYSTEM_CALLBACK(IModelSystemCallback)

	protected:
		std::string _key;
		gwApp::IApplication* _app;
		osg::ref_ptr<osgEarth::Map> _map;
		osg::ref_ptr<osgEarth::MapNode> _mapNode;
	};

	typedef osg::ref_ptr<IModelSystem> IModelSystemPtr;
}


#endif // GWAPP_IMODELSYSTEM_H
