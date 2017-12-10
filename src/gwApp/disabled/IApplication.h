#ifndef GWAPP_IAPPLICATION_H
#define GWAPP_IAPPLICATION_H 1

#include <osg/ref_ptr>
#include <osg/Referenced>
#include <string>

//#include <mongocxx/instance.hpp>
//#include <mongocxx/pool.hpp>
//#include <mongocxx/uri.hpp>

//namespace mongocxx { class pool; }

namespace osg { class Node; }

class IToolSystem;
class IPipelineSystem;
class IGeologicalSystem;
class GWPropertyNode;
class IEnvironment;

namespace gwEarth
{
	class ITerrainSystem;
	class IModelSystem;
}

namespace osgEarth
{
	class Map;
	class MapNode;
	class Config;
	namespace Util
	{
		class SkyNode;
	}
}

namespace gwBase
{
	class IRoot;
	class IRender;
	class ICacheMgr;
	class IEventMgr;
	class Subsystem;
	class ICommandMgr;
	class SubsystemMgr;
	class ICameraManipulatorManager;
}

namespace gwApp
{
	class IApplication : public osg::Referenced
	{
	public:
		virtual ~IApplication() {};

		virtual void initApplicationFromConfig(const osgEarth::Config& appConfig) = 0;

		virtual void setMap(osgEarth::Map* map) = 0;
		virtual osgEarth::Map* getMap() const = 0;

		virtual void setMapNode(osgEarth::MapNode* mapnode) = 0;
		virtual osgEarth::MapNode* getMapNode()const = 0;

		virtual void setRoot(gwBase::IRoot* root) = 0;
		virtual gwBase::IRoot* getRoot() const = 0;

		virtual osgEarth::Util::SkyNode* getSky() const = 0;
		virtual void attatchSky() = 0;
		virtual void detatchSky() = 0;

		template<typename SYSTEM_TYPE>
		SYSTEM_TYPE* getSystem(const std::string& key) const
		{
			if (!getSubSystemMgr())return 0L;
			return getSubSystemMgr()->getSubsystem<SYSTEM_TYPE>(key);
		}

		virtual gwEarth::ITerrainSystem* getTerrainSystem(const std::string& key) const = 0;
		virtual osg::Node* getTerrainNode(const std::string& key) const = 0;

		virtual gwEarth::IModelSystem* getModelSystem(const std::string& key) const = 0;
		//virtual osg::Node* getModelSystemNode(const std::string& key) const = 0;

		//tool: pick tool, query tool, camera manipulator......
		//virtual IToolSystem* getToolSystem(const std::string& key) const = 0;

		virtual void setCacheMgr(gwBase::ICacheMgr* cacheMgr) = 0;
		virtual gwBase::ICacheMgr* getCacheMgr() const = 0;

		//
		virtual gwBase::ICommandMgr* getCommandMgr() const = 0;
		virtual void setCommandMgr(gwBase::ICommandMgr* cmdMgr) = 0;
		virtual void executeCommand(const std::string & commandKey, const GWPropertyNode& arg, bool isExecuteAsOperation) = 0;

		virtual gwBase::IRender* getRender() const = 0;
		virtual void setRender(gwBase::IRender* render) = 0;

		virtual void setCameraManipulatorManager(gwBase::ICameraManipulatorManager* cmSwitch) = 0;
		virtual gwBase::ICameraManipulatorManager* getCameraManipulatorManager() const = 0;

		virtual void setEventMgr(gwBase::IEventMgr* eventMgr) = 0;
		virtual gwBase::IEventMgr* getEventMgr() const = 0;

		virtual gwBase::SubsystemMgr* getSubSystemMgr()const = 0;
		virtual void setSubsystemMgr(gwBase::SubsystemMgr* susMgr) = 0;
		virtual void removeSubsystem(const std::string& key) = 0;
		virtual void addSubsystem(const std::string& key, gwBase::Subsystem* sys, const std::string& groupType, double time_sec) = 0;

		template<class T>
		T* getSubsystem(const std::string& key) const
		{
			if (!getSubSystemMgr()) return 0L;
			return getSubSystemMgr()->getSubsystem<T>(key);
		}

		virtual GWPropertyNode* getPropsRoot() const = 0;

		//virtual void setEnvironment(IEnvironment* env) = 0;
		virtual IEnvironment* getEnvironment() const = 0;

		//virtual void connectToMongoDB(const mongocxx::pool& url) = 0;
		//virtual const mongocxx::pool& getMongoDBPool() const = 0;
		//virtual std::unique_ptr<mongocxx::pool>& mongodbPool() = 0;
		//virtual const std::unique_ptr<mongocxx::pool>& mongodbPool() const = 0;

		virtual void render() = 0;
	};
}

#endif // GWAPP_IAPPLICATION_H
