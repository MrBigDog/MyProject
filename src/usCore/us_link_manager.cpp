///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_link_manager.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : link_manager class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "Stdafx.h"
#include "us_link_manager.h"
#include <usCore/us_link_http.h>
#include <usUtil/us_string_ext.h>
#include <WinInet.h>

namespace uniscope_globe
{
	//link_manager* singleton_link_manager::s_ins = NULL;

	link_manager::link_manager(void)
	{
	}

	link_manager::~link_manager(void)
	{
		http_link_map::iterator map_map_itr;
		for (map_map_itr = m_http_links.begin(); map_map_itr != m_http_links.end(); map_map_itr++)
		{
			thread_http_link_map* thread_map = (thread_http_link_map*)map_map_itr->second;
			thread_http_link_map::iterator itr = thread_map->begin();
			for (; itr != thread_map->end(); itr++)
			{
				AUTO_CLOSE_INTERNET_HANDLE(itr->second.section);
				AUTO_CLOSE_INTERNET_HANDLE(itr->second.internet);
			}
			thread_map->clear();
			AUTO_DELETE(thread_map);
		}

		m_http_links.clear();
	}

	link_base* link_manager::get_link(const LPCTSTR link_url)
	{
		ustring str_link_url = link_url;
		int	npos = (int)str_link_url.find(/*L*/"://");
		ustring	link_type;

		link_base* ret_link = 0;

		if (npos == -1)
		{
			npos = (int)str_link_url.find(/*L*/":\\");
			if (npos > 0)
			{
				ret_link = new link_base(link_url, US_LINK_TYPE_LOCAL);
			}
		}
		else
		{
			link_type = str_link_url.substr(0, npos + 3);
			if (link_type == /*L*/"db://")
			{
				ret_link = new link_http(link_url, US_LINK_TYPE_UDE);
			}
			else if (link_type == /*L*/"uds://")
			{
				ret_link = new link_http(link_url, US_LINK_TYPE_UDS);
			}
			else if (link_type == /*L*/"root://")
			{
				ret_link = new link_base(link_url, US_LINK_TYPE_ROOT);
			}
			else if (link_type == /*L*/"http://")
			{
				ret_link = new link_http(link_url, US_LINK_TYPE_HTTP);
			}
			else if (link_type == /*L*/"wmts://")
			{
				ret_link = new link_http(link_url, US_LINK_TYPE_WMTS);
			}
		}

		if (ret_link)
		{
			ret_link->set_link_manager(this);
		}

		return ret_link;
	}

	void link_manager::update()
	{
		http_link_map::iterator map_map_itr = m_http_links.begin();

		for (; map_map_itr != m_http_links.end(); map_map_itr++)
		{
			thread_http_link_map* thread_map = (thread_http_link_map*)map_map_itr->second;
			thread_http_link_map::iterator itr = thread_map->begin();
			for (; itr != thread_map->end(); itr++)
			{
				ulong v_link_id = itr->first;
				HANDLE thd = OpenThread(THREAD_ALL_ACCESS, false, v_link_id);
				if (thd == NULL)
				{
					AUTO_CLOSE_INTERNET_HANDLE(itr->second.section);
					AUTO_CLOSE_INTERNET_HANDLE(itr->second.internet);
					thread_map->erase(itr);
					break;
				}
			}
		}
	}

	http_link_handle link_manager::get_http_link(http_link_info& link_ref)
	{
		// return value
		http_link_handle link;

		US_LOCK_AUTO_MUTEX

			// map key
#ifdef _UNICODE
			ustring str_key = link_ref.http_host + string_ext::to_wstring(string_ext::from_dword(link_ref.port));
#else
			ustring str_key = link_ref.http_host + string_ext::from_dword(link_ref.port);
#endif //_UNICODE

		ulong v_link_id = ::GetCurrentThreadId();

		// get link
		http_link_map::iterator map_map_itr = m_http_links.find(str_key);
		if (map_map_itr != m_http_links.end())
		{
			thread_http_link_map* thread_map = (thread_http_link_map*)map_map_itr->second;
			thread_http_link_map::iterator itr = thread_map->find(v_link_id);
			if (itr != thread_map->end())
			{
				link = itr->second;
			}
			else
			{
				link = create_http_link(link_ref);
				thread_map->insert(make_pair(v_link_id, link));
			}
		}
		else
		{
			link = create_http_link(link_ref);
			thread_http_link_map* thread_map = new thread_http_link_map;
			thread_map->insert(make_pair(v_link_id, link));
			m_http_links[str_key] = thread_map;
		}

		return link;
	}

	http_link_handle link_manager::get_http_link_ex(http_link_info& link_ref)
	{
		// return value

		US_LOCK_AUTO_MUTEX

			http_link_handle link;

		// map key
#ifdef _UNICODE
		ustring str_key = link_ref.http_host + string_ext::to_wstring(string_ext::from_dword(link_ref.port));
#else
		ustring str_key = link_ref.http_host + string_ext::from_dword(link_ref.port);
#endif //_UNICODE

		ulong v_link_id = ::GetCurrentThreadId();

		// get link
		http_link_map::iterator map_map_itr = m_http_links.find(str_key);
		if (map_map_itr != m_http_links.end())
		{
			thread_http_link_map* thread_map = (thread_http_link_map*)map_map_itr->second;
			thread_http_link_map::iterator itr = thread_map->find(v_link_id);
			if (itr != thread_map->end())
			{
				AUTO_CLOSE_INTERNET_HANDLE(itr->second.section);
				AUTO_CLOSE_INTERNET_HANDLE(itr->second.internet);

				thread_map->erase(itr);
			}

			link = create_http_link(link_ref);
			thread_map->insert(make_pair(v_link_id, link));
		}
		else
		{
			link = create_http_link(link_ref);
			thread_http_link_map* thread_map = new thread_http_link_map;
			thread_map->insert(make_pair(v_link_id, link));
			m_http_links[str_key] = thread_map;
		}

		return link;
	}

	void link_manager::create_http_link(http_link_info& link_ref, http_link_handle& v_link)
	{
		v_link.internet = InternetOpen(NULL, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);

		v_link.section = InternetConnect(v_link.internet, link_ref.http_host.c_str(), (INTERNET_PORT)link_ref.port, 0, 0, INTERNET_SERVICE_HTTP, 0, 1);
	}

	http_link_handle link_manager::create_http_link(http_link_info& link_ref)
	{
		http_link_handle link;
		link.internet = InternetOpen(NULL, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);

		// create http_link
		link.section = InternetConnect(link.internet,
			link_ref.http_host.c_str(),
			(INTERNET_PORT)link_ref.port,
			0, 0,
			INTERNET_SERVICE_HTTP,
			0, 1);

		return link;
	}

}