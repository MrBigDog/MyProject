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

#define LC "[WjhConfiglSource] "

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
			std::string prj = "+proj=tmerc +lat_0=0 +lon_0=6 +k=1 +x_0=2500000 +y_0=0 +ellps=bessel +datum=potsdam +units=m +no_defs";

			osgEarth::SpatialReference* srs = osgEarth::SpatialReference::create(prj);
			osgEarth::GeoExtent extent(srs);

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

					//osgEarth::GeoPoint pos(srs, posX, posY, posZ);

					extent.expand(posX, posY);
				}
			}

			FeatureProfile* fp = new FeatureProfile(extent);
			setFeatureProfile(fp);

			return Status::OK();
		}

		virtual osgEarth::Features::FeatureCursor* createFeatureCursor(const osgEarth::Symbology::Query& query)
		{
			const osgEarth::Config* modelclass = _featuresConfig.find("modelpointclass");
			if (!modelclass) return;

			FeatureCursor* result = 0L;

			FeatureList features;
			const osgEarth::ConfigSet& models = modelclass->children("modelpoint");
			for (osgEarth::ConfigSet::const_iterator it = models.begin(); it != models.end(); ++it)
			{
				std::string name = it->value("modelname");

				double posX = osgEarth::as(it->value("locationx"), 0.0);
				double posY = osgEarth::as(it->value("locationy"), 0.0);
				double posZ = osgEarth::as(it->value("locationz"), 0.0);

				osgEarth::GeoPoint pos;
				if (query.bounds().value().contains(pos.x(), pos.y()))
				{
					//·ûºÏÌõ¼þ;
					std::string matStr = it->value("matrix3");
					osgEarth::Features::Feature* feature;
				}
			}
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
	};


};


class WJHConfigSourceFactory : public FeatureSourceDriver
{
public:
	WJHConfigSourceFactory()
	{
		supportsExtension("osgearth_feature_wjhconfig", "wjhconfig driver for osgEarth");
	}

	virtual const char* className() const
	{
		return "wjhconfig Feature Reader";
	}

	virtual ReadResult readObject(const std::string& file_name, const osgDB::Options* readOptions) const
	{
		if (!acceptsExtension(osgDB::getLowerCaseFileExtension(file_name)))
			return ReadResult::FILE_NOT_HANDLED;

		return ReadResult(new WJHConfigSource(getFeatureSourceOptions(readOptions)));
	}
};

REGISTER_OSGPLUGIN(osgearth_feature_wfs, WJHConfigSourceFactory)