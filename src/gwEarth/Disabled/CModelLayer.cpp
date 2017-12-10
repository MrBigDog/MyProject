#include <gwEarth/CModelLayer.h>

namespace gwEarth
{
	CModelLayer::CModelLayer(const osgEarth::ModelLayerOptions& options)
		: ICModelLayer(options)
	{ }

	CModelLayer::CModelLayer(const std::string& name, const osgEarth::ModelSourceOptions& options)
		: ICModelLayer(name, options)
	{ }

	CModelLayer::CModelLayer(const osgEarth::ModelLayerOptions& options, osgEarth::ModelSource* source)
		: ICModelLayer(options, source)
	{ }

	CModelLayer::CModelLayer(const std::string& name, osg::Node* node)
		: ICModelLayer(name, node)
	{ }

	CModelLayer::~CModelLayer()
	{ }
}
