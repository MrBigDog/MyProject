///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_http_loader.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : http_loader class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "Stdafx.h"
#include "us_http_loader.h"
#include <usCore/us_link_http.h>
#include <usUtil/us_common_file.h>
#include <usCore/us_system_environment.h>
#include <usCore/us_link_http.h>
#include <wtypes.h>
#include <wininet.h>

namespace uniscope_globe
{

	int http_loader::load_from_ude(link_http* v_link, byte* &data_stream, int& v_size)
	{
		return http_loader::load_from_http(v_link, data_stream, v_size);
	}

	int http_loader::load_from_uds(link_http* v_link, byte* &data_stream, int& v_size)
	{
		return http_loader::load_from_http(v_link, data_stream, v_size);
	}

	int http_loader::load_from_http(link_http* v_link, byte* &data_stream, int& v_size)
	{///modify by felix ÄÚ´æ¿ØÖÆ
		if (system_environment::s_reclaim_memory_size <= system_environment::s_max_process_memory_size)
			return -1;
		bool load_result = LOAD_RESULT_ERR;

		http_link_handle link = v_link->get_http_link();

		http_link_info link_info = v_link->get_http_link_info();

	Loop:
		// create request
		HINTERNET inet_file = NULL;
		inet_file = HttpOpenRequestW(link.section, L"GET", link_info.link_path.c_str(), NULL, NULL, NULL, INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE, 1);
		if (!inet_file)
		{
			link = v_link->get_http_link_ex();
			inet_file = HttpOpenRequestW(link.section, L"GET", link_info.link_path.c_str(), NULL, NULL, NULL, INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE, 1);
			if (!inet_file)
			{
				return load_result;
			}

			return load_result;
		}

		// send request
		if (!HttpSendRequest(inet_file, NULL, 0, NULL, 0))
		{
			AUTO_CLOSE_INTERNET_HANDLE(inet_file);

			link = v_link->get_http_link_ex();
			inet_file = HttpOpenRequestW(link.section, L"GET", link_info.link_path.c_str(), NULL, NULL, NULL, INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE, 1);
			if (!inet_file)
			{
				return load_result;
			}

			if (!HttpSendRequest(inet_file, NULL, 0, NULL, 0))
				return load_result;
		}

		// set proxy
		DWORD query_code = 0, size = sizeof(DWORD);
		if (HttpQueryInfo(inet_file, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &query_code, &size, NULL) == false)
		{
			AUTO_CLOSE_INTERNET_HANDLE(inet_file);
			return load_result;
		}

		if (query_code == HTTP_STATUS_NOT_FOUND)
		{
			AUTO_CLOSE_INTERNET_HANDLE(inet_file);
			return LOAD_RESULT_NOF;
		}

		if (query_code == HTTP_STATUS_PROXY_AUTH_REQ)
		{
			if (link_http::s_proxy_username.size() || link_http::s_proxy_password.size())
			{
				InternetSetOption(inet_file, INTERNET_OPTION_PROXY_USERNAME, (LPVOID)link_http::s_proxy_username.c_str(), MAX_PS_LENGTH);
				InternetSetOption(inet_file, INTERNET_OPTION_PROXY_PASSWORD, (LPVOID)link_http::s_proxy_password.c_str(), MAX_PS_LENGTH);
			}
			else if (InternetErrorDlg(GetDesktopWindow(), inet_file, ERROR_INTERNET_INCORRECT_PASSWORD,
				FLAGS_ERROR_UI_FILTER_FOR_ERRORS | FLAGS_ERROR_UI_FLAGS_GENERATE_DATA | FLAGS_ERROR_UI_FLAGS_CHANGE_OPTIONS,
				NULL) == ERROR_INTERNET_FORCE_RETRY)
			{
				wchar_t proxy_user[MAX_PS_LENGTH], proxy_password[MAX_PS_LENGTH];
				memset(proxy_user, 0, sizeof(wchar_t) * MAX_PS_LENGTH);
				memset(proxy_password, 0, sizeof(wchar_t) * MAX_PS_LENGTH);

				size = MAX_PS_LENGTH;
				InternetQueryOption(inet_file, INTERNET_OPTION_PROXY_USERNAME, proxy_user, &size);

				size = MAX_PS_LENGTH;
				InternetQueryOption(inet_file, INTERNET_OPTION_PROXY_PASSWORD, proxy_password, &size);

				link_http::s_proxy_username = proxy_user;
				link_http::s_proxy_password = proxy_password;

				InternetCloseHandle(inet_file);
				inet_file = NULL;

				goto Loop;
			}
		}

		bool wmsflag = false;
		if (link_info.link_path.find(L"MapServer") != link_info.link_path.npos)
			wmsflag = true;
		// query size
		query_code = 0;
		size = sizeof(DWORD);
		if (HttpQueryInfo(inet_file, HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER, &query_code, &size, NULL) == false && !wmsflag)
		{
			AUTO_CLOSE_INTERNET_HANDLE(inet_file);
			return LOAD_RESULT_NOF;
		}
		else if (wmsflag)
			query_code = 1024 * 100;

		v_size = query_code;
		data_stream = new byte[v_size];
		memset(data_stream, 0, v_size);

		int v_cursor = 0;
		bool break_out = false;
		while (!break_out)
		{
			DWORD byte_count = 0, tmp_count = 0;

			if (!InternetQueryDataAvailable(inet_file, &byte_count, 0, 0))
				break;

			BYTE* p_buffer = data_stream + v_cursor;
			if (!InternetReadFile(inet_file, p_buffer, byte_count, &tmp_count))
			{
				break;
			}

			if (!tmp_count)
			{
				break;
			}

			v_cursor += byte_count;
		}

		// check data
		if (!break_out)
		{
			// check data
			if (memcmp(data_stream, "<!DOCTYPE", TEST_BYTE_COUNT) != 0)
			{
				load_result = LOAD_RESULT_SUC;
			}
			else
			{
				v_size = 0;
				AUTO_DELETE(data_stream);
			}
		}
		else
		{
			v_size = 0;
			AUTO_DELETE(data_stream);
		}

		AUTO_CLOSE_INTERNET_HANDLE(inet_file);

		return load_result;
	}


}