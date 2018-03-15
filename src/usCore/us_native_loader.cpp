///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_native_loader.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : native_loader class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "Stdafx.h"
#include "us_native_loader.h"
#include "us_native_loader.h"
#include <usUtil/us_common_file.h>
#include <usCore/us_link_base.h>
#include <usUtil/us_file_buffer.h>
#include <usUtil/us_file_directory.h>
#include <usCore/us_http_loader.h>

namespace uniscope_globe
{
	int native_loader::load_from_root( link_base* v_link, byte* &data_stream, int& v_size )
	{
		return native_loader::load_from_local( v_link, data_stream, v_size );
	}

	int native_loader::load_from_local( link_base* v_link, byte* &data_stream, int& v_size )
	{
		ustring str_file = v_link->get_link_url();

		if ( file_directory::is_path_file_exist( str_file.c_str() ) == false )
			return LOAD_RESULT_NOF;

		file_buffer v_file;
		if ( !v_file.file_open( str_file.c_str(), L"rb" ) )
			return LOAD_RESULT_ERR;
		
		interior_index file_size = v_file.get_size();
		data_stream = new byte[file_size];

		v_file.read( data_stream, file_size );
		v_size = file_size;

		v_file.file_close();

		return LOAD_RESULT_SUC;
	

		//bool load_result = false;

		//ustring str_file = v_link->get_link_url();

		//HANDLE h_file = CreateFile( str_file.c_str(),
		//	FILE_SHARE_READ,
		//	0,
		//	NULL,
		//	OPEN_EXISTING,
		//	FILE_FLAG_RANDOM_ACCESS | FILE_ATTRIBUTE_NORMAL,
		//	NULL
		//	);

		//DWORD err = 0;
		//if ( h_file == NULL || h_file == INVALID_HANDLE_VALUE )
		//{
		//	err = GetLastError();
		//}
		//else
		//{
		//	DWORD n_cnt = 0, n_low = 0, n_high = 0;
		//	v_size = GetFileSize( h_file, &n_high );

		//	data_stream = new byte[v_size];

		//	ReadFile( h_file, data_stream, v_size, &n_cnt, NULL );

		//	load_result = true;
		//}
		//		
		//AUTO_CLOSE_HANDLE( h_file );
		//
		//return load_result;
	}


	
}