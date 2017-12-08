#include <osgEarthFeaturesExt/SolidGeometryCompiler.h>
#include <osgEarthFeaturesExt/BuildSolidGeometryFilter.h>
#include <osgEarthFeaturesExt/SolidLineSymbol.h>

#include <osgEarthFeatures/BuildGeometryFilter>
#include <osgEarthFeatures/BuildTextFilter>
#include <osgEarthFeatures/AltitudeFilter>
#include <osgEarthFeatures/CentroidFilter>
#include <osgEarthFeatures/ExtrudeGeometryFilter>
#include <osgEarthFeatures/ScatterFilter>
#include <osgEarthFeatures/SubstituteModelFilter>
#include <osgEarthFeatures/TessellateOperator>
#include <osgEarthFeatures/Session>
#include <osgEarth/Utils>
//#include <osgEarth/AutoScale>
#include <osgEarth/CullingUtils>
#include <osgEarth/Registry>
#include <osgEarth/Capabilities>
#include <osgEarth/ShaderGenerator>
#include <osgEarth/ShaderUtils>
#include <osgEarth/Utils>
#include <osg/MatrixTransform>
#include <osg/Timer>
#include <osgDB/WriteFile>
#include <osgUtil/Optimizer>

#define LC "[SolidGeometryCompiler] "

namespace oeFeaturesExt
{
	SolidGeometryCompilerOptions SolidGeometryCompilerOptions::s_defaults(true);

	void SolidGeometryCompilerOptions::setDefaults(const SolidGeometryCompilerOptions& defaults)
	{
		s_defaults = defaults;
	}

	SolidGeometryCompilerOptions::SolidGeometryCompilerOptions(bool stockDefaults)
		: _maxGranularity_deg(10.0)
		, _mergeGeometry(true)
		, _clustering(false)
		, _instancing(false)
		, _ignoreAlt(false)
		, _useVertexBufferObjects(true)
		, _shaderPolicy(SHADERPOLICY_GENERATE)
		, _geoInterp(GEOINTERP_GREAT_CIRCLE)
		, _optimizeStateSharing(true)
		, _optimize(false)
		, _validate(false)
		, _maxPolyTilingAngle(45.0f)
	{ }

	//-----------------------------------------------------------------------
	SolidGeometryCompilerOptions::SolidGeometryCompilerOptions(const ConfigOptions& conf)
		: ConfigOptions(conf)
		, _maxGranularity_deg(s_defaults.maxGranularity().value())
		, _mergeGeometry(s_defaults.mergeGeometry().value())
		, _clustering(s_defaults.clustering().value())
		, _instancing(s_defaults.instancing().value())
		, _ignoreAlt(s_defaults.ignoreAltitudeSymbol().value())
		, _useVertexBufferObjects(s_defaults.useVertexBufferObjects().value())
		, _shaderPolicy(s_defaults.shaderPolicy().value())
		, _geoInterp(s_defaults.geoInterp().value())
		, _optimizeStateSharing(s_defaults.optimizeStateSharing().value())
		, _optimize(s_defaults.optimize().value())
		, _validate(s_defaults.validate().value())
		, _maxPolyTilingAngle(s_defaults.maxPolygonTilingAngle().value())
	{
		fromConfig(_conf);
	}

	void SolidGeometryCompilerOptions::fromConfig(const Config& conf)
	{
		conf.getIfSet("max_granularity", _maxGranularity_deg);
		conf.getIfSet("merge_geometry", _mergeGeometry);
		conf.getIfSet("clustering", _clustering);
		conf.getIfSet("instancing", _instancing);
		conf.getObjIfSet("feature_name", _featureNameExpr);
		conf.getIfSet("ignore_altitude", _ignoreAlt);
		conf.getIfSet("geo_interpolation", "great_circle", _geoInterp, GEOINTERP_GREAT_CIRCLE);
		conf.getIfSet("geo_interpolation", "rhumb_line", _geoInterp, GEOINTERP_RHUMB_LINE);
		conf.getIfSet("use_vbo", _useVertexBufferObjects);
		conf.getIfSet("optimize_state_sharing", _optimizeStateSharing);
		conf.getIfSet("optimize", _optimize);
		conf.getIfSet("validate", _validate);
		conf.getIfSet("max_polygon_tiling_angle", _maxPolyTilingAngle);

		conf.getIfSet("shader_policy", "disable", _shaderPolicy, SHADERPOLICY_DISABLE);
		conf.getIfSet("shader_policy", "inherit", _shaderPolicy, SHADERPOLICY_INHERIT);
		conf.getIfSet("shader_policy", "generate", _shaderPolicy, SHADERPOLICY_GENERATE);
	}

	Config SolidGeometryCompilerOptions::getConfig() const
	{
		Config conf = ConfigOptions::getConfig();
		conf.addIfSet("max_granularity", _maxGranularity_deg);
		conf.addIfSet("merge_geometry", _mergeGeometry);
		conf.addIfSet("clustering", _clustering);
		conf.addIfSet("instancing", _instancing);
		conf.addObjIfSet("feature_name", _featureNameExpr);
		conf.addIfSet("ignore_altitude", _ignoreAlt);
		conf.addIfSet("geo_interpolation", "great_circle", _geoInterp, GEOINTERP_GREAT_CIRCLE);
		conf.addIfSet("geo_interpolation", "rhumb_line", _geoInterp, GEOINTERP_RHUMB_LINE);
		conf.addIfSet("use_vbo", _useVertexBufferObjects);
		conf.addIfSet("optimize_state_sharing", _optimizeStateSharing);
		conf.addIfSet("optimize", _optimize);
		conf.addIfSet("validate", _validate);
		conf.addIfSet("max_polygon_tiling_angle", _maxPolyTilingAngle);

		conf.addIfSet("shader_policy", "disable", _shaderPolicy, SHADERPOLICY_DISABLE);
		conf.addIfSet("shader_policy", "inherit", _shaderPolicy, SHADERPOLICY_INHERIT);
		conf.addIfSet("shader_policy", "generate", _shaderPolicy, SHADERPOLICY_GENERATE);
		return conf;
	}

	void SolidGeometryCompilerOptions::mergeConfig(const Config& conf)
	{
		ConfigOptions::mergeConfig(conf);
		fromConfig(conf);
	}

	//-----------------------------------------------------------------------
	SolidGeometryCompiler::SolidGeometryCompiler()
	{ }

	SolidGeometryCompiler::SolidGeometryCompiler(const SolidGeometryCompilerOptions& options)
		: options_(options)
	{ }

	SolidGeometryCompiler::~SolidGeometryCompiler(void) { }

	osg::Node* SolidGeometryCompiler::compile(Geometry* geometry, const Style& style, const FilterContext& context)
	{
		osg::ref_ptr<Feature> f = new Feature(geometry, 0L); // no SRS!
		return compile(f.get(), style, context);
	}

	osg::Node* SolidGeometryCompiler::compile(Geometry* geometry, const Style& style)
	{
		osg::ref_ptr<Feature> f = new Feature(geometry, 0L); // no SRS!
		return compile(f.get(), style, FilterContext(0L));
	}

	osg::Node* SolidGeometryCompiler::compile(Geometry* geometry, const FilterContext&  context)
	{
		return compile(geometry, Style(), context);
	}

	osg::Node* SolidGeometryCompiler::compile(Feature* feature, const Style& style, const FilterContext& context)
	{
		FeatureList workingSet;
		workingSet.push_back(feature);
		return compile(workingSet, style, context);
	}

	osg::Node* SolidGeometryCompiler::compile(Feature* feature, const FilterContext& context)
	{
		return compile(feature, *feature->style(), context);
	}

	osg::Node* SolidGeometryCompiler::compile(FeatureCursor* cursor, const Style& style, const FilterContext& context)
	{
		// start by making a working copy of the feature set
		FeatureList workingSet;
		cursor->fill(workingSet);
		return compile(workingSet, style, context);
	}

	osg::Node* SolidGeometryCompiler::compile(FeatureList& workingSet, const Style& style, const FilterContext& context)
	{
#ifdef PROFILING
		osg::Timer_t p_start = osg::Timer::instance()->tick();
		unsigned p_features = workingSet.size();
#endif

		// for debugging/validation.
		std::vector<std::string> history;
		bool trackHistory = (options_.validate() == true);

		osg::ref_ptr<osg::Group> resultGroup = new osg::Group();

		// create a filter context that will track feature data through the process
		FilterContext sharedCX = context;

		if (!sharedCX.extent().isSet() && sharedCX.profile())
		{
			sharedCX.extent() = sharedCX.profile()->getExtent();
		}

		const SolidLineSymbol * solidLineSym = style.get<SolidLineSymbol>();
		//const gwUtil::SolidJointSymbol* solidJointSym = style<gwUtil::SolidJointSymbol>.get();
		if (solidLineSym /*|| solidJointSym*/)
		{
			BuildSolidGeometryFilter filter(style);
			osg::Group* node = filter.push(workingSet, sharedCX);
			if (node)
			{
				if (trackHistory) history.push_back("solid_geometry");
				resultGroup->addChild(node);
			}
		}
		return resultGroup.release();
	}
}
