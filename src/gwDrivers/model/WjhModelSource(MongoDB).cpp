#pragma warning(disable: 4996)

#include <bsoncxx/json.hpp>
#include <bsoncxx/stdx/make_unique.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/basic/document.hpp>

#include <mongocxx/client.hpp>
#include <mongocxx/gridfs/bucket.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/pool.hpp>

#include <osgEarth/Map>
#include <osgEarth/Registry>
#include <osgEarth/ModelSource>
#include <osgEarth/GeoTransform>
#include <osgEarthFeatures/GeometryCompiler>

#include <osg/Notify>
#include <osg/MatrixTransform>
#include <osg/ComputeBoundsVisitor>
#include <osgDB/FileNameUtils>
#include <osgViewer/Viewer>
#include "WjhModelOptions.h"

using namespace osgEarth;
using namespace osgEarth::Features;
using namespace osgEarth::Symbology;
using namespace gwDrivers;

#define LC "[WJHModelSource] "

namespace
{
	static bool stringToMatix3(const std::string& matstr, osg::Matrix3d& mat)
	{
		std::vector<std::string> stringVector;
		osgEarth::StringTokenizer st(matstr, stringVector, ",");
		if (stringVector.size() != 9) return false;
		osg::Matrix3d matrix;
		matrix(0, 0) = osgEarth::as(stringVector[0], 0.0);
		matrix(0, 1) = osgEarth::as(stringVector[1], 0.0);
		matrix(0, 2) = osgEarth::as(stringVector[2], 0.0);
		matrix(1, 0) = osgEarth::as(stringVector[3], 0.0);
		matrix(1, 1) = osgEarth::as(stringVector[4], 0.0);
		matrix(1, 2) = osgEarth::as(stringVector[5], 0.0);
		matrix(2, 0) = osgEarth::as(stringVector[6], 0.0);
		matrix(2, 1) = osgEarth::as(stringVector[7], 0.0);
		matrix(2, 2) = osgEarth::as(stringVector[8], 0.0);
		mat = matrix;
		return true;
	}

	static osg::Matrixd asMatrix4(const osg::Matrix3d& mat)
	{
		osg::Matrixd mat4;
		mat4(0, 0) = mat(0, 0);
		mat4(0, 1) = mat(0, 1);
		mat4(0, 2) = mat(0, 2);
		mat4(0, 3) = 0.0;
		mat4(1, 0) = mat(1, 0);
		mat4(1, 1) = mat(1, 1);
		mat4(1, 2) = mat(1, 2);
		mat4(1, 3) = 0.0;
		mat4(2, 0) = mat(2, 0);
		mat4(2, 1) = mat(2, 1);
		mat4(2, 2) = mat(2, 2);
		mat4(2, 3) = 0.0;
		mat4(3, 0) = 0.0;
		mat4(3, 1) = 0.0;
		mat4(3, 2) = 0.0;
		mat4(3, 3) = 1.0;
		return mat4;
	}

	//管理MongoDB连接池的单例类
	class MongoPool
	{
	public:
		static MongoPool& instance()
		{
			static MongoPool instance;
			return instance;
		}

		using connection = mongocxx::pool::entry;
		void configure(std::unique_ptr<mongocxx::pool> pool)
		{
			_pool = std::move(pool);
		}
		connection get_connection() { return _pool->acquire(); }

		bsoncxx::stdx::optional<connection> try_get_connection()
		{
			return _pool->try_acquire();
		}
	private:
		MongoPool() = default;
		std::unique_ptr<mongocxx::pool> _pool = nullptr;
	};

	using bsoncxx::stdx::make_unique;
	using bsoncxx::builder::basic::kvp;
	using bsoncxx::builder::basic::make_document;

	bool getBufferFromMongo(std::string nodename, std::stringstream& nodebuffer)
	{
		auto connection = MongoPool::instance().get_connection();
		auto db = (*connection)["wjh_model"];
		auto bucket = db.gridfs_bucket();
		auto coll = db.collection("fs.files");
		auto doc_value = bsoncxx::builder::basic::make_document(kvp("filename", nodename));
		//auto docview = coll.find_one(bsoncxx::document::view_or_value(doc_value)).value();
		auto cursor = coll.find(bsoncxx::document::view_or_value(doc_value));
		auto id_ele = /*docview*/(*cursor.begin())["_id"];
		if (!id_ele.raw()) return false;
		auto id = id_ele.get_value();
		auto downloader = bucket.open_download_stream(id);
		auto file_length = downloader.file_length();
		auto buffer_size = std::min(file_length, static_cast<std::int64_t>(downloader.chunk_size()));
		auto buffer = make_unique<std::uint8_t[]>(static_cast<std::size_t>(buffer_size));
		while (auto length_read = downloader.read(buffer.get(), static_cast<std::size_t>(buffer_size)))
		{
			nodebuffer << std::string(reinterpret_cast<char*>(buffer.get()), length_read);
		}
		return true;
	}

	//纹理共享;（要不要模型也共享呢？？）
	class ReadAndShareImageCallback : public osgDB::ReadFileCallback
	{
	public:
		ReadAndShareImageCallback(osgEarth::MemCache* memcache) :_memCache(memcache)
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
			std::string fileext = osgDB::getFileExtension(filename);
			osg::ref_ptr<osgDB::ReaderWriter> rw = osgDB::Registry::instance()->getReaderWriterForExtension(fileext);
			if (!rw) return image;

			std::string nameinmongo = osgDB::getNameLessExtension(filename);
			std::stringstream imagebuffer;
			if (!getBufferFromMongo(nameinmongo, imagebuffer))
			{
				return image;
			}

			osgDB::ReaderWriter::ReadResult rr = rw->readImage(imagebuffer);
			//rr = osgDB::Registry::instance()->readImageImplementation(filename, options);
			if (rr.success())
			{
				image = rr.getImage();
				if (_memCache)
				{
					_memCache->getOrCreateDefaultBin()->write(filename, image.get(), 0L);
				}
			}
			return image;
		}

	private:
		osgEarth::MemCache* _memCache;
	};

	class WJHNodeFactory :public FeatureNodeFactory
	{
	public:
		WJHNodeFactory(const WJHModelOptions& options) : _options(options)
		{
			_memCache = getMemcacheFromConfig();
			_readoptions = new osgDB::Options;
			_readoptions->setReadFileCallback(new ReadAndShareImageCallback(_memCache));
			//_readoptions->setBuildKdTreesHint(osgDB::Options::BUILD_KDTREES);
			_wsg84Srs = osgEarth::SpatialReference::create("wgs84");

			_rw = osgDB::Registry::instance()->getReaderWriterForExtension("osgb");

			//创建mongodb连接池;
			mongocxx::uri uri("mongodb://localhost:27017/?minPoolSize=10&maxPoolSize=100");
			auto pool = std::unique_ptr<mongocxx::pool>(new mongocxx::pool(std::move(uri)));
			MongoPool::instance().configure(std::move(pool));
		}
		~WJHNodeFactory() {}

	public:
		virtual bool createOrUpdateNode(FeatureCursor* cursor, const Style& style,
			const FilterContext& context, osg::ref_ptr<osg::Node>&  node)
		{
			if (!_rw.valid()) return false;

			FeatureList features;
			cursor->fill(features);

			osg::ref_ptr<osg::Group> result = new osg::Group;
			for (FeatureList::const_iterator f = features.begin(); f != features.end(); ++f)
			{
				Feature* input = f->get();

				osgEarth::Geometry* geom = input->getGeometry();
				if (geom->empty()) continue;

				std::string nodename = input->getString("node_name");
				//std::string nodeurl = osgDB::concatPaths(_options.modelUrl().value().full(), nodename + ".osgb");

				std::stringstream nodebuffer;
				if (!getBufferFromMongo(nodename, nodebuffer))
				{
					continue;
				}
				osgDB::ReaderWriter::ReadResult readresult = _rw->readNode(nodebuffer, _readoptions);

				osg::ref_ptr<osg::Node> newnode = readresult.takeNode()/*osgDB::readNodeFile(nodeurl, _readoptions)*/;
				if (!newnode) continue;

				//添加shaderGeneration
				osgEarth::Registry::shaderGenerator().run(newnode, "wjh.ModelNode", Registry::stateSetCache());

				osg::ComputeBoundsVisitor cbv;
				newnode->accept(cbv);

				double zfilter = (cbv.getBoundingBox().zMax() - cbv.getBoundingBox().zMin())*0.5;

				//build KDTree for faster intersection.
				if (osgDB::Registry::instance()->getBuildKdTreesHint() == osgDB::ReaderWriter::Options::BUILD_KDTREES &&
					osgDB::Registry::instance()->getKdTreeBuilder())
				{
					osg::ref_ptr<osg::KdTreeBuilder> builder = osgDB::Registry::instance()->getKdTreeBuilder()->clone();
					newnode->accept(*builder);
				}

				osg::Vec3d pos = *geom->begin(); pos.z() += zfilter;
				osgEarth::GeoPoint geoPos = osgEarth::GeoPoint(_wsg84Srs, pos);
				osg::Matrixd local2world;
				geoPos.createLocalToWorld(local2world);

				std::string matstr = input->getString("matrix3");
				osg::Matrix3d rotscalmat3;
				stringToMatix3(matstr, rotscalmat3);
				osg::Matrixd rotscalmat4 = asMatrix4(rotscalmat3);

				osg::ref_ptr<osg::MatrixTransform> mat = new osg::MatrixTransform;
				mat->setMatrix(rotscalmat4*local2world);
				mat->addChild(newnode);

				result->addChild(mat);
			}
			node = result;
			return node.valid();
		}

	private:
		osgEarth::MemCache* getMemcacheFromConfig()
		{
			bool ismemcache = _options.isMemCache().isSet() ? _options.isMemCache().value() : false;
			if (!ismemcache) return 0L;

			unsigned int memcachesize = _options.memCacheSize().isSet() ? _options.memCacheSize().value() : 1000;
			return new osgEarth::MemCache(memcachesize);
		}

	private:
		osg::ref_ptr<osgDB::Options> _readoptions;
		osg::ref_ptr<osgEarth::SpatialReference> _wsg84Srs;
		const WJHModelOptions _options;

		osg::ref_ptr<osgEarth::MemCache> _memCache;
		osg::ref_ptr<osgDB::ReaderWriter> _rw;
	};

	//避免跨瓦片模型被过早裁剪;
	class PagedLodOption : public osgEarth::NodeOperation
	{
	public:
		PagedLodOption() {}
		virtual void operator()(osg::Node* node)
		{
			osg::PagedLOD* pl = osgEarth::findFirstParentOfType<osg::PagedLOD>(node);
			if (pl)
			{
				osg::BoundingSphere bs = pl->computeBound();
				bs.expandBy(node->getBound());

				pl->setRadius(bs.radius()*2.0);
				pl->setCenter(bs.center());
			}
		}
	};

	/** The model source implementation for WJH model */
	class WJHModelSource : public FeatureModelSource
	{
	public:
		WJHModelSource(const ModelSourceOptions& options)
			: FeatureModelSource(options)
			, _options(options)
		{
			addPostMergeOperation(new PagedLodOption);
		}

		const WJHModelSource& getOptions() const { return _options; }

	public: // FeatureModelSource
		FeatureNodeFactory* createFeatureNodeFactory()
		{
			return new WJHNodeFactory(_options);
		}

	private:
		const WJHModelOptions _options;
	};
}

//------------------------------------------------------------------------
class WJHModelSourceDriver : public ModelSourceDriver
{
public:
	WJHModelSourceDriver()
	{
		supportsExtension("osgearth_model_wjh", "gwEarth WJH model plugin");
	}

	virtual const char* className() const
	{
		return "gwEarth WJH Model Plugin";
	}

	virtual ReadResult readObject(const std::string& file_name, const Options* options) const
	{
		if (!acceptsExtension(osgDB::getLowerCaseFileExtension(file_name)))
		{
			return ReadResult::FILE_NOT_HANDLED;
		}

		return new WJHModelSource(getModelSourceOptions(options));
	}
};

REGISTER_OSGPLUGIN(osgearth_model_wjh, WJHModelSourceDriver)
