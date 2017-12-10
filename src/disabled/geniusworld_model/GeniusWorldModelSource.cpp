#include "GeniusWorldModelOptions.h"
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

using namespace osgEarth;
using namespace gwDrivers;

namespace
{
	osg::PagedLOD* createPagedLod()
	{
		osg::ref_ptr<osg::PagedLOD> pg = new osg::PagedLOD;


		return pg.release();
	}


	class TiledDirectoryVisitor
	{
	public:
		TiledDirectoryVisitor(osg::Group* root) :_root(root) {}

		virtual bool handleDir(const std::string& path) { return true; }
		virtual void traverse(const std::string& path)
		{
			osgDB::DirectoryContents files = osgDB::getDirectoryContents(path);
			for (osgDB::DirectoryContents::const_iterator f = files.begin(); f != files.end(); ++f)
			{
				if (f->compare(".") == 0 || f->compare("..") == 0)
					continue;

				std::string filepath = osgDB::concatPaths(path, *f);

				osgDB::DirectoryContents subFiles = osgDB::getDirectoryContents(filepath);


			}
		}

	private:
		osg::Group* _root;
	};
}


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
		osgEarth::GeoPoint point(
			map->getProfile()->getSRS(),
			113.06277,
			22.64785,
			0.0,
			ALTMODE_ABSOLUTE);

		osg::Matrixd matrix;

		point.createLocalToWorld(matrix);

		osg::ref_ptr<osg::MatrixTransform> result = new osg::MatrixTransform;
		result->setMatrix(matrix);

		//root->addChild(osgDB::readNodeFile("D:/BYL/ive/ZZZ8CM/Production_3/Data/Tile_+000_+000/Tile_+000_+000.osgb"));
		TiledDirectoryVisitor tdv(result);
		tdv.traverse("D:/BYL/ive/ZZZ8CM/Production_3/Data");

		osgEarth::Registry::shaderGenerator().run(result, "tiled.ModelNode", Registry::stateSetCache());

		return result.release();
	}


protected:
	const GeniusWorldModelOptions      _options;
	osg::ref_ptr<const osgDB::Options> _dbOptions;
};

class GeniusWorldModelSourceFactory : public ModelSourceDriver
{
public:
	GeniusWorldModelSourceFactory()
	{
		supportsExtension("osgearth_model_geniusworld", "osgEarth GeniusWorld model plugin");
	}

	virtual const char* className() const
	{
		return "osgEarth GeniusWorld Model Plugin";
	}

	virtual ReadResult readObject(const std::string& file_name, const Options* options) const
	{
		if (!acceptsExtension(osgDB::getLowerCaseFileExtension(file_name)))
			return ReadResult::FILE_NOT_HANDLED;

		return ReadResult(new TiledModelSource(getModelSourceOptions(options)));
	}
};

REGISTER_OSGPLUGIN(osgearth_model_geniusworld, GeniusWorldModelSourceFactory)