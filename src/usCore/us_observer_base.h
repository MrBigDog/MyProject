///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_observer_base.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : observer_base class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_OBSERVER_BASE_H_
#define _US_OBSERVER_BASE_H_

#include <usCore\Export.h>
#include <usCore\us_spherical_object.h>
#include <usCore\us_view_port.h>

#include <usUtil/us_ray.h>
#include <usUtil/us_point.h>
#include <usUtil/us_frustum.h>
#include <usUtil/us_matrix4.h>
#include <usUtil/us_spherical_coords.h>

namespace uniscope_globe
{
	class render_device;

	typedef enum
	{
		IDLE = 0,
		FIRST_PERSON_TRACKING,
		THIRD_PERSON_TRACKING,
		FIRST_PERSON_TRACKING_LOCAL,
		THIRD_PERSON_TRACKING_LOCAL,
		FREE_TRACKING,
		ON_TRACK,
	}observer_state;

	class USCORE_EXPORT observer_base : public spherical_object
	{
	public:
		observer_base(void);

		observer_base(observer_base& observer);

		virtual ~observer_base(void);

		friend class observer_helper;

		// method
	public:
		virtual void update(time_value v_current_time) {}

		//virtual vector3<double> projection( const vector3<double>& vector_in_world );

		//virtual vector3<double> unprojection( const vector3<double>& vector_on_viewport );

		void copy(observer_base& observer);

		void submit_to_device(render_device* device);

		void resize_viewport(int cx, int cy);

		virtual void set_clip_rect(float x, float y, float w, float h);

		// property
	public:
		void set_aim_geo(vector3<double>& in_aim) { m_aim_geo = in_aim; }
		void set_origin_geo(vector3<double>& in_origin) { m_origin_geo = in_origin; }
		void set_up_normal_geo(vector3<double>& in_up_normal) { m_up_normal_geo = in_up_normal; }
		void set_fov(double in_fov) { m_fov = in_fov; }
		void set_aspect(double in_aspect) { m_aspect = in_aspect; }
		void set_far_plane(double in_far_plane) { m_far_plane = in_far_plane; }
		void set_near_plane(double in_near_plane) { m_near_plane = in_near_plane; }
		void set_view_port(view_port& in_view_port) { m_view_port = in_view_port; }
		void set_speed(double in_speed) { m_speed = in_speed; }


		vector3<double> get_aim_geo(void) { return m_aim_geo; }
		vector3<double> get_origin_geo(void) { return m_origin_geo; }
		vector3<double> get_up_normal_geo(void) { return m_up_normal_geo; }
		double get_fov(void) { return m_fov; }
		double get_aspect(void) { return m_aspect; }
		double get_near_plane(void) { return m_near_plane; }
		double get_far_plane(void) { return m_far_plane; }
		view_port get_view_port(void) { return m_view_port; }
		double get_speed(void) { return m_speed; }

		vector3<double> get_aim_local(void) { return m_aim_local; }
		vector3<double> get_position_local(void) { return vector3<double>::s_zero; }
		vector3<double> get_direction_local(void) { return m_direction_local; }
		matrix4<double> get_view_center_mat_tnb(void) { return m_view_center_mat_tnb; }
		matrix4<double> get_view_center_mat_tnb_invs(void) { return m_view_center_mat_tnb_invs; }


		vector3<double> get_aim_sphr(void) { return m_aim_sphr; }
		vector3<double> get_origin_sphr(void) { return m_origin_sphr; }

		matrix4<double> get_view_matrix_geo(void) { return m_view_matrix_geo; }
		matrix4<double> get_projection_matrix(void) { return m_projection_matrix; }
		frustum<double>* get_frustum(void) { return &m_frustum; }

		int	get_max_level(void) { return m_max_level; }
		double get_tile_width(void) { return m_tile_width; }

		double get_pixel_to_geo_scale_by_aim(void) { return m_pixel_to_geo_scale_by_aim; }
		double get_geo_to_pixel_scale_by_aim(void) { return m_geo_to_pixel_scale_by_aim; }

		double get_pixel_to_geo_scale_by_height(void) { return m_pixel_to_geo_scale_by_height; }
		double get_geo_to_pixel_scale_by_height(void) { return m_geo_to_pixel_scale_by_height; }

		double get_pixel_to_geo_scale_by_distance(double v_distance);
		double get_geo_to_pixel_scale_by_distance(double v_distance);

		bool is_moving(void) { return m_is_moving; }

		double get_dem_height(void) { return (m_current_dem_height + spherical_coords::s_semi_major); }


		void set_ortho_width(double in_ortho_width) { m_ortho_width = in_ortho_width; }
		double get_ortho_width(void) { return m_ortho_width; }

		void set_ortho_height(double in_ortho_height) { m_ortho_height = in_ortho_height; }
		double get_ortho_height(void) { return m_ortho_height; }

		void set_ortho_mode(bool enable) { m_enable_ortho_mode = enable; }
		bool get_ortho_mode(void) { return m_enable_ortho_mode; }

		void set_custom_near_far_mode(bool enable) { m_custom_near_far_mode = enable; }
		bool get_custom_near_far_mode(void) { return m_custom_near_far_mode; }
	public:
		virtual ray<double>		calcu_ray(point& pt) = 0;

	public:
		//vector3<double> projection_goecentric( const vector3<double>& vector_geo );

		//vector3<double> projection_world( const vector3<double>& vector_world );

		//vector3<double> unprojection_geocentric( const vector3<double>& vector_on_viewport );

		//vector3<double> unprojection_world( const vector3<double>& vector_on_viewport );

		//void project_line_sphr(coordinates_array_f& out_vec_array_scrn, coordinates_array_d& in_vec_array_sphr);

	protected:
		virtual void compute_aim_point();

		virtual void calculate_local_parameter(void);


	protected:
		vector3<double> m_aim_geo;
		vector3<double> m_origin_geo;
		vector3<double> m_up_normal_geo;

		vector3<double> m_aim_sphr;
		vector3<double> m_origin_sphr;

		double			m_fov;
		double			m_aspect;
		double			m_near_plane;
		double			m_far_plane;

		matrix4<double> m_view_matrix_geo;
		matrix4<double> m_view_matrix_rp;
		matrix4<double> m_projection_matrix;
		frustum<double> m_frustum;
		view_port		m_view_port;

		double			m_speed;
		bool			m_is_moving;
		double			m_current_dem_height;

		int				m_max_level;
		double			m_tile_width;

		double			m_geo_to_pixel_scale_by_height;
		double			m_pixel_to_geo_scale_by_height;

		double			m_geo_to_pixel_scale_by_aim;
		double			m_pixel_to_geo_scale_by_aim;

		double			m_ortho_width;
		double			m_ortho_height;

		bool			m_enable_ortho_mode;


		vector3<double> m_position_local;
		vector3<double>	m_direction_local;
		vector3<double>	m_aim_local;
		matrix4<double> m_view_center_mat_tnb;
		matrix4<double> m_view_center_mat_tnb_invs;


		matrix4<double> m_clip_matrix;
		vector3<double>	m_last_position_geo;

	public:
		double m_clip_x;
		double m_clip_y;
		double m_clip_width;
		double m_clip_height;

	public:
		observer_state  m_state;

		bool			m_intersect_model;

		bool			m_custom_near_far_mode;
	};
}

#endif // _US_OBSERVER_BASE_H_