/************************************************************************/
//Genius World 模型
/************************************************************************/
#include "GeniusWorldModelOptions.h"
#include <gwBase/StringUtils.h>
#include <gwBase/Math.h>

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

#include "us/us_square_description.h"
#include "us/us_spherical_coords.h"

#define SQUARE_PL_EXTENSION "gw_square"

//GeniusWorld related;
namespace
{
	using namespace uniscope_globe;

	double s_square_size_radian = math_d::s_pi / math_d::pow_(2, 15);//在第15级加载模型数据;
	double s_square_size_radian_inv = 1.0 / s_square_size_radian;
	double s_square_size_degree = s_square_size_radian * math_d::s_rad_to_deg;
	double s_square_size_degree_inv = 1.0 / s_square_size_degree;
	double s_square_size = spherical_coords::s_semi_major * s_square_size_radian * 1000.0;;
	double s_square_size_inv = 1.0 / s_square_size;
	bool s_new_building = false;

	double s_range = s_square_size*6.0;

	static lon_lat_rect calculate_square_rect_by_index(const interior_index& index)
	{
		interior_index v_col = 0x0000000000ffffff & (index);
		interior_index v_row = 0x0000000000ffffff & (index >> 24);

		return lon_lat_rect(((v_row + 1) * s_square_size_degree - 90)  * math_d::s_deg_to_rad,
			((v_row)* s_square_size_degree - 90)  * math_d::s_deg_to_rad,
			((v_col + 1) * s_square_size_degree - 180) * math_d::s_deg_to_rad,
			((v_col)* s_square_size_degree - 180) * math_d::s_deg_to_rad);
	}

#	define MAX_PATH          260
#	define MASK_24_BITS (0xffffff)
#	define MASK_5_BITS  (0x1f)
#	define MASK_3_BITS  (0x07)

#	define	INTERIOR_INDEX_LAT(id)		((int)((id) & MASK_24_BITS))
#	define	INTERIOR_INDEX_LON(id)		((int)(((id)>>24) & MASK_24_BITS))
#	define	INTERIOR_INDEX_LEVEL(id)	((int)(((id)>>48) & MASK_5_BITS))

	static std::string parse_building_absolute_path_by_index(const interior_index& index)
	{
		wchar_t str_path[MAX_PATH];
		memset(str_path, 0, MAX_PATH * 2);
		if ((index & 0x8000000000000000) == 0)
		{
			// new one
			long lon_index = INTERIOR_INDEX_LON(index);
			long lat_index = INTERIOR_INDEX_LAT(index);
			long lon1 = (lon_index & 0xff0000) >> 16;
			long lon2 = (lon_index & 0x00ff00) >> 8;
			long lon3 = (lon_index & 0x0000ff);
			long lat1 = (lat_index & 0xff0000) >> 16;
			long lat2 = (lat_index & 0x00ff00) >> 8;
			long lat3 = (lat_index & 0x0000ff);
			swprintf(str_path, 255, L"%x%x/%x%x/%x%x/", lon1, lat1, lon2, lat2, lon3, lat3);
		}
		else
		{
			interior_index part = (index & 0x7fffffffffffffff) >> 10;
			if (s_new_building)
			{
				swprintf(str_path, 255, L"/%I64x/", part);
			}
			else
			{
				swprintf(str_path, 255, L"/common/%I64x/", part);
			}
		}
		return gwBase::strutils::WStringToString(str_path);
	}

	static double getRectRadius(const lon_lat_rect& rect)
	{
		return math_d::sqrt_(rect.get_width()*rect.get_width() + rect.get_height()*rect.get_height());
	}

	static int hex_table[] =
	{
		0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,1,2,3,4,5,6, 7,8,9,0,0,0,0,0,0, 0,10,11,12,13,14,15,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,10, 11,12,13,14,15
	};

	static interior_index hexToDecimal(const char* hex_str)
	{
		//return _wtoi64(gwBase::strutils::StringToWString(hex_str).c_str());
		char ch;
		interior_index iret = 0;
		while (ch = *hex_str++)
		{
			iret = (iret << 4) | hex_table[ch];
		}
		return iret;
	}
}

//
namespace
{
	std::string vec3dToString(const osg::Vec3f& v)
	{
		std::stringstream buf;
		buf << std::setprecision(15)
			<< v.x() << " " << v.y() << " " << v.z()
			<< std::endl;
		std::string result;
		result = buf.str();
		return result;
	}

	osg::Vec3d stringToVec3d(const std::string& str, const osg::Vec3d& default_value)
	{
		std::stringstream buf(str);
		osg::Vec3d out = default_value;
		buf >> out.x();
		if (!buf.eof())
		{
			buf >> out.y() >> out.z();
		}
		else
		{
			out.y() = out.x();
			out.z() = out.x();
		}
		return out;
	}

	struct SquarePseudoLoader : public osgDB::ReaderWriter
	{
		SquarePseudoLoader()
		{
			this->supportsExtension(SQUARE_PL_EXTENSION, "gw square pseudoloader");
		}
		const char* className() const { return "gw square gen pseudoloader"; }
		bool acceptsExtension(const std::string& extension) const
		{
			return osgDB::equalCaseInsensitive(extension, SQUARE_PL_EXTENSION);
		}
		ReadResult readObject(const std::string& filename, const osgDB::Options* options) const
		{
			return readNode(filename, options);
		}
		ReadResult readNode(const std::string& filename, const osgDB::Options* options) const
		{
			if (!acceptsExtension(osgDB::getFileExtension(filename)))
				return ReadResult::FILE_NOT_HANDLED;

			std::string path = osgDB::getFilePath(filename);

			osg::ref_ptr<osg::Group> root = 0L;
			osgDB::DirectoryContents files = osgDB::getDirectoryContents(path);
			for (osgDB::DirectoryContents::const_iterator f = files.begin(); f != files.end(); ++f)
			{
				if (f->compare(".") == 0 || f->compare("..") == 0)
					continue;

				std::string filepath = osgDB::concatPaths(path, *f);
				std::string fileExt = osgDB::getLowerCaseFileExtension(filepath);
				if (fileExt != "m") continue;

				osgEarth::URI fileuri(filepath);
				osg::ref_ptr<osg::Node> node = fileuri.readNode().getNode();// osgDB::readNodeFile(filepath);
				if (!node.valid()) continue;

				if (!root)root = new osg::Group;
				root->addChild(node);
			}
			return ReadResult(root);
		}
	};
	REGISTER_OSGPLUGIN(SQUARE_PL_EXTENSION, SquarePseudoLoader)


		osg::PagedLOD* createPagedLod(const osg::Vec3d& pos, double radius, const std::string& filepath)
	{
		osg::ref_ptr<osg::PagedLOD> pg = new osg::PagedLOD;
		pg->setFileName(0, filepath);
		pg->setCenter(pos);
		pg->setRadius(radius);
		pg->setRange(0, 0, s_range);

		return pg.release();
	}

	class GeniusWorldModelDirectoryVisitor
	{
	public:
		GeniusWorldModelDirectoryVisitor(osg::Group* root, const osgEarth::Map* map)
			:_root(root), _map(map)
		{}

		void traverse(const std::string& path)
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
						traverse(filepath);
					}
				}
			}
			else if (osgDB::fileType(path) == osgDB::REGULAR_FILE)
			{
				handleFile(path);
			}
		}
	private:
		bool handleDir(const std::string& path) { return true; }
		void handleFile(const std::string& filename)
		{
			//找到usmt格式的文件名称;
			std::string fileexten = osgDB::convertToLowerCase(osgDB::getFileExtension(filename));
			if (fileexten != "usmt") return;

			//得到瓦片编码;
			std::string tileIndexStr = osgDB::getNameLessExtension(osgDB::getSimpleFileName(filename));
			interior_index tileIndex = hexToDecimal(tileIndexStr.c_str());

			//得到瓦片外包盒;
			uniscope_globe::lon_lat_rect tileRect = calculate_square_rect_by_index(tileIndex);

			//瓦片中心;
			uniscope_globe::vector_3d tileCenter = tileRect.get_center();
			osg::Vec3d cc(osg::RadiansToDegrees(tileCenter.x), osg::RadiansToDegrees(tileCenter.y), tileCenter.z);

			//瓦片尺寸（半径）;
			double tileRadius = s_square_size/*getRectRadius(tileRect)*/;

			//瓦片的地址;
			std::string path = osgDB::concatPaths("//172.30.17.135/GeniusWorld/data/model/chanyeyuan/yiqi/sm",
				parse_building_absolute_path_by_index(tileIndex) + "0." + SQUARE_PL_EXTENSION);

			//瓦片的geo位置;
			osgEarth::GeoPoint geoCenter(
				_map->getProfile()->getSRS(),
				osg::RadiansToDegrees(tileCenter.x),
				osg::RadiansToDegrees(tileCenter.y),
				tileCenter.z,
				ALTMODE_ABSOLUTE);

			//瓦片的RST矩阵;
			osg::Matrixd ltw;
			geoCenter.createLocalToWorld(ltw);

			osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
			mt->setMatrix(ltw);
			mt->addChild(
				createPagedLod(
					osg::Vec3d(),/*此参数应该为模型包围盒中心，但是初始时候并不知道，可以在加载后重新计算;*/
					tileRadius,
					path));

			_root->addChild(mt);
		}

	private:
		osg::Group* _root;
		const osgEarth::Map* _map;
	};
}

class GeniusWorldModelSource : public ModelSource
{
public:
	GeniusWorldModelSource(const ModelSourceOptions& options)
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
		osg::ref_ptr<osg::Group> root = new osg::Group;

		GeniusWorldModelDirectoryVisitor gwmdv(root, map);
		gwmdv.traverse("//172.30.17.135/GeniusWorld/data/model/chanyeyuan/yiqi/sm");

		return root.release();
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
		supportsExtension("osgearth_model_geniusworld", "gwEarth GeniusWorld model plugin");
	}

	virtual const char* className() const
	{
		return "gwEarth  GeniusWorld Model Plugin";
	}

	virtual ReadResult readObject(const std::string& file_name, const Options* options) const
	{
		if (!acceptsExtension(osgDB::getLowerCaseFileExtension(file_name)))
			return ReadResult::FILE_NOT_HANDLED;

		return ReadResult(new GeniusWorldModelSource(getModelSourceOptions(options)));
	}
};

REGISTER_OSGPLUGIN(osgearth_model_geniusworld, GeniusWorldModelSourceFactory)