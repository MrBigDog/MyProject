#include <gwApp/Application.h>
#include <gwApp/Environment.h>

#include <gwEarth/Root.h>
#include <gwEarth/Render.h>
#include <gwEarth/TerrainSystem.h>
#include <gwEarth/ModelSystem.h>
#include <gwEarth/CameraManipulatorManager.h>

#include <gwBase/CacheMgr.h>
#include <gwBase/EventMgr.h>
#include <gwBase/CommandMgr.h>
//#include <gwBase/SubsystemMgr.h>
#include <gwBase/PerformMonitor.h>
#include <gwBase/PropertyInterpolationMgr.h>

#include <osgEarth/MapNode>
#include <osgEarth/TerrainEngineNode>
#include <osgEarthUtil/LODBlending>
#include <osgEarthUtil/VerticalScale>
#include <osgEarthUtil/Sky>
//#include <osgEarthDrivers/cache_leveldb/LevelDBCacheOptions>
#include <osgEarthDrivers/cache_filesystem/FileSystemCache>

#include <osg/Group>
#include <osgViewer/Viewer>

using namespace osgEarth::Util;
using namespace osgEarth::Drivers;
using namespace gwBase;

namespace gwApp
{
	namespace
	{
		// safe method to add sky.
		class SkyOperation : public osg::Operation
		{
		public:
			enum OperationType
			{
				ATTATCH_SKY,
				DETATCH_SKY,
				NO_OPERATION
			};

			SkyOperation(osg::Group* root, osg::Node* child, SkyNode* sky, const OperationType& opType)
				: _root(root)
				, _child(child)
				, _sky(sky)
				, _operationType(opType)
			{}
			virtual void operator()(osg::Object*)
			{
				if (_operationType == ATTATCH_SKY)
				{
					if (!_root || !_sky) return;
					//if (_root->containsNode(_sky)) { return; }
					if (!_sky->containsNode(_child))
						_sky->addChild(_child);

					_root->replaceChild(_child, _sky);
				}
				else if (_operationType == DETATCH_SKY)
				{
					if (!_root) return;
					_root->replaceChild(_sky, _child);
				}
				else {}
			}
		private:
			osg::ref_ptr<osg::Group> _root;
			osg::ref_ptr<osg::Node> _child;
			osg::ref_ptr<osgEarth::Util::SkyNode> _sky;
			OperationType _operationType;
		};

		class SubsystemMgrRenderCallback :public gwEarth::RenderCallback
		{
		public:
			SubsystemMgrRenderCallback(gwBase::SubsystemMgr* sysMgr)
				:_subSysMgr(sysMgr) {}

			virtual void update(double deltaTime)
			{
				if (!_subSysMgr) return;
				_subSysMgr->update(deltaTime);
			}

		private:
			osg::ref_ptr<gwBase::SubsystemMgr> _subSysMgr;
		};
	}

	//...............................................................................................
	Application::Application()
	{
		init();
	}

	Application::~Application()
	{
		destruct();
	}

	Application * Application::instance(bool erase)
	{
		static osg::ref_ptr<Application> sAPP = new Application;
		if (erase)
		{
			sAPP->destruct();
			sAPP = 0;
		}
		return sAPP.get();
	}

	void Application::destruct()
	{
		_systemMgr->shutdown();
	}

	void Application::init()
	{
		//LevelDBCache::LevelDBCacheOptions lc;
		//lc.rootPath() = "D:/gwCache/LD";

		FileSystemCacheOptions fs;
		fs.rootPath() = "D:/gwCache/FS";

		osgEarth::MapOptions mapOptions;
		mapOptions.cache() = fs;

		LODBlendingOptions lodoptions;
		//lodoptions.verticalScale() = 50.0;

		_map = new osgEarth::Map(mapOptions);
		_mapNode = new osgEarth::MapNode(_map);
		_mapNode->setName("MAP_NODE");
		_mapNode->getTerrainEngine()->addEffect(new LODBlending(lodoptions));

		_propsRoot = new GWPropertyNode();
		_environment = new Environment;
		_cacheMgr = new gwBase::CacheMgr();
		_systemMgr = new gwBase::SubsystemMgr();
		_commandMgr = new gwBase::CommandMgr();
		_eventMgr = new gwBase::EventMgr();

		_root = new gwEarth::Root;
		_root->addChild(_mapNode);
		_render = new gwEarth::Render;
		_render->setRenderData(_root->get());
		_render->addCallback(new SubsystemMgrRenderCallback(_systemMgr));

		_cameraManipulatorManager = new gwEarth::CameraManipulatorManager;
		_cameraManipulatorManager->attatchViewer(_render->getViewer());

		osg::ref_ptr<PerformanceMonitor> pefMonitor = new PerformanceMonitor(_systemMgr, _propsRoot);
		pefMonitor->bind();
		_systemMgr->add("PerformanceMonitor", pefMonitor, gwBase::SubsystemMgr::GENERAL);
		_systemMgr->add("CacheMgr", _cacheMgr, gwBase::SubsystemMgr::GENERAL);
		_systemMgr->add("EventMgr", _eventMgr, gwBase::SubsystemMgr::GENERAL);
	}

	void Application::initApplicationFromConfig(const osgEarth::Config & appConfig)
	{
		// @todo: read config file and init subsystem...
	}

	void Application::setRoot(gwEarth::Root * root)
	{
		_root = root;
	}

	gwEarth::Root * Application::getRoot() const
	{
		return _root;
	}

	void Application::setMap(osgEarth::Map * map)
	{
		_map = map;
	}

	osgEarth::Map * Application::getMap() const
	{
		return _map;
	}

	void Application::setMapNode(osgEarth::MapNode * mapnode)
	{
		_mapNode = mapnode;
	}

	osgEarth::MapNode * Application::getMapNode() const
	{
		return _mapNode;
	}

	osgEarth::Util::SkyNode * Application::getSky() const
	{
		return _sky;
	}

	void Application::attatchSky()
	{
		if (!_root) return;
		if (_root->containsNode(_sky)) return;

		if (!_sky)
		{
			osgEarth::Util::SkyOptions option;
			option.ambient() = 1.0;
			option.hours() = 24;
			_sky = SkyNode::create(option, _mapNode);
			_sky->setName("SKY_NODE");
			_sky->attach(_render->getViewer());
			_sky->addChild(_mapNode);
		}
		_render->addOperation(new SkyOperation(_root->get(), _mapNode, _sky, SkyOperation::ATTATCH_SKY));
	}

	void Application::detatchSky()
	{
		if (!_root) return;
		if (!_root->containsNode(_sky)) { return; }
		//_root->replaceChild(_sky, _mapNode);
		_render->addOperation(new SkyOperation(_root->get(), _mapNode, _sky, SkyOperation::DETATCH_SKY));
	}

	void Application::setCacheMgr(gwBase::CacheMgr * cacheMgr)
	{
		_cacheMgr = cacheMgr;
	}

	gwBase::CacheMgr * Application::getCacheMgr() const
	{
		return _cacheMgr;
	}

	void Application::executeCommand(const std::string & commandKey, const GWPropertyNode& arg, bool isExecuteAsOperation)
	{
		if (!_commandMgr) return;
		_commandMgr->execute(commandKey, arg, isExecuteAsOperation);
	}

	gwBase::CommandMgr * Application::getCommandMgr() const
	{
		return _commandMgr;
	}

	void Application::setCommandMgr(gwBase::CommandMgr * cmdMgr)
	{
		_commandMgr = cmdMgr;
	}

	gwEarth::Render * Application::getRender() const
	{
		return _render;
	}

	void Application::setRender(gwEarth::Render * render)
	{
		_render = render;
	}

	void Application::setCameraManipulatorManager(gwEarth::CameraManipulatorManager * cmManager)
	{
		_cameraManipulatorManager = cmManager;
	}

	gwEarth::CameraManipulatorManager * Application::getCameraManipulatorManager() const
	{
		return _cameraManipulatorManager;
	}

	void Application::setEventMgr(gwBase::EventMgr * eventMgr)
	{
		_eventMgr = eventMgr;
	}

	gwBase::EventMgr * Application::getEventMgr() const
	{
		return _eventMgr;
	}

	gwBase::SubsystemMgr * Application::getSystemMgr() const
	{
		return _systemMgr;
	}

	void Application::setSystemMgr(gwBase::SubsystemMgr * susMgr)
	{
		_systemMgr = susMgr;
	}

	GWPropertyNode * Application::getPropsRoot() const
	{
		return _propsRoot;
	}

	Environment * Application::getEnvironment() const
	{
		return _environment;
	}

	void Application::render()
	{
		if (!_render) return;
		_render->update();
	}
}
