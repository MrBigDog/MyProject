#ifndef GWPIPELINE_PIPELINELAYER_H
#define GWPIPELINE_PIPELINELAYER_H 1

#include <gwPipeline/IPipelineLayer.h>

namespace gwPipeline
{
	class PipelineLayer : public IPipelineLayer
	{
	public:
		PipelineLayer(const osgEarth::ModelLayerOptions& options) :IPipelineLayer(options) {}
		PipelineLayer(const std::string& name, const osgEarth::ModelSourceOptions& options) :IPipelineLayer(name, options) {}
		PipelineLayer(const osgEarth::ModelLayerOptions& options, osgEarth::ModelSource* source) :IPipelineLayer(options, source) {}
		PipelineLayer(const std::string& name, osg::Node* node) :IPipelineLayer(name, node) {}
		~PipelineLayer() {}
	};

	//typedef std::vector< osg::ref_ptr<PipelineLayer> > PipelineLayers;
}

#endif // GWPIPELINE_PIPELINELAYER_H
