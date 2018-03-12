///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_jpeg_loader.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : jpeg_loader class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_JPEG_LOADER_H_
#define _US_JPEG_LOADER_H_
#include <usFilter\Export.h>

namespace uniscope_globe
{
	class LIB_FILTER_API jpeg_loader
	{
	public:
		jpeg_loader( void ){}

		virtual ~jpeg_loader( void ){}

	public:
		static bool load_jpg_file( const LPCTSTR str_file, byte*& out_buffer, int& out_width, int& out_height, int tex_format );

		static bool load( byte* in_buffer, int buffer_size, byte*& out_buffer, int& out_width, int& out_height, int tex_format );

		static bool release( byte* buffer );

		

		static bool load( byte* in_buffer, int buffer_size, byte*& out_buffer, int& out_width, int& out_height, int tex_format, int v_mip_level );
		
		static long calculate_mipmap_size( int in_row, int in_col, int mip_level, int t_fmt );

		static bool get_mipmap( byte* in_buffer, long in_width, long in_height , byte* out_buffer, int v_level, long texture_format );

		static void save_as_bmp( byte* in_buffer, int v_row, int v_col, LPCTSTR str_file );

	public:
		static bool resize( byte* in_buffer, long in_width, long in_height , byte* out_buffer, long out_width, long out_height , long texture_format );

		static bool resample( byte* in_buffer, long in_width, long in_height, byte*& out_buffer, long out_width, long out_height, long texture_format );

		static bool get_image_size_exp( int in_row, int in_col, int& row_exp, int& col_exp );

		static bool get_nearest_exp_size( int in_row, int in_col, float v_ref, int& out_row, int& out_col );
	};
}

#endif // _US_JPEG_LOADER_H_