
///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_render_object_polygon_factory.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : render_object_polygon_factory class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_render_object_polygon_factory.h"

namespace uniscope_globe
{
	void render_object_polygon_factory::create_fill_polygon_cart( render_object_polygon* geo_polygon, coordinates_part_array& v_vecs_cart, ulong v_fill_color )
	{
		// 拷贝顶点
		int v_vertex_num = 0;
		for ( int ni = 0; ni < v_vecs_cart.size(); ni++ )
		{
			v_vertex_num += v_vecs_cart[ni].size();
		}

		geo_polygon->m_vertex_array.clear();
		geo_polygon->m_vertex_array.resize( v_vertex_num );
		int v_vertex_index = 0;

		vector_3d vec_min(	FLT_MAX,  FLT_MAX,  FLT_MAX );
		vector_3d vec_max( -FLT_MAX, -FLT_MAX, -FLT_MAX );
		for ( int ni = 0; ni < v_vecs_cart.size(); ni++ )
		{
			coordinates_array& v_coord_array = v_vecs_cart[ni];

			for ( int nj = 0; nj < v_coord_array.size(); nj++ )
			{
				vec_min.x = min( vec_min.x, v_coord_array[nj].x );
				vec_min.y = min( vec_min.y, v_coord_array[nj].y );
				vec_min.z = min( vec_min.z, v_coord_array[nj].z );

				vec_max.x = max( vec_max.x, v_coord_array[nj].x );
				vec_max.y = max( vec_max.y, v_coord_array[nj].y );
				vec_max.z = max( vec_max.z, v_coord_array[nj].z );

				geo_polygon->m_vertex_array[v_vertex_index].x = v_coord_array[nj].x;
				geo_polygon->m_vertex_array[v_vertex_index].y = v_coord_array[nj].y;
				geo_polygon->m_vertex_array[v_vertex_index].z = v_coord_array[nj].z;
				geo_polygon->m_vertex_array[v_vertex_index].color = v_fill_color;

				v_vertex_index ++;
			}
		}

		geo_polygon->m_render_aabb.set_extent( vec_min, vec_max );

		// 三角化
		vector<int> tri_index;
		polygon_triangulation v_triangulation_algorithm;
		v_triangulation_algorithm.do_trianglulate_lh( v_vecs_cart, tri_index );

		geo_polygon->m_triangle_index.clear();
		geo_polygon->m_triangle_index.resize( tri_index.size() );
		for ( int ni = 0; ni < geo_polygon->m_triangle_index.size(); ni++ )
		{
			geo_polygon->m_triangle_index[ni] = tri_index[ni];
		}
	}

	void render_object_polygon_factory::create_fill_polygon_geo( render_object_polygon* geo_polygon, 
		coordinates_array& v_vecs_geo, part_array& v_part, matrix_4d& v_mat_geo2cart, ulong v_fill_color )
	{
		// 拷贝顶点
		int v_vertex_num = v_vecs_geo.size();

		geo_polygon->m_vertex_array.clear();
		geo_polygon->m_vertex_array.resize( v_vertex_num );

		vector_3d vec_min(	FLT_MAX,  FLT_MAX,  FLT_MAX );
		vector_3d vec_max( -FLT_MAX, -FLT_MAX, -FLT_MAX );
		for( int ni = 0; ni < v_vertex_num; ni++ )
		{
			vec_min.x = min( vec_min.x, v_vecs_geo[ni].x );
			vec_min.y = min( vec_min.y, v_vecs_geo[ni].y );
			vec_min.z = min( vec_min.z, v_vecs_geo[ni].z );

			vec_max.x = max( vec_max.x, v_vecs_geo[ni].x );
			vec_max.y = max( vec_max.y, v_vecs_geo[ni].y );
			vec_max.z = max( vec_max.z, v_vecs_geo[ni].z );

			geo_polygon->m_vertex_array[ni].x = v_vecs_geo[ni].x;
			geo_polygon->m_vertex_array[ni].y = v_vecs_geo[ni].y;
			geo_polygon->m_vertex_array[ni].z = v_vecs_geo[ni].z;
			geo_polygon->m_vertex_array[ni].color = v_fill_color;
		}
		
		geo_polygon->m_render_aabb.set_extent( vec_min, vec_max );

		// 三角化
		vector<int> tri_index;
		polygon_triangulation v_triangulation_algorithm;
		v_triangulation_algorithm.do_trianglulate_lh( v_vecs_geo, v_part, tri_index );

		geo_polygon->m_triangle_index.clear();
		geo_polygon->m_triangle_index.resize( tri_index.size() );
		for ( int ni = 0; ni < geo_polygon->m_triangle_index.size(); ni++ )
		{
			geo_polygon->m_triangle_index[ni] = tri_index[ni];
		}
	}

	void render_object_polygon_factory::create_fill_polygon_geo( render_object_polygon* geo_polygon,
		vector_3d* v_vecs_geo, int v_vecs_num, matrix_4d& v_mat_geo2cart, ulong v_fill_color )
	{
		geo_polygon->m_vertex_array.clear();
		geo_polygon->m_vertex_array.resize( v_vecs_num );

		vector_3d vec = v_vecs_geo[0] * v_mat_geo2cart;
		geo_polygon->m_vertex_array[0].x = vec.x;
		geo_polygon->m_vertex_array[0].y = vec.y;
		geo_polygon->m_vertex_array[0].z = vec.z;
		geo_polygon->m_vertex_array[0].color = v_fill_color;

		vector_3d vec_min( vec.x , vec.y, vec.z );
		vector_3d vec_max( vec.x , vec.y, vec.z );

		for( int ni = 1; ni < v_vecs_num; ni++ )
		{
			vec = v_vecs_geo[ni] * v_mat_geo2cart;

			geo_polygon->m_vertex_array[ni].x = vec.x;
			geo_polygon->m_vertex_array[ni].y = vec.y;
			geo_polygon->m_vertex_array[ni].z = vec.z;
			geo_polygon->m_vertex_array[ni].color = v_fill_color;

			vec_min.x = min( vec_min.x, vec.x );
			vec_min.y = min( vec_min.y, vec.y );
			vec_min.z = min( vec_min.z, vec.z );

			vec_max.x = max( vec_max.x, vec.x );
			vec_max.y = max( vec_max.y, vec.y );
			vec_max.z = max( vec_max.z, vec.z );
		}
		geo_polygon->m_render_aabb.set_extent( vec_min, vec_max );

		generate_polygon_triangle_list_geo( geo_polygon, v_vecs_geo, v_vecs_num );
	}

	void render_object_polygon_factory::generate_polygon_triangle_list_geo( render_object_polygon* geo_polygon, vector_3d* v_vecs_geo, int v_vecs_num )
	{
		// calculate direction
		// 0:CW    1:CCW
		int in_triangle_start_mode = 0;

		vector3<double> temp_vec1 = v_vecs_geo[1] - v_vecs_geo[0];
		vector3<double> temp_vec2 = v_vecs_geo[2] - v_vecs_geo[0];
		vector3<double> temp_cross = vector3<double>::cross( temp_vec2, temp_vec1 );
		double temp_value = vector3<double>::dot( temp_cross, v_vecs_geo[1] );

		if ( temp_value > 0 )
		{
			in_triangle_start_mode =  1;
		}

		// calculate average normal
		vector3<double> v_average_normal = vector3<double>::s_zero;
		for ( int i = 0; i < v_vecs_num; i ++ )
		{
			vector3<double> pos_geo = v_vecs_geo[i];
			pos_geo.normalize();

			v_average_normal += pos_geo;
		}
		v_average_normal /= v_vecs_num;
		v_average_normal.normalize();

		// get a point on projection surface
		vector3<double> v_proj_surface_pos = spherical_coords::s_semi_major * v_average_normal;

		// project polygons to the surface
		coordinates_array v_proj_array;
		double proj_height = v_proj_surface_pos.length();
		for ( int i = 0; i < v_vecs_num; i ++ )
		{
			vector3<double> pos_geo;
			pos_geo = v_vecs_geo[i];
			pos_geo.normalize();
			double v_length = proj_height / vector3<double>::dot( pos_geo, v_average_normal );
			pos_geo = pos_geo * v_length;		
			v_proj_array.push_back( pos_geo );
		}

		// generate sub coordinates
		vector3<double> v_x_axis, v_y_axis, v_z_axis;
		v_x_axis = vector3<double>::normalize( v_proj_array[0] - v_proj_surface_pos );
		v_y_axis = v_average_normal;
		v_z_axis = vector3<double>::cross( v_x_axis, v_y_axis );
		v_z_axis.normalize();

		matrix4<double> mat_tnb = matrix4<double>::from_tnbp( v_x_axis, v_y_axis, v_z_axis, v_proj_surface_pos );

		std::vector< vector2<double> > v_array_local;
		for ( int ni = 0; ni < v_vecs_num; ni ++ )
		{
			vector3<double> pos_geo = v_proj_array[ni] * mat_tnb.inverse() ;

			if ( math<double>::fabs_( pos_geo.x ) < math<double>::s_zero_tolerance )
			{
				pos_geo.x = 0.0;
			}
			if ( math<double>::fabs_( pos_geo.y ) < math<double>::s_zero_tolerance )
			{
				pos_geo.y = 0.0;
			}
			if ( math<double>::fabs_( pos_geo.z ) < math<double>::s_zero_tolerance )
			{
				pos_geo.z = 0.0;
			}

			v_array_local.push_back( vector2<double>( pos_geo.x, pos_geo.z ) );
		}

		// generate triangle list
		geo_polygon->m_triangle_index.clear();
		geo_polygon->m_triangle_index.resize( (v_vecs_num-2)*3 );

		triangulation<double> v_triangulation_algorithm;
		assert(!v_array_local.empty());
		assert(!geo_polygon->m_triangle_index.empty());
		v_triangulation_algorithm.triangulation_polygon( &v_array_local[0], v_vecs_num, &(geo_polygon->m_triangle_index[0]) );
	}

	void render_object_polygon_factory::create_fill_polygon_cart( render_object_polygon* geo_polygon, vector_3d* v_vecs_cart, int v_vecs_num, ulong v_line_color, ulong v_fill_color )
	{
		geo_polygon->m_vertex_array.clear();
		geo_polygon->m_vertex_array.resize( v_vecs_num );

		geo_polygon->m_line_array.clear();
		geo_polygon->m_line_array.resize( v_vecs_num );

		vector_3d vec = v_vecs_cart[0];

		geo_polygon->m_line_array[0].x = vec.x;
		geo_polygon->m_line_array[0].y = vec.y;
		geo_polygon->m_line_array[0].z = vec.z;
		geo_polygon->m_line_array[0].color = v_line_color;
		
		geo_polygon->m_vertex_array[0].x = vec.x;
		geo_polygon->m_vertex_array[0].y = vec.y;
		geo_polygon->m_vertex_array[0].z = vec.z;
		geo_polygon->m_vertex_array[0].color = v_fill_color;

		vector_3d vec_min( vec.x , vec.y, vec.z );
		vector_3d vec_max( vec.x , vec.y, vec.z );

		for( int ni = 1; ni < v_vecs_num; ni++ )
		{
			vec = v_vecs_cart[ni];

			geo_polygon->m_line_array[ni].x = vec.x;
			geo_polygon->m_line_array[ni].y = vec.y;
			geo_polygon->m_line_array[ni].z = vec.z;
			geo_polygon->m_line_array[ni].color = v_line_color;

			geo_polygon->m_vertex_array[ni].x = vec.x;
			geo_polygon->m_vertex_array[ni].y = vec.y;
			geo_polygon->m_vertex_array[ni].z = vec.z;
			geo_polygon->m_vertex_array[ni].color = v_fill_color;

			vec_min.x = min( vec_min.x, vec.x );
			vec_min.y = min( vec_min.y, vec.y );
			vec_min.z = min( vec_min.z, vec.z );

			vec_max.x = max( vec_max.x, vec.x );
			vec_max.y = max( vec_max.y, vec.y );
			vec_max.z = max( vec_max.z, vec.z );
		}
		geo_polygon->m_render_aabb.set_extent( vec_min, vec_max );

		generate_polygon_triangle_list_cart( geo_polygon, v_vecs_cart, v_vecs_num );
	}

	void render_object_polygon_factory::generate_polygon_triangle_list_cart( render_object_polygon* geo_polygon, vector_3d* v_vecs_cart, int v_vecs_num )
	{
		// 三角化
		int max_ni = 0;
		double max_z = -math_d::s_max_real;
		std::vector<vector_2d> tri_array;
		for ( int ni = 0; ni < v_vecs_num; ni ++ )
		{
			tri_array.push_back( vector_2d( v_vecs_cart[ni].x, v_vecs_cart[ni].z ) );
			if ( v_vecs_cart[ni].z > max_z )
			{
				max_ni = 0;
				max_z = v_vecs_cart[ni].z;
			}
		}

		int in_triangle_start_mode = 0;
		vector_2d prev_pos = tri_array[ (max_ni - 1 + v_vecs_num ) % v_vecs_num ];
		vector_2d next_pos = tri_array[ (max_ni + 1 + v_vecs_num ) % v_vecs_num ];
		if( prev_pos.x < next_pos.x )
		{
			in_triangle_start_mode = 1;
		}
		else
		{
			in_triangle_start_mode = 0;
		}

		// 三角化
		triangulation<double> v_triangulation_algorithm;
		geo_polygon->m_triangle_index.clear();
		geo_polygon->m_triangle_index.resize(( v_vecs_num - 2 ) * 3);
		v_triangulation_algorithm.triangulation_polygon( &tri_array[0], v_vecs_num, &(geo_polygon->m_triangle_index[0]) );
	}

	void render_object_polygon_factory::create_fill_circle_cart( render_object_polygon* geo_polygon, 
		vector_3d& v_center_cart, double v_radius_metric, ulong v_line_color, ulong v_fill_color, ulong v_segment )
	{
		int v_vertex_size = v_segment + 1;
		vector_3d* v_circle_points = new vector_3d[v_vertex_size];
		shape_creator::create_circle_on_plane( v_circle_points, v_center_cart, v_radius_metric, v_segment );
		v_circle_points[v_segment] = v_circle_points[0];

		create_fill_polygon_cart( geo_polygon, v_circle_points, v_vertex_size, v_line_color, v_fill_color);
	}


	void render_object_polygon_factory::create_stencil_mesh_from_polygon_geo( render_object_geometry_mesh* geo_mesh,
		vector_3d* v_vecs_geo, int v_vecs_num, ulong v_fill_color, double in_upper_height, double in_lower_height )
	{
		if ( v_vecs_num < 3 ) return;

		geo_mesh->clear();

		coordinates_array v_array;
		generate_polygon_stencil_mesh( geo_mesh, v_vecs_geo, v_vecs_num, v_array, v_fill_color, in_upper_height, in_lower_height - 0.01 );

		geo_mesh->m_center = geo_mesh->m_render_aabb.center();

		for ( int i = 0; i < (int)v_array.size(); i ++ )
		{
			position_color pc;
			pc.x = v_array[i].x - geo_mesh->m_center.x;
			pc.y = v_array[i].y - geo_mesh->m_center.y;
			pc.z = v_array[i].z - geo_mesh->m_center.z;
			pc.color = v_fill_color;
			geo_mesh->m_vertex_array.push_back( pc );
		}

		matrix_4d v_collapse_mat = matrix4<double>::translation( geo_mesh->m_center.x, geo_mesh->m_center.y, geo_mesh->m_center.z );
		geo_mesh->set_collapse( v_collapse_mat );
	}

	void render_object_polygon_factory::create_stencil_mesh_from_multiple_polygon_geo( render_object_geometry_mesh* geo_mesh,
		vector3<double>* v_vecs_geo, ulong v_vecs_num, ulong* v_part_info, ulong v_part_num, ulong v_fill_color, double in_upper_height, double in_lower_height )
	{
		geo_mesh->clear();

		coordinates_array v_array;
		ulong cur_index = 0;
		for ( int i = 0; i < (int)v_part_num; i++ )
		{
			if( v_part_info[i] < 3 )
			{
				cur_index += v_part_info[i];
				continue;
			}

			generate_polygon_stencil_mesh( geo_mesh, &v_vecs_geo[cur_index], v_part_info[i], v_array, v_fill_color, in_upper_height, in_lower_height - 0.01 );

			cur_index += v_part_info[i];
		}

		geo_mesh->m_center = geo_mesh->m_render_aabb.center();


		for ( int i = 0; i < (int)v_array.size(); i ++ )
		{
			position_color pc;
			pc.x = v_array[i].x - geo_mesh->m_center.x;
			pc.y = v_array[i].y - geo_mesh->m_center.y;
			pc.z = v_array[i].z - geo_mesh->m_center.z;
			pc.color = v_fill_color;
			geo_mesh->m_vertex_array.push_back( pc );
		}

		matrix_4d v_collapse_mat = matrix4<double>::translation( geo_mesh->m_center.x, geo_mesh->m_center.y, geo_mesh->m_center.z );
		geo_mesh->set_collapse( v_collapse_mat );
	}

	void render_object_polygon_factory::create_stencil_mesh_from_polygon_geo( render_object_geometry_mesh* geo_mesh,
		vector_3d* v_vecs_geo, int v_vecs_num, matrix_4d& v_mat_geo2cart, ulong v_fill_color, double in_upper_height, double in_lower_height )
	{
		if ( v_vecs_num < 3 ) return;

		geo_mesh->clear();

		coordinates_array v_array;
		generate_polygon_stencil_mesh( geo_mesh, v_vecs_geo, v_vecs_num, v_array, v_fill_color, in_upper_height, in_lower_height - 0.01 );

		axis_aligned_box_d v_aabb;
		v_aabb.make_invalid();
		for ( int i = 0; i < (int)v_array.size(); i ++ )
		{
			v_array[i] = v_array[i] * v_mat_geo2cart;
			v_aabb.combine( v_array[i] );

			position_color pc;
			pc.x = v_array[i].x;
			pc.y = v_array[i].y;
			pc.z = v_array[i].z;
			pc.color = v_fill_color;
			geo_mesh->m_vertex_array.push_back( pc );
		}

		geo_mesh->set_render_aabb( v_aabb );
	}

	void render_object_polygon_factory::generate_polygon_stencil_mesh( render_object_geometry_mesh* geo_mesh,
		vector_3d* v_vecs_geo, ulong v_vecs_num, coordinates_array& out_array, ulong v_fill_color, double in_upper_height, double in_lower_height )
	{
		geo_mesh->set_shadow_color( v_fill_color );

		// 计算平均法向量
		vector3<double> v_average_normal = vector3<double>::s_zero;
		for ( int i = 0; i < (int)v_vecs_num; i ++ )
		{
			vector3<double> pos_geo = v_vecs_geo[i];
			pos_geo.normalize();
			v_average_normal += pos_geo;
		}
		v_average_normal /= v_vecs_num;
		v_average_normal.normalize();

		// 计算平均法向量上的最小投影点
		double lower_dist = spherical_coords::s_semi_major;
		for ( int i = 0; i < (int)v_vecs_num; i ++ )
		{
			vector3<double> pos_geo = v_vecs_geo[i];

			double cur_dist = vector3<double>::dot( pos_geo, v_average_normal );
			lower_dist = min( lower_dist, cur_dist );
		}

		// 上投影面上的一个点
		vector3<double> v_upper_pos = in_upper_height * v_average_normal;
		// 下投影面上的一个点
		vector3<double> v_lower_pos = in_lower_height * v_average_normal;

		// 将多边形投影到上下投影面上
		coordinates_array v_upper_array;
		coordinates_array v_lower_array;

		double upper_height = v_upper_pos.length();
		double lower_height = v_lower_pos.length();

		for ( int i = 0; i < (int)v_vecs_num; i ++ )
		{
			vector3<double> pos_geo;
			pos_geo = v_vecs_geo[i];
			pos_geo.normalize();
			double v_length = upper_height / vector3<double>::dot( pos_geo, v_average_normal );
			pos_geo = pos_geo * v_length;		
			v_upper_array.push_back( pos_geo );

			pos_geo = v_vecs_geo[i];
			pos_geo.normalize();
			v_length = lower_height / vector3<double>::dot( pos_geo, v_average_normal );
			pos_geo = pos_geo * v_length;		
			v_lower_array.push_back( pos_geo );
		}

		// 三角化
		// 建立局部坐标系
		vector3<double> v_x_axis, v_y_axis, v_z_axis;
		v_x_axis = vector3<double>::normalize( v_upper_array[0] - v_upper_pos );
		v_y_axis = v_average_normal;
		v_z_axis = vector3<double>::cross( v_x_axis, v_y_axis );
		v_z_axis.normalize();

		matrix4<double> mat_tnb = matrix4<double>::from_tnbp( v_x_axis, v_y_axis, v_z_axis, v_upper_pos );

		std::vector< vector2<double> > v_upper_array_local;
		ulong max_ni = 0;
		double max_z = 0.0;
		std::vector< vector3<double> > v_upper_array_local_geo;
		for ( int ni = 0; ni < (int)v_vecs_num; ni ++ )
		{
			vector3<double> pos_geo = v_upper_array[ni] * mat_tnb.inverse() ;

			if ( math<double>::fabs_( pos_geo.x ) < math<double>::s_zero_tolerance )
			{
				pos_geo.x = 0.0;
			}
			if ( math<double>::fabs_( pos_geo.y ) < math<double>::s_zero_tolerance )
			{
				pos_geo.y = 0.0;
			}
			if ( math<double>::fabs_( pos_geo.z ) < math<double>::s_zero_tolerance )
			{
				pos_geo.z = 0.0;
			}

			v_upper_array_local_geo.push_back( pos_geo );

			v_upper_array_local.push_back( vector2<double>( pos_geo.x, pos_geo.z ) );

			if( ni == 0 )
			{
				max_ni = 0;
				max_z = pos_geo.z;
			}
			else
			{
				if( max_z < pos_geo.z )
				{
					max_z = pos_geo.z;
					max_ni = ni;
				}
			}
		}

		int in_triangle_start_mode = 0;
		vector3<double> prev_pos, next_pos, curr_pos;
		curr_pos = v_upper_array_local_geo[max_ni];
		for (int i = 0; i < (int)v_vecs_num; i ++)
		{
			next_pos = v_upper_array_local_geo[(max_ni + 1) % v_vecs_num];
			if ((next_pos - curr_pos).length() > 0.000001)
				break;
		}
		for (int i = 0; i < (int)v_vecs_num; i ++)
		{
			prev_pos = v_upper_array_local_geo[(max_ni + v_vecs_num - 1) % v_vecs_num];
			if ((prev_pos - curr_pos).length() > 0.000001)
				break;
		}

		prev_pos = prev_pos - curr_pos;
		prev_pos.normalize();
		prev_pos = prev_pos + curr_pos;
		next_pos = next_pos - curr_pos;
		next_pos.normalize();
		next_pos = next_pos + curr_pos;

		if( prev_pos.x < next_pos.x )
		{
			in_triangle_start_mode = 0;
		}
		else
		{
			in_triangle_start_mode = 1;
		}

		// 三角化
		triangulation<double> v_triangulation_algorithm;
		std::vector<ushort> v_temp_index_array;
		v_temp_index_array.resize(( v_vecs_num - 2 ) * 3);
		v_triangulation_algorithm.triangulation_polygon( &v_upper_array_local[0], v_vecs_num, &v_temp_index_array[0] );

		//
		// 组织多面体索引
		//

		// 上表面
		ulong start_index = out_array.size();
		for ( int i = 0; i < (int)v_temp_index_array.size(); i += 3 )
		{
			if ( in_triangle_start_mode == 0 )
			{
				// CW
				geo_mesh->m_index_array.push_back( start_index + v_temp_index_array[i] );
				geo_mesh->m_index_array.push_back( start_index + v_temp_index_array[i+1] );
				geo_mesh->m_index_array.push_back( start_index + v_temp_index_array[i+2] );
			}
			else if ( in_triangle_start_mode == 1 )
			{
				// CCW
				geo_mesh->m_index_array.push_back( start_index + v_temp_index_array[i] );
				geo_mesh->m_index_array.push_back( start_index + v_temp_index_array[i+2] );
				geo_mesh->m_index_array.push_back( start_index + v_temp_index_array[i+1] );
			}			
		}

		// 下表面
		for ( int i = 0; i < (int)v_temp_index_array.size(); i += 3 )
		{
			if ( in_triangle_start_mode == 0 )
			{
				// CW
				geo_mesh->m_index_array.push_back( start_index + v_temp_index_array[i] + v_vecs_num );
				geo_mesh->m_index_array.push_back( start_index + v_temp_index_array[i+2] + v_vecs_num);
				geo_mesh->m_index_array.push_back( start_index + v_temp_index_array[i+1] + v_vecs_num);
			}
			else if( in_triangle_start_mode == 1 )
			{
				// CCW
				geo_mesh->m_index_array.push_back( start_index + v_temp_index_array[i] + v_vecs_num);
				geo_mesh->m_index_array.push_back( start_index + v_temp_index_array[i+1] + v_vecs_num);
				geo_mesh->m_index_array.push_back( start_index + v_temp_index_array[i+2] + v_vecs_num);
			}
		}

		// 侧面
		for ( int i = 0; i < (int)v_vecs_num; i++ )
		{
			if ( in_triangle_start_mode == 0 )
			{
				// CW
				geo_mesh->m_index_array.push_back( start_index + i );
				geo_mesh->m_index_array.push_back( start_index + i + v_vecs_num );
				geo_mesh->m_index_array.push_back( start_index + ( i + 1 ) % v_vecs_num + v_vecs_num );

				geo_mesh->m_index_array.push_back( start_index + i );
				geo_mesh->m_index_array.push_back( start_index + ( i + 1 ) % v_vecs_num + v_vecs_num );
				geo_mesh->m_index_array.push_back( start_index + ( i + 1 ) % v_vecs_num );
			}
			else if( in_triangle_start_mode == 1 )
			{
				// CCW
				geo_mesh->m_index_array.push_back( start_index + i );
				geo_mesh->m_index_array.push_back( start_index + ( i + 1 ) % v_vecs_num + v_vecs_num );
				geo_mesh->m_index_array.push_back( start_index + i + v_vecs_num );

				geo_mesh->m_index_array.push_back( start_index + i );
				geo_mesh->m_index_array.push_back( start_index + ( i + 1 ) % v_vecs_num );
				geo_mesh->m_index_array.push_back( start_index + ( i + 1 ) % v_vecs_num + v_vecs_num );
			}
		}

		// 生成顶点集合
		for ( int i = 0 ; i < (int)v_upper_array.size(); i++ )
		{			
			geo_mesh->m_render_aabb.combine( v_upper_array[i] );
			out_array.push_back( v_upper_array[i] );
		}

		for ( int i = 0 ; i < (int)v_lower_array.size(); i++ )
		{
			geo_mesh->m_render_aabb.combine( v_lower_array[i] );
			out_array.push_back( v_lower_array[i] );
		}
	}


	void render_object_polygon_factory::create_stencil_mesh_from_polygon_cart( render_object_geometry_mesh* geo_mesh, 
		coordinates_part_array& v_vecs_cart, ulong v_fill_color, double in_upper_height, double in_lower_height )
	{
		if ( v_vecs_cart.size() == 0 )
			return;

		int in_triangle_start_mode = 0;
		bool b_ccw = polygon_triangulation::is_polygon_ccw( v_vecs_cart[0] );
		if ( b_ccw ) in_triangle_start_mode = 1;

		for ( int ni = 0; ni < v_vecs_cart.size(); ni++ )
		{
			coordinates_array& v_coord_array = v_vecs_cart[ni];
			v_coord_array.push_back( v_coord_array[0] );
		}
		
		// 三角化
		vector<int> v_temp_index_array;
		polygon_triangulation v_triangulation_algorithm;
		v_triangulation_algorithm.do_trianglulate_lh( v_vecs_cart, v_temp_index_array );

		// 顶点
		int v_vecs_num = 0;
		for ( int ni = 0; ni < v_vecs_cart.size(); ni++ )
		{
			coordinates_array& v_coord_array = v_vecs_cart[ni];
			v_vecs_num += v_coord_array.size();
		}

		for ( int ni = 0; ni < v_vecs_cart.size(); ni++ )
		{
			coordinates_array& v_coord_array = v_vecs_cart[ni];
			for ( int nj = 0; nj < v_coord_array.size(); nj++ )
			{
				position_color pc;
				pc.x = v_coord_array[nj].x;
				pc.y = in_upper_height;
				pc.z = v_coord_array[nj].z;
				pc.color = v_fill_color;
				geo_mesh->m_vertex_array.push_back( pc );
			}
		}
		for ( int ni = 0; ni < v_vecs_cart.size(); ni++ )
		{
			coordinates_array& v_coord_array = v_vecs_cart[ni];
			for ( int nj = 0; nj < v_coord_array.size(); nj++ )
			{
				position_color pc;
				pc.x = v_coord_array[nj].x;
				pc.y = in_lower_height;
				pc.z = v_coord_array[nj].z;
				pc.color = v_fill_color;
				geo_mesh->m_vertex_array.push_back( pc );
			}
		}
		geo_mesh->set_shadow_color( v_fill_color );

		//
		// 组织多面体索引
		//
		// 上表面
		ulong start_index = 0;
		for ( int i = 0; i < (int)v_temp_index_array.size(); i += 3 )
		{
			geo_mesh->m_index_array.push_back( start_index + v_temp_index_array[i] );
			geo_mesh->m_index_array.push_back( start_index + v_temp_index_array[i+2] );
			geo_mesh->m_index_array.push_back( start_index + v_temp_index_array[i+1] );
		}

		// 下表面
		for ( int i = 0; i < (int)v_temp_index_array.size(); i += 3 )
		{
			geo_mesh->m_index_array.push_back( start_index + v_temp_index_array[i] + v_vecs_num);
			geo_mesh->m_index_array.push_back( start_index + v_temp_index_array[i+1] + v_vecs_num);
			geo_mesh->m_index_array.push_back( start_index + v_temp_index_array[i+2] + v_vecs_num);
		}

		// 外侧面
		coordinates_array& exter_ring = v_vecs_cart[0];
		int v_exter_point_num = exter_ring.size();
		// CW
		if( in_triangle_start_mode == 0 )
		{
			for ( int i = 0; i < v_exter_point_num; i++ )
			{	
				geo_mesh->m_index_array.push_back( start_index + i );
				geo_mesh->m_index_array.push_back( start_index + ( i + 1 ) % v_exter_point_num + v_vecs_num );
				geo_mesh->m_index_array.push_back( start_index + i + v_vecs_num );

				geo_mesh->m_index_array.push_back( start_index + i );
				geo_mesh->m_index_array.push_back( start_index + ( i + 1 ) % v_exter_point_num );
				geo_mesh->m_index_array.push_back( start_index + ( i + 1 ) % v_exter_point_num + v_vecs_num );
			}
		}
		// CCW
		else if ( in_triangle_start_mode == 1 )
		{
			for ( int i = 0; i < (int)exter_ring.size(); i++ )
			{	
				geo_mesh->m_index_array.push_back( start_index + i );
				geo_mesh->m_index_array.push_back( start_index + i + v_vecs_num );
				geo_mesh->m_index_array.push_back( start_index + ( i + 1 ) % v_exter_point_num + v_vecs_num );

				geo_mesh->m_index_array.push_back( start_index + i );
				geo_mesh->m_index_array.push_back( start_index + ( i + 1 ) % v_exter_point_num + v_vecs_num );
				geo_mesh->m_index_array.push_back( start_index + ( i + 1 ) % v_exter_point_num );
			}
		}

		// 内侧面
		in_triangle_start_mode = 1;
		if ( b_ccw ) in_triangle_start_mode = 0;

		start_index += v_exter_point_num;

		// CW
		if( in_triangle_start_mode == 0 )
		{
			for ( int nii = 1; nii < v_vecs_cart.size(); nii++ )
			{
				coordinates_array& inter_ring = v_vecs_cart[nii];
				int v_inter_point_num = inter_ring.size();

				for ( int i = 0; i < v_inter_point_num; i++ )
				{
					geo_mesh->m_index_array.push_back( start_index + i );
					geo_mesh->m_index_array.push_back( start_index + i + v_vecs_num );
					geo_mesh->m_index_array.push_back( start_index + ( i + 1 ) % v_inter_point_num + v_vecs_num );

					geo_mesh->m_index_array.push_back( start_index + i );
					geo_mesh->m_index_array.push_back( start_index + ( i + 1 ) % v_inter_point_num + v_vecs_num );
					geo_mesh->m_index_array.push_back( start_index + ( i + 1 ) % v_inter_point_num );
				}

				start_index += v_inter_point_num;
			}
		}
		// CCW
		else if ( in_triangle_start_mode == 1 )
		{
			for ( int nii = 1; nii < v_vecs_cart.size(); nii++ )
			{
				coordinates_array& inter_ring = v_vecs_cart[nii];
				int v_inter_point_num = inter_ring.size();

				for ( int i = 0; i < (int)inter_ring.size(); i++ )
				{
					geo_mesh->m_index_array.push_back( start_index + i );
					geo_mesh->m_index_array.push_back( start_index + ( i + 1 ) % v_inter_point_num + v_vecs_num );
					geo_mesh->m_index_array.push_back( start_index + i + v_vecs_num );

					geo_mesh->m_index_array.push_back( start_index + i );
					geo_mesh->m_index_array.push_back( start_index + ( i + 1 ) % v_inter_point_num );
					geo_mesh->m_index_array.push_back( start_index + ( i + 1 ) % v_inter_point_num + v_vecs_num );
				}

				start_index += v_inter_point_num;
			}
		}

	}
	
}