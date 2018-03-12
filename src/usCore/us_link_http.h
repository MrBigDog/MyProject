///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_link_http.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : link_http class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_LINK_HTTP_H_
#define _US_LINK_HTTP_H_
#include <usCore\Export.h>
namespace uniscope_globe
{
	class http_link_desc
	{
	public:
		int port;
		wstring host;
		wstring path;

	public:
		http_link_desc()
		{
			port = 80;
		}
	};

	class http_link_handle
	{
	public:
		HINTERNET internet;
		HINTERNET section;

	public:
		http_link_handle()
		{
			internet = 0;
			section = 0;
		}
	};

	struct http_link_info
	{
		ulong	port;
		ustring	http_host;
		ustring link_path;
		http_link_info()
		{
			port = 80;
		}
	};

	class USCORE_EXPORT link_http : public link_base
	{
	private:
		link_http(void) {}

	public:
		link_http(const LPCTSTR url, int type);

		virtual ~link_http(void);

	public:
		http_link_handle get_http_link(void);

		http_link_handle get_http_link_ex(void);

		http_link_info	get_http_link_info(void);

		static void get_http_link_info(const LPCTSTR str_url, http_link_info& link_info);

	private:
		void			parser_url(const  LPCTSTR str_url);

	public:
		// proxy user
		static ustring s_proxy_username;
		static ustring s_proxy_password;

	private:
		http_link_info	m_link_info;
	};

#define AUTO_CLOSE_INTERNET_HANDLE( handle ) if ( handle ){ InternetCloseHandle(handle ); handle = NULL; }
}

#endif // _US_LINK_HTTP_H_