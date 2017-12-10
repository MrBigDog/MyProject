/************************************************************************/
/*                    地下地上管线的绘制;                                 */
/************************************************************************/
#ifndef GWPIPELINE_PIPELINESYSTEM_H
#define GWPIPELINE_PIPELINESYSTEM_H 1

#include <gwPipeline/IPipelineSystem.h>
#include <gwPipeline/PipelineLayer.h>
#include <osgEarth/Map>
#include <osgEarth/MapNode>
#include <osgEarth/JsonUtils>
#include <osg/Group>

namespace gwPipeline
{
	class PipelineSystem : public IPipelineSystem
	{
	public:
		PipelineSystem(gwApp::IApplication* app);
		~PipelineSystem();

		// base system method.
		void init();
		void shutdown();
		void update(double dt);

		//osg::Group* getRootNode() { return _root.get(); }
		osg::Node* getLayerNode(const std::string& layername);

		virtual void addLayer(IPipelineLayer* layer);
		virtual void addLayer(const osgEarth::Config& layerconfig);
		virtual void removeLayer(IPipelineLayer* layer);
		virtual void removeLayer(const std::string &layername);
		virtual void removeAllLayers();
		virtual void getLayers(IPipelineLayerVector& layers, PipelineLayer::PipeType type = PipelineLayer::ALL_TYPE);
		IPipelineLayer* getLayer(const std::string& layername);

		virtual void flowAnalysis() {}
		virtual void burstAnalysis() {}
		virtual void connectivityAnalysis() {}

	private:
		//osg::ref_ptr<osg::Group> _root;
		//osgEarth::MapNode* _mapNode;
		//osgEarth::Map* _map;

		typedef std::map<std::string, osg::ref_ptr<IPipelineLayer> >IPipelineLayerMap;
		IPipelineLayerMap _layerMap;
	};
}

#endif // GWPIPELINE_PIPELINESYSTEM_H
