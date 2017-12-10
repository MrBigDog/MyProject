#ifndef MAP_H
#define MAP_H 1

#include "GeologicalLayer.h"
#include "GeologicalProfileLayer.h"

#include <osg/Referenced>
#include <string>

class Map :public osg::Referenced
{
public:
	struct Callback :public osg::Referenced
	{
		virtual void onGeologicalLayerAdded(GeologicalLayer0* layer) {}
		virtual void onGeologicalLayerRemoved(GeologicalLayer0* layer) {}

		virtual void onGeologicalProfileLayerAdded(GeologicalProfileLayer* layer) {}
		virtual void onGeologicalProfileLayerRemoved(GeologicalProfileLayer*layer) {}
	};
	typedef std::vector<osg::ref_ptr<Callback> > CallbackVector;

public:
	Map();
	~Map();

	void addGeologicalLayer(GeologicalLayer0* layer);
	void removeGeologicalLayer(GeologicalLayer0* layer);
	void removeGeologicalLayer(const std::string& layerName);
	GeologicalLayer0* getGeologicalLayer(const std::string& layerName);
	void getGeologicalLayers(GeologicalLayer0Vector& out_layers);

	void addGeologicalProfileLayer(GeologicalProfileLayer* layer);
	void removeGeologicalProfileLayer(GeologicalProfileLayer*layer);
	void removeGeologicalProfileLayer(const std::string& layerName);
	GeologicalProfileLayer* getGeologicalProfileLayer(const std::string& layerName);
	void getGeologicalProfileLayers(GeologicalProfileLayerVector& out_layers);

	void addCallback(Callback* cb);
	void removeCallback(Callback* cb);

private:
	CallbackVector _callbacks;
	GeologicalLayer0Vector _geologicalLayers;
	GeologicalProfileLayerVector _geologicalProfileLayers;
};
#endif // Map_h__
