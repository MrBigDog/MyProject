///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_time_cost.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : time_cost class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_time_cost.h"
#include <usUtil/us_string_ext.h>

namespace uniscope_globe
{
	time_cost* singleton_time_cost_checker::s_ins = NULL;

	time_cost::time_cost( void )
	{
		m_render_file.file_open( L"C:\\time\\render_time.txt", L"w" );
		m_update_file.file_open( L"C:\\time\\update_time.txt", L"w");
		m_download_file.file_open( L"C:\\time\\download_time.txt", L"w");
	}

	time_cost::~time_cost( void )
	{
		m_render_file.file_close();
		m_update_file.file_close();
		m_download_file.file_close();
	}

	void time_cost::write_render_time()
	{
		string str = string("render: ") + string_ext::from_int( m_render_time ) + "	create: " + string_ext::from_int( m_create_time ) + "	destroy: " + string_ext::from_int( m_destroy_time ) + "\n";
		m_render_file.write( str );
	}

	void time_cost::write_update_time()
	{
		string str = string("update: ") + string_ext::from_int( m_update_time ) + "	reclaim: " + string_ext::from_int( m_reclaim_time ) + "\n";
		m_update_file.write( str );
	}

	void time_cost::write_download_time()
	{
		string str = string("download: ") + string_ext::from_int( m_download_time ) + "	uncompress: " + string_ext::from_int( m_uncompress_time ) + "\n";
		m_download_file.write( str );
	}

	
}