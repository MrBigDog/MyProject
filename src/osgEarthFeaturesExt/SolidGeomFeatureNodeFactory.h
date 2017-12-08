#ifndef OEFEATURESEXT_SOLIDEGEOMFEATURENODEFACTORY_H
#define OEFEATURESEXT_SOLIDEGEOMFEATURENODEFACTORY_H 1

#include <osgEarthFeaturesExt/Export.h>
#include <osgEarthFeaturesExt/SolidGeometryCompiler.h>
#include <osgEarthFeatures/FeatureModelSource>

namespace oeFeaturesExt
{
	class OSGEARTHFEATRUESEXT_EXPORT SolidGeomFeatureNodeFactory : public osgEarth::Features::FeatureNodeFactory
	{
	public:
		SolidGeomFeatureNodeFactory(const SolidGeometryCompilerOptions& options);

		bool createOrUpdateNode(FeatureCursor* features
			, const Style& style
			, const FilterContext& context
			, osg::ref_ptr<osg::Node>& node);

	public:
		SolidGeometryCompilerOptions options_;
	};
}

#endif // GWPIPELINE_SolidGeomFeatureNodeFactory_H
