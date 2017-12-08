#include <osgEarth/Map>
#include <osgEarth/Registry>
#include <osgEarth/ModelSource>
#include <osgEarth/GeoTransform>
#include <osgEarthFeatures/GeometryCompiler>

#include <osg/Notify>
#include <osg/MatrixTransform>
#include <osgDB/FileNameUtils>

#include "WjhConfigOptions.h"

using namespace osgEarth;
using namespace osgEarth::Features;
using namespace osgEarth::Symbology;
using namespace gwDrivers;

#define LC "[WJHConfigSource] "

namespace
{
	class WJHConfigSource :public osgEarth::Features::FeatureSource
	{
	public:
		WJHConfigSource(const WJHConfigOptions& options)
			: FeatureSource(options)
			, _featureOption(options)
		{ }

		virtual ~WJHConfigSource() {}

		//override
		Status initialize(const osgDB::Options* readOptions)
		{
			_wgs84Srs = osgEarth::SpatialReference::create("wgs84");

			//Google Proj4js.defs["EPSG:2436"]
			std::string gauss3 = "+proj=tmerc +lat_0=0 +lon_0=117 +k=1 +x_0=39500000 +y_0=0 +ellps=krass +units=m +no_defs";

			_dataSrs = osgEarth::SpatialReference::create(gauss3);

			osgEarth::GeoExtent extent(_dataSrs);

			_featureOption.filters();

			readConfigFromXml(_featureOption.url().value(), _featuresConfig);
			const osgEarth::Config* modelclass = _featuresConfig.find("modelpointclass");
			if (modelclass)
			{
				const osgEarth::ConfigSet& models = modelclass->children("modelpoint");
				for (osgEarth::ConfigSet::const_iterator it = models.begin(); it != models.end(); ++it)
				{
					std::string name = it->value("modelname");

					double posX = osgEarth::as(it->value("locationx"), 0.0);
					double posY = osgEarth::as(it->value("locationy"), 0.0);
					double posZ = osgEarth::as(it->value("locationz"), 0.0);

					extent.expandToInclude(osg::Vec3d(posX, posY, posZ));
				}
			}

			osgEarth::GeoExtent wgs84extent;
			extent.transform(_wgs84Srs, wgs84extent);

			osg::ref_ptr<FeatureProfile> fp = new FeatureProfile(wgs84extent);
			setFeatureProfile(fp);

			return Status::OK();
		}

		virtual osgEarth::Features::FeatureCursor* createFeatureCursor(const osgEarth::Symbology::Query& query)
		{
			const osgEarth::Config* modelclass = _featuresConfig.find("modelpointclass");
			if (!modelclass) return 0L;

			FeatureCursor* result = 0L;

			FeatureList features;
			const osgEarth::ConfigSet& models = modelclass->children("modelpoint");
			for (osgEarth::ConfigSet::const_iterator it = models.begin(); it != models.end(); ++it)
			{
				std::string name = it->value("modelname");

				double posX = osgEarth::as(it->value("locationx"), 0.0);
				double posY = osgEarth::as(it->value("locationy"), 0.0);
				double posZ = osgEarth::as(it->value("locationz"), 0.0);

				osgEarth::GeoPoint pos(_dataSrs, posX, posY);
				osgEarth::GeoPoint wgs84pos;
				pos.transform(_wgs84Srs, wgs84pos);
				if (query.bounds().value().contains(wgs84pos.x(), wgs84pos.y()))
				{
					//·ûºÏÌõ¼þ;
					std::string matStr = it->value("matrix3");

					osgEarth::Geometry* geom = new osgEarth::Geometry;
					geom->push_back(osg::Vec3d(wgs84pos.x(), wgs84pos.y(), wgs84pos.z() + 50.0));

					osg::ref_ptr<osgEarth::Features::Feature> feature =
						new osgEarth::Features::Feature(geom, _wgs84Srs);
					feature->set("matrix3", matStr);
					feature->set("node_name", name);
					features.push_back(feature);
				}
			}

			result = new FeatureListCursor(features);

			return result;
		}

	private:
		bool readConfigFromXml(const osgEarth::URI& url, osgEarth::Config& arg)
		{
			osgEarth::ReadResult r = url.readString();
			if (r.failed())
			{
				return false;
			}
			std::stringstream buf(r.getString());
			arg.fromXML(buf);
			return true;
		}

	private:
		osgEarth::Config _featuresConfig;
		WJHConfigOptions _featureOption;

		osg::ref_ptr<osgEarth::SpatialReference> _dataSrs;
		osg::ref_ptr<osgEarth::SpatialReference> _wgs84Srs;
	};
}

class WJHConfigSourceFactory : public FeatureSourceDriver
{
public:
	WJHConfigSourceFactory()
	{
		supportsExtension("osgearth_feature_wjh", "wjh feature driver for osgEarth");
	}

	virtual const char* className() const
	{
		return "wjh Feature Reader";
	}

	virtual ReadResult readObject(const std::string& file_name, const osgDB::Options* readOptions) const
	{
		if (!acceptsExtension(osgDB::getLowerCaseFileExtension(file_name)))
		{
			return ReadResult::FILE_NOT_HANDLED;
		}
		return ReadResult(new WJHConfigSource(getFeatureSourceOptions(readOptions)));
	}
};

REGISTER_OSGPLUGIN(osgearth_feature_wjh, WJHConfigSourceFactory)
