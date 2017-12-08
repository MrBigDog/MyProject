#ifndef GWEARTH_TERRAINUTIL_H
#define GWEARTH_TERRAINUTIL_H 1

#include <Compiler.h>
#include <gwEarth/Export.h>
#include <osgEarth/ImageLayer>
#include <osgEarth/ElevationLayer>

namespace gwApp { class Application; }

namespace gwEarth
{
	class TerrainSystem;

	class GWEARTH_EXPORT TerrainUtil
	{
	public:
		//--create image layer
		static osgEarth::ImageLayer* createTMSImageLayer(const std::string& layername, const std::string&url);
		static osgEarth::ImageLayer* createWMSImageLayer(const std::string& layername, const std::string&url);
		static osgEarth::ImageLayer* createWCSImageLayer(const std::string& layername, const std::string&url);
		static osgEarth::ImageLayer* createGDALImageLayer(const std::string& layername, const std::string&url);
		static osgEarth::ImageLayer* createArcgisImageLayer(const std::string& layername, const std::string&url);
		//XYZ可以加载二维地图，比如OSM、Google地图、百度地图等。
		static osgEarth::ImageLayer* createXYZImageLayer(const std::string& layername, const std::string&url, const std::string&profile, bool isInvertY);

		//--create elevation layer
		static osgEarth::ElevationLayer* createTMSElevationLayer(const std::string& layername, const std::string&url);
		static osgEarth::ElevationLayer* createWMSElevationLayer(const std::string& layername, const std::string&url);
		static osgEarth::ElevationLayer* createWCSElevationLayer(const std::string& layername, const std::string&url);
		static osgEarth::ElevationLayer* createGDALElevationLayer(const std::string& layername, const std::string&url);
		static osgEarth::ElevationLayer* createArcgisElevationLayer(const std::string& layername, const std::string&url);
		static osgEarth::ElevationLayer* createFeatureElevationLayer(const std::string& name, const std::string& url);
	};
}

#endif // GWEARTH_TERRAINUTIL_H
