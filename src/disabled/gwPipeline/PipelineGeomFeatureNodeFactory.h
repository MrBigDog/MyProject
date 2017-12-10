#ifndef GWPIPELINE_PIPELINEGEOMFEATURENODEFACTORY_H
#define GWPIPELINE_PIPELINEGEOMFEATURENODEFACTORY_H 1

#include <gwPipeline/PipelineGeometryCompiler.h>
#include <osgEarthFeatures/FeatureModelSource>

namespace gwPipeline
{
	class PipelineGeomFeatureNodeFactory: public osgEarth::Features::FeatureNodeFactory
	{
	public:
		PipelineGeomFeatureNodeFactory(const PipelineGeometryCompilerOptions& options);

		bool createOrUpdateNode(FeatureCursor* features
			, const Style& style
			, const FilterContext& context
			, osg::ref_ptr<osg::Node>& node );

	public:
		PipelineGeometryCompilerOptions options_;
	};
}

#endif // GWPIPELINE_PIPELINEGEOMFEATURENODEFACTORY_H
