// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
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
//// ������뽫λ������ָ��ƽ̨֮ǰ��ƽ̨��ΪĿ�꣬���޸����ж��塣
//// �йز�ͬƽ̨��Ӧֵ��������Ϣ����ο� MSDN��
//#ifndef WINVER				// ����ʹ���ض��� Windows XP ����߰汾�Ĺ��ܡ�
//#define WINVER 0x0501		// ����ֵ����Ϊ��Ӧ��ֵ���������� Windows �������汾��
//#endif
//
//#ifndef _WIN32_WINNT		// ����ʹ���ض��� Windows XP ����߰汾�Ĺ��ܡ�
//#define _WIN32_WINNT 0x0501	// ����ֵ����Ϊ��Ӧ��ֵ���������� Windows �������汾��
//#endif						
//
//#ifndef _WIN32_WINDOWS		// ����ʹ���ض��� Windows 98 ����߰汾�Ĺ��ܡ�
//#define _WIN32_WINDOWS 0x0410 // ����ֵ����Ϊ�ʵ���ֵ����ָ���� Windows Me ����߰汾��ΪĿ�ꡣ
//#endif
//
//#ifndef _WIN32_IE			// ����ʹ���ض��� IE 6.0 ����߰汾�Ĺ��ܡ�
//#define _WIN32_IE 0x0600	// ����ֵ����Ϊ��Ӧ��ֵ���������� IE �������汾��
//#endif

#define WIN32_LEAN_AND_MEAN		// �� Windows ͷ���ų�����ʹ�õ�����
// Windows ͷ�ļ�:
#include <windows.h>
#include <winsock.h>




// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
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

