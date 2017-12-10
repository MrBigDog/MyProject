#include "IGeologicalSystem.h"

namespace gwGeological
{
	class GeologicalSystem :public IGeologicalSystem
	{
	public:
		GeologicalSystem(gwApp::IApplication* app);
		~GeologicalSystem() {}

		virtual void init();
		virtual void shutdown();
		virtual void update(double dt);

		virtual void initSystemFromConfig(const osgEarth::Config& systemConfig);

		virtual void addLayer(const osgEarth::Config& layerconfig);
		virtual void addLayer(IGeologicalLayer* layer);
		virtual void removeLayer(IGeologicalLayer* layer);
		virtual void removeLayer(const std::string& layername);
		virtual void removeAllLayers();

		virtual IGeologicalLayer* getLayer(const std::string& layer_name);
		virtual void getLayers(IGeologicalLayerVector& out_layers);

		virtual void queryAnalysis();

		virtual osg::Node* getLayerNode(const std::string& layername) const;

		virtual void fenceCuttingAnalysis();
		virtual void signalCuttingAnalysis();
		virtual void continueCuttingAnalysis();
		virtual void tunnelAnalysis();

	private:
		osg::ref_ptr<osgEarth::Map>_map;
		osg::ref_ptr<osgEarth::MapNode> _mapNode;

	};
}