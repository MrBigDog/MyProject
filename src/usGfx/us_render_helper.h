///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_render_helper.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : render_helper class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_RENDER_HELPER_H_
#define _US_RENDER_HELPER_H_
#include <usGfx/Export.h>
//#include <usUtil/us_vector3.h>
#include <usUtil/us_common_file.h>
#include <usUtil/us_lon_lat_rect.h>
#include <usUtil/us_rect.h>

namespace uniscope_globe
{
	class render_device;
	class hardware_texture;
	class observer_base;

	class USGFX_EXPORT render_helper
	{
	public:
		render_helper(void);

		virtual ~render_helper(void);

	public:
		static void draw_points(render_device* device, vector3<double>* vertices_buffer, ulong vertices_count, ulong vertices_color, ulong vertices_size, bool is_ztest = false);

		static void draw_line_list(render_device* device, vector3<double>* vertices_buffer, ulong vertices_count, ulong vertices_color, bool is_ztest = false);

		static void draw_line(render_device* device, vector3<double>& vert1, vector3<double>& vert2, ulong vertices_color, bool is_ztest = false);

		static void draw_line_strip(render_device* device, vector3<double>* vertices_buffer, ulong vertices_count, ulong vertices_color, bool is_ztest = false);

		static void draw_line_ring(render_device* device, vector3<double>* vertices_buffer, ulong vertices_count, ulong vertices_color, bool is_ztest = false);

		static void draw_view_shed_frame(render_device* device, vector3<double>& start_point, vector3<double>& end_point, double vertical_angle, double horizon_angle, ulong vertices_color, bool is_ztest = false);

		static void draw_spherical_points_line(render_device* device, vector3<double>* vertices_buffer, ulong vertices_count, ulong point_color, ulong v_point_size, ulong line_color, bool is_ztest = false);

		static void draw_triangle_strip(render_device* device, vector3<double>* vertices_buffer, ulong vertices_count, ulong vertices_color, bool is_ztest = false);

		static void draw_polygon(render_device* device, vector3<double>* vertices_buffer, ulong vertices_count, ulong vertices_color, bool is_ztest = false);

		static void draw_indexed_primitive_up(render_device* device, int v_type, ulong min_index, void* vertices_buffer, ushort* index_buffer, ulong verteices_count, ulong v_line_num, ulong vertices_size);

		static void draw_box(render_device* device, vector3<double>* vertices_buffer, ulong vertices_color);

		static void draw_box_ex(render_device* device, const vector3<double>* vertices_buffer, ulong vertices_color);

		static void draw_box(render_device* device, double v_width, double v_length, double v_height, ulong vertices_color, bool is_ztest = true, bool is_lighting = false);

		static void draw_lon_lat_rect(render_device* device, lon_lat_rect& v_rect, ulong vertices_color);

		static void draw_filled_box(render_device* device, vector3<double>* vertices_buffer, ulong vertices_color);

		static void draw_terrain_polygon(render_device* device, vector3<double>* vertices_buffer, ulong vertices_count, ulong fill_color);

		static void draw_rect(render_device* device, rect& v_rect, ulong vertices_color);

		static void draw_transformed_line(render_device* device, vector3<double>* vertices_buffer, ulong vertices_count, ulong vertices_color);

		static void draw_text(render_device* device, const LPCTSTR text_string, vector3<double>& position_in_geo, ulong text_color);

		static void draw_text_ext(render_device* device, const LPCTSTR text_string, vector3<double>& position_in_geo, ulong text_color, double scale);

		static void draw_icon(render_device* device, hardware_texture* in_texture, LPCTSTR text_string, vector3<double>& position_in_geo, ulong text_color);

		static void draw_transformed_points(render_device* device, vector3<double>* vertices_buffer, ulong vertices_count, ulong vertices_color);

		static void draw_transformed_region(render_device* device, vector3<double>* vertices_buffer, ulong vertices_count, ulong vertices_color);

		static void draw_transformed_text(render_device* device, const LPCTSTR text_string, point& position, ulong text_color);

		static void draw_frustum(render_device* device, observer_base* v_observer, ulong vertices_color);

		static void draw_filled_frustum(render_device* device, observer_base* v_observer, ulong vertices_color);

		static hardware_texture* create_fill_texture(render_device* device, ulong width, ulong height, ulong fill_color);

		static hardware_texture* create_texture_from_file(render_device* device, const LPCTSTR texture_path);

		static hardware_texture* create_texture_from_file_ex(render_device* device, const LPCTSTR texture_path);

		static hardware_texture* create_texture_from_resource(render_device* device, ULONG_PTR handle);



		//���߲�ֵ
		static void line_approximation(vector3<double>* in_pos_array, ulong in_num_of_points,
			vector3<double>** out_pos_array, ulong* out_num_of_points,
			double angle_tolerance);
	};
}

#endif // _US_RENDER_HELPER_H_