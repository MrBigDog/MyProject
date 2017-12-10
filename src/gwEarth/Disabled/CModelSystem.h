#ifndef GWEARTH_COMMONMODELSYSTEM_H
#define GWEARTH_COMMONMODELSYSTEM_H 1

#define COMMON_MODEL_SYSTEM_ 1

#include <gwEarth/ICModelSystem.h>
#include <osgEarth/MapNode>

namespace gwEarth
{
	class ICModelLayer;

	class CModelSystem : public ICModelSystem
	{
	public:
		CModelSystem(gwApp::IApplication* app);
		~CModelSystem();

		// base system method
		virtual void init();
		virtual void shutdown();
		virtual void update(double dt);

		virtual void initSystemFromConfig(const osgEarth::Config& systemConfig);

		virtual void addLayer(ICModelLayer* layer);
		virtual void addLayer(const osgEarth::Config& layerconfig);
		virtual void removeLayer(ICModelLayer* layer);
		virtual void removeLayer(const std::string& layername);
		virtual void removeAllLayers();
		virtual void getLayers(ICModelLayerVector& layers)  const;
		virtual ICModelLayer* getLayer(const std::string& layerName)  const;

		virtual void queryAnalysis();

		virtual osg::Node* getLayerNode(const std::string& layername) const;

	//private:
	//	void addLayerMethod(const osgEarth::Config& arg);
	//	void removeLayerMethod(const osgEarth::Config&arg);
	};
}

#endif // GWEARTH_COMMONMODELSYSTEM_H
