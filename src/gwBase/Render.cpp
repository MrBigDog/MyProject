#include <gwBase/Render.h>

#include <osgDB/Registry>
#include <osgGA/StateSetManipulator>
#include <osgViewer/ViewerEventHandlers>

namespace gwBase
{
	Render::Render() : IRender()
	{
		_viewer = new osgViewer::Viewer;
		_viewer->getCamera()->setNearFarRatio(0.00002);
		_viewer->addEventHandler(new osgGA::StateSetManipulator(_viewer->getCamera()->getOrCreateStateSet()));
		_viewer->addEventHandler(new osgViewer::StatsHandler());
		_viewer->addEventHandler(new osgViewer::WindowSizeHandler());
		_viewer->addEventHandler(new osgViewer::ThreadingHandler());
		_viewer->addEventHandler(new osgViewer::LODScaleHandler());
		_viewer->addEventHandler(new osgViewer::RecordCameraPathHandler());
		_viewer->addEventHandler(new osgViewer::ScreenCaptureHandler());
		_viewer->getDatabasePager()->setUnrefImageDataAfterApplyPolicy(true, true);
		_viewer->getDatabasePager()->setDoPreCompile(true);
		//_viewer->getDatabasePager()->setTargetMaximumNumberOfPageLOD(100);
		//_viewer->getDatabasePager()->setex
		_viewer->getCamera()->setSmallFeatureCullingPixelSize(-1.0f);

		//osgDB::Registry::instance()->getObjectWrapperManager()->findWrapper("osg::Image");
	}

	void Render::update()
	{
		if (!_viewer) return;

		float last_sleep = 0.000001;

		osg::Timer timer;
		while (!_viewer->done())
		{
			if (_viewer->getRunFrameScheme() != osgViewer::ViewerBase::CONTINUOUS && !_viewer->checkNeedToDoFrame())
			{
				continue;
			}

			osg::Timer_t start_time = timer.tick();
			_viewer->frame();
			osg::Timer_t end_time = timer.tick();

			double ft = timer.delta_s(start_time, end_time);

			//double sleeptime = 1.0 / 35.0 - ft;//为了把帧速控制到35，将需要sleep的时间算出来  
			//if (sleeptime < 0.0)
			//{
			//	//当sleep小于0的时候，也就是不需要sleep时，也让程序小睡一会，睡个八分饱，为了解决帧速猛增的问题  
			//	sleeptime = last_sleep*0.8;
			//}
			//last_sleep = sleeptime;

			//OpenThreads::Thread::microSleep(sleeptime * 1000000);//microSleep方法的参数为百万分之一秒，所以一百万为1s  

			//callbacks
			double delta_time_secend = ft /*+ sleeptime*/;
			for (RenderCallbackVector::const_iterator it = _callbacks.begin(); it != _callbacks.end(); ++it)
			{
				(*it)->update(delta_time_secend);
			}
		}
	}

	osgViewer::Viewer * Render::getViewer(/*std::string viewName*/)
	{
		return _viewer;
	}

	void Render::setRenderData(osg::Node * node)
	{
		if (!_viewer || !node) return;
		_viewer->setSceneData(node);
	}

	void Render::setWindowSize(int x, int y, int width, int height)
	{
		if (!_viewer) return;
		_viewer->setUpViewInWindow(x, y, width, height);
	}

	void Render::addOperation(osg::Operation * operation)
	{
		if (!_viewer || !operation) return;
		_viewer->addUpdateOperation(operation);
	}

	void Render::addCallback(RenderCallback * renderCallback)
	{
		if (!renderCallback) return;
		_callbacks.push_back(renderCallback);
	}

	void Render::removeCallback(RenderCallback * renderCallback)
	{
		RenderCallbackVector::const_iterator it
			= std::find(_callbacks.begin(), _callbacks.end(), renderCallback);
		if (it != _callbacks.end())
		{
			_callbacks.erase(it);
		}
	}

	void Render::fullScreen()
	{
		if (!_viewer) return;
		_viewer->setUpViewAcrossAllScreens();
	}

	void Render::escFullScreen()
	{
		setWindowSize(100, 100, 1200, 800);
	}
}
