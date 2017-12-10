#ifndef GWBASE_CAMERAMANIPULATORMANAGER_H
#define GWBASE_CAMERAMANIPULATORMANAGER_H 1

#include <gwBase/Export.h>//GW_BASE_EXPORT

const std::string FIRST_CAMERAMANIPULATOR = "FIRSTPERSONAL_CAMERAMANIPULATOR";
const std::string THIRD_CAMERAMANIPULATOR = "THIRDPERSONAL_CAMERAMANIPULATOR";
const std::string EARTH_CAMERAMANIPULATOR = "EARTH_CAMERAMANIPULATOR";

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

	class GW_BASE_EXPORT CameraManipulatorManager :public osg::Referenced
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
}

#endif // GWBASE_CAMERAMANIPULATORMANAGER_H
