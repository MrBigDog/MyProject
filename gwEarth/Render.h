#ifndef GWEARTH_RENDER_H
#define GWEARTH_RENDER_H 1

#include <compiler.h>
//#include "Win32ViewerT.h"
#include <gwEarth/Export.h>
#include <osgViewer/Viewer>

namespace gwEarth
{
	class RenderCallback : public osg::Referenced
	{
	public:
		RenderCallback() {}
		virtual ~RenderCallback() {}
		virtual void update(double deltaTime) = 0;
	};
	typedef std::vector<osg::ref_ptr<RenderCallback> > RenderCallbackVector;

	class GWEARTH_EXPORT Render : public osg::Referenced
	{
	public:
		Render();

		virtual osgViewer::Viewer* getViewer(/*std::string viewName*/);

		virtual void setRenderData(osg::Node* node);
		virtual void setWindowSize(int x, int y, int width, int height);

		virtual void addOperation(osg::Operation* operation);

		virtual void addCallback(RenderCallback* renderCallback);
		virtual void removeCallback(RenderCallback* renderCallback);

		virtual void fullScreen();
		virtual void escFullScreen();

		virtual void update();

	private:
		void initViewer()
		{
			//#ifdef USE_WIN32_VIEWER
			//		_viewer = new Win32ViewerT<osgViewer::Viewer>(true);
			//#else
			_viewer = new osgViewer::Viewer;
			//if (_cmSwitch)
			//{
			//	osgGA::CameraManipulator* cm = _cmSwitch->get(FIRST_CAMERAMANIPULATOR);
			//	if (cm) _viewer->setCameraManipulator(cm);
			//}
	//#endif
		}

	private:
		//#ifdef USE_WIN32_VIEWER
		//	osg::ref_ptr<Win32ViewerT<osgViewer::Viewer> > _viewer;// = new Win32ViewerT<osgViewer::Viewer>(true);
		//#else
		osg::ref_ptr<osgViewer::Viewer> _viewer;
		//#endif

		RenderCallbackVector _callbacks;

		//osg::ref_ptr<ICameraManipulatorManager> _cmSwitch;
	};

}

#endif // GWBASE_RENDER_H