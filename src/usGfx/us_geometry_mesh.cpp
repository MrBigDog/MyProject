///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_geometry_mesh.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : geometry_mesh class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_geometry_mesh.h"

namespace uniscope_globe
{
	geometry_mesh::geometry_mesh( void )
	{
		m_center = vector_3d::s_zero;

		m_fvf = position_color::fvf;
		m_shadow_type = US_SHADOW_NONE;
		m_trans_matrix = matrix_4d::s_identity;
		m_collapse_matrix = matrix_4d::s_identity;
	}

	geometry_mesh::~geometry_mesh( void )
	{
		m_vertex_array.clear();
		m_index_array.clear();
	}

	geometry_mesh* geometry_mesh::create_shared_instance()
	{
		geometry_mesh* v_geometry = new geometry_mesh();
		v_geometry->add_ref();
		return v_geometry;
	}

	void geometry_mesh::draw_figure( render_device* device, matrix_4f& in_mat )
	{
		if ( m_frame_index_array.size() > 0 && m_frame_vertex_array.size() > 0 )
		{
			device->set_texture( 0, NULL );

			matrix_4f v_mat = in_mat;
			v_mat.m41 = in_mat.m41 - cartesian_coords::s_reference_position_geo.x;
			v_mat.m42 = in_mat.m42 - cartesian_coords::s_reference_position_geo.y;
			v_mat.m43 = in_mat.m43 - cartesian_coords::s_reference_position_geo.z;

			device->set_vertex_declaration( position_color::fvf );
			device->push_transform();

			device->multiply_transform( v_mat );

			render_helper::draw_indexed_primitive_up (  device, D3DPT_LINELIST, 0, 
				m_frame_vertex_array.begin()._Myptr, m_frame_index_array.begin()._Myptr, 
				m_frame_vertex_array.size(), m_frame_index_array.size() / 2, position_color::stride );

			device->pop_transform();
		}
	}

	void geometry_mesh::draw( render_argument* args )
	{
		args->m_device->set_texture( 0, NULL );

		d3d9_mesh_render* v_mesh_render = (d3d9_mesh_render*)args->m_render;

		matrix_4d v_mat = m_collapse_matrix * m_trans_matrix;
		v_mat.m41 = v_mat.m41 - cartesian_coords::s_reference_position_geo.x;
		v_mat.m42 = v_mat.m42 - cartesian_coords::s_reference_position_geo.y;
		v_mat.m43 = v_mat.m43 - cartesian_coords::s_reference_position_geo.z;
	
		args->m_device->set_vertex_declaration( m_fvf );

		v_mesh_render->push_transform();

		v_mesh_render->multiply_transform( matrix_4f( v_mat.m ) );

		v_mesh_render->commit_changes();
		args->m_device->draw_indexed_triangle_list( D3DFMT_INDEX16, m_vertex_array.begin()._Myptr, m_vertex_array.size(),
											m_index_array.begin()._Myptr, m_index_array.size(), position_color::stride );
		
		if ( m_frame_index_array.size() > 0 && m_frame_vertex_array.size() > 0 )
		{
			render_helper::draw_indexed_primitive_up (  args->m_device, D3DPT_LINELIST, 0, 
														m_frame_vertex_array.begin()._Myptr, m_frame_index_array.begin()._Myptr, 
														m_frame_vertex_array.size(), m_frame_index_array.size() / 2, position_color::stride );
		}
		
		v_mesh_render->pop_transform();
	}
	
	void geometry_mesh::clear()
	{
		m_vertex_array.clear();
		m_index_array.clear();

		m_frame_vertex_array.clear();
		m_frame_index_array.clear();

		m_collapse_matrix = matrix_4d::s_identity;
		m_trans_matrix = matrix_4d::s_identity;

		m_render_aabb.make_invalid();
	}

	bool geometry_mesh::intersect( const ray<double>& a_ray, intersect_result& result )
	{
		bool b_ret = false;
		ray<double> in_ray = a_ray;

		//in_ray.ray_origin -= m_center;
		//in_ray.ray_origin.x -= m_collapse_matrix.m41;
		//in_ray.ray_origin.y -= m_collapse_matrix.m42;
		//in_ray.ray_origin.z -= m_collapse_matrix.m43;

		vector_3d pick_point;
		double pick_length = 0.0;
		for ( int i = 0; i < m_index_array.size(); i+=3 )
		{
			vector_3d vec1( m_vertex_array[m_index_array[i]].x, m_vertex_array[m_index_array[i]].y, m_vertex_array[m_index_array[i]].z );
			vector_3d vec2( m_vertex_array[m_index_array[i+1]].x, m_vertex_array[m_index_array[i+1]].y, m_vertex_array[m_index_array[i+1]].z );
			vector_3d vec3( m_vertex_array[m_index_array[i+2]].x, m_vertex_array[m_index_array[i+2]].y, m_vertex_array[m_index_array[i+2]].z );

			triangle_d tri( vec1, vec2, vec3 );
			bool b_pick = triangle_d::intersect( in_ray, tri );
			if ( triangle_d::pick( in_ray, tri, pick_point, 10000000.0 ) )
			{
				pick_length = (pick_point - in_ray.ray_origin).length();
				if ( result.m_distance > pick_length )
				{
					result.m_distance = pick_length;
					result.m_position = pick_point + m_center;
				}

				b_ret = true;
			}
		}

		return b_ret;
	}

	void geometry_mesh::set_collapse( matrix_4d& mat )
	{
		m_collapse_matrix = m_collapse_matrix * mat;
	}

	render_object* geometry_mesh::get_shadow_object( void )
	{
		return this;
	}

//	void geometry_mesh::create_from_vertex_and_index( vector_3d* v_vertex, int v_vertex_size, ulong in_color, vector3<ushort>* v_index, int v_index_num )
//	{
//		if ( v_vertex == NULL || v_index == NULL )
//			return;
//
//		m_shadow_color = in_color;
//
//		m_vertex_array.clear();
//		m_index_array.clear();
//
//		m_vertex_array.resize( v_vertex_size );
//		m_index_array.resize( v_index_num * 3 );
//	
//		m_render_aabb.make_invalid();
//		for ( int i = 0; i < v_vertex_size; i++ )
//		{
//			m_render_aabb.combine( v_vertex[i] );
//		}
//		
//		m_center = ( m_render_aabb.get_max() + m_render_aabb.get_min() ) * 0.5;
//		for ( int i = 0; i < v_vertex_size; i++ )
//		{
//			m_vertex_array[i].x = v_vertex[i].x - m_center.x;		
//			m_vertex_array[i].y = v_vertex[i].y - m_center.y;
//			m_vertex_array[i].z = v_vertex[i].z - m_center.z;
//			m_vertex_array[i].color = in_color;
//		}
//
//		for ( int i = 0; i < v_index_num; i++ )
//		{
//			m_index_array[i*3] = v_index[i].x;		
//			m_index_array[i*3+1] = v_index[i].y;
//			m_index_array[i*3+2] = v_index[i].z;
//		}
//	}
//
//	void geometry_mesh::create_from_line( vector_3d* v_src, int v_size, double v_width, ulong v_color, double v_height )
//	{
//		m_render_aabb.make_invalid();
//		m_index_array.clear();
//		m_vertex_array.clear();
//
//		m_shadow_color = v_color;
//		generate_line( v_src, v_size, v_width, v_height, v_color );
//	}
//
//	void geometry_mesh::create_from_multiple_line( vector3<double>* vertex_buffer, ulong vertex_size, ushort* index_buffer, ulong index_size, double in_width, ulong in_color, double in_height )
//	{
//		m_shadow_color = in_color;
//		m_render_aabb.make_invalid();
//
//		m_index_array.clear();
//		m_vertex_array.clear();
//
//		ulong cur_index = 0;
//		for ( ulong i = 0; i < index_size; i++ )
//		{
//			if( index_buffer[i] <= 1 )
//			{
//				cur_index += index_buffer[i];
//				continue;
//			}
//
//			generate_line_ex( &vertex_buffer[cur_index], index_buffer[i], in_width, in_height, in_color );
//
//			cur_index += index_buffer[i];
//		}
//
//		m_center = m_render_aabb.center();
//		for ( int i = 0; i < (int)m_vertex_array.size(); i ++ )
//		{
//			m_vertex_array[i].x = m_vertex_array[i].x - m_center.x;
//			m_vertex_array[i].y = m_vertex_array[i].y - m_center.y;
//			m_vertex_array[i].z = m_vertex_array[i].z - m_center.z;
//		}
//	}
//
//	void geometry_mesh::create_from_polygon( vector_3d* v_src, int v_size, ulong v_color,double v_height )
//	{
//		m_shadow_color = v_color;
//		m_render_aabb.make_invalid();
//
//		// calculate triangle 
//		m_vertex_array.clear();
//		m_index_array.clear();
//
//		generate_polygon_ex( v_src, v_size, v_height, v_color );
//	}
//
//	void geometry_mesh::create_from_multiple_polygon( vector3<double>* vertex_buffer, ulong vertex_size, ushort* index_buffer, ulong index_size, ulong in_color, double in_height )
//	{
//		m_shadow_color = in_color;
//		m_render_aabb.make_invalid();
//
//		m_index_array.clear();
//		m_vertex_array.clear();
//
//		ulong cur_index = 0;
//		for ( ulong i = 0; i < index_size; i++ )
//		{
//			if( index_buffer[i] <= 2 )
//			{
//				cur_index += index_buffer[i];
//				continue;
//			}
//
//			generate_polygon_coarse( &vertex_buffer[cur_index], index_buffer[i], in_height, in_color );
//
//			cur_index += index_buffer[i];
//		}
//
//		m_center = ( m_render_aabb.get_min() + m_render_aabb.get_max() ) * 0.5;
//		for ( int i = 0; i < (int)m_vertex_array.size(); i ++ )
//		{
//			m_vertex_array[i].x -= m_center.x;
//			m_vertex_array[i].y -= m_center.y;
//			m_vertex_array[i].z -= m_center.z;
//		}
//	}
//
//	// create from box
//	//////////////////////////////////////////////////////////////////////////
//	/*
//	   5-----4
//	  /|    /|
//	 / |   / |
//	1-----2  |
//	|  6--|--7
//	| /   | /
//	|/    |/
//	0-----3
//	*/
//	//////////////////////////////////////////////////////////////////////////
//	void geometry_mesh::create_from_box( lon_lat_box& v_box, ulong line_color, ulong fill_color )
//	{
//		m_shadow_color = fill_color;
//
//		m_vertex_array.clear();
//		m_index_array.clear();
//
//		m_vertex_array.resize( 8 );
//		m_index_array.resize( 36 );
//
//		m_render_aabb = axis_aligned_box<double>( v_box.corner_value, 8 );
//		m_center = m_render_aabb.center();
//
//		ushort index_array[36] = 
//		{	
//			0,1,3,1,2,3,
//			3,2,7,2,4,7,
//			7,4,6,4,5,6,
//			6,5,0,5,1,0,
//			1,5,2,5,4,2,
//			0,3,6,3,7,6
//		};
//		m_index_array.resize( 36 );
//		memcpy( m_index_array.begin()._Myptr, index_array, sizeof(ushort) * 36 );
//		
//		for ( int i = 0; i < 8; i++ )
//		{
//			m_vertex_array[i].x = v_box.corner_value[i].x - m_center.x;		
//			m_vertex_array[i].y = v_box.corner_value[i].y - m_center.y;
//			m_vertex_array[i].z = v_box.corner_value[i].z - m_center.z;
//			m_vertex_array[i].color = fill_color;
//		}
//
//		m_frame_vertex_array.resize( m_vertex_array.size() );
//		std::copy( m_vertex_array.begin(), m_vertex_array.end(), m_frame_vertex_array.begin() );
//		for ( ulong i = 0; i < m_frame_vertex_array.size(); i ++ )
//		{
//			m_frame_vertex_array[i].color = line_color;
//		}
//		
//		// 边框
//		ushort frame_index_array[24] =
//		{
//			0,1, 1,2, 2,3, 3,0,
//			4,5, 5,6, 6,7, 7,4,
//			0,6, 3,7, 1,5, 2,4
//		};
//		m_frame_index_array.resize( 24 );
//		memcpy( m_frame_index_array.begin()._Myptr, frame_index_array, sizeof(ushort) * 24 );
//	}
//
//	void geometry_mesh::create_from_volume( vector_3d* in_pos_array, ulong v_size, double min_height, double max_height, ulong line_color, ulong fill_color )
//	{
//		m_shadow_color = fill_color;
//		m_render_aabb.make_invalid();
//
//		// calculate triangle 
//		m_vertex_array.clear();
//		m_index_array.clear();
//
//		generate_polygon( in_pos_array, v_size, min_height, max_height, fill_color );
//
//		m_frame_vertex_array.resize( m_vertex_array.size() );
//		std::copy( m_vertex_array.begin(), m_vertex_array.end(), m_frame_vertex_array.begin() );
//		for ( int i = 0; i < (int)m_frame_vertex_array.size(); i ++ )
//		{
//			m_frame_vertex_array[i].color = line_color;
//		}
//
//		// 边框
//		ulong v_line_num = v_size * 3;
//		m_frame_index_array.resize( v_line_num * 2 );
//		ushort* p_index = m_frame_index_array.begin()._Myptr;
//
//		ulong v_off = 0;
//		for ( ulong i = 0; i < v_size-1; i++ )
//		{
//			v_off = i<<1;
//			p_index[v_off]   = i;
//			p_index[v_off+1] = i+1;
//
//			v_off = (i+v_size)<<1;
//			p_index[v_off]	 = v_size + i;
//			p_index[v_off+1] = v_size + i + 1;
//
//			v_off = (i+2*v_size)<<1;
//			p_index[v_off]	 = i;
//			p_index[v_off+1] = v_size + i;
//		}
//		v_off = (v_size -1)<<1;
//		p_index[v_off]   = v_size - 1;
//		p_index[v_off+1] = 0;
//
//		v_off = (2*v_size-1)<<1;
//		p_index[v_off]   = v_size*2 - 1;
//		p_index[v_off+1] = v_size;
//
//		v_off = (3*v_size-1)<<1;
//		p_index[v_off]   = v_size*2 - 1;
//		p_index[v_off+1] = v_size - 1;
//	}
//
//	void geometry_mesh::create_from_volume_in_plane( vector_3d* in_pos_array, ulong v_size, double vol_height, ulong line_color, ulong fill_color )
//	{
//		m_shadow_color = fill_color;
//		m_render_aabb.make_invalid();
//
//		// calculate triangle 
//		m_vertex_array.clear();
//		m_index_array.clear();
//
//		generate_polygon_in_plane( in_pos_array, v_size, vol_height, fill_color );
//	
//		m_frame_vertex_array.resize( m_vertex_array.size() );
//		std::copy( m_vertex_array.begin(), m_vertex_array.end(), m_frame_vertex_array.begin() );
//		for ( int i = 0; i < (int)m_frame_vertex_array.size(); i ++ )
//		{
//			m_frame_vertex_array[i].color = line_color;
//		}
//
//		// 边框
//		ulong v_line_num = v_size * 3;
//		m_frame_index_array.resize( v_line_num * 2 );
//		ushort* p_index = m_frame_index_array.begin()._Myptr;
//
//		ulong v_off = 0;
//		for ( ulong i = 0; i < v_size-1; i++ )
//		{
//			v_off = i<<1;
//			p_index[v_off]   = i;
//			p_index[v_off+1] = i+1;
//
//			v_off = (i+v_size)<<1;
//			p_index[v_off]	 = v_size + i;
//			p_index[v_off+1] = v_size + i + 1;
//
//			v_off = (i+2*v_size)<<1;
//			p_index[v_off]	 = i;
//			p_index[v_off+1] = v_size + i;
//		}
//		v_off = (v_size -1)<<1;
//		p_index[v_off]   = v_size - 1;
//		p_index[v_off+1] = 0;
//
//		v_off = (2*v_size-1)<<1;
//		p_index[v_off]   = v_size*2 - 1;
//		p_index[v_off+1] = v_size;
//
//		v_off = (3*v_size-1)<<1;
//		p_index[v_off]   = v_size*2 - 1;
//		p_index[v_off+1] = v_size - 1;
//	}
//
//	void geometry_mesh::create_from_cone( vector_3d in_center, ulong in_sides, double in_radius, double in_height, ulong line_color, ulong fill_color )
//	{
//		m_shadow_color = fill_color;
//		m_vertex_array.clear();
//		m_index_array.clear();
//		m_render_aabb.make_invalid();
//
//		vector_3d v_point_center = in_center;
//		v_point_center = geocentric_coords::from_spherical_d( v_point_center );
//		vector_3d v_point_top = in_center;
//		v_point_top.z += in_height;
//		v_point_top = geocentric_coords::from_spherical_d( v_point_top );
//
//		m_render_aabb.combine( v_point_top );
//				
//		vector_3d* v_circle_points = new vector_3d[in_sides];
//		shape_creator::create_circle_not_clamp( v_circle_points, in_center, in_radius, in_sides );
//
//		for ( ulong i = 0; i < in_sides; i ++ )
//		{		
//			m_render_aabb.combine( v_circle_points[i] );
//		}
//		m_center = m_render_aabb.center();
//
//		position_color pc;
//		for ( ulong i = 0; i < in_sides; i ++ )
//		{
//			vector3<double> pos_geo = v_circle_points[i];
//			pc.x = pos_geo.x - m_center.x;
//			pc.y = pos_geo.y - m_center.y;
//			pc.z = pos_geo.z - m_center.z;
//			pc.color = fill_color;
//			m_vertex_array.push_back( pc );
//		}
//		pc.x = v_point_center.x - m_center.x;
//		pc.y = v_point_center.y - m_center.y;
//		pc.z = v_point_center.z - m_center.z;
//		pc.color = fill_color;
//		m_vertex_array.push_back( pc );
//		
//		pc.x = v_point_top.x - m_center.x;
//		pc.y = v_point_top.y - m_center.y;
//		pc.z = v_point_top.z - m_center.z;
//		pc.color = fill_color;
//		m_vertex_array.push_back( pc );
//		m_render_aabb.combine( v_point_top );
//		
//		int in_triangle_start_mode = 0;	// 0: 右手，CCW. 1: 左手，CW
//		//
//		//                    |x1     x2     x3|   
//		//	S(P1,P2,P3)   =   |y1     y2     y3|   =   (x1-x3)*(y2-y3)   -   (y1-y3)(x2-x3)   
//		//	                  |1      1      1 |   
//		//
//		double v_area = (v_point_center.x - v_circle_points[1].x) * (v_circle_points[0].y - v_circle_points[1].y) - (v_point_center.y - v_circle_points[1].y) * (v_circle_points[0].x - v_circle_points[1].x);
//
//		if( v_area > 0 )
//		{
//			in_triangle_start_mode = 0;
//		}
//		else
//		{
//			in_triangle_start_mode = 1;
//		}
//
//
//		// 构建底面，同时构建侧面
//		for ( ulong i = 0; i < in_sides; i ++ )
//		{
//			if ( in_triangle_start_mode == 0 ) // CCW，右手
//			{
//				// 底面
//				m_index_array.push_back( in_sides );
//				m_index_array.push_back( (i == in_sides - 1) ? 0 : i + 1 );
//				m_index_array.push_back( i );
//				// 锥面
//				m_index_array.push_back( in_sides + 1 );
//				m_index_array.push_back( i );
//				m_index_array.push_back( (i == in_sides - 1) ? 0 : i + 1 );
//			}
//			else // CW，左手
//			{
//				// 底面
//				m_index_array.push_back( in_sides );
//				m_index_array.push_back( i );
//				m_index_array.push_back( (i == in_sides - 1) ? 0 : i + 1 );
//				// 锥面
//				m_index_array.push_back( in_sides + 1 );
//				m_index_array.push_back( (i == in_sides - 1) ? 0 : i + 1 );
//				m_index_array.push_back( i );
//			}
//		}
//
//		m_frame_vertex_array.resize( m_vertex_array.size() );
//		std::copy( m_vertex_array.begin(), m_vertex_array.end(), m_frame_vertex_array.begin() );
//		for ( int i = 0; i < (int)m_frame_vertex_array.size(); i ++ )
//		{
//			m_frame_vertex_array[i].color = line_color;
//		}
//
//		// 边框
//		ulong v_line_num = in_sides * 2;
//		m_frame_index_array.resize( v_line_num * 2 );
//		ushort* p_index = m_frame_index_array.begin()._Myptr;
//
//		ulong v_off = 0;
//		for ( ulong i = 0; i < in_sides-1; i++ )
//		{
//			v_off = i<<1;
//			p_index[v_off]   = i;
//			p_index[v_off+1] = i+1;
//
//			v_off = (i+in_sides)<<1;
//			p_index[v_off]	 = in_sides+1;
//			p_index[v_off+1] = i;
//		}
//		v_off = (in_sides-1)<<1;
//		p_index[in_sides-1] = in_sides - 1;
//		p_index[in_sides]   = 0;
//
//		v_off = (2*in_sides-1)<<1;
//		p_index[v_off]   = in_sides+1;
//		p_index[v_off+1] = in_sides - 1;
//		
//	}
//
//	#define NORMALIZE_ANGLE( val ) { if(val > PI * 2) val -= PI *2; if(val < -PI * 2) val += PI *2; }
//	void geometry_mesh::create_from_sphere( vector_3d center_point, double in_radius, ulong line_color, ulong fill_color )
//	{
//		m_shadow_color = fill_color;
//		m_vertex_array.clear();
//		m_index_array.clear();
//		int	v_segment = (1 << 5) + 1;
//		double v_lon_scale_factor = 1.0 / v_segment;
//		double v_lat_scale_factor = 1.0 / v_segment;
//
//		double v_longitude	= - PI;
//		double v_latitude	= - HALF_PI;
//		double v_radius		= in_radius * spherical_coords::s_semi_major;
//		double v_delta_longitude = v_lon_scale_factor * PI;
//		double v_delta_latitude  = v_lat_scale_factor * PI;
//
//		int v_segment_plus_1 = v_segment + 1;
//		int v_double_segment_plus_1 = v_segment * 2 + 1;
//		int v_double_segment = v_segment * 2;
//
//		int v_vertex_count = v_segment_plus_1 * v_double_segment_plus_1 ;
//		int v_index_count =  v_segment * v_double_segment * 2 * 3;
//
//		m_vertex_array.resize(v_vertex_count);
//		
//		m_center = center_point;
//		m_center.z += v_radius;
//
//		spherical_transform v_tansfrom( m_center.x, m_center.y, m_center.z, 0.0, math_d::s_half_pi, 0.0 );
//		
//		m_center = geocentric_coords::from_spherical_d( m_center );
//
//		int v_index = 0;
//		for ( int i = 0; i < v_segment_plus_1; i++ )
//		{
//			double v_longitude = 0;
//			for ( int j = 0; j < v_double_segment_plus_1; j++ )
//			{
//				vector3<double> v_out = geocentric_coords::from_spherical_d(v_longitude, v_latitude, v_radius);
//				v_out = v_out * v_tansfrom.m_mat_tnb_no_scale;
//				
//				m_vertex_array[v_index].x = (float)(v_out.x - m_center.x);
//				m_vertex_array[v_index].y = (float)(v_out.y - m_center.y);
//				m_vertex_array[v_index].z = (float)(v_out.z - m_center.z);
//				m_vertex_array[v_index].color = m_shadow_color;
//							v_index += 1;
//				v_longitude += v_delta_longitude;
//				NORMALIZE_ANGLE(v_longitude);
//			}
//			v_latitude += v_delta_latitude;
//			NORMALIZE_ANGLE(v_latitude);
//		}
//
//		m_index_array.resize( v_index_count );
//		
//		v_index = 0;
//		for (int i = 0; i < v_segment ; i++)
//		{
//			v_index = ( 2 * 3 * i * v_double_segment );
//
//			for (int j = 0; j < v_double_segment; j++)
//			{
//				m_index_array[v_index] = (short)(i * v_double_segment_plus_1 + j);
//				m_index_array[v_index + 1] = (short)((i + 1) * v_double_segment_plus_1 + j);
//				m_index_array[v_index + 2] = (short)(i * v_double_segment_plus_1 + j + 1);
//
//				m_index_array[v_index + 3] = (short)(i * v_double_segment_plus_1 + j + 1);
//				m_index_array[v_index + 4] = (short)((i + 1) * v_double_segment_plus_1 + j);
//				m_index_array[v_index + 5] = (short)((i + 1) * v_double_segment_plus_1 + j + 1);
//
//				v_index += 6;
//			}
//		}
//
//		// 边框
//		m_frame_vertex_array.resize( m_vertex_array.size() );
//		m_frame_index_array.resize( m_index_array.size() );
//		std::copy( m_vertex_array.begin(), m_vertex_array.end(), m_frame_vertex_array.begin() );
//		for ( int i = 0; i < (int)m_frame_vertex_array.size(); i ++ )
//		{
//			m_frame_vertex_array[i].color = line_color;
//		}
//
//		v_latitude = -HALF_PI;
//		int v_frame_index_cursor = 0;
//		for ( int i = 0; i < v_double_segment_plus_1; i+= 4 )
//		{
//			for ( int j = 0; j < v_segment; j++ )
//			{
//				m_frame_index_array[v_frame_index_cursor+(j<<1)]   = j * v_double_segment_plus_1 + i;
//				m_frame_index_array[v_frame_index_cursor+(j<<1)+1] = ( j + 1 ) * v_double_segment_plus_1 + i;
//			}
//			v_frame_index_cursor += v_segment_plus_1<<1;
//		}
//	}
//
//	//void geometry_mesh::create_from_sphere( vector_3d center_point, double v_radius, ulong line_color, ulong fill_color )
//	//{
//	//	m_shadow_color = fill_color;
//	//	m_vertex_array.clear();
//	//	m_index_array.clear();
//	//	m_aabb.make_invalid();
//	//	double v_radius_km = v_radius * spherical_coords::s_semi_major; // 公里；
// //		double v_radius_rad = v_radius; // 半径弧度；
//	//	vector_3d v_vec_center = center_point;
//	//	v_vec_center.z += v_radius_km;
//
//	//	 生成球
//	//	int v_segment = 24;
//	//	int v_lon_segement = v_segment * 2;
//	//	int v_lat_segement = v_segment;
//	//	int v_lon_segement_plus1 = v_lon_segement + 1;
//	//	int v_lat_segement_plus1 = v_lat_segement + 1;
//	//	double v_latitude  = - HALF_PI;
//	//	double v_delta_angle = PI / v_segment;
//
//	//	ulong v_line_num = ( v_segment / 4 - 2 ) * v_lon_segement * 3;
//	//	m_frame_index_array.resize( v_line_num * 2 );
//	//	ulong v_frame_index_cursor = 0;
//
//	//	m_center = geocentric_coords::from_spherical_d( center_point );
//
//	//	 获取点序列
//	//	for ( int i = 0; i < v_lat_segement_plus1; i ++ )
//	//	{
//	//		 获得纬度圆中心点和半径
//	//		vector_3d v_lat_center = v_vec_center;
//	//		v_lat_center.z += math_d::sin_( v_latitude ) * v_radius_km;
//	//		double v_lat_radius = math_d::cos_( v_latitude ) * v_radius_rad;
//
//	//		 获得纬度圈
//	//		vector_3d* v_circle_points = new vector_3d[v_lon_segement_plus1];
//	//		shape_creator::create_circle_not_clamp( v_circle_points, v_lat_center, v_lat_radius, v_lon_segement );
//	//		v_circle_points[v_lon_segement] = v_circle_points[0];
//
//	//		if ( (i%4==0) && (i!=0) && (i!=v_segment) )
//	//		{
//	//			ushort cur_vert = m_vertex_array.size();
//	//			for ( ushort ni = 0; ni < v_lon_segement; ni++ )
//	//			{
//	//				m_frame_index_array[v_frame_index_cursor+(ni<<1)]   = cur_vert+ni;
//	//				m_frame_index_array[v_frame_index_cursor+(ni<<1)+1] = cur_vert+ni+1;
//	//				
//	//			}
//	//			v_frame_index_cursor += v_lon_segement<<1;
//	//		}
//
//	//		position_color pc;
//	//		for ( int j = 0; j < v_lon_segement_plus1; j ++ )
//	//		{
//	//			vector3<double> pos_geo = v_circle_points[j];
//	//			pc.x = pos_geo.x;
//	//			pc.y = pos_geo.y;
//	//			pc.z = pos_geo.z;
//	//			pc.color = fill_color;
//	//			m_vertex_array.push_back( pc );
//	//		}
//
//	//		AUTO_DELETE_ARRAY(v_circle_points);
//
//	//		v_latitude += v_delta_angle;
//	//	}
//
//	//	 获取索引序列
//	//	for ( int i = 0; i < v_lat_segement; i ++ )
//	//	{
//	//		for (int j = 0; j < v_lon_segement; j ++)
//	//		{
//	//			m_index_array.push_back( i * v_lon_segement_plus1 + j );
//	//			m_index_array.push_back( i * v_lon_segement_plus1 + j + 1);
//	//			m_index_array.push_back( (i + 1) * v_lon_segement_plus1 + j );
//
//	//			m_index_array.push_back( i * v_lon_segement_plus1 + j + 1 );
//	//			m_index_array.push_back( (i + 1) * v_lon_segement_plus1 + j + 1);
//	//			m_index_array.push_back( (i + 1) * v_lon_segement_plus1 + j );
//	//		}
//	//	}
//
//	//}
//
//	void geometry_mesh::generate_line_ex( vector_3d* in_pos_array, ulong v_size, double v_width, double v_height, ulong v_color )
//	{
//		if ( v_size <= 1 ) return;
//
//		coordinates_array v_vertex_array;
//		coordinates_array v_upper_coords_array;
//		coordinates_array v_lower_coords_array;
//
//		if ( v_size == 2 )
//		{
//			// 两个点的情况
//			vector3<double> pos_geo1 = in_pos_array[0];
//			vector3<double> pos_geo2 = in_pos_array[1];
//			vector3<double> direction = vector3<double>::cross( pos_geo1, pos_geo2 );
//			direction.normalize();
//			double length_scale = compute_scaled_length( pos_geo1, pos_geo2, v_height );
//
//			// 第一个点
//			vector3<double> pos_geo1_scaled = vector3<double>::normalize(pos_geo1) * length_scale;
//
//			v_upper_coords_array.push_back( pos_geo1_scaled - direction * v_width );
//			v_upper_coords_array.push_back( pos_geo1_scaled + direction * v_width );
//
//			pos_geo1_scaled = vector3<double>::normalize(pos_geo1) * ( spherical_coords::s_semi_major - 2 );
//
//			v_lower_coords_array.push_back( pos_geo1_scaled - direction * v_width );
//			v_lower_coords_array.push_back( pos_geo1_scaled + direction * v_width );
//
//			// 第二个点
//			vector3<double> pos_geo2_scaled = vector3<double>::normalize(pos_geo2) * length_scale;
//
//			v_upper_coords_array.push_back( pos_geo2_scaled - direction * v_width );
//			v_upper_coords_array.push_back( pos_geo2_scaled + direction * v_width );
//
//			pos_geo2_scaled = vector3<double>::normalize(pos_geo2) * ( spherical_coords::s_semi_major - 2 );
//
//			v_lower_coords_array.push_back( pos_geo2_scaled - direction * v_width );
//			v_lower_coords_array.push_back( pos_geo2_scaled + direction * v_width );
//
//			for ( ulong i = 0 ;i < v_upper_coords_array.size() ;i ++ )
//			{
//				v_vertex_array.push_back( v_upper_coords_array[i] );
//			}
//
//			for ( ulong i = 0 ;i < v_lower_coords_array.size() ;i ++ )
//			{
//				v_vertex_array.push_back( v_lower_coords_array[i] );
//			}
//
//			generate_index_from_line( v_size * 2 );
////			generate_vertex_from_line( v_vertex_array, v_color );
//			for ( int i = 0; i < (int)v_vertex_array.size(); i ++ )
//			{
//				vector_3d pos = v_vertex_array[i];
//				m_render_aabb.combine( pos );
//				position_color pc;
//				pc.x = pos.x;
//				pc.y = pos.y;
//				pc.z = pos.z;
//				pc.color = v_color;
//				m_vertex_array.push_back( pc );
//			}
//
//			return;
//		}
//
//
//		//
//		// 三个点以上的情况
//		//		
//
//		//
//		// 第一个点
//		//
//		{
//			vector3<double> pos_geo1 = in_pos_array[0];
//			vector3<double> pos_geo2 = in_pos_array[1];
//			vector3<double> direction = vector3<double>::cross( pos_geo1, pos_geo2 );
//			direction.normalize();
//			double length_scale = compute_scaled_length( pos_geo1, pos_geo2, v_height );
//			vector3<double> pos_geo1_scaled = vector3<double>::normalize(pos_geo1) * length_scale;
//
//			v_upper_coords_array.push_back( pos_geo1_scaled - direction * v_width );
//			v_upper_coords_array.push_back( pos_geo1_scaled + direction * v_width );
//
//			pos_geo1_scaled = vector3<double>::normalize(pos_geo1) * ( spherical_coords::s_semi_major - 2 );
//
//			v_lower_coords_array.push_back( pos_geo1_scaled - direction * v_width );
//			v_lower_coords_array.push_back( pos_geo1_scaled + direction * v_width );
//		}	
//
//
//		for( int i = 1; i < int(v_size-1); i ++ )
//		{
//			vector3<double> pos_geo1 = in_pos_array[i - 1];
//			vector3<double> pos_geo2 = in_pos_array[i];
//			vector3<double> pos_geo3 = in_pos_array[i + 1];
//
//			//
//			// scale
//			//
//			double max_length_scale = compute_scaled_length( pos_geo1, pos_geo2, v_height );
//			max_length_scale = max( max_length_scale, compute_scaled_length( pos_geo2, pos_geo3, v_height ) );
//
//			//
//			// direction
//			//
//			vector3<double> direction1 = vector3<double>::cross( pos_geo1, pos_geo2 );
//			direction1.normalize();
//
//			vector3<double> direction2 = vector3<double>::cross( pos_geo2, pos_geo3 );
//			direction2.normalize();
//
//			vector3<double> direction = ( direction1 + direction2 ) * 0.5;
//			direction.normalize();
//
//			vector3<double> v_temp_normal = vector3<double>::normalize( pos_geo1 - pos_geo2 );
//			double dot_val = vector3<double>::dot( v_temp_normal, direction );
//			if ( dot_val < 0.0 )
//			{
//				dot_val = vector3<double>::dot( v_temp_normal, -direction );
//			}
//
//			double width;
//			if ( math<double>::acos_( dot_val ) < 1 * RADIAN ) 
//			{
//				width = v_width;
//			}
//			else
//			{
//				width = v_width / math<double>::sin_( math<double>::acos_( dot_val ) );
//			}
//
//			vector3<double> pos_geo2_scaled = vector3<double>::normalize(pos_geo2) * max_length_scale;
//			v_upper_coords_array.push_back( pos_geo2_scaled - direction * width );
//			v_upper_coords_array.push_back( pos_geo2_scaled + direction * width );
//
//			pos_geo2_scaled = vector3<double>::normalize(pos_geo2) * ( spherical_coords::s_semi_major - 2 );
//
//			v_lower_coords_array.push_back( pos_geo2_scaled - direction * width );
//			v_lower_coords_array.push_back( pos_geo2_scaled + direction * width );
//		}
//
//		//
//		// 最后一个点
//		//
//		{
//			vector3<double> pos_geo1 = in_pos_array[v_size - 2];
//			vector3<double> pos_geo2 = in_pos_array[v_size - 1];
//
//			vector3<double> direction = vector3<double>::cross( pos_geo1, pos_geo2 );
//			direction.normalize();
//			double length_scale = compute_scaled_length( pos_geo1, pos_geo2, v_height );
//			vector3<double> pos_geo2_scaled = vector3<double>::normalize(pos_geo2) * length_scale;
//
//			v_upper_coords_array.push_back( pos_geo2_scaled - direction * v_width );
//			v_upper_coords_array.push_back( pos_geo2_scaled + direction * v_width );
//
//			pos_geo2_scaled = vector3<double>::normalize(pos_geo2) * ( spherical_coords::s_semi_major - 2 );
//
//			v_lower_coords_array.push_back( pos_geo2_scaled - direction * v_width );
//			v_lower_coords_array.push_back( pos_geo2_scaled + direction * v_width );
//		}
//
//		for ( ulong i = 0 ;i < v_upper_coords_array.size() ;i ++ )
//		{
//			v_vertex_array.push_back( v_upper_coords_array[i] );
//		}
//
//		for ( ulong i = 0 ;i < v_lower_coords_array.size() ;i ++ )
//		{
//			v_vertex_array.push_back( v_lower_coords_array[i] );
//		}
//
//		generate_index_from_line( v_size * 2 );
//		//generate_vertex_from_line( v_vertex_array, v_color );
//		for ( int i = 0; i < (int)v_vertex_array.size(); i ++ )
//		{
//			vector_3d pos = v_vertex_array[i];
//			m_render_aabb.combine( pos );
//			position_color pc;
//			pc.x = pos.x;
//			pc.y = pos.y;
//			pc.z = pos.z;
//			pc.color = v_color;
//			m_vertex_array.push_back( pc );
//		}
//	}
//
//	void geometry_mesh::generate_line( vector_3d* in_pos_array, ulong v_size, double v_width, double v_height, ulong v_color )
//	{
//		if ( v_size <= 1 ) return;
//		
//		coordinates_array v_vertex_array;
//		coordinates_array v_upper_coords_array;
//		coordinates_array v_lower_coords_array;
//
//		if ( v_size == 2 )
//		{
//			// 两个点的情况
//			vector3<double> pos_geo1 = in_pos_array[0];
//			vector3<double> pos_geo2 = in_pos_array[1];
//			vector3<double> direction = vector3<double>::cross( pos_geo1, pos_geo2 );
//			direction.normalize();
//			double length_scale = compute_scaled_length( pos_geo1, pos_geo2, v_height );
//
//			// 第一个点
//			vector3<double> pos_geo1_scaled = vector3<double>::normalize(pos_geo1) * length_scale;
//
//			v_upper_coords_array.push_back( pos_geo1_scaled - direction * v_width );
//			v_upper_coords_array.push_back( pos_geo1_scaled + direction * v_width );
//
//			pos_geo1_scaled = vector3<double>::normalize(pos_geo1) * ( spherical_coords::s_semi_major - 2 );
//
//			v_lower_coords_array.push_back( pos_geo1_scaled - direction * v_width );
//			v_lower_coords_array.push_back( pos_geo1_scaled + direction * v_width );
//
//			// 第二个点
//			vector3<double> pos_geo2_scaled = vector3<double>::normalize(pos_geo2) * length_scale;
//
//			v_upper_coords_array.push_back( pos_geo2_scaled - direction * v_width );
//			v_upper_coords_array.push_back( pos_geo2_scaled + direction * v_width );
//
//			pos_geo2_scaled = vector3<double>::normalize(pos_geo2) * ( spherical_coords::s_semi_major - 2 );
//
//			v_lower_coords_array.push_back( pos_geo2_scaled - direction * v_width );
//			v_lower_coords_array.push_back( pos_geo2_scaled + direction * v_width );
//
//			for ( ulong i = 0 ;i < v_upper_coords_array.size() ;i ++ )
//			{
//				v_vertex_array.push_back( v_upper_coords_array[i] );
//			}
//
//			for ( ulong i = 0 ;i < v_lower_coords_array.size() ;i ++ )
//			{
//				v_vertex_array.push_back( v_lower_coords_array[i] );
//			}
//
//			generate_index_from_line( v_size * 2 );
//			generate_vertex_from_line( v_vertex_array, v_color );
//
//			return;
//		}
//
//
//		//
//		// 三个点以上的情况
//		//		
//
//		//
//		// 第一个点
//		//
//		{
//			vector3<double> pos_geo1 = in_pos_array[0];
//			vector3<double> pos_geo2 = in_pos_array[1];
//			vector3<double> direction = vector3<double>::cross( pos_geo1, pos_geo2 );
//			direction.normalize();
//			double length_scale = compute_scaled_length( pos_geo1, pos_geo2, v_height );
//			vector3<double> pos_geo1_scaled = vector3<double>::normalize(pos_geo1) * length_scale;
//
//			v_upper_coords_array.push_back( pos_geo1_scaled - direction * v_width );
//			v_upper_coords_array.push_back( pos_geo1_scaled + direction * v_width );
//
//			pos_geo1_scaled = vector3<double>::normalize(pos_geo1) * ( spherical_coords::s_semi_major - 2 );
//
//			v_lower_coords_array.push_back( pos_geo1_scaled - direction * v_width );
//			v_lower_coords_array.push_back( pos_geo1_scaled + direction * v_width );
//		}	
//
//
//		for( int i = 1; i < int(v_size-1); i ++ )
//		{
//			vector3<double> pos_geo1 = in_pos_array[i - 1];
//			vector3<double> pos_geo2 = in_pos_array[i];
//			vector3<double> pos_geo3 = in_pos_array[i + 1];
//
//			//
//			// scale
//			//
//			double max_length_scale = compute_scaled_length( pos_geo1, pos_geo2, v_height );
//			max_length_scale = max( max_length_scale, compute_scaled_length( pos_geo2, pos_geo3, v_height ) );
//
//			//
//			// direction
//			//
//			vector3<double> direction1 = vector3<double>::cross( pos_geo1, pos_geo2 );
//			direction1.normalize();
//
//			vector3<double> direction2 = vector3<double>::cross( pos_geo2, pos_geo3 );
//			direction2.normalize();
//
//			vector3<double> direction = ( direction1 + direction2 ) * 0.5;
//			direction.normalize();
//
//			vector3<double> v_temp_normal = vector3<double>::normalize( pos_geo1 - pos_geo2 );
//			double dot_val = vector3<double>::dot( v_temp_normal, direction );
//			if ( dot_val < 0.0 )
//			{
//				dot_val = vector3<double>::dot( v_temp_normal, -direction );
//			}
//
//			double width;
//			if ( math<double>::acos_( dot_val ) < 1 * RADIAN ) 
//			{
//				width = v_width;
//			}
//			else
//			{
//				width = v_width / math<double>::sin_( math<double>::acos_( dot_val ) );
//			}
//
//			vector3<double> pos_geo2_scaled = vector3<double>::normalize(pos_geo2) * max_length_scale;
//			v_upper_coords_array.push_back( pos_geo2_scaled - direction * width );
//			v_upper_coords_array.push_back( pos_geo2_scaled + direction * width );
//
//			pos_geo2_scaled = vector3<double>::normalize(pos_geo2) * ( spherical_coords::s_semi_major - 2 );
//
//			v_lower_coords_array.push_back( pos_geo2_scaled - direction * width );
//			v_lower_coords_array.push_back( pos_geo2_scaled + direction * width );
//		}
//
//		//
//		// 最后一个点
//		//
//		{
//			vector3<double> pos_geo1 = in_pos_array[v_size - 2];
//			vector3<double> pos_geo2 = in_pos_array[v_size - 1];
//
//			vector3<double> direction = vector3<double>::cross( pos_geo1, pos_geo2 );
//			direction.normalize();
//			double length_scale = compute_scaled_length( pos_geo1, pos_geo2, v_height );
//			vector3<double> pos_geo2_scaled = vector3<double>::normalize(pos_geo2) * length_scale;
//
//			v_upper_coords_array.push_back( pos_geo2_scaled - direction * v_width );
//			v_upper_coords_array.push_back( pos_geo2_scaled + direction * v_width );
//
//			pos_geo2_scaled = vector3<double>::normalize(pos_geo2) * ( spherical_coords::s_semi_major - 2 );
//
//			v_lower_coords_array.push_back( pos_geo2_scaled - direction * v_width );
//			v_lower_coords_array.push_back( pos_geo2_scaled + direction * v_width );
//		}
//
//		for ( ulong i = 0 ;i < v_upper_coords_array.size() ;i ++ )
//		{
//			v_vertex_array.push_back( v_upper_coords_array[i] );
//		}
//
//		for ( ulong i = 0 ;i < v_lower_coords_array.size() ;i ++ )
//		{
//			v_vertex_array.push_back( v_lower_coords_array[i] );
//		}
//
//		generate_index_from_line( v_size * 2 );
//		generate_vertex_from_line( v_vertex_array, v_color );
//	}
//
//	double geometry_mesh::compute_scaled_length( vector3<double> pos1, vector3<double> pos2, double v_height )
//	{
//		vector3<double> pos_normal1 = vector3<double>::normalize( pos1 );
//		vector3<double> pos_normal2 = vector3<double>::normalize( pos2 );
//
//		double half_angle = math<double>::acos_( vector3<double>::dot( pos_normal1, pos_normal2 ) ) * 0.5;
//
//		return ( spherical_coords::s_semi_major + v_height ) / math<double>::cos_( half_angle );
//	}
//
//	void geometry_mesh::generate_vertex_from_line( coordinates_array& in_array, ulong v_color )
//	{
//		for ( int i = 0; i < (int)in_array.size(); i ++ )
//		{
//			m_render_aabb.combine( in_array[i] );
//		}
//
//		m_center = m_render_aabb.center();
//
//		for ( int i = 0; i < (int)in_array.size(); i ++ )
//		{
//			vector_3d pos = in_array[i];
//			position_color pc;
//			pc.x = pos.x - m_center.x;
//			pc.y = pos.y - m_center.y;
//			pc.z = pos.z - m_center.z;
//			pc.color = v_color;
//			m_vertex_array.push_back( pc );
//		}
//	}
//
//	void geometry_mesh::generate_index_from_line( ulong v_size )
//	{
//		ulong start_index = (ulong)m_vertex_array.size();
//
//		for ( ulong i = start_index;i < start_index + v_size - 3; i += 2 )
//		{
//			m_index_array.push_back( i );
//			m_index_array.push_back( i + 3 );
//			m_index_array.push_back( i + 1 );
//
//			m_index_array.push_back( i );
//			m_index_array.push_back( i + 2 );
//			m_index_array.push_back( i + 3 );
//		}
//
//		for ( ulong i = start_index;i < start_index + v_size - 3; i += 2 )
//		{
//			m_index_array.push_back( i + v_size );
//			m_index_array.push_back( i + 1 + v_size );
//			m_index_array.push_back( i + 3 + v_size );
//
//
//			m_index_array.push_back( i + v_size);
//			m_index_array.push_back( i + 3 + v_size );
//			m_index_array.push_back( i + 2 + v_size );			
//		}
//
//		for ( ulong i = start_index;i < start_index + v_size - 3; i += 2 )
//		{
//			m_index_array.push_back( i );
//			m_index_array.push_back( i + v_size );
//			m_index_array.push_back( i + 2 + v_size );
//
//			m_index_array.push_back( i );
//			m_index_array.push_back( i + 2 + v_size );
//			m_index_array.push_back( i + 2 );
//
//
//			m_index_array.push_back( i + 1 );
//			m_index_array.push_back( i + 3 + v_size );
//			m_index_array.push_back( i + 1 + v_size );
//
//
//			m_index_array.push_back( i + 1 );
//			m_index_array.push_back( i + 3 );
//			m_index_array.push_back( i + 3 + v_size );		
//		}
//
//		// 前后两个面
//		m_index_array.push_back( start_index );
//		m_index_array.push_back( start_index + 1 + v_size );
//		m_index_array.push_back( start_index + v_size );
//
//		m_index_array.push_back( start_index );
//		m_index_array.push_back( start_index + 1 );
//		m_index_array.push_back( start_index + 1 + v_size );
//
//		m_index_array.push_back( start_index + v_size - 1 );
//		m_index_array.push_back( start_index + 2 * v_size - 2 );
//		m_index_array.push_back( start_index + 2 * v_size - 1 );
//
//		m_index_array.push_back( start_index + v_size - 1 );
//		m_index_array.push_back( start_index + v_size - 2 );
//		m_index_array.push_back( start_index + 2 * v_size - 2 );
//	}
//
//	void geometry_mesh::generate_polygon_ex( vector_3d* in_pos_array, ulong v_size, double in_height, ulong v_color )
//	{
//		vector3<double> v_average_normal = vector3<double>::s_zero;
//		for ( ulong i = 0; i < v_size; i ++ )
//		{
//			vector3<double> pos_geo = in_pos_array[i];
//			pos_geo.normalize();
//			v_average_normal += pos_geo;
//		}
//		v_average_normal /= v_size;
//		v_average_normal.normalize();
//
//		double lower_dist = spherical_coords::s_semi_major;
//		//double max_dist = 0.0;
//		for ( ulong i = 0; i < v_size; i ++ )
//		{
//			vector3<double> pos_geo = in_pos_array[i];
//
//			double cur_dist = vector3<double>::dot( pos_geo, v_average_normal );
//			lower_dist = min( lower_dist, cur_dist );
//			//max_dist   = max( max_dist, cur_dist );
//		}
//
//		// 上投影面上的一个点
//		vector3<double> v_upper_pos = ( spherical_coords::s_semi_major + in_height ) * v_average_normal;
//		// 下投影面上的一个点
//		vector3<double> v_lower_pos = ( lower_dist - in_height ) * v_average_normal;
//
//
//		// 将多边形投影到上下投影面上
//		coordinates_array v_upper_array;
//		coordinates_array v_lower_array;
//
//		double upper_height = v_upper_pos.length();
//		double lower_height = v_lower_pos.length();
//
//		for ( ulong i = 0; i < v_size; i ++ )
//		{
//			vector3<double> pos_geo;
//			pos_geo = in_pos_array[i];
//			pos_geo.normalize();
//			double v_length = upper_height / vector3<double>::dot( pos_geo, v_average_normal );
//			pos_geo = pos_geo * v_length;		
//			v_upper_array.push_back( pos_geo );
//
//			pos_geo = in_pos_array[i];
//			pos_geo.normalize();
//			v_length = lower_height / vector3<double>::dot( pos_geo, v_average_normal );
//			pos_geo = pos_geo * v_length;		
//			v_lower_array.push_back( pos_geo );
//		}
//
//
//		// 三角化
//		// 建立局部坐标系
//		vector3<double> v_x_axis, v_y_axis, v_z_axis;
//		v_x_axis = vector3<double>::normalize( v_upper_array[0] - v_upper_pos );
//		v_y_axis = v_average_normal;
//		v_z_axis = vector3<double>::cross( v_x_axis, v_y_axis );
//		v_z_axis.normalize();
//
//		matrix4<double> mat_tnb = matrix4<double>::from_tnbp( v_x_axis, v_y_axis, v_z_axis, v_upper_pos );
//
//		std::vector< vector2<double> > v_upper_array_local;
//		ulong max_ni = 0;
//		double max_z = 0.0;
//		std::vector< vector3<double> > v_upper_array_local_geo;
//		for ( ulong ni = 0; ni < v_size; ni ++ )
//		{
//			vector3<double> pos_geo = v_upper_array[ni] * mat_tnb.inverse() ;
//
//			if ( math<double>::fabs_( pos_geo.x ) < math<double>::s_zero_tolerance )
//			{
//				pos_geo.x = 0.0;
//			}
//			if ( math<double>::fabs_( pos_geo.y ) < math<double>::s_zero_tolerance )
//			{
//				pos_geo.y = 0.0;
//			}
//			if ( math<double>::fabs_( pos_geo.z ) < math<double>::s_zero_tolerance )
//			{
//				pos_geo.z = 0.0;
//			}
//
//			v_upper_array_local_geo.push_back( pos_geo );
//
//			v_upper_array_local.push_back( vector2<double>( pos_geo.x, pos_geo.z ) );
//
//			//us_vertex2d pos_2d;
//			//pos_2d.x = pos_geo.x;
//			//pos_2d.y = pos_geo.z;
//			//v_upper_array_local.push_back( pos_2d );
//
//			if( ni == 0 )
//			{
//				max_ni = 0;
//
//				max_z = pos_geo.z;
//			}
//			else
//			{
//				if( max_z < pos_geo.z )
//				{
//					max_z = pos_geo.z;
//					max_ni = ni;
//				}
//			}
//		}
//
//		int in_triangle_start_mode = 0;
//		vector3<double> prev_pos = v_upper_array_local_geo[ (max_ni - 1 + v_size ) % v_size ];
//		vector3<double> next_pos = v_upper_array_local_geo[ (max_ni + 1 + v_size ) % v_size ];
//		if( prev_pos.x < next_pos.x )
//		{
//			in_triangle_start_mode = 0;
//		}
//		else
//		{
//			in_triangle_start_mode = 1;
//		}
//
//
//		// 三角化
//		triangulation<double> v_triangulation_algorithm;
//		std::vector<ushort> v_temp_index_array;
//		v_temp_index_array.resize(( v_size - 2 ) * 3);
//		v_triangulation_algorithm.triangulation_polygon( v_upper_array_local.begin()._Myptr, v_size, v_temp_index_array.begin()._Myptr );
//
//
//		//
//		// 组织整个多面体
//		//
//
//		// 上表面
//		ulong start_index = (ulong)m_vertex_array.size();
//		for ( ulong i = 0; i < v_temp_index_array.size(); i += 3 )
//		{
//			if ( in_triangle_start_mode == 0 )
//			{
//				// CW
//				m_index_array.push_back( start_index + v_temp_index_array[i] );
//				m_index_array.push_back( start_index + v_temp_index_array[i+1] );
//				m_index_array.push_back( start_index + v_temp_index_array[i+2] );
//			}
//			else if ( in_triangle_start_mode == 1 )
//			{
//				// CCW
//				m_index_array.push_back( start_index + v_temp_index_array[i] );
//				m_index_array.push_back( start_index + v_temp_index_array[i+2] );
//				m_index_array.push_back( start_index + v_temp_index_array[i+1] );
//			}			
//		}
//
//		// 下表面
//		for ( ulong i = 0; i < v_temp_index_array.size(); i += 3 )
//		{
//			if ( in_triangle_start_mode == 0 )
//			{
//				// CW
//				m_index_array.push_back( start_index + v_temp_index_array[i] + v_size );
//				m_index_array.push_back( start_index + v_temp_index_array[i+2] + v_size);
//				m_index_array.push_back( start_index + v_temp_index_array[i+1] + v_size);
//			}
//			else if( in_triangle_start_mode == 1 )
//			{
//				// CCW
//				m_index_array.push_back( start_index + v_temp_index_array[i] + v_size);
//				m_index_array.push_back( start_index + v_temp_index_array[i+1] + v_size);
//				m_index_array.push_back( start_index + v_temp_index_array[i+2] + v_size);
//			}
//		}
//
//		// 侧面
//		for ( ulong i = 0; i < v_size; i++ )
//		{
//			if ( in_triangle_start_mode == 0 )
//			{
//				// CW
//				m_index_array.push_back( start_index + i );
//				m_index_array.push_back( start_index + i + v_size );
//				m_index_array.push_back( start_index + ( i + 1 ) % v_size + v_size );
//
//				m_index_array.push_back( start_index + i );
//				m_index_array.push_back( start_index + ( i + 1 ) % v_size + v_size );
//				m_index_array.push_back( start_index + ( i + 1 ) % v_size );
//			}
//			else if( in_triangle_start_mode == 1 )
//			{
//				// CCW
//				m_index_array.push_back( start_index + i );
//				m_index_array.push_back( start_index + ( i + 1 ) % v_size + v_size );
//				m_index_array.push_back( start_index + i + v_size );
//
//				m_index_array.push_back( start_index + i );
//				m_index_array.push_back( start_index + ( i + 1 ) % v_size );
//				m_index_array.push_back( start_index + ( i + 1 ) % v_size + v_size );
//			}
//		}
//
//		//
//		// 生成顶点集合
//		//
//		for ( ulong i = 0 ; i < v_upper_array.size(); i++ )
//		{			
//			m_render_aabb.combine( v_upper_array[i] );
//		}
//		for ( ulong i = 0 ; i < v_lower_array.size(); i++ )
//		{
//			m_render_aabb.combine( v_lower_array[i] );			
//		}
//
//		m_center = m_render_aabb.center();
//
//		position_color pc;
//		for ( ulong i = 0 ; i < v_upper_array.size(); i++ )
//		{			
//			pc.x = v_upper_array[i].x - m_center.x;
//			pc.y = v_upper_array[i].y - m_center.y;
//			pc.z = v_upper_array[i].z - m_center.z;
//			pc.color = v_color;
//
//			m_vertex_array.push_back( pc );
//		}
//
//		for ( ulong i = 0 ; i < v_lower_array.size(); i++ )
//		{
//			pc.x = v_lower_array[i].x - m_center.x;
//			pc.y = v_lower_array[i].y - m_center.y;
//			pc.z = v_lower_array[i].z - m_center.z;;
//			pc.color = v_color;
//
//			m_vertex_array.push_back( pc );
//		}
//	}
//
//	void geometry_mesh::generate_polygon_in_plane( vector_3d* in_pt_array, ulong v_size, double in_height, ulong v_color )
//	{
//		// 三角化
//		int max_ni = 0;
//		double max_z = -math_d::s_max_real;
//		std::vector<vector_2d> tri_array;
//		for ( ulong ni = 0; ni < v_size; ni ++ )
//		{
//			tri_array.push_back( vector_2d( in_pt_array[ni].x, in_pt_array[ni].z ) );
//			if ( in_pt_array[ni].z > max_z )
//			{
//				max_ni = ni;
//				max_z = in_pt_array[ni].z;
//			}
//
//		}
//
//		int in_triangle_start_mode = 0;
//		vector_2d prev_pos = tri_array[ (max_ni - 1 + v_size ) % v_size ];
//		vector_2d next_pos = tri_array[ (max_ni + 1 + v_size ) % v_size ];
//		if( prev_pos.x < next_pos.x )
//		{
//			in_triangle_start_mode = 1;
//		}
//		else
//		{
//			in_triangle_start_mode = 0;
//		}
//
//		// 三角化
//		triangulation<double> v_triangulation_algorithm;
//		std::vector<ushort> v_temp_index_array;
//		v_temp_index_array.resize(( v_size - 2 ) * 3);
//		v_triangulation_algorithm.triangulation_polygon( tri_array.begin()._Myptr, v_size, v_temp_index_array.begin()._Myptr );
//		
//		//
//		// 组织整个多面体
//		//
//
//		// 上表面
//		ulong start_index = (ulong)m_vertex_array.size();
//		for ( ulong i = 0; i < v_temp_index_array.size(); i += 3 )
//		{
//			if ( in_triangle_start_mode == 0 )
//			{
//				// CW
//				m_index_array.push_back( start_index + v_temp_index_array[i] );
//				m_index_array.push_back( start_index + v_temp_index_array[i+1] );
//				m_index_array.push_back( start_index + v_temp_index_array[i+2] );
//			}
//			else if ( in_triangle_start_mode == 1 )
//			{
//				// CCW
//				m_index_array.push_back( start_index + v_temp_index_array[i] );
//				m_index_array.push_back( start_index + v_temp_index_array[i+2] );
//				m_index_array.push_back( start_index + v_temp_index_array[i+1] );
//			}			
//		}
//
//		// 下表面
//		for ( ulong i = 0; i < v_temp_index_array.size(); i += 3 )
//		{
//			if ( in_triangle_start_mode == 0 )
//			{
//				// CW
//				m_index_array.push_back( start_index + v_temp_index_array[i] + v_size );
//				m_index_array.push_back( start_index + v_temp_index_array[i+2] + v_size);
//				m_index_array.push_back( start_index + v_temp_index_array[i+1] + v_size);
//			}
//			else if( in_triangle_start_mode == 1 )
//			{
//				// CCW
//				m_index_array.push_back( start_index + v_temp_index_array[i] + v_size);
//				m_index_array.push_back( start_index + v_temp_index_array[i+1] + v_size);
//				m_index_array.push_back( start_index + v_temp_index_array[i+2] + v_size);
//			}
//		}
//
//		// 侧面
//		for ( ulong i = 0; i < v_size; i++ )
//		{
//			if ( in_triangle_start_mode == 0 )
//			{
//				// CW
//				m_index_array.push_back( start_index + i );
//				m_index_array.push_back( start_index + i + v_size );
//				m_index_array.push_back( start_index + ( i + 1 ) % v_size + v_size );
//
//				m_index_array.push_back( start_index + i );
//				m_index_array.push_back( start_index + ( i + 1 ) % v_size + v_size );
//				m_index_array.push_back( start_index + ( i + 1 ) % v_size );
//			}
//			else if( in_triangle_start_mode == 1 )
//			{
//				// CCW
//				m_index_array.push_back( start_index + i );
//				m_index_array.push_back( start_index + ( i + 1 ) % v_size + v_size );
//				m_index_array.push_back( start_index + i + v_size );
//
//				m_index_array.push_back( start_index + i );
//				m_index_array.push_back( start_index + ( i + 1 ) % v_size );
//				m_index_array.push_back( start_index + ( i + 1 ) % v_size + v_size );
//			}
//		}
//
//		//
//		// 生成顶点集合
//		//
//		vector_3d vec_min( math_d::s_max_real, math_d::s_max_real, math_d::s_max_real );
//		vector_3d vec_max( -math_d::s_max_real, -math_d::s_max_real, -math_d::s_max_real );
//
//		m_vertex_array.resize( v_size * 2 );
//		for ( ulong i = 0 ; i < v_size; i++ )
//		{			
//			m_vertex_array[i].x = in_pt_array[i].x;
//			m_vertex_array[i].y = in_pt_array[i].y;
//			m_vertex_array[i].z = in_pt_array[i].z;
//			m_vertex_array[i].color = v_color;
//
//			vec_min.x = min( vec_min.x, in_pt_array[i].x );
//			vec_min.y = min( vec_min.y, in_pt_array[i].y );
//			vec_min.z = min( vec_min.z, in_pt_array[i].z );
//
//			vec_max.x = max( vec_max.x, in_pt_array[i].x );
//			vec_max.y = max( vec_max.y, in_pt_array[i].y );
//			vec_max.z = max( vec_max.z, in_pt_array[i].z );
//		}
//
//		int v_crsr = 0;
//		for ( ulong i = 0 ; i < v_size; i++ )
//		{
//			v_crsr = i + v_size;
//			m_vertex_array[v_crsr].x = in_pt_array[i].x;
//			m_vertex_array[v_crsr].y = in_pt_array[i].y + in_height;
//			m_vertex_array[v_crsr].z = in_pt_array[i].z;
//			m_vertex_array[v_crsr].color = v_color;
//
//			vec_min.x = min( vec_min.x, in_pt_array[i].x );
//			vec_min.y = min( vec_min.y, in_pt_array[i].y + in_height );
//			vec_min.z = min( vec_min.z, in_pt_array[i].z );
//
//			vec_max.x = max( vec_max.x, in_pt_array[i].x );
//			vec_max.y = max( vec_max.y, in_pt_array[i].y + in_height );
//			vec_max.z = max( vec_max.z, in_pt_array[i].z );
//		}
//
//	}
//
//	//*
//	void geometry_mesh::generate_polygon( vector_3d* in_pos_array, ulong v_size, double min_height, double max_height, ulong v_color )
//	{
//		//// 上投影面上的一个点
//		//vector3<double> v_upper_pos = spherical_coords::s_semi_major + max_height;
//		//// 下投影面上的一个点
//		//vector3<double> v_lower_pos = spherical_coords::s_semi_major + min_height
//
//		vector3<double> v_average_normal = vector3<double>::s_zero;
//		for ( ulong i = 0; i < v_size; i ++ )
//		{
//			vector3<double> pos_geo = in_pos_array[i];
//			pos_geo.normalize();
//			v_average_normal += pos_geo;
//		}
//		v_average_normal /= v_size;
//		v_average_normal.normalize();
//
//		double upper_height = spherical_coords::s_semi_major + max_height;
//		double lower_height = spherical_coords::s_semi_major + min_height;
//
//		vector3<double> v_upper_pos = upper_height * v_average_normal;
//		vector3<double> v_lower_pos = lower_height * v_average_normal;
//
//		// 将多边形投影到上下投影面上
//		coordinates_array v_upper_array;
//		coordinates_array v_lower_array;
//
//
//		for ( ulong i = 0; i < v_size; i ++ )
//		{
//			vector3<double> pos_geo;
//			pos_geo = in_pos_array[i];
//			pos_geo.normalize();
//			double v_length = upper_height / vector3<double>::dot( pos_geo, v_average_normal );
//			pos_geo = pos_geo * v_length;		
//			v_upper_array.push_back( pos_geo );
//
//			pos_geo = in_pos_array[i];
//			pos_geo.normalize();
//			v_length = lower_height / vector3<double>::dot( pos_geo, v_average_normal );
//			pos_geo = pos_geo * v_length;		
//			v_lower_array.push_back( pos_geo );
//		}
//
//
//		// 三角化
//		// 建立局部坐标系
//		vector3<double> v_x_axis, v_y_axis, v_z_axis;
//		v_x_axis = vector3<double>::normalize( v_upper_array[0] - v_upper_pos );
//		v_y_axis = v_average_normal;
//		v_z_axis = vector3<double>::cross( v_x_axis, v_y_axis );
//		v_z_axis.normalize();
//
//		matrix4<double> mat_tnb = matrix4<double>::from_tnbp( v_x_axis, v_y_axis, v_z_axis, v_upper_pos );
//
//		std::vector< vector2<double> > v_upper_array_local;
//		ulong max_ni = 0;
//		double max_z = 0.0;
//		std::vector< vector3<double> > v_upper_array_local_geo;
//		for ( ulong ni = 0; ni < v_size; ni ++ )
//		{
//			vector3<double> pos_geo = v_upper_array[ni] * mat_tnb.inverse() ;
//
//			if ( math<double>::fabs_( pos_geo.x ) < math<double>::s_zero_tolerance )
//			{
//				pos_geo.x = 0.0;
//			}
//			if ( math<double>::fabs_( pos_geo.y ) < math<double>::s_zero_tolerance )
//			{
//				pos_geo.y = 0.0;
//			}
//			if ( math<double>::fabs_( pos_geo.z ) < math<double>::s_zero_tolerance )
//			{
//				pos_geo.z = 0.0;
//			}
//
//			v_upper_array_local_geo.push_back( pos_geo );
//			v_upper_array_local.push_back( vector2<double>( pos_geo.x, pos_geo.z ) );
//
//			//us_vertex2d pos_2d;
//			//pos_2d.x = pos_geo.x * 100;
//			//pos_2d.y = pos_geo.z * 100;
//			//v_upper_array_local.push_back( pos_2d );
//
//			if( ni == 0 )
//			{
//				max_ni = 0;
//
//				max_z = pos_geo.z;
//			}
//			else
//			{
//				if( max_z < pos_geo.z )
//				{
//					max_z = pos_geo.z;
//					max_ni = ni;
//				}
//			}
//		}
//
//		int in_triangle_start_mode = 0;
//		vector3<double> prev_pos = v_upper_array_local_geo[ (max_ni - 1 + v_size ) % v_size ];
//		vector3<double> next_pos = v_upper_array_local_geo[ (max_ni + 1 + v_size ) % v_size ];
//		if( prev_pos.x < next_pos.x )
//		{
//			in_triangle_start_mode = 0;
//		}
//		else
//		{
//			in_triangle_start_mode = 1;
//		}
//
//
//		// 三角化
//		triangulation<double> v_triangulation_algorithm;
//		std::vector<ushort> v_temp_index_array;
//		v_temp_index_array.resize(( v_size - 2 ) * 3);
//		v_triangulation_algorithm.triangulation_polygon( v_upper_array_local.begin()._Myptr, v_size, v_temp_index_array.begin()._Myptr );
//
//
//		//
//		// 组织整个多面体
//		//
//
//		// 上表面
//		ulong start_index = (ulong)m_vertex_array.size();
//		for ( ulong i = 0; i < v_temp_index_array.size(); i += 3 )
//		{
//			if ( in_triangle_start_mode == 0 )
//			{
//				// CW
//				m_index_array.push_back( start_index + v_temp_index_array[i] );
//				m_index_array.push_back( start_index + v_temp_index_array[i+1] );
//				m_index_array.push_back( start_index + v_temp_index_array[i+2] );
//			}
//			else if ( in_triangle_start_mode == 1 )
//			{
//				// CCW
//				m_index_array.push_back( start_index + v_temp_index_array[i] );
//				m_index_array.push_back( start_index + v_temp_index_array[i+2] );
//				m_index_array.push_back( start_index + v_temp_index_array[i+1] );
//			}			
//		}
//
//		// 下表面
//		for ( ulong i = 0; i < v_temp_index_array.size(); i += 3 )
//		{
//			if ( in_triangle_start_mode == 0 )
//			{
//				// CW
//				m_index_array.push_back( start_index + v_temp_index_array[i] + v_size );
//				m_index_array.push_back( start_index + v_temp_index_array[i+2] + v_size);
//				m_index_array.push_back( start_index + v_temp_index_array[i+1] + v_size);
//			}
//			else if( in_triangle_start_mode == 1 )
//			{
//				// CCW
//				m_index_array.push_back( start_index + v_temp_index_array[i] + v_size);
//				m_index_array.push_back( start_index + v_temp_index_array[i+1] + v_size);
//				m_index_array.push_back( start_index + v_temp_index_array[i+2] + v_size);
//			}
//		}
//
//		// 侧面
//		for ( ulong i = 0; i < v_size; i++ )
//		{
//			if ( in_triangle_start_mode == 0 )
//			{
//				// CW
//				m_index_array.push_back( start_index + i );
//				m_index_array.push_back( start_index + i + v_size );
//				m_index_array.push_back( start_index + ( i + 1 ) % v_size + v_size );
//
//				m_index_array.push_back( start_index + i );
//				m_index_array.push_back( start_index + ( i + 1 ) % v_size + v_size );
//				m_index_array.push_back( start_index + ( i + 1 ) % v_size );
//			}
//			else if( in_triangle_start_mode == 1 )
//			{
//				// CCW
//				m_index_array.push_back( start_index + i );
//				m_index_array.push_back( start_index + ( i + 1 ) % v_size + v_size );
//				m_index_array.push_back( start_index + i + v_size );
//
//				m_index_array.push_back( start_index + i );
//				m_index_array.push_back( start_index + ( i + 1 ) % v_size );
//				m_index_array.push_back( start_index + ( i + 1 ) % v_size + v_size );
//			}
//		}
//
//		//
//		// 生成顶点集合
//		//
//		for ( ulong i = 0 ; i < v_upper_array.size(); i++ )
//		{			
//			m_render_aabb.combine( v_upper_array[i] );
//		}
//		for ( ulong i = 0 ; i < v_lower_array.size(); i++ )
//		{
//			m_render_aabb.combine( v_lower_array[i] );			
//		}
//
//		m_center = m_render_aabb.center();
//
//		position_color pc;
//		for ( ulong i = 0 ; i < v_upper_array.size(); i++ )
//		{			
//			pc.x = v_upper_array[i].x - m_center.x;
//			pc.y = v_upper_array[i].y - m_center.y;
//			pc.z = v_upper_array[i].z - m_center.z;
//			pc.color = v_color;
//
//			m_vertex_array.push_back( pc );
//		}
//
//		for ( ulong i = 0 ; i < v_lower_array.size(); i++ )
//		{
//			pc.x = v_lower_array[i].x - m_center.x;
//			pc.y = v_lower_array[i].y - m_center.y;
//			pc.z = v_lower_array[i].z - m_center.z;;
//			pc.color = v_color;
//
//			m_vertex_array.push_back( pc );
//		}
//	}	
//	//*/
//
//	void geometry_mesh::generate_polygon_coarse( vector_3d* in_pos_array, ulong v_size, double in_height, ulong v_color )
//	{
//		vector3<double> v_average_normal = vector3<double>::s_zero;
//		for ( ulong i = 0; i < v_size; i ++ )
//		{
//			vector3<double> pos_geo = in_pos_array[i];
//			pos_geo.normalize();
//			v_average_normal += pos_geo;
//		}
//		v_average_normal /= v_size;
//		v_average_normal.normalize();
//
//		double lower_dist = spherical_coords::s_semi_major;
//		//double max_dist = 0.0;
//		for ( ulong i = 0; i < v_size; i ++ )
//		{
//			vector3<double> pos_geo = in_pos_array[i];
//
//			double cur_dist = vector3<double>::dot( pos_geo, v_average_normal );
//			lower_dist = min( lower_dist, cur_dist );
//			//max_dist   = max( max_dist, cur_dist );
//		}
//
//		// 上投影面上的一个点
//		vector3<double> v_upper_pos = ( spherical_coords::s_semi_major + in_height ) * v_average_normal;
//		// 下投影面上的一个点
//		vector3<double> v_lower_pos = ( lower_dist - in_height ) * v_average_normal;
//
//
//		// 将多边形投影到上下投影面上
//		coordinates_array v_upper_array;
//		coordinates_array v_lower_array;
//
//		double upper_height = v_upper_pos.length();
//		double lower_height = v_lower_pos.length();
//
//		for ( ulong i = 0; i < v_size; i ++ )
//		{
//			vector3<double> pos_geo;
//			pos_geo = in_pos_array[i];
//			pos_geo.normalize();
//			double v_length = upper_height / vector3<double>::dot( pos_geo, v_average_normal );
//			pos_geo = pos_geo * v_length;		
//			v_upper_array.push_back( pos_geo );
//
//			pos_geo = in_pos_array[i];
//			pos_geo.normalize();
//			v_length = lower_height / vector3<double>::dot( pos_geo, v_average_normal );
//			pos_geo = pos_geo * v_length;		
//			v_lower_array.push_back( pos_geo );
//		}
//
//
//		// 三角化
//		// 建立局部坐标系
//		vector3<double> v_x_axis, v_y_axis, v_z_axis;
//		v_x_axis = vector3<double>::normalize( v_upper_array[0] - v_upper_pos );
//		v_y_axis = v_average_normal;
//		v_z_axis = vector3<double>::cross( v_x_axis, v_y_axis );
//		v_z_axis.normalize();
//
//		matrix4<double> mat_tnb = matrix4<double>::from_tnbp( v_x_axis, v_y_axis, v_z_axis, v_upper_pos );
//
//		std::vector< vector2<double> > v_upper_array_local;
//		ulong max_ni = 0;
//		double max_z = 0.0;
//		std::vector< vector3<double> > v_upper_array_local_geo;
//		for ( ulong ni = 0; ni < v_size; ni ++ )
//		{
//			vector3<double> pos_geo = v_upper_array[ni] * mat_tnb.inverse() ;
//
//			if ( math<double>::fabs_( pos_geo.x ) < math<double>::s_zero_tolerance )
//			{
//				pos_geo.x = 0.0;
//			}
//			if ( math<double>::fabs_( pos_geo.y ) < math<double>::s_zero_tolerance )
//			{
//				pos_geo.y = 0.0;
//			}
//			if ( math<double>::fabs_( pos_geo.z ) < math<double>::s_zero_tolerance )
//			{
//				pos_geo.z = 0.0;
//			}
//
//			v_upper_array_local_geo.push_back( pos_geo );
//
//			v_upper_array_local.push_back( vector2<double>( pos_geo.x, pos_geo.z ) );
//
//			//us_vertex2d pos_2d;
//			//pos_2d.x = pos_geo.x;
//			//pos_2d.y = pos_geo.z;
//			//v_upper_array_local.push_back( pos_2d );
//
//			if( ni == 0 )
//			{
//				max_ni = 0;
//
//				max_z = pos_geo.z;
//			}
//			else
//			{
//				if( max_z < pos_geo.z )
//				{
//					max_z = pos_geo.z;
//					max_ni = ni;
//				}
//			}
//		}
//
//		int in_triangle_start_mode = 0;
//		vector3<double> prev_pos = v_upper_array_local_geo[ (max_ni - 1 + v_size ) % v_size ];
//		vector3<double> next_pos = v_upper_array_local_geo[ (max_ni + 1 + v_size ) % v_size ];
//		if( prev_pos.x < next_pos.x )
//		{
//			in_triangle_start_mode = 0;
//		}
//		else
//		{
//			in_triangle_start_mode = 1;
//		}
//
//
//		// 三角化
//		triangulation<double> v_triangulation_algorithm;
//		std::vector<ushort> v_temp_index_array;
//		v_temp_index_array.resize(( v_size - 2 ) * 3);
//		v_triangulation_algorithm.triangulation_polygon( v_upper_array_local.begin()._Myptr, v_size, v_temp_index_array.begin()._Myptr );
//
//
//		//
//		// 组织整个多面体
//		//
//
//		// 上表面
//		ulong start_index = (ulong)m_vertex_array.size();
//		for ( ulong i = 0; i < v_temp_index_array.size(); i += 3 )
//		{
//			if ( in_triangle_start_mode == 0 )
//			{
//				// CW
//				m_index_array.push_back( start_index + v_temp_index_array[i] );
//				m_index_array.push_back( start_index + v_temp_index_array[i+1] );
//				m_index_array.push_back( start_index + v_temp_index_array[i+2] );
//			}
//			else if ( in_triangle_start_mode == 1 )
//			{
//				// CCW
//				m_index_array.push_back( start_index + v_temp_index_array[i] );
//				m_index_array.push_back( start_index + v_temp_index_array[i+2] );
//				m_index_array.push_back( start_index + v_temp_index_array[i+1] );
//			}			
//		}
//
//		// 下表面
//		for ( ulong i = 0; i < v_temp_index_array.size(); i += 3 )
//		{
//			if ( in_triangle_start_mode == 0 )
//			{
//				// CW
//				m_index_array.push_back( start_index + v_temp_index_array[i] + v_size );
//				m_index_array.push_back( start_index + v_temp_index_array[i+2] + v_size);
//				m_index_array.push_back( start_index + v_temp_index_array[i+1] + v_size);
//			}
//			else if( in_triangle_start_mode == 1 )
//			{
//				// CCW
//				m_index_array.push_back( start_index + v_temp_index_array[i] + v_size);
//				m_index_array.push_back( start_index + v_temp_index_array[i+1] + v_size);
//				m_index_array.push_back( start_index + v_temp_index_array[i+2] + v_size);
//			}
//		}
//
//		// 侧面
//		for ( ulong i = 0; i < v_size; i++ )
//		{
//			if ( in_triangle_start_mode == 0 )
//			{
//				// CW
//				m_index_array.push_back( start_index + i );
//				m_index_array.push_back( start_index + i + v_size );
//				m_index_array.push_back( start_index + ( i + 1 ) % v_size + v_size );
//
//				m_index_array.push_back( start_index + i );
//				m_index_array.push_back( start_index + ( i + 1 ) % v_size + v_size );
//				m_index_array.push_back( start_index + ( i + 1 ) % v_size );
//			}
//			else if( in_triangle_start_mode == 1 )
//			{
//				// CCW
//				m_index_array.push_back( start_index + i );
//				m_index_array.push_back( start_index + ( i + 1 ) % v_size + v_size );
//				m_index_array.push_back( start_index + i + v_size );
//
//				m_index_array.push_back( start_index + i );
//				m_index_array.push_back( start_index + ( i + 1 ) % v_size );
//				m_index_array.push_back( start_index + ( i + 1 ) % v_size + v_size );
//			}
//		}
//
//		//
//		// 生成顶点集合
//		//
//		position_color pc;
//		for ( ulong i = 0 ; i < v_upper_array.size(); i++ )
//		{			
//			pc.x = v_upper_array[i].x;
//			pc.y = v_upper_array[i].y;
//			pc.z = v_upper_array[i].z;
//			pc.color = v_color;
//
//			m_vertex_array.push_back( pc );
//			m_render_aabb.combine( v_upper_array[i] );
//		}
//
//		for ( ulong i = 0 ; i < v_lower_array.size(); i++ )
//		{
//			pc.x = v_lower_array[i].x;
//			pc.y = v_lower_array[i].y;
//			pc.z = v_lower_array[i].z;
//			pc.color = v_color;
//
//			m_vertex_array.push_back( pc );
//			m_render_aabb.combine( v_lower_array[i] );			
//		}
//	}

}