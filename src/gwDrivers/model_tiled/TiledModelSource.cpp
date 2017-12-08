#include "TiledModelOptions.h"
#include <osgEarth/ModelSource>
#include <osgEarth/Registry>
#include <osgEarth/Map>
#include <osgEarth/ShaderGenerator>
#include <osgEarth/FileUtils>
#include <osgEarth/StateSetCache>
#include <osg/CullStack>
#include <osg/LOD>
#include <osg/ProxyNode>
#include <osg/Notify>
#include <osg/MatrixTransform>
#include <osg/io_utils>
#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>

#define VBO_PL_EXTENSION "gw_vbo_gen"

using namespace osgEarth;
using namespace gwDrivers;

namespace
{
	//使用VBO绘制方式;
	class VBOAplly :public osg::NodeVisitor
	{
	public:
		VBOAplly() :osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN) {}

		virtual void apply(osg::Geode& geode)
		{
			for (unsigned int i = 0; i < geode.getNumDrawables(); ++i)
			{
				osg::Geometry* geom = dynamic_cast<osg::Geometry*>(geode.getDrawable(i));
				if (!geom) continue;

				//
				osg::Vec4Array* ca = dynamic_cast<osg::Vec4Array*>(geom->getColorArray());
				if (ca && ca->size() > 1)
				{
					osg::ref_ptr<osg::Vec4Array> newCa = new osg::Vec4Array;
					newCa->push_back(ca->at(0));
					geom->setColorArray(newCa, osg::Array::BIND_OVERALL);
				}
				geom->setUseDisplayList(false);
				geom->setUseVertexBufferObjects(true);
			}
		}
	};

	//读写插件;
	struct VBOPseudoLoader : public osgDB::ReaderWriter
	{
		VBOPseudoLoader()
		{
			this->supportsExtension(VBO_PL_EXTENSION, "vbo pseudoloader");
		}

		const char* className() const { return "gw vbo gen pseudoloader"; }

		bool acceptsExtension(const std::string& extension) const
		{
			return osgDB::equalCaseInsensitive(extension, VBO_PL_EXTENSION);
		}

		ReadResult readObject(const std::string& filename, const osgDB::Options* options) const
		{
			return readNode(filename, options);
		}

		ReadResult readNode(const std::string& filename, const osgDB::Options* options) const
		{
			if (!acceptsExtension(osgDB::getFileExtension(filename)))
				return ReadResult::FILE_NOT_HANDLED;

			std::string stripped = osgDB::getNameLessExtension(filename);

			osgEarth::ReadResult result = osgEarth::URI(stripped).readNode(options);
			if (result.succeeded() && result.getNode() != 0L)
			{
				osg::ref_ptr<osg::Node> node = result.releaseNode();
				VBOAplly vbov;
				node->accept(vbov);
				return ReadResult(node);
			}
			else
			{
				OE_WARN << "VBOPseudoLoader" << "Error loading \"" << stripped << "\": " << result.errorDetail() << "\n";
				return ReadResult::ERROR_IN_READING_FILE;
			}
		}
	};
	REGISTER_OSGPLUGIN(VBO_PL_EXTENSION, VBOPseudoLoader)

	//调用读写插件;
	class VBOGen : public osg::NodeVisitor
	{
	public:
		VBOGen(void)
		{
			setTraversalMode(TRAVERSE_ALL_CHILDREN);
			setNodeMaskOverride(~0);
		}
		~VBOGen(void) {}

		virtual void apply(osg::PagedLOD& node)
		{
			for (unsigned i = 0; i < node.getNumFileNames(); ++i)
			{
				static osgEarth::Threading::Mutex s_mutex;
				s_mutex.lock();
				const std::string& filename = node.getFileName(i);
				if (!filename.empty() && osgDB::getLowerCaseFileExtension(filename).compare(VBO_PL_EXTENSION) != 0)
				{
					node.setFileName(i, osgEarth::Stringify() << filename << "." << VBO_PL_EXTENSION);
				}
				s_mutex.unlock();
			}
		}

		virtual void apply(osg::ProxyNode& node)
		{
			if (node.getLoadingExternalReferenceMode() != osg::ProxyNode::LOAD_IMMEDIATELY)
			{
				for (unsigned i = 0; i < node.getNumFileNames(); ++i)
				{
					const std::string& filename = node.getFileName(i);
					if (!filename.empty() &&
						osgDB::getLowerCaseFileExtension(filename).compare(VBO_PL_EXTENSION) != 0)
					{
						node.setFileName(i, osgEarth::Stringify() << filename << "." << VBO_PL_EXTENSION);
					}
				}
			}
		}
	};

	//纹理内存缓存;
	class ReadAndShareImageCallback : public osgDB::ReadFileCallback
	{
	public:
		ReadAndShareImageCallback(osgEarth::MemCache* memcache, const TiledModelOptions& options)
			:_memCache(memcache), _options(options)
		{ }

		virtual osgDB::ReaderWriter::ReadResult readImage(
			const std::string& filename,
			const osgDB::Options* options)
		{
			if (_memCache)
			{
				osgEarth::ReadResult r = _memCache->getOrCreateDefaultBin()->readImage(filename, 0L);
				if (r.succeeded()) return r.releaseImage();
			}

			osg::ref_ptr<osg::Image> image = 0L;
			osgEarth::URI imageUri = _options.url().value().append(osgDB::getNameLessExtension(filename));
			image = imageUri.readImage().getImage();
			if (image)
			{
				if (_memCache)
				{
					_memCache->getOrCreateDefaultBin()->write(filename, image.get(), 0L);
				}
			}
			return image;
		}

	private:
		osgEarth::MemCache* _memCache;
		const TiledModelOptions _options;
	};

	//遍历文件夹;
	class TiledDirectoryVisitor
	{
	public:
		TiledDirectoryVisitor(osg::Group* root) :_root(root) {}

		virtual bool handleDir(const std::string& path) { return true; }
		virtual void traverse(const std::string& path)
		{
			if (osgDB::fileType(path) == osgDB::DIRECTORY)
			{
				if (handleDir(path))
				{
					osgDB::DirectoryContents files = osgDB::getDirectoryContents(path);
					for (osgDB::DirectoryContents::const_iterator f = files.begin(); f != files.end(); ++f)
					{
						if (f->compare(".") == 0 || f->compare("..") == 0)
							continue;

						std::string filepath = osgDB::concatPaths(path, *f);
						std::string filename = osgDB::concatPaths(filepath, *f + ".osgb");
						if (osgDB::fileExists(filename))
						{
							osgEarth::URI uri(filename);
							osg::ref_ptr<osg::Node> node = uri.readNode().getNode();
							if (node.valid())
							{
								VBOGen vbov;
								node->accept(vbov);
								_root->addChild(node);
							}
						}
						else
						{
							traverse(filepath);
						}
					}
				}
			}
		}
	private:
		osg::Group* _root;
	};
}

//
class TiledModelSource : public ModelSource
{
public:
	TiledModelSource(const ModelSourceOptions& options)
		: ModelSource(options), _options(options)
	{ }

	//override
	Status initialize(const osgDB::Options* dbOptions)
	{
		_dbOptions = dbOptions;
		//ModelSource::initialize( dbOptions );
		return Status::OK();
	}

	// override
	osg::Node* createNodeImplementation(const Map* map, ProgressCallback* progress)
	{
		const osg::Vec3d& pos = _options.location().value();
		osgEarth::GeoPoint point(
			map->getProfile()->getSRS(),
			pos[0],
			pos[1],
			pos[2],
			ALTMODE_ABSOLUTE);

		osg::Matrixd matrix;
		point.createLocalToWorld(matrix);

		osg::ref_ptr<osg::MatrixTransform> result = new osg::MatrixTransform;
		result->setMatrix(matrix);

		TiledDirectoryVisitor tdv(result);
		tdv.traverse(_options.url().value().base());

		osgEarth::Registry::shaderGenerator().run(result, "tiled.ModelNode", Registry::stateSetCache());

		return result.release();
	}

protected:
	const TiledModelOptions            _options;
	osg::ref_ptr<const osgDB::Options> _dbOptions;
};

//
class TiledModelSourceFactory : public ModelSourceDriver
{
public:
	TiledModelSourceFactory()
	{
		supportsExtension("osgearth_model_tiled", "gwEarth Tiled model plugin");
	}

	virtual const char* className() const
	{
		return "gwEarth Tiled Model Plugin";
	}

	virtual ReadResult readObject(const std::string& file_name, const Options* options) const
	{
		if (!acceptsExtension(osgDB::getLowerCaseFileExtension(file_name)))
			return ReadResult::FILE_NOT_HANDLED;

		return ReadResult(new TiledModelSource(getModelSourceOptions(options)));
	}
};

REGISTER_OSGPLUGIN(osgearth_model_tiled, TiledModelSourceFactory)