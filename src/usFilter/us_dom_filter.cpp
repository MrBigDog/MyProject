///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_dom_filter.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : dom_filter class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "us_jpeg_loader.h"
#include "us_dom_filter.h"

namespace uniscope_globe
{
	object_base* dom_filter::parse( raw_buffer* in_buffer )
	{
		return parse_from_compressed_stream( in_buffer );
	}

	object_base* dom_filter_v2::parse( raw_buffer* in_buffer )
	{
		return parse_from_compressed_stream( in_buffer );
	}


	//*
	object_base* dom_filter::parse_from_compressed_stream( raw_buffer* in_buffer )
	{
		// 读取文件头
		us_square_head v_square_head;
		in_buffer->read( (void*)&v_square_head, sizeof( v_square_head ) );
		short v_grid_size;
		in_buffer->read( (void*)&v_grid_size, sizeof( v_grid_size ) );

		// 读取jpeg影像数据头
		us_jpeg_segment_head v_jpeg_head;
		memset(&v_jpeg_head, 0, sizeof(v_jpeg_head));
		in_buffer->read( (void*)&v_jpeg_head.segment_type, sizeof( v_jpeg_head.segment_type )  );
		in_buffer->read( (void*)&v_jpeg_head.reverse_flags, sizeof( v_jpeg_head.reverse_flags ) );	
		in_buffer->read( (void*)&v_jpeg_head.zip_data_size, sizeof( v_jpeg_head.zip_data_size ) );	

		// 读取jpeg影像数据
		byte* v_jpeg_buffer = (byte*) AUTO_ALLOC_MEMORY( v_jpeg_head.zip_data_size );
		if(v_jpeg_buffer == NULL ) { return NULL; }
		in_buffer->read( v_jpeg_buffer, v_jpeg_head.zip_data_size );

		// 读取alpha数据头
		us_alpha_1bit_segment_head v_alpha_head;
		memset(&v_alpha_head, 0, sizeof(v_alpha_head));
		in_buffer->read( (void*)&v_alpha_head.segment_type, sizeof(v_alpha_head.segment_type) );
		in_buffer->read( (void*)&v_alpha_head.zip_data_size, sizeof(v_alpha_head.zip_data_size));	
		in_buffer->read( (void*)&v_alpha_head.unzip_data_size, sizeof(v_alpha_head.unzip_data_size));	
		int v_mask_length = 0;

		//// 读取alpha数据
		//byte* v_zip_alpha_buffer = new BYTE[v_alpha_head.zip_data_size];
		//in_buffer->read(v_zip_alpha_buffer, v_alpha_head.zip_data_size);

		//// 使用Zip解压缩alpha数据
		//byte* v_unzip_alpha_buffer = new BYTE[ v_alpha_head.unzip_data_size ];
		//ulong v_unzip_alpha_size;
		//uncompress((byte*)v_unzip_alpha_buffer, &v_unzip_alpha_size, v_zip_alpha_buffer, v_alpha_head.zip_data_size );
		//AUTO_DELETE( v_zip_alpha_buffer );

		// 解压缩JPEG
		int v_width, v_height;
		byte* v_dib_buffer = NULL;
		jpeg_loader::load( v_jpeg_buffer, v_jpeg_head.zip_data_size, v_dib_buffer, v_width, v_height, texture_format::US_TF_A8R8G8B8 );
		if( v_dib_buffer == NULL )
		{
			AUTO_FREE_MEMORY( v_jpeg_buffer );
			return NULL;
		}

		dds_file v_dds_file;
		bool b_suc = false;
		if( system_environment::s_use_compressed_texture )
		{
			/*b_suc = */v_dds_file.create_dxt1( v_dib_buffer, v_width, v_height, 0, false );
		}
		else
		{
			/*b_suc = */v_dds_file.create_argb( v_dib_buffer, v_width, v_height, 0, false );
		}

		AUTO_ALIGNED_FREE_MEMORY( v_dib_buffer );
		AUTO_FREE_MEMORY( v_jpeg_buffer );
		return (object_base*)(new d3d9_hardware_texture( v_dds_file.get_data(), v_dds_file.m_tex_info, true ));
	}
	//*/
	//*
	object_base* dom_filter_v2::parse_from_compressed_stream( raw_buffer* in_buffer )
	{
		// 读取文件头
		us_square_head v_square_head;
		in_buffer->read( (void*)&v_square_head, sizeof( v_square_head ) );

		// 读取jpeg影像数据头
		us_common_segment v_jpeg_head;
		memset(&v_jpeg_head, 0, sizeof(v_jpeg_head));
		in_buffer->read( (void*)&v_jpeg_head, sizeof( v_jpeg_head ) );

		// 读取jpeg影像数据
		byte* v_jpeg_buffer = (byte*) AUTO_ALLOC_MEMORY( v_jpeg_head.data_size );
		if(v_jpeg_buffer == NULL ) { return NULL; }
		in_buffer->read( v_jpeg_buffer, v_jpeg_head.data_size );

		// 读取alpha数据头
		us_zipped_segment	v_alpha_head;
		memset(&v_alpha_head, 0, sizeof(v_alpha_head));
		in_buffer->read( (void*)&v_alpha_head, sizeof( v_alpha_head ) );
		int v_mask_length = 0;

		// 解压缩JPEG
		int v_width, v_height;
		byte* v_dib_buffer = NULL;
		jpeg_loader::load( v_jpeg_buffer, v_jpeg_head.data_size, v_dib_buffer, v_width, v_height, texture_format::US_TF_A8R8G8B8 );
		if( v_dib_buffer == NULL )
		{
			AUTO_FREE_MEMORY( v_jpeg_buffer );
			return NULL;
		}

		dds_file v_dds_file;
		bool b_suc = false;
		if( system_environment::s_use_compressed_texture )
		{
			/*b_suc = */v_dds_file.create_dxt1( v_dib_buffer, v_width, v_height, 0, false );
		}
		else
		{
			/*b_suc = */v_dds_file.create_argb( v_dib_buffer, v_width, v_height, 0, false );
		}

		AUTO_ALIGNED_FREE_MEMORY( v_dib_buffer );
		AUTO_FREE_MEMORY( v_jpeg_buffer );
		return (object_base*)(new d3d9_hardware_texture( v_dds_file.get_data(), v_dds_file.m_tex_info, true ));
	}
	//*/


	object_base* dom_filter_v3::parse( raw_buffer* in_buffer )
	{
		return parse_from_compressed_stream( in_buffer );
	}

	object_base* dom_filter_v3::parse_from_compressed_stream( raw_buffer* in_buffer )
	{
		// 读取文件头
		us_square_head v_square_head;
		in_buffer->read( (void*)&v_square_head, sizeof( v_square_head ) );
		if (v_square_head.square_type != 1000)
		{
			return NULL;
		}

		// 读取rgb影像数据头
		us_zipped_segment v_rgb_head;
		memset(&v_rgb_head, 0, sizeof(v_rgb_head));
		in_buffer->read( (void*)&v_rgb_head, sizeof( v_rgb_head ) );


		// 读取bmp影像数据
		int v_pic_size = (int)sqrt((float)v_rgb_head.unzipped_data_size / sizeof(ulong));

		// segment data;
		byte* v_uchr_data = (byte*) AUTO_ALLOC_MEMORY( v_rgb_head.unzipped_data_size );

		in_buffer->read( (void*)v_uchr_data, v_rgb_head.zipped_data_size );

		byte* v_rgb_buffer = (byte*) AUTO_ALLOC_MEMORY( v_rgb_head.unzipped_data_size );
		uncompress((byte*)v_rgb_buffer, &v_rgb_head.unzipped_data_size, v_uchr_data, v_rgb_head.zipped_data_size);

		byte* v_upset_buffer = (byte*) AUTO_ALLOC_MEMORY( v_rgb_head.unzipped_data_size );
		// 		ulong* v_upset_ul = (ulong*)v_upset_buffer;
		// 		ulong* v_rgb_ul = (ulong*)v_rgb_buffer;
		int index_rgb, index_upset;

		for ( int i = 0; i < v_pic_size; i ++ )
		{
			for ( int j = 0; j < v_pic_size; j ++ )
			{
				index_upset = (v_pic_size - 1 - i) * v_pic_size + j;
				index_rgb = i * v_pic_size + j;
				v_upset_buffer[index_rgb*4 + 0] = v_rgb_buffer[index_upset*4 + 2];
				v_upset_buffer[index_rgb*4 + 1] = v_rgb_buffer[index_upset*4 + 1];
				v_upset_buffer[index_rgb*4 + 2] = v_rgb_buffer[index_upset*4 + 0];
				v_upset_buffer[index_rgb*4 + 3] = 0xff;//v_rgb_buffer[index_upset*4 + 3];
			}
		}

		//////////////////////////////////////////////////////////////////////////
		dds_file v_dds_file;
		bool b_suc = v_dds_file.create_argb( v_upset_buffer, v_pic_size, v_pic_size, 0, false );

		AUTO_FREE_MEMORY( v_rgb_buffer );
		AUTO_FREE_MEMORY( v_uchr_data );
		AUTO_FREE_MEMORY( v_upset_buffer );


		if ( !b_suc )
			return NULL;

		return (object_base*)(new d3d9_hardware_texture( v_dds_file.get_data(), v_dds_file.m_tex_info, true ));

		//		//////////////////////////////////////////////////////////////////////////
		//		texture_info v_text_info;
		//		v_text_info.format = US_TF_A8R8G8B8;
		//		v_text_info.width = 512;
		//		v_text_info.height = 512;
		//		v_text_info.mipmap = 0;
		//		managed_buffer* v_buffer = new managed_buffer( v_upset_buffer, v_rgb_head.unzipped_data_size );
		//
		//		AUTO_FREE_MEMORY( v_rgb_buffer );
		//		AUTO_FREE_MEMORY( v_uchr_data );
		//// 		AUTO_FREE_MEMORY( v_upset_buffer );
		//
		//		return new d3d9_hardware_texture(v_buffer, v_text_info, false );

	}
	
}
