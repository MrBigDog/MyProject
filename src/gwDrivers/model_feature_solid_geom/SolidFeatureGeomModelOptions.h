#ifndef GWDRIVERS_SOLIDFEATUREGEOMMODELOPTIONS_H
#define GWDRIVERS_SOLIDFEATUREGEOMMODELOPTIONS_H 1

#include <osgEarthFeaturesExt/SolidGeometryCompiler.h>
#include <osgEarth/Common>
#include <osgEarthFeatures/FeatureModelSource>

using namespace oeFeaturesExt;

namespace gwDrivers
{
	using namespace osgEarth;
	using namespace osgEarth::Features;

	class SolidFeatureGeomModelOptions : public FeatureModelSourceOptions
	{
	public:
		SolidFeatureGeomModelOptions(const ConfigOptions& options = ConfigOptions())
			: FeatureModelSourceOptions(options)
		{
			setDriver("solid_feature_geom");//_solid_feature_geom
			fromConfig(_conf);
		}
		virtual ~SolidFeatureGeomModelOptions() { }

		SolidGeometryCompilerOptions& compilerOptions() { return _compilerOptions; }
		const SolidGeometryCompilerOptions& compilerOptions() const { return _compilerOptions; }

		osgEarth::optional<std::string> radiusPropertyName() { return _radiusPropertyName; }
		const osgEarth::optional<std::string> radiusPropertyName() const { return _radiusPropertyName; }

	public:
		Config getConfig() const
		{
			Config conf = FeatureModelSourceOptions::getConfig();
			return conf;
		}

	protected:
		virtual void mergeConfig(const Config& conf)
		{
			FeatureModelSourceOptions::mergeConfig(conf);
			fromConfig(conf);
		}

	private:
		void fromConfig(const Config& conf)
		{}

	private:
		osgEarth::optional<std::string> _radiusPropertyName;
		SolidGeometryCompilerOptions _compilerOptions;
	};

} // namespace gwDrivers

#endif // GWDRIVERS_SolidFeatureGeomModelOptions_H

