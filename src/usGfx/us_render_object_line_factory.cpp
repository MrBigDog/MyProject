///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_render_object_line_factory.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : render_object_line_factory class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_render_object_line_factory.h"

namespace uniscope_globe
{
	void render_object_line_factory::push_tri_strip_point( std::vector<position_normal_color>& triangle_list, vector3<double>& pt, vector3<double>& normal, ulong color )
	{
		position_normal_color vec;
		vec.x = pt.x;
		vec.y = pt.y;
		vec.z = pt.z;
		vec.nx = normal.x;
		vec.ny = normal.y;
		vec.nz = normal.z;
		vec.color = color;
		triangle_list.push_back( vec );
	}

	void render_object_line_factory::generate_line_stencil_mesh( render_object_geometry_mesh* geo_mesh, 
		vector_3d* v_vecs_geo,  ulong v_src_point_size, coordinates_array& out_array, 
		double v_width_geo, ulong v_line_color,  double v_uper_height,  double v_lower_height )
	{
		geo_mesh->set_shadow_color( v_line_color );

		coordinates_array v_vertex_array;
		coordinates_array v_upper_coords_array;
		coordinates_array v_lower_coords_array;

		// 两个点的情况
		if ( v_src_point_size == 2 )
		{	
			vector3<double> pos_geo1 = v_vecs_geo[0];
			vector3<double> pos_geo2 = v_vecs_geo[1];
			vector3<double> direction = vector3<double>::cross( pos_geo1, pos_geo2 );
			direction.normalize();

			// 第一个点
			vector3<double> pos_geo1_scaled = vector3<double>::normalize(pos_geo1) * v_uper_height;

			v_upper_coords_array.push_back( pos_geo1_scaled - direction * v_width_geo );
			v_upper_coords_array.push_back( pos_geo1_scaled + direction * v_width_geo );

			pos_geo1_scaled = vector3<double>::normalize(pos_geo1) * v_lower_height;

			v_lower_coords_array.push_back( pos_geo1_scaled - direction * v_width_geo );
			v_lower_coords_array.push_back( pos_geo1_scaled + direction * v_width_geo );

			// 第二个点
			vector3<double> pos_geo2_scaled = vector3<double>::normalize(pos_geo2) * v_uper_height;

			v_upper_coords_array.push_back( pos_geo2_scaled - direction * v_width_geo );
			v_upper_coords_array.push_back( pos_geo2_scaled + direction * v_width_geo );

			pos_geo2_scaled = vector3<double>::normalize(pos_geo2) * v_lower_height;

			v_lower_coords_array.push_back( pos_geo2_scaled - direction * v_width_geo );
			v_lower_coords_array.push_back( pos_geo2_scaled + direction * v_width_geo );

			v_vertex_array.insert( v_vertex_array.end(), v_upper_coords_array.begin(), v_upper_coords_array.end() );
			v_vertex_array.insert( v_vertex_array.end(), v_lower_coords_array.begin(), v_lower_coords_array.end() );

			insert_vertex_to_line_mesh( geo_mesh, v_src_point_size, v_vertex_array, out_array, v_line_color );

			return;
		}


		// 三个点以上的情况

		// 第一个点
		{
			vector3<double> pos_geo1 = v_vecs_geo[0];
			vector3<double> pos_geo2 = v_vecs_geo[1];
			vector3<double> direction = vector3<double>::cross( pos_geo1, pos_geo2 );
			direction.normalize();

			vector3<double> pos_geo1_scaled = vector3<double>::normalize(pos_geo1) * v_uper_height;

			v_upper_coords_array.push_back( pos_geo1_scaled - direction * v_width_geo );
			v_upper_coords_array.push_back( pos_geo1_scaled + direction * v_width_geo );

			pos_geo1_scaled = vector3<double>::normalize(pos_geo1) * v_lower_height;

			v_lower_coords_array.push_back( pos_geo1_scaled - direction * v_width_geo );
			v_lower_coords_array.push_back( pos_geo1_scaled + direction * v_width_geo );
		}	

		for( int i = 1; i < int(v_src_point_size - 1); i ++ )
		{
			vector3<double> pos_geo1 = v_vecs_geo[i-1];
			vector3<double> pos_geo2 = v_vecs_geo[i];
			vector3<double> pos_geo3 = v_vecs_geo[i+1];

			// direction
			vector3<double> direction1 = vector3<double>::cross( pos_geo1, pos_geo2 );
			direction1.normalize();

			vector3<double> direction2 = vector3<double>::cross( pos_geo2, pos_geo3 );
			direction2.normalize();

			vector3<double> direction = ( direction1 + direction2 ) * 0.5;
			direction.normalize();

			vector3<double> v_temp_normal = vector3<double>::normalize( pos_geo1 - pos_geo2 );
			double dot_val = vector3<double>::dot( v_temp_normal, direction );
			if ( dot_val < 0.0 )
			{
				dot_val = vector3<double>::dot( v_temp_normal, -direction );
			}

			double width;
			if ( math<double>::acos_( dot_val ) < 1 * math_d::s_deg_to_rad ) 
			{
				width = v_width_geo;
			}
			else
			{
				width = v_width_geo / math<double>::sin_( math<double>::acos_( dot_val ) );
			}

			vector3<double> pos_geo2_scaled = vector3<double>::normalize(pos_geo2) * v_uper_height;
			v_upper_coords_array.push_back( pos_geo2_scaled - direction * width );
			v_upper_coords_array.push_back( pos_geo2_scaled + direction * width );

			pos_geo2_scaled = vector3<double>::normalize(pos_geo2) * v_lower_height;

			v_lower_coords_array.push_back( pos_geo2_scaled - direction * width );
			v_lower_coords_array.push_back( pos_geo2_scaled + direction * width );
		}

		// 最后一个点
		{
			vector3<double> pos_geo1 = v_vecs_geo[v_src_point_size - 2];
			vector3<double> pos_geo2 = v_vecs_geo[v_src_point_size - 1];

			vector3<double> direction = vector3<double>::cross( pos_geo1, pos_geo2 );
			direction.normalize();

			vector3<double> pos_geo2_scaled = vector3<double>::normalize(pos_geo2) * v_uper_height;

			v_upper_coords_array.push_back( pos_geo2_scaled - direction * v_width_geo );
			v_upper_coords_array.push_back( pos_geo2_scaled + direction * v_width_geo );

			pos_geo2_scaled = vector3<double>::normalize(pos_geo2) * v_lower_height;

			v_lower_coords_array.push_back( pos_geo2_scaled - direction * v_width_geo );
			v_lower_coords_array.push_back( pos_geo2_scaled + direction * v_width_geo );
		}

		v_vertex_array.insert( v_vertex_array.end(), v_upper_coords_array.begin(), v_upper_coords_array.end() );
		v_vertex_array.insert( v_vertex_array.end(), v_lower_coords_array.begin(), v_lower_coords_array.end() );

		insert_vertex_to_line_mesh( geo_mesh, v_src_point_size, v_vertex_array, out_array, v_line_color );		
	}

	void render_object_line_factory::insert_vertex_to_line_mesh( render_object_geometry_mesh* v_mesh, ulong vertex_num, coordinates_array& in_array, coordinates_array& out_array, ulong v_line_color )
	{
		ulong v_size = vertex_num<<1;
		ulong start_index = out_array.size();

		for ( int i = start_index;i < (int)(start_index + v_size - 3); i += 2 )
		{
			v_mesh->m_index_array.push_back( i );
			v_mesh->m_index_array.push_back( i + 3 );
			v_mesh->m_index_array.push_back( i + 1 );

			v_mesh->m_index_array.push_back( i );
			v_mesh->m_index_array.push_back( i + 2 );
			v_mesh->m_index_array.push_back( i + 3 );
		}

		for ( int i = start_index;i < (int)(start_index + v_size - 3); i += 2 )
		{
			v_mesh->m_index_array.push_back( i + v_size );
			v_mesh->m_index_array.push_back( i + 1 + v_size );
			v_mesh->m_index_array.push_back( i + 3 + v_size );

			v_mesh->m_index_array.push_back( i + v_size);
			v_mesh->m_index_array.push_back( i + 3 + v_size );
			v_mesh->m_index_array.push_back( i + 2 + v_size );			
		}

		for ( int i = start_index;i < (int)(start_index + v_size - 3); i += 2 )
		{
			v_mesh->m_index_array.push_back( i );
			v_mesh->m_index_array.push_back( i + v_size );
			v_mesh->m_index_array.push_back( i + 2 + v_size );

			v_mesh->m_index_array.push_back( i );
			v_mesh->m_index_array.push_back( i + 2 + v_size );
			v_mesh->m_index_array.push_back( i + 2 );

			v_mesh->m_index_array.push_back( i + 1 );
			v_mesh->m_index_array.push_back( i + 3 + v_size );
			v_mesh->m_index_array.push_back( i + 1 + v_size );


			v_mesh->m_index_array.push_back( i + 1 );
			v_mesh->m_index_array.push_back( i + 3 );
			v_mesh->m_index_array.push_back( i + 3 + v_size );		
		}

		// 前后两个面
		v_mesh->m_index_array.push_back( start_index );
		v_mesh->m_index_array.push_back( start_index + 1 + v_size );
		v_mesh->m_index_array.push_back( start_index + v_size );

		v_mesh->m_index_array.push_back( start_index );
		v_mesh->m_index_array.push_back( start_index + 1 );
		v_mesh->m_index_array.push_back( start_index + 1 + v_size );

		v_mesh->m_index_array.push_back( start_index + v_size - 1 );
		v_mesh->m_index_array.push_back( start_index + 2 * v_size - 2 );
		v_mesh->m_index_array.push_back( start_index + 2 * v_size - 1 );

		v_mesh->m_index_array.push_back( start_index + v_size - 1 );
		v_mesh->m_index_array.push_back( start_index + v_size - 2 );
		v_mesh->m_index_array.push_back( start_index + 2 * v_size - 2 );

		for ( int i = 0; i < (int)in_array.size(); i ++ )
		{
			v_mesh->m_render_aabb.combine( in_array[i] );
		}

		out_array.insert( out_array.end(), in_array.begin(), in_array.end() );
	}


	void render_object_line_factory::insert_index_to_line_mesh( render_object_line_volume* v_mesh, ulong size )
	{
		ulong v_size = size<<1;
		ulong start_index = v_mesh->m_vertex_array.size();

		for ( int i = start_index;i < (int)(start_index + v_size - 3); i += 2 )
		{
			v_mesh->m_index_array.push_back( i );
			v_mesh->m_index_array.push_back( i + 3 );
			v_mesh->m_index_array.push_back( i + 1 );

			v_mesh->m_index_array.push_back( i );
			v_mesh->m_index_array.push_back( i + 2 );
			v_mesh->m_index_array.push_back( i + 3 );
		}

		for ( int i = start_index;i < (int)(start_index + v_size - 3); i += 2 )
		{
			v_mesh->m_index_array.push_back( i + v_size );
			v_mesh->m_index_array.push_back( i + 1 + v_size );
			v_mesh->m_index_array.push_back( i + 3 + v_size );

			v_mesh->m_index_array.push_back( i + v_size);
			v_mesh->m_index_array.push_back( i + 3 + v_size );
			v_mesh->m_index_array.push_back( i + 2 + v_size );			
		}

		for ( int i = start_index;i < (int)(start_index + v_size - 3); i += 2 )
		{
			v_mesh->m_index_array.push_back( i );
			v_mesh->m_index_array.push_back( i + v_size );
			v_mesh->m_index_array.push_back( i + 2 + v_size );

			v_mesh->m_index_array.push_back( i );
			v_mesh->m_index_array.push_back( i + 2 + v_size );
			v_mesh->m_index_array.push_back( i + 2 );

			v_mesh->m_index_array.push_back( i + 1 );
			v_mesh->m_index_array.push_back( i + 3 + v_size );
			v_mesh->m_index_array.push_back( i + 1 + v_size );


			v_mesh->m_index_array.push_back( i + 1 );
			v_mesh->m_index_array.push_back( i + 3 );
			v_mesh->m_index_array.push_back( i + 3 + v_size );		
		}

		// 前后两个面
		v_mesh->m_index_array.push_back( start_index );
		v_mesh->m_index_array.push_back( start_index + 1 + v_size );
		v_mesh->m_index_array.push_back( start_index + v_size );

		v_mesh->m_index_array.push_back( start_index );
		v_mesh->m_index_array.push_back( start_index + 1 );
		v_mesh->m_index_array.push_back( start_index + 1 + v_size );

		v_mesh->m_index_array.push_back( start_index + v_size - 1 );
		v_mesh->m_index_array.push_back( start_index + 2 * v_size - 2 );
		v_mesh->m_index_array.push_back( start_index + 2 * v_size - 1 );

		v_mesh->m_index_array.push_back( start_index + v_size - 1 );
		v_mesh->m_index_array.push_back( start_index + v_size - 2 );
		v_mesh->m_index_array.push_back( start_index + 2 * v_size - 2 );
	}


	//////////////////////////////////////////////////////////////////////////////////////////////
	// @brief  创建矢量线
	//
	// @param  geo_mesh		[out]	局部坐标系中的模型
	// @param  in_vecs_cart [in]	局部坐标下的点集
	// @param  line_color	[in]	矢量线颜色
	//////////////////////////////////////////////////////////////////////////////////////////////
	void render_object_line_factory::create_render_object_line_cart( OUT render_object_line* geo_mesh, IN coordinates_array& in_vecs_cart, IN ulong line_color )
	{
		int v_size = (int)in_vecs_cart.size();
		geo_mesh->m_vertex_array.clear();
		geo_mesh->m_vertex_array.resize( v_size );

		for( int ni = 0; ni < v_size; ni++ )
		{
			geo_mesh->m_vertex_array[ni].x = in_vecs_cart[ni].x;
			geo_mesh->m_vertex_array[ni].y = in_vecs_cart[ni].y;
			geo_mesh->m_vertex_array[ni].z = in_vecs_cart[ni].z;
			geo_mesh->m_vertex_array[ni].color = line_color;
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////////
	// @brief  创建矢量线
	//
	// @param  geo_mesh		[out]	局部坐标系中的模型
	// @param  in_vecs_geo  [in]	Geocentric坐标下的点集
	// @param  v_mat_geo2cart [in]	Geocentric坐标转局部坐标举证
	// @param  line_color	[in]	矢量线颜色
	//////////////////////////////////////////////////////////////////////////////////////////////
	void render_object_line_factory::create_render_object_line_geo( OUT render_object_line* geo_mesh, IN coordinates_array& in_vecs_geo, IN matrix_4d& v_mat_geo2cart, IN ulong line_color )
	{
		int v_size = (int)in_vecs_geo.size();
		geo_mesh->m_vertex_array.clear();
		geo_mesh->m_vertex_array.resize( v_size );

		for( int ni = 0; ni < v_size; ni++ )
		{
			vector_3d vec = in_vecs_geo[ni] * v_mat_geo2cart;

			geo_mesh->m_vertex_array[ni].x = vec.x;
			geo_mesh->m_vertex_array[ni].y = vec.y;
			geo_mesh->m_vertex_array[ni].z = vec.z;
			geo_mesh->m_vertex_array[ni].color = line_color;
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////////
	// @brief  创建片状的矢量线
	//
	// @param  geo_mesh		[out]	局部坐标系中的模型
	// @param  in_vecs_cart [in]	局部坐标下的点集
	// @param  pixel_width	[in]	矢量线像素宽度
	// @param  line_color	[in]	矢量线颜色
	//////////////////////////////////////////////////////////////////////////////////////////////
	void render_object_line_factory::create_render_object_line_flake_cart( OUT render_object_line_flake* geo_mesh, IN coordinates_array& in_vecs_cart, IN double pixel_width, IN ulong line_color )
	{
		geo_mesh->set_line_width(pixel_width);
		
		int n_vector_count = in_vecs_cart.size();
		if ( n_vector_count < 2 ) return;
		
		if ( n_vector_count == 2)
		{
			vector3<double> A = in_vecs_cart[0];
			vector3<double> B = in_vecs_cart[1];
			vector3<double> normal = vector3<double>::normalize(vector3<double>::cross( A, B ));

			vector3<double> v1,v2,v3,v4;

			v1 = A;
			v2 = A;
			v3 = B;
			v4 = B;

			vector3<double> tangent =  vector3<double>::normalize(B - A);

			push_tri_strip_point( geo_mesh->m_vertex_array, v1, -tangent, line_color );
			push_tri_strip_point( geo_mesh->m_vertex_array, v1, -tangent, line_color );

			push_tri_strip_point( geo_mesh->m_vertex_array, v2,  tangent, line_color );
			push_tri_strip_point( geo_mesh->m_vertex_array, v3, -tangent, line_color );

			push_tri_strip_point( geo_mesh->m_vertex_array, v4,  tangent, line_color );
			push_tri_strip_point( geo_mesh->m_vertex_array, v4,  tangent, line_color );

			return;
		}

		// n_vector_count >= 3
		{
			vector3<double> A = in_vecs_cart[0];
			vector3<double> B = in_vecs_cart[1];
			vector3<double> normal = vector3<double>::normalize(vector3<double>::cross( A, B ));

			vector3<double> v1,v2;
			v1 = A;
			v2 = A;

			vector3<double> tangent =  vector3<double>::normalize(B - A);

			push_tri_strip_point( geo_mesh->m_vertex_array, v1, -tangent, line_color );
			push_tri_strip_point( geo_mesh->m_vertex_array, v1, -tangent, line_color );
			push_tri_strip_point( geo_mesh->m_vertex_array, v2,  tangent, line_color );
		}			


		for ( int ni = 1; ni< n_vector_count - 1; ni++ )
		{
			vector3<double> A  = in_vecs_cart[ni - 1];
			vector3<double> B  = in_vecs_cart[ni];
			vector3<double> C  = in_vecs_cart[ni + 1];

			// normal
			vector3<double> normal1 = vector3<double>::normalize(vector3<double>::cross( A, B ));
			vector3<double> normal2 = vector3<double>::normalize(vector3<double>::cross( B, C ));
			vector3<double> normal  = vector3<double>::normalize( ( normal1 + normal2 ) * 0.5 );
			vector3<double> v_temp_normal = vector3<double>::normalize( A - B );
			double dot_val = vector3<double>::dot( v_temp_normal, normal );
			if ( dot_val < 0.0 )
			{
				dot_val = vector3<double>::dot( v_temp_normal, -normal );
			}

			double v_width_scale;

			if ( math<double>::acos_( dot_val ) < 1 * RADIAN ) 
			{
				v_width_scale = 1.0;
			}
			else
			{
				v_width_scale = 1.0 / math<double>::sin_( math<double>::acos_( dot_val ) );
			}

			vector3<double> v1,v2;
			v1 = B;
			v2 = B;

			vector3<double> tangent1 =  vector3<double>::normalize( B - A );
			vector3<double> tangent2 =  vector3<double>::normalize( C - B );
			vector3<double> tangent3 =  vector3<double>::normalize( C - A );

			push_tri_strip_point( geo_mesh->m_vertex_array, v1, -tangent1, line_color );
			push_tri_strip_point( geo_mesh->m_vertex_array, v2,  tangent1, line_color );
			push_tri_strip_point( geo_mesh->m_vertex_array, v2,  -tangent3, line_color );
			push_tri_strip_point( geo_mesh->m_vertex_array, v2,  tangent3, line_color );
			push_tri_strip_point( geo_mesh->m_vertex_array, v2, -tangent2, line_color );
			push_tri_strip_point( geo_mesh->m_vertex_array, v2,  tangent2, line_color );
		}

		{
			vector3<double> A = in_vecs_cart[n_vector_count - 2];
			vector3<double> B = in_vecs_cart[n_vector_count - 1];
			vector3<double> normal = vector3<double>::normalize(vector3<double>::cross( A, B ));

			vector3<double> v1,v2;
			v1 = B;
			v2 = B;

			vector3<double> tangent =  vector3<double>::normalize(B - A);


			push_tri_strip_point( geo_mesh->m_vertex_array, v1, -tangent, line_color );
			push_tri_strip_point( geo_mesh->m_vertex_array, v2,  tangent, line_color );
			push_tri_strip_point( geo_mesh->m_vertex_array, v2,  tangent, line_color );
		}
		
	}

	//////////////////////////////////////////////////////////////////////////////////////////////
	// @brief  创建片状的矢量线
	//
	// @param  geo_mesh		[out]	局部坐标系中的模型
	// @param  in_vecs_geo  [in]	Geocentric坐标下的点集
	// @param  v_mat_geo2cart [in]  Geocentric坐标转局部坐标矩阵
	// @param  pixel_width	[in]	矢量线像素宽度
	// @param  line_color	[in]	矢量线颜色
	//////////////////////////////////////////////////////////////////////////////////////////////
	void render_object_line_factory::create_render_object_line_flake_geo( OUT render_object_line_flake* geo_mesh, IN vector3<double>* in_vecs_geo, IN ulong v_vecs_num, IN matrix_4d& v_mat_geo2cart, IN double pixel_width, IN ulong line_color )
	{
		geo_mesh->set_line_width(pixel_width);

		int n_vector_count = v_vecs_num;
		if ( n_vector_count < 2 )
		{
			return;
		}

		if ( n_vector_count == 2)
		{
			vector3<double> A = in_vecs_geo[0];
			vector3<double> B = in_vecs_geo[1];
			vector3<double> normal = vector3<double>::normalize(vector3<double>::cross( A, B ));

			vector3<double> v1,v2,v3,v4;

			v1 = A * v_mat_geo2cart;
			v2 = v1;
			v3 = B * v_mat_geo2cart;
			v4 = v3;

			vector3<double> tangent =  vector3<double>::normalize(B - A);

			push_tri_strip_point( geo_mesh->m_vertex_array, v1, -tangent, line_color );
			push_tri_strip_point( geo_mesh->m_vertex_array, v1, -tangent, line_color );

			push_tri_strip_point( geo_mesh->m_vertex_array, v2,  tangent, line_color );
			push_tri_strip_point( geo_mesh->m_vertex_array, v3, -tangent, line_color );

			push_tri_strip_point( geo_mesh->m_vertex_array, v4,  tangent, line_color );
			push_tri_strip_point( geo_mesh->m_vertex_array, v4,  tangent, line_color );

			return;
		}

		// n_vector_count >= 3
		{
			vector3<double> A = in_vecs_geo[0];
			vector3<double> B = in_vecs_geo[1];
			vector3<double> normal = vector3<double>::normalize(vector3<double>::cross( A, B ));

			vector3<double> v1,v2;
			v1 = A * v_mat_geo2cart;
			v2 = v1;

			vector3<double> tangent =  vector3<double>::normalize(B - A);

			push_tri_strip_point( geo_mesh->m_vertex_array, v1, -tangent, line_color );
			push_tri_strip_point( geo_mesh->m_vertex_array, v1, -tangent, line_color );
			push_tri_strip_point( geo_mesh->m_vertex_array, v2,  tangent, line_color );
		}			


		for ( int ni = 1; ni< n_vector_count - 1; ni++ )
		{
			vector3<double> A  = in_vecs_geo[ni - 1];
			vector3<double> B  = in_vecs_geo[ni];
			vector3<double> C  = in_vecs_geo[ni + 1];

			// normal
			vector3<double> normal1 = vector3<double>::normalize(vector3<double>::cross( A, B ));
			vector3<double> normal2 = vector3<double>::normalize(vector3<double>::cross( B, C ));
			vector3<double> normal  = vector3<double>::normalize( ( normal1 + normal2 ) * 0.5 );
			vector3<double> v_temp_normal = vector3<double>::normalize( A - B );
			double dot_val = vector3<double>::dot( v_temp_normal, normal );
			if ( dot_val < 0.0 )
			{
				dot_val = vector3<double>::dot( v_temp_normal, -normal );
			}

			double v_width_scale;

			if ( math<double>::acos_( dot_val ) < 1 * RADIAN ) 
			{
				v_width_scale = 1.0;
			}
			else
			{
				v_width_scale = 1.0 / math<double>::sin_( math<double>::acos_( dot_val ) );
			}

			vector3<double> v1,v2;
			v1 = B * v_mat_geo2cart;
			v2 = v1;

			vector3<double> tangent1 =  vector3<double>::normalize( B - A );
			vector3<double> tangent2 =  vector3<double>::normalize( C - B );
			vector3<double> tangent3 =  vector3<double>::normalize( C - A );

			push_tri_strip_point( geo_mesh->m_vertex_array, v1, -tangent1, line_color );
			push_tri_strip_point( geo_mesh->m_vertex_array, v2,  tangent1, line_color );
			push_tri_strip_point( geo_mesh->m_vertex_array, v2,  -tangent3, line_color );
			push_tri_strip_point( geo_mesh->m_vertex_array, v2,  tangent3, line_color );
			push_tri_strip_point( geo_mesh->m_vertex_array, v2, -tangent2, line_color );
			push_tri_strip_point( geo_mesh->m_vertex_array, v2,  tangent2, line_color );
		}

		{
			vector3<double> A = in_vecs_geo[n_vector_count - 2];
			vector3<double> B = in_vecs_geo[n_vector_count - 1];
			vector3<double> normal = vector3<double>::normalize(vector3<double>::cross( A, B ));

			vector3<double> v1,v2;
			v1 = B * v_mat_geo2cart;
			v2 = v1;

			vector3<double> tangent =  vector3<double>::normalize(B - A);


			push_tri_strip_point( geo_mesh->m_vertex_array, v1, -tangent, line_color );
			push_tri_strip_point( geo_mesh->m_vertex_array, v2,  tangent, line_color );
			push_tri_strip_point( geo_mesh->m_vertex_array, v2,  tangent, line_color );
		}
	}

	void render_object_line_factory::create_render_object_multiple_line_flake_geo( render_object_line_flake* geo_mesh, IN vector3<double>* v_vecs_geo, IN ulong v_vecs_num, IN ulong* v_part_info, IN ulong v_part_num, IN matrix_4d& v_mat_geo2cart, IN double pixel_width, IN ulong line_color )
	{
		if ( v_vecs_num <= 1 || v_part_num == 0 ) return;

		geo_mesh->clear();
		ulong cur_index = 0;
		for ( int i = 0; i < (int)v_part_num; i++ )
		{
			if( v_part_info[i] <= 1 )
			{
				cur_index += v_part_info[i];
				continue;
			}

			create_render_object_line_flake_geo( geo_mesh, &v_vecs_geo[cur_index], v_part_info[i], v_mat_geo2cart, pixel_width, line_color );

			cur_index += v_part_info[i];
		}

		//geo_mesh->m_center = geo_mesh->m_render_aabb.center();

		//for ( int i = 0; i < (int)v_array.size(); i ++ )
		//{
		//	position_color pc;
		//	pc.x = v_array[i].x - geo_mesh->m_center.x;
		//	pc.y = v_array[i].y - geo_mesh->m_center.y;
		//	pc.z = v_array[i].z - geo_mesh->m_center.z;
		//	pc.color = v_line_color;
		//	geo_mesh->m_vertex_array.push_back( pc );
		//}

		//matrix_4d v_collapse_mat = matrix4<double>::translation( geo_mesh->m_center.x, geo_mesh->m_center.y, geo_mesh->m_center.z );
		//geo_mesh->set_collapse( v_collapse_mat );
	}


	//////////////////////////////////////////////////////////////////////////////////////////////
	// @brief  创建体状的矢量线
	//
	// @param  geo_mesh		[out]	局部坐标系中的模型
	// @param  in_vecs_cart [in]	局部坐标下的点集
	// @param  pixel_width	[in]	矢量线宽度
	// @param  line_color	[in]	矢量线颜色
	// @param  upper_height		[in]	矢量线的高度
	// @param  lower_height	[in]	矢量线最低点高度
	//////////////////////////////////////////////////////////////////////////////////////////////
	void render_object_line_factory::create_render_object_line_volume_cart( render_object_line_volume* geo_mesh, IN coordinates_array& in_vecs_cart, IN double pixel_width, IN ulong line_color, IN double upper_height, IN double lower_height )
	{
		geo_mesh->set_line_width(pixel_width);
		geo_mesh->set_shadow_color(line_color);
		
		int v_size = in_vecs_cart.size();	
		if ( v_size < 2 ) return;

		insert_index_to_line_mesh(geo_mesh, v_size);
		
		std::vector<position_normal_color> v_upper_coords_array;
		std::vector<position_normal_color> v_lower_coords_array;
		
		// 两个点的情况
		if ( v_size == 2 )
		{	
			vector3<double> pos_geo1 = in_vecs_cart[0];
			vector3<double> pos_geo2 = in_vecs_cart[1];
			vector3<double> normal = vector3<double>::cross( pos_geo1, pos_geo2 );
			normal.normalize();			

			// 第一个点
			vector3<double> pos_geo1_scaled = vector3<double>::normalize(pos_geo1) * upper_height;
			push_tri_strip_point( v_upper_coords_array, pos_geo1_scaled, -normal, line_color );
			push_tri_strip_point( v_upper_coords_array, pos_geo1_scaled,  normal, line_color );

			pos_geo1_scaled = vector3<double>::normalize(pos_geo1) * lower_height;
			push_tri_strip_point( v_lower_coords_array, pos_geo1_scaled, -normal, line_color );
			push_tri_strip_point( v_lower_coords_array, pos_geo1_scaled,  normal, line_color );

			// 第二个点
			vector3<double> pos_geo2_scaled = vector3<double>::normalize(pos_geo2) * upper_height;
			push_tri_strip_point( v_upper_coords_array, pos_geo2_scaled, -normal, line_color );
			push_tri_strip_point( v_upper_coords_array, pos_geo2_scaled,  normal, line_color );

			pos_geo2_scaled = vector3<double>::normalize(pos_geo2) * lower_height;
			push_tri_strip_point( v_lower_coords_array, pos_geo2_scaled, -normal, line_color );
			push_tri_strip_point( v_lower_coords_array, pos_geo2_scaled,  normal, line_color );
		}
		else
		{
			// 三个点以上的情况
			// 第一个点
			{
				vector3<double> pos_geo1 = in_vecs_cart[0];
				vector3<double> pos_geo2 = in_vecs_cart[1];
				vector3<double> normal = vector3<double>::cross( pos_geo1, pos_geo2 );
				normal.normalize();

				vector3<double> pos_geo1_scaled = vector3<double>::normalize(pos_geo1) * upper_height;
				push_tri_strip_point( v_upper_coords_array, pos_geo1_scaled, -normal, line_color );
				push_tri_strip_point( v_upper_coords_array, pos_geo1_scaled,  normal, line_color );

				pos_geo1_scaled = vector3<double>::normalize(pos_geo1) * lower_height;
				push_tri_strip_point( v_lower_coords_array, pos_geo1_scaled, -normal, line_color );
				push_tri_strip_point( v_lower_coords_array, pos_geo1_scaled,  normal, line_color );
			}	

			for( int i = 1; i < int(v_size - 1); i ++ )
			{
				vector3<double> pos_geo1 = in_vecs_cart[i-1];
				vector3<double> pos_geo2 = in_vecs_cart[i];
				vector3<double> pos_geo3 = in_vecs_cart[i+1];

				// direction
				vector3<double> normal1 = vector3<double>::cross( pos_geo1, pos_geo2 );
				normal1.normalize();

				vector3<double> normal2 = vector3<double>::cross( pos_geo2, pos_geo3 );
				normal2.normalize();

				vector3<double> normal = ( normal1 + normal2 ) * 0.5;
				normal.normalize();

				vector3<double> v_temp_normal = vector3<double>::normalize( pos_geo1 - pos_geo2 );
				double dot_val = vector3<double>::dot( v_temp_normal, normal );
				if ( dot_val < 0.0 )
				{
					dot_val = vector3<double>::dot( v_temp_normal, -normal );
				}

				double v_width_scale;

				if ( math<double>::acos_( dot_val ) < 1 * RADIAN ) 
				{
					v_width_scale = 1.0;
				}
				else
				{
					v_width_scale = 1.0 / math<double>::sin_( math<double>::acos_( dot_val ) );
				}

				vector3<double> pos_geo2_scaled = vector3<double>::normalize(pos_geo2) * upper_height;
				push_tri_strip_point( v_upper_coords_array, pos_geo2_scaled, -normal * v_width_scale, line_color );
				push_tri_strip_point( v_upper_coords_array, pos_geo2_scaled,  normal * v_width_scale, line_color );

				pos_geo2_scaled = vector3<double>::normalize(pos_geo2) * lower_height;
				push_tri_strip_point( v_lower_coords_array, pos_geo2_scaled, -normal * v_width_scale, line_color );
				push_tri_strip_point( v_lower_coords_array, pos_geo2_scaled,  normal * v_width_scale, line_color );
			}

			// 最后一个点
			{
				vector3<double> pos_geo1 = in_vecs_cart[v_size - 2];
				vector3<double> pos_geo2 = in_vecs_cart[v_size - 1];

				vector3<double> normal = vector3<double>::cross( pos_geo1, pos_geo2 );
				normal.normalize();

				vector3<double> pos_geo2_scaled = vector3<double>::normalize(pos_geo2) * upper_height;
				push_tri_strip_point( v_upper_coords_array, pos_geo2_scaled, -normal, line_color );
				push_tri_strip_point( v_upper_coords_array, pos_geo2_scaled,  normal, line_color );


				pos_geo2_scaled = vector3<double>::normalize(pos_geo2) * lower_height;
				push_tri_strip_point( v_lower_coords_array, pos_geo2_scaled, -normal, line_color );
				push_tri_strip_point( v_lower_coords_array, pos_geo2_scaled,  normal, line_color );

			}

		}

		geo_mesh->m_vertex_array.insert( geo_mesh->m_vertex_array.end(), v_upper_coords_array.begin(), v_upper_coords_array.end() );
		geo_mesh->m_vertex_array.insert( geo_mesh->m_vertex_array.end(), v_lower_coords_array.begin(), v_lower_coords_array.end() );

		
	}

	//////////////////////////////////////////////////////////////////////////////////////////////
	// @brief  创建体状的矢量线
	//
	// @param  geo_mesh		[out]	局部坐标系中的模型
	// @param  in_vecs_geo  [in]	Geocentric坐标下的点集
	// @param  v_mat_geo2cart [in]  Geocentric坐标转局部坐标矩阵
	// @param  pixel_width	[in]	矢量线宽度
	// @param  line_color	[in]	矢量线颜色
	// @param  upper_height		[in]	矢量线的高度
	// @param  lower_height	[in]	矢量线最低点高度
	//////////////////////////////////////////////////////////////////////////////////////////////
	void render_object_line_factory::create_render_object_line_volume_geo( render_object_line_volume* geo_mesh, IN vector3<double>* in_vecs_geo, IN ulong v_vecs_num, IN matrix_4d& v_mat_geo2cart,  IN double pixel_width, IN ulong line_color, IN double upper_height, IN double lower_height )
	{
		geo_mesh->set_line_width(pixel_width);
		geo_mesh->set_shadow_color(line_color);
		
		int v_size = v_vecs_num;	
		if ( v_size < 2 ) return;

		insert_index_to_line_mesh(geo_mesh, v_size);
		

		std::vector<position_normal_color> v_upper_coords_array;
		std::vector<position_normal_color> v_lower_coords_array;
		
		// 两个点的情况
		if ( v_size == 2 )
		{	
			

			vector3<double> pos_geo1 = in_vecs_geo[0];
			vector3<double> pos_geo2 = in_vecs_geo[1];
			vector3<double> normal = vector3<double>::cross( pos_geo1, pos_geo2 );
			normal.normalize();

			normal = matrix_4d::rotate_normal( normal, v_mat_geo2cart );
			
			// 第一个点
			vector3<double> pos_geo1_scaled = ( vector3<double>::normalize(pos_geo1) * upper_height ) * v_mat_geo2cart ;
			push_tri_strip_point( v_upper_coords_array, pos_geo1_scaled, -normal, line_color );
			push_tri_strip_point( v_upper_coords_array, pos_geo1_scaled,  normal, line_color );

			pos_geo1_scaled = ( vector3<double>::normalize(pos_geo1) * lower_height ) * v_mat_geo2cart ;
			push_tri_strip_point( v_lower_coords_array, pos_geo1_scaled, -normal, line_color );
			push_tri_strip_point( v_lower_coords_array, pos_geo1_scaled,  normal, line_color );

			// 第二个点
			vector3<double> pos_geo2_scaled = ( vector3<double>::normalize(pos_geo2) * upper_height ) * v_mat_geo2cart ;
			push_tri_strip_point( v_upper_coords_array, pos_geo2_scaled, -normal, line_color );
			push_tri_strip_point( v_upper_coords_array, pos_geo2_scaled,  normal, line_color );

			pos_geo2_scaled = ( vector3<double>::normalize(pos_geo2) * lower_height ) * v_mat_geo2cart ;
			push_tri_strip_point( v_lower_coords_array, pos_geo2_scaled, -normal, line_color );
			push_tri_strip_point( v_lower_coords_array, pos_geo2_scaled,  normal, line_color );
		}
		else
		{
			// 三个点以上的情况
			// 第一个点
			{
				vector3<double> pos_geo1 = in_vecs_geo[0];
				vector3<double> pos_geo2 = in_vecs_geo[1];
				vector3<double> normal = vector3<double>::cross( pos_geo1, pos_geo2 );
				normal.normalize();
				normal = matrix_4d::rotate_normal( normal, v_mat_geo2cart );
				
				vector3<double> pos_geo1_scaled = ( vector3<double>::normalize(pos_geo1) * upper_height ) * v_mat_geo2cart;
				push_tri_strip_point( v_upper_coords_array, pos_geo1_scaled, -normal, line_color );
				push_tri_strip_point( v_upper_coords_array, pos_geo1_scaled,  normal, line_color );

				pos_geo1_scaled = ( vector3<double>::normalize(pos_geo1) * lower_height ) * v_mat_geo2cart ;
				push_tri_strip_point( v_lower_coords_array, pos_geo1_scaled, -normal, line_color );
				push_tri_strip_point( v_lower_coords_array, pos_geo1_scaled,  normal, line_color );
			}	

			for( int i = 1; i < int(v_size - 1); i ++ )
			{
				vector3<double> pos_geo1 = in_vecs_geo[i-1];
				vector3<double> pos_geo2 = in_vecs_geo[i];
				vector3<double> pos_geo3 = in_vecs_geo[i+1];

				// direction
				vector3<double> normal1 = vector3<double>::cross( pos_geo1, pos_geo2 );
				normal1.normalize();

				vector3<double> normal2 = vector3<double>::cross( pos_geo2, pos_geo3 );
				normal2.normalize();

				vector3<double> normal = ( normal1 + normal2 ) * 0.5;
				normal.normalize();

				vector3<double> v_temp_normal = vector3<double>::normalize( pos_geo1 - pos_geo2 );
				double dot_val = vector3<double>::dot( v_temp_normal, normal );
				if ( dot_val < 0.0 )
				{
					dot_val = vector3<double>::dot( v_temp_normal, -normal );
				}

				double v_width_scale;

				if ( math<double>::acos_( dot_val ) < 1 * RADIAN ) 
				{
					v_width_scale = 1.0;
				}
				else
				{
					v_width_scale = 1.0 / math<double>::sin_( math<double>::acos_( dot_val ) );
				}

				normal = normal * v_width_scale;
				normal = matrix_4d::rotate_normal( normal, v_mat_geo2cart );
				
				vector3<double> pos_geo2_scaled = ( vector3<double>::normalize(pos_geo2) * upper_height ) * v_mat_geo2cart;
				push_tri_strip_point( v_upper_coords_array, pos_geo2_scaled, -normal, line_color );
				push_tri_strip_point( v_upper_coords_array, pos_geo2_scaled,  normal, line_color );

				pos_geo2_scaled = ( vector3<double>::normalize(pos_geo2) * lower_height ) * v_mat_geo2cart;
				push_tri_strip_point( v_lower_coords_array, pos_geo2_scaled, -normal, line_color );
				push_tri_strip_point( v_lower_coords_array, pos_geo2_scaled,  normal, line_color );
			}

			// 最后一个点
			{
				vector3<double> pos_geo1 = in_vecs_geo[v_size - 2];
				vector3<double> pos_geo2 = in_vecs_geo[v_size - 1];

				vector3<double> normal = vector3<double>::cross( pos_geo1, pos_geo2 );
				normal.normalize();
				normal = matrix_4d::rotate_normal( normal, v_mat_geo2cart );
				
				vector3<double> pos_geo2_scaled = ( vector3<double>::normalize(pos_geo2) * upper_height ) * v_mat_geo2cart;
				push_tri_strip_point( v_upper_coords_array, pos_geo2_scaled, -normal, line_color );
				push_tri_strip_point( v_upper_coords_array, pos_geo2_scaled,  normal, line_color );


				pos_geo2_scaled = ( vector3<double>::normalize(pos_geo2) * lower_height ) * v_mat_geo2cart;
				push_tri_strip_point( v_lower_coords_array, pos_geo2_scaled, -normal, line_color );
				push_tri_strip_point( v_lower_coords_array, pos_geo2_scaled,  normal, line_color );

			}

		}

		geo_mesh->m_vertex_array.insert( geo_mesh->m_vertex_array.end(), v_upper_coords_array.begin(), v_upper_coords_array.end() );
		geo_mesh->m_vertex_array.insert( geo_mesh->m_vertex_array.end(), v_lower_coords_array.begin(), v_lower_coords_array.end() );

		
	}

	//////////////////////////////////////////////////////////////////////////////////////////////
	// @brief  创建固定像素宽度的贴地矢量线，针对VectorLayer
	//
	// @param  geo_mesh		[out]	局部坐标系中的模型
	// @param  v_vecs_geo   [in]	Geocentric坐标下的点集
	// @param  v_vecs_num   [in]    传入的点个数
	// @param  v_part_info  [in]    分段信息
	// @param  v_part_num   [in]    分段个数
	// @param  v_width_geo	[in]	矢量线地理宽度
	// @param  v_line_color	[in]	矢量线颜色
	// @param  v_upper_height [in]	矢量线的高度
	// @param  v_lower_height [in]	矢量线最低点高度
	//////////////////////////////////////////////////////////////////////////////////////////////
	void render_object_line_factory::create_render_object_multiple_line_volume_geo( render_object_line_volume* geo_mesh, IN vector3<double>* v_vecs_geo, IN ulong v_vecs_num, IN ulong* v_part_info, IN ulong v_part_num, IN matrix_4d& v_mat_geo2cart, IN double pixel_width, IN ulong line_color, IN double upper_height, IN double lower_height )
	{
		if ( v_vecs_num <= 1 || v_part_num == 0 ) return;

		geo_mesh->clear();
		ulong cur_index = 0;
		for ( int i = 0; i < (int)v_part_num; i++ )
		{
			if( v_part_info[i] <= 1 )
			{
				cur_index += v_part_info[i];
				continue;
			}

			create_render_object_line_volume_geo( geo_mesh, &v_vecs_geo[cur_index], v_part_info[i], v_mat_geo2cart, pixel_width, line_color, upper_height, lower_height );

			cur_index += v_part_info[i];
		}

		//geo_mesh->m_center = geo_mesh->m_render_aabb.center();

		//for ( int i = 0; i < (int)v_array.size(); i ++ )
		//{
		//	position_color pc;
		//	pc.x = v_array[i].x - geo_mesh->m_center.x;
		//	pc.y = v_array[i].y - geo_mesh->m_center.y;
		//	pc.z = v_array[i].z - geo_mesh->m_center.z;
		//	pc.color = v_line_color;
		//	geo_mesh->m_vertex_array.push_back( pc );
		//}

		//matrix_4d v_collapse_mat = matrix4<double>::translation( geo_mesh->m_center.x, geo_mesh->m_center.y, geo_mesh->m_center.z );
		//geo_mesh->set_collapse( v_collapse_mat );
	}

	//////////////////////////////////////////////////////////////////////////////////////////////
	// @brief  创建贴地的矢量线
	//
	// @param  geo_mesh		[out]	局部坐标系中的模型
	// @param  v_vecs_geo   [in]	Geocentric坐标下的点集
	// @param  v_vecs_num   [in]    传入的点个数
	// @param  v_width_geo	[in]	矢量线地理宽度
	// @param  v_line_color	[in]	矢量线颜色
	// @param  v_upper_height [in]	矢量线的高度
	// @param  v_lower_height [in]	矢量线最低点高度
	//////////////////////////////////////////////////////////////////////////////////////////////
	void render_object_line_factory::create_stencil_mesh_from_line_geo( render_object_geometry_mesh* geo_mesh, vector_3d* v_vecs_geo, int v_vecs_num, double v_width_geo, ulong v_line_color, double v_upper_height, double v_lower_height )
	{
		if ( v_vecs_num <= 1 ) return;

		geo_mesh->clear();

		coordinates_array v_array;
		generate_line_stencil_mesh( geo_mesh, v_vecs_geo, v_vecs_num, v_array, v_width_geo, v_line_color, v_upper_height, v_lower_height - 0.01 );

		geo_mesh->m_center = geo_mesh->m_render_aabb.center();
		for ( int i = 0; i < (int)v_array.size(); i ++ )
		{
			position_color pc;
			pc.x = v_array[i].x - geo_mesh->m_center.x;
			pc.y = v_array[i].y - geo_mesh->m_center.y;
			pc.z = v_array[i].z - geo_mesh->m_center.z;
			pc.color = v_line_color;
			geo_mesh->m_vertex_array.push_back( pc );
		}

		matrix_4d v_collapse_mat = matrix4<double>::translation( geo_mesh->m_center.x, geo_mesh->m_center.y, geo_mesh->m_center.z );
		geo_mesh->set_collapse( v_collapse_mat );
	}

	//////////////////////////////////////////////////////////////////////////////////////////////
	// @brief  创建贴地的矢量线，针对VectorLayer
	//
	// @param  geo_mesh		[out]	局部坐标系中的模型
	// @param  v_vecs_geo   [in]	Geocentric坐标下的点集
	// @param  v_vecs_num   [in]    传入的点个数
	// @param  v_part_info  [in]    分段信息
	// @param  v_part_num   [in]    分段个数
	// @param  v_width_geo	[in]	矢量线地理宽度
	// @param  v_line_color	[in]	矢量线颜色
	// @param  v_upper_height [in]	矢量线的高度
	// @param  v_lower_height [in]	矢量线最低点高度
	//////////////////////////////////////////////////////////////////////////////////////////////
	void render_object_line_factory::create_stencil_mesh_from_multiple_line_geo( render_object_geometry_mesh* geo_mesh,  vector3<double>* v_vecs_geo, ulong v_vecs_num, ulong* v_part_info, ulong v_part_num, double v_width_geo, ulong v_line_color, double v_upper_height, double v_lower_height )
	{
		if ( v_vecs_num <= 1 || v_part_num == 0 )
			return;

		geo_mesh->clear();

		coordinates_array v_array;

		ulong cur_index = 0;
		for ( int i = 0; i < (int)v_part_num; i++ )
		{
			if( v_part_info[i] <= 1 )
			{
				cur_index += v_part_info[i];
				continue;
			}

			generate_line_stencil_mesh( geo_mesh, &v_vecs_geo[cur_index], v_part_info[i], v_array, v_width_geo, v_line_color, v_upper_height, v_lower_height - 0.01 );

			cur_index += v_part_info[i];
		}

		geo_mesh->m_center = geo_mesh->m_render_aabb.center();

		for ( int i = 0; i < (int)v_array.size(); i ++ )
		{
			position_color pc;
			pc.x = v_array[i].x - geo_mesh->m_center.x;
			pc.y = v_array[i].y - geo_mesh->m_center.y;
			pc.z = v_array[i].z - geo_mesh->m_center.z;
			pc.color = v_line_color;
			geo_mesh->m_vertex_array.push_back( pc );
		}

		matrix_4d v_collapse_mat = matrix4<double>::translation( geo_mesh->m_center.x, geo_mesh->m_center.y, geo_mesh->m_center.z );
		geo_mesh->set_collapse( v_collapse_mat );

	}

	//////////////////////////////////////////////////////////////////////////////////////////////
	// @brief  创建贴地的矢量线，针对VectorLayer
	//
	// @param  geo_mesh		[out]	局部坐标系中的模型
	// @param  v_coord_geo  [in]	Geocentric坐标下的点集
	// @param  v_mat_geo2cart [in]  Geocentric到Cartesian坐标变换
	// @param  v_width_geo	[in]	矢量线地理宽度
	// @param  v_line_color	[in]	矢量线颜色
	// @param  v_upper_height [in]	矢量线的高度
	// @param  v_lower_height [in]	矢量线最低点高度
	//////////////////////////////////////////////////////////////////////////////////////////////
	void render_object_line_factory::create_stencil_mesh_from_line_geo( render_object_geometry_mesh* geo_mesh,
		coordinates_array& v_coord_geo, matrix_4d& v_mat_geo2cart, double v_width_geo, ulong v_line_color, double v_upper_height, double v_lower_height )
	{
		if ( v_coord_geo.size() <= 1 ) return;

		geo_mesh->clear();

		coordinates_array v_array;
		generate_line_stencil_mesh( geo_mesh, &v_coord_geo[0], v_coord_geo.size(), v_array, v_width_geo, v_line_color, v_upper_height, v_lower_height - 0.01 );

		geo_mesh->m_center = geo_mesh->m_render_aabb.center();
		for ( int i = 0; i < (int)v_array.size(); i ++ )
		{
			v_array[i] = v_array[i] * v_mat_geo2cart;

			position_color pc;
			pc.x = v_array[i].x;
			pc.y = v_array[i].y;
			pc.z = v_array[i].z;
			pc.color = v_line_color;
			geo_mesh->m_vertex_array.push_back( pc );
		}
	}


}