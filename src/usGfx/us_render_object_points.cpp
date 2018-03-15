///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_render_object_points.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : render_object_points class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_render_object_points.h"

namespace uniscope_globe
{
	render_object_points::render_object_points( void )
	{
		m_mat = matrix_4f::s_identity;
	}

	render_object_points::~render_object_points( void )
	{
		m_vertex_array.clear();
	}

	void render_object_points::draw( render_argument* args )
	{
		ulong vertices_count = (ulong)m_vertex_array.size();
		if ( vertices_count < 1 )
			return;

		args->m_device->set_vertex_declaration( position_color::fvf );
		args->m_device->set_texture( 0, NULL );

		float v_point_size = 5.0f;
		render_state v_render_state(args->m_device);
		v_render_state.set_state( D3DRS_POINTSIZE, *((DWORD*)&v_point_size) );
		v_render_state.set_state( D3DRS_POINTSPRITEENABLE, TRUE );

		matrix_4f v_mat = m_mat;
		vector_3d v_center = m_render_aabb.center();
		v_mat.m41 = v_mat.m41 + v_center.x - cartesian_coords::s_reference_position_geo.x;
		v_mat.m42 = v_mat.m42 + v_center.y - cartesian_coords::s_reference_position_geo.y;
		v_mat.m43 = v_mat.m43 + v_center.z -cartesian_coords::s_reference_position_geo.z;

		args->m_device->push_transform();
		args->m_device->multiply_transform( v_mat );

		args->m_device->draw_point_list( (void*)&m_vertex_array[0], vertices_count , sizeof(position_color) );

		args->m_device->pop_transform();
	}

	void render_object_points::set_matrix( matrix_4f& mat )
	{
		m_mat = mat;
	}

	void render_object_points::create_from_point_list( vector_3d* v_src, int v_size, ulong v_color )
	{
		m_vertex_array.clear();
		m_vertex_array.resize( v_size );

		m_render_aabb.make_invalid();
		for( int ni = 0; ni < v_size; ni++ )
		{
			m_render_aabb.combine( v_src[ni] );
		}

		vector_3d v_center = m_render_aabb.center();
		for( int ni = 0; ni < v_size; ni++ )
		{
			m_vertex_array[ni].x = v_src[ni].x - v_center.x;
			m_vertex_array[ni].y = v_src[ni].y - v_center.y;
			m_vertex_array[ni].z = v_src[ni].z - v_center.z;
			m_vertex_array[ni].color = v_color;

			m_render_aabb.combine( v_src[ni] );
		}
	}

	
}