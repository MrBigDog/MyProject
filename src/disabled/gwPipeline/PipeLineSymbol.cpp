#include <gwPipeline/PipeLineSymbol.h>

#include <osgEarthSymbology/Symbol>
#include <osgEarthSymbology/ModelResource>

namespace gwPipeline
{
	OSGEARTH_REGISTER_SIMPLE_SYMBOL(mdPipeline, PipeLineSymbol);

	PipeLineSymbol::PipeLineSymbol(const PipeLineSymbol& rhs, const osg::CopyOp& copyop)
		: osgEarth::Symbology::InstanceSymbol(rhs, copyop)
		, unitPipe_(rhs.unitPipe_)
		, unitPipeTexture_(rhs.unitPipeTexture_)
		, segmentType_(rhs.segmentType_)
		, segmentNum_(rhs.segmentNum_)
		, resourceLib_(rhs.resourceLib_)
	{ }

	PipeLineSymbol::PipeLineSymbol( const osgEarth::Config& conf )
		: osgEarth::Symbology::InstanceSymbol(conf)
	{
		mergeConfig(conf);
	}

	PipeLineSymbol::~PipeLineSymbol(void)
	{ }

	osgEarth::Config PipeLineSymbol::getConfig() const
	{
		osgEarth::Config conf = osgEarth::Symbology::InstanceSymbol::getConfig();
		conf.key() = "mdPipeline";
		conf.addNonSerializable( "PipeLineSymbol::Node", unitPipe_.get() );
		return conf;
	}

	void PipeLineSymbol::mergeConfig( const osgEarth::Config& conf )
	{
		osgEarth::Symbology::InstanceSymbol::mergeConfig(conf);
		unitPipe_ = conf.getNonSerializable<osg::Node>( "PipeLineSymbol::Node" );
	}

	void PipeLineSymbol::parseSLD(const osgEarth::Config& c, class osgEarth::Symbology::Style& style)
	{

	}

	osgEarth::Symbology::InstanceResource* PipeLineSymbol::createResource() const
	{
		return new osgEarth::Symbology::ModelResource();
	}

	void PipeLineSymbol::setUnitPipe(osg::Node* unitPipe)
	{
		unitPipe_ = unitPipe;
	}

	osg::Node* PipeLineSymbol::getUnipPipe() const
	{
		return unitPipe_.get();
	}
}
