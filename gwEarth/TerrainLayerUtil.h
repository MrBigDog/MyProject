#ifndef GWEARTH_TERRAINLAYERUTIL_H
#define GWEARTH_TERRAINLAYERUTIL_H 1

#include <osgEarth/ImageLayer>
#include <osgEarth/ElevationLayer>

#include <osgEarthDrivers/tms/TMSOptions>

namespace gwEarth
{
	class TerrainUtil
	{
	public:
		static osgEarth::ImageLayer* createTMSImageLayer(const std::string& layername, const std::string&url);
		static osgEarth::ImageLayer* createWMSImageLayer(const std::string& layername, const std::string&url);
		static osgEarth::ImageLayer* createGDALImageLayer(const std::string& layername, const std::string&url);
		static osgEarth::ImageLayer* createArcgisImageLayer(const std::string& layername, const std::string&url);

		static osgEarth::ElevationLayer* createTMSElevationLayer(const std::string& layername, const std::string&url);
		static osgEarth::ElevationLayer* createWMSElevationLayer(const std::string& layername, const std::string&url);
		static osgEarth::ElevationLayer* createGDALElevationLayer(const std::string& layername, const std::string&url);

	};
}

#endif // GWEARTH_UTIL_H
