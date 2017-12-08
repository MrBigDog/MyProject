#ifndef GWAPP_APPLICATION_H
#define GWAPP_APPLICATION_H 1

#include <Compiler.h>
#include <gwApp/Export.h>
#include <gwBase/SubsystemMgr.h>

#include <osg/ref_ptr>
#include <osg/Referenced>

#include <string>

namespace osgEarth
{
	class Map;
	class MapNode;
	class Config;
	namespace Util { class SkyNode; }
}

class GWPropertyNode;

namespace gwBase
{
	class CacheMgr;
	class CommandMgr;
	class EventMgr;
	//class SubsystemMgr;
	//class Subsystem;
}

namespace gwEarth
{
	class Root;
	class Render;
	class CameraManipulatorManager;
}

namespace gwApp
{
	class Environment;

	class GW_APP_EXPORT Application : public osg::Referenced
	{
	public:
		Application();
		~Application();

		static Application* instance(bool erase = false);

		virtual void destruct();

		virtual void initApplicationFromConfig(const osgEarth::Config& appConfig);

		virtual void setRoot(gwEarth::Root* root);
		virtual gwEarth::Root* getRoot() const;

		virtual void setMap(osgEarth::Map* map);
		virtual osgEarth::Map* getMap() const;

		virtual void setMapNode(osgEarth::MapNode* mapnode);
		virtual osgEarth::MapNode* getMapNode() const;

		virtual osgEarth::Util::SkyNode* getSky() const;
		virtual void attatchSky();
		virtual void detatchSky();

		virtual void setCacheMgr(gwBase::CacheMgr* cacheMgr);
		virtual gwBase::CacheMgr* getCacheMgr() const;

		//
		virtual void executeCommand(const std::string & cmdKey, const GWPropertyNode& arg, bool isExecuteAsOperation);
		virtual gwBase::CommandMgr* getCommandMgr() const;
		virtual void setCommandMgr(gwBase::CommandMgr* cmdMgr);

		virtual gwEarth::Render* getRender() const;
		virtual void setRender(gwEarth::Render* render);

		virtual gwEarth::CameraManipulatorManager* getCameraManipulatorManager() const;
		virtual void setCameraManipulatorManager(gwEarth::CameraManipulatorManager* cmManager);

		virtual void setEventMgr(gwBase::EventMgr* eventMgr);
		virtual gwBase::EventMgr* getEventMgr() const;

		//system.
		virtual gwBase::SubsystemMgr* getSystemMgr() const;
		virtual void setSystemMgr(gwBase::SubsystemMgr* susMgr);
		template<typename T>
		T* getOrCreateSubsystem(const std::string& name, gwBase::SubsystemMgr::GroupType type)
		{
			T* subSys = dynamic_cast<T*>(_systemMgr->get_subsystem(name));
			if (!subSys)
			{
				subSys = new T(name, this);
				subSys->bind();
				_systemMgr->add(name.c_str(), subSys, type);
			}
			return subSys;
		}

		virtual GWPropertyNode* getPropsRoot() const;

		virtual Environment* getEnvironment() const;

		virtual void render();

	protected:
		void init();

	private:
		osg::ref_ptr<osgEarth::Map> _map;
		osg::ref_ptr<osgEarth::MapNode> _mapNode;
		osg::ref_ptr<osgEarth::Util::SkyNode> _sky;

		osg::ref_ptr<gwBase::CacheMgr> _cacheMgr;
		osg::ref_ptr<gwBase::EventMgr> _eventMgr;
		osg::ref_ptr<gwBase::CommandMgr>  _commandMgr;
		osg::ref_ptr<gwBase::SubsystemMgr>  _systemMgr;
		osg::ref_ptr<GWPropertyNode> _propsRoot;

		osg::ref_ptr<gwEarth::Root>  _root;
		osg::ref_ptr<gwEarth::Render>  _render;
		osg::ref_ptr<gwEarth::CameraManipulatorManager> _cameraManipulatorManager;

		osg::ref_ptr<Environment> _environment;
	};

	typedef osg::ref_ptr<Application> ApplicationPtr;
}

#endif // GWAPP_APPLICATION_H
