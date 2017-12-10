#include <gwPipeline/PipelineSystem.h>
#include <osgEarth/Registry>

namespace gwPipeline
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	PipelineSystem::PipelineSystem(gwApp::IApplication* app)
		: IPipelineSystem(app)
	{
		//_map = _mapNode->getMap();
		//_root = new osg::Group;
	}

	PipelineSystem::~PipelineSystem() { }

	// base system method
	void PipelineSystem::init() { }
	void PipelineSystem::shutdown() { }
	void PipelineSystem::update(double dt)
	{
		fireCallback(dt, &PipelineSystemCallback::onUpdate);
	}

	osg::Node* PipelineSystem::getLayerNode(const std::string& layername)
	{
		if (!_mapNode) return 0L;
		return _mapNode->getModelLayerNode(getLayer(layername));
	}

	void PipelineSystem::addLayer(IPipelineLayer * layer)
	{
		if (IModelSystem::addLayer(layer))
		{
			fireCallback(layer, &PipelineSystemCallback::onLayerAdded);
		}
	}

	void PipelineSystem::addLayer(const osgEarth::Config & layerconfig)
	{
	}

	void PipelineSystem::removeLayer(IPipelineLayer * layer)
	{
		if (IModelSystem::removeLayer(layer))
		{
			fireCallback(layer, &PipelineSystemCallback::onLayerRemoved);
		}
	}

	void PipelineSystem::removeLayer(const std::string &layername)
	{
		removeLayer(getLayer(layername));
	}

	void PipelineSystem::removeAllLayers()
	{
		IPipelineLayerVector layers;
		getLayers(layers);
		for (IPipelineLayerVector::const_iterator it = layers.begin(); it != layers.end(); ++it)
		{
			removeLayer(*it);
		}
	}

	IPipelineLayer* PipelineSystem::getLayer(const std::string& layername)
	{
		return IModelSystem::getLayer<IPipelineLayer>(layername);
	}

	void PipelineSystem::getLayers(IPipelineLayerVector& layers, PipelineLayer::PipeType type)
	{
		if (!_map) return;
		osgEarth::ModelLayerVector mlv;
		_map->getModelLayers(mlv);
		if (mlv.empty()) return;
		for (osgEarth::ModelLayerVector::iterator it = mlv.begin(); it != mlv.end(); ++it)
		{
			IPipelineLayer* layer = dynamic_cast<IPipelineLayer*>((*it).get());
			if (!layer) continue;
			if (type & layer->getPipeType())
			{
				layers.push_back(layer);
			}
		}
	}
}
