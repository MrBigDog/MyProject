#include <gwPipeline/PipelineGeomFeatureNodeFactory.h>

namespace gwPipeline
{
	PipelineGeomFeatureNodeFactory::PipelineGeomFeatureNodeFactory( const PipelineGeometryCompilerOptions& options ) 
		: options_( options ) 
	{
		//nop
	}

	bool PipelineGeomFeatureNodeFactory::createOrUpdateNode(
		FeatureCursor*            features,
		const Style&              style,
		const FilterContext&      context,
		osg::ref_ptr<osg::Node>&  node )
	{
		PipelineGeometryCompiler compiler( options_ );
		node = compiler.compile( features, style, context );
		return node.valid();
	}
}