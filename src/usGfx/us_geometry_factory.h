///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_geometry_factory.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : geometry_factory class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_GEOMETRY_FACTORY_H_
#define _US_GEOMETRY_FACTORY_H_

namespace uniscope_globe
{
	class LIB_GFX_API geometry_factory
	{
	public:
		geometry_factory( void ){}

		~geometry_factory( void ){}

		friend class geometry_mesh;
		friend class geometry_line;
		friend class geometry_polygon;

	//////////////////////////////////////////////////////////////////////////
	// common
	//////////////////////////////////////////////////////////////////////////
	protected:
		static double compute_scaled_length( vector3<double> pos1, vector3<double> pos2, double v_height );

	//////////////////////////////////////////////////////////////////////////
	// geometry_mesh
	//////////////////////////////////////////////////////////////////////////
	public:
		// new mesh

		static geometry_mesh* create_mesh_from_line( vector_3d* v_src, int v_size, double v_width, ulong v_color, double in_height, double in_lower_height = spherical_coords::s_datum_plane );

		static geometry_mesh* create_mesh_from_multiple_line( vector3<double>* in_vertex_buffer, ulong in_vertex_size, ulong* in_index_buffer, ulong in_index_size, double in_width, ulong in_color, double in_height, double in_lower_height = spherical_coords::s_datum_plane );

		static geometry_mesh* create_mesh_from_polygon( vector_3d* v_src, int v_size, ulong v_color, double in_height, double in_lower_height = spherical_coords::s_datum_plane );

		static geometry_mesh* create_mesh_from_multiple_polygon( vector3<double>* in_vertex_buffer, ulong in_vertex_size, ulong* in_index_buffer, ulong in_index_size, ulong v_color, double in_height, double in_lower_height = spherical_coords::s_datum_plane );

		static geometry_mesh* create_mesh_from_box( double x_len, double y_len, double z_len, ulong line_color, ulong fill_color );

		static geometry_mesh* create_mesh_from_volume( vector_3d* in_pos_array, ulong v_size, double v_height, ulong line_color, ulong fill_color );

		static geometry_mesh* create_mesh_from_cone( ulong in_sides, double in_radius, double in_height,  ulong line_color, ulong fill_color );

		static geometry_mesh* create_mesh_from_cylinder( ulong in_sides, double in_radius, double in_height, ulong line_color, ulong fill_color );

		static geometry_mesh* create_mesh_from_sphere( double v_radius,  ulong line_color, ulong fill_color );

		static geometry_mesh* create_mesh_from_vertex_and_index( vector_3d* v_vertex, int v__vertex_size, ulong v_color, vector3<ushort>* v_index, int v_index_num );

		static geometry_mesh* create_mesh_from_vertex_and_index_cart( vector_3d* v_vertex, int v__vertex_size, ulong v_color, vector3<ushort>* v_index, int v_index_num );

		// modify mesh
		static void create_mesh_from_line( geometry_mesh* geo_mesh, vector_3d* v_src, int v_size, double v_width, ulong v_color, double in_height, double in_lower_height = spherical_coords::s_datum_plane );

		static void create_mesh_from_multiple_line( geometry_mesh* geo_mesh, vector3<double>* in_vertex_buffer, ulong in_vertex_size, ulong* in_index_buffer, ulong in_index_size, double in_width, ulong in_color, double in_height, double in_lower_height = spherical_coords::s_datum_plane );

		static void create_mesh_from_polygon( geometry_mesh* geo_mesh, vector_3d* v_src, int v_size, ulong v_color, double in_height, double in_lower_height = spherical_coords::s_datum_plane );

		static void create_mesh_from_multiple_polygon( geometry_mesh* geo_mesh, vector3<double>* in_vertex_buffer, ulong in_vertex_size, ulong* in_index_buffer, ulong in_index_size, ulong v_color, double in_height, double in_lower_height = spherical_coords::s_datum_plane );

		static void create_mesh_from_box( geometry_mesh* geo_mesh, double x_len, double y_len, double z_len, ulong line_color, ulong fill_color );

		static void create_mesh_from_volume( geometry_mesh* geo_mesh, vector_3d* in_pos_array, ulong v_size, double v_height, ulong line_color, ulong fill_color );

		static void create_mesh_from_cone( geometry_mesh* geo_mesh, ulong in_sides, double in_radius, double in_height,  ulong line_color, ulong fill_color );

		static void create_mesh_from_cylinder( geometry_mesh* geo_mesh, ulong in_sides, double in_radius, double in_height, ulong line_color, ulong fill_color );

		static void create_mesh_from_sphere( geometry_mesh* geo_mesh, double v_radius,  ulong line_color, ulong fill_color );

		static void create_mesh_from_lon_lat_box( geometry_mesh* geo_mesh, lon_lat_box& v_box, ulong line_color, ulong fill_color );

		// update mesh
		static void reset_mesh_color( geometry_mesh* geo_mesh, ulong v_color, ulong v_line_color, ulong v_fill_color );

	protected:
		static void generate_line_mesh( geometry_mesh* geo_mesh, vector_3d* v_src, coordinates_array& out_array, ulong v_size, double v_width, ulong v_color, double v_height, double v_lower_height );

		static void insert_vertex_to_line_mesh( geometry_mesh* v_mesh, ulong size, coordinates_array& in_array, coordinates_array& out_array, ulong v_color );

	protected:
		static void generate_polygon_mesh( geometry_mesh* geo_mesh, vector_3d* in_pos_array, coordinates_array& out_array, ulong v_size, ulong v_color, double in_height, double in_lower_height );

		static void generate_polygon_mesh_cart( geometry_mesh* geo_mesh, vector_3d* in_pos_array, ulong v_size, double in_height, ulong v_color );

	public:
		static matrix_4d collapse_geometry_mesh( geometry_mesh* v_mesh, vector_3d& vec_cen );

		static void collapse_geometry_mesh( geometry_mesh* v_mesh, matrix_4f v_mat );

	//////////////////////////////////////////////////////////////////////////
	// line
	//////////////////////////////////////////////////////////////////////////
	public:
		// line
		static void create_line_string( geometry_line* geo_line, vector_3d* v_src, int v_size, ulong color );

		static void create_line_string_cart( geometry_line* geo_line, vector_3d* v_src, int v_size, ulong color );

	protected:
		static void generate_line_string( geometry_line* geo_line, vector_3d* v_src, int v_size, ulong v_color );

	public:
		static matrix_4d collapse_geometry_line( geometry_line* v_line, vector_3d& vec_cen );

		static void collapse_geometry_line( geometry_line* v_line, matrix_4f v_mat );

	//////////////////////////////////////////////////////////////////////////
	// polygon
	//////////////////////////////////////////////////////////////////////////
	public:
		// circle
		static geometry_polygon* create_fill_circle( vector_3d& v_center, double v_radius, ulong v_line_color, ulong v_fill_color, ulong v_segment = 32);

		static geometry_polygon* create_fill_circle_cart( vector_3d& v_center, double v_radius, ulong v_line_color, ulong v_fill_color, ulong v_segment = 32);

		static void create_fill_circle( geometry_polygon* geo_polygon, vector_3d& v_center, double v_radius, ulong v_line_color, ulong v_fill_color, ulong v_segment = 32);

		static void create_fill_circle_cart( geometry_polygon* geo_polygon, vector_3d& v_center, double v_radius, ulong v_line_color, ulong v_fill_color, ulong v_segment = 32);

		// polygon
		static void create_fill_polygon( geometry_polygon* geo_polygon, vector_3d* v_src, int v_size, ulong v_line_color, ulong v_fill_color );

		static void create_fill_polygon_cart( geometry_polygon* geo_polygon, vector_3d* v_src, int v_size, ulong v_line_color, ulong v_fill_color );


	protected:
		static void generate_polygon( geometry_polygon* geo_polygon, vector_3d* v_src, int v_size, ulong v_line_color, ulong v_fill_color );

		static void generate_triangle_list( geometry_polygon* geo_polygon, vector_3d* v_src, int v_size );

		static void generate_triangle_list_cart( geometry_polygon* geo_polygon, vector_3d* v_src, int v_size );

	public:
		static matrix_4d collapse_geometry_polygon( geometry_polygon* geo_polygon, vector_3d& vec_cen );

		static void collapse_geometry_polygon( geometry_polygon* geo_polygon, matrix_4f v_mat );

	////////////////////////////////////////////////////////////////////////////////////////////////////
	//public:
	//	static void create_line_mesh( OUT geometry_line_flake* geo_mesh, IN coordinates_array& in_vec_array, IN vector_3d& center_geo, IN double line_width, IN ulong line_color );

	//	static void push_tri_strip_point( std::vector<position_normal_color>& triangle_list, vector3<double>& pt, vector3<double>& normal, ulong color );

	};
}

#endif // _US_GEOMETRY_FACTORY_H_