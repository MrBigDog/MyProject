///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_usb_filter.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : usb_filter class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_usb_filter.h"

namespace uniscope_globe
{
	object_base* usb_filter::parse( raw_buffer* v_buffer )
	{
		building_create_buffer* in_buffer = (building_create_buffer*)v_buffer;

		// new one
		byte* v_src_data = in_buffer->get_buffer();
		ulong v_src_size = in_buffer->get_size();
		//in_buffer->seek_to_begin();

		bldg_builder_head v_head( 0, 0 );
		in_buffer->read( &v_head, sizeof(v_head) );

		ulong v_data_size = 0;
		in_buffer->read( &v_data_size, sizeof(v_data_size) );

		ulong v_compress_data_offset = sizeof(v_head) + sizeof(v_data_size);

		BYTE* v_uncompress_data = new BYTE[v_data_size];
		uncompress( v_uncompress_data, (unsigned long*)&v_data_size, v_src_data + v_compress_data_offset, v_src_size - v_compress_data_offset );

		building_create_buffer buf( v_uncompress_data, v_data_size );
		buf.m_height = in_buffer->m_height;

		building_usd_buffer* v_usc_data = new building_usd_buffer( &buf );
		block_mesh* v_bldg_mesh = new block_mesh( v_usc_data );

		AUTO_DELETE( v_uncompress_data );

		return (object_base*)v_bldg_mesh;

	}	
	
}