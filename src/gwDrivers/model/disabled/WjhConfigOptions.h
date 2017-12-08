#ifndef GWDRIVERS_CONFIG_OPTIONS
#define GWDRIVERS_CONFIG_OPTIONS 1

#include <osgEarth/Common>
#include <osgEarthFeatures/GeometryCompiler>
#include <osgEarthFeatures/FeatureModelSource>

namespace gwDrivers
{
	using namespace osgEarth;
	using namespace osgEarth::Features;

	class WJHConfigOptions : public osgEarth::Features::FeatureSourceOptions
	{
	public:
		WJHConfigOptions(const ConfigOptions& opt = ConfigOptions())
			: FeatureSourceOptions(opt)
		{
			setDriver("wjh");
		}
		osgEarth::optional<osgEarth::URI>& url() { return _url; }
		const osgEarth::optional<osgEarth::URI>& url() const { return _url; }

	public:
		Config getConfig() const
		{
			Config conf = FeatureSourceOptions::getConfig();
			conf.updateIfSet("url", _url);
			return conf;
		}

	protected:
		void mergeConfig(const Config& conf)
		{
			FeatureSourceOptions::mergeConfig(conf);
			fromConfig(conf);
		}

	private:
		void fromConfig(const Config& conf)
		{
			conf.getIfSet("url", _url);
		}

	private:
		osgEarth::optional<osgEarth::URI> _url;
	};

} // namespace gwDrivers

#endif // GWDRIVERS_CONFIG_OPTIONS

