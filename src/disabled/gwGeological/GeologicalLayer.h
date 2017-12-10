#ifndef GWGEOLOGICAL_GEOLOGICALLAYER_H
#define GWGEOLOGICAL_GEOLOGICALLAYER_H 1

#include <gwGeological/IGeologicalLayer.h>

namespace gwGeological
{
	class GeologicalLayer :public IGeologicalLayer
	{
	public:
		GeologicalLayer(const osgEarth::ModelLayerOptions& options) : IGeologicalLayer(options) {}
		GeologicalLayer(const std::string& name, const osgEarth::ModelSourceOptions& options) : IGeologicalLayer(name, options) {}
		GeologicalLayer(const osgEarth::ModelLayerOptions& options, osgEarth::ModelSource* source) :IGeologicalLayer(options, source) {}
		GeologicalLayer(const std::string& name, osg::Node* node) :IGeologicalLayer(name, node) {}
		~GeologicalLayer() {}
	};
}

#endif // GWGEOLOGICAL_GEOLOGICALLAYER_H
