///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_render_object_line.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : render_object_line class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_render_object_line.h"
#include <usCore/us_render_argument.h>
#include <usCore/us_render_device.h>
#include <usCore/us_render_target.h>
#include <usCore/us_system_environment.h>
#include <usCore/us_observer_base.h>
#include <usCore/us_render_state.h>
#include <usCore/us_frame_counter.h>

#include <usUtil/us_spherical_coords.h>
#include <usUtil/us_cartesian_coords.h>
#include <usUtil/us_geocentric_coords.h>

namespace uniscope_globe
{
	render_object_line::render_object_line( void )
	{
		m_trans_matrix = matrix_4d::s_identity;
		m_collapse_matrix = matrix_4d::s_identity;
		m_center = vector_3d::s_zero;
		m_depth_enable = false;
	}

	render_object_line::~render_object_line( void )
	{
		m_vertex_array.clear();
	}

	render_object_line* render_object_line::create_shared_instance()
	{
		render_object_line* v_geometry = new render_object_line();
		v_geometry->add_ref();
		return v_geometry;
	}

	void render_object_line::draw( render_argument* args )
	{
		ulong vertices_count = m_vertex_array.size();
		if ( vertices_count < 2 )
			return;

		render_state rs( args->m_device );
		if( !m_depth_enable )
		{
			//rs.set_state(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		}


		//float slopeScaleDepthBias = 1.0f;
		//rs.set_state( D3DRS_SLOPESCALEDEPTHBIAS, *((DWORD*)&slopeScaleDepthBias) );

		//float depth_bias = -10.0f / (float)(( 1 << 24) - 1);


		//rs.set_state(D3DRS_ZWRITEENABLE, FALSE);	
		//float depth_bias = -1.0f;
		//rs.set_state( D3DRS_SLOPESCALEDEPTHBIAS, *((DWORD*)&depth_bias));
		 

		
		args->m_device->set_vertex_declaration( position_color::fvf );
		args->m_device->set_texture( 0, NULL );

		matrix_4d v_mat = m_collapse_matrix * m_trans_matrix;
		v_mat.m41 = v_mat.m41 - cartesian_coords::s_reference_position_geo.x;
		v_mat.m42 = v_mat.m42 - cartesian_coords::s_reference_position_geo.y;
		v_mat.m43 = v_mat.m43 - cartesian_coords::s_reference_position_geo.z;

		args->m_device->push_transform();
		args->m_device->multiply_transform( matrix_4f(v_mat.m) );

		args->m_device->draw_line_strip( (void*)&m_vertex_array[0], vertices_count , sizeof(position_color) );
		
		args->m_device->pop_transform();
	}

	void render_object_line::set_world_matrix( matrix_4d& mat )
	{
		m_trans_matrix = mat;
	}

	void render_object_line::set_collapse( matrix_4d& mat )
	{
		m_collapse_matrix = m_collapse_matrix * mat;
	}

	void render_object_line::clear()
	{
		m_vertex_array.clear();
	}

	void render_object_line::create_from_line_string( vector_3d* v_src, int v_size, ulong v_color )
	{
		m_vertex_array.clear();
		m_vertex_array.resize( v_size );

		vector_3d vec = geocentric_coords::from_spherical_d( v_src[0] );
		m_vertex_array[0].x = vec.x;
		m_vertex_array[0].y = vec.y;
		m_vertex_array[0].z = vec.z;
		m_vertex_array[0].color = v_color;

		vector_3d vec_min( vec.x , vec.y, vec.z );
		vector_3d vec_max( vec.x , vec.y, vec.z );

		for( int ni = 1; ni < v_size; ni++ )
		{
			vec = geocentric_coords::from_spherical_d( v_src[ni] );
			m_vertex_array[ni].x = vec.x;
			m_vertex_array[ni].y = vec.y;
			m_vertex_array[ni].z = vec.z;
			m_vertex_array[ni].color = v_color;

			vec_min.x = min( vec_min.x, vec.x );
			vec_min.y = min( vec_min.y, vec.y );
			vec_min.z = min( vec_min.z, vec.z );

			vec_max.x = max( vec_max.x, vec.x );
			vec_max.y = max( vec_max.y, vec.y );
			vec_max.z = max( vec_max.z, vec.z );
		}
		m_render_aabb.set_extent( vec_min, vec_max );
		m_center = m_render_aabb.center();

		for ( int i = 0; i < v_size; i ++ )
		{
			m_vertex_array[i].x -= m_center.x;
			m_vertex_array[i].y -= m_center.y;
			m_vertex_array[i].z -= m_center.z; 
		}

		
	}

	
}