
///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_vector_line_filter4.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : vector_line_filter4 class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_vector_line_filter4.h"

namespace uniscope_globe
{
	object_base* vector_line_filter4::parse( raw_buffer* stream )
	{
		vector_raw_data* v_raw_data = new vector_raw_data();

		v_raw_data->m_type = vector_type::VECTOR_LINE;

		//
		// 数据版本及类型
		//
		us_square_head v_square_head;
		stream->read( (void* )&v_square_head, sizeof( us_square_head ) );

		v_raw_data->m_version = v_square_head.version;

		//
		// vertex buffer
		//
		us_zipped_segment v_zipped_segment;

		stream->read((void*)&(v_zipped_segment.segment_type), sizeof(short));
		stream->read((void*)&(v_zipped_segment.unzipped_data_size), sizeof(ulong));
		stream->read((void*)&(v_zipped_segment.zipped_data_size), sizeof(ulong));

		stream->read((void*)&(v_raw_data->m_divide_number), sizeof(short));
		stream->read((void*)&(v_raw_data->m_elevation_max), sizeof(float));
		stream->read((void*)&(v_raw_data->m_elevation_min), sizeof(float));

		if( v_raw_data->m_elevation_max < v_raw_data->m_elevation_min )
		{
			v_raw_data->m_elevation_min = 0;
			v_raw_data->m_elevation_max = 0;
		}
		else
		{
			v_raw_data->m_terrain_elevation = true;
		}


		byte* v_zipped_vb = new byte[v_zipped_segment.zipped_data_size];
		stream->read( (void*)v_zipped_vb, v_zipped_segment.zipped_data_size );
		byte* v_unzipped_vb = new byte[ v_zipped_segment.unzipped_data_size ];
		uncompress( v_unzipped_vb, &v_zipped_segment.unzipped_data_size, v_zipped_vb, v_zipped_segment.zipped_data_size );
		AUTO_DELETE( v_zipped_vb );

		v_raw_data->m_vertex_buffer = (short* )v_unzipped_vb;
		v_raw_data->m_vertex_buffer_size = v_zipped_segment.unzipped_data_size / sizeof(short);

		//
		// index buffer
		//
		stream->read((void*)&(v_zipped_segment.segment_type), sizeof(short));
		stream->read((void*)&(v_zipped_segment.unzipped_data_size), sizeof(ulong));
		stream->read((void*)&(v_zipped_segment.zipped_data_size), sizeof(ulong));


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
		// color segment
		//
		stream->read((void*)&(v_zipped_segment.segment_type), sizeof(short));
		stream->read((void*)&(v_zipped_segment.unzipped_data_size), sizeof(ulong));
		stream->read((void*)&(v_zipped_segment.zipped_data_size), sizeof(ulong));

		byte* v_zipped_cb = new byte[v_zipped_segment.zipped_data_size];
		stream->read( (void*)v_zipped_cb, v_zipped_segment.zipped_data_size );
		byte* v_unzipped_cb = new byte[ v_zipped_segment.unzipped_data_size ];
		uncompress( v_unzipped_cb, &v_zipped_segment.unzipped_data_size, v_zipped_cb, v_zipped_segment.zipped_data_size );
		AUTO_DELETE( v_zipped_cb );

		v_raw_data->m_color_buffer = (ulong*)v_unzipped_cb;
		v_raw_data->m_color_buffer_size = v_zipped_segment.unzipped_data_size / sizeof(ulong);

		//
		// guid segment
		//
		stream->read((void*)&(v_zipped_segment.segment_type), sizeof(short));
		stream->read((void*)&(v_zipped_segment.unzipped_data_size), sizeof(ulong));
		stream->read((void*)&(v_zipped_segment.zipped_data_size), sizeof(ulong));

		byte* v_zipped_gb = new byte[v_zipped_segment.zipped_data_size];
		stream->read( (void*)v_zipped_gb, v_zipped_segment.zipped_data_size );
		byte* v_unzipped_gb = new byte[ v_zipped_segment.unzipped_data_size ];
		uncompress( v_unzipped_gb, &v_zipped_segment.unzipped_data_size, v_zipped_gb, v_zipped_segment.zipped_data_size );
		AUTO_DELETE( v_zipped_gb );

		v_raw_data->m_guid_buffer = (byte*)v_unzipped_gb;
		v_raw_data->m_guid_buffer_size = v_zipped_segment.unzipped_data_size;

		return v_raw_data;
	}

	
}