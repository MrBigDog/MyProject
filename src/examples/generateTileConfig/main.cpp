#include <osgEarth/URI>
#include <osgEarth/Config>
#include <osgEarth/IOTypes>
#include <osgEarth/GeoData>
#include <osgEarth/StringUtils>

#include <osg/Node>
#include <osg/Array>
#include <osg/MatrixTransform>
#include <osg/ComputeBoundsVisitor>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>
#include <osgViewer/Viewer>

namespace
{
	class TiledDirectoryVisitor
	{
	public:
		TiledDirectoryVisitor() {}

		virtual void traverse(const std::string& path)
		{
			osgEarth::Config layerConfig("layer");

			osgEarth::ConfigSet cs;

			if (osgDB::fileType(path) == osgDB::DIRECTORY)
			{
				std::cout << "正在扫描数据......" << std::endl;
				osgDB::DirectoryContents files = osgDB::getDirectoryContents(path);
				int fnum = files.size() - 2;

				for (osgDB::DirectoryContents::const_iterator f = files.begin(); f != files.end(); ++f)
				{
					if (f->compare(".") == 0 || f->compare("..") == 0) continue;

					std::cout << "共" << fnum << "个文件，正在处理第" << f - files.begin() << "个文件......" << std::endl;
					std::string filepath = osgDB::concatPaths(path, *f);
					std::string filename = osgDB::concatPaths(filepath, *f + ".osgb");
					if (osgDB::fileExists(filename))
					{
						osgEarth::URI uri(filename);
						osg::ref_ptr<osg::Node> node = uri.readNode().getNode();
						if (node.valid())
						{
							osg::ComputeBoundsVisitor cbv;
							node->accept(cbv);

							osg::BoundingBox bb = cbv.getBoundingBox();

							std::string tileUrl = filename;

							std::string tileName = osgDB::getStrippedName(filename);
							osgEarth::Config tileConfig("tile");
							tileConfig.add("tile_name", tileName);
							tileConfig.add("tile_url", tileUrl);
							tileConfig.add("tile_min_corner", bb.corner(0));
							tileConfig.add("tile_max_corner", bb.corner(7));

							cs.push_back(tileConfig);
							//layerConfig.add("tiles", tileConfig);
							_bbox.expandBy(bb);
						}
					}
					//else
					//{
					//	traverse(filepath);
					//}
				}

				std::cout << "处理完成，正在写出结果......" << std::endl;
				layerConfig.add(/*"tiles",*/ cs);

				std::vector<std::string> pathElements;
				osgDB::getPathElements(path, pathElements);

				std::string layerName = pathElements.empty() ? "" : pathElements.back();
				layerConfig.add("layer_name", layerName);
				layerConfig.add("layer_min_corner", _bbox.corner(0));
				layerConfig.add("layer_max_corner", _bbox.corner(7));
				layerConfig.add("layer_min_range", 0.0f);
				layerConfig.add("layer_max_range", 1000.0f);

				std::string metaPath = path.substr(0, path.size() - layerName.size());
				std::fstream output(/*osgDB::concatPaths(metaPath, "layer.meta")*/"layer.meta", std::ios_base::out);
				if (output.is_open())
				{
					output << layerConfig.toJSON();
					output.flush();
					output.close();
				}
				std::cout << "成功结束！" << std::endl;
			}
		}
	private:
		virtual bool handleDir(const std::string& path) { return true; }

	private:
		osg::BoundingBox _bbox;
		osg::Vec3d _location;
	};
}

//#include "us/string_util.h"
#include "us/us_vector3.h"
#include "us/us_spherical_coords.h"
#include "us/us_geocentric_coords.h"
#include "us/us_cartesian_coords.h"

#include <osgEarth/Map>
using namespace uniscope_globe;

static vector_3d string_to_vec3d(const std::string& str, const vector_3d& default_value)
{
	std::stringstream buf(str);
	vector_3d out = default_value;
	buf >> out.x;
	if (!buf.eof())
	{
		buf >> out.y >> out.z;
	}
	else
	{
		out.y = out.x;
		out.z = out.x;
	}
	return out;
}


int main()
{
	TiledDirectoryVisitor tdv;
	tdv.traverse("Z:\\宿州成果osgb\\Data");
	//tdv.traverse("D:\\BYL\\ive\\ZZZ8CM\\Production_3\\Data");


	system("pause");
	//osgEarth::Config layerConfig;
	//layerConfig.fromJSON(osgEarth::URI("D:/BYL/ive/ZZZ8CM/Production_3/layer.meta").getString());

	//const osgEarth::ConfigSet& tilesConfigSet = layerConfig.children("tile");
	//osgEarth::ConfigSet::const_iterator it;
	//for (it = tilesConfigSet.begin(); it != tilesConfigSet.end(); ++it)
	//{
	//	const osgEarth::Config& conf = *it;
	//	std::string tile_name = conf.value<std::string>("tile_name", "");
	//	std::string tile_url = conf.value<std::string>("tile_url", "");
	//	std::string min_corner_str = conf.value<std::string>("tile_min_corner", "");
	//	std::string max_corner_str = conf.value<std::string>("tile_max_corner", "");

	//	vector_3d min_corner_temp = string_to_vec3d(min_corner_str, vector_3d());
	//	vector_3d min_corner = vector_3d(min_corner_temp.x, min_corner_temp.z, min_corner_temp.y);
	//	vector_3d max_corner_temp = string_to_vec3d(max_corner_str, vector_3d());
	//	vector_3d max_corner = vector_3d(max_corner_temp.x, max_corner_temp.z, max_corner_temp.y);

	//	vector_3d tile_center = (min_corner + max_corner)*0.5;

	//	osgEarth::Map* map = new osgEarth::Map;

	//	osgEarth::GeoPoint p(map->getSRS(), 113.06277, 22.64785, 0.0);
	//	osg::Matrixd tranmat;
	//	p.createLocalToWorld(tranmat);

	//	osg::Vec3d ppp = osg::Vec3d(min_corner.x, min_corner.y, min_corner.z)*tranmat;

	//	osgEarth::GeoPoint pp;
	//	pp.fromWorld(map->getSRS(), ppp);


	//	const vector_3d& location = vector_3d(osg::DegreesToRadians(113.06277), osg::DegreesToRadians(22.64785), 0.0); //m_parent_layer->_layer_location;

	//	vector3<double> center_sphr = location;
	//	center_sphr.z = spherical_coords::s_semi_major;
	//	vector3<double> center_geo = geocentric_coords::from_spherical_d(center_sphr);

	//	matrix_4d translat_mat = geocentric_coords::transform_from_cartesian_scale_d(center_sphr.x, center_sphr.y, center_sphr.z);

	//	vector_3d bb_min = min_corner*translat_mat;
	//	vector_3d bb_max = max_corner*translat_mat;

	//	vector_3d bb_min_spr = spherical_coords::from_geocentric_d(bb_min);
	//	vector_3d bb_max_spr = spherical_coords::from_geocentric_d(bb_max);

	//	double min_lon = osg::RadiansToDegrees(bb_min_spr.x);
	//	double min_lat = osg::RadiansToDegrees(bb_min_spr.y);

	//	double max_lon = osg::RadiansToDegrees(bb_max_spr.x);
	//	double max_lat = osg::RadiansToDegrees(bb_max_spr.y);

	//	//m_parent_layer->getloc
	//}


	return 0;
}
