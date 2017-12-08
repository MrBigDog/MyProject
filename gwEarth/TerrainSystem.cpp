#include <gwEarth/TerrainSystem.h>
#include <osgEarthDrivers/tms/TMSOptions>

#include <osgEarth/ImageLayer>
#include <osgEarth/ElevationLayer>
#include <osgEarth/MaskLayer>

#undef  LC
#define LC "[TerrainSystem] "

namespace gwEarth
{
	namespace
	{
		void layerWarning(const std::string& layerType, const std::string& layerName, const std::string& content)
		{
			OE_WARN << LC << layerType << "Layer \"" << layerName << "\" : "
				<< content << std::endl;
		}
	}

	TerrainSystem::TerrainSystem(const std::string & name, gwApp::Application * app)
		: _app(app), _mapNode(0L), _map(0L)
	{
		_name = name;
		if (_app)
		{
			_map = _app->getMap();
			_mapNode = _app->getMapNode();
		}
		_uid = osgEarth::Registry::instance()->createUID();
	}

	TerrainSystem::~TerrainSystem()
	{
		shutdown();
	}

	void TerrainSystem::init()
	{
		std::for_each(_callbacks.begin(), _callbacks.end(), [](Callback* cb)
		{
			cb->onInit();
		});
	}

	void TerrainSystem::shutdown()
	{
		removeAllImageLayers();
		removeAllElevationLayers();
		std::for_each(_callbacks.begin(), _callbacks.end(), [](Callback* cb)
		{
			cb->onShutdown();
		});
	}

	void TerrainSystem::update(double delta_t)
	{
		std::for_each(_callbacks.begin(), _callbacks.end(), [&delta_t](Callback* cb)
		{
			cb->onUpdate(delta_t);
		});
	}

	bool TerrainSystem::addLayer(const osgEarth::Config & conf)
	{
		std::string name = conf.key();
		Layer* layer = Layer::create(name, conf);
		if (!layer) return false;

		ImageLayer* imageLayer = dynamic_cast<ImageLayer*>(layer);
		if (imageLayer)
		{
			return addImageLayer(imageLayer);
		}
		ElevationLayer* eleLayer = dynamic_cast<ElevationLayer*>(layer);
		if (eleLayer)
		{
			return addElevationLayer(eleLayer);
		}
		MaskLayer* maskLayer = dynamic_cast<MaskLayer*>(layer);
		if (maskLayer)
		{
			return addMaskLayer(maskLayer);
		}
		return false;
	}

	//-------------------------------------------------------------------------------------
	osgEarth::ImageLayer * TerrainSystem::getImageLayer(const std::string & layername) const
	{
		std::vector<osg::ref_ptr<osgEarth::ImageLayer> >::const_iterator it;
		for (it = _imageLayers.begin(); it != _imageLayers.end(); ++it)
		{
			if ((*it)->getName() == layername) return *it;
		}
		return 0L;
	}

	void TerrainSystem::getImageLayers(osgEarth::ImageLayerVector & out_layers) const
	{
		if (!_map) return;
		out_layers.reserve(_imageLayers.size());
		osgEarth::ImageLayerVector::const_iterator it;
		for (it = _imageLayers.begin(); it != _imageLayers.end(); ++it)
		{
			out_layers.push_back(*it);
		}
	}

	bool TerrainSystem::addImageLayer(osgEarth::ImageLayer * layer)
	{
		if (!_map || !layer) return false;
		_map->beginUpdate();
		_map->addLayer(layer);
		_map->endUpdate();
		_imageLayers.push_back(layer);
		if (layer->getStatus().isError())
		{
			layerWarning("Image", layer->getName(), layer->getStatus().toString());
		}
		std::for_each(_callbacks.begin(), _callbacks.end(), [&layer](Callback* cb)
		{
			cb->onImageLayerAdded(layer);
		});
		return true;
	}

	//bool TerrainSystem::addImageLayer(const Config & conf)
	//{
	//	if (!_map) return false;
	//	ImageLayerOptions options(conf);
	//	options.name() = conf.value("name");
	//	osg::ref_ptr<ImageLayer> layer = new ImageLayer(options);
	//	_map->beginUpdate();
	//	_map->addImageLayer(layer);
	//	_map->endUpdate();
	//	if (layer->getStatus().isError())
	//	{
	//		layerWarning("Image", layer->getName(), layer->getStatus().toString());
	//	}
	//	_imageLayers.push_back(layer);
	//	std::for_each(_callbacks.begin(), _callbacks.end(), [&layer](Callback* cb)
	//	{
	//		cb->onImageLayerAdded(layer);
	//	});
	//	return true;
	//}

	bool TerrainSystem::removeImageLayer(osgEarth::ImageLayer * layer)
	{
		if (!_map || !layer) return false;

		osgEarth::ImageLayerVector::iterator it;
		it = std::find(_imageLayers.begin(), _imageLayers.end(), layer);

		if (it == _imageLayers.end()) return false;

		_map->beginUpdate();
		_map->removeLayer(layer);
		_map->endUpdate();
		_imageLayers.erase(it);
		std::for_each(_callbacks.begin(), _callbacks.end(), [&layer](Callback* cb)
		{
			cb->onImageLayerRemoved(layer);
		});
		return true;
	}

	bool TerrainSystem::removeImageLayer(const std::string & layername)
	{
		return removeImageLayer(getImageLayer(layername));
	}

	void TerrainSystem::removeAllImageLayers()
	{
		osgEarth::ImageLayerVector layers;
		getImageLayers(layers);
		for (osgEarth::ImageLayerVector::iterator it = layers.begin(); it != layers.end(); ++it)
		{
			removeImageLayer(*it);
		}
	}

	//------------------------------------------------------------------------------------------------
	osgEarth::ElevationLayer * TerrainSystem::getElevationLayer(const std::string & layerName) const
	{
		osgEarth::ElevationLayerVector::const_iterator it;
		for (it = _elevationLayers.begin(); it != _elevationLayers.end(); ++it)
		{
			if ((*it)->getName() == layerName) return *it;
		}
		return 0L;
	}

	void TerrainSystem::getElevationLayers(osgEarth::ElevationLayerVector & out_layers) const
	{
		if (!_map) return;
		out_layers.reserve(_elevationLayers.size());
		osgEarth::ElevationLayerVector::const_iterator it;
		for (it = _elevationLayers.begin(); it != _elevationLayers.end(); ++it)
		{
			out_layers.push_back(*it);
		}
	}

	bool TerrainSystem::addElevationLayer(osgEarth::ElevationLayer * layer)
	{
		if (!_map || !layer) return false;
		_map->beginUpdate();
		_map->addLayer(layer);
		_map->endUpdate();
		_elevationLayers.push_back(layer);
		if (layer->getStatus().isError())
		{
			layerWarning("Elevation", layer->getName(), layer->getStatus().toString());
		}
		std::for_each(_callbacks.begin(), _callbacks.end(), [&layer](Callback* cb)
		{
			cb->onElevationLayerAdded(layer);
		});
		return true;
	}

	//bool TerrainSystem::addElevationLayer(const Config & conf)
	//{
	//	if (!_map) return false;
	//	ElevationLayerOptions options(conf);
	//	options.name() = conf.value("name");
	//	osg::ref_ptr<ElevationLayer> layer = new ElevationLayer(options);
	//	_map->beginUpdate();
	//	_map->addElevationLayer(layer);
	//	_map->endUpdate();
	//	if (layer->getStatus().isError())
	//	{
	//		layerWarning("Elevation", layer->getName(), layer->getStatus().toString());
	//	}
	//	_elevationLayers.push_back(layer);
	//	std::for_each(_callbacks.begin(), _callbacks.end(), [&layer](Callback* cb)
	//	{
	//		cb->onElevationLayerAdded(layer);
	//	});
	//	return true;
	//}

	bool TerrainSystem::removeElevationLayer(osgEarth::ElevationLayer * layer)
	{
		if (!_map || !layer) return false;

		osgEarth::ElevationLayerVector::iterator it;
		it = std::find(_elevationLayers.begin(), _elevationLayers.end(), layer);

		if (it == _elevationLayers.end()) return false;

		_map->beginUpdate();
		_map->removeElevationLayer(layer);
		_map->endUpdate();
		_elevationLayers.erase(it);
		std::for_each(_callbacks.begin(), _callbacks.end(), [&layer](Callback* cb)
		{
			cb->onElevationLayerRemoved(layer);
		});
		return true;
	}

	bool TerrainSystem::removeElevationLayer(const std::string & layername)
	{
		return removeElevationLayer(getElevationLayer(layername));
	}

	void TerrainSystem::removeAllElevationLayers()
	{
		osgEarth::ElevationLayerVector layers;
		getElevationLayers(layers);
		std::vector<osg::ref_ptr<osgEarth::ElevationLayer> >::iterator it;
		for (it = layers.begin(); it != layers.end(); ++it)
		{
			removeElevationLayer(*it);
		}
	}


	////////------------------------------------------------------------------------------------------------------
	osgEarth::MaskLayer * TerrainSystem::getMaskLayer(const std::string & layerName) const
	{
		osgEarth::MaskLayerVector::const_iterator it;
		for (it = _maskLayers.begin(); it != _maskLayers.end(); ++it)
		{
			if ((*it)->getName() == layerName) return *it;
		}
		return 0L;
	}

	void TerrainSystem::getMaskLayers(osgEarth::MaskLayerVector & out_layers) const
	{
		if (!_map) return;
		out_layers.reserve(_maskLayers.size());
		osgEarth::MaskLayerVector::const_iterator it;
		for (it = _maskLayers.begin(); it != _maskLayers.end(); ++it)
		{
			out_layers.push_back(*it);
		}
	}

	bool TerrainSystem::addMaskLayer(osgEarth::MaskLayer * layer)
	{
		if (!_map || !layer) return false;
		_map->beginUpdate();
		_map->addTerrainMaskLayer(layer);
		_map->endUpdate();
		_maskLayers.push_back(layer);
		if (layer->getStatus().isError())
		{
			layerWarning("Mask", layer->getName(), layer->getStatus().toString());
		}
		std::for_each(_callbacks.begin(), _callbacks.end(), [&layer](Callback*cb)
		{
			cb->onMaskLayerAdded(layer);
		});
		return true;
	}

	//bool TerrainSystem::addMaskLayer(const Config & conf)
	//{
	//	if (!_map) return false;
	//	MaskLayerOptions options(conf);
	//	options.name() = conf.value("name");
	//	options.driver() = MaskSourceOptions(options);
	//	MaskLayer* layer = new MaskLayer(options);

	//	_map->beginUpdate();
	//	_map->addTerrainMaskLayer(layer);
	//	_map->endUpdate();
	//	if (layer->getStatus().isError())
	//	{
	//		layerWarning("Mask", layer->getName(), layer->getStatus().toString());
	//	}
	//	_maskLayers.push_back(layer);
	//	std::for_each(_callbacks.begin(), _callbacks.end(), [&layer](Callback*cb)
	//	{
	//		cb->onMaskLayerAdded(layer);
	//	});
	//	return true;
	//}

	bool TerrainSystem::removeMaskLayer(osgEarth::MaskLayer * layer)
	{
		if (!_map || !layer) return false;

		osgEarth::MaskLayerVector::iterator it;
		it = std::find(_maskLayers.begin(), _maskLayers.end(), layer);

		if (it == _maskLayers.end()) return false;

		_map->beginUpdate();
		_map->removeTerrainMaskLayer(layer);
		_map->endUpdate();
		_maskLayers.erase(it);
		std::for_each(_callbacks.begin(), _callbacks.end(), [&layer](Callback* cb)
		{
			cb->onMaskLayerRemoved(layer);
		});
		return true;
	}

	bool TerrainSystem::removeMaskLayer(const std::string & layername)
	{
		return removeMaskLayer(getMaskLayer(layername));
	}

	void TerrainSystem::removeAllMaskLayers()
	{
		std::vector<osg::ref_ptr<osgEarth::MaskLayer> > layers;
		getMaskLayers(layers);
		for (std::vector<osg::ref_ptr<osgEarth::MaskLayer> >::iterator it = layers.begin(); it != layers.end(); ++it)
		{
			removeMaskLayer(*it);
		}
	}


	//------------------------------------------------------------------------------------------------------------------
	void TerrainSystem::addCallback(Callback * cb)
	{
		if (!cb) return;
		_callbacks.push_back(cb);
	}

	void TerrainSystem::removeCallback(Callback * cb)
	{
		std::vector<osg::ref_ptr<Callback> >::iterator it;
		it = std::find(_callbacks.begin(), _callbacks.end(), cb);
		if (it == _callbacks.end()) return;
		_callbacks.erase(it);
	}


}