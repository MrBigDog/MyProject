#ifndef GWPIPELINE_PIPELINEGEOMETRYCOMPILER_H
#define GWPIPELINE_PIPELINEGEOMETRYCOMPILER_H 1

#include <osgEarth/Config>
#include <osgEarthFeatures/Common>
#include <osgEarthFeatures/Feature>
#include <osgEarthFeatures/FeatureCursor>
#include <osgEarthFeatures/ResampleFilter>
#include <osgEarthSymbology/Style>
#include <osgEarth/GeoMath>

using namespace osgEarth;
using namespace osgEarth::Features;
using namespace osgEarth::Symbology;

namespace gwPipeline
{
	class PipelineGeometryCompilerOptions: public osgEarth::ConfigOptions
	{
	public:
		/* Set the global default values for the options. */
		static void setDefaults(const PipelineGeometryCompilerOptions& defaults);

	public:
		PipelineGeometryCompilerOptions(const ConfigOptions& conf =ConfigOptions());
		virtual ~PipelineGeometryCompilerOptions() { }

	public:
		/** Maximum span of a generated edge, in degrees. Applicable to geocentric maps only */
		osgEarth::optional<double>& maxGranularity() { return _maxGranularity_deg; }
		const osgEarth::optional<double>& maxGranularity() const { return _maxGranularity_deg; }

		/** Interpolation type to use for geodetic points */
		osgEarth::optional<osgEarth::GeoInterpolation>& geoInterp() { return _geoInterp; }
		const osgEarth::optional<osgEarth::GeoInterpolation>& geoInterp() const { return _geoInterp; }

		/** Whether to merge geometry from multiple features */
		osgEarth::optional<bool>& mergeGeometry() { return _mergeGeometry; }
		const osgEarth::optional<bool>& mergeGeometry() const { return _mergeGeometry; }

		/** Expression to evaluate to extract a feature's readable name */
		osgEarth::optional<osgEarth::StringExpression>& featureName() { return _featureNameExpr; }
		const osgEarth::optional<osgEarth::StringExpression>& featureName() const { return _featureNameExpr; }

		/** Whether to cluster feature geometries together for speed */
		osgEarth::optional<bool>& clustering() { return _clustering; }
		const osgEarth::optional<bool>& clustering() const { return _clustering; }

		/** Whether to enabled draw-instancing for model substitution */
		osgEarth::optional<bool>& instancing() { return _instancing; }
		const osgEarth::optional<bool>& instancing() const { return _instancing; }

		/** Whether to ignore the altitude filter (e.g. if you plan to do auto-clamping layer) */
		osgEarth::optional<bool>& ignoreAltitudeSymbol() { return _ignoreAlt; }
		const osgEarth::optional<bool>& ignoreAltitudeSymbol() const { return _ignoreAlt; }

		//todo: merge this with geoInterp()
		osgEarth::optional<osgEarth::Features::ResampleFilter::ResampleMode>& resampleMode() { return _resampleMode;}
		const osgEarth::optional<osgEarth::Features::ResampleFilter::ResampleMode>& resampleMode() const { return _resampleMode;}

		osgEarth::optional<double>& resampleMaxLength() { return _resampleMaxLength; }
		const osgEarth::optional<double>& resampleMaxLength() const { return _resampleMaxLength;}

		/** @deprecated Whether to use VBOs in geometry */
		osgEarth::optional<bool>& useVertexBufferObjects() { return _useVertexBufferObjects;}
		const osgEarth::optional<bool>& useVertexBufferObjects() const { return _useVertexBufferObjects;}

		/** Whether to generate shader components on compiled geometry */
		osgEarth::optional<osgEarth::ShaderPolicy>& shaderPolicy() { return _shaderPolicy; }
		const osgEarth::optional<osgEarth::ShaderPolicy>& shaderPolicy() const { return _shaderPolicy; }

		/** Whether to run consolidate equivalent state attributes for better performance. */
		osgEarth::optional<bool>& optimizeStateSharing() { return _optimizeStateSharing; }
		const osgEarth::optional<bool>& optimizeStateSharing() const { return _optimizeStateSharing; }

		/** Whether to run the optimizer on the resulting group. */
		osgEarth::optional<bool>& optimize() { return _optimize; }
		const osgEarth::optional<bool>& optimize() const { return _optimize; }

		/** Whether to run a geometry validation pass on teh resulting group. This is for debugging
		purposes and will dump issues to the console. */
		osgEarth::optional<bool>& validate() { return _validate; }
		const osgEarth::optional<bool>& validate() const { return _validate; }

		/** Maximum size (angle, degrees) of a polygon tile, when breaking up a large polygon for tessellation;
		only applies to geocentric maps - detault = 5.0 */
		osgEarth::optional<float>& maxPolygonTilingAngle() { return _maxPolyTilingAngle; }
		const osgEarth::optional<float>& maxPolygonTilingAngle() const { return _maxPolyTilingAngle; }

	public:
		osgEarth::Config getConfig() const;
		void mergeConfig( const osgEarth::Config& conf );

	private:
		osgEarth::optional<double>											_maxGranularity_deg;
		osgEarth::optional<osgEarth::GeoInterpolation>						_geoInterp;
		osgEarth::optional<bool>											_mergeGeometry;
		osgEarth::optional<osgEarth::StringExpression>						_featureNameExpr;
		osgEarth::optional<bool>											_clustering;
		osgEarth::optional<bool>											_instancing;
		osgEarth::optional<osgEarth::Features::ResampleFilter::ResampleMode>_resampleMode;
		osgEarth::optional<double>											_resampleMaxLength;
		osgEarth::optional<bool>											_ignoreAlt;
		osgEarth::optional<bool>											_useVertexBufferObjects;
		osgEarth::optional<osgEarth::ShaderPolicy>							_shaderPolicy;
		osgEarth::optional<bool>											_optimizeStateSharing;
		osgEarth::optional<bool>											_optimize;
		osgEarth::optional<bool>											_validate;
		osgEarth::optional<float>											_maxPolyTilingAngle;

		void fromConfig( const osgEarth::Config& conf );

		static PipelineGeometryCompilerOptions s_defaults;

	public:
		PipelineGeometryCompilerOptions(bool); // internal
	};


	class PipelineGeometryCompiler
	{
	protected:
		PipelineGeometryCompilerOptions options_;

	public:
		PipelineGeometryCompiler(void);
		PipelineGeometryCompiler(const PipelineGeometryCompilerOptions& options);

		~PipelineGeometryCompiler(void);

		//Ö»¶Á;
		const PipelineGeometryCompilerOptions& options() const 
		{
			return options_; 
		}

		//¿É±à¼­;
		PipelineGeometryCompilerOptions& options() 
		{
			return options_;
		}

	public:
		/** Compiles a collection of features into an OSG scene graph. */
		osg::Node* compile(FeatureCursor* input, const Style& style, const FilterContext& context);
		osg::Node* compile(Feature		* input, const Style& style, const FilterContext& context);
		osg::Node* compile(Feature		* input, const FilterContext& context);
		osg::Node* compile(Geometry* geom,  const Style& style,const FilterContext& context);
		osg::Node* compile(Geometry* geom,  const Style& style);
		osg::Node* compile(Geometry* geom,  const FilterContext&  context);
		osg::Node* compile(FeatureList& mungeableInput, const Style& style, const FilterContext& context);
	};
}

#endif // GWPIPELINE_PIPELINEGEOMETRYCOMPILER_H
