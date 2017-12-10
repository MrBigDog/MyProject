#ifndef TILEREGISTER_H
#define TILEREGISTER_H 1

#include <osg/Node>
#include <sstream>

static std::string makeTileKey(int l, int x, int y)
{
	std::stringstream ss;
	ss << l << "_" << x << "_" << y;
	return ss.str();
}

struct TileNode : public osg::Referenced
{
	TileNode(unsigned int l, unsigned int x, unsigned int y, osg::Node* node)
		: _l(l), _x(x), _y(y), _tileNode(node) {}

	unsigned int _l;
	unsigned int _x;
	unsigned int _y;
	osg::ref_ptr<osg::Node> _tileNode;
};
typedef std::map<std::string, osg::ref_ptr<TileNode> > TileNodeMap;

class GeologicalLayer0;

class TileRegister :public osg::Referenced
{
public:
	struct Callback :public osg::Referenced
	{
		virtual void onTileAdded(GeologicalLayer0*layer, TileNode* tileNode) = 0;
		virtual void onTileRemoved(GeologicalLayer0*layer, TileNode* tileNode) = 0;
	};
	typedef std::vector<osg::ref_ptr<Callback> > CallbackVector;

	void addTile(GeologicalLayer0*layer, TileNode* tileNode);
	void removeTile(GeologicalLayer0*layer, TileNode* tileNode);

	void addCallback(Callback* cb);
	void removeCallback(Callback* cb);

	const TileNodeMap& getTiles() const { return _tiles; }

private:
	TileNodeMap _tiles;//当前场景内的瓦片;
	CallbackVector _callbacks;
};

#endif // TileRegister_h__
