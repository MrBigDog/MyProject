#include "TileRegister.h"

void TileRegister::addTile(GeologicalLayer0*layer, TileNode * tileNode)
{
	if (!tileNode) return;
	_tiles[makeTileKey(tileNode->_l, tileNode->_x, tileNode->_y)] = tileNode;

	for (CallbackVector::const_iterator it = _callbacks.begin(); it != _callbacks.end(); ++it)
	{
		(*it)->onTileAdded(layer, tileNode);
	}
}

void TileRegister::removeTile(GeologicalLayer0*layer, TileNode * tileNode)
{
	if (!tileNode) return;
	TileNodeMap::iterator it = _tiles.find(makeTileKey(tileNode->_l, tileNode->_x, tileNode->_y));
	if (it != _tiles.end())
	{
		_tiles.erase(it);
	}
	for (CallbackVector::const_iterator it = _callbacks.begin(); it != _callbacks.end(); ++it)
	{
		(*it)->onTileRemoved(layer, tileNode);
	}
}

void TileRegister::addCallback(Callback * cb)
{
	if (!cb) return;
	_callbacks.push_back(cb);
}

void TileRegister::removeCallback(Callback * cb)
{
	if (!cb) return;
	CallbackVector::iterator it = std::find(_callbacks.begin(), _callbacks.end(), cb);
	if (it == _callbacks.end()) return;
	_callbacks.erase(it);
}
