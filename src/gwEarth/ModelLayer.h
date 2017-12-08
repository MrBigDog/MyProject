#ifndef GWEARTH_MODELLAYER_H
#define GWEARTH_MODELLAYER_H 1

#include <compiler.h>
#include <osgEarth/ModelLayer>

namespace gwEarth
{
	//class ModelLayerOptions :public osgEarth::ModelLayerOptions
	//{
	//public:
	//	ModelLayerOptions(const osgEarth::ConfigOptions& options = osgEarth::ConfigOptions())
	//		:osgEarth::ModelLayerOptions(options)
	//	{}

	//	/** Construct or deserialize new model layer options. */
	//	ModelLayerOptions(const std::string& name,	const osgEarth::ModelSourceOptions& driverOptions = osgEarth::ModelSourceOptions());
	//	{}
	//	/** Copy ctor*/
	//	ModelLayerOptions(const ModelLayerOptions& rhs);

	//	/** dtor */
	//	virtual ~ModelLayerOptions() { }
	//};

	class ModelLayer :public osgEarth::ModelLayer
	{
	public:
		ModelLayer(const osgEarth::ModelLayerOptions& options) :osgEarth::ModelLayer(options) {}

	private:

	};
}


#endif // GWEARTH_MODELLAYER_H
