///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_vector_region_filter.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : vector_region_filter class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_vector_region_filter.h"

namespace uniscope_globe
{
	vector_region_filter::vector_region_filter( void )
	{

	}

	vector_region_filter::~vector_region_filter( void )
	{

	}

	object_base* vector_region_filter::parse( raw_buffer* stream )
	{
		vector_raw_data* v_raw_data = new vector_raw_data();

		//
		// 数据版本及类型
		//
		us_square_head v_square_head;
		stream->read( (void* )&v_square_head, sizeof( us_square_head ) );
		if ( v_square_head.square_type != 7 )
		{
			return NULL;
		}


		//
		// vertex buffer
		//
		us_zipped_segment v_zipped_segment;
		stream->read( (void* )&v_zipped_segment, sizeof( us_zipped_segment ) );

		us_vertex_segment_ex v_vertex_segment_ex;
		stream->read( (void* )&v_vertex_segment_ex, sizeof( us_vertex_segment_ex ) );

		byte* v_zipped_vb = new byte[v_zipped_segment.zipped_data_size];
		stream->read( (void*)v_zipped_vb, v_zipped_segment.zipped_data_size );
		byte* v_unzipped_vb = new byte[ v_zipped_segment.unzipped_data_size ];
		uncompress( v_unzipped_vb, &v_zipped_segment.unzipped_data_size, v_zipped_vb, v_zipped_segment.zipped_data_size );
		AUTO_DELETE( v_zipped_vb );

		v_raw_data->m_divide_number = v_vertex_segment_ex.divide_number;
		if( v_vertex_segment_ex.elevation_max < v_vertex_segment_ex.elevation_min )
		{
			v_raw_data->m_elevation_min = 0;
			v_raw_data->m_elevation_max = 0;
		}
		else
		{
			v_raw_data->m_terrain_elevation = true;
			v_raw_data->m_elevation_min = v_vertex_segment_ex.elevation_min;
			v_raw_data->m_elevation_max = v_vertex_segment_ex.elevation_max;
		}
		
		v_raw_data->m_vertex_buffer = (short* )v_unzipped_vb;
		v_raw_data->m_vertex_buffer_size = v_zipped_segment.unzipped_data_size / sizeof(short);

		//
		// index buffer
		//
		stream->read( (void* )&v_zipped_segment, sizeof( us_zipped_segment ) );

		byte* v_zipped_ib = new byte[v_zipped_segment.zipped_data_size];
		stream->read( (void*)v_zipped_ib, v_zipped_segment.zipped_data_size );
		byte* v_unzipped_ib = new byte[ v_zipped_segment.unzipped_data_size ];
		uncompress( v_unzipped_ib, &v_zipped_segment.unzipped_data_size, v_zipped_ib, v_zipped_segment.zipped_data_size );
		AUTO_DELETE( v_zipped_ib );

		v_raw_data->m_index_buffer_size = v_zipped_segment.unzipped_data_size / sizeof(ushort);
		v_raw_data->m_index_buffer = new ulong[v_raw_data->m_index_buffer_size];
		for ( int i = 0; i < v_raw_data->m_index_buffer_size; i++ )
		{
			v_raw_data->m_index_buffer[i] = (ulong)((ushort*)v_unzipped_ib)[i];
		}
		AUTO_DELETE( v_unzipped_ib );
		
		


		//
		// line index buffer
		//
		stream->read( (void* )&v_zipped_segment, sizeof( us_zipped_segment ) );

		v_zipped_ib = new byte[v_zipped_segment.zipped_data_size];
		stream->read( (void*)v_zipped_ib, v_zipped_segment.zipped_data_size );
		v_unzipped_ib = new byte[ v_zipped_segment.unzipped_data_size ];
		uncompress( v_unzipped_ib, &v_zipped_segment.unzipped_data_size, v_zipped_ib, v_zipped_segment.zipped_data_size );
		AUTO_DELETE( v_zipped_ib );

		v_raw_data->m_line_index_buffer_size = v_zipped_segment.unzipped_data_size / sizeof(ushort);
		v_raw_data->m_line_index_buffer = new ulong[v_raw_data->m_line_index_buffer_size];
		for ( int i = 0; i < v_raw_data->m_line_index_buffer_size; i++ )
		{
			v_raw_data->m_line_index_buffer[i] = (ulong)((ushort*)v_unzipped_ib)[i];
		}
		AUTO_DELETE( v_unzipped_ib );

		//if( v_square_head.version == 4 )
		//{
		//	//
		//	// contour index buffer
		//	//
		//	stream->read( (void* )&v_zipped_segment, sizeof( us_zipped_segment ) );

		//	v_zipped_ib = new byte[v_zipped_segment.zipped_data_size];
		//	stream->read( (void*)v_zipped_ib, v_zipped_segment.zipped_data_size );
		//	v_unzipped_ib = new byte[ v_zipped_segment.unzipped_data_size ];
		//	uncompress( v_unzipped_ib, &v_zipped_segment.unzipped_data_size, v_zipped_ib, v_zipped_segment.zipped_data_size );
		//	AUTO_DELETE( v_zipped_ib );

		//	v_raw_data->m_contour_index_buffer_size = v_zipped_segment.unzipped_data_size / sizeof(ulong);
		//	v_raw_data->m_contour_index_buffer = new ulong[v_raw_data->m_contour_index_buffer_size];
		//	for ( int i = 0; i < v_raw_data->m_contour_index_buffer_size; i++ )
		//	{
		//		v_raw_data->m_contour_index_buffer[i] = (ulong)((ushort*)v_unzipped_ib)[i];
		//	}
		//	AUTO_DELETE( v_unzipped_ib );
		//}

		v_raw_data->m_version = v_square_head.version;

		//vector_region* v_region = new vector_region( v_raw_data );
		v_raw_data->m_type = vector_type::VECTOR_REGION;




		return v_raw_data;
	}
	
}