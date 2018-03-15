///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_render_object_factory.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : render_object_factory class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_RENDER_OBJECT_FACTORY_H_
#define _US_RENDER_OBJECT_FACTORY_H_

namespace uniscope_globe
{
	class LIB_GFX_API render_object_factory
	{
	public:
		render_object_factory( void ){}

		~render_object_factory( void ){}

		friend class render_object_geometry_mesh;
		friend class render_object_line;
		friend class render_object_polygon;

	//////////////////////////////////////////////////////////////////////////
	// common
	//////////////////////////////////////////////////////////////////////////
	protected:
		static double compute_scaled_length( vector3<double> pos1, vector3<double> pos2, double v_height );

	//////////////////////////////////////////////////////////////////////////
	// render_object_geometry_mesh
	//////////////////////////////////////////////////////////////////////////

		// 创建stencil渲染的线对象
	public:
		static render_object_geometry_mesh* create_mesh_from_line( vector_3d* v_src, int v_size, double v_width, ulong v_color, double in_height, double in_lower_height = spherical_coords::s_datum_plane );

		static void create_mesh_from_line( render_object_geometry_mesh* geo_mesh, vector_3d* v_src, int v_size, double v_width, ulong v_color, double in_height, double in_lower_height = spherical_coords::s_datum_plane );

	public:
		static render_object_geometry_mesh* create_mesh_from_multiple_line( vector3<double>* in_vertex_buffer, ulong in_vertex_size, ulong* in_index_buffer, ulong in_index_size, double in_width, ulong in_color, double in_height, double in_lower_height = spherical_coords::s_datum_plane );

		static void create_mesh_from_multiple_line( render_object_geometry_mesh* geo_mesh, vector3<double>* in_vertex_buffer, ulong in_vertex_size, ulong* in_index_buffer, ulong in_index_size, double in_width, ulong in_color, double in_height, double in_lower_height = spherical_coords::s_datum_plane );

	protected:
		static void generate_line_mesh( render_object_geometry_mesh* geo_mesh, vector_3d* v_src, coordinates_array& out_array, ulong v_size, double v_width, ulong v_color, double v_height, double v_lower_height );

		static void insert_vertex_to_line_mesh( render_object_geometry_mesh* v_mesh, ulong size, coordinates_array& in_array, coordinates_array& out_array, ulong v_color );

		// 创建stencil渲染的面对象
	public:
		static render_object_geometry_mesh* create_mesh_from_polygon( vector_3d* v_src, int v_size, ulong v_color, double in_height, double in_lower_height = spherical_coords::s_datum_plane );

		static render_object_geometry_mesh* create_mesh_from_multiple_polygon( vector3<double>* in_vertex_buffer, ulong in_vertex_size, ulong* in_index_buffer, ulong in_index_size, ulong v_color, double in_height, double in_lower_height = spherical_coords::s_datum_plane );

	public:
		static void create_mesh_from_polygon( render_object_geometry_mesh* geo_mesh, vector_3d* v_src, int v_size, ulong v_color, double in_height, double in_lower_height = spherical_coords::s_datum_plane );

		static void create_mesh_from_multiple_polygon( render_object_geometry_mesh* geo_mesh, vector3<double>* in_vertex_buffer, ulong in_vertex_size, ulong* in_index_buffer, ulong in_index_size, ulong v_color, double in_height, double in_lower_height = spherical_coords::s_datum_plane );

	public:
		static render_object_geometry_mesh* create_mesh_from_box( double x_len, double y_len, double z_len, ulong line_color, ulong fill_color );

		static render_object_geometry_mesh* create_mesh_from_volume( vector_3d* in_pos_array, ulong v_size, double v_height, ulong line_color, ulong fill_color );

		static render_object_geometry_mesh* create_mesh_from_cone( ulong in_sides, double in_radius, double in_height,  ulong line_color, ulong fill_color );

		static render_object_geometry_mesh* create_mesh_from_cylinder( ulong in_sides, double in_radius, double in_height, ulong line_color, ulong fill_color );

		static render_object_geometry_mesh* create_mesh_from_sphere( double v_radius,  ulong line_color, ulong fill_color );


		//static render_object_geometry_mesh* create_mesh_from_sphere_ex( double v_radius,  ulong line_color, ulong fill_color );

		static render_object_geometry_mesh* create_mesh_from_vertex_and_index( vector_3d* v_vertex, int v__vertex_size, ulong v_color, vector3<ushort>* v_index, int v_index_num );

		static render_object_geometry_mesh* create_mesh_from_vertex_and_index_cart( vector_3d* v_vertex, int v__vertex_size, ulong v_color, vector3<ushort>* v_index, int v_index_num );

		static render_object_geometry_mesh* create_mesh_from_vertex_and_index_cart_ex( vector_3d* v_vertex, int v__vertex_size, ulong v_color, vector3<int>* v_index, int v_index_num );

		// modify mesh


		static void create_mesh_from_box( render_object_geometry_mesh* geo_mesh, double x_len, double y_len, double z_len, ulong line_color, ulong fill_color );

		static void create_mesh_from_volume( render_object_geometry_mesh* geo_mesh, vector_3d* in_pos_array, ulong v_size, double v_height, ulong line_color, ulong fill_color );

		static void create_mesh_from_cone( render_object_geometry_mesh* geo_mesh, ulong in_sides, double in_radius, double in_height,  ulong line_color, ulong fill_color );

		static void create_mesh_from_cone2( render_object_geometry_mesh* geo_mesh, ulong in_sides, double in_radius_bottom, double in_radius_top, double in_height,  ulong line_color, ulong fill_color );

		static void create_mesh_from_cylinder( render_object_geometry_mesh* geo_mesh, ulong in_sides, double in_radius, double in_height, ulong line_color, ulong fill_color );

		static void create_mesh_from_sphere( render_object_geometry_mesh* geo_mesh, double v_radius,  ulong line_color, ulong fill_color );

		static void create_mesh_from_sphere_ex( render_object_geometry_mesh* geo_mesh, double v_radius,  ulong line_color, ulong fill_color );

		static void create_mesh_from_lon_lat_box( render_object_geometry_mesh* geo_mesh, lon_lat_box& v_box, ulong line_color, ulong fill_color );

		// update mesh
		static void reset_mesh_color( render_object_geometry_mesh* geo_mesh, ulong v_color, ulong v_line_color, ulong v_fill_color );

	protected:
		// 多边形使用的三角化算法
		// Geocentric坐标下
		static void generate_polygon_mesh( render_object_geometry_mesh* geo_mesh, vector_3d* in_pos_array, coordinates_array& out_array, ulong v_size, ulong v_color, double in_height, double in_lower_height );

		// volume 使用的底面三角化算法
		// 局部坐标下
		static void generate_polygon_mesh_cart( render_object_geometry_mesh* geo_mesh, vector_3d* in_pos_array, ulong v_size, double in_height, ulong v_color );

	public:
		static matrix_4d collapse_render_object_geometry_mesh( render_object_geometry_mesh* v_mesh, vector_3d& vec_cen );

		static void collapse_render_object_geometry_mesh( render_object_geometry_mesh* v_mesh, matrix_4f v_mat );

	public:
		static matrix_4d collapse_render_object_polygon( render_object_polygon* geo_polygon, vector_3d& vec_cen );

		static void collapse_render_object_polygon( render_object_polygon* geo_polygon, matrix_4f v_mat );

	public:
		static void collapse_mesh_buffer( d3d9_hardware_mesh_buffer* src_mesh, matrix_4f& mat, vector_3f* min_boundary = NULL, vector_3f* max_boundary = NULL );

		static void translate_mesh_buffer( d3d9_hardware_mesh_buffer* src_mesh, vector_3d& vec_trans );
		
	};
}

#endif // _US_RENDER_OBJECT_FACTORY_H_