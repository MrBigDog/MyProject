#include <gwBase/Export.h>

#include <gwBase/Root.h>
#include <gwBase/Render.h>
#include <gwBase/PerfMon.h>
#include <gwBase/EventMgr.h>
#include <gwBase/CommandMgr.h>
#include <gwBase/SubsystemMgr.h>
#include <gwBase/CacheMgr.h>
#include <gwBase/PropertyInterpolationMgr.h>
#include <gwBase/CameraManipulatorManager.h>

#include <osgEarth/MapNode>

namespace gwBase
{
	/// subsystem update
	class SystemUpdateRenderCallback : public RenderCallback
	{
	public:
		SystemUpdateRenderCallback(ISubsystemMgr* systemMgr)
			: _systemMgr(systemMgr)
		{}

		void update(double deltaTimeScend)
		{
			if (!_systemMgr) return;
			_systemMgr->update(deltaTimeScend);
		}

	private:
		osg::ref_ptr<ISubsystemMgr> _systemMgr;
	};
}

extern "C" GW_BASE_EXPORT bool Login(gwApp::IApplication* app)
{
	static gwBase::SubsystemMgr subsysMgr;
	app->setSubsystemMgr(&subsysMgr);

	static gwBase::Root root;
	root.addChild(app->getMapNode());
	app->setRoot(&root);

	static gwBase::Render render;
	render.addCallback(new gwBase::SystemUpdateRenderCallback(&subsysMgr));
	render.setRenderData(root.get());
	app->setRender(&render);

	static gwBase::CommandMgr commandMgr;
	commandMgr.getUpdateOperationQueue(render.getViewer());
	app->setCommandMgr(&commandMgr);

	static gwBase::CameraManipulatorManager camMgr;
	camMgr.attatchViewer(render.getViewer());
	app->setCameraManipulatorManager(&camMgr);

	static gwBase::CacheMgr cacheMgr;
	app->setCacheMgr(&cacheMgr);
	subsysMgr.add("CacheMgr", &cacheMgr, gwBase::ISubsystemMgr::GENERAL);

	static gwBase::EventMgr evengMgr;
	app->setEventMgr(&evengMgr);
	subsysMgr.add("EventMgr", &evengMgr, gwBase::ISubsystemMgr::GENERAL);

	//不对外暴露接口;
	static gwBase::PerformanceMonitor pm(&subsysMgr, app->getPropsRoot());
	pm.bind();
	subsysMgr.add("PerformanceMonitor", &pm, gwBase::ISubsystemMgr::GENERAL);

	static gwBase::PropertyInterpolationMgr pim;
	pim.bind();
	subsysMgr.add("PropertyInterpolationMgr", &pim, gwBase::ISubsystemMgr::GENERAL);

	return true;
}