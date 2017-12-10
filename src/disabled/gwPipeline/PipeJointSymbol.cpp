#include <gwPipeline/PipeJointSymbol.h>
#include <osgEarthSymbology/ModelResource>
#include <osg/Texture2D>

namespace gwPipeline
{
	OSGEARTH_REGISTER_SIMPLE_SYMBOL(gwPipeJoint, PipeJointSymbol);

	PipeJointSymbol::PipeJointSymbol(const PipeJointSymbol& rhs, const osg::CopyOp& copyop)
		: osgEarth::Symbology::InstanceSymbol(rhs, copyop)
		, jointTexture_(rhs.jointTexture_)
		, segmentType_(rhs.segmentType_)
	{

	}

	PipeJointSymbol::PipeJointSymbol( const osgEarth::Config& conf)
		: osgEarth::Symbology::InstanceSymbol(conf)
	{
		mergeConfig(conf);
	}

	PipeJointSymbol::~PipeJointSymbol()
	{

	}

	osgEarth::Config PipeJointSymbol::getConfig() const
	{
		osgEarth::Config conf = osgEarth::Symbology::InstanceSymbol::getConfig();
		conf.key() = "gwPipeJoint";
		conf.addNonSerializable( "PipeJointSymbol::texture2D", jointTexture_.get() );
		return conf;
	}

	void PipeJointSymbol::mergeConfig( const osgEarth::Config& conf )
	{
		osgEarth::Symbology::InstanceSymbol::mergeConfig(conf);
		jointTexture_ = conf.getNonSerializable<osg::Texture2D>( "PipeJointSymbol::texture2D" );
	}

	void PipeJointSymbol::parseSLD(const osgEarth::Config& c, class osgEarth::Symbology::Style& style)
	{

	}

	osgEarth::Symbology::InstanceResource* PipeJointSymbol::createResource() const
	{
		return new osgEarth::Symbology::ModelResource();
	}

	void PipeJointSymbol::setJointTexture(osg::Texture2D* texture)
	{
		jointTexture_ = texture;
	}
}
