///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_render_object_line_factory.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : render_object_line_factory class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_RENDER_OBJECT_LINE_FACTORY_H_
#define _US_RENDER_OBJECT_LINE_FACTORY_H_

namespace uniscope_globe
{
	class render_object_line_factory
	{
	public:
		render_object_line_factory( void ) {}

		virtual ~render_object_line_factory( void ) {}

		friend class render_object_line;
		friend class render_object_geometry_mesh;

	protected:
		static void push_tri_strip_point( std::vector<position_normal_color>& triangle_list, vector3<double>& pt, vector3<double>& normal, ulong color );

		static void insert_index_to_line_mesh( render_object_line_volume* v_mesh, ulong size );

		static void generate_line_stencil_mesh( render_object_geometry_mesh* geo_mesh,  vector_3d* v_vecs_geo,  ulong v_src_point_size, coordinates_array& out_array,  double v_width_geo, ulong v_line_color,  double v_uper_height,  double v_lower_height );

		static void insert_vertex_to_line_mesh( render_object_geometry_mesh* v_mesh, ulong vertex_num, coordinates_array& in_array, coordinates_array& out_array, ulong v_line_color );

		// create render_object_line
	public:
		// 通过局部坐标点集创建简单线渲染对象
		static void create_render_object_line_cart( OUT render_object_line* geo_mesh, IN coordinates_array& in_vecs_cart, IN ulong line_color );

		// 通过Geocentric坐标点集创建简单线渲染对象
		static void create_render_object_line_geo( OUT render_object_line* geo_mesh, IN coordinates_array& in_vecs_geo, IN matrix_4d& v_mat_geo2cart, IN ulong line_color );

		// create render_object_line_flake
	public:
		// 通过局部坐标点集创建 flake线渲染对象
		static void create_render_object_line_flake_cart( OUT render_object_line_flake* geo_mesh, IN coordinates_array& in_vecs_cart, IN double pixel_width, IN ulong line_color );

		// 通过Geocentric坐标点集创建 flake线渲染对象
		static void create_render_object_line_flake_geo( OUT render_object_line_flake* geo_mesh, IN vector3<double>* in_vecs_geo, IN ulong v_vecs_num, IN matrix_4d& v_mat_geo2cart, IN double pixel_width, IN ulong line_color );

		// 通过Geocentric坐标点集创建 flake线渲染对象, 点集中包含了多了Line的集合
		static void create_render_object_multiple_line_flake_geo( render_object_line_flake* geo_mesh, IN vector3<double>* in_vecs_geo, IN ulong v_vecs_num, IN ulong* v_part_info, IN ulong v_part_num, IN matrix_4d& v_mat_geo2cart,  IN double pixel_width, IN ulong line_color );

		// create render_object_line_volume
	public:
		// 通过局部坐标点集创建 line_volume线渲染对象
		static void create_render_object_line_volume_cart( render_object_line_volume* geo_mesh, IN coordinates_array& in_vecs_cart, IN double pixel_width, IN ulong line_color, IN double upper_height, IN double lower_height );

		// 通过Geocentric坐标点集创建 line_volume线渲染对象
		static void create_render_object_line_volume_geo( render_object_line_volume* geo_mesh, IN vector3<double>* in_vecs_geo, IN ulong v_vecs_num, IN matrix_4d& v_mat_geo2cart,  IN double pixel_width, IN ulong line_color, IN double upper_height, IN double lower_height );

		// 通过Geocentric坐标点集创建 line_volume线渲染对象, 点集中包含了多了Line的集合
		static void create_render_object_multiple_line_volume_geo( render_object_line_volume* geo_mesh, IN vector3<double>* in_vecs_geo, IN ulong v_vecs_num, IN ulong* v_part_info, IN ulong v_part_num, IN matrix_4d& v_mat_geo2cart, IN double pixel_width, IN ulong line_color, IN double upper_height, IN double lower_height );

		// create render_object_geometry_mesh
	public:
		// 通过Geocentric坐标点集创建
		static void create_stencil_mesh_from_line_geo( render_object_geometry_mesh* geo_mesh, vector_3d* v_vecs_geo, int v_vecs_num, double v_width_geo, ulong v_line_color, double v_upper_height, double v_lower_height = spherical_coords::s_datum_plane );

		static void create_stencil_mesh_from_multiple_line_geo( render_object_geometry_mesh* geo_mesh, vector3<double>* v_vecs_geo, ulong v_vecs_num, ulong* v_part_info, ulong v_part_num, double v_width_geo, ulong v_line_color, double v_upper_height, double v_lower_height = spherical_coords::s_datum_plane );

	public:
		static void create_stencil_mesh_from_line_geo( render_object_geometry_mesh* geo_mesh, IN coordinates_array& v_coord_geo, IN matrix_4d& v_mat_geo2cart, IN double v_width_geo, IN ulong v_line_color, IN double v_upper_height, IN double v_lower_height = spherical_coords::s_datum_plane );


	};
}

#endif // _US_RENDER_OBJECT_LINE_FACTORY_H_ 