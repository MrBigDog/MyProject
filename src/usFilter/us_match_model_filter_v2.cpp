///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: match_model_filter_v2.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : match_model_filter_v2 class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_match_model_filter_v2.h"

namespace uniscope_globe
{
	object_base* match_model_filter_v2::parse( raw_buffer* in_buffer )
	{

		square_buffer* v_stream = (square_buffer*) in_buffer;

		// ¿éÐÅÏ¢
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

		ushort num_of_model_set;
		bytes_read += v_stream->read( &num_of_model_set, sizeof( ushort ) );

		ushort divide_num;
		bytes_read += v_stream->read( &divide_num, sizeof( ushort ) );

		float min_elevate, max_elevate;
		bytes_read += v_stream->read( &max_elevate, sizeof( float ) );
		bytes_read += v_stream->read( &min_elevate, sizeof( float ) );
		
		match_model_square_resource* v_resource = new match_model_square_resource(v_stream->get_document());
		v_resource->m_elevation_max = max_elevate * 0.001 + spherical_coords::s_semi_major + v_stream->get_height_offset();
		v_resource->m_elevation_min = min_elevate * 0.001 + spherical_coords::s_semi_major + v_stream->get_height_offset();

		for ( int i = 0; i < (int)num_of_model_set; i++ )
		{
			string guid_str;
			bytes_read += v_stream->read( guid_str );

			uint number_of_model;
			bytes_read += v_stream->read( &number_of_model, sizeof( uint ) );

			//match_model_set* v_model_set = new match_model_set();
			//v_model_set->m_resource_guid = string_ext::to_wstring( guid_str.c_str() );
			//v_model_set->m_number_of_match_model = number_of_model;

			ustring v_resource_id = string_ext::to_wstring( guid_str.c_str() );

			for ( int j = 0; j< (int)number_of_model; j++ )
			{
				vector3<short> model_pos;
				vector3<double> model_rotation = vector3<double>(0.0, 0.0, 0.0);
				vector3<double> model_scale;
				double quat[4];

				bytes_read += v_stream->read( &model_pos, sizeof(vector3<short>) );
// 				bytes_read += v_stream->read( &model_rotation, sizeof(vector3<double>) );
				bytes_read += v_stream->read( quat, sizeof(double) * 4 );
				bytes_read += v_stream->read( &model_scale, sizeof(vector3<double>) );

				vector3<double> model_pos_sphr;
				model_pos_sphr.x = model_pos.x * sd.m_bound.get_width() / divide_num + sd.m_bound.m_west;
				model_pos_sphr.y = model_pos.y * sd.m_bound.get_height() / divide_num + sd.m_bound.m_south;
				model_pos_sphr.z = model_pos.z * ( max_elevate - min_elevate ) / divide_num + min_elevate;
				model_pos_sphr.z = model_pos_sphr.z * 0.001 + spherical_coords::s_semi_major + v_stream->get_height_offset();

				match_model_description desc;
				desc.m_guid_string = v_resource_id;
				desc.m_transform.create_from_sphr(	model_pos_sphr.x, 
					model_pos_sphr.y, 
					model_pos_sphr.z, 
					model_rotation.x * RADIAN,
					model_rotation.y * RADIAN,
					model_rotation.z * RADIAN );

				desc.m_cart_transform.set_rotation(quaternion<float>(quat[0], quat[1], quat[2], quat[3]));
				desc.m_transform.set_scale( model_scale );

				v_resource->add_match_model( desc );

				//v_model_set->m_model_transform_array.push_back( v_transform );
			}


			//v_resource->m_model_set_array.push_back( v_model_set );
		}		

		return v_resource;

	}


}