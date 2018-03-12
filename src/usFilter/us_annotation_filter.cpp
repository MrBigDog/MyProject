///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_annotation_filter.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : annotation_filter class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////

#include "us_annotation_filter.h"

namespace uniscope_globe
{
	annotation_filter::annotation_filter( void )
	{

	}

	annotation_filter::~annotation_filter( void )
	{

	}

	object_base* annotation_filter::parse( raw_buffer* in_buffer )
	{
		interior_index v_index = ((square_buffer*)in_buffer)->get_index();

		return parse_from_stream( in_buffer );
	}

	annotation_square_resource* annotation_filter::parse_from_stream( raw_buffer* stream)
	{
		ulong bytes_read = 0;

		us_square_head v_square_head;
		bytes_read += stream->read( (void* )&v_square_head, sizeof( us_square_head ) );
		if ( v_square_head.square_type != 8 )
		{
			return NULL;
		}

		us_common_segment v_segment;
		bytes_read += stream->read( (void* )&v_segment, sizeof( us_common_segment ) );

		us_vertex_segment_ex v_vertex_segment_ex;
		bytes_read += stream->read( (void* )&v_vertex_segment_ex, sizeof( us_vertex_segment_ex ) );
		

		ulong number_of_annotation = 0;

		annotation_square_raw_data* square_rd = new annotation_square_raw_data();
		square_rd->m_divide_number = v_vertex_segment_ex.divide_number;
		if( v_vertex_segment_ex.elevation_max == -9999 )
		{
			square_rd->m_elevation_min = 0;
			square_rd->m_elevation_max = 0;
		}
		else
		{
			square_rd->m_elevation_min = v_vertex_segment_ex.elevation_min;
			square_rd->m_elevation_max = v_vertex_segment_ex.elevation_max;
		}		
		
		while ( bytes_read < stream->get_size() )
		{
			vector3<ushort> v_pos;
			bytes_read += stream->read( (void*)&v_pos, sizeof(ushort) * 3 );
			square_rd->m_pos_array.push_back( v_pos );

			double v_heading = 0.0;
			bytes_read += stream->read( (void*)&v_heading, sizeof( double ) );
			square_rd->m_heading_array.push_back( v_heading );


			string annotation_text;
			bytes_read += stream->read( annotation_text );
			square_rd->m_name_array.push_back( string_ext::to_wstring( annotation_text ) );
		}

		annotation_square_resource* v_annotation_res = new annotation_square_resource(square_rd);

		return v_annotation_res;
	}
}