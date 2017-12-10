#include "Map.h"

Map::Map() { }
Map::~Map() { }

void Map::addGeologicalLayer(GeologicalLayer0* layer)
{
	if (!layer) return;
	_geologicalLayers.push_back(layer);
	for (CallbackVector::const_iterator it = _callbacks.begin(); it != _callbacks.end(); ++it)
	{
		(*it)->onGeologicalLayerAdded(layer);
	}
}

void Map::removeGeologicalLayer(GeologicalLayer0 * layer)
{
	if (!layer) return;

	GeologicalLayer0Vector::iterator it = std::find(_geologicalLayers.begin(), _geologicalLayers.end(), layer);
	if (it == _geologicalLayers.end()) return;

	_geologicalLayers.erase(it);
	for (CallbackVector::const_iterator it = _callbacks.begin(); it != _callbacks.end(); ++it)
	{
		(*it)->onGeologicalLayerRemoved(layer);
	}
}

void Map::removeGeologicalLayer(const std::string & layerName)
{
	GeologicalLayer0* layer = getGeologicalLayer(layerName);
	removeGeologicalLayer(layer);
}

void Map::addGeologicalProfileLayer(GeologicalProfileLayer * layer)
{
	if (!layer) return;
	_geologicalProfileLayers.push_back(layer);
	for (CallbackVector::const_iterator it = _callbacks.begin(); it != _callbacks.end(); ++it)
	{
		(*it)->onGeologicalProfileLayerAdded(layer);
	}
}

void Map::removeGeologicalProfileLayer(GeologicalProfileLayer * layer)
{
	if (!layer) return;
	GeologicalProfileLayerVector::iterator it =
		std::find(_geologicalProfileLayers.begin(), _geologicalProfileLayers.end(), layer);
	if (it == _geologicalProfileLayers.end()) return;

	_geologicalProfileLayers.erase(it);

	for (CallbackVector::const_iterator it = _callbacks.begin(); it != _callbacks.end(); ++it)
	{
		(*it)->onGeologicalProfileLayerRemoved(layer);
	}
}

void Map::removeGeologicalProfileLayer(const std::string & layerName)
{
	GeologicalProfileLayer* layer = getGeologicalProfileLayer(layerName);
	removeGeologicalProfileLayer(layer);
}

GeologicalProfileLayer * Map::getGeologicalProfileLayer(const std::string & layerName)
{
	for (GeologicalProfileLayerVector::const_iterator it = _geologicalProfileLayers.begin(); it != _geologicalProfileLayers.end(); ++it)
	{
		if ((*it)->getName() == layerName) return *it;
	}
	return 0L;
}

void Map::getGeologicalProfileLayers(GeologicalProfileLayerVector & out_layers)
{
	if (!out_layers.empty()) out_layers.clear();
	out_layers.reserve(_geologicalProfileLayers.size());
	for (GeologicalProfileLayerVector::const_iterator it = _geologicalProfileLayers.begin(); it != _geologicalProfileLayers.end(); ++it)
	{
		out_layers.push_back(*it);
	}
}

GeologicalLayer0 * Map::getGeologicalLayer(const std::string & layerName)
{
	for (GeologicalLayer0Vector::const_iterator it = _geologicalLayers.begin(); it != _geologicalLayers.end(); ++it)
	{
		if ((*it)->getName() == layerName)
		{
			return *it;
		}
	}
	return 0L;
}

void Map::getGeologicalLayers(GeologicalLayer0Vector & out_layers)
{
	if (!out_layers.empty()) out_layers.clear();
	out_layers.reserve(_geologicalLayers.size());
	for (GeologicalLayer0Vector::const_iterator it = _geologicalLayers.begin(); it != _geologicalLayers.end(); ++it)
	{
		out_layers.push_back(*it);
	}
}

void Map::addCallback(Callback * cb)
{
	if (!cb) return;
	_callbacks.push_back(cb);
}

void Map::removeCallback(Callback * cb)
{
	if (!cb) return;
	CallbackVector::iterator it = std::find(_callbacks.begin(), _callbacks.end(), cb);
	if (it == _callbacks.end()) return;
	_callbacks.erase(it);
}
