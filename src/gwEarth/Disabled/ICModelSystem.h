#ifndef GWEARTH_ICMODELSYSTEM_H
#define GWEARTH_ICMODELSYSTEM_H 1

#include <gwEarth/IModelSystem.h>
#include <gwEarth/ICModelLayer.h>

namespace gwEarth
{
	class ICModelSystemCallback :public IModelSystemCallback<ICModelLayer>
	{
	public:
		ICModelSystemCallback() {}
		virtual ~ICModelSystemCallback() {}
	};
	typedef std::vector<osg::ref_ptr<ICModelSystemCallback> > ICModelSystemCallbackVector;
	typedef void (ICModelSystemCallback::*CmodelSystemCallbackMethodLayerPtr)(ICModelLayer* layer);
	typedef void (ICModelSystemCallback::*CmodelSystemCallbackMethodUpdatePtr)(double dt);


	///
	class ICModelSystem :public IModelSystem
	{
	public:
		ICModelSystem(gwApp::IApplication* app) :IModelSystem(app) { }
		virtual ~ICModelSystem() {}

		virtual void addLayer(ICModelLayer* layer) = 0;
		virtual void addLayer(const osgEarth::Config& layerconfig) = 0;
		virtual void removeLayer(ICModelLayer* layer) = 0;
		virtual void removeAllLayers() = 0;
		virtual void getLayers(ICModelLayerVector& layers) const = 0;
		virtual ICModelLayer* getLayer(const std::string& layerName) const = 0;

		virtual osg::Node* getLayerNode(const std::string& layername) const = 0;

		META_SYSTEM_CALLBACK(ICModelSystemCallback)

	protected:
		virtual void fireCallback(ICModelLayer*layer, CmodelSystemCallbackMethodLayerPtr method)
		{
			for (ICModelSystemCallbackVector::const_iterator it = _callbacks.begin(); it != _callbacks.end(); ++it)
			{
				ICModelSystemCallback*cb = it->get();
				(cb->*method)(layer);
			}
		}

		virtual void fireCallback(double dt, CmodelSystemCallbackMethodUpdatePtr method)
		{
			for (ICModelSystemCallbackVector::const_iterator it = _callbacks.begin(); it != _callbacks.end(); ++it)
			{
				ICModelSystemCallback*cb = it->get();
				(cb->*method)(dt);
			}
		}

	protected:
		ICModelSystemCallbackVector _callbacks;
	};
}


#endif // ICModelSystem_h__
