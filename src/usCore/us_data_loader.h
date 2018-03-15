///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_data_loader.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : data_loader class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_DATA_LOADER_H_
#define _US_DATA_LOADER_H_

#include <usCore\Export.h>
#include <usUtil\us_common_file.h>
#include <wtypes.h>
#include <string>
#include <minwindef.h>

using namespace std;

namespace uniscope_globe
{

#define US_SERVER_TYPE_DEFAULT		0
#define US_SERVER_TYPE_FILE			1

	class raw_buffer;
	class http_link_desc;
	class http_link_handle;

	class USCORE_EXPORT data_loader
	{
	public:
		data_loader(void);

		virtual ~data_loader(void);

	public:
		static int get_link_protocol(const LPCTSTR str_url);

		static ustring get_http_host(const LPCTSTR str_url);

		static int get_http_port(const LPCTSTR str_url);

		static ustring get_http_path(const LPCTSTR str_url);

		static bool split_http_link(const LPCTSTR str_url, int& protocol, int& port, ustring& host, ustring& path);

		static void set_data_stream(const string& data);///<用于http的POST数据传输
	protected:
		static void create_http_desc(IN LPCTSTR str_url, OUT http_link_desc& v_desc);

		static void create_http_handle(IN http_link_desc& v_link_desc, OUT http_link_handle& v_link_handle);

		static bool get_data_from_http(IN http_link_desc& v_link_desc, IN int server_type, IN LPCTSTR mode, OUT byte* &v_buf, OUT int &v_size);

		static bool get_data_from_local(IN LPCTSTR str_url, OUT byte* &v_buf, OUT int &v_size);

	public:
		raw_buffer* load(LPCTSTR str_url, int server_type = US_SERVER_TYPE_DEFAULT, LPCTSTR http_mode = /*L*/"GET");
	protected:
		void clear();

		//public:
		//	raw_buffer* load( LPCTSTR str_url );

		//	raw_buffer* load_data( LPCTSTR str_conn );

		//private:
		//	void create_http_link( LPCTSTR host, WORD port, http_link_handle& v_link );

		//	bool load_from_http( http_link_handle& v_link, LPCTSTR link_path, byte* &v_buf, int& v_size );

		//	bool get_data_from_http( http_link_handle& v_link, LPCTSTR link_path, byte* &v_buf, int& v_size );

		//private:
		//	bool load_from_local( LPCTSTR link_path, byte* &v_buf, int& v_size ); 

	private:
		raw_buffer* m_data;
		static string post_data_stream_;
	};
}

#endif // _US_DATA_LOADER_H_ 