#ifndef GWPIPELINE_PIPELINESYMBOL_H
#define GWPIPELINE_PIPELINESYMBOL_H 1

#include <osgEarth/Config>
#include <osgEarthSymbology/ModelSymbol>
#include <osgEarthSymbology/Style>
#include <osgEarthSymbology/ResourceLibrary>

#include <osg/Node>
#include <osg/Texture2D>

namespace gwPipeline
{
	class PipeLineSymbol: public osgEarth::Symbology::Symbol
	{
	public:
		enum PIPELINE_SEGMENT_TYPE
		{
			CIRCLE,
			SQUAR
		};

	public:
		META_Object(PipeLineSymbol, PipeLineSymbol);

		PipeLineSymbol(const PipeLineSymbol& rhs, const osg::CopyOp& copyop=osg::CopyOp::SHALLOW_COPY);
		PipeLineSymbol( const osgEarth::Config& conf = osgEarth::Config() );

		virtual ~PipeLineSymbol(void);

	public:
		virtual osgEarth::Config getConfig() const;
		virtual void mergeConfig( const osgEarth::Config& conf );
		static void parseSLD(const osgEarth::Config& c, class osgEarth::Symbology::Style& style);

	public: // InstanceSymbol
		/** Creates a new (empty) resource appropriate for this symbol */
		virtual osgEarth::Symbology::InstanceResource* createResource() const;

		osgEarth::Symbology::SkinResource* createSkinResource() const;

		void setTextureUrl();
		void setTexture(osg::Texture2D* texture);
		osg::Texture2D* getTexture(){ return unitPipeTexture_.get();}

		void setUnitPipe(osg::Node* unitPipe);
		osg::Node* getUnipPipe() const;


	private:
		osg::ref_ptr<osg::Node> unitPipe_;
		osg::ref_ptr<osg::Texture2D> unitPipeTexture_;

		PIPELINE_SEGMENT_TYPE segmentType_;

		int segmentNum_;

		float radius_;
		float width_;
		float height_;

		osg::ref_ptr<osgEarth::Symbology::ResourceLibrary> resourceLib_;
	};
}

#endif // GWPIPELINE_PIPELINESYMBOL_H
