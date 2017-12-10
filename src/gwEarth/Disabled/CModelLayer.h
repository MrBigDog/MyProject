#ifndef GWEARTH_COMMONMODELLAYER_H
#define GWEARTH_COMMONMODELLAYER_H 1

#include <gwEarth/ICModelLayer.h>
#include <osg/ref_ptr>
#include <string>
#include <vector>

namespace gwEarth
{
	class CModelLayer : public ICModelLayer
	{
	public:
		CModelLayer(const osgEarth::ModelLayerOptions& options);
		CModelLayer(const std::string& name, const osgEarth::ModelSourceOptions& options);
		CModelLayer(const osgEarth::ModelLayerOptions& options, osgEarth::ModelSource* source);
		CModelLayer(const std::string& name, osg::Node* node);

		~CModelLayer();
	};

	//typedef std::vector< osg::ref_ptr<CModelLayer> > CModelLayerVector;
}

#endif // GWEARTH_COMMONMODELLAYER_H
