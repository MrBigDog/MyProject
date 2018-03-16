
///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_sorted_render_queue.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : sorted_render_queue class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
////#include "stdafx.h"
#include "us_sorted_render_queue.h"
#include <usCore/us_render_argument.h>

namespace uniscope_globe
{
	bool render_object_compare_function(render_object* ro1, render_object* ro2)
	{
		return ro1->get_priority() > ro2->get_priority();
	}

	bool render_object_compare_function_ex(render_object* ro1, render_object* ro2)
	{
		return ro1->m_render_priority > ro2->m_render_priority;
	}

	sorted_render_queue::sorted_render_queue(void)
	{

	}

	sorted_render_queue::~sorted_render_queue(void)
	{

	}

	// Ö§³Öalpha
	void sorted_render_queue::draw(render_argument* args)
	{
		{
			US_LOCK_AUTO_MUTEX

				if (args->m_render_flag & 0x1000)
				{
					args->m_render_flag |= US_MATERIAL_TYPE_NORMAL | US_MATERIAL_TYPE_DYNAMIC | US_MATERIAL_TYPE_ALPHA_TEST | US_MATERIAL_TYPE_ALPHA_BLEND;
				}
				else
				{
					args->m_render_flag |= US_MATERIAL_TYPE_NORMAL | US_MATERIAL_TYPE_DYNAMIC;
				}



			// set cur render state
			render_state v_render_state(args->m_device);
			if (system_environment::s_all_model_cull_none)
			{
				v_render_state.set_state(D3DRS_CULLMODE, D3DCULL_NONE);
			}

			v_render_state.set_state(D3DRS_ALPHATESTENABLE, TRUE);
			v_render_state.set_state(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
			v_render_state.set_state(D3DRS_ALPHAREF, 15);

			v_render_state.set_state(D3DRS_ALPHABLENDENABLE, TRUE);
			v_render_state.set_state(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			v_render_state.set_state(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

			texture_state v_ts(args->m_device);
			v_ts.set_state(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
			v_ts.set_state(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			v_ts.set_state(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

			v_ts.set_state(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
			v_ts.set_state(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			v_ts.set_state(1, D3DTSS_COLORARG2, D3DTA_CURRENT);

			v_ts.set_state(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
			v_ts.set_state(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			v_ts.set_state(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

			v_ts.set_state(1, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
			v_ts.set_state(1, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
			v_ts.set_state(1, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

			render_object_array::iterator itr = m_current_render_array->begin();
			for (; itr != m_current_render_array->end(); itr++)
			{
				if ((*itr)->get_blend_color() != 0x00000000)
				{
					render_state v_child_render_state(args->m_device);
					v_child_render_state.set_state(D3DRS_TEXTUREFACTOR, (*itr)->get_blend_color());

					texture_state v_child_ts(args->m_device);
					v_child_ts.set_state(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

					(*itr)->draw(args);
				}
				else
				{
					(*itr)->draw(args);
				}
			}

		}

		render_node::draw(args);
	}


	void sorted_render_queue::draw_alpha(render_argument* args)
	{
		if (args->m_render_flag & 0x1000) return;
		draw_alpha_test(args);

		draw_alpha_blend(args);

		draw_water(args);
	}

	void sorted_render_queue::draw_alpha_test(render_argument* args)
	{
		US_LOCK_AUTO_MUTEX;

		args->m_render_flag |= US_MATERIAL_TYPE_ALPHA_TEST;

		{
			render_state v_render_state(args->m_device);
			v_render_state.set_state(D3DRS_ALPHABLENDENABLE, FALSE);

			v_render_state.set_state(D3DRS_ZENABLE, TRUE);
			v_render_state.set_state(D3DRS_ZWRITEENABLE, TRUE);
			v_render_state.set_state(D3DRS_ALPHATESTENABLE, TRUE);
			v_render_state.set_state(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
			v_render_state.set_state(D3DRS_ALPHAREF, 165);

			float slopeScaleDepthBias = 0.0f;
			v_render_state.set_state(D3DRS_SLOPESCALEDEPTHBIAS, *((DWORD*)&slopeScaleDepthBias));

			texture_state v_ts(args->m_device);

			v_ts.set_state(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
			v_ts.set_state(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

			v_ts.set_state(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
			v_ts.set_state(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

			v_ts.set_state(1, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
			v_ts.set_state(1, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
			v_ts.set_state(1, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

			v_ts.set_state(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
			v_ts.set_state(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			v_ts.set_state(1, D3DTSS_COLORARG2, D3DTA_CURRENT);

			render_object_array::iterator itr = m_current_render_array->begin();
			for (; itr != m_current_render_array->end(); itr++)
			{
				if ((*itr)->get_blend_color() != 0x00000000)
				{
					render_state v_child_render_state(args->m_device);
					v_child_render_state.set_state(D3DRS_TEXTUREFACTOR, (*itr)->get_blend_color());

					texture_state v_child_ts(args->m_device);
					v_child_ts.set_state(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

					(*itr)->draw(args);
				}
				else
				{
					(*itr)->draw(args);
				}
			}
		}

		{
			render_state v_render_state(args->m_device);
			v_render_state.set_state(D3DRS_ALPHABLENDENABLE, TRUE);
			v_render_state.set_state(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			v_render_state.set_state(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

			v_render_state.set_state(D3DRS_ZENABLE, TRUE);
			v_render_state.set_state(D3DRS_ZWRITEENABLE, FALSE);
			v_render_state.set_state(D3DRS_ALPHATESTENABLE, TRUE);
			v_render_state.set_state(D3DRS_ALPHAFUNC, D3DCMP_LESSEQUAL);
			v_render_state.set_state(D3DRS_ALPHAREF, 165);

			float slopeScaleDepthBias = 0.0f;
			v_render_state.set_state(D3DRS_SLOPESCALEDEPTHBIAS, *((DWORD*)&slopeScaleDepthBias));

			texture_state v_ts(args->m_device);

			v_ts.set_state(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
			v_ts.set_state(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

			v_ts.set_state(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
			v_ts.set_state(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

			v_ts.set_state(1, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
			v_ts.set_state(1, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
			v_ts.set_state(1, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

			v_ts.set_state(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
			v_ts.set_state(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			v_ts.set_state(1, D3DTSS_COLORARG2, D3DTA_CURRENT);

			render_object_array::iterator itr = m_current_render_array->begin();
			for (; itr != m_current_render_array->end(); itr++)
			{
				if ((*itr)->get_blend_color() != 0x00000000)
				{
					render_state v_child_render_state(args->m_device);
					v_child_render_state.set_state(D3DRS_TEXTUREFACTOR, (*itr)->get_blend_color());

					texture_state v_child_ts(args->m_device);
					v_child_ts.set_state(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

					(*itr)->draw(args);
				}
				else
				{
					(*itr)->draw(args);
				}
			}
		}
		args->m_render_flag ^= US_MATERIAL_TYPE_ALPHA_TEST;

	}

	void sorted_render_queue::draw_alpha_blend(render_argument* args)
	{
		US_LOCK_AUTO_MUTEX;

		args->m_render_flag |= US_MATERIAL_TYPE_ALPHA_BLEND;

		{
			render_state v_render_state(args->m_device);

			v_render_state.set_state(D3DRS_ALPHABLENDENABLE, TRUE);
			v_render_state.set_state(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			v_render_state.set_state(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

			v_render_state.set_state(D3DRS_ZENABLE, TRUE);
			v_render_state.set_state(D3DRS_ZWRITEENABLE, FALSE);
			v_render_state.set_state(D3DRS_ALPHATESTENABLE, FALSE);

			float slopeScaleDepthBias = 0.0f;
			v_render_state.set_state(D3DRS_SLOPESCALEDEPTHBIAS, *((DWORD*)&slopeScaleDepthBias));

			texture_state v_ts(args->m_device);

			v_ts.set_state(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
			v_ts.set_state(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

			v_ts.set_state(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
			v_ts.set_state(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

			v_ts.set_state(1, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
			v_ts.set_state(1, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
			v_ts.set_state(1, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

			v_ts.set_state(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
			v_ts.set_state(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			v_ts.set_state(1, D3DTSS_COLORARG2, D3DTA_CURRENT);

			render_object_array::iterator itr = m_current_render_array->begin();
			for (; itr != m_current_render_array->end(); itr++)
			{
				if ((*itr)->get_blend_color() != 0x00000000)
				{
					render_state v_child_render_state(args->m_device);
					v_child_render_state.set_state(D3DRS_TEXTUREFACTOR, (*itr)->get_blend_color());

					texture_state v_child_ts(args->m_device);
					v_child_ts.set_state(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

					(*itr)->draw(args);
				}
				else
				{
					(*itr)->draw(args);
				}
			}
		}

		{
			render_state v_render_state(args->m_device);

			v_render_state.set_state(D3DRS_ALPHABLENDENABLE, TRUE);
			v_render_state.set_state(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			v_render_state.set_state(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

			v_render_state.set_state(D3DRS_ZENABLE, TRUE);
			v_render_state.set_state(D3DRS_ZWRITEENABLE, TRUE);
			v_render_state.set_state(D3DRS_ALPHATESTENABLE, FALSE);

			float slopeScaleDepthBias = 0.0f;
			v_render_state.set_state(D3DRS_SLOPESCALEDEPTHBIAS, *((DWORD*)&slopeScaleDepthBias));

			texture_state v_ts(args->m_device);

			v_ts.set_state(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
			v_ts.set_state(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

			v_ts.set_state(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
			v_ts.set_state(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

			v_ts.set_state(1, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
			v_ts.set_state(1, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
			v_ts.set_state(1, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

			v_ts.set_state(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
			v_ts.set_state(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			v_ts.set_state(1, D3DTSS_COLORARG2, D3DTA_CURRENT);

			render_object_array::iterator itr = m_current_render_array->begin();
			for (; itr != m_current_render_array->end(); itr++)
			{
				if ((*itr)->get_blend_color() != 0x00000000)
				{
					render_state v_child_render_state(args->m_device);
					v_child_render_state.set_state(D3DRS_TEXTUREFACTOR, (*itr)->get_blend_color());

					texture_state v_child_ts(args->m_device);
					v_child_ts.set_state(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

					(*itr)->draw(args);
				}
				else
				{
					(*itr)->draw(args);
				}
			}
		}
		args->m_render_flag ^= US_MATERIAL_TYPE_ALPHA_BLEND;
	}

	void sorted_render_queue::flush(void)
	{
		//US_RENDER_TRACE1( L"sorted ro : %d\n", (int)m_background_render_array->size() );

		// sort render_object
		std::sort(m_background_render_array->begin(), m_background_render_array->end(), render_object_compare_function);

		// begin flip render_object
		{
			US_LOCK_AUTO_MUTEX;

			render_object_array* temp_array = m_current_render_array;

			m_current_render_array = m_background_render_array;

			m_background_render_array = temp_array;
		}

		clear_background();
	}


}