#include <mdEarth/SystemTypeDef.h>
#include <mdEarth/CommandDef.h>
#include <mdCommonFeature/CommonFeatureSystem.h>
#include <mdEarth/SceneSystem.h>
#include <mdEarth/Application.h>
#include <mdEarth/tileSourceUtil.h>

using namespace mdEarth;

namespace mdCommonFeature
{
	class AddCommonFeatureLayerOperation : public osg::Operation
	{
	public:
		AddCommonFeatureLayerOperation(const MDPropertyNode* arg)
			: arg_(arg)
		{ }
		virtual void operator()(osg::Object* pObject)
		{
			SceneSystem* scene = dynamic_cast<SceneSystem*>(Application::instance()->getSubsystem(SCENE_SYSTEM));
			if (!scene) return;

			CommonFeatureSystem* cfsys = dynamic_cast<CommonFeatureSystem*>(scene->get_subsystem(COMMON_FEATURE_SYSTEM));
			if (!cfsys) return;

			cfsys->addLayer(*arg_);
// 			osgEarth::Features::FeatureModelSourceOptions opt;
// 			if (!SourceUtil::createVectorSourceOptions(arg_, opt))
// 			{
// 				return;
// 			}
// 			cfsys->addLayer(arg_->getStringValue("layer_name"), opt);
		}

	protected:
		virtual ~AddCommonFeatureLayerOperation() { }
	private:
		osg::ref_ptr<const MDPropertyNode> arg_;
	};

	class RemoveCommonFeatureLayerOperation : public osg::Operation
	{
	public:
		RemoveCommonFeatureLayerOperation(const MDPropertyNode* arg) :arg_(arg)
		{ }
		virtual void operator()(osg::Object* pObject)
		{
			SceneSystem* scene = dynamic_cast<SceneSystem*>(Application::instance()->getSubsystem(SCENE_SYSTEM));
			if (!scene) return;

			CommonFeatureSystem* cfsys = dynamic_cast<CommonFeatureSystem*>
				(scene->get_subsystem(COMMON_FEATURE_SYSTEM));
			if (!cfsys) return;

			std::string vectorlayername = arg_->getStringValue("layer_name");
			cfsys->removeLayer(vectorlayername);
		}

	protected:
		virtual ~RemoveCommonFeatureLayerOperation() { }
	private:
		osg::ref_ptr<const MDPropertyNode> arg_;
	};

	static bool addCommonFeatureLayerCommand(const MDPropertyNode* arg)
	{
		Render* render = Application::instance()->getRender();
		osgViewer::CompositeViewer* comViewer = render->getCompositeViewer();
		if (!comViewer)
		{
			return false;
		}
		//OpenThreads::ScopedWriteLock(render->getRWMutex());
		comViewer->addUpdateOperation(new AddCommonFeatureLayerOperation(arg));

		return true;
	}

	static bool removeCommonFeatureLayerCommand(const MDPropertyNode* arg)
	{
		Render* render = Application::instance()->getRender();
		osgViewer::CompositeViewer* comViewer = render->getCompositeViewer();
		if (!comViewer)
		{
			return false;
		}
		//OpenThreads::ScopedWriteLock(render->getRWMutex());
		comViewer->addUpdateOperation(new RemoveCommonFeatureLayerOperation(arg));

		return true;
	}

	static bool setCommonFeatureLayerVisibleCommand(const MDPropertyNode* arg)
	{
		SceneSystem* scene = dynamic_cast<SceneSystem*>(Application::instance()->getSubsystem(SCENE_SYSTEM));
		if (!scene) return false;

		CommonFeatureSystem* cfsys = dynamic_cast<CommonFeatureSystem*>(scene->get_subsystem(COMMON_FEATURE_SYSTEM));
		if (!cfsys) return false;

		std::string vectorlayername = arg->getStringValue("layer_name");
		bool isVisible = arg->getBoolValue("is_visible");
		cfsys->setLayerVisible(vectorlayername, isVisible);

		return true;
	}

	static bool setCommonFeatureLayerOpcityCommand(const MDPropertyNode* arg)
	{
		SceneSystem* scene = dynamic_cast<SceneSystem*>(Application::instance()->getSubsystem(SCENE_SYSTEM));
		if (!scene) return false;

		CommonFeatureSystem* cfsys = dynamic_cast<CommonFeatureSystem*>(scene->get_subsystem(COMMON_FEATURE_SYSTEM));
		if (!cfsys) return false;

		std::string vectorlayername = arg->getStringValue("layer_name");
		float opcity = arg->getFloatValue("opcity");
		cfsys->setLayerVisible(vectorlayername, opcity);

		return true;
	}


	CommonFeatureSystem::CommonFeatureSystem(osgEarth::MapNode* mapnode)
		: SubSceneSystem(mapnode)
	{

	}

	CommonFeatureSystem::~CommonFeatureSystem()
	{

	}

	void CommonFeatureSystem::init()
	{
		mdBase::CommandMgr* cmdMgr = Application::instance()->getCommands();
		if (cmdMgr)
		{
			
			cmdMgr->addCommand(ADD_COMMON_FEATURE_LAYER, addCommonFeatureLayerCommand);
			cmdMgr->addCommand(REMOVE_COMMON_FEATURE_LAYER, removeCommonFeatureLayerCommand);
			cmdMgr->addCommand(SET_COMMON_FEATURE_LAYER_VISIBLE, setCommonFeatureLayerVisibleCommand);
			cmdMgr->addCommand(SET_COMMON_FEATURE_LAYER_OPCITY, setCommonFeatureLayerOpcityCommand);
		}
	}
	void CommonFeatureSystem::shutdown()
	{
		mdBase::CommandMgr* cmdMgr = Application::instance()->getCommands();
		if (cmdMgr)
		{
			cmdMgr->removeCommand(ADD_COMMON_FEATURE_LAYER);
			cmdMgr->removeCommand(REMOVE_COMMON_FEATURE_LAYER);
			cmdMgr->removeCommand(SET_COMMON_FEATURE_LAYER_VISIBLE);
			cmdMgr->removeCommand(SET_COMMON_FEATURE_LAYER_OPCITY);
		}
	}

	void CommonFeatureSystem::update(double dt)
	{

	}

	void CommonFeatureSystem::addLayer(const std::string& layername, const osgEarth::Features::FeatureModelSourceOptions& layeroption)
	{
		if (!_sceneMap || _sceneMap == NULL) return;

		osg::ref_ptr<CommonFeatureLayer> layer = new CommonFeatureLayer(layername, layeroption);
		_sceneMap->beginUpdate();
		_sceneMap->addModelLayer(layer);
		_sceneMap->endUpdate();
	}

	void CommonFeatureSystem::addLayer(CommonFeatureLayer* layer)
	{
		if (!_sceneMap || _sceneMap == NULL || !layer || layer == NULL) return;

		_sceneMap->beginUpdate();
		_sceneMap->addModelLayer(layer);
		_sceneMap->endUpdate();
	}

	void CommonFeatureSystem::addLayer(const MDPropertyNode & layerPropertyNode)
	{
		if (!_sceneMap || _sceneMap == NULL) return;

		osgEarth::Features::FeatureModelSourceOptions opt;
		if (!SourceUtil::createVectorSourceOptions(&layerPropertyNode, opt))
		{
			return;
		}
		addLayer(layerPropertyNode.getStringValue("layer_name"), opt);
	}

	void CommonFeatureSystem::removeLayer(const std::string& layername)
	{
		if (!_sceneMap || _sceneMap == NULL) return;

		CommonFeatureLayer* layer = dynamic_cast<CommonFeatureLayer*>(_sceneMap->getModelLayerByName(layername));
		if (!layer) return;

		_sceneMap->beginUpdate();
		_sceneMap->removeModelLayer(layer);
		_sceneMap->endUpdate();
	}

	void CommonFeatureSystem::removeLayer(CommonFeatureLayer* layer)
	{
		if (!_sceneMap || _sceneMap == NULL || !layer || layer == NULL) return;

		_sceneMap->beginUpdate();
		_sceneMap->removeModelLayer(layer);
		_sceneMap->endUpdate();
	}

	CommonFeatureLayer* CommonFeatureSystem::getLayer(const std::string& layername)
	{
		if (!_sceneMap || _sceneMap == NULL) return NULL;

		CommonFeatureLayer* layer = dynamic_cast<CommonFeatureLayer*>(_sceneMap->getModelLayerByName(layername));
		if (!layer) return NULL;

		return layer;
	}

	void CommonFeatureSystem::getAllLayer(CommonFeatureLayerVector& layers)
	{
		if (!_sceneMap || _sceneMap == NULL)
		{
			return;
		}

		osgEarth::ModelLayerVector mlv;
		_sceneMap->getModelLayers(mlv);

		if (mlv.empty()) return;

		for (osgEarth::ModelLayerVector::iterator it = mlv.begin(); it != mlv.end(); ++it)
		{
			CommonFeatureLayer* veclayer = dynamic_cast<CommonFeatureLayer*>((*it).release());
			if (veclayer)
			{
				layers.push_back(veclayer);
			}
		}
	}

	void CommonFeatureSystem::setLayerVisible(const std::string& layername, bool isVisible)
	{
		if (!_sceneMap || _sceneMap == NULL) return;

		CommonFeatureLayer* layer = dynamic_cast<CommonFeatureLayer*>(_sceneMap->getModelLayerByName(layername));
		if (layer)
		{
			layer->setVisible(isVisible);
		}
	}

	bool CommonFeatureSystem::getLayerVisible(const std::string & layername)
	{
		if (!_sceneMap || _sceneMap == NULL) return false;

		CommonFeatureLayer* layer = dynamic_cast<CommonFeatureLayer*>(_sceneMap->getModelLayerByName(layername));
		if (layer)
		{
			return layer->getVisible();
		}
		return false;
	}

	void CommonFeatureSystem::setLayerOpcity(const std::string & layername, const float opcity)
	{
		if (!_sceneMap || _sceneMap == NULL) return;

		CommonFeatureLayer* layer = dynamic_cast<CommonFeatureLayer*>(_sceneMap->getModelLayerByName(layername));
		if (layer)
		{
			layer->setOpacity(opcity);
		}
	}

	float CommonFeatureSystem::getLayerOpcity(const std::string & layername)
	{
		if (!_sceneMap || _sceneMap == NULL) return -1.0;

		CommonFeatureLayer* layer = dynamic_cast<CommonFeatureLayer*>(_sceneMap->getModelLayerByName(layername));
		if (layer)
		{
			return layer->getOpacity();
		}
		return -1.0;
	}

	void CommonFeatureSystem::setLayerLightEnabled(const std::string & layername, bool isLightEnabled)
	{
		if (!_sceneMap || _sceneMap == NULL) return;

		CommonFeatureLayer* layer = dynamic_cast<CommonFeatureLayer*>(_sceneMap->getModelLayerByName(layername));
		if (layer)
		{
			layer->setLightingEnabled(isLightEnabled);
		}
	}

	bool CommonFeatureSystem::getLayerLightEnabled(const std::string & layername)
	{
		if (!_sceneMap || _sceneMap == NULL) return false;

		CommonFeatureLayer* layer = dynamic_cast<CommonFeatureLayer*>(_sceneMap->getModelLayerByName(layername));
		if (layer)
		{
			return layer->isLightingEnabled();
		}
		return false;
	}


	void CommonFeatureSystem::setLayerVisible(CommonFeatureLayer* layer, bool isVisible)
	{
		if (!_sceneMap || _sceneMap == NULL || !layer || layer == NULL)
		{
			return;
		}

		layer->setVisible(isVisible);
	}

	void CommonFeatureSystem::addLayerCallback(const std::string& layername, osgEarth::ModelLayerCallback* cb)
	{
		if ( !cb || cb == NULL) return;

		CommonFeatureLayer* layer = dynamic_cast<CommonFeatureLayer*>(_sceneMap->getModelLayerByName(layername));
		addLayerCallback(layer, cb);
	}

	void CommonFeatureSystem::addLayerCallback(CommonFeatureLayer* layer, osgEarth::ModelLayerCallback* cb)
	{
		if ( !layer || layer == NULL || !cb || cb == NULL)return;

		layer->addCallback(cb);
	}


	void CommonFeatureSystem::removeLayerCallback(const std::string& layername, osgEarth::ModelLayerCallback* cb)
	{
		if (!_sceneMap || _sceneMap == NULL || !cb || cb == NULL)
		{
			return;
		}

		CommonFeatureLayer* layer = dynamic_cast<CommonFeatureLayer*>(_sceneMap->getModelLayerByName(layername));
		removeLayerCallback(layer, cb);
	}

	osg::Node * CommonFeatureSystem::getLayerNode(const std::string & layername)
	{
		return nullptr;
	}

	void CommonFeatureSystem::removeLayerCallback(CommonFeatureLayer* layer, osgEarth::ModelLayerCallback* cb)
	{
		if (!layer || layer == NULL || !cb || cb == NULL)
		{
			return;
		}

		layer->removeCallback(cb);
	}

// 	void CommonFeatureSystem::addCallback(CommonFeatureSystemCallback* cb)
// 	{
// 		if (!cb || cb == NULL)
// 		{
// 			return;
// 		}
// 		_callbackVector.push_back(cb);
// 	}
// 
// 
// 	void CommonFeatureSystem::removeCallback(CommonFeatureSystemCallback* cb)
// 	{
// 		CommonFeatureSystemCallbackVector::iterator it = std::find(_callbackVector.begin(), _callbackVector.end(), cb);
// 		if (it != _callbackVector.end())
// 		{
// 			_callbackVector.erase(it);
// 		}
// 	}
}