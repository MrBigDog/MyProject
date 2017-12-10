#include "GeologicalLayer.h"

#include <osg/PagedLOD>
#include <osg/ValueObject>
#include <osgDB/Options>
#include <osgDB/ReadFile>
#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>
#include <osgDB/ReaderWriter>

namespace
{
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
				_tiles[tkey] = &node;
			}
			traverse(node);
		}
		const std::map<std::string, osg::ref_ptr<osg::Node> >& getTiles() const
		{
			return _tiles;
		}

	private:
		std::map<std::string, osg::ref_ptr<osg::Node> > _tiles;
	};

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

	//
	class PagedLodVisitor :public osg::NodeVisitor
	{
	public:
		PagedLodVisitor()
			: osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN)
		{}

		void setOptions(const osgDB::Options* options)
		{
			_readoptions = options;
		}

		void apply(osg::PagedLOD& pg)
		{
			for (unsigned int i = 0; i < pg.getNumFileNames(); ++i)
			{
				std::string filename = pg.getFileName(i);
				if (filename.empty()) continue;

				filename = filename + ".geological_tile";
				pg.setFileName(i, filename);
			}
			//pg.setDatabaseOptions(_readoptions);
		}

	private:
		osg::ref_ptr< const osgDB::Options> _readoptions;
	};

	//
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

			osgDB::ReaderWriter::ReadResult readresult = rw->readNode(realUri, options);
			if (!readresult.success())
			{
				return ReadResult::FILE_NOT_HANDLED;
			}
			osg::Node* resultnode = readresult.takeNode();

			//在这里添加profile;

			if (resultnode)
			{
				PagedLodVisitor plv;
				resultnode->accept(plv);
			}
			return resultnode;
		}
	};
	REGISTER_OSGPLUGIN(geological_tile, GeologicalModelReaderWriter)


	struct NodeModelSource : public GeologicalModelSource
	{
		NodeModelSource(osg::Node* node) : _node(node) { }
		osg::Node* createNodeImplementation()
		{
			return _node.get();
		}
	private:
		osg::ref_ptr<osg::Node> _node;
	};

	struct UrlModelSource :public GeologicalModelSource
	{
		UrlModelSource(const std::string& url) :_url(url) {}
		osg::Node* createNodeImplementation()
		{
			//std::string ext = osgDB::getLowerCaseFileExtension(_url);
			return  osgDB::readNodeFile(_url);
		}
	private:
		std::string _url;
	};
}

//////////////////////////////////////////////////////////////////////////
GeologicalLayer0::GeologicalLayer0(const std::string& name, const std::string& url)
	: _name(name)
	, _modelSource(new UrlModelSource(url))
	, _tileRegister(new TileRegister)
{ }

GeologicalLayer0::GeologicalLayer0(const std::string & name, GeologicalModelSource * modelSource)
	: _name(name)
	, _modelSource(modelSource)
	, _tileRegister(new TileRegister)
{ }

GeologicalLayer0::GeologicalLayer0(const std::string & name, osg::Node * node)
	: _name(name)
	, _modelSource(new NodeModelSource(node))
	, _tileRegister(new TileRegister)
{ }

GeologicalLayer0::~GeologicalLayer0()
{ }

const std::string & GeologicalLayer0::getName() const
{
	return _name;
}

void GeologicalLayer0::setVisible(bool isVisible)
{
	if (_isVisible == isVisible) return;
	_isVisible = isVisible;

	_graph->setNodeMask(isVisible ? ~0 : 0);
}

bool GeologicalLayer0::getVisible() const
{
	return _isVisible;
}

void GeologicalLayer0::setLightEnable(bool isLightEnable)
{
	if (_isLightEnable == isLightEnable) return;
	_isLightEnable = isLightEnable;

	_graph->getOrCreateStateSet()->setMode(GL_LIGHTING, _isLightEnable ? osg::StateAttribute::ON : osg::StateAttribute::OFF);
}

bool GeologicalLayer0::getLightEnable() const
{
	return _isLightEnable;
}

void GeologicalLayer0::addCallback(Callback * cb)
{
	if (!cb) return;
	_callbacks.push_back(cb);
}

void GeologicalLayer0::removeCallback(Callback * cb)
{
	if (!cb) return;
	CallbackVector::iterator it = std::find(_callbacks.begin(), _callbacks.end(), cb);
	if (it == _callbacks.end()) return;
	_callbacks.erase(it);
}

osg::Node * GeologicalLayer0::getOrCreateSceneGraph()
{
	osg::Node* node = _modelSource->createNode();
	if (!node) return 0L;

	_graph = node;
	return _modelSource->createNode();
}
