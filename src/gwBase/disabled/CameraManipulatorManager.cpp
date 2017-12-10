#include <gwBase/CameraManipulatorManager.h>
#include <osgEarthUtil/EarthManipulator>
#include <osgViewer/Viewer>
#include <osgGA/CameraManipulator>

namespace gwBase
{
	class ManipulatorSwitchCallback : public CameraManipulatorManagerCallback
	{
	public:
		ManipulatorSwitchCallback()
		{}

		virtual void onCurrentManipulatorChanged(osgGA::CameraManipulator* currendCm)
		{
			if (_currentCameraManipulator == currendCm)
			{
				return;
			}
			for (ViewerVector::const_iterator it = _viewers.begin(); it != _viewers.end(); ++it)
			{
				(*it)->setCameraManipulator(currendCm);
			}
			for (ViewVector::const_iterator it = _views.begin(); it != _views.end(); ++it)
			{
				(*it)->setCameraManipulator(currendCm);
			}
			_currentCameraManipulator = currendCm;
		}

		void addViewer(osgViewer::Viewer* viewer)
		{
			if (!viewer) return;
			ViewerVector::const_iterator it = std::find(_viewers.begin(), _viewers.end(), viewer);
			if (it == _viewers.end())
			{
				_viewers.push_back(viewer);
				if (_currentCameraManipulator&&_currentCameraManipulator != NULL)
				{
					viewer->setCameraManipulator(_currentCameraManipulator);
				}
			}
		}

		virtual void removeViewer(osgViewer::Viewer* viewer)
		{
			ViewerVector::const_iterator it = std::find(_viewers.begin(), _viewers.end(), viewer);
			if (it != _viewers.end())
			{
				viewer->setCameraManipulator(NULL);
				_viewers.erase(it);
			}
		}

		virtual void addView(osgViewer::View* view)
		{
			if (!view) return;
			ViewVector::const_iterator it = std::find(_views.begin(), _views.end(), view);
			if (it == _views.end())
			{
				_views.push_back(view);
				if (_currentCameraManipulator&&_currentCameraManipulator != NULL)
				{
					view->setCameraManipulator(_currentCameraManipulator);
				}
			}
		}

		virtual void removeView(osgViewer::View* view)
		{
			ViewVector::const_iterator it = std::find(_views.begin(), _views.end(), view);
			if (it != _views.end())
			{
				view->setCameraManipulator(NULL);
				_views.erase(it);
			}
		}
	private:
		typedef std::vector<osg::ref_ptr<osgViewer::Viewer> > ViewerVector;
		typedef std::vector<osg::ref_ptr<osgViewer::View> > ViewVector;

		ViewerVector _viewers;
		ViewVector _views;
		osg::ref_ptr<osgGA::CameraManipulator> _currentCameraManipulator;
	};

	//////////////////////////////////////////////////////////////////////////
	CameraManipulatorManager::CameraManipulatorManager()
	{
		//add(FIRST_CAMERAMANIPULATOR, new FSPManipulator());
		//add(THIRD_CAMERAMANIPULATOR, new TRDManipulator());
		osg::ref_ptr<osgEarth::Util::EarthManipulator> em = new osgEarth::Util::EarthManipulator;
		em->getSettings()->setThrowingEnabled(true);
		em->getSettings()->setThrowDecayRate(0.01);
		addCameraManipulator(EARTH_CAMERAMANIPULATOR, em);

		_manipulatorSwitchCallback = new ManipulatorSwitchCallback();
		addCallback(_manipulatorSwitchCallback);

		setCurrentCameraManipulator(EARTH_CAMERAMANIPULATOR);
	}

	void CameraManipulatorManager::addCameraManipulator(const std::string& key, osgGA::CameraManipulator* cm)
	{
		//cover the cm which has the same name in the map.
		if (!cm) return;
		_cmMap[key] = cm;
	}

	void CameraManipulatorManager::removeCameraManipulator(const std::string& key)
	{
		CameraManipulatorMap::iterator it = _cmMap.find(key);//
		if (it != _cmMap.end())
		{
			_cmMap.erase(it);
		}
	}

	osgGA::CameraManipulator* CameraManipulatorManager::getCameraManipulator(const std::string& key)
	{
		return _cmMap[key];
	}

	void CameraManipulatorManager::setCurrentCameraManipulator(const std::string& key)
	{
		if (_currentManipulatorPair.first == key) return;

		osgGA::CameraManipulator* cm = _cmMap[key];
		if (!cm) return;

		_currentManipulatorPair = CameraManipulatorPair(key, cm);// std::make_pair(key, cm);
		CameraManipulatorManagerCallbackVector::const_iterator it;
		for (it = _callbacks.begin(); it != _callbacks.end(); ++it)
		{
			(*it)->onCurrentManipulatorChanged(cm);
		}
	}

	osgGA::CameraManipulator* CameraManipulatorManager::getCurrentCameraManipulator()
	{
		return _currentManipulatorPair.second;
	}

	std::string CameraManipulatorManager::getCurrentCameraManipulatorKey()
	{
		return _currentManipulatorPair.first;
	}

	void CameraManipulatorManager::addCallback(CameraManipulatorManagerCallback* cb)
	{
		if (!cb) return;
		_callbacks.push_back(cb);
	}

	void CameraManipulatorManager::removeCallback(CameraManipulatorManagerCallback* cb)
	{
		if (!cb) return;
		CameraManipulatorManagerCallbackVector::iterator it;
		it = std::find(_callbacks.begin(), _callbacks.end(), cb);
		if (it == _callbacks.end()) return;

		_callbacks.erase(it);
	}

	void CameraManipulatorManager::attatchViewer(osgViewer::Viewer* viewer)
	{
		if (!_manipulatorSwitchCallback || !viewer) return;
		_manipulatorSwitchCallback->addViewer(viewer);
	}

	void CameraManipulatorManager::detatchViewer(osgViewer::Viewer * viewer)
	{
		if (!_manipulatorSwitchCallback || !viewer) return;
		_manipulatorSwitchCallback->removeViewer(viewer);
	}

	void CameraManipulatorManager::attatchView(osgViewer::View * view)
	{
		if (!_manipulatorSwitchCallback || !view) return;
		_manipulatorSwitchCallback->addView(view);
	}

	void CameraManipulatorManager::detatchView(osgViewer::View * view)
	{
		if (!_manipulatorSwitchCallback || !view) return;
		_manipulatorSwitchCallback->removeView(view);
	}
}
