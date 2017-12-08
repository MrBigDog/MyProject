#include <gwEarth/TerrainUtil.h>

namespace gwEarth
{
	osgEarth::ImageLayer * TerrainUtil::createTMSImageLayer(const std::string & layername, const std::string & url)
	{
		osgEarth::Drivers::TMSOptions options;
		options.url() = osgEarth::URI(url);
		osg::ref_ptr<osgEarth::ImageLayer> layer = new osgEarth::ImageLayer(layername, options);
		return layer.release();
	}

	osgEarth::ImageLayer * TerrainUtil::createWMSImageLayer(const std::string & layername, const std::string & url)
	{
		return nullptr;
	}

	osgEarth::ImageLayer * TerrainUtil::createGDALImageLayer(const std::string & layername, const std::string & url)
	{
		return nullptr;
	}


	osgEarth::ElevationLayer * TerrainUtil::createTMSElevationLayer(const std::string & layername, const std::string & url)
	{
		osgEarth::Drivers::TMSOptions options;
		options.url() = osgEarth::URI(url);
		osg::ref_ptr<osgEarth::ElevationLayer> layer = new osgEarth::ElevationLayer(layername, options);
		return layer.release();
	}

	osgEarth::ElevationLayer * TerrainUtil::createWMSElevationLayer(const std::string & layername, const std::string & url)
	{
		return nullptr;
	}

	osgEarth::ElevationLayer * TerrainUtil::createGDALElevationLayer(const std::string & layername, const std::string & url)
	{
		return nullptr;
	}

}