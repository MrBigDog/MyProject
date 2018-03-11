///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_grid_filter.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : grid_filter class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_grid_filter.h"

namespace uniscope_globe
{
	object_base* grid_filter::parse( raw_buffer* in_buffer )
	{
		interior_index v_index = ((square_buffer*)in_buffer)->get_index();

		return parse_from_compressed_stream( in_buffer, v_index );
	}

	object_base* grid_filter::parse_from_compressed_stream( raw_buffer* stream, interior_index index )
	{
		// read file head	
		//dem_mesh* v_mesh = NULL;
		try
		{
			
			us_square_head v_file_head;
			stream->read( &v_file_head, sizeof(v_file_head) );

			// read grid head data
			us_zipped_grid_segment_head v_grid_head;
			memset( &v_grid_head, 0, sizeof(v_grid_head) );
			stream->read( &v_grid_head, sizeof( v_grid_head ) );
			//stream->read( &v_grid_head.segment_type, sizeof(v_grid_head.segment_type) );
			//stream->read( &v_grid_head.grid_size, sizeof(v_grid_head.grid_size) );
			//stream->read( &v_grid_head.max_height, sizeof(v_grid_head.max_height) );
			//stream->read( &v_grid_head.min_height, sizeof(v_grid_head.min_height) );
			//stream->read( &v_grid_head.interval_count, sizeof(v_grid_head.interval_count) );
			//stream->read( &v_grid_head.data_length, sizeof(v_grid_head.data_length) );

			// read zipped grid data;
			byte* v_zip_data = new byte[v_grid_head.data_length];
			stream->read( v_zip_data, v_grid_head.data_length );

			// unzipped grid data
			int v_unzip_data_size = 33 * 33;
			ushort* v_unzip_data = new ushort[ v_unzip_data_size ];
			ulong v_unzip_data_real_size;
			uncompress( (byte*)v_unzip_data, &v_unzip_data_real_size, v_zip_data, v_grid_head.data_length );

			// 
			ushort v_invalide_value = 0x7fff;
			float v_interval_factor = (v_grid_head.max_height - v_grid_head.min_height) / ((float) v_grid_head.interval_count);
			int v_grid_buffer_size = v_unzip_data_real_size / sizeof(short);

			float* v_grid_buffer = new float[v_grid_buffer_size];

			// warning : v_unzip_data_real_size is not the exact data size

			//for( int ni = 0;ni < v_grid_buffer_size; ni++ )
			for( int ni = 0;ni < v_unzip_data_size; ni++ )
			{
				ushort v_relative_height = ((ushort*)v_unzip_data)[ni];
				if(v_relative_height == v_invalide_value)
				{
					v_grid_buffer[ni] = v_grid_head.min_height;
				}
				else
				{
					v_grid_buffer[ni] = v_grid_head.min_height + ((float)v_relative_height) * v_interval_factor;
				}

				//v_grid_buffer[ni] = -1;
			}

			// clean up temp data
			AUTO_DELETE_ARRAY(v_unzip_data);
			AUTO_DELETE_ARRAY(v_zip_data);

			// create grid mesh;
			dem_mesh* v_mesh = new dem_mesh( index, (byte*)v_grid_buffer, v_grid_head.min_height * 0.001, v_grid_head.max_height * 0.001 );

			AUTO_DELETE( v_grid_buffer );

		

			return (object_base*)v_mesh;

			
		}
		catch (...)
		{
			/*
			WCHAR erro[80];
			wsprintf(erro, L"error");
			OutputDebugStringW(erro);
			//*/
		}
		return NULL;
	}

	object_base* grid_filter_v2::parse( raw_buffer* in_buffer )
	{
		interior_index v_index = ((square_buffer*)in_buffer)->get_index();

		return parse_from_compressed_stream( in_buffer, v_index );
	}

	object_base* grid_filter_v2::parse_from_compressed_stream( raw_buffer* stream, interior_index index )
	{
		try
		{	
			//file_buffer v_file ;
			//v_file.file_open( L"C:\\ttt.txt", L"w+" );
			//v_file.write( wstring(L"start\n") );
			//v_file.file_flush();
			
			// read file head	
			us_square_head v_file_head;
			stream->read( &v_file_head, sizeof(v_file_head) );

			// read zipped segment
			us_zipped_segment v_grid_segment;
			memset( &v_grid_segment, 0, sizeof(v_grid_segment) );
			stream->read( &v_grid_segment, sizeof( v_grid_segment ) );

			us_grid_segment_ex v_grid_segment_ex;
			stream->read( &v_grid_segment_ex, sizeof( v_grid_segment_ex ) );

			//us_zipped_grid_segment_head v_grid_head;
			//memset( &v_grid_head, 0, sizeof(v_grid_head) );
			//stream->read( &v_grid_head, sizeof( v_grid_head ) );
			//stream->read( &v_grid_head.segment_type, sizeof(v_grid_head.segment_type) );
			//stream->read( &v_grid_head.grid_size, sizeof(v_grid_head.grid_size) );
			//stream->read( &v_grid_head.max_height, sizeof(v_grid_head.max_height) );
			//stream->read( &v_grid_head.min_height, sizeof(v_grid_head.min_height) );
			//stream->read( &v_grid_head.interval_count, sizeof(v_grid_head.interval_count) );
			//stream->read( &v_grid_head.data_length, sizeof(v_grid_head.data_length) );

			// read zipped grid data;
			byte* v_zip_data = new byte[v_grid_segment.zipped_data_size];
			stream->read( v_zip_data, v_grid_segment.zipped_data_size );

			// unzipped grid data
			int v_unzip_data_size = 33 * 33;
			ushort* v_unzip_data = new ushort[ v_unzip_data_size ];
			ulong v_unzip_data_real_size;
			uncompress( (byte*)v_unzip_data, &v_unzip_data_real_size, v_zip_data, v_grid_segment.zipped_data_size );

			// 
			ushort v_invalide_value = v_grid_segment_ex.invalid_value;
			float v_interval_factor = (v_grid_segment_ex.elevation_max - v_grid_segment_ex.elevation_min) / ((float) v_grid_segment_ex.divide_number);
			int v_grid_buffer_size = v_unzip_data_real_size / sizeof(short);

			float* v_grid_buffer = new float[v_grid_buffer_size];
			for( int ni = 0;ni < v_grid_buffer_size; ni++ )
			{
				ushort v_relative_height = ((ushort*)v_unzip_data)[ni];
				if(v_relative_height == v_invalide_value)
				{
					v_grid_buffer[ni] = v_grid_segment_ex.elevation_min;
				}
				else
				{
					v_grid_buffer[ni] = v_grid_segment_ex.elevation_min + ((float)v_relative_height) * v_interval_factor;
				}

				//v_grid_buffer[ni] = -1;
			}

			// clean up temp data
			AUTO_DELETE_ARRAY(v_unzip_data);
			AUTO_DELETE_ARRAY(v_zip_data);

			// create grid mesh;
			dem_mesh* v_mesh = new dem_mesh( index, (byte*)v_grid_buffer, v_grid_segment_ex.elevation_min * 0.001,  v_grid_segment_ex.elevation_max * 0.001);

			AUTO_DELETE( v_grid_buffer );

			//v_file.write( wstring(L"end\n") );
			//v_file.file_flush();

			//v_file.file_close();

			return (object_base*)v_mesh;
		}
		catch( exception& /*e*/ )
		{
			//MessageBoxW( NULL, string_ext::to_wstring(e.what()).c_str(), L"xxx", MB_OK );
		}
		return NULL;
	}
	
}