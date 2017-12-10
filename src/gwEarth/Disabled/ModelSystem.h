///  [9/4/2017 BigDog]: 管理模型。
#ifndef GWEARTH_MODELSYSTEM_H
#define GWEARTH_MODELSYSTEM_H 1

#include <gwBase/compiler.h>
#include <gwBase/SubsystemMgr.h>

#include <osgEarth/MapNode>
#include <osgEarth/Registry>
#include <vector>

namespace gwEarth
{
	template<typename T = osgEarth::ModelLayer>
	class ModelSystem : public gwBase::Subsystem
	{
	public:
		class Callback : public osg::Referenced
		{
		public:
			Callback() {}
			virtual ~Callback() {}

			virtual void onInit() {}
			virtual void onLayerAdded(T* layer) {}
			virtual void onLayerRemoved(T* layer) {}
			virtual void onUpdate(double delta_time) {}
			virtual void onShutdown() {}
		};
		typedef std::vector<osg::ref_ptr<Callback > > CallbackVector;

	public:
		ModelSystem(const std::string& name, osgEarth::MapNode* mapNode)
			: _name(name), _mapNode(mapNode), _map(0L)
		{
			if (_mapNode)
			{
				_map = _mapNode->getMap();
			}
			_uid = osgEarth::Registry::instance()->createUID();
		}

		virtual ~ModelSystem() { shutdown(); }
		const osgEarth::UID& getUid() const { return _uid; }
		void setName(const std::string& name) { _name = name; }
		std::string getName() const { return _name; }

		virtual void init()
		{
			std::for_each(_callbacks.begin(), _callbacks.end(), [](Callback* cb)
			{
				cb->onInit();
			});
		}

		virtual void shutdown()
		{
			removeAllLayers();
			std::for_each(_callbacks.begin(), _callbacks.end(), [](Callback* cb)
			{
				cb->onShutdown();
			});
		}

		virtual void update(double delta_t)
		{
			std::for_each(_callbacks.begin(), _callbacks.end(), [&delta_t](Callback* cb)
			{
				cb->onUpdate(delta_t);
			});
		}

		T* getLayer(const std::string& layername) const
		{
			std::vector<osg::ref_ptr<T> >::const_iterator it;
			for (it = _layers.begin(); it != _layers.end(); ++it)
			{
				if ((*it)->getName() == layername) return *it;
			}
			return 0L;
		}

		void getLayers(std::vector<osg::ref_ptr<T> >& out_layers) const
		{
			if (!_map) return;
			out_layers.reserve(_layers.size());
			std::vector<osg::ref_ptr<T> >::const_iterator it;
			for (it = _layers.begin(); it != _layers.end(); ++it)
			{
				out_layers.push_back(*it);
			}
		}

		bool addLayer(T* layer)
		{
			if (!_map || !layer) return false;
			_map->beginUpdate();
			_map->addModelLayer(layer);
			_map->endUpdate();
			_layers.push_back(layer);
			std::for_each(_callbacks.begin(), _callbacks.end(), [&layer](Callback* cb)
			{
				cb->onLayerAdded(layer);
			});
			return true;
		}

		virtual void addLayer(const osgEarth::Config& layerconfig)
		{
			//no operation.
		}

		bool removeLayer(T* layer)
		{
			if (!_map || !layer) return false;

			std::vector<osg::ref_ptr<T> >::iterator it;
			it = std::find(_layers.begin(), _layers.end(), layer);

			if (it == _layers.end()) return false;

			_map->beginUpdate();
			_map->removeModelLayer(layer);
			_map->endUpdate();
			_layers.erase(it);
			std::for_each(_callbacks.begin(), _callbacks.end(), [&layer](Callback* cb)
			{
				cb->onLayerRemoved(layer);
			});
			return true;
		}

		bool removeLayer(const std::string& layername)
		{
			return removeLayer(getLayer(layername));
		}

		void removeAllLayers()
		{
			std::vector<osg::ref_ptr<T> > layers;
			getLayers(layers);
			for (std::vector<osg::ref_ptr<T> >::iterator it = layers.begin(); it != layers.end(); ++it)
			{
				removeLayer(*it);
			}
		}

		virtual void addCallback(Callback* cb)
		{
			if (!cb) return;
			_callbacks.push_back(cb);
		}

		virtual void removeCallback(Callback* cb)
		{
			std::vector<osg::ref_ptr<Callback> >::iterator it;
			it = std::find(_callbacks.begin(), _callbacks.end(), cb);
			if (it == _callbacks.end()) return;
			_callbacks.erase(it);
		}

		virtual osg::Node* getLayerNode(T* layer) const
		{
			return _mapNode ? _mapNode->getModelLayerNode(layer) : 0L;
		}

	protected:
		std::string _name;
		osgEarth::UID _uid;
		osg::ref_ptr<osgEarth::Map> _map;
		osg::ref_ptr<osgEarth::MapNode> _mapNode;
		std::vector< osg::ref_ptr<T> > _layers;

		CallbackVector _callbacks;
	};

	//type define.
	typedef ModelSystem<> DefaultModelSystem;
	typedef osg::ref_ptr<ModelSystem<> > ModelSystemPtr;
	typedef osg::ref_ptr<osgEarth::ModelLayer> ModelLayerPtr;

	//make new.
	template<typename T = osgEarth::ModelLayer>
	static ModelSystem<T>* makeNewModelSystem(const std::string& name, gwApp::IApplication* app)
	{
		return new ModelSystem<T>(name, app);
	}

	template<typename MODEL_LAYER = osgEarth::ModelLayer>
	static MODEL_LAYER* makeNewModelLayer(const osgEarth::ModelLayerOptions& options)
	{
		return new MODEL_LAYER(options);
	}
}

#endif // GWEARTH_MODELSYSTEM_H
