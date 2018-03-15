///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_render_object_shadow_factory.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : render_object_shadow_factory class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_render_object_shadow_factory.h"

namespace uniscope_globe
{
	//////////////////////////////////////////////////////////////////////////
	// common
	//////////////////////////////////////////////////////////////////////////
	double render_object_shadow_factory::compute_scaled_length( vector3<double> pos1, vector3<double> pos2, double v_height )
	{
		vector3<double> pos_normal1 = vector3<double>::normalize( pos1 );
		vector3<double> pos_normal2 = vector3<double>::normalize( pos2 );

		double half_angle = math<double>::acos_( vector3<double>::dot( pos_normal1, pos_normal2 ) ) * 0.5;

		return ( spherical_coords::s_semi_major + v_height ) / math<double>::cos_( half_angle );
	}

	vector_3f render_object_shadow_factory::compute_normal( vector_3f* vec1, vector_3f* vec2, vector_3f* vec3 )
	{
		vector3<float> tmp1 = vector3<float>::normalize( *vec2 - *vec1 );
		vector3<float> tmp2 = vector3<float>::normalize( *vec3 - *vec1 );
		vector3<float> normal = vector3<float>::cross( tmp1, tmp2 );
		normal.normalize();

		return normal;

		//vector3<float> tmp1 = vec2 - vec1;
		//vector3<float> tmp2 = vec3 - vec1;
		//tmp1.normalize();
		//tmp2.normalize();
		//vector3<float> normal = vector3<float>::cross( tmp1, tmp2 );
		//normal.normalize();

		//return normal;
	}

	void render_object_shadow_factory::generate_polygon_mesh( vector_3d* in_pos_array, ulong v_size, double v_height,
														 std::vector<vector_3f> &out_points,
														 std::vector<usx_tri_face_index>& out_index, axis_aligned_box_d& out_box )
	{
		// 三角化
		int max_ni = 0;
		double max_z = -math_d::s_max_real;
		std::vector<vector_2d> tri_array;
		for ( int ni = 0; ni < (int)v_size; ni ++ )
		{
			tri_array.push_back( vector_2d( in_pos_array[ni].x, in_pos_array[ni].z ) );
			if ( in_pos_array[ni].z > max_z )
			{
				max_ni = ni;
				max_z = in_pos_array[ni].z;
			}
		}

		int in_triangle_start_mode = 0;
		vector_2d prev_pos, next_pos, curr_pos;
		curr_pos = tri_array[max_ni];
		if (max_ni == 0 || max_ni == v_size - 1)
		{
			prev_pos = tri_array[v_size - 2];
			next_pos = tri_array[1];
		}
		else
		{
			prev_pos = tri_array[max_ni - 1];
			next_pos = tri_array[max_ni + 1];
		}
		prev_pos = prev_pos - curr_pos;
		prev_pos.normalize();
		prev_pos = prev_pos + curr_pos;
		next_pos = next_pos - curr_pos;
		next_pos.normalize();
		next_pos = next_pos + curr_pos;
		
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
		std::vector<ushort> v_temp_index_array;
		v_temp_index_array.resize(( v_size - 2 ) * 3);
		v_triangulation_algorithm.triangulation_polygon( &tri_array[0], v_size, &v_temp_index_array[0] );

		//
		// 组织整个多面体
		//
		// 上表面
		for ( int i = 0; i < v_temp_index_array.size(); i += 3 )
		{
			if ( in_triangle_start_mode == 0 )
			{
				// CW
				out_index.push_back( usx_tri_face_index( v_temp_index_array[i], v_temp_index_array[i+1], v_temp_index_array[i+2] ) );
			}
			else if ( in_triangle_start_mode == 1 )
			{
				// CCW
				out_index.push_back( usx_tri_face_index( v_temp_index_array[i], v_temp_index_array[i+2], v_temp_index_array[i+1] ) );
			}			
		}

		// 下表面
		for ( int i = 0; i < v_temp_index_array.size(); i += 3 )
		{
			if ( in_triangle_start_mode == 0 )
			{
				// CW
				out_index.push_back( usx_tri_face_index( v_temp_index_array[i] + v_size, v_temp_index_array[i+2] + v_size, v_temp_index_array[i+1] + v_size ) );
			}
			else if( in_triangle_start_mode == 1 )
			{
				// CCW
				out_index.push_back( usx_tri_face_index( v_temp_index_array[i] + v_size, v_temp_index_array[i+1] + v_size, v_temp_index_array[i+2] + v_size ) );
			}
		}

		// 侧面
		for ( int i = 0; i < (int)v_size; i++ )
		{
			if ( in_triangle_start_mode == 0 )
			{
				// CW
				out_index.push_back( usx_tri_face_index( i, i + v_size, ( i + 1 ) % v_size + v_size ) );
				out_index.push_back( usx_tri_face_index( i, ( i + 1 ) % v_size + v_size, ( i + 1 ) % v_size ) );
			}
			else if( in_triangle_start_mode == 1 )
			{
				// CCW	
				out_index.push_back( usx_tri_face_index( i, ( i + 1 ) % v_size + v_size, i + v_size ) );
				out_index.push_back( usx_tri_face_index( i, ( i + 1 ) % v_size, ( i + 1 ) % v_size + v_size ) );
			}
		}

		// 生成顶点集合
		out_points.resize( v_size * 2 );
		for ( int i = 0 ; i < (int)v_size; i++ )
		{			
			out_points[i].x = in_pos_array[i].x;
			out_points[i].y = in_pos_array[i].y;
			out_points[i].z = in_pos_array[i].z;

			out_box.combine( in_pos_array[i] );
		}

		vector_3d vec;
		int v_crsr = 0;
		for ( int i = 0 ; i < (int)v_size; i++ )
		{
			v_crsr = i + v_size;
			vec = in_pos_array[i];
			vec.y += v_height;

			out_points[v_crsr].x = vec.x;
			out_points[v_crsr].y = vec.y;
			out_points[v_crsr].z = vec.z;
			
			out_box.combine( vec );
		}
	}

	void render_object_shadow_factory::insert_triangle( ulong o1, ulong o2, ulong n1, ulong n2, 
												   edge_map& in_edge_map, std::vector<usx_tri_face_index>& in_index_array )
	{
		edge_map::iterator itr = in_edge_map.find( make_pair( o2, o1 ) );		
		if( itr != in_edge_map.end() )
		{
			degenerate_quad& quad = itr->second;

			usx_tri_face_index v_new_face;

			v_new_face.a = n1;
			v_new_face.b = quad.m_v2;
			v_new_face.c = quad.m_v1;
			in_index_array.push_back( v_new_face );
			
			v_new_face.a = n1;
			v_new_face.b = quad.m_v1;
			v_new_face.c = n2;
			in_index_array.push_back( v_new_face );
			
			in_edge_map.erase( itr );
		}
		else
		{
			degenerate_quad quad;
			quad.m_v1 = n1;
			quad.m_v2 = n2;
			in_edge_map.insert( make_pair( make_pair(o1,o2), quad ) );
		}
	}	

	void render_object_shadow_factory::generate_shadow_mesh( vector_3f* v_vertex, int vertex_num,
														usx_tri_face_index* v_indices, int face_num,
														std::vector<position_normal_color>& new_vertex,
														std::vector<usx_tri_face_index>& new_indices,
														ulong v_fill_color )
	{
		//
		// 添加退化四边形
		//
		edge_map v_edge_map;
		for ( int i = 0; i < face_num; i++ )
		{
			vector_3f vec_a = v_vertex[v_indices[i].a];
			vector_3f vec_b = v_vertex[v_indices[i].b];
			vector_3f vec_c = v_vertex[v_indices[i].c];

			// 计算法矢
			vector3<float> v_normal = compute_normal( &vec_a, &vec_b, &vec_c );

			// 增加顶点
			position_normal_color v1,v2,v3;
			v1.x = vec_a.x; v1.y = vec_a.y; v1.z = vec_a.z;
			v1.nx = v_normal.x; v1.ny = v_normal.y; v1.nz = v_normal.z;
			v1.color = v_fill_color;

			v2.x = vec_b.x; v2.y = vec_b.y; v2.z = vec_b.z;
			v2.nx = v_normal.x;v2.ny = v_normal.y;v2.nz = v_normal.z;
			v2.color = v_fill_color;

			v3.x = vec_c.x; v3.y = vec_c.y; v3.z = vec_c.z;
			v3.nx = v_normal.x;v3.ny = v_normal.y;v3.nz = v_normal.z;
			v3.color = v_fill_color;

			// 增加三角形
			usx_tri_face_index v_new_face;
			v_new_face.a = (int)new_vertex.size();
			v_new_face.b = v_new_face.a + 1;
			v_new_face.c = v_new_face.b + 1;
			new_indices.push_back( v_new_face );
			
			new_vertex.push_back( v1 );
			new_vertex.push_back( v2 );
			new_vertex.push_back( v3 );

			insert_triangle( v_indices[i].a, v_indices[i].b, v_new_face.a, v_new_face.b, v_edge_map, new_indices );
			insert_triangle( v_indices[i].b, v_indices[i].c, v_new_face.b, v_new_face.c, v_edge_map, new_indices );
			insert_triangle( v_indices[i].c, v_indices[i].a, v_new_face.c, v_new_face.a, v_edge_map, new_indices );
		}
	}

	///////////////////////////////////////////////////////////////////////////
	// render_object_shadow_mesh
	//////////////////////////////////////////////////////////////////////////
	void render_object_shadow_factory::collapse_geometry_mesh( render_object_shadow_mesh* v_mesh, matrix_4f v_mat )
	{
		vector_3f vec_tmp;

		for ( int i = 0; i < (int)v_mesh->m_frame_vertex_array.size(); i ++ )
		{
			vec_tmp.x = v_mesh->m_frame_vertex_array[i].x;
			vec_tmp.y = v_mesh->m_frame_vertex_array[i].y;
			vec_tmp.z = v_mesh->m_frame_vertex_array[i].z;
			vec_tmp = vec_tmp * v_mat;

			v_mesh->m_frame_vertex_array[i].x = vec_tmp.x;
			v_mesh->m_frame_vertex_array[i].y = vec_tmp.y;
			v_mesh->m_frame_vertex_array[i].z = vec_tmp.z;
		}

		for ( int i = 0; i < (int)v_mesh->m_vertex_array.size(); i ++ )
		{
			vec_tmp.x = v_mesh->m_vertex_array[i].x;
			vec_tmp.y = v_mesh->m_vertex_array[i].y;
			vec_tmp.z = v_mesh->m_vertex_array[i].z;
			vec_tmp = vec_tmp * v_mat;

			v_mesh->m_vertex_array[i].x = vec_tmp.x;
			v_mesh->m_vertex_array[i].y = vec_tmp.y;
			v_mesh->m_vertex_array[i].z = vec_tmp.z;
		}
	}

	matrix_4d render_object_shadow_factory::collapse_geometry_mesh( render_object_shadow_mesh* v_mesh, vector_3d& vec_cen )
	{
		for ( int i = 0; i < (int)v_mesh->m_frame_vertex_array.size(); i ++ )
		{
			v_mesh->m_frame_vertex_array[i].x -= vec_cen.x;
			v_mesh->m_frame_vertex_array[i].y -= vec_cen.y;
			v_mesh->m_frame_vertex_array[i].z -= vec_cen.z;
		}

		for ( int i = 0; i < (int)v_mesh->m_vertex_array.size(); i ++ )
		{
			v_mesh->m_vertex_array[i].x -= vec_cen.x;
			v_mesh->m_vertex_array[i].y -= vec_cen.y;
			v_mesh->m_vertex_array[i].z -= vec_cen.z;
		}

		return matrix_4d::translation( vec_cen.x, vec_cen.y, vec_cen.z );
	}

	void render_object_shadow_factory::create_mesh_from_lon_lat_box( render_object_shadow_mesh* geo_mesh, lon_lat_box& v_box, ulong line_color, ulong fill_color )
	{
		geo_mesh->clear();

		geo_mesh->m_vertex_array.resize( 8 );
		geo_mesh->m_index_array.resize( 36 );

		geo_mesh->m_render_aabb = axis_aligned_box<double>( v_box.corner_value, 8 );
		geo_mesh->m_center = geo_mesh->m_render_aabb.center();

		std::vector<vector_3f> vertex_array;
		vertex_array.resize(8);
		for ( int i = 0; i < 8; i++ )
		{
			vertex_array[i].x = v_box.corner_value[i].x - geo_mesh->m_center.x;
			vertex_array[i].y = v_box.corner_value[i].y - geo_mesh->m_center.y;
			vertex_array[i].z = v_box.corner_value[i].z - geo_mesh->m_center.z;
		}

		ulong index_array[36] = 
		{	
			0,1,3,
			1,2,3,
			3,2,7,
			2,4,7,
			7,4,6,
			4,5,6,
			6,5,0,
			5,1,0,
			1,5,2,
			5,4,2,
			0,3,6,
			3,7,6
		};

		std::vector<usx_tri_face_index> tri_face_array;
		generate_shadow_mesh( &vertex_array[0], vertex_array.size(), (usx_tri_face_index*)index_array, 12, 
			geo_mesh->m_vertex_array, tri_face_array, fill_color );

		geo_mesh->m_index_array.resize( tri_face_array.size() * 3 );
		memcpy( &geo_mesh->m_index_array[0], &tri_face_array[0], sizeof(ulong) * geo_mesh->m_index_array.size() );

		// 边框
		geo_mesh->m_frame_vertex_array.resize( vertex_array.size() );
		for ( int i = 0; i < (int)geo_mesh->m_frame_vertex_array.size(); i ++ )
		{
			geo_mesh->m_frame_vertex_array[i].x = vertex_array[i].x;
			geo_mesh->m_frame_vertex_array[i].y = vertex_array[i].y;
			geo_mesh->m_frame_vertex_array[i].z = vertex_array[i].z;
			geo_mesh->m_frame_vertex_array[i].color = line_color;
		}

		ushort frame_index_array[24] =
		{
			0,1, 1,2, 2,3, 3,0,
			4,5, 5,6, 6,7, 7,4,
			0,6, 3,7, 1,5, 2,4
		};
		geo_mesh->m_frame_index_array.resize( 24 );
		memcpy( &(geo_mesh->m_frame_index_array[0]), frame_index_array, sizeof(ushort) * 24 );

		matrix_4d v_collapse_mat = matrix_4d::translation( geo_mesh->m_center.x, geo_mesh->m_center.y, geo_mesh->m_center.z );
		geo_mesh->set_collapse( v_collapse_mat );
	}

	// create from box
	//////////////////////////////////////////////////////////////////////////
	/*
	   5-----4
	  /|    /|
	 / |   / |
	1-----2  |
	|  6--|--7
	| /   | /
	|/    |/
	0-----3
	*/
	void render_object_shadow_factory::create_mesh_from_box( render_object_shadow_mesh* geo_mesh, double x_len, double y_len, double z_len, ulong line_color, ulong fill_color )
	{
		geo_mesh->clear();

		std::vector<vector_3f> v_vertex_array;
		v_vertex_array.resize(8);
		
		std::vector<ulong> v_index_array;
		v_index_array.resize( 36 );

		double half_x = x_len * 0.5;
		double half_z = z_len * 0.5;
		geo_mesh->m_render_aabb.set_extent( vector_3d(-half_x,0.0,-half_z), vector_3d(half_x,y_len,half_z) );

		v_vertex_array[0].x = -half_x;
		v_vertex_array[0].y = 0.0;
		v_vertex_array[0].z = -half_z;
		
		v_vertex_array[1].x = -half_x;
		v_vertex_array[1].y = y_len;
		v_vertex_array[1].z = -half_z;
		
		v_vertex_array[2].x = half_x;
		v_vertex_array[2].y = y_len;
		v_vertex_array[2].z = -half_z;
		
		v_vertex_array[3].x = half_x;
		v_vertex_array[3].y = 0.0;
		v_vertex_array[3].z = -half_z;
		
		v_vertex_array[4].x = half_x;
		v_vertex_array[4].y = y_len;
		v_vertex_array[4].z = half_z;
		
		v_vertex_array[5].x = -half_x;
		v_vertex_array[5].y = y_len;
		v_vertex_array[5].z = half_z;
		
		v_vertex_array[6].x = -half_x;
		v_vertex_array[6].y = 0.0;
		v_vertex_array[6].z = half_z;
		
		v_vertex_array[7].x = half_x;
		v_vertex_array[7].y = 0.0;
		v_vertex_array[7].z = half_z;
		
		ulong index_array[36] = 
		{	
			0,1,3,
			1,2,3,
			3,2,7,
			2,4,7,
			7,4,6,
			4,5,6,
			6,5,0,
			5,1,0,
			1,5,2,
			5,4,2,
			0,3,6,
			3,7,6
		};
	
		std::vector<usx_tri_face_index> tri_face_array;
		generate_shadow_mesh( &v_vertex_array[0], v_vertex_array.size(), (usx_tri_face_index*)index_array, 12, 
							  geo_mesh->m_vertex_array, tri_face_array, fill_color );

		geo_mesh->m_index_array.resize( tri_face_array.size() * 3 );
		memcpy( &(geo_mesh->m_index_array[0]), &tri_face_array[0], sizeof(ulong) * geo_mesh->m_index_array.size() );

		// 边框
		geo_mesh->m_frame_vertex_array.resize( v_vertex_array.size() );
		for ( int i = 0; i < (int)geo_mesh->m_frame_vertex_array.size(); i ++ )
		{
			geo_mesh->m_frame_vertex_array[i].x = v_vertex_array[i].x;
			geo_mesh->m_frame_vertex_array[i].y = v_vertex_array[i].y;
			geo_mesh->m_frame_vertex_array[i].z = v_vertex_array[i].z;
			geo_mesh->m_frame_vertex_array[i].color = line_color;
		}

		
		ushort frame_index_array[24] =
		{
			0,1, 1,2, 2,3, 3,0,
			4,5, 5,6, 6,7, 7,4,
			0,6, 3,7, 1,5, 2,4
		};
		geo_mesh->m_frame_index_array.resize( 24 );
		memcpy( &geo_mesh->m_frame_index_array[0], frame_index_array, sizeof(ushort) * 24 );
	}

	void render_object_shadow_factory::create_mesh_from_volume( render_object_shadow_mesh* geo_mesh, vector_3d* in_pos_array, ulong v_size, double v_height, ulong line_color, ulong fill_color )
	{
		geo_mesh->clear();
		geo_mesh->m_fvf = position_normal_color::fvf;

		std::vector<vector_3f> vertex_array;
		std::vector<usx_tri_face_index> index_array;
		generate_polygon_mesh( in_pos_array, v_size, v_height, vertex_array, index_array, geo_mesh->m_render_aabb );

		// 边框
		geo_mesh->m_frame_vertex_array.resize( vertex_array.size() );
		for ( int i = 0; i < geo_mesh->m_frame_vertex_array.size(); i ++ )
		{
			geo_mesh->m_frame_vertex_array[i].x = vertex_array[i].x;
			geo_mesh->m_frame_vertex_array[i].y = vertex_array[i].y;
			geo_mesh->m_frame_vertex_array[i].z = vertex_array[i].z;
			geo_mesh->m_frame_vertex_array[i].color = line_color;
		}

		ulong v_line_num = v_size * 3;
		geo_mesh->m_frame_index_array.resize( v_line_num * 2 );
		ushort* p_index = &geo_mesh->m_frame_index_array[0];

		ulong v_off = 0;
		for ( int i = 0; i < int(v_size-1); i++ )
		{
			v_off = i<<1;
			p_index[v_off]   = i;
			p_index[v_off+1] = i+1;

			v_off = (i+v_size)<<1;
			p_index[v_off]	 = v_size + i;
			p_index[v_off+1] = v_size + i + 1;

			v_off = (i+2*v_size)<<1;
			p_index[v_off]	 = i;
			p_index[v_off+1] = v_size + i;
		}
		v_off = (v_size -1)<<1;
		p_index[v_off]   = v_size - 1;
		p_index[v_off+1] = 0;

		v_off = (2*v_size-1)<<1;
		p_index[v_off]   = v_size*2 - 1;
		p_index[v_off+1] = v_size;

		v_off = (3*v_size-1)<<1;
		p_index[v_off]   = v_size*2 - 1;
		p_index[v_off+1] = v_size - 1;
		
		std::vector<usx_tri_face_index> tri_face_array;
		generate_shadow_mesh( &vertex_array[0], vertex_array.size(), &index_array[0], index_array.size(), 
							  geo_mesh->m_vertex_array, tri_face_array, fill_color );
		
		geo_mesh->m_index_array.resize( tri_face_array.size() * 3 );
		memcpy( &(geo_mesh->m_index_array[0]), &tri_face_array[0], sizeof(ulong) * geo_mesh->m_index_array.size() );
	}

	void render_object_shadow_factory::create_mesh_from_cone( render_object_shadow_mesh* geo_mesh, ulong in_sides, double in_radius, double in_height,  ulong line_color, ulong fill_color )
	{
		vector_3d v_point_center(0.0, 0.0, 0.0);
		vector_3d v_point_top(0.0,in_height,0.0);

		// 构建顶点
		vector_3d* v_circle_points = new vector_3d[in_sides];
		shape_creator::create_circle_on_plane( v_circle_points, v_point_center, in_radius, in_sides );

		geo_mesh->m_render_aabb.combine( v_point_top );
		for ( int i = 0; i < (int)in_sides; i ++ )
		{		
			geo_mesh->m_render_aabb.combine( v_circle_points[i] );
		}
		
		std::vector<vector_3f> vertex_array;
		vertex_array.resize( in_sides + 2 );
		for ( int i = 0; i < (int)in_sides; i ++ )
		{
			vertex_array[i].x = v_circle_points[i].x;
			vertex_array[i].y = v_circle_points[i].y;
			vertex_array[i].z = v_circle_points[i].z;
		}

		vertex_array[in_sides].x = v_point_center.x;
		vertex_array[in_sides].y = v_point_center.y;
		vertex_array[in_sides].z = v_point_center.z;
		
		vertex_array[in_sides+1].x = v_point_top.x;
		vertex_array[in_sides+1].y = v_point_top.y;
		vertex_array[in_sides+1].z = v_point_top.z;

		// 构建底面，同时构建侧面
		int in_triangle_start_mode = 0;
		// 0: 右手，CCW. 1: 左手，CW
		//
		//                    |x1     x2     x3|   
		//	S(P1,P2,P3)   =   |y1     y2     y3|   =   (x1-x3)*(y2-y3)   -   (y1-y3)(x2-x3)   
		//	                  |1      1      1 |   
		//
		double v_area = (v_point_center.x - v_circle_points[1].x) * (v_circle_points[0].y - v_circle_points[1].y) - (v_point_center.y - v_circle_points[1].y) * (v_circle_points[0].x - v_circle_points[1].x);
		if( v_area > 0 )
		{
			in_triangle_start_mode = 0;
		}
		else
		{
			in_triangle_start_mode = 1;
		}
		
		std::vector<usx_tri_face_index> index_array;
		for ( int i = 0; i < (int)in_sides; i ++ )
		{
			if ( in_triangle_start_mode == 0 ) // CCW，右手
			{
				// 底面
				index_array.push_back( usx_tri_face_index( in_sides, (i==(in_sides-1))?0:(i+1), i ) );

				// 锥面
				index_array.push_back( usx_tri_face_index( in_sides+1, i, (i==(in_sides-1))?0:(i+1) ) );
			}
			else // CW，左手
			{
				// 底面
				index_array.push_back( usx_tri_face_index( in_sides, i, (i==(in_sides-1))?0:(i+1) ) );

				// 锥面
				index_array.push_back( usx_tri_face_index( in_sides+1, (i==(in_sides-1))?0:(i+1), i ) );
			}
		}
		
		// 构建shadow
		std::vector<usx_tri_face_index> tri_face_array;
		generate_shadow_mesh( &vertex_array[0], vertex_array.size(), &index_array[0], index_array.size(), 
			geo_mesh->m_vertex_array, tri_face_array, fill_color );

		geo_mesh->m_index_array.resize( tri_face_array.size() * 3 );
		memcpy( &(geo_mesh->m_index_array[0]), &tri_face_array[0], sizeof(ulong) * geo_mesh->m_index_array.size() );

		// 构建边框
		geo_mesh->m_frame_vertex_array.resize( vertex_array.size() );
		for ( int i = 0; i < (int)vertex_array.size(); i ++ )
		{
			geo_mesh->m_frame_vertex_array[i].x = vertex_array[i].x;
			geo_mesh->m_frame_vertex_array[i].y = vertex_array[i].y;
			geo_mesh->m_frame_vertex_array[i].z = vertex_array[i].z;
			geo_mesh->m_frame_vertex_array[i].color = line_color;
		}

		ulong v_line_num = in_sides * 2;
		geo_mesh->m_frame_index_array.resize( v_line_num * 2 );
		ushort* p_index = &(geo_mesh->m_frame_index_array[0]);

		ulong v_off = 0;
		for ( int i = 0; i < int(in_sides-1); i++ )
		{
			v_off = i<<1;
			p_index[v_off]   = i;
			p_index[v_off+1] = i+1;

			v_off = (i+in_sides)<<1;
			p_index[v_off]	 = in_sides+1;
			p_index[v_off+1] = i;
		}
		v_off = (in_sides-1)<<1;
		p_index[in_sides-1] = in_sides - 1;
		p_index[in_sides]   = 0;

		v_off = (2*in_sides-1)<<1;
		p_index[v_off]   = in_sides+1;
		p_index[v_off+1] = in_sides - 1;
	}

	void render_object_shadow_factory::create_mesh_from_cylinder( render_object_shadow_mesh* geo_mesh, ulong in_sides, double in_radius, double in_height, ulong line_color, ulong fill_color )
	{
		// create cylinder
		vector_3d vec_cen( 0.0, 0.0, 0.0 );
		vector_3d* v_circle = new vector_3d[in_sides+1];
		shape_creator::create_circle_on_plane( v_circle, vec_cen, in_radius, in_sides );
		v_circle[in_sides] = v_circle[0];

		create_mesh_from_volume( geo_mesh,  v_circle, in_sides, in_height, line_color, fill_color );

		AUTO_DELETE_ARRAY( v_circle );
	}

	#define NORMALIZE_ANGLE( val ) { if(val > PI * 2) val -= PI *2; if(val < -PI * 2) val += PI *2; }
	void render_object_shadow_factory::create_mesh_from_sphere( render_object_shadow_mesh* geo_mesh, double in_radius,  ulong line_color, ulong fill_color )
	{
		int	v_segment = (1<<4);// + 1;
		double v_lon_scale_factor = 1.0 / v_segment;
		double v_lat_scale_factor = 1.0 / v_segment;

		double v_longitude	= - PI;
		double v_latitude	= - HALF_PI;
		double v_radius		= in_radius;
		double v_delta_longitude = v_lon_scale_factor * PI;
		double v_delta_latitude  = v_lat_scale_factor * PI;

		int v_segment_plus_1 = v_segment + 1;
		int v_double_segment_plus_1 = v_segment * 2 + 1;
		int v_double_segment = v_segment * 2;

		int v_vertex_count = v_segment_plus_1 * v_double_segment_plus_1 ;
		
		std::vector<vector_3f> vertex_array;
		vertex_array.resize( v_vertex_count );

		vector_3f vec_cen( 0.0, v_radius, 0.0 );

		vector_3d vec_min( -v_radius,0.0,-v_radius );
		vector_3d vec_max( v_radius,2*v_radius,v_radius );
		geo_mesh->m_render_aabb.set_extent( vec_min, vec_max );

		vector_3f tmp_start;

		int v_index = 0;
		for ( int j = 0; j < v_double_segment_plus_1; j++ )
		{						
			vertex_array[v_index] = vector_3f( 0.0, 0.0, 0.0 );
			v_index += 1;
		}
		v_latitude += v_delta_latitude;
		NORMALIZE_ANGLE(v_latitude);
		for ( int i = 1; i < v_segment_plus_1-1; i++ )
		{
			double v_longitude = 0;
			double tmp_radius = math_d::cos_( v_latitude ) * v_radius;
			
			tmp_start.x = float(vec_cen.x + math_d::sin_( v_longitude ) * tmp_radius);
			tmp_start.y = float(vec_cen.y + math_d::sin_( v_latitude ) * v_radius);
			tmp_start.z = float(vec_cen.z + math_d::cos_( v_longitude ) * tmp_radius);
			for ( int j = 0; j < v_double_segment_plus_1-1; j++ )
			{
				vertex_array[v_index].x = float(vec_cen.x + math_d::sin_( v_longitude ) * tmp_radius);
				vertex_array[v_index].y = float(vec_cen.y + math_d::sin_( v_latitude ) * v_radius);
				vertex_array[v_index].z = float(vec_cen.z + math_d::cos_( v_longitude ) * tmp_radius);

				v_index += 1;
				v_longitude += v_delta_longitude;
				NORMALIZE_ANGLE(v_longitude);
			}
			vertex_array[v_index].x = tmp_start.x;
			vertex_array[v_index].y = tmp_start.y;
			vertex_array[v_index].z = tmp_start.z;
			v_index += 1;

			v_latitude += v_delta_latitude;
			NORMALIZE_ANGLE(v_latitude);
		}
		for ( int j = 0; j < v_double_segment_plus_1; j++ )
		{						
			vertex_array[v_index] = vector_3f( 0.0, 2*v_radius, 0.0 );
			v_index += 1;
		}

		std::vector<usx_tri_face_index> index_array;
		for ( int ni = 0; ni < v_segment; ni++ )
		{
			for (int j = 0; j < v_double_segment_plus_1; j++)
			{
				index_array.push_back( usx_tri_face_index( (ni*v_double_segment_plus_1+j), (ni*v_double_segment_plus_1+j+1), ((ni+1)*v_double_segment_plus_1+j) ) );
				index_array.push_back( usx_tri_face_index( (ni*v_double_segment_plus_1+j+1), ((ni+1)*v_double_segment_plus_1+j+1), ((ni+1)*v_double_segment_plus_1+j) ) );
			}
		}
		
		// 构建shadow
		std::vector<usx_tri_face_index> tri_face_array;
		generate_shadow_mesh( &vertex_array[0], vertex_array.size(), &index_array[0], index_array.size(), 
			geo_mesh->m_vertex_array, tri_face_array, fill_color );

		geo_mesh->m_index_array.resize( tri_face_array.size() * 3 );
		memcpy( &(geo_mesh->m_index_array[0]), &tri_face_array[0], sizeof(ulong) * geo_mesh->m_index_array.size() );

		// 边框
		geo_mesh->m_frame_vertex_array.resize( vertex_array.size() );
		geo_mesh->m_frame_index_array.resize( index_array.size() * 3 );

		for ( int i = 0; i < (int)geo_mesh->m_frame_vertex_array.size(); i ++ )
		{
			geo_mesh->m_frame_vertex_array[i].x = vertex_array[i].x;
			geo_mesh->m_frame_vertex_array[i].y = vertex_array[i].y;
			geo_mesh->m_frame_vertex_array[i].z = vertex_array[i].z;
			geo_mesh->m_frame_vertex_array[i].color = line_color;
		}

		v_latitude = -HALF_PI;
		int v_frame_index_cursor = 0;
		for ( int i = 0; i < v_double_segment_plus_1; i+= 4 )
		{
			for ( int j = 0; j < v_segment; j++ )
			{
				geo_mesh->m_frame_index_array[v_frame_index_cursor+(j<<1)]   = j * v_double_segment_plus_1 + i;
				geo_mesh->m_frame_index_array[v_frame_index_cursor+(j<<1)+1] = ( j + 1 ) * v_double_segment_plus_1 + i;
			}
			v_frame_index_cursor += v_segment_plus_1<<1;
		}

		for ( int i = 0; i < v_segment; i+= 4 )
		{
			for ( int j = 0; j < v_double_segment_plus_1; j++ )
			{
				geo_mesh->m_frame_index_array[v_frame_index_cursor+(j<<1)]   = i * v_double_segment_plus_1 + j;
				geo_mesh->m_frame_index_array[v_frame_index_cursor+(j<<1)+1] = i * v_double_segment_plus_1 + j + 1;
			}
			v_frame_index_cursor += v_double_segment_plus_1<<1;
		}

		//vector_3d trans( 0.0, in_radius, 0.0 );
		//matrix_4d v_collapse_mat = matrix4<double>::translation( trans.x, trans.y, trans.z );
		//geo_mesh->set_collapse( v_collapse_mat );

	}
	//{
	//	int	v_segment = (1<<4);// + 1;
	//	double v_lon_scale_factor = 1.0 / v_segment;
	//	double v_lat_scale_factor = 1.0 / v_segment;

	//	double v_longitude	= - PI;
	//	double v_latitude	= - HALF_PI;
	//	double v_radius		= in_radius;
	//	double v_delta_longitude = v_lon_scale_factor * PI;
	//	double v_delta_latitude  = v_lat_scale_factor * PI;

	//	int v_segment_plus_1 = v_segment + 1;
	//	int v_double_segment_plus_1 = v_segment * 2 + 1;
	//	int v_double_segment = v_segment * 2;

	//	int v_vertex_count = v_segment_plus_1 * v_double_segment_plus_1 ;
	//	int v_index_count =  v_segment * v_double_segment * 2 * 3;

	//	std::vector<vector_3f> vertex_array;
	//	vertex_array.resize( v_vertex_count );
	//	
	//	vector_3f vec_cen( 0.0, 0.0, 0.0 );

	//	vector_3d vec_min( -v_radius,0.0,-v_radius );
	//	vector_3d vec_max( v_radius,2*v_radius,v_radius );
	//	geo_mesh->m_render_aabb.set_extent( vec_min, vec_max );

	//	int v_index = 0;
	//	for ( int i = 0; i < v_segment_plus_1; i++ )
	//	{
	//		double v_longitude = 0;
	//		double tmp_radius = math_d::cos_( v_latitude ) * v_radius;
	//		for ( int j = 0; j < v_double_segment_plus_1; j++ )
	//		{						
	//			vertex_array[v_index].x = float(vec_cen.x + math_d::sin_( v_longitude ) * tmp_radius);
	//			vertex_array[v_index].y = float(vec_cen.y + math_d::sin_( v_latitude ) * v_radius);
	//			vertex_array[v_index].z = float(vec_cen.z + math_d::cos_( v_longitude ) * tmp_radius);

	//			v_index += 1;
	//			v_longitude += v_delta_longitude;
	//			NORMALIZE_ANGLE(v_longitude);
	//		}
	//		v_latitude += v_delta_latitude;
	//		NORMALIZE_ANGLE(v_latitude);
	//	}

	//	std::vector<usx_tri_face_index> index_array;
	//	for ( int i = 0; i < v_segment; i++ )
	//	{
	//		for (int j = 0; j < v_double_segment; j++)
	//		{
	//			index_array.push_back( usx_tri_face_index( (i*v_double_segment_plus_1+j), (i*v_double_segment_plus_1+j+1), ((i+1)*v_double_segment_plus_1+j) ) );
	//			index_array.push_back( usx_tri_face_index( (i*v_double_segment_plus_1+j+1), ((i+1)*v_double_segment_plus_1+j+1), ((i+1)*v_double_segment_plus_1+j) ) );
	//		}
	//	}

	//	// 构建shadow
	//	std::vector<usx_tri_face_index> tri_face_array;
	//	generate_shadow_mesh( vertex_array[0], vertex_array.size(), index_array[0], index_array.size(), 
	//		geo_mesh->m_vertex_array, tri_face_array, fill_color );

	//	geo_mesh->m_index_array.resize( tri_face_array.size() * 3 );
	//	memcpy( geo_mesh->m_index_array[0], tri_face_array[0], sizeof(ulong) * geo_mesh->m_index_array.size() );

	//	// 边框
	//	geo_mesh->m_frame_vertex_array.resize( vertex_array.size() );
	//	geo_mesh->m_frame_index_array.resize( index_array.size() * 3 );

	//	for ( ulong i = 0; i < geo_mesh->m_frame_vertex_array.size(); i ++ )
	//	{
	//		geo_mesh->m_frame_vertex_array[i].x = vertex_array[i].x;
	//		geo_mesh->m_frame_vertex_array[i].y = vertex_array[i].y;
	//		geo_mesh->m_frame_vertex_array[i].z = vertex_array[i].z;
	//		geo_mesh->m_frame_vertex_array[i].color = line_color;
	//	}

	//	v_latitude = -HALF_PI;
	//	int v_frame_index_cursor = 0;
	//	for ( int i = 0; i < v_double_segment_plus_1; i+= 4 )
	//	{
	//		for ( int j = 0; j < v_segment; j++ )
	//		{
	//			geo_mesh->m_frame_index_array[v_frame_index_cursor+(j<<1)]   = j * v_double_segment_plus_1 + i;
	//			geo_mesh->m_frame_index_array[v_frame_index_cursor+(j<<1)+1] = ( j + 1 ) * v_double_segment_plus_1 + i;
	//		}
	//		v_frame_index_cursor += v_segment_plus_1<<1;
	//	}

	//	for ( int i = 0; i < v_segment; i+= 4 )
	//	{
	//		for ( int j = 0; j < v_double_segment_plus_1; j++ )
	//		{
	//			geo_mesh->m_frame_index_array[v_frame_index_cursor+(j<<1)]   = i * v_double_segment_plus_1 + j;
	//			geo_mesh->m_frame_index_array[v_frame_index_cursor+(j<<1)+1] = i * v_double_segment_plus_1 + j + 1;
	//		}
	//		v_frame_index_cursor += v_double_segment_plus_1<<1;
	//	}

	//	vector_3d trans( 0.0, in_radius, 0.0 );
	//	matrix_4d v_collapse_mat = matrix4<double>::translation( trans.x, trans.y, trans.z );
	//	geo_mesh->set_collapse( v_collapse_mat );

	//}


	void render_object_shadow_factory::reset_mesh_color( render_object_shadow_mesh* geo_mesh,  ulong v_color, ulong v_line_color, ulong v_fill_color )
	{
		geo_mesh->set_shadow_color( v_color );

		for ( int ni = 0; ni < (int)geo_mesh->m_vertex_array.size(); ni++ )
		{
			geo_mesh->m_vertex_array[ni].color = v_fill_color;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 点光源生成阴影体
	//////////////////////////////////////////////////////////////////////////

	void render_object_shadow_factory::create_point_light_shadow_mesh( render_object_geometry_mesh* geo_mesh, sphere<double>& dome, d3d9_hardware_mesh_buffer* v_buf )
	{
		if ( v_buf->m_num_of_vertices > 32767 ) return;

		short* v_vis = new short[v_buf->m_num_of_triangles];
		memset( v_vis, 0, sizeof(short) * v_buf->m_num_of_triangles );

		// 产生顶点
		generate_shadow_mesh_vertex( geo_mesh, dome, v_buf );

		// 产生面索引
		generate_shadow_mesh_index( geo_mesh, dome, v_buf, v_vis );

		AUTO_DELETE( v_vis );
	}
	
	// 标记面的可见性
	void render_object_shadow_factory::generate_shadow_mesh_index( render_object_geometry_mesh* geo_mesh, sphere<double>& dome,
																	d3d9_hardware_mesh_buffer* v_buf, short* in_indice_vis )
	{
		face_normal_array& in_normals = v_buf->get_face_normal();
		long in_normal_size = in_normals.size();

		// 初始化 face_visible_array
		std::vector<int> face_visible_array;
		face_visible_array.resize( in_normal_size );

		for ( long i = 0; i < in_normal_size; i++ )
		{
			double v_val =  in_normals[i].a * dome.sphere_center.x +
							in_normals[i].b * dome.sphere_center.y +
							in_normals[i].c * dome.sphere_center.z +
							in_normals[i].d ;
			if( v_val > -0.1  )
			{
				face_visible_array[i] = in_indice_vis[i];				
			}
			else
			{
				face_visible_array[i] = 0x01;
			}
		}

		edge_array& v_edges = v_buf->get_edge();
		long in_edge_size = v_edges.size();

		geo_mesh->m_index_array.resize( ( in_edge_size + in_normal_size ) * 6 );

		// check face visible & create_cap_triangles
		ulong v_cursor = 0;
		if ( v_buf->m_32bit_index )
		{
			ulong* index_buf = (ulong*)(v_buf->m_index_buffer);

			for ( int i = 0; i < (int)in_normal_size; i++ )
			{
				if ( face_visible_array[i] == 0x01 )
				{
					int k = i * 3;
					int a = index_buf[k];
					int b = index_buf[k+1];
					int c = index_buf[k+2];

					geo_mesh->m_index_array[v_cursor++] = a;
					geo_mesh->m_index_array[v_cursor++] = b;
					geo_mesh->m_index_array[v_cursor++] = c;

					geo_mesh->m_index_array[v_cursor++] = a + v_buf->m_num_of_vertices;
					geo_mesh->m_index_array[v_cursor++] = c + v_buf->m_num_of_vertices;
					geo_mesh->m_index_array[v_cursor++] = b + v_buf->m_num_of_vertices;
				}
			}

			// create_silhouette_triangles
			for ( int i = 0; i < in_edge_size; i ++ )
			{

				byte f1 = ( v_edges[i].left_poly == -1 ) ? 0x00 : face_visible_array[ v_edges[i].left_poly ];
				byte f2 = ( v_edges[i].right_poly == -1 ) ? 0x00 : face_visible_array[ v_edges[i].right_poly ];

				if( f1 == f2 )
					continue;

				int i1 = v_edges[i].start_point;
				int i2 = v_edges[i].end_point;

				if( f1 == 0x01 )
				{
					geo_mesh->m_index_array[v_cursor++] = i1;
					geo_mesh->m_index_array[v_cursor++] = i2;
					geo_mesh->m_index_array[v_cursor++] = i2 + v_buf->m_num_of_vertices;

					geo_mesh->m_index_array[v_cursor++] = i1;
					geo_mesh->m_index_array[v_cursor++] = i2 + v_buf->m_num_of_vertices;
					geo_mesh->m_index_array[v_cursor++] = i1 + v_buf->m_num_of_vertices;
				}
				else
				{
					geo_mesh->m_index_array[v_cursor++] = i1;
					geo_mesh->m_index_array[v_cursor++] = i1 + v_buf->m_num_of_vertices;
					geo_mesh->m_index_array[v_cursor++] = i2 + v_buf->m_num_of_vertices;

					geo_mesh->m_index_array[v_cursor++] = i1;
					geo_mesh->m_index_array[v_cursor++] = i2 + v_buf->m_num_of_vertices;
					geo_mesh->m_index_array[v_cursor++] = i2;
				}

			}
		}
		else
		{
			ushort* index_buf = (ushort*)(v_buf->m_index_buffer);

			for ( int i = 0; i < (int)in_normal_size; i++ )
			{
				if ( face_visible_array[i] == 0x01 )
				{
					int k = i * 3;
					int a = index_buf[k];
					int b = index_buf[k+1];
					int c = index_buf[k+2];

					geo_mesh->m_index_array[v_cursor++] = a;
					geo_mesh->m_index_array[v_cursor++] = b;
					geo_mesh->m_index_array[v_cursor++] = c;

					geo_mesh->m_index_array[v_cursor++] = a + v_buf->m_num_of_vertices;
					geo_mesh->m_index_array[v_cursor++] = c + v_buf->m_num_of_vertices;
					geo_mesh->m_index_array[v_cursor++] = b + v_buf->m_num_of_vertices;
				}
			}

			// create_silhouette_triangles
			for ( int i = 0; i < in_edge_size; i ++ )
			{

				byte f1 = ( v_edges[i].left_poly == -1 ) ? 0x00 : face_visible_array[ v_edges[i].left_poly ];
				byte f2 = ( v_edges[i].right_poly == -1 ) ? 0x00 : face_visible_array[ v_edges[i].right_poly ];

				if( f1 == f2 )
					continue;
				
				int i1 = v_edges[i].start_point;
				int i2 = v_edges[i].end_point;

				if( f1 == 0x01 )
				{
					geo_mesh->m_index_array[v_cursor++] = i1;
					geo_mesh->m_index_array[v_cursor++] = i2;
					geo_mesh->m_index_array[v_cursor++] = i2 + v_buf->m_num_of_vertices;

					geo_mesh->m_index_array[v_cursor++] = i1;
					geo_mesh->m_index_array[v_cursor++] = i2 + v_buf->m_num_of_vertices;
					geo_mesh->m_index_array[v_cursor++] = i1 + v_buf->m_num_of_vertices;
				}
				else
				{
					geo_mesh->m_index_array[v_cursor++] = i1;
					geo_mesh->m_index_array[v_cursor++] = i1 + v_buf->m_num_of_vertices;
					geo_mesh->m_index_array[v_cursor++] = i2 + v_buf->m_num_of_vertices;

					geo_mesh->m_index_array[v_cursor++] = i1;
					geo_mesh->m_index_array[v_cursor++] = i2 + v_buf->m_num_of_vertices;
					geo_mesh->m_index_array[v_cursor++] = i2;
				}
				
			}
		}

		geo_mesh->m_index_array.resize(v_cursor);
	}

	void render_object_shadow_factory::generate_shadow_mesh_vertex( render_object_geometry_mesh* geo_mesh, sphere<double>& dome, d3d9_hardware_mesh_buffer* v_buf )
	{
		geo_mesh->m_vertex_array.resize( v_buf->m_num_of_vertices * 2 );
		geo_mesh->m_render_aabb.make_invalid();

		vector3<double> light_pos;
		light_pos.x = dome.sphere_center.x;
		light_pos.y = dome.sphere_center.y;
		light_pos.z = dome.sphere_center.z;

		plane<double> v_plane( vector_3d( 0, 0, -1 ), vector_3d( 0.0, 0.0, dome.sphere_radius ) );

		for ( int ni = 0; ni < (int)v_buf->m_num_of_vertices; ni++ )
		{
			vector3<double> un_extruded_point( (float*)(v_buf->m_vertex_buffer) + ni * v_buf->m_vertex_stride / 4 );

			geo_mesh->m_render_aabb.combine( un_extruded_point );

			geo_mesh->m_vertex_array[ni+v_buf->m_num_of_vertices].x = un_extruded_point.x;
			geo_mesh->m_vertex_array[ni+v_buf->m_num_of_vertices].y = un_extruded_point.y;
			geo_mesh->m_vertex_array[ni+v_buf->m_num_of_vertices].z = un_extruded_point.z;
			geo_mesh->m_vertex_array[ni+v_buf->m_num_of_vertices].color = 0xff00ff00;

			vector3<double> temp_vec( 0.0, 0.0, 0.0 );
			ray<double> v_ray( light_pos, vector_3d::normalize(un_extruded_point - light_pos) );
			
			if ( !v_plane.intersect( v_ray, temp_vec ) )
			{
				temp_vec = un_extruded_point;
			}

			if ( ( temp_vec - un_extruded_point ).length() < 0.01 )
			{
				temp_vec = temp_vec + v_ray.ray_direction * 0.1;
			}
			//vector3<double> temp_vec = un_extruded_point - light_pos;
			//vector3<double> temp_vec_xz( temp_vec.x, 0.0, temp_vec.z );
			//double v_len = temp_vec_xz.dot( vector_3d::s_unit_z );
			//if ( v_len <= 0 ) v_len = 1.0;

			//v_len = ( dome.sphere_radius / v_len ) * temp_vec.length();
			//temp_vec.normalize();

			//temp_vec = light_pos + temp_vec * v_len;
			geo_mesh->m_render_aabb.combine( temp_vec );

			geo_mesh->m_vertex_array[ni].x = temp_vec.x;
			geo_mesh->m_vertex_array[ni].y = temp_vec.y;
			geo_mesh->m_vertex_array[ni].z = temp_vec.z;
			geo_mesh->m_vertex_array[ni].color = 0xff00ff00;
		}

	}

	//  点光源生成阴影体
	//////////////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////////////////
	//// 点光源生成阴影体,专门针对threat_dome

	void render_object_shadow_factory::create_point_light_shadow_mesh_td( render_object_geometry_mesh* geo_mesh, sphere<double>& dome, d3d9_hardware_mesh_buffer* v_buf )
	{
		if ( v_buf->m_num_of_vertices > 32767 ) return;

		short* v_vis = new short[v_buf->m_num_of_triangles];
		memset( v_vis, 0, sizeof(short) * v_buf->m_num_of_triangles );

		generate_face_visible_flag( v_vis, dome, v_buf );

		// 产生顶点
		generate_shadow_mesh_vertex_td( geo_mesh, dome, v_buf );

		// 产生面索引
		generate_shadow_mesh_index( geo_mesh, dome, v_buf, v_vis );

		AUTO_DELETE( v_vis );
	}

	void render_object_shadow_factory::generate_face_visible_flag( short* &v_vis_buf, sphere<double>& dome, d3d9_hardware_mesh_buffer* v_buf  )
	{
		vector_3d vec_src( dome.sphere_center );
		int p1 = 0, p2 = 0, p3 = 0;

		if ( v_buf->m_32bit_index )
		{
			ulong* index_buf = (ulong*)(v_buf->m_index_buffer);

			for ( int i = 0; i < (int)v_buf->m_num_of_triangles; i++ )
			{
				int k = i * 3;
				p1 = index_buf[k];
				p2 = index_buf[k+1];
				p3 = index_buf[k+2];

				vector3<double> point1( (float*)(v_buf->m_vertex_buffer) + p1 * v_buf->m_vertex_stride / 4 );
				vector3<double> point2( (float*)(v_buf->m_vertex_buffer) + p2 * v_buf->m_vertex_stride / 4 );
				vector3<double> point3( (float*)(v_buf->m_vertex_buffer) + p3 * v_buf->m_vertex_stride / 4 );

				// 判断线到球心的距离
				if( d3d9_intersect_helper::point_to_segment_distance_d( vec_src, point1, point2 ) > dome.sphere_radius &&
					d3d9_intersect_helper::point_to_segment_distance_d( vec_src, point2, point3 ) > dome.sphere_radius &&
					d3d9_intersect_helper::point_to_segment_distance_d( vec_src, point3, point1 ) > dome.sphere_radius )
				{
					// 所有的边都在球外
					v_vis_buf[i] = 1;
					continue;
				}
			}
		}
		else
		{
			ushort* index_buf = (ushort*)(v_buf->m_index_buffer);

			for ( int i = 0; i < (int)v_buf->m_num_of_triangles; i++ )
			{
				int k = i * 3;
				p1 = index_buf[k];
				p2 = index_buf[k+1];
				p3 = index_buf[k+2];

				vector3<double> point1( (float*)(v_buf->m_vertex_buffer) + p1 * v_buf->m_vertex_stride / 4 );
				vector3<double> point2( (float*)(v_buf->m_vertex_buffer) + p2 * v_buf->m_vertex_stride / 4 );
				vector3<double> point3( (float*)(v_buf->m_vertex_buffer) + p3 * v_buf->m_vertex_stride / 4 );

				// 判断线到球心的距离
				if( d3d9_intersect_helper::point_to_segment_distance_d( vec_src, point1, point2 ) > dome.sphere_radius &&
					d3d9_intersect_helper::point_to_segment_distance_d( vec_src, point2, point3 ) > dome.sphere_radius &&
					d3d9_intersect_helper::point_to_segment_distance_d( vec_src, point3, point1 ) > dome.sphere_radius )
				{
					// 所有的边都在球外
					v_vis_buf[i] = 1;
					continue;
				}
			}
		}
	}

	void render_object_shadow_factory::generate_shadow_mesh_index_td( render_object_geometry_mesh* geo_mesh, sphere<double>& dome, d3d9_hardware_mesh_buffer* v_buf, short* in_indice_vis )
	{

	}

	void render_object_shadow_factory::generate_shadow_mesh_vertex_td( render_object_geometry_mesh* geo_mesh, sphere<double>& dome, d3d9_hardware_mesh_buffer* v_buf )
	{
		geo_mesh->m_vertex_array.resize( v_buf->m_num_of_vertices * 2 );
		geo_mesh->m_render_aabb.make_invalid();

		vector3<double> light_pos;
		light_pos.x = dome.sphere_center.x;
		light_pos.y = dome.sphere_center.y;
		light_pos.z = dome.sphere_center.z;

		for ( int ni = 0; ni < (int)v_buf->m_num_of_vertices; ni++ )
		{
			vector3<double> un_extruded_point( (float*)(v_buf->m_vertex_buffer) + ni * v_buf->m_vertex_stride / 4 );

			geo_mesh->m_render_aabb.combine( un_extruded_point );

			geo_mesh->m_vertex_array[ni+v_buf->m_num_of_vertices].x = un_extruded_point.x;
			geo_mesh->m_vertex_array[ni+v_buf->m_num_of_vertices].y = un_extruded_point.y;
			geo_mesh->m_vertex_array[ni+v_buf->m_num_of_vertices].z = un_extruded_point.z;
			geo_mesh->m_vertex_array[ni+v_buf->m_num_of_vertices].color = 0xff00ff00;

			vector3<double> temp_vec = vector_3d::normalize( un_extruded_point - light_pos );
			temp_vec = temp_vec * dome.sphere_radius;

			geo_mesh->m_render_aabb.combine( temp_vec );

			geo_mesh->m_vertex_array[ni].x = temp_vec.x;
			geo_mesh->m_vertex_array[ni].y = temp_vec.y;
			geo_mesh->m_vertex_array[ni].z = temp_vec.z;
			geo_mesh->m_vertex_array[ni].color = 0xff00ff00;
		}
	}

	////  点光源生成阴影体,专门针对threat_dome
	////////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////
	// create shadow mesh buffer
	//////////////////////////////////////////////////////////////////////////
	void render_object_shadow_factory::generate_shadow_mesh_buffer( d3d9_hardware_mesh_buffer* src_mesh, d3d9_hardware_mesh_buffer* des_mesh )
	{
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// 创建阴影渲染对象
		//
		// 获取顶点信息和面信息
		ulong* v_adj_info  = new ulong[3 * src_mesh->get_num_of_triangles()];
		ulong* v_point_rep = new ulong[src_mesh->get_num_of_vertices()];

		src_mesh->generate_adjacency( 0.0001f, v_adj_info );		
		src_mesh->generate_point_rep( v_adj_info, v_point_rep );

		//
		// 添加退化四边形
		//
		usx_tri_face_index_array v_new_face_index;
		position_normal_array v_new_vertex_array;
		
		edge_map v_edge_map;
		for ( int i = 0; i < (int)src_mesh->get_num_of_triangles(); i++ )
		{
			// 取面索引
			ulong a, b, c;
			if(src_mesh->is_32bit_index())
			{
				a = ((ulong*)src_mesh->m_index_buffer)[i * 3];
				b = ((ulong*)src_mesh->m_index_buffer)[i * 3 + 1];
				c = ((ulong*)src_mesh->m_index_buffer)[i * 3 + 2];
			}
			else
			{
				a = ((ushort*)src_mesh->m_index_buffer)[i * 3];
				b = ((ushort*)src_mesh->m_index_buffer)[i * 3 + 1];
				c = ((ushort*)src_mesh->m_index_buffer)[i * 3 + 2];
			}

			// 计算法向
			vector3<float> v_normal = render_object_shadow_factory::compute_normal(	(vector3<float>*)(src_mesh->get_fat_vertex(a)),
				(vector3<float>*)(src_mesh->get_fat_vertex(b)),
				(vector3<float>*)(src_mesh->get_fat_vertex(c)) );

			vector3<float> tmp_vec( 0.0, 0.0, 0.0 );

			position_normal v1,v2,v3;

			tmp_vec = src_mesh->get_vector3( 0, a );
			v1.x = tmp_vec.x; v1.y = tmp_vec.y; v1.z = tmp_vec.z;
			v1.nx = v_normal.x;v1.ny = v_normal.y;v1.nz = v_normal.z;

			tmp_vec = src_mesh->get_vector3( 0, b );
			v2.x = tmp_vec.x; v2.y = tmp_vec.y; v2.z = tmp_vec.z;
			v2.nx = v_normal.x;v2.ny = v_normal.y;v2.nz = v_normal.z;

			tmp_vec = src_mesh->get_vector3( 0, c );
			v3.x = tmp_vec.x; v3.y = tmp_vec.y; v3.z = tmp_vec.z;
			v3.nx = v_normal.x;v3.ny = v_normal.y;v3.nz = v_normal.z;

			// 增加三角形
			usx_tri_face_index v_new_face;
			v_new_face.a = v_new_vertex_array.size();
			v_new_face.b = v_new_face.a + 1;
			v_new_face.c = v_new_face.b + 1;
			v_new_face_index.push_back( v_new_face );
						
			v_new_vertex_array.push_back( v1 );
			v_new_vertex_array.push_back( v2 );
			v_new_vertex_array.push_back( v3 );

			render_object_shadow_factory::insert_triangle( v_point_rep[a], v_point_rep[b],  v_new_face.a, v_new_face.b, v_edge_map, v_new_face_index );
			render_object_shadow_factory::insert_triangle( v_point_rep[b], v_point_rep[c],  v_new_face.b, v_new_face.c, v_edge_map, v_new_face_index );
			render_object_shadow_factory::insert_triangle( v_point_rep[c], v_point_rep[a],  v_new_face.c, v_new_face.a, v_edge_map, v_new_face_index );
		}

		// 缝合
		render_object_shadow_factory::patching( src_mesh, v_edge_map, v_new_vertex_array, v_new_face_index );
		
		AUTO_DELETE( v_adj_info );
		AUTO_DELETE( v_point_rep );

		// 给目标Mesh赋值
		des_mesh->set_fvf( position_normal::fvf );
		des_mesh->set_stride( position_normal::stride );
		des_mesh->set_num_of_vertices( v_new_vertex_array.size() );
		des_mesh->set_vertex_buffer_size( sizeof( position_normal ) * des_mesh->m_num_of_vertices );
		position_normal* v_vertex_buf = new position_normal[v_new_vertex_array.size()];
		memcpy( (void*)v_vertex_buf, &v_new_vertex_array[0], des_mesh->m_vertex_buffer_size  );
		des_mesh->set_vertex_buffer( v_vertex_buf );
		
		des_mesh->set_32bit_index( true );
		des_mesh->set_num_of_triangles( v_new_face_index.size() );
		des_mesh->set_index_buffer_size( v_new_face_index.size() * sizeof( usx_tri_face_index ) );
		ulong* v_index_buf = new ulong[des_mesh->m_num_of_triangles * 3];
		memcpy( v_index_buf, &v_new_face_index[0], des_mesh->m_index_buffer_size );
		des_mesh->set_index_buffer( v_index_buf );
		
	}

	void render_object_shadow_factory::patching( d3d9_hardware_mesh_buffer* v_buffer, edge_map& in_edge_map, position_normal_array& in_new_vertex_array,  usx_tri_face_index_array& in_new_face_index )
	{
		while ( in_edge_map.size() < 2 )
			return;

		edge_map::iterator itr1 = in_edge_map.begin();
		edge_map::iterator itr2 = in_edge_map.begin();
		itr2++;
		bool v_alone = true;
		for ( ; itr2 != in_edge_map.end(); itr2++ )
		{
			if(	( itr1->second.m_v3 == 0xffffffff || itr1->second.m_v4 == 0xffffffff ) &&
				( itr2->second.m_v3 == 0xffffffff  || itr2->second.m_v4 == 0xffffffff ))
			{
				int shared_vertex = 0;

				ulong o1, o2, o3;
				ulong n1, n2, n3, n4;
				if( itr2->first.first == itr1->first.second )
				{
					shared_vertex++;
					o1 = itr1->first.first;
					o2 = itr1->first.second;
					o3 = itr2->first.second;

					n1 = itr1->second.m_v1;
					n2 = itr1->second.m_v2;
					n3 = itr2->second.m_v1;
					n4 = itr2->second.m_v2;
				}

				if( itr2->first.second == itr1->first.first )
				{
					shared_vertex++;	
					o1 = itr2->first.first;
					o2 = itr2->first.second;
					o3 = itr1->first.second;

					n1 = itr2->second.m_v1;
					n2 = itr2->second.m_v2;
					n3 = itr1->second.m_v1;
					n4 = itr1->second.m_v2;
				}

				if( shared_vertex == 2 )
				{
					usx_tri_face_index tri1;
					tri1.a = itr1->second.m_v1;
					tri1.b = itr2->second.m_v2;
					tri1.c = itr2->second.m_v1;
					in_new_face_index.push_back( tri1 );

					usx_tri_face_index tri2;
					tri2.a = itr1->second.m_v1;
					tri2.b = itr2->second.m_v1;
					tri2.c = itr1->second.m_v2;
					in_new_face_index.push_back( tri2 );

					in_edge_map.erase( itr1 );
					in_edge_map.erase( itr2 );

					v_alone = false;

					break;
				}
				else if( shared_vertex == 1 )
				{
					// 计算法矢
					vector3<float> v_normal = render_object_shadow_factory::compute_normal(	(vector_3f*)(v_buffer->get_fat_vertex(o1)),
						(vector_3f*)(v_buffer->get_fat_vertex(o3)),
						(vector_3f*)(v_buffer->get_fat_vertex(o2)) );

					// 增加顶点
					position_normal v1,v2,v3;
					v1 = in_new_vertex_array[n1];
					v1.nx = v_normal.x;v1.ny = v_normal.y;v1.nz = v_normal.z;
					v2 = in_new_vertex_array[n4];
					v2.nx = v_normal.x;v2.ny = v_normal.y;v2.nz = v_normal.z;
					v3 = in_new_vertex_array[n3];
					v3.nx = v_normal.x;v3.ny = v_normal.y;v3.nz = v_normal.z;

					// 增加三角形
					usx_tri_face_index v_new_face;
					v_new_face.a = in_new_vertex_array.size();
					v_new_face.b = v_new_face.a + 1;
					v_new_face.c = v_new_face.b + 1;
					in_new_face_index.push_back( v_new_face );

					in_new_vertex_array.push_back( v1 );
					in_new_vertex_array.push_back( v2 );
					in_new_vertex_array.push_back( v3 );

					// 增加退化四边形
					// first quad
					usx_tri_face_index tri1;
					tri1.a = n1;
					tri1.b = v_new_face.a;
					tri1.c = v_new_face.c;
					in_new_face_index.push_back( tri1 );

					usx_tri_face_index tri2;
					tri2.a = n1;
					tri2.b = v_new_face.c;
					tri2.c = n2;
					in_new_face_index.push_back( tri2 );

					// second quad
					usx_tri_face_index tri3;
					tri3.a = n3;
					tri3.b = v_new_face.c;
					tri3.c = v_new_face.b;
					in_new_face_index.push_back( tri3 );

					usx_tri_face_index tri4;
					tri4.a = n3;
					tri4.b = v_new_face.b;
					tri4.c = n4;
					in_new_face_index.push_back( tri4 );

					degenerate_quad v_new_quad;
					v_new_quad.m_v1 = v_new_face.a;
					v_new_quad.m_v2 = v_new_face.b;
					in_edge_map.insert( make_pair( make_pair( o1, o3 ), v_new_quad ) );
					in_edge_map.erase( itr1 );
					in_edge_map.erase( itr2 );

					v_alone = false;

					break;
				}								
			}

		}// for

		if( v_alone == true )
		{
			in_edge_map.erase( itr1 );
		}

	}


	//
	//////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////
	// 封闭体裁剪模型
	d3d9_hardware_mesh_buffer* render_object_shadow_factory::closed_mesh_clip_mesh( d3d9_hardware_mesh_buffer* close_mesh, d3d9_hardware_mesh_buffer* v_mesh )
	{
		// 判断点的状态，在close_mesh内，在close_mesh外
		std::vector<int> point_type_array;
		point_type_array.resize( v_mesh->m_num_of_vertices );

		face_normal_array& v_close_mesh_face = close_mesh->get_face_normal();

		for ( int ni = 0; ni < v_mesh->m_num_of_vertices; ni++ )
		{
			point_type_array[ni] = -1;
			vector_3f vec( (float*)(v_mesh->m_vertex_buffer) + ni * v_mesh->m_vertex_stride / 4 );
			
			for ( int nj = 0; nj < close_mesh->m_num_of_triangles; nj++ )
			{
				double v_val = v_close_mesh_face[nj].a * vec.x + v_close_mesh_face[nj].b * vec.y + v_close_mesh_face[nj].c * vec.z + v_close_mesh_face[nj].d ;
				if ( v_val > 0.0 )
				{
					// 点在mesh外面
					point_type_array[ni] = 1;
					// 跳出循环
					break;
				}
			}
		}

		d3d9_hardware_mesh_buffer* v_new_mesh = new d3d9_hardware_mesh_buffer();
		
		// 新旧顶点对照表
		std::vector<int> new_old_vertex_mapping;
		new_old_vertex_mapping.resize( v_mesh->m_num_of_vertices );
		
		// 遍历所有的三角形，判断是否要做裁剪,如果要做，就做
		if ( v_mesh->m_32bit_index )
		{
			v_new_mesh->set_32bit_index( true );
			// 新顶点列表
			std::vector<vector_3f> new_vertex_array;
			// 新索引列表
			int v_index_cursor = 0;
			std::vector<ulong> new_index_array;

			// 开始
			ulong* index_buf = (ulong*)(v_mesh->m_index_buffer);
			for ( int ni = 0; ni < v_mesh->m_num_of_triangles; ni++ )
			{
				int k = ni * 3;
				int a = index_buf[k];
				int b = index_buf[k+1];
				int c = index_buf[k+2];

				vector_3f vec_a( (float*)(v_mesh->m_vertex_buffer) + a * v_mesh->m_vertex_stride / 4 );
				vector_3f vec_b( (float*)(v_mesh->m_vertex_buffer) + b * v_mesh->m_vertex_stride / 4 );
				vector_3f vec_c( (float*)(v_mesh->m_vertex_buffer) + c * v_mesh->m_vertex_stride / 4 );

				// 如果三个点都在里面，加入新的模型中
				if ( point_type_array[a] == 1 && point_type_array[b] == 1 && point_type_array[c] == 1 )
				{
					// 判断顶点
					if ( new_old_vertex_mapping[a] == -1 )
					{
						new_old_vertex_mapping[a] = new_vertex_array.size();
						new_vertex_array.push_back( vec_a );
					}
					if ( new_old_vertex_mapping[b] == -1 )
					{
						new_old_vertex_mapping[b] = new_vertex_array.size();
						new_vertex_array.push_back( vec_b );
					}
					if ( new_old_vertex_mapping[c] == -1 )
					{
						new_old_vertex_mapping[c] = new_vertex_array.size();
						new_vertex_array.push_back( vec_c );
					}
					
					new_index_array[v_index_cursor++] = new_old_vertex_mapping[a];
					new_index_array[v_index_cursor++] = new_old_vertex_mapping[b];
					new_index_array[v_index_cursor++] = new_old_vertex_mapping[c];
										
					continue;
				}

				// 如果3个点都在外面，用3条边去求交
				if ( point_type_array[a] == -1 && point_type_array[b] == -1 && point_type_array[c] == -1 )
				{
					// 必然会产生新的点啦
					render_object_shadow_factory::face_intersect_mesh_int( vec_a, vec_b, vec_c, close_mesh, new_vertex_array, new_index_array );

					continue;
				}

				// 严重说明
				// 目前下面的裁剪算法只对close_mesh是凸的时候有效
				//
				vector_3f_array v_pick_points;

				// 裁剪
				if ( point_type_array[a] == 1 )
				{
					v_pick_points.push_back( vec_a );
				}

				render_object_shadow_factory::segment_intersect_mesh( vec_a, vec_b, close_mesh, v_pick_points );

				if ( point_type_array[b] == 1 )
				{
					v_pick_points.push_back( vec_b );
				}
				
				render_object_shadow_factory::segment_intersect_mesh( vec_b, vec_c, close_mesh, v_pick_points );

				if ( point_type_array[c] == 1 )
				{
					v_pick_points.push_back( vec_c );
				}

				render_object_shadow_factory::segment_intersect_mesh( vec_c, vec_a, close_mesh, v_pick_points );
				
				// 添加裁剪面
				render_object_shadow_factory::add_polygon_to_mesh( v_pick_points, new_vertex_array, new_index_array );

				v_pick_points.clear();
			}
		}
		else
		{

		}
		
		return v_new_mesh;
	}

	// 严重说明
	// 目前下面的裁剪算法只对close_mesh是凸的时候有效
	void render_object_shadow_factory::segment_intersect_mesh( vector_3f& s_vec, vector_3f& e_vec, d3d9_hardware_mesh_buffer* v_mesh, vector_3f_array& v_vec_array )
	{

	}

	void render_object_shadow_factory::add_polygon_to_mesh( vector_3f_array& v_new_points, vector_3f_array& pt_array, face_int_index_array& v_index_array )
	{

	}

	void render_object_shadow_factory::face_intersect_mesh_int( vector_3f& vec1, vector_3f& vec2, vector_3f& vec3, d3d9_hardware_mesh_buffer* v_mesh, vector_3f_array& pt_array, face_int_index_array& v_index_array )
	{

	}

	void render_object_shadow_factory::face_intersect_mesh_short( vector_3f& vec1, vector_3f& vec2, vector_3f& vec3, d3d9_hardware_mesh_buffer* v_mesh, vector_3f_array& pt_array, face_short_index_array& v_index_array )
	{

	}

	// 封闭体裁剪模型
	//////////////////////////////////////////////////////////////////////////

}