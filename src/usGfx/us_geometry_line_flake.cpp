
///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_geometry_line_ex.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : geometry_line_ex class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_geometry_line_flake.h"

namespace uniscope_globe
{
	geometry_line_flake::geometry_line_flake( void )
	{
		m_trans_matrix = matrix_4d::s_identity;
		m_collapse_matrix = matrix_4d::s_identity;
		m_depth_enable = false;
	}

	geometry_line_flake::~geometry_line_flake( void )
	{
		m_vertex_array.clear();
	}

	geometry_line_flake* geometry_line_flake::create_shared_instance( void )
	{
		geometry_line_flake* v_geometry = new geometry_line_flake();
		v_geometry->add_ref();
		return v_geometry;
	}

	void geometry_line_flake::draw( render_argument* args )
	{
		ulong vertices_count = m_vertex_array.size();
		if ( vertices_count < 2 )
			return;

		render_state rs( args->m_device );
		rs.set_state(D3DRS_CULLMODE, D3DCULL_NONE);
		if( !m_depth_enable )
		{
			rs.set_state(D3DRS_ZFUNC, D3DCMP_ALWAYS);	
		}


		matrix_4f v_world_mat = matrix_4f((double*)&(m_collapse_matrix * m_trans_matrix));
		v_world_mat.m41 = v_world_mat.m41 - cartesian_coords::s_reference_position_geo.x;
		v_world_mat.m42 = v_world_mat.m42 - cartesian_coords::s_reference_position_geo.y;
		v_world_mat.m43 = v_world_mat.m43 - cartesian_coords::s_reference_position_geo.z;

		//matrix_4f v_world_mat = matrix_4f::s_identity;
		//v_world_mat.m41 = m_trans_matrix.m41 - cartesian_coords::s_reference_position_geo.x;
		//v_world_mat.m42 = m_trans_matrix.m42 - cartesian_coords::s_reference_position_geo.y;
		//v_world_mat.m43 = m_trans_matrix.m43 - cartesian_coords::s_reference_position_geo.z;

		view_port v_view_port = args->m_device->get_view_port();

		matrix_4f v_mat_view = args->m_device->get_view_matrix();
		matrix_4f v_mat_project = args->m_device->get_projection_matrix();
		matrix_4f v_mat_view_project = v_mat_view * v_mat_project;

		args->m_device->set_vertex_declaration(position_normal_color::fvf);
		d3d9_effect_geometry_line_flake* v_render = (d3d9_effect_geometry_line_flake*)args->m_device->get_effect(US_EFFECT_GEOMETRY_LINE_FLAKE);
		v_render->set_technique();
		v_render->begin(NULL,NULL);
		{
			v_render->begin_pass(0);
			v_render->set_transform(v_world_mat * v_mat_view_project);	
			v_render->set_view_proj_matrix(v_mat_view_project);
			v_render->set_line_param(v_view_port.m_width, v_view_port.m_height, m_line_width);
			//v_render->set_line_color()
			v_render->commit_changes();
			args->m_device->draw_triangle_strip( (void*)m_vertex_array.begin()._Myptr, vertices_count , sizeof(position_normal_color) );
			v_render->end_pass();
		}
		v_render->end();
	}

	//void geometry_line_flake::draw( render_argument* args )
	//{
	//	ulong vertices_count = m_vertex_array.size();
	//	if ( vertices_count < 2 )
	//		return;

	//	render_state rs( args->m_device );
	//	//rs.set_state(D3DRS_ZENABLE, FALSE);
	//	rs.set_state(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	//	rs.set_state(D3DRS_CULLMODE, D3DCULL_NONE);

	//	matrix_4f v_world_mat = matrix_4f::s_identity;
	//	v_world_mat.m41 = m_trans_matrix.m41 - cartesian_coords::s_reference_position_geo.x;
	//	v_world_mat.m42 = m_trans_matrix.m42 - cartesian_coords::s_reference_position_geo.y;
	//	v_world_mat.m43 = m_trans_matrix.m43 - cartesian_coords::s_reference_position_geo.z;

	//	view_port v_view_port = args->m_device->get_view_port();

	//	matrix_4f v_mat_view = args->m_device->get_view_matrix();
	//	matrix_4f v_mat_project = args->m_device->get_projection_matrix();
	//	matrix_4f v_mat_view_project = v_mat_view * v_mat_project;
	//	float v_near_plane = args->m_observer->get_near_plane();


	//	std::vector<transformed_color> v_vertex_array;
	//	v_vertex_array.resize(vertices_count);
	//	int n_real_count = project_line( v_vertex_array, m_vertex_array, v_view_port, v_mat_project, v_mat_view, v_world_mat, v_near_plane );

	//	args->m_device->set_vertex_declaration(transformed_color::fvf);
	//	d3d9_effect_geometry_line* v_render = (d3d9_effect_geometry_line*)args->m_device->get_effect(US_EFFECT_GEOMETRY_LINE);
	//	v_render->set_technique();
	//	v_render->begin(NULL,NULL);
	//	{
	//		v_render->begin_pass(0);
	//		v_render->set_transform(v_world_mat * v_mat_view_project);	
	//		v_render->set_view_proj_matrix(v_mat_view_project);
	//		v_render->set_line_param(v_view_port.m_width, v_view_port.m_height, m_line_width);
	//		//v_render->set_line_color()
	//		v_render->commit_changes();
	//		args->m_device->draw_triangle_strip( (void*)v_vertex_array.begin()._Myptr, vertices_count , sizeof(transformed_color) );
	//		v_render->end_pass();
	//	}
	//	v_render->end();
	//}


	int geometry_line_flake::project_line( std::vector<transformed_color>& out_vec_array, std::vector<position_normal_color>& in_vec_array, view_port& viewport, matrix_4f& proj_mat, matrix_4f& view_mat, matrix_4f& world_mat, float near_plane )
	{
		matrix_4f v_mat_world_view = world_mat * view_mat;

		int n_count = in_vec_array.size();
		for ( int ni = 0; ni < n_count - 1; ni++ )
		{
			vector3<float> vec1 = vector3<float>(in_vec_array[ni].x, in_vec_array[ni].y, in_vec_array[ni].z);
			vector3<float> vec2 = vector3<float>(in_vec_array[ni+1].x, in_vec_array[ni+1].y, in_vec_array[ni+1].z);

			vector3<float> v1 = vec1 * v_mat_world_view;
			vector3<float> v2 = vec2 * v_mat_world_view;
			if( v1.z < 0 && v2.z < 0)
			{
				continue;
			}
			else if( v2.z < 0 )
			{
				vector3<float> v3 = vector3<float>::normalize(v2 - v1);
				v2 = v3 * (v2-v1).length() * fabs(( v1.z - near_plane) / (v1.z - v2.z)) + v1;
			}
			else if(  v1.z < 0 )
			{
				vector3<float> v3 = vector3<float>::normalize(v1 - v2);
				v1 = v3 * (v1-v2).length() * fabs(( v2.z - near_plane) / (v2.z - v1.z)) + v2;
			}

			{
				vector3<float> normal;
				normal.x = in_vec_array[ni].nx;
				normal.y = in_vec_array[ni].ny;
				normal.z = in_vec_array[ni].nz;

				float v_line_length = normal.length();

				//////////////////////////////////////////////////////////////
				normal.normalize();
				normal = matrix_4f::rotate_normal(normal, view_mat * proj_mat);

				normal.normalize();
				//normal.y = -normal.y;
				//normal.z = 0;
				normal.normalize();
				normal = vector_3f::cross(vector_3f(0,0,1), normal);
				normal.normalize();
				normal.y = normal.y;
				normal.z = 0;

				//////////////////////////////////////////////
				vector_3f vec_trans = matrix_4f::transform_coordinate( v1 + normal * m_line_width * 0.001 , proj_mat);
				out_vec_array[ni].x   = viewport.m_x +  ( 1.0f + vec_trans.x ) * viewport.m_width / 2.0f;
				out_vec_array[ni].y   = viewport.m_y +  ( 1.0f - vec_trans.y ) * viewport.m_height / 2.0f;
				out_vec_array[ni].z   = 0;
				out_vec_array[ni].rhw = 1.0;
				out_vec_array[ni].color = in_vec_array[ni].color;

			}

			{
				vector3<float> normal;
				normal.x = in_vec_array[ni+1].nx;
				normal.y = in_vec_array[ni+1].ny;
				normal.z = in_vec_array[ni+1].nz;

				float v_line_length = normal.length();

				//////////////////////////////////////////////////////////////
				normal.normalize();
				normal = matrix_4f::rotate_normal(normal, view_mat * proj_mat);

				normal.normalize();
				//normal.y = -normal.y;
				//normal.z = 0;
				normal.normalize();
				normal = vector_3f::cross(vector_3f(0,0,1), normal);
				normal.normalize();
				normal.y = normal.y;
				normal.z = 0;

				//////////////////////////////////////////////

				vector_3f vec_trans = matrix_4f::transform_coordinate( v2 + normal * m_line_width * 0.001, proj_mat);
				out_vec_array[ni+1].x   = viewport.m_x +  ( 1.0f + vec_trans.x ) * viewport.m_width / 2.0f;
				out_vec_array[ni+1].y   = viewport.m_y +  ( 1.0f - vec_trans.y ) * viewport.m_height / 2.0f;
				out_vec_array[ni+1].z   = 0;
				out_vec_array[ni+1].rhw = 1.0;
				out_vec_array[ni+1].color = in_vec_array[ni+1].color;
			}
		}
		return 0;
	}

	//void geometry_line_flake::draw( render_argument* args )
	//{
	//	ulong vertices_count = m_vertex_array.size();
	//	if ( vertices_count < 2 )
	//		return;

	//	render_state rs( args->m_device );
	//	//rs.set_state(D3DRS_ZENABLE, FALSE);
	//	rs.set_state(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	//	rs.set_state(D3DRS_CULLMODE, D3DCULL_NONE);

	//	matrix_4f v_world_mat = matrix_4f::s_identity;
	//	v_world_mat.m41 = m_trans_matrix.m41 - cartesian_coords::s_reference_position_geo.x;
	//	v_world_mat.m42 = m_trans_matrix.m42 - cartesian_coords::s_reference_position_geo.y;
	//	v_world_mat.m43 = m_trans_matrix.m43 - cartesian_coords::s_reference_position_geo.z;

	//	view_port v_view_port = args->m_device->get_view_port();

	//	matrix_4f v_mat_view = args->m_device->get_view_matrix();
	//	matrix_4f v_mat_project = args->m_device->get_projection_matrix();
	//	matrix_4f v_mat_view_project = v_mat_view * v_mat_project;

	//	matrix_4f mat = v_world_mat * v_mat_view_project;
	//	vector_3f v_direction = vector3<float>((double*)&args->m_observer->get_direction_geo());
	//	std::vector<position_normal_color> v_vertex_array;
	//	v_vertex_array.resize(vertices_count);
	//	for(int ni = 0; ni < vertices_count; ni++ )
	//	{
	//		vector3<float> vec;
	//		vec.x = m_vertex_array[ni].x;
	//		vec.y = m_vertex_array[ni].y;
	//		vec.z = m_vertex_array[ni].z;
	//		float w = 1.0;

	//		vector3<float> pos;
	//		{
	//			pos.x = mat.m11 * vec.x + mat.m21 * vec.y + mat.m31 * vec.z + mat.m41;
	//			pos.y = mat.m12 * vec.x + mat.m22 * vec.y + mat.m32 * vec.z + mat.m42;
	//			pos.z = mat.m13 * vec.x + mat.m23 * vec.y + mat.m33 * vec.z + mat.m43;
	//			w	  = mat.m14 * vec.x + mat.m24 * vec.y + mat.m34 * vec.z + mat.m44;

	//			//assert( w != float(0.0) );
	//			//float det = float(1.0)/w;

	//			//pos.x *= det;
	//			//pos.y *= det;
	//			//pos.z *= det;	

	//		}
	//		
	//		//vector_3f vec = matrix_4f::transform_coordinate( vec, mat );
	//		
	//		//pos = this->project( pos, v_view_port, v_world_mat * v_mat_view_project );

	//		vector3<float> normal;
	//		normal.x = m_vertex_array[ni].nx;
	//		normal.y = m_vertex_array[ni].ny;
	//		normal.z = m_vertex_array[ni].nz;

	//		float v_line_length = normal.length();

	//		//////////////////////////////////////////////////////////////
	//		normal.normalize();
	//		normal = matrix_4f::rotate_normal(normal, v_mat_view_project);

	//		normal.normalize();
	//		//normal.y = -normal.y;
	//		//normal.z = 0;
	//		normal.normalize();
	//		normal = vector_3f::cross(vector_3f(0,0,1), normal);
	//		normal.normalize();
	//		normal.y = normal.y;
	//		normal.z = 0;

	//		//////////////////////////////////////////////////////////////
	//		//normal = vector_3f::cross(vec, normal);
	//		//normal.normalize();
	//		//normal = matrix_4f::rotate_normal(normal, v_mat_view_project);
	//		//normal.normalize();
	//		//normal.y = -normal.y;
	//		//normal.z = 0;
	//		//normal.normalize();
	//		//////////////////////////////////////////////////////////////
	//		normal.x = normal.x * m_line_width * w * v_line_length / (float)v_view_port.m_width;
	//		normal.y = normal.y * m_line_width * w * v_line_length / (float)v_view_port.m_height;

	//		vector3<float> out_pos = pos + normal;
	//		v_vertex_array[ni].x = out_pos.x ;
	//		v_vertex_array[ni].y = out_pos.y;
	//		v_vertex_array[ni].z = out_pos.z;
	//		v_vertex_array[ni].nx = w;
	//		//v_vertex_array[ni].rhw = 1.0f;
	//		v_vertex_array[ni].color = m_vertex_array[ni].color;

	//		continue;
	//	}
	//	args->m_device->set_vertex_declaration(position_normal_color::fvf);
	//	d3d9_effect_geometry_line* v_render = (d3d9_effect_geometry_line*)args->m_device->get_effect(US_EFFECT_GEOMETRY_LINE);
	//	v_render->set_technique();
	//	v_render->begin(NULL,NULL);
	//	{
	//		v_render->begin_pass(0);
	//		v_render->set_transform(v_world_mat * v_mat_view_project);	
	//		v_render->set_view_proj_matrix(v_mat_view_project);
	//		v_render->set_line_param(v_view_port.m_width, v_view_port.m_height, m_line_width);
	//		//v_render->set_line_color()
	//		v_render->commit_changes();
	//		args->m_device->draw_triangle_strip( (void*)m_vertex_array.begin()._Myptr, vertices_count , sizeof(position_normal_color) );
	//		v_render->end_pass();
	//	}
	//	v_render->end();


	//}


	vector_3f geometry_line_flake::project2(vector_3f& vector_in_world, view_port& viewport, matrix_4f& wvp)
	{
		//vector_3f v_out;
		//vector_3f vec_trans = matrix_4f::transform_coordinate( vec, wvp);

		//v_out.x = viewport.m_x +  ( 1.0f + vec_trans.x ) * viewport.m_width / 2.0f;
		//v_out.y = viewport.m_y +  ( 1.0f - vec_trans.y ) * viewport.m_height / 2.0f;
		//v_out.z = viewport.m_z_min + vec_trans.z * ( viewport.m_z_max - viewport.m_z_min );
		//return v_out;

		vector3<float> ret_vec;
		D3DXVec3Project((D3DXVECTOR3*)&ret_vec, 
						(D3DXVECTOR3*)&vector_in_world, 
						(D3DVIEWPORT9*)&viewport,
						(D3DXMATRIX*)&matrix4<float>::s_identity, 
						(D3DXMATRIX*)&matrix4<float>::s_identity, 
						(D3DXMATRIX*)&matrix4<float>::s_identity);
		return ret_vec;

	}


	vector_3f geometry_line_flake::project(vector_3f& vec, view_port& viewport, matrix_4f& wvp)
	{
		vector_3f v_out;
		vector_3f vec_trans = matrix_4f::transform_coordinate( vec, wvp);

		v_out.x = viewport.m_x +  ( 1.0f + vec_trans.x ) * viewport.m_width / 2.0f;
		v_out.y = viewport.m_y +  ( 1.0f - vec_trans.y ) * viewport.m_height / 2.0f;
		v_out.z = viewport.m_z_min + vec_trans.z * ( viewport.m_z_max - viewport.m_z_min );
		return v_out;
	}

	//void geometry_line_flake::draw( render_argument* args )
	//{
	//	ulong vertices_count = m_vertex_array.size();
	//	if ( vertices_count < 2 )
	//		return;

	//	args->m_device->set_vertex_declaration( position_normal_color::fvf );
	//	args->m_device->set_texture( 0, NULL );
	//	render_state rs( args->m_device );
	//	rs.set_state(D3DRS_ZENABLE, FALSE);

	//	//matrix_4d v_mat = m_collapse_matrix * m_trans_matrix;
	//	matrix_4d v_mat = matrix_4d::s_identity;


	//	v_mat.m41 = m_trans_matrix.m41 - cartesian_coords::s_reference_position_geo.x;
	//	v_mat.m42 = m_trans_matrix.m42 - cartesian_coords::s_reference_position_geo.y;
	//	v_mat.m43 = m_trans_matrix.m43 - cartesian_coords::s_reference_position_geo.z;

	//	args->m_device->push_transform();
	//	args->m_device->multiply_transform( matrix_4f(v_mat.m) );

	//	//args->m_device->draw_point_list( (void*)m_vertex_array.begin()._Myptr, vertices_count , sizeof(position_normal_color) );
	//	args->m_device->draw_line_strip( (void*)m_vertex_array.begin()._Myptr, vertices_count , sizeof(position_normal_color) );

	//	args->m_device->pop_transform();
	//}



	//void geometry_line_flake::create_from_line_string( vector_3d* v_src, int v_size, ulong v_color )
	//{
	//	m_vertex_array.clear();
	//	m_vertex_array.resize( v_size );

	//	vector_3d vec = geocentric_coords::from_spherical_d( v_src[0] );
	//	m_vertex_array[0].x = vec.x;
	//	m_vertex_array[0].y = vec.y;
	//	m_vertex_array[0].z = vec.z;
	//	m_vertex_array[0].color = v_color;

	//	vector_3d vec_min( vec.x , vec.y, vec.z );
	//	vector_3d vec_max( vec.x , vec.y, vec.z );

	//	for( int ni = 1; ni < v_size; ni++ )
	//	{
	//		vec = geocentric_coords::from_spherical_d( v_src[ni] );
	//		m_vertex_array[ni].x = vec.x;
	//		m_vertex_array[ni].y = vec.y;
	//		m_vertex_array[ni].z = vec.z;
	//		m_vertex_array[ni].color = v_color;

	//		vec_min.x = min( vec_min.x, vec.x );
	//		vec_min.y = min( vec_min.y, vec.y );
	//		vec_min.z = min( vec_min.z, vec.z );

	//		vec_max.x = max( vec_max.x, vec.x );
	//		vec_max.y = max( vec_max.y, vec.y );
	//		vec_max.z = max( vec_max.z, vec.z );
	//	}
	//	m_render_aabb.set_extent( vec_min, vec_max );
	//	m_center = m_render_aabb.center();

	//	for ( int i = 0; i < v_size; i ++ )
	//	{
	//		m_vertex_array[i].x -= m_center.x;
	//		m_vertex_array[i].y -= m_center.y;
	//		m_vertex_array[i].z -= m_center.z; 
	//	}
	//}

	//bool geometry_line_flake::intersect( const point& pt, intersect_result& out_result )
	//{
	//	bool ret_val = false;
	//	float min_distance = 99999999999;
	//	vector3<float> vec_screen( pt.x, pt.y, 0);
	//	int n_count =  m_vertex_array.size();

	//	std::vector<vector3<float>> v_vec_screen_array;
	//	v_vec_screen_array.resize(n_count);
	//	matrix_4f v_world_mat = matrix_4f::s_identity;
	//	v_world_mat.m41 = m_trans_matrix.m41 - cartesian_coords::s_reference_position_geo.x;
	//	v_world_mat.m42 = m_trans_matrix.m42 - cartesian_coords::s_reference_position_geo.y;
	//	v_world_mat.m43 = m_trans_matrix.m43 - cartesian_coords::s_reference_position_geo.z;

	//	
	//	for ( int ni = 0; ni < n_count; ni++ )
	//	{
	//		vector3<float> vec;
	//		vec.x = m_vertex_array[ni].x;
	//		vec.y = m_vertex_array[ni].y;
	//		vec.z = m_vertex_array[ni].z;


	//		v_vec_screen_array[ni] = m_render->get_device()->projection(vec * v_world_mat);
	//		v_vec_screen_array[ni].z = 0;
	//	}

	//	for( int ni = 0; ni < n_count - 1; ni++ )
	//	{
	//		float v_distance = d3d9_intersect_helper::point_to_segment_distance( vec_screen, v_vec_screen_array[ni], v_vec_screen_array[ni+1] );
	//		if( v_distance < 5 && v_distance < min_distance)
	//		{
	//			min_distance = v_distance;
	//			ret_val = true;
	//		}
	//	}
	//	return ret_val;
	//}


	
}