
///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_uni_jpeg_filter_lm.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : uni_jpeg_filter_lm class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_uni_jpeg_filter_lm.h"

namespace uniscope_globe
{
	object_base* uni_jpeg_filter_lm::parse( raw_buffer* in_buffer )
	{
		byte* jpg_buff = in_buffer->get_buffer();
		int v_type = 0xe0ffd8ff;
		memcpy( jpg_buff, &v_type, sizeof(v_type) );

		// ½âÑ¹ËõJPEG
		int v_width, v_height;
		byte* v_dib_buffer = NULL;
		jpeg_loader::load( jpg_buff, in_buffer->get_size(), v_dib_buffer, v_width, v_height, texture_format::US_TF_A8R8G8B8 );
		if ( NULL == v_dib_buffer ) 
		{
			int v_type = 0xe1ffd8ff;
			memcpy( jpg_buff, &v_type, sizeof(v_type) );
			jpeg_loader::load( jpg_buff, in_buffer->get_size(), v_dib_buffer, v_width, v_height, texture_format::US_TF_A8R8G8B8 );

			if ( NULL == v_dib_buffer )
				return NULL;
		}

		if( v_width < 4 || v_height < 4 ) 
		{
			return NULL;
		}

		int new_width = 0, new_height = 0;
		jpeg_loader::get_nearest_exp_size( v_height, v_width, 0.382, new_height, new_width );
		if ( new_width != v_width || new_height != v_height )
		{
			byte* v_new_dib_buf = NULL;
			jpeg_loader::resample( v_dib_buffer, v_width, v_height, v_new_dib_buf, new_width, new_height, texture_format::US_TF_A8R8G8B8 );
			AUTO_ALIGNED_FREE_MEMORY( v_dib_buffer );
			v_dib_buffer = v_new_dib_buf;
			v_width = new_width;
			v_height = new_height;
		}

		dds_file v_dds_file;
		bool b_suc = v_dds_file.create_argb( v_dib_buffer, v_width, v_height, 0, true );
		AUTO_ALIGNED_FREE_MEMORY( v_dib_buffer );

		if ( !b_suc )
			return NULL;

		return (object_base*)(new d3d9_hardware_texture( v_dds_file.get_data(), v_dds_file.m_tex_info, false ));
	}
	
}