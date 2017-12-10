#ifndef GWBASE_IRENDER_H
#define GWBASE_IRENDER_H 1

#include <gwApp/IApplication.h>
#include <osg/ref_ptr>
#include <osg/Referenced>

#include <vector>

namespace osg
{
	class Node;
	class View;
	class Operation;
}
namespace osgGA { class CameraManipulator; }
namespace osgViewer
{
	class Viewer;
	class CompositeViewer;
}
//namespace OpenThreads { class ReadWriteMutex; }

namespace gwBase
{
	class RenderCallback : public osg::Referenced
	{
	public:
		RenderCallback() {}
		virtual ~RenderCallback() {}
		virtual void update(double deltaTime) = 0;
	};
	typedef std::vector<osg::ref_ptr<RenderCallback> > RenderCallbackVector;

	//.............................................................................
	class IRender : public osg::Referenced
	{
	public:
		IRender() {}
		virtual ~IRender() {}

	public:
		virtual void update() = 0;

		virtual osgViewer::Viewer* getViewer(/*std::string viewName*/) = 0;

		virtual void setRenderData(osg::Node* node) = 0;
		virtual void setWindowSize(int x, int y, int width, int height) = 0;

		virtual void addOperation(osg::Operation* operation) = 0;

		virtual void addCallback(RenderCallback* renderCallback) = 0;
		virtual void removeCallback(RenderCallback* renderCallback) = 0;

		virtual void fullScreen() = 0;
		virtual void escFullScreen() = 0;

		//virtual void addCameraManipulator(const std::string& key, osgGA::CameraManipulator* cm) = 0;
		//virtual osgGA::CameraManipulator* getCameraManipulator(const std::string& key) = 0;
		//virtual void setCurrentCameraManipulator(const std::string& key) = 0;
		//virtual osgGA::CameraManipulator* getCurrentCameraManipulator() = 0;
		//virtual std::string getCurrentCameraManipulatorkey() = 0;

	private:
	};

	typedef osg::ref_ptr<IRender> IRenderPtr;
}

#endif //GWAPP_IRENDER_H
