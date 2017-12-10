#include <gwApp/AppInstance.h>
#include <gwApp/Application.h>

extern "C" GW_APP_EXPORT gwApp::IApplication* getApplication()
{
	static gwApp::Application app;
	return &app;
}