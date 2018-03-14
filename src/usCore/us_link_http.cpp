///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_link_http.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : link_http class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "Stdafx.h"
#include "us_link_http.h"
#include <usCore/us_link_manager.h>
#include <usUtil/us_string_ext.h>

namespace uniscope_globe
{
	ustring link_http::s_proxy_username;
	ustring link_http::s_proxy_password;

	link_http::link_http(const LPCTSTR url, int type)
		: link_base(url, type)
	{
		parser_url(url);
	}

	link_http::~link_http(void)
	{
	}

	void link_http::parser_url(const LPCTSTR str_url)
	{
		ustring string_url = str_url;
		int url_len = (int)string_url.length();

		int pos = (int)string_url.find(/*L*/"//");
		pos += 2;
		int next_pos = (int)string_url.find(/*L*/"/", pos);
		ustring	str_conf = string_url.substr(pos, next_pos - pos);

		m_link_info.link_path = string_url.substr(next_pos + 1, url_len - next_pos - 1);

		int conf_len = (int)str_conf.length();
		pos = (int)str_conf.find(/*L*/":");

		if (pos == -1)
		{
			m_link_info.http_host = str_conf;
			m_link_info.port = 80;
		}
		else
		{
			m_link_info.http_host = str_conf.substr(0, pos);

			ustring str_port = str_conf.substr(pos + 1, conf_len - pos - 1);
			m_link_info.port = _wtol(string_ext::to_wstring(str_port).c_str());
		}

		if (m_link_type == US_LINK_TYPE_UDE)
		{
			m_link_info.link_path += /*L*/"_ude";

			std::replace(m_link_info.link_path.begin(), m_link_info.link_path.end(), L'\\', L'/');
		}
		else if (m_link_type == US_LINK_TYPE_UDS)
		{
			std::replace(m_link_info.link_path.begin(), m_link_info.link_path.end(), L'\\', L'/');
		}
		else if (m_link_type == US_LINK_TYPE_WMTS)
		{
			std::replace(m_link_info.link_path.begin(), m_link_info.link_path.end(), L'\\', L'/');
		}
	}

	http_link_handle link_http::get_http_link(void)
	{
		return m_link_mngr->get_http_link(m_link_info);
	}

	http_link_handle link_http::get_http_link_ex(void)
	{
		return m_link_mngr->get_http_link_ex(m_link_info);
	}

	http_link_info	link_http::get_http_link_info(void)
	{
		return m_link_info;
	}

	void link_http::get_http_link_info(const LPCTSTR str_url, http_link_info& link_info)
	{
		ustring string_url = str_url;
		int url_len = (int)string_url.length();

		int pos = (int)string_url.find(/*L*/"//");
		pos += 2;
		int next_pos = (int)string_url.find(/*L*/"/", pos);
		ustring	str_conf = string_url.substr(pos, next_pos - pos);

		link_info.link_path = string_url.substr(next_pos + 1, url_len - next_pos - 1);

		int conf_len = (int)str_conf.length();
		pos = (int)str_conf.find(/*L*/":");

		if (pos == -1)
		{
			link_info.http_host = str_conf;
			link_info.port = 80;
		}
		else
		{
			link_info.http_host = str_conf.substr(0, pos);

			ustring str_port = str_conf.substr(pos + 1, conf_len - pos - 1);
			link_info.port = _wtol(string_ext::to_wstring(str_port).c_str());
		}
	}


}