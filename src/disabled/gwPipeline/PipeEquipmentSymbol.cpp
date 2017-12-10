#include <gwPipeline/PipeEquipmentSymbol.h>

#include <osgEarthSymbology/ModelResource>
#include <osg/Node>

namespace gwPipeline
{
	OSGEARTH_REGISTER_SIMPLE_SYMBOL(gwPipeEquipment, PipeEquipmentSymbol);

	PipeEquipmentSymbol::PipeEquipmentSymbol(const PipeEquipmentSymbol& rhs, const osg::CopyOp& copyop)
		: osgEarth::Symbology::InstanceSymbol(rhs, copyop)
		, equipmentNode_(rhs.equipmentNode_)
		, scaleVec_(rhs.scaleVec_)
	{ }

	PipeEquipmentSymbol::PipeEquipmentSymbol( const osgEarth::Config& conf)
	{
		mergeConfig(conf);
	}

	PipeEquipmentSymbol::~PipeEquipmentSymbol(void)
	{ }

	void PipeEquipmentSymbol::setEquipmentNode(osg::Node* equipmentNode)
	{
		equipmentNode_ = equipmentNode;
	}

	osg::Node* PipeEquipmentSymbol::getEquipmentNode()
	{
		return equipmentNode_.get();
	}


	osgEarth::Config PipeEquipmentSymbol::getConfig() const
	{
		osgEarth::Config conf = osgEarth::Symbology::InstanceSymbol::getConfig();
		conf.key() = "gwPipeEquipment";
		conf.addNonSerializable( "gwPipeEquipmentSymbol::node", equipmentNode_.get() );
		return conf;
	}

	void PipeEquipmentSymbol::mergeConfig( const osgEarth::Config& conf )
	{
		osgEarth::Symbology::InstanceSymbol::mergeConfig(conf);
		equipmentNode_ = conf.getNonSerializable<osg::Node>( "gwPipeEquipmentSymbol::node" );
	}

	void PipeEquipmentSymbol::parseSLD(const osgEarth::Config& c, class osgEarth::Symbology::Style& style)
	{ }

	osgEarth::Symbology::InstanceResource* PipeEquipmentSymbol::createResource() const
	{
		return new osgEarth::Symbology::ModelResource();
	}
}
