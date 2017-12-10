#include <gwApp/PluginMgr.h>
#include <gwApp/IEnvironment.h>
#include <windows.h>
#include <fstream>
#include <vector>

namespace gwApp
{
	typedef bool(*LoginFun)(IApplication*);

	class PluginMgr::PluginMgrProxy
	{
	public:
		explicit PluginMgrProxy(IApplication* app) :_app(app) 
		{
			HMODULE module = GetModuleHandle(0);

			CHAR buf[MAX_PATH];
			GetModuleFileName(module, buf, sizeof buf);
			std::string fullpath(buf);
			_path = fullpath.substr(0, fullpath.rfind('\\') + 1);
		}

		~PluginMgrProxy()
		{
			for (unsigned int index = 0; index < _hdl_container.size(); ++index)
			{
				::FreeLibrary(_hdl_container[index]);
			}
		}

		bool LoginBaseFunc()
		{
			std::string dll = ".dll";
			///std::string fullpath = _getcwd(buf,256);
			HMODULE module = GetModuleHandle(0);

			CHAR buf[MAX_PATH];
			GetModuleFileName(module, buf, sizeof buf);
			std::string fullpath(buf);

			_path = fullpath.substr(0, fullpath.rfind('\\') + 1);

			std::string render_name;
#			ifdef _DEBUG
			render_name = "gwBased";
#			else 
			render_name = "gwBase";
#			endif
			const std::string  dllpath = _path + (render_name + dll);
			std::ifstream fin(dllpath);
			if (!fin) return false;
			HMODULE hdl = LoadLibraryA(dllpath.c_str());
			_hdl_container.push_back(hdl);
			LoginFun login = (LoginFun)GetProcAddress(hdl, "Login");

			/*
			LPVOID lpMsgBuf;
			LPVOID lpDisplayBuf;
			DWORD dw = GetLastError();

			FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dw,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR) &lpMsgBuf,
			0, NULL );
			const char* cch = (const char*)lpMsgBuf;*/

			if (!login) return false;

			if (!login(_app))
			{
				///todo:日志  插件加载失败
				return false;
			}
			return true;
		}

		bool LoadAllPlugins()
		{
			std::vector<std::string> plugins;
			_app->getEnvironment()->GetAllPluginsFromConfig(plugins);
			for (unsigned int index = 0; index < plugins.size(); ++index)
			{
				LoadPlugin(plugins[index]);
			}
			return true;
		}

		bool LoadPlugin(const std::string& name)
		{
			std::string dll = ".dll";

			std::string tmp_str = name;

#			ifdef _DEBUG
			tmp_str += "d";
#			endif

			const std::string dllpath = _path + tmp_str + dll;

			std::ifstream fin(dllpath);
			if (!fin) return false;
			HMODULE hdl = LoadLibraryA(dllpath.c_str());
			LoginFun login = (LoginFun)GetProcAddress(hdl, "Login");
			_hdl_container.push_back(hdl);

			if (!login(_app))
			{
				///todo:日志插件加载失败
				return false;
			}
			return true;
		}

		bool UnRegisterPlugin()
		{
			return true;
		}

		static std::string GetDllPath()
		{
			static std::string dllname;
			if (dllname.empty())
			{
				char buff[1024];
				GetCurrentDirectory(1000, buff);  //得到当前工作路径  
				//{
				//	HANDLE hDllhandle = GetModuleHandle( "GWApp.dll" ); //获取当前模块句柄
				//	if ( hDllhandle == NULL )
				//	{
				//	}
				//	memset( buff, 0, 1024 );
				//	GetModuleFileName( ( HMODULE )hDllhandle, buff, 1024 ); //获取当前模块句柄路径
				//}
				std::string pp(buff);
				dllname = pp;
			}
			return dllname;
		}

	private:
		IApplication* _app;
		std::string _path;
		std::vector<HMODULE> _hdl_container;
	};

	PluginMgr::PluginMgr(IApplication* app)
	{
		_app = app;
		_proxy = new PluginMgrProxy(_app);
	}

	PluginMgr::~PluginMgr(void)
	{
		if (NULL != _proxy)
		{
			delete _proxy;
			_proxy = NULL;
		}
	}

	bool PluginMgr::LoginBaseFunc()
	{
		return _proxy->LoginBaseFunc();
	}

	bool PluginMgr::RegisterAllPlugins()
	{
		return _proxy->LoadAllPlugins();
	}

	bool PluginMgr::RegisterPlugin(const std::string& name)
	{
		return _proxy->LoadPlugin(name);
	}

	bool PluginMgr::UnRegisterPlugin()
	{
		return _proxy->UnRegisterPlugin();
	}

	std::string PluginMgr::GetDllPath()
	{
		return PluginMgrProxy::GetDllPath();
	}
}
