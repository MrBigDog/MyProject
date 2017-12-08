#include "SolidGeomFeatureNodeFactory.h"

namespace oeFeaturesExt
{
	SolidGeomFeatureNodeFactory::SolidGeomFeatureNodeFactory(const SolidGeometryCompilerOptions& options)
		: options_(options)
	{}

	bool SolidGeomFeatureNodeFactory::createOrUpdateNode(
		FeatureCursor*            features,
		const Style&              style,
		const FilterContext&      context,
		osg::ref_ptr<osg::Node>&  node)
	{
		SolidGeometryCompiler compiler(options_);
		node = compiler.compile(features, style, context);
		return node.valid();
	}
}