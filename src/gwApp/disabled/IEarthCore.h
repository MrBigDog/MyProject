#ifndef  INTERFACE_EARTH_CORE
#define  INTERFACE_EARTH_CORE


	enum NodeMasks
	{
		TERRAIN = 0x1 << 0,
		ICONS = 0x1 << 1,
		TEXT_LABELS = 0x1 << 2,
		MODELS = 0x1 << 3
	};

	namespace mdEarth {
		class  ShaderAndObjectIndexGen;
		class  ModelSourceIndex;
		class  Render;
	}

	class IEarthCore 
	{
	public:
		virtual ~IEarthCore(){};

		virtual mdEarth::ShaderAndObjectIndexGen* shaderAndObjectindexGen() = 0;
		virtual mdEarth::ModelSourceIndex* gwModelSourceIndex() = 0;

		virtual mdEarth::Render* getRender() = 0;

	};



#endif //INTERFACE_EARTH_CORE
