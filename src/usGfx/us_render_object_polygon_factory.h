
///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_render_object_polygon_factory.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : render_object_polygon_factory class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_RENDER_OBJECT_POLYGON_FACTORY_H_
#define _US_RENDER_OBJECT_POLYGON_FACTORY_H_

#include <usGfx/Export.h>
#include <usUtil/us_matrix4.h>
#include <usUtil/us_spherical_coords.h>

namespace uniscope_globe
{
	class USGFX_EXPORT render_object_polygon_factory
	{
	public:
		render_object_polygon_factory(void) {}

		~render_object_polygon_factory(void) {}

		typedef std::vector<int> part_array;

	public:
		friend class render_object_polygon;
		friend class render_object_geometry_mesh;

		// 通过Geocentric坐标点创建普通多边形对象
	public:
		static void create_fill_polygon_cart(render_object_polygon* geo_polygon, coordinates_part_array& v_vecs_cart, ulong v_fill_color);

		static void create_fill_polygon_geo(render_object_polygon* geo_polygon, coordinates_array& v_vecs_geo, part_array& v_part, matrix_4d& v_mat_geo2cart, ulong v_fill_color);

	public:
		static void create_fill_polygon_geo(render_object_polygon* geo_polygon, vector_3d* v_vecs_geo, int v_vecs_num, matrix_4d& v_mat_geo2cart, ulong v_fill_color);

	protected:
		static void generate_polygon_triangle_list_geo(render_object_polygon* geo_polygon, vector_3d* v_vecs_geo, int v_vecs_num);

		// 通过Cartesian坐标点创建普通多边形对象
	public:
		static void create_fill_polygon_cart(render_object_polygon* geo_polygon, vector_3d* v_vecs_cart, int v_vecs_num, ulong v_line_color, ulong v_fill_color);

	protected:
		static void generate_polygon_triangle_list_cart(render_object_polygon* geo_polygon, vector_3d* v_vecs_cart, int v_vecs_num);

	public:
		static void create_fill_circle_cart(render_object_polygon* geo_polygon, vector_3d& v_center_cart, double v_radius_metric, ulong v_line_color, ulong v_fill_color, ulong v_segment = 32);

		// 通过Geocentric坐标点创建Stencil渲染对象
	public:
		static void create_stencil_mesh_from_polygon_geo(render_object_geometry_mesh* geo_mesh, vector_3d* v_vecs_geo, int v_vecs_num, ulong v_fill_color, double in_upper_height, double in_lower_height = spherical_coords::s_datum_plane);

		static void create_stencil_mesh_from_multiple_polygon_geo(render_object_geometry_mesh* geo_mesh, vector3<double>* v_vecs_geo, ulong v_vecs_num, ulong* v_part_info, ulong v_part_num, ulong v_fill_color, double in_upper_height, double in_lower_height = spherical_coords::s_datum_plane);

	public:
		static void create_stencil_mesh_from_polygon_geo(render_object_geometry_mesh* geo_mesh, vector_3d* v_vecs_geo, int v_vecs_num, matrix_4d& v_mat_geo2cart, ulong v_fill_color, double in_upper_height, double in_lower_height = spherical_coords::s_datum_plane);

	protected:
		static void generate_polygon_stencil_mesh(render_object_geometry_mesh* geo_mesh, vector_3d* v_vecs_geo, ulong v_vecs_num, coordinates_array& out_array, ulong v_fill_color, double in_upper_height, double in_lower_height = spherical_coords::s_datum_plane);

	public:
		static void create_stencil_mesh_from_polygon_cart(render_object_geometry_mesh* geo_mesh, coordinates_part_array& v_coord_parts, ulong v_fill_color, double in_upper_height, double in_lower_height);


	};
}

#endif // _US_RENDER_OBJECT_POLYGON_FACTORY_H_ 