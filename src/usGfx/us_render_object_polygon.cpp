///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_render_object_polygon.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : render_object_polygon class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_render_object_polygon.h"

namespace uniscope_globe
{
	render_object_polygon::render_object_polygon( void )
	{
		m_trans_matrix = matrix_4d::s_identity;
		m_collapse_matrix = matrix_4d::s_identity;
		m_center = vector_3d::s_zero;
	}

	render_object_polygon::~render_object_polygon( void )
	{
		m_line_array.clear();

		m_vertex_array.clear();
		m_triangle_index.clear();
	}

	render_object_polygon* render_object_polygon::create_shared_instance( void )
	{
		render_object_polygon* v_geometry = new render_object_polygon();
		v_geometry->add_ref();
		return v_geometry;
	}

	void render_object_polygon::draw( render_argument* args )
	{
		ulong vertices_count = m_vertex_array.size();
		
		render_state v_render_state( args->m_device );
		v_render_state.set_state( D3DRS_CULLMODE, D3DCULL_NONE );
		if( !m_depth_enable )
		{
			v_render_state.set_state(D3DRS_ZFUNC, D3DCMP_ALWAYS);	
		}

		args->m_device->set_vertex_declaration( position_color::fvf );
		args->m_device->set_texture( 0, NULL );

		matrix_4d v_mat = m_collapse_matrix * m_trans_matrix;
		v_mat.m41 = v_mat.m41 - cartesian_coords::s_reference_position_geo.x;
		v_mat.m42 = v_mat.m42 - cartesian_coords::s_reference_position_geo.y;
		v_mat.m43 = v_mat.m43 - cartesian_coords::s_reference_position_geo.z;

		args->m_device->push_transform();
		args->m_device->multiply_transform( matrix_4f(v_mat.m) );

		//args->m_device->draw_line_strip( (void*)m_line_array[0], vertices_count , sizeof(position_color) );
		
		args->m_device->draw_indexed_triangle_list( D3DFMT_INDEX16, (void*)&m_vertex_array[0], vertices_count,
											(ushort*)&m_triangle_index[0], m_triangle_index.size(),
											sizeof(position_color) );

		args->m_device->pop_transform();
	}

	void render_object_polygon::set_collapse( matrix_4d& mat )
	{
		m_collapse_matrix = m_collapse_matrix * mat;
	}

	void render_object_polygon::clear()
	{
		m_vertex_array.clear();
		m_triangle_index.clear();
		m_line_array.clear();
	}	
}