///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_spherical_object.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : spherical_object class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_SPHERICAL_OBJECT_H_
#define _US_SPHERICAL_OBJECT_H_

#include <usCore\Export.h>
#include <usCore\us_styling_object.h>
#include <usCore\us_rigid_body.h>
#include <usCore\us_spatial_object.h>
#include <usCore\us_editable_object.h>

#include <usUtil/us_spherical_transform.h>
#include <usUtil/us_spatial_reference.h>
#include <usUtil/us_lon_lat_rect.h>

namespace uniscope_globe
{
	typedef enum
	{
		SPHERICAL_OBJECT_IDLE,
		SPHERICAL_OBJECT_ON_TRACK
	}spherical_object_state;

#define US_SO_NO_BOUNDBOX_IN_SELECT_SET 0x1


	class observer_base;
	class cartesian_object;

	class USCORE_EXPORT spherical_object
		: public styling_object
		, public rigid_body
		, virtual public spatial_object
		, public editable_object
	{
	public:
		spherical_object(void);
		spherical_object(document_base* in_doc);
		virtual ~spherical_object(void);

		// override
	public:
		virtual void finalize(void);

	public:
		virtual void set_selectable(bool in_enable) { m_is_selectable = in_enable; }

		virtual bool is_selectable(void) { return m_is_selectable; }

		virtual matrix_4d get_matrix_cart2geo(void) { return m_spherical_transform->m_mat_tnb; }

	public:
		virtual style_base* get_render_style() { return NULL; }

		// rigid_body
	public:
		virtual void update_motion(time_value in_frame_time);

	protected:
		void update_position(time_value in_elapsed_time);

		void update_rotation(time_value in_elapsed_time);

	public:
		virtual void attach_to_clip_tree(spatial_node* v_root) { }

		virtual void detach_from_clip_tree(void) { }

	public:
		virtual void set_clip_tree_root(spatial_node* in_tree) {}

		virtual spatial_node* get_clip_tree_root(void) { return NULL; }

	public:
		virtual void add_sub_object(spatial_object* v_sub_object) {}

		virtual void insert_sub_object(int index, spatial_object* v_sub_object) {}

		virtual void remove_sub_object(spatial_object* v_sub_object) {}

		virtual void remove_sub_object(int index) {}

		virtual spatial_object* get_sub_object(int index) { return NULL; }

		virtual int get_sub_object_count(void) { return 0; }

		virtual void remove_all_sub_object(void) {}

	public:
		spherical_transform* get_spherical_transform(void) { return m_spherical_transform; }

		void set_spherical_transform(spherical_transform* v_transf);

		spatial_reference* get_spatial_reference(void) { return m_spatial_reference; }
		void set_spatial_reference(spatial_reference* v_ref);

		virtual spherical_transform* change_spherical_transform(spherical_transform* v_transf) { return m_spherical_transform; }

		virtual void change_cartesion_transform(vector3<double>& pos) { }

		bool distance_clip(observer_base* v_observer);

		bool frustum_clip(observer_base* v_observer);

		// ·½±ã×ªµ÷
	public:
		//void set_position_geo( vector3<double>& in_pos )		{ m_spherical_transform->m_position_geo = in_pos; }
		//void set_target_geo( vector3<double>& in_target )		{ m_spherical_transform->m_target_geo = in_target; }
		//void set_direction_geo( vector3<double>& in_direction ) { m_spherical_transform->m_direction_geo = in_direction; }
		//void set_up_geo( vector3<double>& in_up )				{ m_spherical_transform->m_up_geo = in_up; }

		vector3<double> get_position_geo(void) { return m_spherical_transform->m_position_geo; }
		vector3<double> get_target_geo(void) { return m_spherical_transform->m_target_geo; }
		vector3<double> get_direction_geo(void) { return m_spherical_transform->m_direction_geo; }
		vector3<double> get_up_geo(void) { return m_spherical_transform->m_up_geo; }

		vector3<double> get_direction_cart(void) { return m_spherical_transform->m_direction_cart; }
		vector3<double> get_up_cart(void) { return m_spherical_transform->m_up_cart; }

		vector3<double> get_position_sphr(void) { return m_spherical_transform->m_position_sphr; }
		vector3<double> get_target_sphr(void) { return m_spherical_transform->m_target_sphr; }

		//void set_longitude( double val ) { m_spherical_transform->m_longitude = val; }
		//void set_latitude( double val ) { m_spherical_transform->m_latitude = val; }
		//void set_altitude( double val ) { m_spherical_transform->m_altitude = val; }

		double get_longitude(void) { return m_spherical_transform->m_longitude; }
		double get_latitude(void) { return m_spherical_transform->m_latitude; }
		double get_altitude(void) { return m_spherical_transform->m_altitude; }

		//void set_heading( double val ) { m_spherical_transform->m_heading = val; }
		//void set_tilt( double val ) { m_spherical_transform->m_tilt = val; }
		//void set_roll( double val ) { m_spherical_transform->m_roll = val; }
		//void set_range( double val ) { m_spherical_transform->m_range = val; }

		double get_heading(void) { return m_spherical_transform->m_heading; }
		double get_tilt(void) { return m_spherical_transform->m_tilt; }
		double get_roll(void) { return m_spherical_transform->m_roll; }
		double get_range(void) { return m_spherical_transform->m_range; }

	public:
		virtual double get_max_visible_range(void) { return m_max_visible_range; }
		virtual void   set_max_visible_range(double visible_range) { m_max_visible_range = visible_range; }

		virtual double get_min_visible_range(void) { return m_min_visible_range; }
		virtual void   set_min_visible_range(double visible_range) { m_min_visible_range = visible_range; }

		virtual lon_lat_rect get_lon_lat_rect(void);

		// editable object;
	public:
		virtual void translate_object(vector_3d& vec);
		virtual void rotate_object(quaternion_4d& quater);
		virtual void scale_object(vector_3d& vec);

	public:
		virtual void copy_from(spatial_object* v_obj);

	protected:
		double	m_max_visible_range;

		double	m_min_visible_range;

		spherical_transform* m_spherical_transform;

		spatial_reference* m_spatial_reference;

	public:
		spherical_object_state  m_state;
	};
}

#endif // _US_SPHERICAL_OBJECT_H_