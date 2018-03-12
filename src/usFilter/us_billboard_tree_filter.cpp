///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_billboard_tree_filter.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : billboard_tree_filter class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_billboard_tree_filter.h"

namespace uniscope_globe
{
	billboard_tree_filter::billboard_tree_filter( void )
	{

	}

	billboard_tree_filter::~billboard_tree_filter( void )
	{

	}

	object_base* billboard_tree_filter::parse( raw_buffer* stream )
	{
		square_buffer* v_stream = (square_buffer*) stream;

		square_description sd;
		sd.create( v_stream->get_index() );

		vector3<double> center_sphr = sd.m_bound.get_center();
		center_sphr.z = spherical_coords::s_semi_major;
		vector3<double> center_geo = geocentric_coords::from_spherical_d( center_sphr );

		matrix4<double> mat_tnb = geocentric_coords::transform_from_cartesian_d( center_geo );
		matrix4<double> mat_tnb_inv = matrix4<double>::inverse( mat_tnb );

		ulong bytes_read = 0;

		short version;
		bytes_read += v_stream->read( &version, sizeof( short ) );

		short type;
		bytes_read += v_stream->read( &type, sizeof( short ) );

		ushort num_of_billboard_set;
		bytes_read += v_stream->read( &num_of_billboard_set, sizeof( ushort ) );

		ushort divide_num;
		bytes_read += v_stream->read( &divide_num, sizeof( ushort ) );

		float min_elevate, max_elevate;
		bytes_read += v_stream->read( &max_elevate, sizeof( float ) );
		bytes_read += v_stream->read( &min_elevate, sizeof( float ) );

		billboard_square_resource* v_resource = new billboard_square_resource();
		v_resource->m_max_elevation = max_elevate * 0.001 + spherical_coords::s_semi_major + v_stream->get_height_offset();
		v_resource->m_min_elevation = min_elevate * 0.001 + spherical_coords::s_semi_major + v_stream->get_height_offset();

		//billboard_square_resource::tree_set_array* v_tree_set_array = new billboard_square_resource::tree_set_array;

		for ( ulong i = 0; i < num_of_billboard_set; i++ )
		{
			string guid_str;
			bytes_read += v_stream->read( guid_str );

			uint number_of_billboard;
			bytes_read += v_stream->read( &number_of_billboard, sizeof( uint ) );

			//billboard_tree_set_desc tree_set_desc;
			//tree_set_desc.m_resource_guid = string_ext::to_wstring( guid_str );
			//tree_set_desc.m_number_of_billboard = number_of_billboard;

			ustring v_resource_guid = string_ext::to_wstring( guid_str );

			//billboard_tree_set* tree_set = new billboard_tree_set( tree_set_desc );

			for ( int j = 0; j< (int)number_of_billboard; j++ )
			{
				vector3<short> billboard_pos;
				vector3<double> billboard_scale;
				
				bytes_read += v_stream->read( &billboard_pos, sizeof(vector3<short>) );
				bytes_read += v_stream->read( &billboard_scale, sizeof(vector3<double>) );

				billboard_tree_desc tree_desc;
				tree_desc.m_tree_position_sphr.x = billboard_pos.x * sd.m_bound.get_width() / divide_num + sd.m_bound.m_west;
				tree_desc.m_tree_position_sphr.y = billboard_pos.y * sd.m_bound.get_height() / divide_num + sd.m_bound.m_south;
				tree_desc.m_tree_position_sphr.z = billboard_pos.z * ( max_elevate - min_elevate ) / divide_num + min_elevate;
				tree_desc.m_tree_position_sphr.z = tree_desc.m_tree_position_sphr.z * 0.001 + spherical_coords::s_semi_major + v_stream->get_height_offset();
				tree_desc.m_position_geo = geocentric_coords::from_spherical_d( tree_desc.m_tree_position_sphr );
				tree_desc.m_position_local = tree_desc.m_position_geo * mat_tnb_inv;
				tree_desc.m_scale = billboard_scale;
				tree_desc.m_guid_string  = v_resource_guid;

				//tree_set->m_tree_description_array.push_back( tree_desc );

				v_resource->add_billboard( tree_desc );
			}

			//v_tree_set_array->push_back( tree_set );
		}		

		return v_resource;
	}

}