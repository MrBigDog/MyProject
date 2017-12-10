#ifndef GWPIPELINE_IPIPELINELAYER_H
#define GWPIPELINE_IPIPELINELAYER_H 1

#include <osgEarth/ModelLayer>

namespace gwPipeline
{
	class IPipelineLayer : public osgEarth::ModelLayer
	{
	public:
		enum PipeType
		{
			CONTAINER	= 0x01,
			JOINT		= 0x02,
			EQUIPMENT	= 0x04,
			WELL		= 0x08,
			ALL_TYPE	= 0x0F,
			UNEXPECTED	= 0x10
		};

	public:
		IPipelineLayer(const osgEarth::ModelLayerOptions& options) : osgEarth::ModelLayer(options) {}
		IPipelineLayer(const std::string& name, const osgEarth::ModelSourceOptions& options) : osgEarth::ModelLayer(name, options) {}
		IPipelineLayer(const osgEarth::ModelLayerOptions& options, osgEarth::ModelSource* source) :osgEarth::ModelLayer(options, source) {}
		IPipelineLayer(const std::string& name, osg::Node* node) :osgEarth::ModelLayer(name, node) {}
		virtual ~IPipelineLayer()=0;

		virtual void setPipeType(PipeType type) { type_ = type; }
		virtual PipeType getPipeType() { return type_; }

	private:
		PipeType type_;
	};

	typedef std::vector< osg::ref_ptr<IPipelineLayer> > IPipelineLayerVector;
}

#endif // GWPIPELINE_PIPELINELAYER_H
