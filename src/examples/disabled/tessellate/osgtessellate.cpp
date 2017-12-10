/************************************************************************/
/* 测试Tornado Server                                                   */
/************************************************************************/

#include <osgEarth/HTTPClient>
#include <osgEarth/Notify>
#include <osgEarth/Config>
#include <osgEarth/NodeUtils>

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/PagedLOD>
#include <osg/ValueObject>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgDB/ReaderWriter>
#include <osgDB/FileNameUtils>

#include <osgGA/TrackballManipulator>
#include <osgGA/StateSetManipulator>

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <ostream>
#include <fstream>

using namespace osgEarth;

struct TileNode : public osg::Referenced
{
	TileNode(unsigned int l, unsigned int x, unsigned int y, osg::Node* node)
		: _l(l), _x(x), _y(y), _tileNode(node) {}

	unsigned int _l;
	unsigned int _x;
	unsigned int _y;
	osg::ref_ptr<osg::Node> _tileNode;
};

typedef std::map<std::string, osg::ref_ptr<TileNode> > NodeMap;

osg::ref_ptr<osgDB::Options> _readoptions;

//纹理共享;
class ReadAndShareImageCallback : public osgDB::ReadFileCallback
{
public:
	ReadAndShareImageCallback() { }
	virtual osgDB::ReaderWriter::ReadResult readImage(const std::string& filename, const osgDB::Options* options)
	{
		osg::ref_ptr<osg::Image> image = _imageCache[filename];
		if (image) return image;

		osgDB::ReaderWriter::ReadResult rr;
		rr = osgDB::Registry::instance()->readImageImplementation(filename, options);
		if (rr.success())
		{
			image = rr.getImage();
			_imageCache[filename] = image;
		}
		return image;
	}
private:
	std::map<std::string, osg::ref_ptr<osg::Image> > _imageCache;//由于纹理数量不会很多，所以就不用LRU了。
};

//class TileRegister :public osg::Referenced
//{
//public:
//	TileRegister() {}
//
//private:
//	std::vector<osg::ref_ptr<osg::Node> > _newTiles;
//	std::vector<osg::ref_ptr<osg::Node> > _deadTiles;
//};

//std::vector<std::string> _livingTiles;

//class TileVistor :public osg::NodeVisitor
//{
//public:
//	TileVistor() :osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN) {}
//	void apply(osg::Node& node)
//	{
//		std::string tkey;
//		node.getUserValue("tile_key", tkey);
//		if (!tkey.empty())
//		{
//			std::vector<std::string>::const_iterator it = std::find(_livingTiles.begin(), _livingTiles.end(), tkey);
//			if (it == _livingTiles.end())
//			{
//				_livingTiles.push_back(tkey);
//			}
//
//		}
//		traverse(node);
//	}
//};

//class MyPagedLod :public osg::PagedLOD
//{
//public:
//	MyPagedLod() {}
//	MyPagedLod(const osg::PagedLOD& pg) :osg::PagedLOD(pg) {}
//
//	virtual bool addChild(osg::Node* child)
//	{
//		if (!child) return false;
//		if (osg::PagedLOD::addChild(child))
//		{
//			TileVistor tv;
//			child->accept(tv);
//
//			//osg::Group* group = dynamic_cast<osg::Group*>(child);
//			//if (group)
//			//{
//			//	for (unsigned int i = 0; i < group->getNumChildren(); ++i)
//			//	{
//			//		std::string key;
//			//		group->getChild(i)->getUserValue("tile_key", key);
//			//		if (!key.empty())
//			//		{
//			//			_livingTiles.push_back(key);
//			//		}
//			//		else
//			//		{
//			//			osg::PagedLOD* pg = dynamic_cast<osg::PagedLOD*>(group->getChild(i));
//			//			if (pg)
//			//			{
//			//				for (unsigned int j = 0; j < pg->getNumChildren(); ++j)
//			//				{
//			//					std::string key;
//			//					pg->getChild(j)->getUserValue("tile_key", key);
//			//					if (!key.empty())
//			//					{
//			//						_livingTiles.push_back(key);
//			//					}
//			//				}
//			//			}
//			//		}
//
//			//	}
//			//}
//
//			return true;
//		}
//		return false;
//	}
//
//	virtual bool removeExpiredChildren(double expiryTime, unsigned expiryFrame, osg::NodeList& removedChildren)
//	{
//		for (unsigned int i = 0; i < getNumChildren(); ++i)
//		{
//			osg::Node* node = getChild(i);
//			std::string key;
//			node->getUserValue("tile_key", key);
//			//std::cout << i << ": " << key << std::endl;
//			std::vector<std::string>::const_iterator it = std::find(_livingTiles.begin(), _livingTiles.end(), key);
//			if (it != _livingTiles.end())
//			{
//				_livingTiles.erase(it);
//			}
//		}
//		if (osg::PagedLOD::removeExpiredChildren(expiryTime, expiryFrame, removedChildren))
//		{
//			return true;
//		}
//		return false;
//	}
//};

//遍历当前块;
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

	const NodeMap& getTiles() const
	{
		return _tiles;
	}
private:
	NodeMap _tiles;
};

class PagedLodVisitor :public osg::NodeVisitor
{
public:
	PagedLodVisitor() :osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN)
	{ }

	void apply(osg::PagedLOD& pg)
	{
		for (unsigned int i = 0; i < pg.getNumFileNames(); ++i)
		{
			std::string filename = pg.getFileName(i);
			if (filename.empty()) continue;

			filename = filename + ".geological_tile";
			pg.setFileName(i, filename);
		}
		pg.setDatabaseOptions(_readoptions);
	}
};

class GeologicalModelReaderWriter :public osgDB::ReaderWriter
{
public:
	GeologicalModelReaderWriter() { }

	virtual bool acceptsExtension(const std::string& extension) const
	{
		return osgDB::equalCaseInsensitive(extension, "geological_tile");
	}

	virtual ReadResult readNode(const std::string& uri, const Options* options) const
	{
		std::string ext = osgDB::getLowerCaseFileExtension(uri);
		if (!acceptsExtension(ext)) return ReadResult::FILE_NOT_HANDLED;

		std::string realUri = osgDB::getNameLessExtension(uri);

		std::string realExt = osgDB::getLowerCaseFileExtension(realUri);

		osg::ref_ptr<osgDB::ReaderWriter> rw;
		rw = osgDB::Registry::instance()->getReaderWriterForExtension(realExt);
		if (!rw) return ReadResult::FILE_NOT_HANDLED;

		osgDB::ReaderWriter::ReadResult readresult = rw->readNode(realUri, _readoptions);
		if (!readresult.success())
		{
			return ReadResult::FILE_NOT_HANDLED;
		}
		osg::Node* resultnode = readresult.takeNode();
		if (resultnode)
		{
			PagedLodVisitor plv;
			resultnode->accept(plv);
		}
		return resultnode;
	}
};

REGISTER_OSGPLUGIN(geological_tile, GeologicalModelReaderWriter)

std::string makeUrl(unsigned int l, unsigned int x, unsigned int y)
{
	std::stringstream ss;
	ss << "L" << l << "_X" << x << "_Y" << y;
	return ss.str();
}

#include <osgEarth/URI>

int main(int argc, char** argv)
{



	_readoptions = new osgDB::Options;
	_readoptions->setReadFileCallback(new ReadAndShareImageCallback());

	osgEarth::URI uri("D:/BYL/osgART-master/share/osgART/media/models/cow.ive");
	std::string baseurl = uri.base();
	osg::ref_ptr<osg::Node> node = uri.readNode().getNode();
	//osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("D:/MongoDB/mongodbcxxtest/GeologicalTool/GeologicalTool/L0_X0_Y0.OSGB.geological_tile", _readoptions);

	osgViewer::Viewer viewer;
	viewer.setCameraManipulator(new osgGA::TrackballManipulator);
	viewer.addEventHandler(new osgGA::StateSetManipulator(viewer.getCamera()->getOrCreateStateSet()));
	viewer.addEventHandler(new osgViewer::StatsHandler());
	viewer.addEventHandler(new osgViewer::WindowSizeHandler());
	viewer.addEventHandler(new osgViewer::ThreadingHandler());
	viewer.addEventHandler(new osgViewer::LODScaleHandler());
	viewer.addEventHandler(new osgViewer::RecordCameraPathHandler());
	viewer.addEventHandler(new osgViewer::ScreenCaptureHandler());
	viewer.getDatabasePager()->setTargetMaximumNumberOfPageLOD(1);
	viewer.setSceneData(node);
	viewer.realize();

	NodeMap preTtiles;

	while (!viewer.done())
	{
		GeologicalTileVistor mnv;
		node->accept(mnv);

		const NodeMap& ttiles = mnv.getTiles();

		//获取当前的叶子节点;
		NodeMap tiles;
		for (NodeMap::const_iterator it = ttiles.begin(); it != ttiles.end(); ++it)
		{
			const TileNode* tile = it->second;

			unsigned int nextl = tile->_l + 1;
			unsigned int nextx = 2 * tile->_x;
			unsigned int nexty = 2 * tile->_y;

			std::string child1key = makeUrl(nextl, nextx, nexty);
			std::string child2key = makeUrl(nextl, nextx, nexty + 1);
			std::string child3key = makeUrl(nextl, nextx + 1, nexty);
			std::string child4key = makeUrl(nextl, nextx + 1, nexty + 1);

			bool ishaschild = false;
			for (NodeMap::const_iterator rit = mnv.getTiles().begin(); rit != mnv.getTiles().end(); ++rit)
			{
				if (rit->first == child1key ||
					rit->first == child2key ||
					rit->first == child3key ||
					rit->first == child4key)
				{
					ishaschild = true;
					break;
				}
			}
			if (!ishaschild)
			{
				tiles[it->first] = it->second;
			}
		}

		NodeMap newTiles;
		for (NodeMap::const_iterator it = tiles.begin(); it != tiles.end(); ++it)
		{
			bool isfind = false;
			for (NodeMap::const_iterator pit = preTtiles.begin(); pit != preTtiles.end(); ++pit)
			{
				if (pit->first == it->first)
				{
					isfind = true;
					break;
				}
			}
			if (!isfind)
			{
				newTiles[it->first] = it->second;
			}
		}

		NodeMap deadTiles;
		for (NodeMap::const_iterator it = preTtiles.begin(); it != preTtiles.end(); ++it)
		{
			bool isfind = false;
			for (NodeMap::const_iterator pit = tiles.begin(); pit != tiles.end(); ++pit)
			{
				if (pit->first == it->first)
				{
					isfind = true;
					break;
				}
			}
			if (!isfind)
			{
				deadTiles[it->first] = it->second;
			}
		}

		preTtiles.clear();
		for (NodeMap::const_iterator it = tiles.begin(); it != tiles.end(); ++it)
		{
			preTtiles[it->first] = it->second;
		}

		if (!deadTiles.empty() || !newTiles.empty())
		{
			std::cout << "new: " << std::endl;
			for (NodeMap::const_iterator it = newTiles.begin(); it != newTiles.end(); ++it)
			{
				std::cout << it->first << std::endl;
			}

			std::cout << "dead: " << std::endl;
			for (NodeMap::const_iterator it = deadTiles.begin(); it != deadTiles.end(); ++it)
			{
				std::cout << it->first << std::endl;
			}
			std::cout << std::endl;
		}
		viewer.frame();
	}
	viewer.run();

	return 0;
}

//http://172.30.17.129:8010/wjh_model/gy1010jz4a0027