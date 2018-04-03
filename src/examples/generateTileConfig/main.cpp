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

#include <windows.h>

namespace
{
	class TiledDirectoryVisitor
	{
	public:
		TiledDirectoryVisitor() {}

		virtual void traverse(const std::string& path, const osg::Vec3d& location)
		{
			std::vector<std::string> pathElements;
			osgDB::getPathElements(path, pathElements);

			std::string layerName = pathElements.size() < 2 ? "" : pathElements[pathElements.size() - 2];
			std::string dataFolder = pathElements.empty() ? "" : pathElements.back();

			std::string layerPath = path.substr(0, path.size() - dataFolder.size());

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

					std::cout << "共" << fnum << "个文件，正在处理第" << f - files.begin() - 1 << "个文件......" << std::endl;
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

							std::string tileUrl = filename.substr(layerPath.size(), filename.size());

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
				}

				std::cout << "处理完成，正在写出结果......" << std::endl;
				layerConfig.add(cs);
				layerConfig.add("layer_name", layerName);
				layerConfig.add("layer_min_corner", _bbox.corner(0));
				layerConfig.add("layer_max_corner", _bbox.corner(7));
				layerConfig.add("layer_min_range", 0.0f);
				layerConfig.add("layer_max_range", 2000.0f);
				layerConfig.add("layer_location", osg::Vec3d());

				std::string metaPath = path.substr(0, path.size() - dataFolder.size());
				std::fstream output(osgDB::concatPaths(metaPath, "layer.meta")/*"layer.meta"*/, std::ios_base::out);
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
//#include "us/us_vector3.h"
//#include "us/us_spherical_coords.h"
//#include "us/us_geocentric_coords.h"
//#include "us/us_cartesian_coords.h"
//
////#include <osgEarth/Map>
//using namespace uniscope_globe;
//
//static vector_3d string_to_vec3d(const std::string& str, const vector_3d& default_value)
//{
//	std::stringstream buf(str);
//	vector_3d out = default_value;
//	buf >> out.x;
//	if (!buf.eof())
//	{
//		buf >> out.y >> out.z;
//	}
//	else
//	{
//		out.y = out.x;
//		out.z = out.x;
//	}
//	return out;
//}
#include "us/us_utm_projection.h"
#include "us/us_axis_aligned_box.h"
#include "us/us_vector3.h"
#include "us/string_util.h"
#include "us/us_lon_lat_rect.h"
#include "us/us_spherical_coords.h"
#include "us/us_geocentric_coords.h"
//#include <us/us_matrix4.h>

using namespace uniscope_globe;

int main(int argc, char ** argv)
{
	osg::ref_ptr<osgEarth::SpatialReference> wgs84ref = osgEarth::SpatialReference::create("EPSG:4326");
	osg::ref_ptr<osgEarth::SpatialReference> utmref = osgEarth::SpatialReference::create("+proj=tmerc +lat_0=0 +lon_0=117 +k=1 +x_0=39500000 +y_0=0 +ellps=krass +units=m +no_defs"/*"EPSG:32650"*/);

	wgs84ref->isCube();
	wgs84ref->isCube();


	double lat1, lon1;
	utmref->transform2D(497223, 3724018, wgs84ref, lon1, lat1);

	std::cout << wgs84ref->isProjected() << ", " << wgs84ref->isProjected() << std::endl;

	system("pause");
	return 0;

	osgEarth::GeoPoint gp(utmref, 497223, 3724018, 0);
	osgEarth::GeoPoint wgsgp = gp.transform(wgs84ref);

	osg::Vec3d world;
	gp.toWorld(world);

	std::cout << world.x() << ", " << world.y() << ", " << world.z() << std::endl;
	system("pause");

	return 0;

	double lat = 0.0, lon = 0.0;
	uniscope_globe::utm_projection utmproj;
	utmproj.m_zone_index = 50;
	utmproj.m_offset_east = 0;
	utmproj.projection_inverse_caculate(497223, 3724018, &lon, &lat);

	//lon += 360;
	//osg::ArgumentParser arguments(&argc, argv);

	//std::string parasStr;// = argv[3];
	//arguments.read("-p", parasStr);
	//osgEarth::StringVector paras;
	//osgEarth::StringTokenizer stk(parasStr, paras, ",");

	//if (paras.size() < 4)
	//{
	//	MessageBox(NULL, "参数的数量不对, 请检查输入的参数", "错误", 1);
	//	return 0;
	//}

	//std::string inPath = paras[0];
	//std::string lon = paras[1];
	//std::string lat = paras[2];
	//std::string heightOffset = paras[3];

	//double dlon = osgEarth::as(lon, 0.0);
	//double dlat = osgEarth::as(lat, 0.0);
	//double dho = osgEarth::as(heightOffset, 0.0);
	//osg::Vec3d location(dlon, dlat, dho);

	//TiledDirectoryVisitor tdv;
	//tdv.traverse(inPath, location);

	//TiledDirectoryVisitor tdv;
	////tdv.traverse("Z:\\宿州成果osgb\\Data", location);
	//tdv.traverse("D:\\BYL\\ive\\ZZZ8CM\\Production_3\\Data", location);


	//system("pause");
	osgEarth::Config layerConfig;
	layerConfig.fromJSON(osgEarth::URI("G:\\SuZhou\\SuZhou-osgb\\layer.meta").getString());

	std::string location_str = layerConfig.value<std::string>("layer_location", "");
	vector_3d location = string_to_vec3d(location_str, vector_3d());

	std::string layer_min_corner_str = layerConfig.value<std::string>("layer_min_corner", "");
	std::string layer_max_corner_str = layerConfig.value<std::string>("layer_max_corner", "");

	double maxHeight = layerConfig.value<double>("layer_max_visible_height", 10000.0);
	double visibleDis = layerConfig.value<double>("layer_max_range", 1500.0);

	//m_parent_layer->_max_height = maxHeight;
	//m_parent_layer->set_visible_range(visibleDis);

	vector_3d layer_min_corner/*_temp*/ = string_to_vec3d(layer_min_corner_str, vector_3d());
	layer_min_corner.z = spherical_coords::s_semi_major + location.z;
	//vector_3d layer_min_corner = vector_3d(layer_min_corner_temp.x, layer_min_corner_temp.z, layer_min_corner_temp.y);
	vector_3d layer_max_corner/*_temp*/ = string_to_vec3d(layer_max_corner_str, vector_3d());
	layer_max_corner.z = spherical_coords::s_semi_major + location.z;
	//vector_3d layer_max_corner = vector_3d(layer_max_corner_temp.x, layer_max_corner_temp.z, layer_max_corner_temp.y);

	//vector_3d location_sph = vector_3d(location.x*math_d::s_deg_to_rad, location.y*math_d::s_deg_to_rad, location.z); //m_parent_layer->_layer_location;
	vector_3d center_sphr = location;
	center_sphr.z += spherical_coords::s_semi_major;
	//m_parent_layer->_translat_mat = geocentric_coords::transform_from_cartesian_scale_d(center_sphr.x, center_sphr.y, center_sphr.z);

	matrix_4d layerMatrix = geocentric_coords::transform_from_cartesian_scale_d(center_sphr.x, center_sphr.y, center_sphr.z);
	matrix_4d layerMatrixInverse = layerMatrix.inverse();

	//axis_aligned_box<double> layer_aab(layer_min_corner, layer_max_corner);
	lon_lat_rect rec0(layer_max_corner.y, layer_min_corner.y, layer_max_corner.x, layer_min_corner.x); /*= lon_lat_rect::from_aabb(center_sphr, layer_aab);*/
	//m_parent_layer->set_layer_rect(rec0);

	const osgEarth::ConfigSet& tilesConfigSet = layerConfig.children("tile");
	osgEarth::ConfigSet::const_iterator it;
	for (it = tilesConfigSet.begin(); it != tilesConfigSet.end(); ++it)
	{
		const osgEarth::Config& conf = *it;
		std::string tile_name = conf.value<std::string>("tile_name", "");

		std::string tile_url = conf.value<std::string>("tile_url", "");
		//tile_url = osgDB::concatPaths(rootPath, tile_url);

		std::string min_corner_str = conf.value<std::string>("tile_min_corner", "");
		std::string max_corner_str = conf.value<std::string>("tile_max_corner", "");

		//osgb的数据是z轴朝上的，这里调换一下;
		vector_3d min_corner/*_temp*/ = string_to_vec3d(min_corner_str, vector_3d());
		min_corner.z = spherical_coords::s_semi_major + location.z;
		//vector_3d min_corner = vector_3d(min_corner_temp.x, min_corner_temp.z, min_corner_temp.y);
		vector_3d max_corner/*_temp*/ = string_to_vec3d(max_corner_str, vector_3d());
		max_corner.z = spherical_coords::s_semi_major + location.z;
		//vector_3d max_corner = vector_3d(max_corner_temp.x, max_corner_temp.z, max_corner_temp.y);

		vector_3d rec_center = (min_corner + max_corner)*0.5;
		rec_center.z = spherical_coords::s_semi_major + location.z;

		matrix_4d square_matrix = geocentric_coords::transform_from_cartesian_scale_d(rec_center.x, rec_center.y, rec_center.z);
		matrix_4d relative_matrix = square_matrix*layerMatrixInverse;

		matrix_4d matrix = square_matrix*relative_matrix.inverse();
		matrix_4d matrix_inv = matrix.inverse();

		vector_3d min_corner_geo = geocentric_coords::from_spherical_d(min_corner);
		vector_3d min_local = min_corner_geo*matrix_inv;

		//matrix_4d::translation()


			//osg::BoundingBox bb(osg::Vec3(min_corner.x, min_corner.z, min_corner.y), osg::Vec3(max_corner.x, max_corner.z, max_corner.y));
			//double tile_size = bb.radius();


			//vector_3d bb_min = min_corner*m_parent_layer->_translat_mat;
			//vector_3d bb_max = max_corner*m_parent_layer->_translat_mat;

			//vector_3d bb_min_spr = spherical_coords::from_geocentric_d(bb_min);
			//vector_3d bb_max_spr = spherical_coords::from_geocentric_d(bb_max);

			//axis_aligned_box<double> square_aab(min_corner, max_corner);
			//lon_lat_rect square_rec = lon_lat_rect::from_aabb(center_sphr, square_aab);

			//lon_lat_box square_box/* = lon_lat_box(square_rec)*/;

			//building_square_description bsd;
			//bsd.rect = lon_lat_rect(max_corner.y, min_corner.y, max_corner.x, min_corner.x);
			////	bsd.geo_aabb = square_box.get_aabb();

			////double v_scale = math_d::cos_(bsd.rect.m_south);
			////vector_3d vec_min(-tile_size * 0.0006 * v_scale, 0.0, -tile_size * 0.0006);
			////vector_3d vec_max(tile_size * 0.0006 * v_scale, 0.05, tile_size * 0.0006);
			////bsd.cart_aabb.set_extent(vec_min, vec_max);
			////bsd.cart_aabb_meter.set_extent(vec_min * 1000.0, vec_max * 1000.0);

			//tiled_model_square* v_square = new tiled_model_square(m_parent_layer, tile_url, bsd);
			//v_square->add_ref();
			//m_square_map[tile_url] = v_square;
	}


	return 0;
}
