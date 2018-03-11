///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_tpkg_filter.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : tpkg_filter class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_tpkg_filter.h"

namespace uniscope_globe
{
	object_base* tpkg_filter::parse( raw_buffer* stream )
	{
		// 读文件头
		int v_four_cc, v_type, v_has_lmap = 0;
		stream->read( (void*)&v_four_cc, sizeof(v_four_cc) );
		stream->read( (void*)&v_type, sizeof(v_type) );
		stream->read( (void*)&v_has_lmap, sizeof(v_has_lmap) );

		//////////////////////////////////////////////////////////////////////////
		// 读取diffuse数据头
		//////////////////////////////////////////////////////////////////////////
		int v_image_type, v_jpeg_size = 0;
		stream->read( (void*)&v_image_type, sizeof(v_image_type) );
		stream->read( (void*)&v_jpeg_size, sizeof(v_jpeg_size) );

		// 读取diffuse数据
		byte* v_jpeg_buffer = (byte*)AUTO_ALLOC_MEMORY(v_jpeg_size);
		if(v_jpeg_buffer == NULL ) { return NULL; }
		stream->read( (void*)v_jpeg_buffer, v_jpeg_size );

		// 解压缩JPEG
		int v_width, v_height;
		byte* v_bmp_buffer = NULL;
		jpeg_loader::load( v_jpeg_buffer, v_jpeg_size, v_bmp_buffer, v_width, v_height, texture_format::US_TF_A8R8G8B8 );
		AUTO_FREE_MEMORY( v_jpeg_buffer );
		if ( v_bmp_buffer == NULL )
		{
			return false;
		}

		//////////////////////////////////////////////////////////////////////////
		// 读取alpha数据头
		//////////////////////////////////////////////////////////////////////////
		int v_alpha_type, v_jpeg_alpha_size = 0;
		stream->read( (void*)&v_alpha_type, sizeof(v_alpha_type) );
		stream->read( (void*)&v_jpeg_alpha_size, sizeof(v_jpeg_alpha_size) );

		// 读取alpha数据(JPG)
		byte* v_jpeg_alpha_buffer = (byte*)AUTO_ALLOC_MEMORY(v_jpeg_alpha_size);
		if(v_jpeg_alpha_buffer == NULL ) { return NULL; }
		stream->read( (void*)v_jpeg_alpha_buffer, v_jpeg_alpha_size );

		int v_alpha_width = 0;
		int v_alpha_height = 0;
		byte* v_bmp_alpha_buffer = NULL;
		jpeg_loader::load( v_jpeg_alpha_buffer, v_jpeg_alpha_size, v_bmp_alpha_buffer, v_alpha_width, v_alpha_height, texture_format::US_TF_A8R8G8B8 );
		
		AUTO_FREE_MEMORY( v_jpeg_alpha_buffer );
		if ( v_bmp_alpha_buffer == NULL )
		{
			AUTO_ALIGNED_FREE_MEMORY( v_bmp_buffer );
			return false;
		}
		
		dds_file v_dds_file_diffuse;
		v_dds_file_diffuse.create_dxt5( v_bmp_buffer, v_width, v_height, 0, true );
		AUTO_ALIGNED_FREE_MEMORY( v_bmp_buffer );
		
		dds_file v_dds_file_lmap;
		v_dds_file_lmap.create_dxt5( v_bmp_alpha_buffer, v_alpha_width, v_alpha_height, 0, true );
		AUTO_ALIGNED_FREE_MEMORY( v_bmp_alpha_buffer );

		d3d9_hierarchy_texture* v_hierarchy_texture = new d3d9_hierarchy_texture;

		// diffuse
		d3d9_texture_frame* v_diffuse_texture_frame = new d3d9_texture_frame( v_dds_file_diffuse.m_dxt_buffer, v_dds_file_diffuse.m_tex_info );
		v_hierarchy_texture->insert_frame( v_diffuse_texture_frame );

		// lmap
		d3d9_texture_frame* v_lmap_texture_frame = new d3d9_texture_frame( v_dds_file_lmap.m_dxt_buffer, v_dds_file_lmap.m_tex_info );
		v_hierarchy_texture->insert_frame( v_lmap_texture_frame );

		return (object_base*)v_hierarchy_texture;
	}

}