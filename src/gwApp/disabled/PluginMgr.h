#ifndef GEOPLUGINMGR_INCLUDE
#define GEOPLUGINMGR_INCLUDE

#include <string>
#include <gwApp/IApplication.h>

namespace gwApp
{
	class PluginMgr
	{
	public:
		explicit PluginMgr(IApplication* app);
		~PluginMgr(void);

		virtual bool LoginBaseFunc();///<必须调用的基本功能

		virtual bool RegisterAllPlugins();

		virtual bool RegisterPlugin(const std::string& name);

		virtual bool UnRegisterPlugin();

		static std::string GetDllPath();

	private:
		class PluginMgrProxy;
		PluginMgrProxy* _proxy;
		IApplication* _app;
	};
}

#endif //GEOPLUGINMGR_INCLUDE

