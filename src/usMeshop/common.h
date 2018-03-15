// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#pragma warning ( disable:4251 )
#pragma warning ( disable:4172 )
#pragma warning ( disable:4996 )
#pragma warning ( disable:4661 )
#pragma warning ( disable:4244 )
#pragma warning ( disable:4018 )
#pragma warning ( disable:4267 )

#include <SDKDDKVer.h>
//// 如果必须将位于下面指定平台之前的平台作为目标，请修改下列定义。
//// 有关不同平台对应值的最新信息，请参考 MSDN。
//#ifndef WINVER				// 允许使用特定于 Windows XP 或更高版本的功能。
//#define WINVER 0x0501		// 将此值更改为相应的值，以适用于 Windows 的其他版本。
//#endif
//
//#ifndef _WIN32_WINNT		// 允许使用特定于 Windows XP 或更高版本的功能。
//#define _WIN32_WINNT 0x0501	// 将此值更改为相应的值，以适用于 Windows 的其他版本。
//#endif						
//
//#ifndef _WIN32_WINDOWS		// 允许使用特定于 Windows 98 或更高版本的功能。
//#define _WIN32_WINDOWS 0x0410 // 将此值更改为适当的值，以指定将 Windows Me 或更高版本作为目标。
//#endif
//
//#ifndef _WIN32_IE			// 允许使用特定于 IE 6.0 或更高版本的功能。
//#define _WIN32_IE 0x0600	// 将此值更改为相应的值，以适用于 IE 的其他版本。
//#endif

#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料
// Windows 头文件:
#include <windows.h>
#include <winsock.h>




// TODO: 在此处引用程序需要的其他头文件
#include <Wininet.h>
#include <MMSystem.h>

#include <d3dx9.h>
#include <dinput.h>
#include <dsound.h>

// crt
#include <float.h>
#include <math.h>

// stl
#include <vector>
#include <string>
#include <list>
#include <iostream>
#include <map>
#include <stack>
#include <queue>
#if _MSC_VER >= 1900
#include <unordered_map>
#else
#include <hash_map>
#endif
#include <algorithm>
using namespace std;


// dependent
//#include <usXml/tinyxml.h>
//#include <usUtil/lib_util.h>
//#include <usLua/LuaPlus.h>
//#include <usDotx/lib_dotx.h>
//#include <usCore/lib_core.h>
//#include <usGfx/lib_gfx.h>
//using namespace uniscope_globe;

