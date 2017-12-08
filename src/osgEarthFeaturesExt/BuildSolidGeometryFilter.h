#ifndef OEFEATURESEXT_BUILDSOLIDGEOMETRYFILTER_H
#define OEFEATURESEXT_BUILDSOLIDGEOMETRYFILTER_H 1

#include <osgEarthFeaturesExt/Export.h>
#include <osgEarthFeaturesExt/SolidLineSymbol.h>

#include <osgEarthFeatures/Common>
#include <osgEarthFeatures/Feature>
#include <osgEarthFeatures/Filter>
#include <osgEarthSymbology/Style>
#include <osgEarth/GeoMath>
#include <osgEarth/MapNode>
#include <osgEarth/ElevationQuery>
#include <osg/Geode>

using namespace osgEarth;
using namespace osgEarth::Features;
using namespace osgEarth::Symbology;

namespace oeFeaturesExt
{
	class OSGEARTHFEATRUESEXT_EXPORT BuildSolidGeometryFilter : public FeaturesToNodeFilter
	{
	public:
		BuildSolidGeometryFilter(const Style& style = Style());
		virtual ~BuildSolidGeometryFilter() { /*delete _eleQuery;*/ }

		static bool isSupported() { return true; }

		void setUnitPipe(osg::Node* unitPipt) { unitPipe_ = unitPipt; }

		/** Pushes a list of features through the filter. */
		osg::Group* push(FeatureList& input, FilterContext& context);

		/** The style to apply to feature geometry */
		const Style& getStyle() { return _style; }
		void setStyle(const Style& s) { _style = s; }

		/*** For geocentric data, sets the granularity of edges created by the filter. This
		* is the maximum angle (in degrees) between the vectors representing two geocentric
		* points comprising an edge. Since large polygons won't naturally conform to the
		* ellipsoid very well, this setting lets you specify a minimum edge size so that they
		* will break down and conform better. */
		optional<double>& maxGranularity() { return _maxAngle_deg; }
		const optional<double>& maxGranularity() const { return _maxAngle_deg; }

		/*** The algorithm to use when interpolating between geodetic locations.
		* The default is GEOINTERP_RHUMBLINE. */
		optional<GeoInterpolation>& geoInterp() { return _geoInterp; }
		const optional<GeoInterpolation>& geoInterp() const { return _geoInterp; }

		/*** Sets an expression to evaluate for setting the name of a Geometry.
		* Warning: this will disable some performance optimizations since the filter
		* can no longer merge geometries. */
		optional<StringExpression>& featureName() { return _featureNameExpr; }
		const optional<StringExpression>& featureName() const { return _featureNameExpr; }

		/*** When a geocentric polygon is very large, the filter has to tile it up in order to
		* properly tessellate it. This is the maximum size of each tile (in degrees). */
		optional<float>& maxPolygonTilingAngle() { return _maxPolyTilingAngle_deg; }
		const optional<float>& maxPolygonTilingAngle() const { return _maxPolyTilingAngle_deg; }

		//bool clampToTerrain(osgEarth::GeoPoint& inpoint, float offset);

	protected:
		Style                      _style;

		optional<double>           _maxAngle_deg;
		optional<GeoInterpolation> _geoInterp;
		optional<StringExpression> _featureNameExpr;
		optional<float>            _maxPolyTilingAngle_deg;

		osg::Group* processSolidLines(FeatureList& input, const SolidLineSymbol* symbol, FilterContext& cx);
		osg::Geode* processSolidJoints(FeatureList& input, const InstanceSymbol* symbol, FilterContext& cx);

		osg::Geode* processSolidLines(FeatureList& input, FilterContext& cx);
		osg::Geode* processSolidJoints(FeatureList& input, FilterContext& cx);

		bool process(const FeatureList& features, const InstanceSymbol* symbol, Session* session, osg::Group* attachPoint, FilterContext& context);

		bool findResource(const URI& uri, const InstanceSymbol* symbol, FilterContext& context, std::set<URI>& missing, osg::ref_ptr<InstanceResource>& output);

	private:
		osg::ref_ptr<ResourceLibrary> resourceLib_;


		typedef LRUCache<URI, osg::ref_ptr<InstanceResource> > InstanceCache;
		InstanceCache instanceCache_;

		osg::ref_ptr<osg::Node> unitPipe_;

		osgEarth::MapNode* _mapNode;
		osgEarth::ElevationQuery* _eleQuery;
	};
}

#endif // OEFEATURESEXT_BUILDSOLIDGEOMETRYFILTER_H

