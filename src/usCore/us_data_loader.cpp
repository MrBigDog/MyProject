///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_data_loader.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : data_loader class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_data_loader.h"

namespace uniscope_globe
{
	data_loader::data_loader( void )
	{
		m_data = new raw_buffer();
	}

	data_loader::~data_loader( void )
	{
		clear();
		AUTO_DELETE( m_data );
	}

	///如果是post模式必须要设置数据流（a-1&b=2&...）
	///add by felix
	string data_loader::post_data_stream_;
	void data_loader::set_data_stream(const string& data)
	{
		//memcpy(&post_data_stream_[0],data,);
		post_data_stream_ = data;
	}

	void data_loader::clear()
	{
		byte* v_buf = m_data->get_buffer();
		AUTO_DELETE( v_buf );
	}

	int data_loader::get_link_protocol( const LPCTSTR str_url )
	{
		wstring str_link_url = str_url;

		ustring::size_type v_pos = str_link_url.find( L"://" );
		if ( v_pos == ustring::npos )
		{
			v_pos = str_link_url.find( L":\\" );
			if ( v_pos > 0 )
			{
				return US_LINK_TYPE_LOCAL;
			}
		}
		else
		{
			ustring link_type = str_link_url.substr( 0, v_pos );
			if ( link_type == L"db" )
			{
				return US_LINK_TYPE_UDE;
			}
			else if ( link_type == L"uds" )
			{
				return US_LINK_TYPE_UDS;
			}
			else if ( link_type == L"root" )
			{
				return US_LINK_TYPE_ROOT;
			}
			else if ( link_type == L"http" )
			{
				return US_LINK_TYPE_HTTP;
			}
			else if ( link_type == L"wmts" )
			{
				return US_LINK_TYPE_WMTS;
			}
		}

		return US_LINK_TYPE_HTTP;
	}

	ustring data_loader::get_http_host( const LPCTSTR str_url )
	{
		ustring string_url = str_url;
		int url_len = (int)string_url.length();

		ustring::size_type pos = string_url.find( L"://" );
		pos += 3;

		ustring::size_type next_pos = string_url.find( L"/" , pos );
		if ( next_pos != ustring::npos )
		{
			string_url = string_url.substr( pos, next_pos - pos );
		}

		pos = string_url.find( L":", 0 );
		if ( pos == ustring::npos )
		{
			return string_url;
		}
				
		return string_url.substr( 0, pos );
	}

	int data_loader::get_http_port( const LPCTSTR str_url )
	{
		ustring string_url = str_url;
		int url_len = (int)string_url.length();

		ustring::size_type pos = string_url.find( L"://" );
		pos += 3;

		ustring::size_type next_pos = string_url.find( L"/" , pos );
		if ( next_pos != ustring::npos )
		{
			string_url = string_url.substr( pos, next_pos - pos );
		}

		pos = string_url.find( L":", 0 );
		if ( pos == ustring::npos )
		{
			return 80;
		}

		ustring str_port = string_url.substr( pos + 1 , string_url.size() );
		return _wtol( str_port.c_str() );
	}

	ustring data_loader::get_http_path( const LPCTSTR str_url )
	{
		ustring string_url = str_url;
		int url_len = (int)string_url.length();

		ustring::size_type pos = string_url.find( L"://" );
		pos += 3;

		ustring::size_type next_pos = string_url.find( L"/" , pos );
		if ( next_pos == ustring::npos )
		{
			return L"";
		}
		
		return string_url.substr( next_pos+1, string_url.size() );
	}

	bool data_loader::split_http_link( const LPCTSTR str_url, int& protocol, int& port, ustring& host, ustring& path )
	{
		ustring string_url = str_url;
		int url_len = (int)string_url.length();

		ustring::size_type pos = string_url.find( L"://" );
		ustring str_protocol = string_url.substr( 0, pos );

		if ( str_protocol == L"http" )
		{
			protocol = US_LINK_TYPE_HTTP;
		}
		else if ( str_protocol == L"db" )
		{
			protocol = US_LINK_TYPE_UDE;
		}
		else if ( str_protocol == L"uds" )
		{
			protocol = US_LINK_TYPE_UDS;
		}
		else if ( str_protocol == L"wmts" )
		{
			protocol = US_LINK_TYPE_WMTS;
		}
		else
		{
			return false;
		}

		pos += 3;
		ustring::size_type next_pos = string_url.find( L"/" , pos );
		ustring	str_conf = string_url.substr( pos, next_pos - pos );

		path = string_url.substr( next_pos + 1, url_len - next_pos - 1 );

		int conf_len = (int)str_conf.length();
		pos = (int)str_conf.find( L":" );

		if ( pos == ustring::npos )
		{
			host = str_conf;
			port = 80;
		}
		else
		{
			host = str_conf.substr( 0, pos );

			ustring str_port = str_conf.substr( pos + 1 , conf_len - pos - 1 );
			port = _wtol( str_port.c_str() );
		}

		return true;
	}


	void data_loader::create_http_desc( IN LPCTSTR str_url, OUT http_link_desc& v_desc )
	{
		int v_protocol = US_LINK_TYPE_HTTP;
		data_loader::split_http_link( str_url, v_protocol, v_desc.port, v_desc.host, v_desc.path );
	}

	void data_loader::create_http_handle( IN http_link_desc& v_link_desc, OUT http_link_handle& v_link_handle )
	{
		v_link_handle.internet = InternetOpen( NULL, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0 );

		v_link_handle.section = InternetConnect( v_link_handle.internet, v_link_desc.host.c_str(), v_link_desc.port, 0, 0, INTERNET_SERVICE_HTTP, 0, 1 );
	}

	//modefy by felix add to mode parameter
	bool data_loader::get_data_from_http( IN http_link_desc& v_link_desc, IN int server_type, IN LPCTSTR mode, OUT byte* &v_buf, OUT int &v_size  )
	{
		ustring v_proxy_username, v_proxy_password;
		wstring str_mode = std::move(mode);

		http_link_handle v_link_handle;
		data_loader::create_http_handle( v_link_desc, v_link_handle );

Loop:
		// create request
		HINTERNET inet_file	= NULL;
		inet_file = HttpOpenRequestW( v_link_handle.section,/* L"POST"*/str_mode.c_str(), v_link_desc.path.c_str(), NULL, NULL, NULL, INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE, 1 );
		if ( !inet_file )
			return false;


		///区分get与post
		// send request
		if ( L"GET" == str_mode && !HttpSendRequest( inet_file, NULL, 0, NULL, 0 ) )
		{
			AUTO_CLOSE_INTERNET_HANDLE( inet_file );
			return false;
		}
		else if ( L"POST" == str_mode )
		{
			const wstring hdrs = L"Content-Type: application/x-www-form-urlencoded";
			// "Context-Length: 11"; // multipart/form-data. 请为二进制数据使用multipart编码

			BOOL bRequest = HttpSendRequest(inet_file, hdrs.c_str(), (DWORD)hdrs.length(), (LPVOID)post_data_stream_.c_str(), (DWORD)post_data_stream_.length()); //发送http请求
			if ( !bRequest )
			{
				AUTO_CLOSE_INTERNET_HANDLE( inet_file );
				return false;
			}
		}

		// set proxy
		DWORD query_code = 0, size = 0;
		size = sizeof( query_code ) ;
		if ( HttpQueryInfo( inet_file, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &query_code, &size, NULL )
			&& query_code == HTTP_STATUS_PROXY_AUTH_REQ )
		{
			if ( v_proxy_username.size() || v_proxy_password.size() )
			{
				InternetSetOption( inet_file, INTERNET_OPTION_PROXY_USERNAME, (LPVOID)v_proxy_username.c_str(), MAX_PS_LENGTH );
				InternetSetOption( inet_file, INTERNET_OPTION_PROXY_PASSWORD, (LPVOID)v_proxy_password.c_str(), MAX_PS_LENGTH );
			}
			else if ( InternetErrorDlg( GetDesktopWindow(), inet_file, ERROR_INTERNET_INCORRECT_PASSWORD,
				FLAGS_ERROR_UI_FILTER_FOR_ERRORS | FLAGS_ERROR_UI_FLAGS_GENERATE_DATA | FLAGS_ERROR_UI_FLAGS_CHANGE_OPTIONS, 
				NULL ) == ERROR_INTERNET_FORCE_RETRY )
			{
				wchar_t proxy_user[MAX_PS_LENGTH], proxy_password[MAX_PS_LENGTH];
				memset( proxy_user, 0, sizeof(wchar_t) * MAX_PS_LENGTH );
				memset( proxy_password, 0, sizeof(wchar_t) * MAX_PS_LENGTH );

				size = MAX_PS_LENGTH;
				InternetQueryOption( inet_file, INTERNET_OPTION_PROXY_USERNAME, proxy_user, &size );

				size = MAX_PS_LENGTH;
				InternetQueryOption( inet_file, INTERNET_OPTION_PROXY_PASSWORD, proxy_password, &size );

				v_proxy_username = proxy_user;
				v_proxy_password = proxy_password;

				InternetCloseHandle( inet_file );
				inet_file = NULL;

				goto Loop;
			}
		}

		// 如果是默认模式，就逐一获取
		bool rslt = false;
		if ( server_type == US_SERVER_TYPE_DEFAULT )
		{
			managed_buffer data_stream;
			while ( true )
			{
				DWORD byte_count = 0, tmp_count = 0;

				if ( !InternetQueryDataAvailable( inet_file, &byte_count, 0, 0 ) )
					break;

				BYTE* p_buffer = new BYTE[byte_count];
				if ( !InternetReadFile( inet_file, p_buffer, byte_count, &tmp_count ) )
				{
					AUTO_DELETE( p_buffer );
					break;
				}

				if ( !tmp_count )
				{
					AUTO_DELETE( p_buffer );
					break;
				}

				data_stream.write( (void*)p_buffer, byte_count );
				AUTO_DELETE( p_buffer );
			}

			// check data
			if (  data_stream.get_size() > 0 && memcmp( data_stream.get_buffer(), "<!DOCTYPE", TEST_BYTE_COUNT ) != 0 )
			{
				rslt = true;

				v_size = data_stream.get_size();
				v_buf = new BYTE[data_stream.get_size()];
				memcpy( v_buf, data_stream.get_buffer(), data_stream.get_size() );
			}

			data_stream.clear();
		}
		// 如果是文件模式，就直接获取文件大小
		else
		{
			// query size
			query_code = 0;
			size = sizeof( DWORD );
			if ( HttpQueryInfo( inet_file, HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER, &query_code, &size, NULL ) == false )
			{
				AUTO_CLOSE_INTERNET_HANDLE( inet_file );
				return false;
			}

			v_size = query_code;
			v_buf = new byte[v_size];

			int v_cursor = 0;
			while ( true )
			{
				DWORD byte_count = 0, tmp_count = 0;

				if ( !InternetQueryDataAvailable( inet_file, &byte_count, 0, 0 ) )
					break;

				BYTE* p_buffer = v_buf + v_cursor;
				if ( !InternetReadFile( inet_file, p_buffer, byte_count, &tmp_count ) )
				{
					break;
				}

				if ( !tmp_count )
				{
					break;
				}

				v_cursor += byte_count;
			}

			bool rslt = false;

			// check data
			if ( memcmp( v_buf, "<!DOCTYPE", TEST_BYTE_COUNT ) != 0 )
			{
				rslt = true;
			}
			else
			{
				AUTO_DELETE( v_buf );
			}
		}

		AUTO_CLOSE_INTERNET_HANDLE( inet_file );

		return rslt;
	}

	bool data_loader::get_data_from_local( IN LPCTSTR str_url, OUT byte* &v_buf, OUT int& v_size )
	{
		HANDLE h_file = CreateFile( str_url,
			GENERIC_READ,
			0,
			NULL,
			OPEN_EXISTING,
			FILE_FLAG_RANDOM_ACCESS | FILE_ATTRIBUTE_NORMAL,
			NULL
			);

		if ( h_file == NULL || h_file == INVALID_HANDLE_VALUE )
		{
			///-----------felix pdb------------------
			DWORD kk =  GetLastError();
			wstring str = str_url;
			str = str + L"__" + string_ext::wfrom_int(kk);
			//MessageBox( NULL, str.c_str(), L"ee", MB_OK );
			///-------------------------------------------
			return false;
		}

		DWORD n_cnt = 0, n_low = 0, n_high = 0;
		n_low = GetFileSize( h_file, &n_high );

		v_size = n_low;
		v_buf = new byte[n_low];
		ReadFile( h_file, v_buf, n_low, &n_cnt, NULL );

		AUTO_CLOSE_HANDLE( h_file );

		return true;
	}

	///@brief str_url get模式下是http://www.baidu.com/test?index=1&image=test.png
	///               post模式下是http://www.baidu.com/test
	raw_buffer* data_loader::load( LPCTSTR str_url, int server_type, LPCTSTR http_mode )
	{
		int v_url_type = data_loader::get_link_protocol( str_url );

		int v_size = 0;
		byte* v_buf = NULL;
		switch( v_url_type )
		{
		case US_LINK_TYPE_HTTP:
			{
				http_link_desc http_desc;
				data_loader::create_http_desc( str_url, http_desc );

				get_data_from_http( http_desc, server_type, http_mode, v_buf, v_size );
			}
			break;
		case US_LINK_TYPE_UDE:
			{
				http_link_desc http_desc;
				data_loader::create_http_desc( str_url, http_desc );
				http_desc.path += L"_ude";

				get_data_from_http( http_desc, server_type, http_mode, v_buf, v_size  );
			}
			break;
		case US_LINK_TYPE_UDS:
			{
				http_link_desc http_desc;
				data_loader::create_http_desc( str_url, http_desc );
				
				get_data_from_http( http_desc, server_type, http_mode, v_buf, v_size  );
			}
			break;
		case US_LINK_TYPE_WMTS:
			{
				http_link_desc http_desc;
				data_loader::create_http_desc( str_url, http_desc );

				get_data_from_http( http_desc, server_type, http_mode, v_buf, v_size  );
			}
			break;
		case US_LINK_TYPE_ROOT:
			{
				ustring str_link = str_url;
				str_link = singleton_system_environment::instance().m_root_path + str_link.substr( 7, str_link.size() - 7 );
				
				get_data_from_local( str_link.c_str(), v_buf, v_size  );
			}
			break;
		case US_LINK_TYPE_LOCAL:
			{
				get_data_from_local( str_url, v_buf, v_size  );
			}
			break;
		default:
			{
				//MessageBox( NULL, L"tt1", L"ee", MB_OK );
			}
			break;
		}
		if( v_size == 0)
			return 0;

		m_data->attach( v_buf, v_size );

		return m_data;
	}

//
//
//	raw_buffer* data_loader::load( LPCTSTR link_url )
//	{
//		clear();
//
//		ustring str_link_url = link_url;
//		int url_len = (int)str_link_url.length();
//
//		byte* v_buf = NULL;
//		int v_size = 0;
//
//		int	npos = (int)str_link_url.find( L"://" );
//		if ( npos == -1 )
//		{			
//			if ( load_from_local( link_url, v_buf, v_size ) )
//			{
//				m_data->attach( v_buf, v_size );
//				return m_data;
//			}
//			else
//			{
//				return NULL;
//			}
//		}
//		else
//		{
//			npos += 3;
//			int next_pos = (int)str_link_url.find( L"/" , npos );
//			ustring	link_conf = str_link_url.substr( npos, next_pos - npos );
//			ustring link_path = str_link_url.substr( next_pos + 1, url_len - next_pos - 1 );
//
//			int port = 80;
//			ustring http_host = link_conf;
//
//			int conf_len = (int)link_conf.length();
//			npos = (int)link_conf.find( L":" );	
//			if ( npos > 0 )
//			{
//				http_host = link_conf.substr( 0, npos );
//
//				ustring str_port = link_conf.substr( npos + 1 , conf_len - npos - 1 );
//				port = _wtol( str_port.c_str() );
//			}
//			
//			http_link_handle v_link;
//			create_http_link( http_host.c_str(), port, v_link );
//
//			if ( load_from_http( v_link, link_path.c_str(), v_buf, v_size ) )
//			{
//				m_data->attach( v_buf, v_size );
//				AUTO_CLOSE_INTERNET_HANDLE( v_link.section );
//				AUTO_CLOSE_INTERNET_HANDLE( v_link.internet );
//				return m_data;
//			}
//			else
//			{
//				AUTO_CLOSE_INTERNET_HANDLE( v_link.section );
//				AUTO_CLOSE_INTERNET_HANDLE( v_link.internet );
//				return NULL;
//			}
//		}
//		
//		return NULL;
//	}
//
//	raw_buffer* data_loader::load_data( LPCTSTR link_url )
//	{
//		m_data->clear();
//
//		ustring str_link_url = link_url;
//		int url_len = str_link_url.length();
//
//		byte* v_buf = NULL;
//		int v_size = 0;
//
//		int	npos = (int)str_link_url.find( L"://" );
//		if ( npos == -1 )
//		{			
//			if ( load_from_local( link_url, v_buf, v_size ) )
//			{
//				m_data->attach( v_buf, v_size );
//				return m_data;
//			}
//			else
//			{
//				AUTO_DELETE( m_data );
//				return NULL;
//			}
//			return NULL;
//		}
//		else
//		{
//			npos += 3;
//			int next_pos = (int)str_link_url.find( L"/" , npos );
//			ustring	link_conf = str_link_url.substr( npos, next_pos - npos );
//			ustring link_path = str_link_url.substr( next_pos + 1, url_len - next_pos - 1 );
//
//			int port = 80;
//			ustring http_host = link_conf;
//
//			int conf_len = (int)link_conf.length();
//			npos = (int)link_conf.find( L":" );	
//			if ( npos > 0 )
//			{
//				http_host = link_conf.substr( 0, npos );
//
//				ustring str_port = link_conf.substr( npos + 1 , conf_len - npos - 1 );
//				port = _wtol( str_port.c_str() );
//			}
//
//			http_link_handle v_link;
//			create_http_link( http_host.c_str(), port, v_link );
//
//			if ( get_data_from_http( v_link, link_path.c_str(), v_buf, v_size ) )
//			{
//				m_data->attach( v_buf, v_size );
//				AUTO_CLOSE_INTERNET_HANDLE( v_link.section );
//				AUTO_CLOSE_INTERNET_HANDLE( v_link.internet );
//				return m_data;
//			}
//			else
//			{
//				AUTO_CLOSE_INTERNET_HANDLE( v_link.section );
//				AUTO_CLOSE_INTERNET_HANDLE( v_link.internet );
//				return NULL;
//			}
//		}
//
//		return NULL;
//	}
//
//
//	bool data_loader::load_from_local( LPCTSTR link_path, byte* &v_buf, int& v_size )
//	{
//		bool load_result = false;
//		
//		HANDLE h_file = CreateFile( link_path,
//			GENERIC_READ,
//			0,
//			NULL,
//			OPEN_EXISTING,
//			FILE_FLAG_RANDOM_ACCESS | FILE_ATTRIBUTE_NORMAL,
//			NULL
//			);
//
//		if ( h_file == NULL || h_file == INVALID_HANDLE_VALUE )
//		{
//
//		}
//		else
//		{
//			DWORD n_cnt = 0, n_low = 0, n_high = 0;
//			v_size = GetFileSize( h_file, &n_high );
//
//			v_buf = new byte[v_size];
//			
//			ReadFile( h_file, v_buf, v_size, &n_cnt, NULL );
//			
//			load_result = true;
//		}
//
//		AUTO_CLOSE_HANDLE( h_file );
//
//		return load_result;
//	}
//
//	void data_loader::create_http_link( LPCTSTR host, WORD port, http_link_handle& v_link )
//	{		
//		v_link.internet = InternetOpen( NULL, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0 );
//
//		v_link.section = InternetConnect( v_link.internet, host, port, 0, 0, INTERNET_SERVICE_HTTP, 0, 1 );
//	}
//
//	bool data_loader::load_from_http( http_link_handle& v_link, LPCTSTR link_path, byte* &v_buf, int& v_size )
//	{
//		ustring v_proxy_username, v_proxy_password;
//
//Loop:
//		// create request
//		HINTERNET inet_file	= NULL;
//		inet_file = HttpOpenRequestW( v_link.section, L"GET", link_path, NULL, NULL, NULL, INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE, 1 );
//		if ( !inet_file )
//			return false;
//
//
//		// send request
//		if ( !HttpSendRequest( inet_file, NULL, 0, NULL, 0 ) )
//		{
//			AUTO_CLOSE_INTERNET_HANDLE( inet_file );
//			return false;
//		}
//
//		// set proxy
//		DWORD query_code = 0, size = 0;
//		size = sizeof( query_code ) ;
//		if ( HttpQueryInfo( inet_file, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &query_code, &size, NULL )
//			&& query_code == HTTP_STATUS_PROXY_AUTH_REQ )
//		{
//			if ( v_proxy_username.size() || v_proxy_password.size() )
//			{
//				InternetSetOption( inet_file, INTERNET_OPTION_PROXY_USERNAME, (LPVOID)v_proxy_username.c_str(), MAX_PS_LENGTH );
//				InternetSetOption( inet_file, INTERNET_OPTION_PROXY_PASSWORD, (LPVOID)v_proxy_password.c_str(), MAX_PS_LENGTH );
//			}
//			else if ( InternetErrorDlg( GetDesktopWindow(), inet_file, ERROR_INTERNET_INCORRECT_PASSWORD,
//				FLAGS_ERROR_UI_FILTER_FOR_ERRORS | FLAGS_ERROR_UI_FLAGS_GENERATE_DATA | FLAGS_ERROR_UI_FLAGS_CHANGE_OPTIONS, 
//				NULL ) == ERROR_INTERNET_FORCE_RETRY )
//			{
//				wchar_t proxy_user[MAX_PS_LENGTH], proxy_password[MAX_PS_LENGTH];
//				memset( proxy_user, 0, sizeof(wchar_t) * MAX_PS_LENGTH );
//				memset( proxy_password, 0, sizeof(wchar_t) * MAX_PS_LENGTH );
//
//				size = MAX_PS_LENGTH;
//				InternetQueryOption( inet_file, INTERNET_OPTION_PROXY_USERNAME, proxy_user, &size );
//
//				size = MAX_PS_LENGTH;
//				InternetQueryOption( inet_file, INTERNET_OPTION_PROXY_PASSWORD, proxy_password, &size );
//
//				v_proxy_username = proxy_user;
//				v_proxy_password = proxy_password;
//
//				InternetCloseHandle( inet_file );
//				inet_file = NULL;
//
//				goto Loop;
//			}
//		}
//
//		// query size
//		query_code = 0;
//		size = sizeof( DWORD );
//		if ( HttpQueryInfo( inet_file, HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER, &query_code, &size, NULL ) == false )
//		{
//			AUTO_CLOSE_INTERNET_HANDLE( inet_file );
//			return false;
//		}
//
//		v_size = query_code;
//		v_buf = new byte[v_size];
//
//		int v_cursor = 0;
//		while ( true )
//		{
//			DWORD byte_count = 0, tmp_count = 0;
//
//			if ( !InternetQueryDataAvailable( inet_file, &byte_count, 0, 0 ) )
//				break;
//
//			BYTE* p_buffer = v_buf + v_cursor;
//			if ( !InternetReadFile( inet_file, p_buffer, byte_count, &tmp_count ) )
//			{
//				break;
//			}
//
//			if ( !tmp_count )
//			{
//				break;
//			}
//
//			v_cursor += byte_count;
//		}
//
//		bool rslt = false;
//		
//		// check data
//		if ( memcmp( v_buf, "<!DOCTYPE", TEST_BYTE_COUNT ) != 0 )
//		{
//			rslt = true;
//		}
//		else
//		{
//			AUTO_DELETE( v_buf );
//		}
//		
//		AUTO_CLOSE_INTERNET_HANDLE( inet_file );
//		//AUTO_CLOSE_INTERNET_HANDLE( v_link.section );
//		//AUTO_CLOSE_INTERNET_HANDLE( v_link.internet );
//				
//		return rslt;
//	}
//
//	bool data_loader::get_data_from_http( http_link_handle& v_link, LPCTSTR link_path,  byte* &v_buf, int& v_size  )
//	{
//		ustring v_proxy_username, v_proxy_password;
//
//Loop:
//		// create request
//		HINTERNET inet_file	= NULL;
//		inet_file = HttpOpenRequestW( v_link.section, L"GET", link_path, NULL, NULL, NULL, INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE, 1 );
//		if ( !inet_file )
//			return false;
//
//
//		// send request
//		if ( !HttpSendRequest( inet_file, NULL, 0, NULL, 0 ) )
//		{
//			AUTO_CLOSE_INTERNET_HANDLE( inet_file );
//			return false;
//		}
//
//		// set proxy
//		DWORD query_code = 0, size = 0;
//		size = sizeof( query_code ) ;
//		if ( HttpQueryInfo( inet_file, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &query_code, &size, NULL )
//			&& query_code == HTTP_STATUS_PROXY_AUTH_REQ )
//		{
//			if ( v_proxy_username.size() || v_proxy_password.size() )
//			{
//				InternetSetOption( inet_file, INTERNET_OPTION_PROXY_USERNAME, (LPVOID)v_proxy_username.c_str(), MAX_PS_LENGTH );
//				InternetSetOption( inet_file, INTERNET_OPTION_PROXY_PASSWORD, (LPVOID)v_proxy_password.c_str(), MAX_PS_LENGTH );
//			}
//			else if ( InternetErrorDlg( GetDesktopWindow(), inet_file, ERROR_INTERNET_INCORRECT_PASSWORD,
//				FLAGS_ERROR_UI_FILTER_FOR_ERRORS | FLAGS_ERROR_UI_FLAGS_GENERATE_DATA | FLAGS_ERROR_UI_FLAGS_CHANGE_OPTIONS, 
//				NULL ) == ERROR_INTERNET_FORCE_RETRY )
//			{
//				wchar_t proxy_user[MAX_PS_LENGTH], proxy_password[MAX_PS_LENGTH];
//				memset( proxy_user, 0, sizeof(wchar_t) * MAX_PS_LENGTH );
//				memset( proxy_password, 0, sizeof(wchar_t) * MAX_PS_LENGTH );
//
//				size = MAX_PS_LENGTH;
//				InternetQueryOption( inet_file, INTERNET_OPTION_PROXY_USERNAME, proxy_user, &size );
//
//				size = MAX_PS_LENGTH;
//				InternetQueryOption( inet_file, INTERNET_OPTION_PROXY_PASSWORD, proxy_password, &size );
//
//				v_proxy_username = proxy_user;
//				v_proxy_password = proxy_password;
//
//				InternetCloseHandle( inet_file );
//				inet_file = NULL;
//
//				goto Loop;
//			}
//		}
//
//		bool break_out = false;
//		managed_buffer data_stream;
//		while ( !break_out )
//		{
//			DWORD byte_count = 0, tmp_count = 0;
//
//			if ( !InternetQueryDataAvailable( inet_file, &byte_count, 0, 0 ) )
//				break;
//
//			BYTE* p_buffer = new BYTE[byte_count];
//			if ( !InternetReadFile( inet_file, p_buffer, byte_count, &tmp_count ) )
//			{
//				AUTO_DELETE( p_buffer );
//				break;
//			}
//
//			if ( !tmp_count )
//			{
//				AUTO_DELETE( p_buffer );
//				break;
//			}
//
//			data_stream.write( (void*)p_buffer, byte_count );
//			AUTO_DELETE( p_buffer );
//		}
//
//		bool rslt = false;
//		// check data
//		if ( !break_out )
//		{
//			// check data
//			if ( memcmp( data_stream.get_buffer(), "<!DOCTYPE", TEST_BYTE_COUNT ) != 0 )
//			{
//				rslt = true;
//
//				v_size = data_stream.get_size();
//				v_buf = new BYTE[data_stream.get_size()];
//				memcpy( v_buf, data_stream.get_buffer(), data_stream.get_size() );
//			}
//			else
//			{
//				data_stream.clear();
//			}
//		}
//
//		data_stream.clear();
//		AUTO_CLOSE_INTERNET_HANDLE( inet_file );
//		//AUTO_CLOSE_INTERNET_HANDLE( v_link.internet );
//		//AUTO_CLOSE_INTERNET_HANDLE( v_link.section );
//
//		return rslt;
//	}

	
}