///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_render_object_shadow_factory.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : render_object_shadow_factory class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_RENDER_OBJECT_SHADOW_FACTORY_H_
#define _US_RENDER_OBJECT_SHADOW_FACTORY_H_

namespace uniscope_globe
{
	class LIB_GFX_API render_object_shadow_factory
	{
	public:
		render_object_shadow_factory( void ){}

		~render_object_shadow_factory( void ){}

		typedef std::pair<ulong, ulong> edge;
		typedef std::map< edge, degenerate_quad > edge_map;

		typedef std::vector<usx_tri_face_index> usx_tri_face_index_array;
		typedef std::vector<position_normal> position_normal_array;

		typedef std::vector<vector_3f> vector_3f_array;
		typedef std::vector<ulong> face_int_index_array;
		typedef std::vector<ushort> face_short_index_array;

		friend class render_object_shadow_mesh;
		friend class render_object_geometry_mesh;
	
		//////////////////////////////////////////////////////////////////////////
		// common
		//////////////////////////////////////////////////////////////////////////
	protected:
		static double compute_scaled_length( vector3<double> pos1, vector3<double> pos2, double v_height );
		
		static vector_3f compute_normal( vector_3f* vec1, vector_3f* vec2, vector_3f* vec3 );

		static void generate_polygon_mesh( vector_3d* in_pos_array, ulong v_size, double v_height,
										   std::vector<vector_3f> &out_points,
										   std::vector<usx_tri_face_index>& out_index, axis_aligned_box_d& out_box );

		static void generate_shadow_mesh( vector_3f* v_vertex, int vertex_num,
										  usx_tri_face_index* v_indices, int face_num,
										  std::vector<position_normal_color>& new_vertex,
										  std::vector<usx_tri_face_index>& new_indices,
										  ulong v_fill_color );

		static void insert_triangle( ulong o1, ulong o2, ulong n1, ulong n2, 
									 edge_map& in_edge_map, std::vector<usx_tri_face_index>& in_index_array );

		

		//////////////////////////////////////////////////////////////////////////
		// render_object_shadow_mesh
		//////////////////////////////////////////////////////////////////////////
	public:
		static void create_mesh_from_box( render_object_shadow_mesh* geo_mesh, double x_len, double y_len, double z_len, ulong line_color, ulong fill_color );

		static void create_mesh_from_volume( render_object_shadow_mesh* geo_mesh, vector_3d* in_pos_array, ulong v_size, double v_height, ulong line_color, ulong fill_color );

		static void create_mesh_from_cone( render_object_shadow_mesh* geo_mesh, ulong in_sides, double in_radius, double in_height,  ulong line_color, ulong fill_color );

		static void create_mesh_from_cylinder( render_object_shadow_mesh* geo_mesh, ulong in_sides, double in_radius, double in_height, ulong line_color, ulong fill_color );

		static void create_mesh_from_sphere( render_object_shadow_mesh* geo_mesh, double v_radius,  ulong line_color, ulong fill_color );

		static void create_mesh_from_lon_lat_box( render_object_shadow_mesh* geo_mesh, lon_lat_box& v_box, ulong line_color, ulong fill_color );

		// update mesh
		static void reset_mesh_color( render_object_shadow_mesh* geo_mesh, ulong v_color, ulong v_line_color, ulong v_fill_color );

	public:
		static matrix_4d collapse_geometry_mesh( render_object_shadow_mesh* v_mesh, vector_3d& vec_cen );

		static void collapse_geometry_mesh( render_object_shadow_mesh* v_mesh, matrix_4f v_mat );

	//////////////////////////////////////////////////////////////////////////
	// 点光源生成阴影体

	public:
		static void create_point_light_shadow_mesh( render_object_geometry_mesh* geo_mesh, sphere<double>& dome, d3d9_hardware_mesh_buffer* v_buf );

	protected:
		static void generate_shadow_mesh_index( render_object_geometry_mesh* geo_mesh, sphere<double>& dome, d3d9_hardware_mesh_buffer* v_buf, short* in_indice_vis );

		static void generate_shadow_mesh_vertex( render_object_geometry_mesh* geo_mesh, sphere<double>& dome, d3d9_hardware_mesh_buffer* v_buf );

	//  点光源生成阴影体
	//////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////
	//// 点光源生成阴影体,专门针对threat_dome

	public:
		static void create_point_light_shadow_mesh_td( render_object_geometry_mesh* geo_mesh, sphere<double>& dome, d3d9_hardware_mesh_buffer* v_buf );

	protected:
		static void generate_face_visible_flag( short* &v_vis_buf, sphere<double>& dome, d3d9_hardware_mesh_buffer* v_buf  );

		static void generate_shadow_mesh_index_td( render_object_geometry_mesh* geo_mesh, sphere<double>& dome, d3d9_hardware_mesh_buffer* v_buf, short* in_indice_vis );

		static void generate_shadow_mesh_vertex_td( render_object_geometry_mesh* geo_mesh, sphere<double>& dome, d3d9_hardware_mesh_buffer* v_buf );

	////  点光源生成阴影体,专门针对threat_dome
	////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// 生成四倍顶点SHADOW对象
	public:
		static void generate_shadow_mesh_buffer( d3d9_hardware_mesh_buffer* src_mesh, d3d9_hardware_mesh_buffer* des_mesh );
	
		static void patching( d3d9_hardware_mesh_buffer* v_buffer, edge_map& in_edge_map, position_normal_array& in_new_vertex_array,  usx_tri_face_index_array& in_new_face_index );

	// 生成四倍顶点SHADOW对象
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// 封闭体裁剪模型
	public:
		static d3d9_hardware_mesh_buffer* closed_mesh_clip_mesh( d3d9_hardware_mesh_buffer* close_mesh, d3d9_hardware_mesh_buffer* v_mesh );

		static void segment_intersect_mesh( vector_3f& s_vec, vector_3f& e_vec, d3d9_hardware_mesh_buffer* v_mesh, vector_3f_array& v_vec_array );

		static void add_polygon_to_mesh( vector_3f_array& v_new_points, vector_3f_array& pt_array, face_int_index_array& v_index_array );

		static void face_intersect_mesh_int( vector_3f& vec1, vector_3f& vec2, vector_3f& vec3, d3d9_hardware_mesh_buffer* v_mesh, vector_3f_array& pt_array, face_int_index_array& v_index_array );

		static void face_intersect_mesh_short( vector_3f& vec1, vector_3f& vec2, vector_3f& vec3, d3d9_hardware_mesh_buffer* v_mesh, vector_3f_array& pt_array, face_short_index_array& v_index_array );

		

	// 封闭体裁剪模型
	//////////////////////////////////////////////////////////////////////////
	};
}

#endif // _US_RENDER_OBJECT_SHADOW_FACTORY_H_ 