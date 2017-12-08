#ifndef GWEARTH_CAMERAMANIPULATORMANAGER_H
#define GWEARTH_CAMERAMANIPULATORMANAGER_H 1

#include <compiler.h>
#include <gwEarth/Export.h>
#include <osgViewer/Viewer>
#include <string>

const std::string EARTH = "EARTH_CAMERAMANIPULATOR";
const std::string FIRST_PERSONAL = "FIRSTPERSONAL_CAMERAMANIPULATOR";
const std::string THIRD_PERSONAL = "THIRDPERSONAL_CAMERAMANIPULATOR";

namespace gwEarth
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

	class GWEARTH_EXPORT CameraManipulatorManager :public osg::Referenced
	{
	public:
		CameraManipulatorManager();
		~CameraManipulatorManager() {}

		virtual void addCameraManipulator(const std::string& key, osgGA::CameraManipulator* cm);
		virtual void removeCameraManipulator(const std::string& key);

		virtual osgGA::CameraManipulator* getCameraManipulator(const std::string& key);

		virtual void setCurrentCameraManipulator(const std::string& key);
		virtual osgGA::CameraManipulator* getCurrentCameraManipulator();
		virtual std::string getCurrentCameraManipulatorKey();

		virtual void addCallback(CameraManipulatorManagerCallback* cb);
		virtual void removeCallback(CameraManipulatorManagerCallback* cb);

		virtual void attatchViewer(osgViewer::Viewer* viewer);
		virtual void detatchViewer(osgViewer::Viewer* viewer);
		virtual void attatchView(osgViewer::View* view);
		virtual void detatchView(osgViewer::View* view);

	private:
		typedef std::pair<std::string, osg::ref_ptr<osgGA::CameraManipulator> > CameraManipulatorPair;
		typedef std::map<std::string, osg::ref_ptr<osgGA::CameraManipulator> > CameraManipulatorMap;

		CameraManipulatorMap _cmMap;
		CameraManipulatorPair _currentManipulatorPair;

		CameraManipulatorManagerCallbackVector _callbacks;

		osg::ref_ptr<CameraManipulatorManagerCallback> _manipulatorSwitchCallback;
	};

	typedef osg::ref_ptr<CameraManipulatorManager> CameraManipulatorManagerPtr;
}

#endif // GWBASE_CAMERAMANIPULATORMANAGER_H
