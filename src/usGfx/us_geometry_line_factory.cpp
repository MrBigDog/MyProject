///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_geometry_line_factory.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : geometry_line_factory class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_geometry_line_factory.h"

namespace uniscope_globe
{
	//////////////////////////////////////////////////////////////////////////////////////////////
	// @brief  创建矢量线
	//
	// @param  geo_mesh		[out]	RP坐标系中的模型
	// @param  in_vec_array [in]	Geocentric坐标下的点集
	// @param  center_geo	[in]	Geocentric坐标系下的点集的中心点位置
	// @param  line_color	[in]	矢量线颜色
	//////////////////////////////////////////////////////////////////////////////////////////////
	void geometry_line_factory::create_geometry_line( OUT geometry_line* geo_mesh, IN coordinates_array& in_vec_array, IN vector_3d& center_geo, IN ulong line_color )
	{
		int v_size = (int)in_vec_array.size();
		geo_mesh->m_vertex_array.clear();
		geo_mesh->m_vertex_array.resize( v_size );

		for( int ni = 0; ni < v_size; ni++ )
		{
			geo_mesh->m_vertex_array[ni].x = in_vec_array[ni].x - center_geo.x;
			geo_mesh->m_vertex_array[ni].y = in_vec_array[ni].y - center_geo.y;
			geo_mesh->m_vertex_array[ni].z = in_vec_array[ni].z - center_geo.z;
			geo_mesh->m_vertex_array[ni].color = line_color;
		}

		geo_mesh->set_collapse( matrix_4d::translation( center_geo.x, center_geo.y, center_geo.z ) );
	}

	//////////////////////////////////////////////////////////////////////////////////////////////
	// @brief  创建片状的矢量线
	//
	// @param  geo_mesh		[out]	RP坐标系中的模型
	// @param  in_vec_array [in]	Geocentric坐标下的点集
	// @param  center_geo	[in]	Geocentric坐标系下的点集的中心点位置
	// @param  line_width	[in]	矢量线宽度
	// @param  line_color	[in]	矢量线颜色
	//////////////////////////////////////////////////////////////////////////////////////////////
	void geometry_line_factory::create_geometry_line_flake( OUT geometry_line_flake* geo_mesh, IN coordinates_array& in_vec_array, IN vector_3d& center_geo, IN double line_width, IN ulong line_color )
	{
		geo_mesh->set_line_width(line_width);
		geo_mesh->set_collapse_matrix( matrix_4d::translation( center_geo.x, center_geo.y, center_geo.z ) );	

		line_width = 0;
		int n_vector_count = in_vec_array.size();
		if ( n_vector_count < 2 )
		{
			return;
		}

		if ( n_vector_count == 2)
		{
			vector3<double> A = in_vec_array[0];
			vector3<double> B = in_vec_array[1];
			vector3<double> normal = vector3<double>::normalize(vector3<double>::cross( A, B ));

			vector3<double> v1,v2,v3,v4;

			v1 = A - normal * line_width;
			v2 = A + normal * line_width;
			v3 = B - normal * line_width;
			v4 = B + normal * line_width;

			vector3<double> tangent =  vector3<double>::normalize(B - A);

			push_tri_strip_point( geo_mesh->m_vertex_array, v1 - center_geo, -tangent, line_color );
			push_tri_strip_point( geo_mesh->m_vertex_array, v1 - center_geo, -tangent, line_color );

			push_tri_strip_point( geo_mesh->m_vertex_array, v2 - center_geo,  tangent, line_color );
			push_tri_strip_point( geo_mesh->m_vertex_array, v3 - center_geo, -tangent, line_color );

			push_tri_strip_point( geo_mesh->m_vertex_array, v4 - center_geo,  tangent, line_color );
			push_tri_strip_point( geo_mesh->m_vertex_array, v4 - center_geo,  tangent, line_color );

			return;
		}

		// n_vector_count >= 3
		{
			vector3<double> A = in_vec_array[0];
			vector3<double> B = in_vec_array[1];
			vector3<double> normal = vector3<double>::normalize(vector3<double>::cross( A, B ));

			vector3<double> v1,v2;
			v1 = A - normal * line_width;
			v2 = A + normal * line_width;

			vector3<double> tangent =  vector3<double>::normalize(B - A);

			push_tri_strip_point( geo_mesh->m_vertex_array, v1 - center_geo, -tangent, line_color );
			push_tri_strip_point( geo_mesh->m_vertex_array, v1 - center_geo, -tangent, line_color );
			push_tri_strip_point( geo_mesh->m_vertex_array, v2 - center_geo,  tangent, line_color );
		}			


		for ( int ni = 1; ni< n_vector_count - 1; ni++ )
		{
			vector3<double> A  = in_vec_array[ni - 1];
			vector3<double> B  = in_vec_array[ni];
			vector3<double> C  = in_vec_array[ni + 1];

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
			v1 = B - normal * v_width_scale * line_width;
			v2 = B + normal * v_width_scale * line_width;

			vector3<double> tangent1 =  vector3<double>::normalize( B - A );
			vector3<double> tangent2 =  vector3<double>::normalize( C - B );
			vector3<double> tangent3 =  vector3<double>::normalize( C - A );

			//push_tri_strip_point( geo_mesh->m_vertex_array, v1 - center_geo, -tangent1, line_color );
			//push_tri_strip_point( geo_mesh->m_vertex_array, v2 - center_geo,  tangent1, line_color );
			//push_tri_strip_point( geo_mesh->m_vertex_array, v2 - center_geo,  tangent1, line_color );
			//push_tri_strip_point( geo_mesh->m_vertex_array, v1 - center_geo, -tangent2, line_color );
			//push_tri_strip_point( geo_mesh->m_vertex_array, v1 - center_geo, -tangent2, line_color );
			//push_tri_strip_point( geo_mesh->m_vertex_array, v2 - center_geo,  tangent2, line_color );

			push_tri_strip_point( geo_mesh->m_vertex_array, v1 - center_geo, -tangent1, line_color );
			push_tri_strip_point( geo_mesh->m_vertex_array, v2 - center_geo,  tangent1, line_color );
			push_tri_strip_point( geo_mesh->m_vertex_array, v2 - center_geo,  -tangent3, line_color );
			push_tri_strip_point( geo_mesh->m_vertex_array, v2 - center_geo,  tangent3, line_color );
			push_tri_strip_point( geo_mesh->m_vertex_array, v2 - center_geo, -tangent2, line_color );
			push_tri_strip_point( geo_mesh->m_vertex_array, v2 - center_geo,  tangent2, line_color );

			//push_tri_strip_point( geo_mesh->m_vertex_array, v2 - center_geo,  tangent1, line_color );
			//push_tri_strip_point( geo_mesh->m_vertex_array, v1 - center_geo, -tangent2, line_color );
			//push_tri_strip_point( geo_mesh->m_vertex_array, v1 - center_geo, -tangent2, line_color );
			//push_tri_strip_point( geo_mesh->m_vertex_array, v2 - center_geo,  tangent2, line_color );

		}

		{
			vector3<double> A = in_vec_array[n_vector_count - 2];
			vector3<double> B = in_vec_array[n_vector_count - 1];
			vector3<double> normal = vector3<double>::normalize(vector3<double>::cross( A, B ));

			vector3<double> v1,v2;
			v1 = B - normal * line_width;
			v2 = B + normal * line_width;

			vector3<double> tangent =  vector3<double>::normalize(B - A);


			push_tri_strip_point( geo_mesh->m_vertex_array, v1 - center_geo, -tangent, line_color );
			push_tri_strip_point( geo_mesh->m_vertex_array, v2 - center_geo,  tangent, line_color );
			push_tri_strip_point( geo_mesh->m_vertex_array, v2 - center_geo,  tangent, line_color );
		}
		
	}

	void geometry_line_factory::push_tri_strip_point( std::vector<position_normal_color>& triangle_list, vector3<double>& pt, vector3<double>& normal, ulong color )
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


	//////////////////////////////////////////////////////////////////////////////////////////////
	// @brief  创建体状的矢量线
	//
	// @param  geo_mesh		[out]	RP坐标系中的模型
	// @param  in_vec_array [in]	Geocentric坐标下的点集
	// @param  center_geo	[in]	Geocentric坐标系下的点集的中心点位置
	// @param  line_width	[in]	矢量线宽度
	// @param  line_color	[in]	矢量线颜色
	// @param  height		[in]	矢量线的高度
	// @param  lower_height	[in]	矢量线最低点高度
	//////////////////////////////////////////////////////////////////////////////////////////////
	void geometry_line_factory::create_geometry_line_volume( geometry_line_volume* geo_mesh, IN coordinates_array& in_vec_array, IN vector_3d& center_geo, IN double line_width, IN ulong line_color, IN double upper_height, IN double lower_height )
	{
		geo_mesh->set_line_width(line_width);
		geo_mesh->set_shadow_color(line_color);
		geo_mesh->set_collapse_matrix( matrix_4d::translation( center_geo.x, center_geo.y, center_geo.z ) );	

		line_width = 0;

		int v_size = in_vec_array.size();	
		if ( v_size < 2 )
		{
			return;
		}

		std::vector<position_normal_color> v_upper_coords_array;
		std::vector<position_normal_color> v_lower_coords_array;
		double v_width = line_width;

		// 两个点的情况
		if ( v_size == 2 )
		{	
			vector3<double> pos_geo1 = in_vec_array[0];
			vector3<double> pos_geo2 = in_vec_array[1];
			vector3<double> normal = vector3<double>::cross( pos_geo1, pos_geo2 );
			normal.normalize();			

			// 第一个点
			vector3<double> pos_geo1_scaled = vector3<double>::normalize(pos_geo1) * upper_height;
			push_tri_strip_point( v_upper_coords_array, pos_geo1_scaled - normal * v_width - center_geo, -normal, line_color );
			push_tri_strip_point( v_upper_coords_array, pos_geo1_scaled + normal * v_width - center_geo,  normal, line_color );

			pos_geo1_scaled = vector3<double>::normalize(pos_geo1) * lower_height;
			push_tri_strip_point( v_lower_coords_array, pos_geo1_scaled - normal * v_width - center_geo, -normal, line_color );
			push_tri_strip_point( v_lower_coords_array, pos_geo1_scaled + normal * v_width - center_geo,  normal, line_color );

			// 第二个点
			vector3<double> pos_geo2_scaled = vector3<double>::normalize(pos_geo2) * upper_height;
			push_tri_strip_point( v_upper_coords_array, pos_geo2_scaled - normal * v_width - center_geo, -normal, line_color );
			push_tri_strip_point( v_upper_coords_array, pos_geo2_scaled + normal * v_width - center_geo,  normal, line_color );

			pos_geo2_scaled = vector3<double>::normalize(pos_geo2) * lower_height;
			push_tri_strip_point( v_lower_coords_array, pos_geo2_scaled - normal * v_width - center_geo, -normal, line_color );
			push_tri_strip_point( v_lower_coords_array, pos_geo2_scaled + normal * v_width - center_geo,  normal, line_color );
		}
		else
		{
			// 三个点以上的情况
			// 第一个点
			{
				vector3<double> pos_geo1 = in_vec_array[0];
				vector3<double> pos_geo2 = in_vec_array[1];
				vector3<double> normal = vector3<double>::cross( pos_geo1, pos_geo2 );
				normal.normalize();

				vector3<double> pos_geo1_scaled = vector3<double>::normalize(pos_geo1) * upper_height;
				push_tri_strip_point( v_upper_coords_array, pos_geo1_scaled - normal * v_width - center_geo, -normal, line_color );
				push_tri_strip_point( v_upper_coords_array, pos_geo1_scaled + normal * v_width - center_geo,  normal, line_color );

				pos_geo1_scaled = vector3<double>::normalize(pos_geo1) * lower_height;
				push_tri_strip_point( v_lower_coords_array, pos_geo1_scaled - normal * v_width - center_geo, -normal, line_color );
				push_tri_strip_point( v_lower_coords_array, pos_geo1_scaled + normal * v_width - center_geo,  normal, line_color );
			}	

			for( int i = 1; i < int(v_size - 1); i ++ )
			{
				vector3<double> pos_geo1 = in_vec_array[i-1];
				vector3<double> pos_geo2 = in_vec_array[i];
				vector3<double> pos_geo3 = in_vec_array[i+1];

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


				//double width;
				//if ( math<double>::acos_( dot_val ) < 1 * math_d::s_deg_to_rad ) 
				//{
				//	width = v_width;
				//}
				//else
				//{
				//	width = v_width / math<double>::sin_( math<double>::acos_( dot_val ) );
				//}

				vector3<double> pos_geo2_scaled = vector3<double>::normalize(pos_geo2) * upper_height;
				push_tri_strip_point( v_upper_coords_array, pos_geo2_scaled - normal * v_width * v_width_scale - center_geo, -normal * v_width_scale, line_color );
				push_tri_strip_point( v_upper_coords_array, pos_geo2_scaled + normal * v_width * v_width_scale - center_geo,  normal * v_width_scale, line_color );

				pos_geo2_scaled = vector3<double>::normalize(pos_geo2) * lower_height;
				push_tri_strip_point( v_lower_coords_array, pos_geo2_scaled - normal * v_width * v_width_scale - center_geo, -normal * v_width_scale, line_color );
				push_tri_strip_point( v_lower_coords_array, pos_geo2_scaled + normal * v_width * v_width_scale - center_geo,  normal * v_width_scale, line_color );
			}

			// 最后一个点
			{
				vector3<double> pos_geo1 = in_vec_array[v_size - 2];
				vector3<double> pos_geo2 = in_vec_array[v_size - 1];

				vector3<double> normal = vector3<double>::cross( pos_geo1, pos_geo2 );
				normal.normalize();

				vector3<double> pos_geo2_scaled = vector3<double>::normalize(pos_geo2) * upper_height;
				push_tri_strip_point( v_upper_coords_array, pos_geo2_scaled - normal * v_width - center_geo, -normal, line_color );
				push_tri_strip_point( v_upper_coords_array, pos_geo2_scaled + normal * v_width,  normal, line_color );


				pos_geo2_scaled = vector3<double>::normalize(pos_geo2) * lower_height;
				push_tri_strip_point( v_lower_coords_array, pos_geo2_scaled - normal * v_width - center_geo, -normal, line_color );
				push_tri_strip_point( v_lower_coords_array, pos_geo2_scaled + normal * v_width - center_geo,  normal, line_color );

			}

		}

		geo_mesh->m_vertex_array.insert( geo_mesh->m_vertex_array.end(), v_upper_coords_array.begin(), v_upper_coords_array.end() );
		geo_mesh->m_vertex_array.insert( geo_mesh->m_vertex_array.end(), v_lower_coords_array.begin(), v_lower_coords_array.end() );

		insert_index_to_line_mesh(geo_mesh, v_size);

		//v_vertex_array.insert( v_vertex_array.end(), v_upper_coords_array.begin(), v_upper_coords_array.end() );
		//v_vertex_array.insert( v_vertex_array.end(), v_lower_coords_array.begin(), v_lower_coords_array.end() );
		//insert_vertex_to_line_mesh( geo_mesh, v_size, v_vertex_array, out_array, line_color );	
	}


	void geometry_line_factory::insert_index_to_line_mesh( geometry_line_volume* v_mesh, ulong size )
	{
		ulong v_size = size<<1;
		ulong start_index = 0;

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

		//for ( int i = 0; i < (int)in_array.size(); i ++ )
		//{
		//	v_mesh->m_render_aabb.combine( in_array[i] );
		//}
		//v_mesh->m_center = v_mesh->m_render_aabb.center();

		//for ( int i = 0; i < (int)in_array.size(); i ++ )
		//{
		//	position_color pc;
		//	pc.x = in_array[i].x;
		//	pc.y = in_array[i].y;
		//	pc.z = in_array[i].z;
		//	pc.color = v_color;
		//	v_mesh->m_vertex_array.push_back( pc );
		//}

		//out_array.insert( out_array.end(), in_array.begin(), in_array.end() );
	}

	
}