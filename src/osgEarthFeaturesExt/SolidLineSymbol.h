#ifndef OEFEATURESEXT_SOLIDLINESYMBOL_H
#define OEFEATURESEXT_SOLIDLINESYMBOL_H 1

#include <osgEarthFeaturesExt/Export.h>
#include <osgEarthSymbology/Symbol>

namespace oeFeaturesExt
{
	using namespace osgEarth::Symbology;

	class OSGEARTHFEATRUESEXT_EXPORT SolidLineSymbol : public /*Instance*/Symbol
	{
	public:
		META_Object(gwUtil, SolidLineSymbol);

		SolidLineSymbol(const SolidLineSymbol& rhs, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY);
		SolidLineSymbol(const osgEarth::Config& conf = osgEarth::Config());
		virtual ~SolidLineSymbol() {}

		//osgEarth::optional<osg::ref_ptr<osg::Node> > model() { return _node; }
		//const osgEarth::optional<osg::ref_ptr<osg::Node> > model() const { return _node; }

		virtual osgEarth::Config getConfig() const;
		virtual void mergeConfig(const osgEarth::Config& conf);
		static void parseSLD(const osgEarth::Config& c, class osgEarth::Symbology::Style& style);

	public:
		void setUnidSolidLineModel(osg::Node* node) { _unidSolidLineModel = node; }
		osg::Node* getUnidSolidLineModel() const { return _unidSolidLineModel.get(); }

	private:
		osg::ref_ptr<osg::Node> _unidSolidLineModel;
		osgEarth::optional<std::string> _radiusPropertyName;
	};
}
#endif // SolidLineSymbol_h__
