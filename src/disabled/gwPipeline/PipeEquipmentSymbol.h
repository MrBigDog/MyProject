#ifndef GWPIPELINE_PIPEEQUIPMENTSYMBOL_H
#define GWPIPELINE_PIPEEQUIPMENTSYMBOL_H 1

#include <osgEarth/Config>
#include <osgEarthSymbology/InstanceSymbol>

namespace osg
{
	class Node;
}

namespace gwPipeline
{
	class PipeEquipmentSymbol: public osgEarth::Symbology::InstanceSymbol
	{
	public:
		META_Object(PipeEquipmentSymbol, PipeEquipmentSymbol);

		PipeEquipmentSymbol(const PipeEquipmentSymbol& rhs, const osg::CopyOp& copyop=osg::CopyOp::SHALLOW_COPY);
		PipeEquipmentSymbol( const osgEarth::Config& conf = osgEarth::Config() );

		virtual ~PipeEquipmentSymbol(void);

	public:
		virtual osgEarth::Config getConfig() const;
		virtual void mergeConfig( const osgEarth::Config& conf );
		static void parseSLD(const osgEarth::Config& c, class osgEarth::Symbology::Style& style);

	public: // InstanceSymbol
		/** Creates a new (empty) resource appropriate for this symbol */
		virtual osgEarth::Symbology::InstanceResource* createResource() const;

		void setEquipmentNode(osg::Node* equipmentNode);
		osg::Node* getEquipmentNode();

		void setScaleVec(const osg::Vec3d& scleVec);
		osg::Vec3d getScaleVec();


	private:
		osg::ref_ptr<osg::Node> equipmentNode_;

		osg::Vec3d scaleVec_;
	};
}

#endif // GWPIPELINE_PIPEEQUIPMENTSYMBOL_H
