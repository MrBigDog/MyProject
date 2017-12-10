#ifndef MAPNODE_H
#define MAPNODE_H 1

#include "Map.h"
#include "TileRegister.h"

#include <osg/Group>
#include <map>

class MapNode :public osg::Group
{
public:
	MapNode(Map* map);
	~MapNode();

	void onGeologicalLayerAdded(GeologicalLayer0* layer);
	void onGeologicalLayerRemoved(GeologicalLayer0* layer);
	osg::Node* getGeologicalLayerNode(GeologicalLayer0* layer);

	void onGeologicalProfileLayerAdded(GeologicalProfileLayer* layer);
	void onGeologicalProfileLayerRemoved(GeologicalProfileLayer* layer);
	osg::Node* getGeologicalProfileLayerNode(GeologicalProfileLayer* layer);

private:
	osg::ref_ptr<Map> _map;
	osg::ref_ptr<osg::Group> _geologicalRoot;
	osg::ref_ptr<osg::Group> _geologicalProfileRoot;

	typedef std::map<GeologicalLayer0*, osg::ref_ptr<osg::Node> > GeologicalLayerNodeMap;
	GeologicalLayerNodeMap _geologicalLayerNodeMap;

	typedef std::map<GeologicalProfileLayer*, osg::ref_ptr<osg::Node> > GeologicalProfileLayerNodeMap;
	GeologicalProfileLayerNodeMap _profileLayerNodeMap;
};

#endif // MapNode_h__
