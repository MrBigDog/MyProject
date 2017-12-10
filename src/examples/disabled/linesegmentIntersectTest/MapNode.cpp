#include "MapNode.h"
#include "TileRegister.h"
#include "StringUtils.h"
#include <osg/observer_ptr>
#include <osg/ValueObject>
#include <osg/PolygonMode>
#include <sstream>
#include <iostream>
namespace
{
	struct UpdateMapCallback :public Map::Callback
	{
		UpdateMapCallback(MapNode* node) :_node(node) {}
		virtual void onGeologicalLayerAdded(GeologicalLayer0* layer)
		{
			_node->onGeologicalLayerAdded(layer);
		}
		virtual void onGeologicalLayerRemoved(GeologicalLayer0* layer)
		{
			_node->onGeologicalLayerRemoved(layer);
		}
		virtual void onGeologicalProfileLayerAdded(GeologicalProfileLayer* layer)
		{
			_node->onGeologicalProfileLayerAdded(layer);
		}
		virtual void onGeologicalProfileLayerRemoved(GeologicalProfileLayer*layer)
		{
			_node->onGeologicalProfileLayerRemoved(layer);
		}
	private:
		osg::observer_ptr<MapNode> _node;
	};
	//////////////////////////////////////////////////////////////////////////

	class GeologicalTileVistor :public osg::NodeVisitor
	{
	public:
		GeologicalTileVistor() :osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN) {}
		void apply(osg::Node& node)
		{
			std::string tkey;
			node.getUserValue("tile_key", tkey);
			if (!tkey.empty())
			{
				unsigned int l, x, y;
				node.getUserValue("L", l);
				node.getUserValue("X", x);
				node.getUserValue("Y", y);
				_tiles[tkey] = new TileNode(l, x, y, &node);
			}
			traverse(node);
		}
		const TileNodeMap& getTiles() const { return _tiles; }

	private:
		TileNodeMap _tiles;
	};

	class ModelTiledLayerNode :public osg::Group
	{
	public:
		ModelTiledLayerNode(GeologicalLayer0* layer) :_layer(layer)
		{
			setName(layer->getName());
		}

		GeologicalLayer0* getLayer() const { return _layer.get(); }

		void traverse(osg::NodeVisitor& nv)
		{
			for (unsigned int i = 0; i < _children.size(); ++i)
			{
				GeologicalTileVistor mnv;
				_children[i]->accept(mnv);

				const TileNodeMap& ttiles = mnv.getTiles();

				//获取当前的叶子节点;
				TileNodeMap tiles;
				for (TileNodeMap::const_iterator it = ttiles.begin(); it != ttiles.end(); ++it)
				{
					std::string tilekey = it->first;
					StringVector stringVec;
					StringTokenizer stokenizer(tilekey, stringVec, "_");

					unsigned int nextl = as(stringVec[0], 0) + 1;
					unsigned int nextx = as(stringVec[1], 0) * 2;
					unsigned int nexty = as(stringVec[2], 0) * 2;

					std::string child1key = makeTileKey(nextl, nextx, nexty);
					std::string child2key = makeTileKey(nextl, nextx, nexty + 1);
					std::string child3key = makeTileKey(nextl, nextx + 1, nexty);
					std::string child4key = makeTileKey(nextl, nextx + 1, nexty + 1);

					bool ishaschild = false;
					for (TileNodeMap::const_iterator rit = mnv.getTiles().begin(); rit != mnv.getTiles().end(); ++rit)
					{
						if (rit->first == child1key || rit->first == child2key ||
							rit->first == child3key || rit->first == child4key)
						{
							ishaschild = true; break;
						}
					}
					if (!ishaschild) { tiles[it->first] = it->second; }
				}
				//得到新添加的瓦片;
				for (TileNodeMap::const_iterator it = tiles.begin(); it != tiles.end(); ++it)
				{
					bool isfind = false;
					for (TileNodeMap::const_iterator pit = _preTtiles.begin(); pit != _preTtiles.end(); ++pit)
					{
						if (pit->first == it->first) { isfind = true; break; }
					}
					if (!isfind)
					{
						_layer->getTileRegister()->addTile(_layer, it->second.get());
					}
				}
				//得到需要删除的瓦片;
				for (TileNodeMap::const_iterator it = _preTtiles.begin(); it != _preTtiles.end(); ++it)
				{
					bool isfind = false;
					for (TileNodeMap::const_iterator pit = tiles.begin(); pit != tiles.end(); ++pit)
					{
						if (pit->first == it->first) { isfind = true; break; }
					}
					if (!isfind)
					{
						_layer->getTileRegister()->removeTile(_layer, it->second.get());
					}
				}

				_preTtiles.clear();
				for (TileNodeMap::const_iterator it = tiles.begin(); it != tiles.end(); ++it)
				{
					_preTtiles[it->first] = it->second;
				}
			}
			osg::Group::traverse(nv);
		}

	private:
		std::string _name;
		TileNodeMap _preTtiles;
		osg::ref_ptr<GeologicalLayer0> _layer;
	};

	typedef std::map <osg::Node*, osg::ref_ptr<osg::Node> > NodeMap;
	typedef std::map < GeologicalLayer0*, NodeMap > LayerNodeMap;
	class ProfileLayerNode :public osg::Group
	{
	private:
		//监视瓦片的添加删除--每个地质体模型图层都有独立的TileRegister;
		struct TileNodeListener :public TileRegister::Callback
		{
		public:
			TileNodeListener(ProfileLayerNode* pro) :_profileMapNode(pro) {}
			virtual void onTileAdded(GeologicalLayer0*layer, TileNode* tileNode)
			{
				_profileMapNode->onTileAdded(layer, tileNode->_tileNode);
			}
			virtual void onTileRemoved(GeologicalLayer0*layer, TileNode* tileNode)
			{
				_profileMapNode->onTileRemoved(layer, tileNode->_tileNode);
			}
		private:
			osg::ref_ptr<ProfileLayerNode> _profileMapNode;
		};

		//监视地质体模型图层的添加删除;
		class UpdateGeologicalLayerCallback :public Map::Callback
		{
		public:
			UpdateGeologicalLayerCallback(ProfileLayerNode* profileLayerNode)
				: _profileLayerNode(profileLayerNode) {}
			virtual void onGeologicalLayerAdded(GeologicalLayer0* layer)
			{
				_profileLayerNode->onGeologicalLayerAdded(layer);
			}
			virtual void onGeologicalLayerRemoved(GeologicalLayer0* layer)
			{
				_profileLayerNode->onGeologicalLayerRemoved(layer);
			}
		private:
			osg::ref_ptr<ProfileLayerNode> _profileLayerNode;
		};

	public:
		ProfileLayerNode(Map* map, GeologicalProfileLayer* layer) :_map(map), _layer(layer)
		{
			_map->addCallback(new UpdateGeologicalLayerCallback(this));

			_tileNodeListener = new TileNodeListener(this);
			GeologicalLayer0Vector layers;
			_map->getGeologicalLayers(layers);
			for (GeologicalLayer0Vector::const_iterator it = layers.begin(); it != layers.end(); ++it)
			{
				(*it)->getTileRegister()->addCallback(new TileNodeListener(this));
				onGeologicalLayerAdded(*it);
			}
			if (layer)
			{
				setName(layer->getName() + "_node");
			}

			{//test
				osg::ref_ptr<osg::PolygonMode> pm = new osg::PolygonMode;
				pm->setMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::FILL);
				this->getOrCreateStateSet()->setAttributeAndModes(pm, osg::StateAttribute::OVERRIDE | osg::StateAttribute::ON);
				this->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
			}
		}

		void onGeologicalLayerAdded(GeologicalLayer0* layer)
		{
			if (!_layer || !layer) return;

			layer->getTileRegister()->addCallback(_tileNodeListener);

			const SlicePLaneVector& sliceplanes = _layer->getOptions().getSlicePlanes();
			if (sliceplanes.empty())
			{
				return;
			}

			NodeMap tileProfileMap;
			const TileNodeMap& tiles = layer->getTileRegister()->getTiles();
			for (TileNodeMap::const_iterator it = tiles.begin(); it != tiles.end(); ++it)
			{
				SlicePlaneNodeGenerator spng;
				osg::ref_ptr<osg::Node> tileProfileNode =
					spng(it->second->_tileNode, _layer->getOptions().isOnEarth(), sliceplanes);
				if (!tileProfileNode)
				{
					continue;
				}

				tileProfileMap[it->second->_tileNode] = tileProfileNode;
				this->addChild(tileProfileNode);
			}
			if (!tileProfileMap.empty())
			{
				_profileLayerTiles[layer] = tileProfileMap;
			}
		}

		void onGeologicalLayerRemoved(GeologicalLayer0* layer)
		{
			if (!_layer || !layer) return;

			std::map < GeologicalLayer0*, NodeMap >::iterator it = _profileLayerTiles.find(layer);
			if (it == _profileLayerTiles.end()) return;
			for (NodeMap::const_iterator iit = it->second.begin(); iit != it->second.end(); ++iit)
			{
				this->removeChild(iit->second);
			}
			_profileLayerTiles.erase(it);
		}

		void onTileAdded(GeologicalLayer0*layer, osg::Node* tileNode)
		{
			if (!_layer || !layer || !tileNode) return;

			SlicePlaneNodeGenerator spng;
			osg::ref_ptr<osg::Node> tileProfile =
				spng(tileNode, _layer->getOptions().isOnEarth(), _layer->getOptions().getSlicePlanes());
			if (!tileProfile) return;

			LayerNodeMap::iterator it = _profileLayerTiles.find(layer);
			if (it == _profileLayerTiles.end())
			{
				NodeMap tileProfileMap;
				_profileLayerTiles[layer] = tileProfileMap;
				this->addChild(tileProfile);
				tileProfileMap[tileNode] = tileProfile;
			}
			else
			{
				if (!_profileLayerTiles[layer][tileNode])
				{
					this->addChild(tileProfile);
					it->second[tileNode] = tileProfile;
				}
				else
				{
					this->replaceChild(it->second[tileNode], tileProfile);
					it->second[tileNode] = tileProfile;
				}
			}
		}

		void onTileRemoved(GeologicalLayer0*layer, osg::Node* tileNode)
		{
			if (!_layer || !layer || !tileNode) return;

			LayerNodeMap::iterator it = _profileLayerTiles.find(layer);
			if (it != _profileLayerTiles.end())
			{
				NodeMap::iterator iit = it->second.find(tileNode);
				if (iit != it->second.end())
				{
					this->removeChild(iit->second);
					it->second.erase(iit);
				}
			}
		}

	private:
		osg::ref_ptr<Map> _map;
		osg::ref_ptr<TileNodeListener> _tileNodeListener;
		osg::ref_ptr<GeologicalProfileLayer> _layer;
		LayerNodeMap _profileLayerTiles;
	};
}

//////////////////////////////////////////////////////////////////////////
MapNode::MapNode(Map * map) :_map(map)
{
	//地质体模型根节点;
	_geologicalRoot = new Group;
	_geologicalRoot->setName("GEOLOGICAL_MODEL_ROOT");
	this->addChild(_geologicalRoot);

	//地质体剖面根节点;
	_geologicalProfileRoot = new Group;
	_geologicalProfileRoot->setName("GEOLOGICAL_PROFILE_MODEL_ROOT");
	this->addChild(_geologicalProfileRoot);

	GeologicalLayer0Vector geoLayers;
	_map->getGeologicalLayers(geoLayers);
	for (GeologicalLayer0Vector::const_iterator it = geoLayers.begin(); it != geoLayers.end(); ++it)
	{
		onGeologicalLayerAdded(*it);
	}
	GeologicalProfileLayerVector profileLayers;
	_map->getGeologicalProfileLayers(profileLayers);
	for (GeologicalProfileLayerVector::const_iterator it = profileLayers.begin(); it != profileLayers.end(); ++it)
	{
		onGeologicalProfileLayerAdded(*it);
	}
	_map->addCallback(new UpdateMapCallback(this));
}

MapNode::~MapNode() { }

void MapNode::onGeologicalLayerAdded(GeologicalLayer0 * layer)
{
	if (!layer) return;
	osg::Node* node = layer->getOrCreateSceneGraph();
	if (!node) return;
	osg::ref_ptr<ModelTiledLayerNode> layernode = new  ModelTiledLayerNode(layer);
	layernode->addChild(node);

	_geologicalRoot->addChild(layernode);
	_geologicalLayerNodeMap[layer] = layernode;
}

void MapNode::onGeologicalLayerRemoved(GeologicalLayer0 * layer)
{
	GeologicalLayerNodeMap::iterator it = _geologicalLayerNodeMap.find(layer);
	if (it == _geologicalLayerNodeMap.end()) return;

	_geologicalRoot->removeChild(it->second);
	_geologicalLayerNodeMap.erase(it);
}

osg::Node * MapNode::getGeologicalLayerNode(GeologicalLayer0 * layer)
{
	GeologicalLayerNodeMap::const_iterator it = _geologicalLayerNodeMap.find(layer);
	if (it == _geologicalLayerNodeMap.end()) return 0L;
	return it->second;
}

void MapNode::onGeologicalProfileLayerAdded(GeologicalProfileLayer * layer)
{
	if (!layer) return;

	osg::ref_ptr<ProfileLayerNode> layernode = new ProfileLayerNode(_map, layer);
	layer->setSceneGraph(layernode);

	_geologicalProfileRoot->addChild(layernode);
	_profileLayerNodeMap[layer] = layernode;
}

void MapNode::onGeologicalProfileLayerRemoved(GeologicalProfileLayer * layer)
{
	GeologicalProfileLayerNodeMap::iterator it = _profileLayerNodeMap.find(layer);
	if (it == _profileLayerNodeMap.end()) return;

	_geologicalProfileRoot->removeChild(it->second);
	_profileLayerNodeMap.erase(it);
}

osg::Node * MapNode::getGeologicalProfileLayerNode(GeologicalProfileLayer * layer)
{
	GeologicalProfileLayerNodeMap::const_iterator it = _profileLayerNodeMap.find(layer);
	if (it == _profileLayerNodeMap.end()) return 0L;
	return it->second;
}
