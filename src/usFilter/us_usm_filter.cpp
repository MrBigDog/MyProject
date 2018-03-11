///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_usm_filter.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : usm_filter class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_usm_filter.h"

namespace uniscope_globe
{
	object_base* usm_filter::parse( raw_buffer* v_buffer )
	{
		building_create_buffer* in_buffer = (building_create_buffer*)v_buffer;

		// new one
		byte* v_src_data = in_buffer->get_buffer();
		ulong v_src_size = in_buffer->get_size();
				
		bldg_builder_head v_head( 0, 0 );
		in_buffer->read( &v_head, sizeof(v_head) );

		ulong v_data_size = 0;
		in_buffer->read( &v_data_size, sizeof(v_data_size) );

		ulong v_compress_data_offset = sizeof(v_head) + sizeof(v_data_size);

		BYTE* v_uncompress_data = new BYTE[v_data_size];
		uncompress( v_uncompress_data, (unsigned long*)&v_data_size, v_src_data + v_compress_data_offset, v_src_size - v_compress_data_offset );
				
		building_create_buffer buf( v_uncompress_data, v_data_size );
		buf.m_height = in_buffer->m_height;

		//if(system_environment::s_new_building)
		{
			building_usd_buffer* v_usd_data = new building_usd_buffer( &buf );
			if(system_environment::s_preprocess_normal)
			{
				v_usd_data->generate_normals();
				v_usd_data->generate_topological();
				v_usd_data->generate_mesh();
			}
			building_mesh_standard* v_bldg_mesh = new building_mesh_standard( v_usd_data );

			AUTO_DELETE( v_uncompress_data );

			return (object_base*)v_bldg_mesh;
		}
		//else
		//{
		//	building_usc_buffer* v_usc_data = new building_usc_buffer( &buf );
		//	building_mesh* v_bldg_mesh = new building_mesh( v_usc_data );

		//	AUTO_DELETE( v_uncompress_data );

		//	return (object_base*)v_bldg_mesh;
		//}

		
		return (object_base*)NULL;
	}	
}