///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_ipkg_filter.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : ipkg_filter class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_ipkg_filter.h"

namespace uniscope_globe
{
	object_base* ipkg_filter::parse( raw_buffer* in_buffer )
	{
		return parse_from_compressed_stream( in_buffer );
	}

	object_base* ipkg_filter::parse_from_compressed_stream( raw_buffer* stream )
	{
		// 读文件头
		int v_four_cc, v_type, v_has_alpha = 0;
		stream->read( (void*)&v_four_cc, sizeof(v_four_cc) );
		stream->read( (void*)&v_type, sizeof(v_type) );
		stream->read( (void*)&v_has_alpha, sizeof(v_has_alpha) );

		// 读取影像数据头
		int v_image_type, v_jpeg_size = 0;
		stream->read( (void*)&v_image_type, sizeof(v_image_type) );
		stream->read( (void*)&v_jpeg_size, sizeof(v_jpeg_size) );

		// 读取影像数据(JPG)
		byte* v_jpeg_buffer = (byte*)AUTO_ALLOC_MEMORY(v_jpeg_size);
		if(v_jpeg_buffer == NULL ) { return NULL; }
		stream->read( (void*)v_jpeg_buffer, v_jpeg_size );

		// 解压缩JPEG
		int v_width, v_height;
		byte* v_bmp_buffer = NULL;
		jpeg_loader::load( v_jpeg_buffer, v_jpeg_size, v_bmp_buffer, v_width, v_height, texture_format::US_TF_A8R8G8B8 );
		if ( v_bmp_buffer == NULL )  return false;

		AUTO_FREE_MEMORY( v_jpeg_buffer );

		// 解压缩Alpha
		if( v_has_alpha == 1 )
		{
			// 读取alpha数据头
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
			if ( v_bmp_alpha_buffer != NULL && v_width == v_alpha_width && v_height == v_alpha_height) 
			{
				// 填入Mask信息
				ulong* v_pw_bmp_buffer = (ulong*)v_bmp_buffer;
				ulong* v_pw_bmp_alpha_buffer = (ulong*)v_bmp_alpha_buffer;
				//int v_pitich = int(v_width >> 3);
				int index = 0;
				for( int ni =  0; ni < v_height ; ni++ )
				{
					for( int nj = 0; nj < v_width; nj++ )
					{
						ulong mask_key = (DWORD)v_pw_bmp_alpha_buffer[index];
						v_pw_bmp_buffer[index] = ((mask_key << 24)) | (v_pw_bmp_buffer[index] & 0x00ffffff);
						index++;
					}
				}
				// 销毁临时德AlphaBuffer（解压后的）
				AUTO_ALIGNED_FREE_MEMORY( v_bmp_alpha_buffer );
			}
			// 销毁读取出的Alpha数据（解压前的）
			AUTO_FREE_MEMORY( v_jpeg_alpha_buffer );
		}

		int new_width = 0, new_height = 0;
		jpeg_loader::get_nearest_exp_size( v_height, v_width, 0.382f, new_height, new_width );
		if ( new_width != v_width || new_height != v_height )
		{
			byte* v_new_dib_buf = NULL;
			jpeg_loader::resample( v_bmp_buffer, v_width, v_height, v_new_dib_buf, new_width, new_height, texture_format::US_TF_A8R8G8B8 );
			AUTO_ALIGNED_FREE_MEMORY( v_bmp_buffer );
			v_bmp_buffer = v_new_dib_buf;
			v_width = new_width;
			v_height = new_height;
		}

		dds_file v_dds_file;
		if ( v_has_alpha == 1 )
		{
			v_dds_file.create_dxt5( v_bmp_buffer, v_width, v_height, 0, true );
		}
		else 
		{
			v_dds_file.create_dxt1( v_bmp_buffer, v_width, v_height, 0, true );
		}
		AUTO_ALIGNED_FREE_MEMORY( v_bmp_buffer );
		return (object_base*)(new d3d9_hardware_texture( v_dds_file.get_data(), v_dds_file.m_tex_info, false ));
	}

	
}