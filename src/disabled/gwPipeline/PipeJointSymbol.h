#ifndef GWPIPELINE_PIPEJOINTSYMBOL_H
#define GWPIPELINE_PIPEJOINTSYMBOL_H 1

#include <osgEarth/Config>
#include <osgEarthSymbology/InstanceSymbol>

namespace osg
{
	class Texture2D;
}

namespace gwPipeline
{
	class PipeJointSymbol: public osgEarth::Symbology::InstanceSymbol
	{
	public:
		enum JOINT_SEGMENT_TYPE
		{
			CIRCLE,
			SQUARE
		};

	public:
		META_Object(PipeJointSymbol, PipeJointSymbol);

		PipeJointSymbol(const PipeJointSymbol& rhs, const osg::CopyOp& copyop=osg::CopyOp::SHALLOW_COPY);
		PipeJointSymbol( const osgEarth::Config& conf = osgEarth::Config() );

		virtual ~PipeJointSymbol(void);

	public:
		virtual osgEarth::Config getConfig() const;
		virtual void mergeConfig( const osgEarth::Config& conf );

		static void parseSLD(const osgEarth::Config& c, class osgEarth::Symbology::Style& style);

	public: // InstanceSymbol
		/** Creates a new (empty) resource appropriate for this symbol */
		virtual osgEarth::Symbology::InstanceResource* createResource() const;

		void setJointTexture(osg::Texture2D* texture);
		osg::Texture2D* getTexture(){return jointTexture_.get();}

	private:
		osg::ref_ptr<osg::Texture2D> jointTexture_;

		JOINT_SEGMENT_TYPE segmentType_;

		bool isGenerateTooth_;
	};
}

#endif // GWPIPELINE_PIPEJOINTSYMBOL_H
