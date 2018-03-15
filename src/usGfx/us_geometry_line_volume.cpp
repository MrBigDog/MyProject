
///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_geometry_line_volume.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : geometry_line_volume class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_geometry_line_volume.h"

namespace uniscope_globe
{
	geometry_line_volume::geometry_line_volume( void )
	{
		m_shadow_type = US_SHADOW_NONE;
		m_trans_matrix = matrix_4d::s_identity;
		m_collapse_matrix = matrix_4d::s_identity;
		m_depth_enable = false;
	}

	geometry_line_volume::~geometry_line_volume( void )
	{
		m_vertex_array.clear();
		m_index_array.clear();
	}
	
	geometry_line_volume* geometry_line_volume::create_shared_instance( void )
	{
		geometry_line_volume* v_geometry = new geometry_line_volume();
		v_geometry->add_ref();
		return v_geometry;
	}

	void geometry_line_volume::draw( render_argument* args )
	{
		ulong vertices_count = m_vertex_array.size();
		if ( vertices_count < 2 ) return;

		matrix_4d v_world_mat = m_collapse_matrix * m_trans_matrix;
		v_world_mat.m41 = v_world_mat.m41 - cartesian_coords::s_reference_position_geo.x;
		v_world_mat.m42 = v_world_mat.m42 - cartesian_coords::s_reference_position_geo.y;
		v_world_mat.m43 = v_world_mat.m43 - cartesian_coords::s_reference_position_geo.z;

		double v_sin_tilt = math<double>::fabs_(math<double>::sin_(args->m_observer->get_tilt())); 
		if( v_sin_tilt < 0.1) v_sin_tilt = 0.1;
		double v_line_width = args->m_observer->get_pixel_to_geo_scale_by_height() * m_line_width / v_sin_tilt;
		
		////////////////////////////////////////////////////////////////////////////////
		d3d9_effect_shadow_volume* v_render = (d3d9_effect_shadow_volume*)args->m_render;
		v_render->push_transform();
		v_render->multiply_transform( matrix_4f( v_world_mat.m ) );

		//v_render->set_world_matrix(v_world_mat);	
		v_render->set_line_width(v_line_width * 0.5);
		v_render->commit_changes();
		args->m_device->set_vertex_declaration(position_normal_color::fvf);
		args->m_device->draw_indexed_triangle_list(D3DFMT_INDEX16, (void*)m_vertex_array.begin()._Myptr, vertices_count , (void*)m_index_array.begin()._Myptr, m_index_array.size(), sizeof(position_normal_color) );
		
		v_render->pop_transform();
		//d3d9_effect_geometry_line_volume* v_render = (d3d9_effect_geometry_line_volume*)args->m_device->get_effect(US_EFFECT_GEOMETRY_LINE_VOLUME);
		//
		//v_render->set_technique();
		//v_render->begin(NULL,NULL);
		//{
		//	v_render->begin_pass(0);
		//	v_render->set_transform(v_world_mat * v_mat_view_project);	
		//	v_render->set_view_proj_matrix(v_mat_view_project);
		//	v_render->set_line_width(v_line_width);
		//	//v_render->set_line_color()
		//	v_render->commit_changes();
		//	args->m_device->draw_indexed_triangle_list(D3DFMT_INDEX16, (void*)m_vertex_array.begin()._Myptr, vertices_count , (void*)m_index_array.begin()._Myptr, m_index_array.size(), sizeof(position_normal_color) );
		//	v_render->end_pass();
		//}
		//v_render->end();
	}

	//void geometry_line_volume::draw( render_argument* args )
	//{
	//	args->m_device->set_texture( 0, NULL );

	//	d3d9_mesh_render* v_mesh_render = (d3d9_mesh_render*)args->m_render;

	//	matrix_4d v_mat = m_collapse_matrix * m_trans_matrix;
	//	v_mat.m41 = v_mat.m41 - cartesian_coords::s_reference_position_geo.x;
	//	v_mat.m42 = v_mat.m42 - cartesian_coords::s_reference_position_geo.y;
	//	v_mat.m43 = v_mat.m43 - cartesian_coords::s_reference_position_geo.z;
	//
	//	args->m_device->set_vertex_declaration( position_normal_color::fvf );

	//	v_mesh_render->push_transform();

	//	v_mesh_render->multiply_transform( matrix_4f( v_mat.m ) );

	//	v_mesh_render->commit_changes();
	//	args->m_device->draw_indexed_triangle_list( D3DFMT_INDEX16, m_vertex_array.begin()._Myptr, m_vertex_array.size(),
	//										m_index_array.begin()._Myptr, m_index_array.size(), position_color::stride );	
	//	v_mesh_render->pop_transform();
	//}
	
	void geometry_line_volume::clear()
	{
		m_vertex_array.clear();
		m_index_array.clear();

		m_collapse_matrix = matrix_4d::s_identity;
		m_trans_matrix = matrix_4d::s_identity;

		m_render_aabb.make_invalid();
	}

	bool geometry_line_volume::intersect( const ray<double>& a_ray, intersect_result& result )
	{
		bool b_ret = false;
		//ray<double> in_ray = a_ray;

		//vector_3d pick_point;
		//double pick_length = 0.0;
		//for ( int i = 0; i < m_index_array.size(); i+=3 )
		//{
		//	vector_3d vec1( m_vertex_array[m_index_array[i]].x, m_vertex_array[m_index_array[i]].y, m_vertex_array[m_index_array[i]].z );
		//	vector_3d vec2( m_vertex_array[m_index_array[i+1]].x, m_vertex_array[m_index_array[i+1]].y, m_vertex_array[m_index_array[i+1]].z );
		//	vector_3d vec3( m_vertex_array[m_index_array[i+2]].x, m_vertex_array[m_index_array[i+2]].y, m_vertex_array[m_index_array[i+2]].z );

		//	triangle_d tri( vec1, vec2, vec3 );
		//	bool b_pick = triangle_d::intersect( in_ray, tri );
		//	if ( triangle_d::pick( in_ray, tri, pick_point, 10000000.0 ) )
		//	{
		//		pick_length = (pick_point - in_ray.ray_origin).length();
		//		if ( result.m_distance > pick_length )
		//		{
		//			result.m_distance = pick_length;
		//			result.m_position = pick_point + m_center;
		//		}

		//		b_ret = true;
		//	}
		//}

		return b_ret;
	}
}