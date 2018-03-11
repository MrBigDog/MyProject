///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_ugp_filter_v1.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : ugp_filter_v1 class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_ugp_filter_v1.h"

namespace uniscope_globe
{
	object_base* ugp_filter_v1::parse( raw_buffer* v_buffer )
	{
		building_create_buffer* compress_buffer = (building_create_buffer*)v_buffer;

		// 读取数据头
		bldg_builder_head v_head;
		compress_buffer->read( &v_head, sizeof(v_head) );

		bldg_compress_parameter v_com_param;
		compress_buffer->read( &v_com_param, sizeof(v_com_param) );

		// 分配解压缩数据内存
		ulong v_data_size = v_com_param.pre_compress_size;
		BYTE* v_uncompress_data = new BYTE[v_data_size];

		// 原始数据
		ulong v_compress_data_offset = sizeof(v_head) + sizeof(v_com_param);
		byte* v_src_data = compress_buffer->get_buffer();
		ulong v_src_size = compress_buffer->get_size();

		// 解压
		uncompress( v_uncompress_data, (unsigned long*)&v_data_size, v_src_data + v_compress_data_offset, v_src_size - v_compress_data_offset );

		building_create_buffer uncompress_buf( v_uncompress_data, v_data_size );
		model_complex_hardware_mesh* v_group_mesh = new model_complex_hardware_mesh();

		// 解析
		int v_readed_bytes = 0;

		bldg_group_parameter v_group_param;
		v_readed_bytes += uncompress_buf.read( &v_group_param, sizeof(v_group_param) );

		long* v_size_array = new long[v_group_param.mesh_num];
		v_readed_bytes += uncompress_buf.read( v_size_array, sizeof(long) * v_group_param.mesh_num );

		byte* v_buff = uncompress_buf.get_buffer();
		int v_off = sizeof( bldg_builder_head );
		usm_filter_v1 v_usm_filter;
		for ( int i = 0; i < v_group_param.mesh_num; i++ )
		{
			building_create_buffer v_sub_mesh_buf( v_buff + v_readed_bytes + v_off, v_size_array[i] - v_off );

			model_lod_hardware_mesh* v_sub_mesh = v_usm_filter.parse_model_lod_hardware_mesh( &v_sub_mesh_buf );
			v_group_mesh->add_sub_mesh( v_sub_mesh );
		
			v_readed_bytes += v_size_array[i];
		}
	
		AUTO_DELETE( v_uncompress_data );

		return v_group_mesh;
	}	
	
}