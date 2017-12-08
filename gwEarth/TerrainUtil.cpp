#include <gwEarth/TerrainUtil.h>
#include <gwEarth/TerrainSystem.h>

#include <osgEarthDrivers/tms/TMSOptions>
#include <osgEarthDrivers/gdal/GDALOptions>
#include <osgEarthDrivers/wms/WMSOptions>
#include <osgEarthDrivers/arcgis/ArcGISOptions>
#include <osgEarthDrivers/xyz/XYZOptions>
#include <osgEarthDrivers/feature_elevation/FeatureElevationOptions>
#include <osgEarthDrivers/gdal/GDALOptions>
#include <osgEarthDrivers/feature_ogr/OGRFeatureOptions>

using namespace osgEarth::Drivers;

namespace gwEarth
{
	//--------------------------------------------------------------------
	osgEarth::ImageLayer * TerrainUtil::createTMSImageLayer(const std::string & layername, const std::string & url)
	{
		osgEarth::Drivers::TMSOptions options;
		options.url() = osgEarth::URI(url);

		osgEarth::ImageLayerOptions layerOptions(options);
		layerOptions.textureCompression() = osg::Texture::USE_S3TC_DXT1_COMPRESSION;
		layerOptions.name() = layername;
		//layerOptions.transparentColor() = osg::Vec4ub(); //可以去除影像黑边

		osg::ref_ptr<osgEarth::ImageLayer> layer = new osgEarth::ImageLayer(layerOptions);
		return layer.release();
	}

	osgEarth::ImageLayer * TerrainUtil::createWMSImageLayer(const std::string & layername, const std::string & url)
	{
		return nullptr;
	}

	osgEarth::ImageLayer * TerrainUtil::createGDALImageLayer(const std::string & layername, const std::string & url)
	{
		GDALOptions options;
		options.url() = osgEarth::URI(url);

		osgEarth::ImageLayerOptions loptions(options);
		loptions.name() = layername;
		loptions.transparentColor() = osg::Vec4ub(); //可以去除影像黑边

		osg::ref_ptr<ImageLayer> layer = new ImageLayer(loptions);
		return layer.release();
	}

	osgEarth::ImageLayer * TerrainUtil::createArcgisImageLayer(const std::string & layername, const std::string & url)
	{
		osgEarth::Drivers::ArcGISOptions options;
		options.url() = osgEarth::URI(url);
		osg::ref_ptr<osgEarth::ImageLayer> layer = new osgEarth::ImageLayer(layername, options);
		return layer.release();
	}

	osgEarth::ImageLayer * TerrainUtil::createXYZImageLayer(const std::string & layername, const std::string & url, const std::string& profile, bool isInvertY)
	{
		osgEarth::ProfileOptions profileOptions(profile);
		osgEarth::Drivers::XYZOptions options;
		options.url() = osgEarth::URI(url);
		//options.format() = "png";
		options.invertY() = isInvertY;
		options.profile() = profileOptions;

		osg::ref_ptr<osgEarth::ImageLayer> layer = new osgEarth::ImageLayer(layername, options);
		return layer.release();
	}

	osgEarth::ImageLayer * TerrainUtil::createWCSImageLayer(const std::string & layername, const std::string & url)
	{
		return nullptr;
	}


	//------------------------------------------------------------------------------------------------------------------
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

	osgEarth::ElevationLayer * TerrainUtil::createWCSElevationLayer(const std::string & layername, const std::string & url)
	{
		return nullptr;
	}

	osgEarth::ElevationLayer * TerrainUtil::createGDALElevationLayer(const std::string & layername, const std::string & url)
	{
		return nullptr;
	}

	osgEarth::ElevationLayer * TerrainUtil::createArcgisElevationLayer(const std::string & layername, const std::string & url)
	{
		return nullptr;
	}

	osgEarth::ElevationLayer * TerrainUtil::createFeatureElevationLayer(const std::string & name, const std::string & url)
	{
		OGRFeatureOptions ogroptions;
		ogroptions.name() = "falt_feature";
		ogroptions.url() = "E:/BYL/BUILD/osgearth-2.8/data/flatten_mt_rainier.shp";
		ogroptions.buildSpatialIndex() = true;

		FeatureElevationOptions options;
		options.featureOptions() = ogroptions;
		options.profile() = osgEarth::ProfileOptions("global-geodetic");

		osg::ref_ptr<osgEarth::ElevationLayer> layer = new osgEarth::ElevationLayer(name, options);
		return layer.release();
	}



}