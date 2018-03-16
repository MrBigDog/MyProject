///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_render_target_manager.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_render_target_manager class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_d3d9_render_target_manager.h"
#include <usGfx/us_d3d9_render_device_impl.h>
#include <usGfx/us_d3d9_render_target_chain.h>
#include <usGfx/us_d3d9_render_target.h>
#include <usGfx/us_d3d9_render_target_ex.h>
#include <usGfx/us_d3d9_render_target_offscreen.h>

namespace uniscope_globe
{
	d3d9_render_target_manager::d3d9_render_target_manager(render_device* rd)
	{
		m_device = rd;
		m_render_target_array.resize(US_RENDER_TARGET_COUNT, NULL);
	}

	d3d9_render_target_manager::~d3d9_render_target_manager(void)
	{
		render_target_array::iterator itr = m_render_target_array.begin();
		for (; itr != m_render_target_array.end(); itr++)
		{
			AUTO_DELETE(*itr);
		}
		m_render_target_array.clear();
	}

	ULONG_PTR d3d9_render_target_manager::get_render_target(long rt_handle)
	{
		if (m_render_target_array[rt_handle] == NULL)
		{
			d3d9_render_device_impl* v_device = (d3d9_render_device_impl*)m_device;

			if (rt_handle == 2)
			{
				d3d9_render_target_chain* v_render_target = new d3d9_render_target_chain(v_device);
				v_render_target->create(v_device->m_d3d_present_param.BackBufferWidth,
					v_device->m_d3d_present_param.BackBufferHeight,
					v_device->m_d3d_present_param.BackBufferFormat,
					v_device->m_d3d_present_param.MultiSampleType,
					v_device->m_d3d_present_param.MultiSampleQuality);

				m_render_target_array[rt_handle] = v_render_target;

			}
			else if (rt_handle < 2)
			{
				d3d9_render_target* v_render_target = new d3d9_render_target(v_device);
				v_render_target->create(v_device->m_d3d_present_param.BackBufferWidth,
					v_device->m_d3d_present_param.BackBufferHeight,
					v_device->m_d3d_present_param.BackBufferFormat,
					v_device->m_d3d_present_param.MultiSampleType,
					v_device->m_d3d_present_param.MultiSampleQuality);

				m_render_target_array[rt_handle] = v_render_target;

			}
			else if (rt_handle == 3)
			{
				d3d9_render_target_ex* v_render_target = new d3d9_render_target_ex(v_device);
				v_render_target->create(v_device->m_d3d_present_param.BackBufferWidth,
					v_device->m_d3d_present_param.BackBufferHeight,
					v_device->m_d3d_present_param.BackBufferFormat,
					v_device->m_d3d_present_param.MultiSampleType,
					v_device->m_d3d_present_param.MultiSampleQuality);

				m_render_target_array[rt_handle] = v_render_target;
			}
			else if (rt_handle == 4)
			{
				d3d9_render_target_offscreen* v_render_target = new d3d9_render_target_offscreen(v_device);
				v_render_target->create(v_device->m_d3d_present_param.BackBufferWidth,
					v_device->m_d3d_present_param.BackBufferHeight,
					v_device->m_d3d_present_param.BackBufferFormat,
					v_device->m_d3d_present_param.MultiSampleType,
					v_device->m_d3d_present_param.MultiSampleQuality);

				m_render_target_array[rt_handle] = v_render_target;
			}
			else if (rt_handle == 5)
			{
				d3d9_render_target_ex* v_render_target = new d3d9_render_target_ex(v_device);
				v_render_target->create(v_device->m_d3d_present_param.BackBufferWidth,
					v_device->m_d3d_present_param.BackBufferHeight,
					v_device->m_d3d_present_param.BackBufferFormat,
					v_device->m_d3d_present_param.MultiSampleType,
					v_device->m_d3d_present_param.MultiSampleQuality);

				m_render_target_array[rt_handle] = v_render_target;
			}
		}
		return (ULONG_PTR)(m_render_target_array[rt_handle]);
	}

	long d3d9_render_target_manager::on_lost_device(void)
	{
		render_target_array::iterator itr = m_render_target_array.begin();
		for (; itr != m_render_target_array.end(); itr++)
		{
			if (*itr != NULL)
				(*itr)->on_lost_device(NULL);
		}
		return S_OK;
	}

	long d3d9_render_target_manager::on_reset_device(void)
	{
		render_target_array::iterator itr = m_render_target_array.begin();
		for (; itr != m_render_target_array.end(); itr++)
		{
			if (*itr != NULL)
				(*itr)->on_reset_device(NULL);
		}
		return S_OK;
	}

	ulong d3d9_render_target_manager::add_render_target(render_target* v_render_target)
	{
		m_render_target_array.push_back(v_render_target);
		return (int)m_render_target_array.size() - 1;
	}

	void d3d9_render_target_manager::delete_render_target(ulong rt_handle)
	{
		AUTO_DELETE(m_render_target_array[rt_handle]);
	}


}