#include <mdCommonFeature/CommonFeatureLayer.h>
#include <osgEarth/ModelSource>

namespace mdCommonFeature
{
	CommonFeatureLayer::CommonFeatureLayer(const osgEarth::ModelLayerOptions& options, osgEarth::ModelSource* source )
		: osgEarth::ModelLayer(options, source)
	{

	}

	CommonFeatureLayer::CommonFeatureLayer( const osgEarth::ModelLayerOptions& options )
		: osgEarth::ModelLayer(options)
	{

	}

	CommonFeatureLayer::CommonFeatureLayer( const std::string& name, const osgEarth::ModelSourceOptions& options )
		: osgEarth::ModelLayer(name, options)
	{

	}

	CommonFeatureLayer::~CommonFeatureLayer()
	{

	}
}