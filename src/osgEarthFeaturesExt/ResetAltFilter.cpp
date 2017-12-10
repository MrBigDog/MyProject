#include <mdPipeline/PipelineResetAltFilter.h>

#include <osgEarthFeatures/Filter>
#include <osgEarthFeatures/FeatureSource>
#include <osgEarth/Registry>

#include <osg/Math>
#include <osgDB/FileNameUtils>
#include <osgDB/FileUtils>


namespace mdPipeline
{
	/*OSGEARTH_REGISTER_SIMPLE_FEATUREFILTER的参数1要和_conf.key()相对应，否则不能不能注册到程序中。*/
	OSGEARTH_REGISTER_SIMPLE_FEATUREFILTER(ResetPipelineAlt, ResetAltFilter);

	ResetAltFilter::ResetAltFilter()
	{
	}

	ResetAltFilter::ResetAltFilter(double minAlt, double maxAlt)
	{

	}

	ResetAltFilter::ResetAltFilter(const osgEarth::ConfigOptions& conf)
	{

	}

	ResetAltFilter::~ResetAltFilter()
	{

	}

	bool ResetAltFilter::isSupported()
	{
		return true;
	}

	FilterContext ResetAltFilter::push(FeatureList& input, FilterContext& context)
	{
		if (!isSupported())
		{
			OE_WARN << "ResetAltFilter support not enabled" << std::endl;
			return context;
		}

		bool ok = true;
		for (FeatureList::iterator i = input.begin(); i != input.end(); ++i)
			if (!push(i->get(), context))
				ok = false;

		return context;
	}

	bool ResetAltFilter::push(Feature* input, FilterContext& context)
	{
		if (!input || !input->getGeometry())
			return true;

		bool success = true;

		double alt;

		//具体的reset alt操作;
		if (type().value() == RESET_CONTAINER)
		{
			double sAlt = 1900, eAlt = 1900;

			GeometryIterator i(input->getGeometry());
			while (i.hasMore())
			{
				Geometry* part = i.next();
				if (part->size() != 2) continue;

				Geometry::iterator it = part->begin();
				(*it).z() = sAlt; it++;
				(*it).z() = eAlt;
			}
			return true;
		}
		else if (
			type().value() == RESET_JOINT ||
			type().value() == RESET_WELL  ||
			type().value() == RESET_EQUIPMENT
			)
		{
			double alt = 1900;

			GeometryIterator i(input->getGeometry());
			while (i.hasMore())
			{
				Geometry* part = i.next();
				if (part->size() != 1) continue;

				(*part->begin()).z() = alt;
			}
			return true;
		}

		return false;
	}



	//class ResetPipelineAltFilterPlugin : public FeatureFilterDriver
	//{
	//public:
	//	ResetPipelineAltFilterPlugin() : FeatureFilterDriver()
	//	{
	//		this->supportsExtension("osgearth_featurefilter_reset_pipeline_alt", className());
	//	}

	//	const char* className() const
	//	{
	//		return "ResetPipelineAltFilterPlugin";
	//	}

	//	ReadResult readObject(const std::string& file_name, const Options* options) const
	//	{
	//		if (!acceptsExtension(osgDB::getLowerCaseFileExtension(file_name)))
	//			return ReadResult::FILE_NOT_HANDLED;

	//		return new ResetAltFilter(getConfigOptions(options));
	//	}
	//};

	//OSGEARTH_REGISTER_SIMPLE_FEATUREFILTER(resetAlt, ResetAltFilter)
	//{
	//}

	//REGISTER_OSGPLUGIN(osgearth_featurefilter_reset_pipeline_alt, ResetPipelineAltFilterPlugin);
}