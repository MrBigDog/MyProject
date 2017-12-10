#ifndef gwVectorSystem_h__
#define gwVectorSystem_h__

#include <mdCommonFeature/CommonFeatureLayer.h>
#include <mdEarth/SubSceneSystem.h>

#include <mdBase/export.h>
#include <mdBase/structure/SubsystemMgr.h>
#include <mdBase/props/props.h>

#include <osgEarthFeatures/FeatureModelSource>
#include <osgEarth/Map>

using namespace mdEarth;

namespace mdCommonFeature
{
// 	class CommonFeatureSystemCallback
// 	{
// 	public:
// 		CommonFeatureSystemCallback(){}
// 		~CommonFeatureSystemCallback(){}
// 
// 		virtual void onLayerAdded(){}
// 		virtual void onLayerRemoved(){}
// 		//virtual void onFeatureLayerMoved(){}
// 		virtual void onLayerVisibleChanged(){}
// 
// 	};
// 
// 	typedef std::vector<CommonFeatureSystemCallback*> CommonFeatureSystemCallbackVector;

	class MD_EXPORT CommonFeatureSystem: public SubSceneSystem/*mdBase::Subsystem*/
	{
	public:
		CommonFeatureSystem(osgEarth::MapNode* mapnode);
		~CommonFeatureSystem();

		// base system method
		virtual void init();
		virtual void shutdown();
		virtual void update(double dt);

		//base SubSceneSystem method
		//virtual void addLayer(const std::string& layername, const std::string& url);
		//virtual void addLayer(const std::string& layername, const osgEarth::Features::FeatureModelSourceOptions& layeroption);
		virtual void addLayer(const MDPropertyNode& layerPropertyNode);

		virtual void removeLayer(const std::string& layername);

		virtual void setLayerVisible(const std::string& layername, bool isVisible);
		virtual bool getLayerVisible(const std::string& layername);

		virtual void setLayerOpcity(const std::string& layername, const float opcity);
		virtual float getLayerOpcity(const std::string& layername);

		virtual void setLayerLightEnabled(const std::string& layername, bool isLightEnabled);
		virtual bool getLayerLightEnabled(const std::string& layername);

		virtual void addLayerCallback(const std::string& layername, osgEarth::ModelLayerCallback* mlc);
		virtual void removeLayerCallback(const std::string& layername, osgEarth::ModelLayerCallback* mlc);

		virtual osg::Node* getLayerNode(const std::string& layername);


		//new method
	private:
		void addLayer(const std::string& layername, const osgEarth::Features::FeatureModelSourceOptions& layeroption);
		void addLayer(CommonFeatureLayer* layer);
		void removeLayer(CommonFeatureLayer* layer);

		CommonFeatureLayer* getLayer(const std::string& layername);
		void getAllLayer(CommonFeatureLayerVector& layers);

		void setLayerVisible(CommonFeatureLayer* layer, bool isVisible);

		void addLayerCallback(CommonFeatureLayer* layer, osgEarth::ModelLayerCallback* cb);
		void removeLayerCallback(CommonFeatureLayer* layer, osgEarth::ModelLayerCallback* cb);

	private:
	};
}


#endif // gwVectorSystem_h__
