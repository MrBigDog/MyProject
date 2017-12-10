#ifndef CommonFeatureLayer_h__
#define CommonFeatureLayer_h__

#include <mdBase/export.h>
#include <osgEarth/ModelLayer>
#include <string>
#include <vector>

namespace mdCommonFeature
{
	class MD_EXPORT CommonFeatureLayer:public osgEarth::ModelLayer
	{
	public:
		CommonFeatureLayer( const osgEarth::ModelLayerOptions& options );
        CommonFeatureLayer( const std::string& name, const osgEarth::ModelSourceOptions& options );
        CommonFeatureLayer(const osgEarth::ModelLayerOptions& options, osgEarth::ModelSource* source );
		~CommonFeatureLayer();
	};

	typedef std::vector<osg::ref_ptr<CommonFeatureLayer> > CommonFeatureLayerVector;
}

#endif // CommonFeatureLayer_h__

