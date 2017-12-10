#ifndef GWEARTH_ICOMMONMODELLAYER_H
#define GWEARTH_ICOMMONMODELLAYER_H 1

#include <osgEarth/ModelLayer>
#include <osg/ref_ptr>
#include <string>
#include <vector>

namespace gwEarth
{
	class ICModelLayer : public osgEarth::ModelLayer
	{
	public:
		ICModelLayer(const osgEarth::ModelLayerOptions& options) : osgEarth::ModelLayer(options){ }
		ICModelLayer(const std::string& name, const osgEarth::ModelSourceOptions& options) : osgEarth::ModelLayer(name, options){ }
		ICModelLayer(const osgEarth::ModelLayerOptions& options, osgEarth::ModelSource* source) :osgEarth::ModelLayer(options, source){ }
		ICModelLayer(const std::string& name, osg::Node* node) :osgEarth::ModelLayer(name, node){ }
		
		virtual ~ICModelLayer() = 0;
	};

	typedef std::vector< osg::ref_ptr<ICModelLayer> > ICModelLayerVector;
}

#endif // GWEARTH_ICOMMONMODELLAYER_H
