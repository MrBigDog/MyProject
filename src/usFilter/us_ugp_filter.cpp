///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_ugp_filter.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : ugp_filter class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_ugp_filter.h"

namespace uniscope_globe
{
	object_base* ugp_filter::parse( raw_buffer* v_buffer )
	{
		building_create_buffer* in_buffer = (building_create_buffer*)v_buffer;

		// new one
		byte* v_src_data = in_buffer->get_buffer();
		ulong v_src_size = in_buffer->get_size();
		
		bldg_builder_head v_head;
		in_buffer->read( &v_head, sizeof(v_head) );

		bldg_compress_parameter v_com_param;
		in_buffer->read( &v_com_param, sizeof(v_com_param) );

		ulong v_compress_data_offset = sizeof(v_head) + sizeof(v_com_param);

		ulong v_data_size = v_com_param.pre_compress_size;
		BYTE* v_uncompress_data = new BYTE[v_data_size];
		uncompress( v_uncompress_data, (unsigned long*)&v_data_size, v_src_data + v_compress_data_offset, v_src_size - v_compress_data_offset );

		building_create_buffer buf( v_uncompress_data, v_data_size );
		buf.m_height = in_buffer->m_height;

		building_mesh_container* v_mesh_container = new building_mesh_container;
		v_mesh_container->read_from_group_buffer( &buf );
		
		AUTO_DELETE( v_uncompress_data );

		return (object_base*)v_mesh_container;
	}	
	
}