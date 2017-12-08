#include <osgEarthSymbologyExt/SolidLineSymbol.h>

namespace oeSymbologyExt
{
	OSGEARTH_REGISTER_SIMPLE_SYMBOL(solid_line, SolidLineSymbol);

	SolidLineSymbol::SolidLineSymbol(const SolidLineSymbol & rhs, const osg::CopyOp & copyop)
		:Symbol(rhs, copyop)
	{
	}

	SolidLineSymbol::SolidLineSymbol(const osgEarth::Config & conf)
		: Symbol(conf)
	{
	}

	SolidLineSymbol::~SolidLineSymbol() {}

	osgEarth::Config SolidLineSymbol::getConfig() const
	{
		osgEarth::Config conf = osgEarth::Symbology::Symbol::getConfig();
		conf.key() = "solid_line";
		//conf.addNonSerializable("PipeLineSymbol::node", unitPipe_.get());
		return conf;
	}

	void SolidLineSymbol::mergeConfig(const osgEarth::Config & conf)
	{
		osgEarth::Symbology::Symbol::mergeConfig(conf);
		//unitPipe_ = conf.getNonSerializable<osg::Node>("PipeLineSymbol::node");
	}

	void SolidLineSymbol::parseSLD(const osgEarth::Config & c, osgEarth::Symbology::Style & style)
	{
	}
}