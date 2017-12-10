#ifndef GWGEOLOGICAL_IGEOLOGICALLAYER_H
#define GWGEOLOGICAL_IGEOLOGICALLAYER_H 1

#include <osgEarth/ModelLayer>

namespace gwGeological
{
	class IGeologicalLayer :public osgEarth::ModelLayer
	{
	public:
		IGeologicalLayer(const osgEarth::ModelLayerOptions& options) : osgEarth::ModelLayer(options) {}
		IGeologicalLayer(const std::string& name, const osgEarth::ModelSourceOptions& options) : osgEarth::ModelLayer(name, options) {}
		IGeologicalLayer(const osgEarth::ModelLayerOptions& options, osgEarth::ModelSource* source) :osgEarth::ModelLayer(options, source) {}
		IGeologicalLayer(const std::string& name, osg::Node* node) :osgEarth::ModelLayer(name, node) {}
		virtual ~IGeologicalLayer() = 0;
	};

	typedef std::vector<osg::ref_ptr<IGeologicalLayer> > IGeologicalLayerVector;
}
#endif // GWGEOLOGICAL_IGEOLOGICALLAYER_H