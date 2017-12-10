#ifndef GWEARTH_SOLIDLINESYMBOL_H
#define GWEARTH_SOLIDLINESYMBOL_H 1

#include <gwEarth/Export.h>
#include <osgEarthSymbology/Symbol>
#include <osgEarthSymbology/Tags>

namespace gwEarth
{
	using namespace osgEarth::Symbology;

	class GWEARTH_EXPORT SolidLineSymbol : public Symbol
	{
	public:
		//META_Object(SolidLineSymbol, SolidLineSymbol);

		SolidLineSymbol(const SolidLineSymbol& rhs, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY);
		SolidLineSymbol(const osgEarth::Config& conf = osgEarth::Config());
		virtual ~SolidLineSymbol();

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
