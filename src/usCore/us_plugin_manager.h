///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_plugin_manager.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : plugin_manager class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_PLUGIN_MANEGER_H_
#define _US_PLUGIN_MANEGER_H_

#include <usCore\Export.h>
#include <usUtil\us_common_file.h>
#include <vector>
#include <windef.h>

namespace uniscope_globe
{
	class application_base;
	const int plugin_init_ring_0 = 0;
	const int plugin_init_reserved_1 = 1;
	const int plugin_init_reserved_2 = 2;
	const int plugin_init_ring_3 = 3;
	const int plugin_result_error = -1;

	typedef int(*PluginInitialize_t)(application_base* core, int ring);
	typedef void(*PluginFinalize_t)(application_base* core);

	struct plugin_base
	{
		ustring m_fileName;
		HMODULE m_hmodule;
		PluginInitialize_t m_initialize;
		PluginFinalize_t m_finalize;
		int m_numIncompleteModules;
	};

	class USCORE_EXPORT plugin_manager
	{
	public:
		void init(application_base* core, cpstr pluginPath);
		void fini(application_base* core);
	public:
		std::vector<plugin_base> m_plugins;
	};

	//AnGeoExport PluginManager* AnGeoGetPluginManager();
	//AnGeoExport PluginManager* CreatePluginManager( Framework* vFrm, angeo_lpstr rootPath );

}

#endif//_US_PLUGIN_MANEGER_H_

