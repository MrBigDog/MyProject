#include <gwEarth/ModelSystem.h>
#include <gwApp/Application.h>
#include <gwApp/Util.h>


#undef  LC
#define LC "[ModelSystem] "

namespace gwEarth
{
	namespace
	{
		void layerWarning(const std::string& layerName, const std::string& content)
		{
			OE_WARN << LC << "Layer \"" << layerName << "\" : "
				<< content << std::endl;
		}
	}

	ModelSystem::ModelSystem(const std::string& name, gwApp::Application* app)
		:_app(app), _mapNode(0L), _map(0L)
	{
		_name = name;
		_uid = osgEarth::Registry::instance()->createUID();
		if (_app)
		{
			_map = _app->getMap();
			_mapNode = _app->getMapNode();
		}
	}

	ModelSystem::~ModelSystem()
	{
		shutdown();
	}

	void ModelSystem::init()
	{
		std::for_each(_callbacks.begin(), _callbacks.end(), [](Callback* cb)
		{
			cb->onInit();
		});
	}

	void ModelSystem::shutdown()
	{
		removeAllLayers();
		std::for_each(_callbacks.begin(), _callbacks.end(), [](Callback* cb)
		{
			cb->onShutdown();
		});
	}

	void ModelSystem::update(double delta_t)
	{
		std::for_each(_callbacks.begin(), _callbacks.end(), [&delta_t](Callback* cb)
		{
			cb->onUpdate(delta_t);
		});
	}

	osgEarth::ModelLayer* ModelSystem::getLayer(const std::string& layername) const
	{
		osgEarth::ModelLayerVector::const_iterator it;
		for (it = _layers.begin(); it != _layers.end(); ++it)
		{
			if ((*it)->getName() != layername) continue;
			return *it;
		}
		return 0L;
	}

	void ModelSystem::getLayers(osgEarth::ModelLayerVector& out_layers) const
	{
		if (!_map) return;
		out_layers.reserve(_layers.size());
		osgEarth::ModelLayerVector::const_iterator it;
		for (it = _layers.begin(); it != _layers.end(); ++it)
		{
			out_layers.push_back(*it);
		}
	}

	bool ModelSystem::addLayer(osgEarth::ModelLayer* layer)
	{
		if (!_map || !layer) return false;

		const std::string& layerName = layer->getName();
		if (isLayerRepeat(layer))
		{
			throw gw_exception("duplicate model layer:" + layerName);
		}
		if (isLayerNameRepeat(layerName))
		{
			throw gw_exception("duplicate model layer name:" + layerName);
		}

		_map->beginUpdate();
		_map->addLayer(layer);
		_map->endUpdate();
		if (layer->getStatus().isError())
		{
			layerWarning(layer->getName(), layer->getStatus().toString());
		}
		_layers.push_back(layer);
		std::for_each(_callbacks.begin(), _callbacks.end(), [&layer](Callback* cb)
		{
			cb->onLayerAdded(layer);
		});
		return true;
	}

	void ModelSystem::addLayers(const osgEarth::ModelLayerVector & layers)
	{
		osgEarth::ModelLayerVector::const_iterator it;
		for (it = layers.begin(); it != layers.end(); ++it)
		{
			addLayer(*it);
		}
	}

	bool ModelSystem::addLayer(const osgEarth::Config& layerconfig)
	{
		if (!_map) return false;

		osgEarth::ModelLayerOptions options(layerconfig);
		options.name() = layerconfig.value("name");
		options.driver() = osgEarth::ModelSourceOptions(layerconfig);
		osgEarth::ModelLayer* layer = new osgEarth::ModelLayer(options);
		_map->beginUpdate();
		_map->addLayer(layer);
		_map->endUpdate();
		if (layer->getStatus().isError())
		{
			layerWarning(layer->getName(), layer->getStatus().toString());
		}
		_layers.push_back(layer);
		std::for_each(_callbacks.begin(), _callbacks.end(), [&layer](Callback* cb)
		{
			cb->onLayerAdded(layer);
		});
		return true;
	}

	bool ModelSystem::removeLayer(osgEarth::ModelLayer* layer)
	{
		if (!_map || !layer) return false;

		osgEarth::ModelLayerVector::iterator it;
		it = std::find(_layers.begin(), _layers.end(), layer);

		if (it == _layers.end()) return false;

		_map->beginUpdate();
		_map->removeLayer(layer);
		_map->endUpdate();
		_layers.erase(it);
		std::for_each(_callbacks.begin(), _callbacks.end(), [&layer](Callback* cb)
		{
			cb->onLayerRemoved(layer);
		});
		return true;
	}

	bool ModelSystem::removeLayer(const std::string& layername)
	{
		osgEarth::ModelLayerVector::iterator it;
		for (it = _layers.begin(); it != _layers.end(); ++it)
		{
			if ((*it)->getName() == layername)
			{
				return removeLayer(*it);
			}
		}
		return false;
	}

	void ModelSystem::removeAllLayers()
	{
		_map->beginUpdate();
		osgEarth::ModelLayerVector::iterator it;
		for (it = _layers.begin(); it != _layers.end(); ++it)
		{
			osgEarth::ModelLayer* layer = *it;
			_map->removeLayer(*it);
			std::for_each(_callbacks.begin(), _callbacks.end(), [&layer](Callback* cb)
			{
				cb->onLayerRemoved(layer);
			});
		}
		_layers.clear();
		_map->endUpdate();
	}

	void ModelSystem::addCallback(Callback* cb)
	{
		if (!cb) return;
		_callbacks.push_back(cb);
	}

	void ModelSystem::removeCallback(Callback* cb)
	{
		std::vector<osg::ref_ptr<Callback> >::iterator it;
		it = std::find(_callbacks.begin(), _callbacks.end(), cb);
		if (it == _callbacks.end()) return;
		_callbacks.erase(it);
	}

	osg::Node* ModelSystem::getLayerNode(osgEarth::ModelLayer* layer) const
	{
		return _mapNode ? _mapNode->getLayerNode(layer) : 0L;
	}

	bool ModelSystem::isCanAddLayer(osgEarth::ModelLayer* layer) const
	{
		if (isLayerRepeat(layer))
		{
			return false;
		}
		if (isLayerNameRepeat(layer->getName()))
		{
			return false;
		}
		return true;
	}

	bool ModelSystem::isLayerNameRepeat(const std::string layerName) const
	{
		osgEarth::ModelLayerVector::const_iterator it;
		for (it = _layers.begin(); it != _layers.end(); ++it)
		{
			if ((*it)->getName() == layerName)
			{
				return true;
			}
		}
		return false;
	}

	bool ModelSystem::isLayerRepeat(osgEarth::ModelLayer* layer) const
	{
		osgEarth::ModelLayerVector::const_iterator it;
		it = std::find(_layers.begin(), _layers.end(), layer);
		if (it != _layers.end())
		{
			return true;
		}
		return false;
	}
}