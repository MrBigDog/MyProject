/*
* 取到矢量数据后对数据做些处理，如果数据中没有高程数据，或者高程数据不在Geometry字段中，
* 就需要得到高程数据，然后把高程数据写进Geometry字段中。BigDog-2017.02.15
*/

#ifndef GWPIPELINE_PIPELINERESETALTFILTER_H
#define GWPIPELINE_PIPELINERESETALTFILTER_H 1

#include <osgEarthFeatures/Common>
#include <osgEarthFeatures/Feature>
#include <osgEarthFeatures/Filter>
#include <osg/Geode>

using namespace osgEarth;
using namespace osgEarth::Features;

namespace gwPipeline
{
	class ResetAltFilterOptions : public osgEarth::ConfigOptions
	{
	public:
		ResetAltFilterOptions(const osgEarth::ConfigOptions& co = osgEarth::ConfigOptions())
			: osgEarth::ConfigOptions(co) 
		{
			_minAlt.init(-10000.0);
			_maxAlt.init(10000.0);
			_conf.key() = "ResetPipelineAlt";
			fromConfig(_conf);
		}

		enum ResetMode
		{
			RESET_CONTAINER /*= 0x01*/,
			RESET_JOINT     /*= 0x02*/,
			RESET_WELL      /*= 0x04*/,
			RESET_EQUIPMENT /*= 0x08*/,
			//RESET_POINT     /*= 0x0E*/
		};

		osgEarth::optional<double>& minAlt() { return _minAlt; }
		const osgEarth::optional<double>& minAlt() const { return _minAlt; }

		osgEarth::optional<double>& maxAlt() { return _maxAlt; }
		const osgEarth::optional<double>& maxAlt() const { return _maxAlt; }

		osgEarth::optional<ResetMode>& type() { return _type; }
		const osgEarth::optional<ResetMode> type() const { return _type; }

		void fromConfig(const osgEarth::Config& conf) 
		{
			conf.getIfSet("min_alt", _minAlt);
			conf.getIfSet("max_alt", _maxAlt);
			conf.getIfSet("mode", "container", _type, RESET_CONTAINER);
			conf.getIfSet("mode", "joint"    , _type, RESET_JOINT);
			conf.getIfSet("mode", "well"     , _type, RESET_WELL);
			conf.getIfSet("mode", "equipment", _type, RESET_EQUIPMENT);
			//conf.getIfSet("mode", "point"    , _type, RESET_POINT);
		}

		osgEarth::Config getConfig() const 
		{
			osgEarth::Config conf = osgEarth::ConfigOptions::getConfig();
			conf.addIfSet("min_alt", _minAlt);
			conf.addIfSet("max_alt", _maxAlt);
			conf.addIfSet("mode", "container", _type, RESET_CONTAINER);
			conf.addIfSet("mode", "joint"    , _type, RESET_JOINT);
			conf.addIfSet("mode", "well"     , _type, RESET_WELL);
			conf.addIfSet("mode", "equipment", _type, RESET_EQUIPMENT);
			//conf.addIfSet("mode", "point"    , _type, RESET_POINT);
			return conf;
		}

	protected:
		osgEarth::optional<double> _maxAlt, _minAlt;
		osgEarth::optional<ResetMode> _type;
	};

	class ResetAltFilter : public osgEarth::Features::FeatureFilter, public ResetAltFilterOptions
	{
	public:
		static bool isSupported();

	public:
		ResetAltFilter();
		ResetAltFilter(double minAlt, double maxAlt);
		ResetAltFilter(const osgEarth::ConfigOptions& conf);

		~ResetAltFilter();

	public:
		virtual FilterContext push(FeatureList& input, FilterContext& context);

	protected:
		bool push(Feature* input, FilterContext& context);
	};


}
#endif // GWPIPELINE_PIPELINERESETALTFILTER_H
