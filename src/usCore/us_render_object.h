///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_render_object.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : render_object class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_RENDER_OBJECT_H_
#define _US_RENDER_OBJECT_H_

#include <usCore\Export.h>
#include <usCore\us_shared_data.h>
#include <usCore\us_intersect_base.h>
#include <usCore\us_renderable_base.h>
#include <usUtil\us_object_base.h>

namespace uniscope_globe
{
	class render_device;
	class render_argument;

	class USCORE_EXPORT render_object
		: virtual public object_base
		, virtual public shared_data
		, public intersect_base
		, public renderable_base
	{
	public:
		render_object(void);

		virtual ~render_object(void);

	public:
		virtual void draw(render_argument* args) {}

		virtual void draw_alpha(render_argument* args) { }

		virtual void occlusion(render_argument* args) {}

	public:
		virtual double get_priority(void) { return m_render_priority; }

		virtual void set_priority(double v_priority) { m_render_priority = v_priority; }

		void set_render_aabb(axis_aligned_box_d& v_aabb) { m_render_aabb = v_aabb; }

		axis_aligned_box_d get_render_aabb(void) { return m_render_aabb; }

		virtual void set_world_matrix(matrix4<double>& v_mat) { }

		virtual matrix4<double> get_world_matrix(void) { return matrix4<double>::s_identity; }

		virtual ulong get_time_stamp(void) { return m_time_stamp; }

		virtual void set_time_stamp(ulong v_time_stamp) { m_time_stamp = v_time_stamp; }

		virtual void set_shadow_type(ulong v_shadow_type) { m_shadow_type = v_shadow_type; }

		ulong get_shadow_type(void) { return m_shadow_type; }

		virtual void set_shadow_color(ulong v_shadow_color) { m_shadow_color = v_shadow_color; }

		ulong get_shadow_color(void) { return m_shadow_color; }

	public:
		bool operator < (const render_object& in_obj) const
		{
			return m_view_distance > in_obj.m_view_distance;
		}

	public:
		static int compare_fun(const void* ptr1, const void* ptr2);

		virtual void copy_from(render_object* v_ro);


	public:
		double m_view_distance;

		double m_render_priority;

	protected:
		render_device*		m_render_device;

		axis_aligned_box_d	m_render_aabb;

		ulong				m_time_stamp;

		ulong				m_shadow_color;

		ulong				m_shadow_type;
	};
}

#endif // _US_RENDER_OBJECT_H_