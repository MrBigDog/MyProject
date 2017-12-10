#include <gwEarth/TerrainSystem.h>
#include <gwBase/IRender.h>

#include <osgEarthDrivers/arcgis/ArcGISOptions>
#include <osgEarthDrivers/tms/TMSOptions>
#include <osgEarthDrivers/wms/WMSOptions>

#include <osgDB/FileNameUtils>


namespace gwEarth
{
	typedef void(TerrainSystem::*method_f)(const osgEarth::Config& arg);
	class LayerOperation :public osg::Operation
	{
	public:
		LayerOperation(TerrainSystem*ts, method_f method, const osgEarth::Config& arg)
			: _ts(ts)
			, _method(method)
			, _arg(arg)
		{}
		virtual void operator()(osg::Object*)
		{
			(_ts->*_method)(_arg);
		}
	private:
		TerrainSystem*_ts;
		method_f _method;
		osgEarth::Config _arg;
	};

	std::string extractBetween(const std::string& str, const std::string &lhs, const std::string &rhs)
	{
		std::string result;
		std::string::size_type start = str.find(lhs);
		if (start != std::string::npos)
		{
			start += lhs.length();
			std::string::size_type count = str.size() - start;
			std::string::size_type end = str.find(rhs, start);
			if (end != std::string::npos) count = end - start;
			result = str.substr(start, count);
		}
		return result;
	}
	bool getTilesourceOptions(const osgEarth::Config& args, osgEarth::TileSourceOptions& out_opt)
	{
		std::string type = args.value("type");
		std::string url = args.value("url");
		if (url.empty()) return false;

		if (type == "tms")
		{
			out_opt = osgEarth::Drivers::TMSOptions(url);
			return true;
		}
		else if (type == "wms")
		{
			std::string layers = args.value("layers");
			std::string format = args.value("format");
			std::string srs = args.value("srs");
			std::string styles = args.value("styles");

			osgEarth::Drivers::WMSOptions wmsOpt;
			wmsOpt.url() = url.substr(0, url.find("?"));

			std::string lower = osgDB::convertToLowerCase(url);

			if (!layers.empty())
				wmsOpt.layers() = layers;
			else if (lower.find("layers=", 0) != std::string::npos)
				wmsOpt.layers() = extractBetween(lower, "layers=", "&");

			if (!format.empty())
				wmsOpt.format() = format;
			else if (lower.find("srs=", 0) != std::string::npos)
				wmsOpt.format() = extractBetween(lower, "format=image/", "&");

			if (!srs.empty())
				wmsOpt.srs() = srs;
			else if (lower.find("format=image/", 0) != std::string::npos)
				wmsOpt.srs() = extractBetween(lower, "srs=", "&");

			if (!styles.empty())
				wmsOpt.style() = styles;
			else if (lower.find("styles=", 0) != std::string::npos)
				wmsOpt.style() = extractBetween(lower, "styles=", "&");

			out_opt = wmsOpt;
			return true;
		}
		else if (type == "arcgis")
		{
			std::string token = args.value("token");
			osgEarth::Drivers::ArcGISOptions arcOpt;
			arcOpt.url() = url;

			if (!token.empty())
				arcOpt.token() = token;

			out_opt = arcOpt;
			return true;
		}
		return false;
	}

	//bool addImageLayerMethod(osgEarth::Map* map, const osgEarth::Config& arg)
	//{
	//	return true;
	//}

	//bool insertImageLayerMethod(osgEarth::Map*map, const osgEarth::Config& arg)
	//{
	//	return true;
	//}

	//bool moveImageLayerMethod(osgEarth::Map*map, const osgEarth::Config&arg)
	//{
	//	unsigned int newIndex;
	//	osgEarth::ImageLayer* layer = (map->getImageLayerByName(""));
	//	if (!layer) return;

	//	map->beginUpdate();
	//	map->moveImageLayer(layer, newIndex);
	//	map->endUpdate();

	//	return true;
	//}

	//bool removeImageLayerMethod(osgEarth::Map* map, const osgEarth::Config& arg)
	//{
	//	osgEarth::ImageLayer* layer = (map->getImageLayerByName(""));
	//	if (!layer) return false;

	//	map->beginUpdate();
	//	map->removeImageLayer(layer);
	//	map->endUpdate();
	//	return true;
	//}

	//bool addElevationLayerMethod(osgEarth::Map* map, const osgEarth::Config& arg)
	//{
	//	return true;
	//}

	//bool insertElevationLayerMethod(osgEarth::Map*map, const osgEarth::Config&arg)
	//{
	//	return true;
	//}

	//bool moveElevationLayerMethod(osgEarth::Map*map, const osgEarth::Config&arg)
	//{
	//	osgEarth::ElevationLayer* layer = map->getElevationLayerByName("");
	//	if (!layer) return false;

	//	map->beginUpdate();
	//	map->removeElevationLayer(layer);
	//	map->endUpdate();

	//	return true;
	//	return true;
	//}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
	TerrainSystem::TerrainSystem(gwApp::IApplication* app)
		: ITerrainSystem(app)
	{}

	TerrainSystem::~TerrainSystem()
	{
	}

	void TerrainSystem::init()
	{
	}

	void TerrainSystem::shutdown()
	{
	}

	void TerrainSystem::update(double dt)
	{
	}

	void TerrainSystem::initSystemFromConfig(const osgEarth::Config & systemConfig)
	{
	}

	void TerrainSystem::addImageLayer(const osgEarth::Config & arg)
	{
		if (!_app || !_app->getMap() || !_app->getRender())
			return;

		_app->getRender()->addOperation
		(
			new LayerOperation(this, &addImageLayerMethod, arg)
		);
	}

	void TerrainSystem::insertImageLayer(const osgEarth::Config & arg)
	{
		if (!_app || !_app->getMap() || !_app->getRender())
			return;

		_app->getRender()->addOperation
		(
			new LayerOperation(this, &insertImageLayerMethod, arg)
		);
	}

	void TerrainSystem::removeImageLayer(const std::string& layerName)
	{
		if (!_app || !_app->getMap() || !_app->getRender())
			return;

		osgEarth::Config arg;
		_app->getRender()->addOperation
		(
			new LayerOperation(this, &removeImageLayerMethod, arg)
		);
	}

	void TerrainSystem::moveImageLayer(const std::string& layerName, int newIndex)
	{
		if (!_app || !_app->getMap() || _app->getRender())
			return;

		osgEarth::Config arg;
		_app->getRender()->addOperation
		(
			new LayerOperation(this, &moveImageLayerMethod, arg)
		);
	}

	osgEarth::ImageLayer* TerrainSystem::getImageLayer(const std::string& layerName)
	{
		if (!_app || !_app->getMap())
			return NULL;

		return _app->getMap()->getImageLayerByName(layerName);
	}

	void TerrainSystem::getAllImageLayers(osgEarth::ImageLayerVector& imageLayers)
	{
		if (!_app) return;
		if (!_app->getMap()) return;

		_app->getMap()->getImageLayers(imageLayers);
	}

	void TerrainSystem::setImageLayerOpcity(const std::string& layerName, float opcity)
	{
		if (!_app) return;
		if (!_app->getMap()) return;

		osgEarth::ImageLayer* layer = (_app->getMap()->getImageLayerByName(layerName));
		if (!layer) return;

		layer->setOpacity(opcity);
	}

	float TerrainSystem::getImageLayerOpcity(const std::string& layerName)
	{
		if (!_app) return -1.0f;
		if (!_app->getMap()) return -1.0f;

		osgEarth::ImageLayer* layer = (_app->getMap()->getImageLayerByName(layerName));
		if (!layer)
		{
			return -1.0f;
		}
		return layer->getOpacity();
	}

	void TerrainSystem::setImageLayerVisible(const std::string& layerName, bool visible)
	{
		if (!_app) return;
		if (!_app->getMap()) return;

		osgEarth::ImageLayer* layer = (_app->getMap()->getImageLayerByName(layerName));
		if (!layer) return;

		layer->setVisible(visible);
	}

	bool TerrainSystem::getImageLayerVisible(const std::string& layerName)
	{
		if (!_app) return false;
		if (!_app->getMap()) return false;

		osgEarth::ImageLayer* layer = (_app->getMap()->getImageLayerByName(layerName));
		if (!layer)
		{
			return false;
		}
		return layer->getVisible();
	}

	void TerrainSystem::addImageLayerCallback(const std::string& layerName, osgEarth::ImageLayerCallback* icb)
	{
		if (!_app) return;
		if (!_app->getMap()) return;

		osgEarth::ImageLayer* layer = (_app->getMap()->getImageLayerByName(layerName));
		if (!layer) return;

		layer->addCallback(icb);
	}

	void TerrainSystem::removeImageLayerCallback(const std::string& layerName, osgEarth::ImageLayerCallback* icb)
	{
		if (!_app) return;
		if (!_app->getMap()) return;

		osgEarth::ImageLayer* layer = (_app->getMap()->getImageLayerByName(layerName));
		if (!layer) return;

		layer->removeCallback(icb);
	}

	void TerrainSystem::setImageLayerEnable(const std::string& layerName, bool enable)
	{
		//    		if (!_app) return;
		//		if (!_app->getMap()) return;

		//    osgEarth::ImageLayer* layer = _app->getMap()->getImageLayerByName(layerName);
		//    if (!layer)
		//    {
		//        return;
		//    }
		//    return layer->setEnable(enable);
	}

	bool TerrainSystem::getImageLayerEnable(const std::string& layerName)
	{
		if (!_app) return false;
		if (!_app->getMap()) return false;

		osgEarth::ImageLayer* layer = (_app->getMap()->getImageLayerByName(layerName));
		if (!layer)
		{
			return false;
		}
		return layer->getEnabled();
	}

	//elevation
	void TerrainSystem::addElevationLayer(const std::string& layerName, const osgEarth::TileSourceOptions& layerOption)
	{
		if (!_app) return;
		if (!_app->getMap()) return;

		osgEarth::Config arg;

		_app->getRender()->addOperation(new LayerOperation(this, &addElevationlayerMethod, arg));
	}

	void TerrainSystem::removeElevationLayer(const std::string& layerName)
	{
		if (!_app) return;
		if (!_app->getMap()) return;

		osgEarth::Config arg;
		_app->getRender()->addOperation(new LayerOperation(this, &removeElevationLayerMethod, arg));

		//osgEarth::ElevationLayer* layer = (_app->getMap()->getElevationLayerByName(layerName));
		//if (!layer)
		//{
		//	return;
		//}
		//_app->getMap()->beginUpdate();
		//_app->getMap()->removeElevationLayer(layer);
		//_app->getMap()->endUpdate();
	}

	void TerrainSystem::removeAllElevationLayers()
	{

	}

	void TerrainSystem::moveElevationLayer(const std::string& layerName, int newIndex)
	{
		if (!_app) return;
		if (!_app->getMap()) return;

		osgEarth::Config arg;
		_app->getRender()->addOperation(new LayerOperation(this, &moveElevationLayerMethod, arg));
		//osgEarth::ElevationLayer* layer = (_app->getMap()->getElevationLayerByName(layerName));
		//if (!layer)
		//	return;

		//_app->getMap()->beginUpdate();
		//_app->getMap()->moveElevationLayer(layer, newIndex);
		//_app->getMap()->endUpdate();
	}

	osgEarth::ElevationLayer* TerrainSystem::getElevationLayer(const std::string& layerName)
	{
		if (!_app) return NULL;
		if (!_app->getMap()) return NULL;

		osgEarth::ElevationLayer* layer = (_app->getMap()->getElevationLayerByName(layerName));
		return layer;
	}

	void TerrainSystem::getAllElevationLayers(osgEarth::ElevationLayerVector& elevationLayers)
	{
		if (!_app) return;
		if (!_app->getMap()) return;

		//osgEarth::ElevationLayerVector layers;
		_app->getMap()->getElevationLayers(elevationLayers);
		//return layers;
	}

	void TerrainSystem::setElevationLayerVisible(const std::string& layerName, bool visible)
	{
		if (!_app) return;
		if (!_app->getMap()) return;

		osgEarth::ElevationLayer* layer = (_app->getMap()->getElevationLayerByName(layerName));
		if (!layer)
		{
			return;
		}
		layer->setVisible(visible);
	}

	bool TerrainSystem::getElevationLayerVisible(const std::string& layerName)
	{
		if (!_app) return false;
		if (!_app->getMap()) return false;

		osgEarth::ElevationLayer* layer = (_app->getMap()->getElevationLayerByName(layerName));
		if (!layer)
		{
			return false;
		}
		return layer->getVisible();
	}

	void TerrainSystem::addElevationLayerCallback(const std::string& layerName, osgEarth::ElevationLayerCallback* ecb)
	{
		if (!_app) return;
		if (!_app->getMap()) return;

		osgEarth::ElevationLayer* layer = (_app->getMap()->getElevationLayerByName(layerName));
		if (!layer)
		{
			return;
		}
		layer->addCallback(ecb);
	}

	void TerrainSystem::removeElevationLayerCallback(const std::string& layerName, osgEarth::ElevationLayerCallback* ecb)
	{
		if (!_app) return;
		if (!_app->getMap()) return;

		osgEarth::ElevationLayer* layer = (_app->getMap()->getElevationLayerByName(layerName));
		if (!layer)
		{
			return;
		}
		layer->removeCallback(ecb);
	}

	bool TerrainSystem::getElevationLayerEnable(const std::string& layerName)
	{
		if (!_app) return false;
		if (!_app->getMap()) return false;

		osgEarth::ElevationLayer* layer = (_app->getMap()->getElevationLayerByName(layerName));
		if (!layer)
		{
			return false;
		}
		return layer->getEnabled();
	}

	void TerrainSystem::addMaskLayer(osgEarth::MaskLayer* masklayer)
	{
		if (!_app) return;
		if (!_app->getMap()) return;

		_app->getMap()->beginUpdate();
		_app->getMap()->addTerrainMaskLayer(masklayer);
		_app->getMap()->endUpdate();
	}

	void TerrainSystem::removeMaskLayer(osgEarth::MaskLayer* masklayer)
	{
		if (!_app) return;
		if (!_app->getMap()) return;

		_app->getMap()->beginUpdate();
		_app->getMap()->removeTerrainMaskLayer(masklayer);
		_app->getMap()->endUpdate();
	}

	osgEarth::MaskLayer* TerrainSystem::getMaskLayer(const std::string& layername)
	{
		if (!_app) return NULL;
		if (!_app->getMap()) return NULL;

		osgEarth::MaskLayerVector mlv;
		_app->getMap()->getTerrainMaskLayers(mlv);
		for (osgEarth::MaskLayerVector::const_iterator it = mlv.begin(); it != mlv.end(); ++it)
		{
			if ((*it)->getName() == layername)
			{
				return (*it);
			}
		}
		return NULL;
	}

	void TerrainSystem::getMaskLayerVector(osgEarth::MaskLayerVector& mlv)
	{
		if (!_app) return;
		if (!_app->getMap()) return;

		_app->getMap()->getTerrainMaskLayers(mlv);
	}

	osg::Node * TerrainSystem::getTerrainNode()
	{
		return nullptr;
	}

	void TerrainSystem::addImageLayerMethod(const osgEarth::Config & arg)
	{
		if (!_app->getMap())
			return;

		osgEarth::TileSourceOptions options;
		if (!getTilesourceOptions(arg, options))
			return;

		std::string layername = arg.value("layer_name");
		if (layername.empty())
		{
			//GW_LOG();
			return;
		}

		_app->getMap()->beginUpdate();
		_app->getMap()->addImageLayer(new osgEarth::ImageLayer(layername, options));
		_app->getMap()->endUpdate();
	}

	void TerrainSystem::moveImageLayerMethod(const osgEarth::Config & arg)
	{
		std::string layername = arg.value("layer_name");

		unsigned int newindex = arg.value("new_index", -1);
		if (newindex == -1) return;

		osgEarth::ImageLayer* layer = (_app->getMap()->getImageLayerByName(layername));
		if (!layer) return;

		_app->getMap()->beginUpdate();
		_app->getMap()->moveImageLayer(layer, newindex);
		_app->getMap()->endUpdate();
	}

	void TerrainSystem::removeImageLayerMethod(const osgEarth::Config & arg)
	{
		std::string layername = arg.value("layer_name");
		osgEarth::ImageLayer* layer = (_app->getMap()->getImageLayerByName(layername));
		if (!layer) return;

		_app->getMap()->beginUpdate();
		_app->getMap()->removeImageLayer(layer);
		_app->getMap()->endUpdate();
	}

	void TerrainSystem::insertImageLayerMethod(const osgEarth::Config & arg)
	{
		if (!_app->getMap())
			return;

		osgEarth::TileSourceOptions options;
		if (!getTilesourceOptions(arg, options))
			return;

		std::string layername = arg.value("layer_name");
		if (layername.empty())
		{
			//GW_LOG();
			return;
		}

		unsigned int index = arg.value("layer_index", -1);
		if (index == -1)
		{
			return;
		}

		_app->getMap()->beginUpdate();
		_app->getMap()->insertImageLayer(new osgEarth::ImageLayer(layername, options), index);
		_app->getMap()->endUpdate();
	}

	void TerrainSystem::addElevationlayerMethod(const osgEarth::Config & arg)
	{
		if (!_app->getMap())
			return;

		osgEarth::TileSourceOptions options;
		if (!getTilesourceOptions(arg, options))
			return;

		std::string layername = arg.value("layer_name");
		if (layername.empty())
		{
			//GW_LOG();
			return;
		}
		_app->getMap()->beginUpdate();
		_app->getMap()->addElevationLayer(new osgEarth::ElevationLayer(layername, options));
		_app->getMap()->endUpdate();
	}

	void TerrainSystem::moveElevationLayerMethod(const osgEarth::Config & arg)
	{
		std::string layername = arg.value("layer_name");

		unsigned int newindex = arg.value("new_index", -1);
		if (newindex == -1) return;

		osgEarth::ElevationLayer* layer = (_app->getMap()->getElevationLayerByName(layername));
		if (!layer) return;

		_app->getMap()->beginUpdate();
		_app->getMap()->moveElevationLayer(layer, newindex);
		_app->getMap()->endUpdate();

	}

	void TerrainSystem::removeElevationLayerMethod(const osgEarth::Config & arg)
	{
		std::string layername = arg.value("layer_name");
		osgEarth::ElevationLayer* layer = (_app->getMap()->getElevationLayerByName(layername));
		if (!layer) return;

		_app->getMap()->beginUpdate();
		_app->getMap()->removeElevationLayer(layer);
		_app->getMap()->endUpdate();
	}


	void TerrainSystem::addMaskLayerMethod(const osgEarth::Config & arg)
	{
	}

	void TerrainSystem::removeMaskLayerMethod(const osgEarth::Config & arg)
	{
	}

}
