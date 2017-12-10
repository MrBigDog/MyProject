#ifndef GWBASE_ICAMERAMANIPULATORMANAGER_H
#define GWBASE_ICAMERAMANIPULATORMANAGER_H 1

#include <osg/ref_ptr>
#include <osg/Referenced>

#include <map>
#include <string>
#include <vector>

const std::string FIRST_CAMERAMANIPULATOR = "FIRSTPERSONAL_CAMERAMANIPULATOR";
const std::string THIRD_CAMERAMANIPULATOR = "THIRDPERSONAL_CAMERAMANIPULATOR";
const std::string EARTH_CAMERAMANIPULATOR = "EARTH_CAMERAMANIPULATOR";

namespace osgGA { class CameraManipulator; }
namespace osgViewer { class Viewer; class View; }

namespace gwBase
{
	class CameraManipulatorManagerCallback :public osg::Referenced
	{
	public:
		CameraManipulatorManagerCallback() {}
		virtual ~CameraManipulatorManagerCallback() {}

		virtual void onCurrentManipulatorChanged(osgGA::CameraManipulator* currendCm) = 0;
		virtual void addViewer(osgViewer::Viewer* viewer) {}
		virtual void removeViewer(osgViewer::Viewer* viewer) {}
		virtual void addView(osgViewer::View* view) {}
		virtual void removeView(osgViewer::View* view) {}
	};
	typedef std::vector<osg::ref_ptr<CameraManipulatorManagerCallback> >CameraManipulatorManagerCallbackVector;


	//.....................................................................................................
	class ICameraManipulatorManager :public osg::Referenced
	{
	public:
		ICameraManipulatorManager() {};
		virtual ~ICameraManipulatorManager() {}

		virtual void addCameraManipulator(const std::string& key, osgGA::CameraManipulator* cm) = 0;

		virtual void removeCameraManipulator(const std::string& key) = 0;

		virtual osgGA::CameraManipulator* getCameraManipulator(const std::string& key) = 0;

		virtual void setCurrentCameraManipulator(const std::string& key) = 0;

		virtual osgGA::CameraManipulator* getCurrentCameraManipulator() = 0;
		virtual std::string getCurrentCameraManipulatorKey() = 0;

		virtual void addCallback(CameraManipulatorManagerCallback* cb) = 0;

		virtual void removeCallback(CameraManipulatorManagerCallback* cb) = 0;

		virtual void attatchViewer(osgViewer::Viewer* viewer) = 0;
		virtual void detatchViewer(osgViewer::Viewer* viewer) = 0;
		virtual void attatchView(osgViewer::View* view) = 0;
		virtual void detatchView(osgViewer::View* view) = 0;
	};

	typedef osg::ref_ptr<ICameraManipulatorManager> ICameraManipulatorManagerPtr;
}

#endif // GWAPP_ICAMERAMANIPULATORMANAGER_H
