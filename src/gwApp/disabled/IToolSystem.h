#ifndef GWAPP_ITOOLSYSTEM_H
#define GWAPP_ITOOLSYSTEM_H 1

#include "IApplication.h"
#include <gwBase/ISubsystemMgr.h>

class IPickTool;
class IQueryTool;
class IFirstCameraManipulator;
class IEarthCameraManipulator;

namespace osgEarth { class Config; }

class IToolSystem:public gwBase::ISubsystem
{
public:
	IToolSystem(IApplication* app) :_app(app) {}
	virtual ~IToolSystem() {}

	void setKey(const std::string& key) { _key = key; }
	std::string getKey() { return _key; }

	virtual void init() = 0;
	virtual void shutdown() = 0;
	virtual void update(double dt) = 0;

	virtual void initSystemFromConfig(const osgEarth::Config& systemConfig) = 0;

	virtual void setPickTool(IPickTool* pick) = 0;
	virtual IPickTool* getPickTool() = 0;

	virtual void setQuerykTool(IQueryTool* pick) = 0;
	virtual IPickTool* getQuerykTool() = 0;

	virtual void setFirstCameraManipulator(IFirstCameraManipulator* firstCM) = 0;
	virtual IFirstCameraManipulator* getFirstCameraManipulator() = 0;

	virtual void setEarthCameraManipulator(IEarthCameraManipulator* firstCM) = 0;
	virtual IEarthCameraManipulator* getEarthCameraManipulator() = 0;

private:
	IApplication* _app;
	std::string _key;
};
#endif // GWAPP_ITOOLSYSTEM_H
