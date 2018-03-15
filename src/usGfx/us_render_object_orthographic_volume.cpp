
///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_render_object_orthographic_volume.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : render_object_orthographic_volume class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_render_object_orthographic_volume.h"

namespace uniscope_globe
{
	render_object_orthographic_volume::render_object_orthographic_volume( void )
	{
	}

	render_object_orthographic_volume::~render_object_orthographic_volume( void )
	{
	}

	render_object_orthographic_volume* render_object_orthographic_volume::create_shared_instance()
	{
		render_object_orthographic_volume* v_geometry = new render_object_orthographic_volume();
		v_geometry->add_ref();
		return v_geometry;
	}

	void render_object_orthographic_volume::draw( render_argument* args )
	{
		d3d9_effect_common_mesh* v_mesh_render = (d3d9_effect_common_mesh*)args->m_render;

		matrix_4d v_mat = matrix_4d::s_identity;
		v_mesh_render->push_transform();
		v_mesh_render->multiply_transform( matrix_4f( v_mat.m ) );
		v_mesh_render->commit_changes();

		render_helper::draw_filled_frustum( args->m_device, args->m_observer, 0x99ff0000 );

		//view_port v_view_port = args->m_device->get_view_port();
		//vector3<float> vec1 = args->m_device->unprojection( vector3<float>(0, v_view_port.m_height * 0.5, 0));
		//vector3<float> vec2 = args->m_device->unprojection( vector3<float>(v_view_port.m_width, v_view_port.m_height * 0.5, 0));

		//double v_width = ( vec2 - vec1 ).length();
		//
		//double v_height = v_width / args->m_observer->get_aspect();
		//frustum<double> m_frustum;
		//m_frustum.update_ex( v_width, v_height, args->m_observer->get_near_plane(), args->get_far_plane(), args->m_observer->get_view_matrix() );

		
		//args->m_device->set_vertex_declaration( position_color::fvf );
		//args->m_device->set_texture( 0, NULL );

		//matrix_4d v_mat = m_collapse_matrix * m_trans_matrix;
		//v_mat.m41 = v_mat.m41 - cartesian_coords::s_reference_position_geo.x;
		//v_mat.m42 = v_mat.m42 - cartesian_coords::s_reference_position_geo.y;
		//v_mat.m43 = v_mat.m43 - cartesian_coords::s_reference_position_geo.z;

		//args->m_device->push_transform();
		//args->m_device->multiply_transform( matrix_4f(v_mat.m) );

		//args->m_device->draw_line_strip( (void*)m_vertex_array[0], vertices_count , sizeof(position_color) );

		//args->m_device->pop_transform();

		v_mesh_render->pop_transform();
	}	
}