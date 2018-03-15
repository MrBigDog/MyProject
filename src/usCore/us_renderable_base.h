
///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2010 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_renderable_base.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : renderable_base class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_RENDERABLE_BASE_H_
#define _US_RENDERABLE_BASE_H_

#include <usCore\Export.h>
#include <usUtil\us_common_file.h>

namespace uniscope_globe
{
	class render_argument;

	class USCORE_EXPORT renderable_base
	{
	public:
		renderable_base(void);

		virtual ~renderable_base(void);

	public:
		virtual void set_light_enable(bool v_light_enable) { m_light_enable = v_light_enable; }

		virtual bool get_light_enable() { return m_light_enable; }

		virtual void set_blend_color(ulong v_blend_color) { m_blend_color = v_blend_color; }

		virtual ulong get_blend_color(void) { return m_blend_color; }

		virtual bool get_depth_enable(void) { return m_depth_enable; }

		virtual void set_depth_enable(bool depth_enable) { m_depth_enable = depth_enable; }

		virtual float get_depth_bias(void) { return m_depth_bias; }

		virtual void set_depth_bias(float depth_bias) { m_depth_bias = depth_bias; }

		virtual int get_draw_order(void) { return m_draw_order; }

		virtual void set_draw_order(int v_draw_order) { m_draw_order = v_draw_order; }

		virtual void set_alpha_blend(bool v_alpha_blend) { m_alpha_blend = v_alpha_blend; }

		virtual bool get_alpha_blend(void) { return m_alpha_blend; }

	public:
		virtual void show_high_light(double v_last_time, double v_interval_time, ulong v_color);

		virtual bool draw_high_light(render_argument* v_args);

		virtual void stop_high_light();

	public:
		virtual void copy_from(renderable_base* v_ro);

	protected:
		ulong				m_blend_color;

		bool				m_depth_enable;

		float				m_depth_bias;

		bool				m_alpha_blend;

		bool				m_light_enable;

		int					m_draw_order;

	protected:
		int					m_render_type_state;

		double				m_last_time;
		double				m_interval_time;
		ulong				m_high_light_color;

		double				m_flash_start_time;

	};
}

#endif // _US_RENDERABLE_BASE_H_ 