#ifndef GWPIPELINE_IPIPELINESYSTEM_H
#define GWPIPELINE_IPIPELINESYSTEM_H 1

#include <gwPipeline/IPipelineLayer.h>
#include <gwEarth/IModelSystem.h>
#include <gwApp/IApplication.h>

using namespace gwEarth;

namespace gwPipeline
{
	class PipelineSystemCallback :public IModelSystemCallback<IPipelineLayer>
	{
	public:
		PipelineSystemCallback() {}
		virtual ~PipelineSystemCallback() {}
	};
	typedef std::vector<osg::ref_ptr<PipelineSystemCallback> > PipelineSystemCallbackVector;
	typedef void (PipelineSystemCallback::*PipelineSystemCallbackMethodLayerPtr)(IPipelineLayer* layer);
	typedef void (PipelineSystemCallback::*PipelineSystemCallbackMethodUpdatePtr)(double dt);


	///
	class IPipelineSystem :public gwEarth::IModelSystem
	{
	public:
		IPipelineSystem(gwApp::IApplication* app) :gwEarth::IModelSystem(app)
		{}

		virtual ~IPipelineSystem() {}

		virtual void flowAnalysis() = 0;
		virtual void burstAnalysis() = 0;
		virtual void connectivityAnalysis() = 0;

		virtual void addLayer(IPipelineLayer* layer) = 0;
		virtual void removeLayer(IPipelineLayer* layer) = 0;
		virtual void removeAllLayers() = 0;

		virtual IPipelineLayer* getLayer(const std::string& layername) = 0;
		virtual void getLayers(IPipelineLayerVector& out_layers) = 0;

		META_SYSTEM_CALLBACK(PipelineSystemCallback)

	protected:
		virtual void fireCallback(IPipelineLayer*layer, PipelineSystemCallbackMethodLayerPtr method)
		{
			for (PipelineSystemCallbackVector::const_iterator it = _callbacks.begin(); it != _callbacks.end(); ++it)
			{
				PipelineSystemCallback*cb = it->get();
				(cb->*method)(layer);
			}
		}

		virtual void fireCallback(double dt, PipelineSystemCallbackMethodUpdatePtr method)
		{
			for (PipelineSystemCallbackVector::const_iterator it = _callbacks.begin(); it != _callbacks.end(); ++it)
			{
				PipelineSystemCallback*cb = it->get();
				(cb->*method)(dt);
			}
		}

	protected:
		PipelineSystemCallbackVector _callbacks;
	};
}

#endif // GWAPP_IPIPELINESYSTEM_H
