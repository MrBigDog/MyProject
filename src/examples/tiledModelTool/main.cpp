#include <osgEarth/URI>
#include <osgEarth/Config>
#include <osgEarth/IOTypes>
#include <osgEarth/GeoData>
#include <osgEarth/StringUtils>

#include <osg/Node>
#include <osg/Array>
#include <osg/Timer>
#include <osg/Vec4>
#include <osg/MatrixTransform>
#include <osg/ComputeBoundsVisitor>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>
#include <osgViewer/Viewer>

#include <algorithm>
#include <windows.h>

#include "prj.h"

namespace
{
	static osg::Vec3d stringToVec3d(const std::string& str, const osg::Vec3d& default_value)
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

	std::string	vec3dToString(const osg::Vec3d& v)
	{
		std::stringstream buf;
		buf << std::setprecision(15)
			<< v.x() << " " << v.y() << " " << v.z() /*<< " " << v.w()*/
			<< std::endl;
		std::string result;
		result = buf.str();
		return result;
	}

	class TiledDirectoryVisitor
	{
	public:
		TiledDirectoryVisitor()
		{}

		virtual void traverse(const std::string& path, const std::string& outpath, const osg::Vec3d& location, int vzone)
		{
			std::vector<std::string> pathElements;
			osgDB::getPathElements(path, pathElements);

			std::string layerName = pathElements.size() < 2 ? "" : pathElements[pathElements.size() - 2];
			std::string dataFolder = pathElements.empty() ? "" : pathElements.back();

			std::string layerPath = path.substr(0, path.size() - dataFolder.size());

			osgEarth::Config layerConfig("layer");

			osgEarth::ConfigSet cs;

			osg::Timer timer;
			osg::Timer_t startTimer = timer.tick();

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
							tileConfig.add<osg::Vec3d>("tile_min_corner", /*minLonlat*/bb.corner(0));
							tileConfig.add<osg::Vec3d>("tile_max_corner", /*maxLonlat*/bb.corner(7));

							cs.push_back(tileConfig);
							//layerConfig.add("tiles", tileConfig);
							_bbox.expandBy(bb);
						}
					}
				}

				std::string locationstr = vec3dToString(location);

				std::cout << "处理完成，正在写出结果......" << std::endl;
				layerConfig.add(/*"tiles",*/ cs);
				layerConfig.add("layer_name", layerName);
				layerConfig.add<osg::Vec3d>("layer_min_corner", /*minLonlat*/_bbox.corner(0));
				layerConfig.add<osg::Vec3d>("layer_max_corner", /*maxLonlat*/_bbox.corner(7));
				layerConfig.add("layer_min_range", 0.0f);
				layerConfig.add("layer_max_range", 3500.0f);
				layerConfig.add("layer_max_visible_height", 10000.0f);
				layerConfig.add("layer_location", /*lonlatLocation*/locationstr);
				layerConfig.add("zone", vzone);

				std::string metaPath = outpath.empty() ? path.substr(0, path.size() - dataFolder.size()) : outpath;
				std::fstream output(osgDB::concatPaths(metaPath, "layer.meta"), std::ios_base::out);
				if (output.is_open())
				{
					output << layerConfig.toJSON();
					output.flush();
					output.close();
				}

				osg::Timer_t endTime = timer.tick();
				double ft = timer.delta_s(startTimer, endTime);

				int ntime = ft / 60.0;
				double mtime = fmod(ft, 60.0);

				std::cout << std::endl << std::endl << std::endl;
				std::cout << "成功结束！用时" << ntime << "分" << mtime << "秒" << std::endl;
				std::cout << std::endl << std::endl << std::endl;
			}
		}
	private:
		virtual bool handleDir(const std::string& path) { return true; }

	private:
		osg::BoundingBox _bbox;
	};
}



int main(int argc, char ** argv)
{
	//osg::ArgumentParser arguments(&argc, argv);

	//std::string parasStr;
	//arguments.read("-p", parasStr);
	//osgEarth::StringVector paras;
	//osgEarth::StringTokenizer stk(parasStr, paras, ",");

	//if (paras.size() < 5)
	//{
	//	MessageBox(NULL, "参数的数量不对, 请检查输入的参数", "错误", 1);
	//	return 0;
	//}

	//std::string inPath = paras[0];
	//std::string outPath = paras[1];
	//std::string lon = paras[2];
	//std::string lat = paras[3];
	//std::string heightOffset = paras[4];
	//std::string zoneStr = paras[5];

	//double dlon = osgEarth::as(lon, 0.0);
	//double dlat = osgEarth::as(lat, 0.0);
	//double dho = osgEarth::as(heightOffset, 0.0);
	//osg::Vec3d location(dlon, dlat, dho);
	//int vzone = osgEarth::as(zoneStr, 0);

	//TiledDirectoryVisitor tdv;
	//tdv.traverse(inPath, outPath, location);

	TiledDirectoryVisitor tdv;
	//tdv.traverse("G:\\SuZhou\\SuZhou-osgb\\Data", "", osg::Vec3d(497223, 3724018, 0), 50);
	tdv.traverse("G:\\SuZhou\\SuZhou-osgb\\testData\\Data", "", osg::Vec3d(497223, 3724018, 0), 50);





	//osg::Vec3d pos = osg::Vec3d(-3049.7, 7702.77, 51.4314) + osg::Vec3d(497223, 3724018, 0);

	//WGS84Corr wgspos;
	//UTMXYToLatLon(pos.x(), pos.y(), 50, false, wgspos);

	//double lat = RadToDeg(wgspos.lat);
	//double log = RadToDeg(wgspos.log);

	//std::cout << "lat: " << lat << " log: " << log << std::endl;
	//tdv.traverse("Y:\\株洲\\part1+2+3+4+5\\Data", "G:\\SuZhou", osg::Vec3d(705888, 3086342, 0), 49);

	system("pause");

	return 0;
}
