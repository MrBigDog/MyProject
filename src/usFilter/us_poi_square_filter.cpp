///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_poi_square_filter.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : poi_square_filter class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_poi_square_filter.h"

namespace uniscope_globe
{
	poi_square_filter::poi_square_filter( void )
	{

	}

	poi_square_filter::~poi_square_filter( void )
	{

	}

	object_base* poi_square_filter::parse( raw_buffer* in_buffer )
	{
		ulong bytes_read = 0;

		us_square_head v_square_head;
		bytes_read += in_buffer->read( (void* )&v_square_head, sizeof( us_square_head ) );
		if ( v_square_head.square_type != 5 )
		{
			return NULL;
		}

		poi_square_raw_data* v_raw_data = new poi_square_raw_data();

		us_common_segment v_segment;
		bytes_read += in_buffer->read( (void* )&v_segment, sizeof( us_common_segment ) );

		us_vertex_segment_ex v_vertex_segment_ex;
		bytes_read += in_buffer->read( (void* )&v_vertex_segment_ex, sizeof( us_vertex_segment_ex ) );

		v_raw_data->m_divide_number = v_vertex_segment_ex.divide_number;
		if( v_vertex_segment_ex.elevation_max < v_vertex_segment_ex.elevation_min )
		{
			v_raw_data->m_elevation_min = 0;
			v_raw_data->m_elevation_max = 0;
		}
		else
		{
			v_raw_data->m_elevation_min = v_vertex_segment_ex.elevation_min;
			v_raw_data->m_elevation_max = v_vertex_segment_ex.elevation_max;
		}		


		while ( bytes_read < in_buffer->get_size() )
		{
			vector3<short> v_pos;

			bytes_read += in_buffer->read( &v_pos.x, sizeof(short) );
			bytes_read += in_buffer->read( &v_pos.y, sizeof(short) );
			bytes_read += in_buffer->read( &v_pos.z, sizeof(short) );

			v_raw_data->m_pos_array.push_back( v_pos );

			GUID v_guid;
			bytes_read += in_buffer->read( &v_guid, sizeof(GUID) );
			poi_index v_poi_index;
			GUID_TO_INT64( v_guid, v_poi_index.first, v_poi_index.second );			
			v_raw_data->m_id_array.push_back( v_poi_index );

			string poi_name;
			bytes_read += in_buffer->read( poi_name );

			v_raw_data->m_name_array.push_back( string_ext::to_wstring( poi_name ) );
		}

		poi_square_resource* v_square_resource = new poi_square_resource( v_raw_data );

		return v_square_resource;

	}
}