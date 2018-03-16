///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_geometry_shadow_factory.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : geometry_shadow_factory class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_GEOMETRY_SHADOW_FACTORY_H_
#define _US_GEOMETRY_SHADOW_FACTORY_H_
#include <usGfx/Export.h>
#include <usGfx/us_d3d9_vertex_declear.h>
#include <usUtil/us_matrix4.h>
#include <usUtil/us_common_file.h>
#include <usUtil/us_lon_lat_box.h>

#include <usDotx/us_usx_data_struct.h>

#include <map>

namespace uniscope_globe
{
	class USGFX_EXPORT geometry_shadow_factory
	{
	public:
		geometry_shadow_factory(void) {}

		~geometry_shadow_factory(void) {}

		typedef std::pair<ulong, ulong> edge;

		typedef std::map< edge, degenerate_quad > edge_map;

		friend class geometry_shadow_mesh;

		//////////////////////////////////////////////////////////////////////////
		// common
		//////////////////////////////////////////////////////////////////////////
	protected:
		static double compute_scaled_length(vector3<double> pos1, vector3<double> pos2, double v_height);

		static vector_3f compute_normal(vector_3f vec1, vector_3f vec2, vector_3f vec3);

		static void generate_polygon_mesh(vector_3d* in_pos_array, ulong v_size, double v_height,
			std::vector<vector_3f> &out_points,
			std::vector<usx_tri_face_index>& out_index, axis_aligned_box_d& out_box);

		static void generate_shadow_mesh(vector_3f* v_vertex, int vertex_num,
			usx_tri_face_index* v_indices, int face_num,
			std::vector<position_normal_color>& new_vertex,
			std::vector<usx_tri_face_index>& new_indices,
			ulong v_fill_color);

		static void insert_triangle(ulong o1, ulong o2, ulong n1, ulong n2,
			edge_map& in_edge_map, std::vector<usx_tri_face_index>& in_index_array);



		//////////////////////////////////////////////////////////////////////////
		// geometry_shadow_mesh
		//////////////////////////////////////////////////////////////////////////
	public:
		static void create_mesh_from_box(geometry_shadow_mesh* geo_mesh, double x_len, double y_len, double z_len, ulong line_color, ulong fill_color);

		static void create_mesh_from_volume(geometry_shadow_mesh* geo_mesh, vector_3d* in_pos_array, ulong v_size, double v_height, ulong line_color, ulong fill_color);

		static void create_mesh_from_cone(geometry_shadow_mesh* geo_mesh, ulong in_sides, double in_radius, double in_height, ulong line_color, ulong fill_color);

		static void create_mesh_from_cylinder(geometry_shadow_mesh* geo_mesh, ulong in_sides, double in_radius, double in_height, ulong line_color, ulong fill_color);

		static void create_mesh_from_sphere(geometry_shadow_mesh* geo_mesh, double v_radius, ulong line_color, ulong fill_color);

		static void create_mesh_from_lon_lat_box(geometry_shadow_mesh* geo_mesh, lon_lat_box& v_box, ulong line_color, ulong fill_color);

		// update mesh
		static void reset_mesh_color(geometry_shadow_mesh* geo_mesh, ulong v_color, ulong v_line_color, ulong v_fill_color);

	public:
		static matrix_4d collapse_geometry_mesh(geometry_shadow_mesh* v_mesh, vector_3d& vec_cen);

		static void collapse_geometry_mesh(geometry_shadow_mesh* v_mesh, matrix_4f v_mat);

	};
}

#endif // _US_GEOMETRY_SHADOW_FACTORY_H_ 