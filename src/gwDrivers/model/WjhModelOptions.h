#ifndef GWDRIVERS_MODEL_OPTIONS
#define GWDRIVERS_MODEL_OPTIONS 1

#include <osgEarth/Common>
#include <osgEarthFeatures/GeometryCompiler>
#include <osgEarthFeatures/FeatureModelSource>

namespace gwDrivers
{
	using namespace osgEarth;
	using namespace osgEarth::Features;

	class WJHModelOptions : public FeatureModelSourceOptions
	{
	public:
		WJHModelOptions(const ConfigOptions& options = ConfigOptions())
			: FeatureModelSourceOptions(options)
		{
			setDriver("wjh");
			fromConfig(_conf);
		}

		virtual ~WJHModelOptions() { }

		osgEarth::optional<osgEarth::URI>& modelUrl() { return _modelUrl; }
		const osgEarth::optional<osgEarth::URI>& modelUrl() const { return _modelUrl; }

		osgEarth::optional<bool>& isMemCache() { return _isMemcache; }
		const osgEarth::optional<bool>& isMemCache() const { return _isMemcache; }

		osgEarth::optional<unsigned int>& memCacheSize() { return _memCacheSize; }
		const osgEarth::optional<unsigned int>& memCacheSize() const { return _memCacheSize; }

	public:
		Config getConfig() const
		{
			Config conf = FeatureModelSourceOptions::getConfig();

			//fromConfig(conf);
			conf.updateIfSet("model_url", _modelUrl);
			conf.updateIfSet("is_mem_cache", _isMemcache);
			conf.updateIfSet("mem_cache_size", _memCacheSize);

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
		{
			conf.getIfSet("model_url", _modelUrl);
			conf.getIfSet("is_mem_cache", _isMemcache);
			conf.getIfSet("mem_cache_size", _memCacheSize);
		}

	private:
		osgEarth::optional<bool> _isMemcache;
		osgEarth::optional<unsigned int> _memCacheSize;
		osgEarth::optional<osgEarth::URI> _modelUrl;
	};

} // namespace gwDrivers

#endif // GWDRIVERS_MODEL_OPTIONS

