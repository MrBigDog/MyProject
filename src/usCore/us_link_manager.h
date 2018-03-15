///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_link_manager.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : link_manager class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_LINK_MANAGER_H_
#define _US_LINK_MANAGER_H_

#include <usCore\Export.h>
#include <usCore\us_link_http.h>
#include <usUtil\us_mutex.h>
#include <usUtil\us_common_file.h>
#include <map>

namespace uniscope_globe
{
	class USCORE_EXPORT link_manager
	{
	public:
		link_manager(void);

		~link_manager(void);

		typedef std::map<ulong, http_link_handle>	 thread_http_link_map;
		typedef std::map<ustring, thread_http_link_map*> http_link_map;

	public:
		link_base* get_link(const LPCTSTR link_url);

		void update();

	public:
		http_link_handle get_http_link(http_link_info& link_ref);

		http_link_handle get_http_link_ex(http_link_info& link_ref);

	private:
		http_link_handle create_http_link(http_link_info& link_ref);

	public:
		static void create_http_link(http_link_info& link_ref, http_link_handle& v_link);

	private:
		http_link_map m_http_links;

		US_AUTO_MUTEX

	};

	/*
	class singleton_link_manager
		: public singleton<link_manager>
	{
	public:
		singleton_link_manager( void )
		{
		}

		virtual ~singleton_link_manager( void )
		{
		}
	};

//*/

}

#endif // _US_LINK_MANAGER_H_