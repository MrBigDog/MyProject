#ifndef GWGEOLOGICAL_IGEOLOGICALSYSTEM_H
#define GWGEOLOGICAL_IGEOLOGICALSYSTEM_H 1

#include <gwGeological/GeologicalLayer.h>
#include <gwEarth/IModelSystem.h>
#include <gwApp/IApplication.h>

using namespace gwEarth;

namespace gwGeological
{
	class GeologicalSystemCallback : public IModelSystemCallback<IGeologicalLayer>
	{
	public:
		GeologicalSystemCallback() {}
		virtual ~GeologicalSystemCallback() {}
	};
	typedef std::vector<osg::ref_ptr<GeologicalSystemCallback> > GeologicalSystemCallbackVector;
	typedef void (GeologicalSystemCallback::*GeologicalSystemCallbackMethodPtr)(IGeologicalLayer* layer);

	///
	class IGeologicalSystem :public gwEarth::IModelSystem
	{
	public:
		IGeologicalSystem(gwApp::IApplication* app)
			:gwEarth::IModelSystem(app)
		{}

		virtual ~IGeologicalSystem() = 0;

		virtual void addLayer(IGeologicalLayer* layer) = 0;
		virtual void removeLayer(IGeologicalLayer* layer) = 0;
		virtual void removeAllLayers() = 0;

		virtual IGeologicalLayer* getLayer(const std::string& layer_name) = 0;
		virtual void getLayers(IGeologicalLayerVector& out_layers) = 0;

		virtual void fenceCuttingAnalysis() = 0;
		virtual void signalCuttingAnalysis() = 0;
		virtual void continueCuttingAnalysis() = 0;
		virtual void tunnelAnalysis() = 0;

		META_SYSTEM_CALLBACK(GeologicalSystemCallback)

	protected:
		void fireCallback(IGeologicalLayer*layer, GeologicalSystemCallbackMethodPtr method)
		{
			for (GeologicalSystemCallbackVector::const_iterator it = _callbacks.begin(); it != _callbacks.end(); ++it)
			{
				GeologicalSystemCallback* cb = it->get();
				(cb->*method)(layer);
			}
		}

	protected:
		GeologicalSystemCallbackVector _callbacks;
	};
}

#endif // GWAPP_IGEOLOGICALSYSTEM_H
