#include<gwGeological/GeologicalSystem.h>

namespace gwGeological
{
	GeologicalSystem::GeologicalSystem(gwApp::IApplication* app)
		: IGeologicalSystem(app)
		, _mapNode(app->getMapNode())
		, _map(app->getMap())
	{ }

	void GeologicalSystem::init()
	{
	}

	void GeologicalSystem::shutdown()
	{
	}

	void GeologicalSystem::update(double dt)
	{
	}

	void GeologicalSystem::initSystemFromConfig(const osgEarth::Config & systemConfig)
	{
	}

	void GeologicalSystem::addLayer(const osgEarth::Config & layerconfig)
	{
	}

	void GeologicalSystem::addLayer(IGeologicalLayer * layer)
	{
		if (!_map || !layer) return;
		if (getLayer(layer->getName()))
		{
			//rename your layer
			return;
		}
		_map->beginUpdate();
		_map->addModelLayer(layer);
		_map->endUpdate();
		fireCallback(layer, &GeologicalSystemCallback::onLayerAdded);
	}

	void GeologicalSystem::removeLayer(IGeologicalLayer * layer)
	{
		if (!_map || !layer) return;
		_map->beginUpdate();
		_map->removeModelLayer(layer);
		_map->endUpdate();
		fireCallback(layer, &GeologicalSystemCallback::onLayerRemoved);
	}

	void GeologicalSystem::removeLayer(const std::string & layername)
	{
		removeLayer(getLayer(layername));
	}

	void GeologicalSystem::removeAllLayers()
	{
		IGeologicalLayerVector layers; getLayers(layers);
		for (IGeologicalLayerVector::const_iterator it = layers.begin(); it != layers.end(); ++it)
		{
			removeLayer(*it);
		}
	}

	IGeologicalLayer * GeologicalSystem::getLayer(const std::string & layer_name)
	{
		if (!_map) return 0L;
		return dynamic_cast<IGeologicalLayer*>(_map->getModelLayerByName(layer_name));
	}

	void GeologicalSystem::getLayers(IGeologicalLayerVector & out_layers)
	{
		if (!_map) return;
		osgEarth::ModelLayerVector mlv;
		_map->getModelLayers(mlv);
		for (osgEarth::ModelLayerVector::const_iterator it = mlv.begin(); it != mlv.end(); ++it)
		{
			IGeologicalLayer* layer = dynamic_cast<IGeologicalLayer*>((*it).get());
			if (!layer) continue;
			out_layers.push_back(layer);
		}
	}

	void GeologicalSystem::queryAnalysis()
	{
	}

	osg::Node * GeologicalSystem::getLayerNode(const std::string & layername) const
	{
		return nullptr;
	}

	void GeologicalSystem::fenceCuttingAnalysis()
	{
	}

	void GeologicalSystem::signalCuttingAnalysis()
	{
	}

	void GeologicalSystem::continueCuttingAnalysis()
	{
	}

	void GeologicalSystem::tunnelAnalysis()
	{
	}

}
