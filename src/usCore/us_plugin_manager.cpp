///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_plugin_manager.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : plugin_manager class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_plugin_manager.h"

namespace uniscope_globe
{
	const int plugin_init_failed		= -1;
	const int plugin_init_error			= -2;

	void plugin_manager::init(application_base* core, cpstr pluginPath)
	{
		ustring path = pluginPath;
		//ustring filter = L"d:\\lib_osgb_reader.plugin";
		ustring filter = path + L"lib_osgb_reader.plugin";
		WIN32_FIND_DATA wfd;
		HANDLE handle = FindFirstFile(filter.c_str(), &wfd);
		if(INVALID_HANDLE_VALUE != handle)
		{
			do
			{
				ustring fileName = wfd.cFileName;
				HMODULE hmodule = LoadLibraryW(fileName.c_str());
				DWORD kk =  GetLastError();
				
				if(0 != hmodule)
				{
					PluginInitialize_t callBackInit = (PluginInitialize_t)GetProcAddress(hmodule, "PluginInitialize");
					
					PluginFinalize_t callBackFini = (PluginFinalize_t)GetProcAddress(hmodule, "PluginFinalize");
					if(0 != callBackInit && 0 != callBackFini)
					{
						plugin_base plugin;
						plugin.m_fileName = wfd.cFileName;
						plugin.m_hmodule = hmodule;
						plugin.m_initialize = callBackInit;
						plugin.m_finalize = callBackFini;
						plugin.m_numIncompleteModules = 0x7fffffff;
						(*plugin.m_initialize)(core, 0);
						m_plugins.push_back(plugin);
					}
				}
			}
			while(FindNextFile(handle, &wfd));
		}

	}

	void plugin_manager::fini(application_base* core)
	{
		size_t count = m_plugins.size();
		for(size_t i = 0; i < count; ++i)
		{
			(*m_plugins[i].m_finalize)(core);
		}
		for(size_t i = 0; i < count; ++i)
		{
			FreeLibrary(m_plugins[i].m_hmodule);
		}
	}

	//#ifdef _DEBUG
	//cpstr plugin_path = L"plugins_d\\";
	//#else
	//cpstr plugin_path = L"plugins\\";
	//#endif

	//plugin_manager* create_plugin_manager( Framework* vFrm, angeo_lpstr rootPath )
	//{
	//	// ³õÊ¼»¯²å¼þ
	//	ustring pluginPath(rootPath);
	//	pluginPath += plugin_path;
	//	uniscope_globe* _uniscope_globe = AnGeoGetuniscope_globe();
	//	_uniscope_globe->Initialize(vFrm, pluginPath.c_str());

	//	return _uniscope_globe;
	//}
}

