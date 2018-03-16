///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_geometry_render_blend.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : geometry_render_blend class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_geometry_render_blend.h"
#include <usGfx/us_d3d9_effect_common_mesh_fp.h>
#include <usGfx/us_render_object_geometry_mesh.h>
#include <usGfx/us_render_object_geometry_textured_mesh.h>

#include <usCore/us_render_argument.h>
#include <usCore/us_render_state.h>
#include <usCore/us_system_environment.h>

namespace uniscope_globe
{
	geometry_render_blend::geometry_render_blend(render_device* device)
	{
		m_render = new d3d9_effect_common_mesh_fp(device);
	}

	geometry_render_blend::~geometry_render_blend(void)
	{
		m_current_render_array->clear();
		m_background_render_array->clear();
		AUTO_DELETE(m_render);
	}

	void geometry_render_blend::set_render(d3d9_effect_common_mesh* v_render)
	{
		m_render = v_render;
	}

	void geometry_render_blend::draw(render_argument* args)
	{
		render_effect* v_render = args->m_render;
		{
			US_LOCK_AUTO_MUTEX

				//args->m_render = m_render;

			{
				render_state v_render_state(args->m_device);
				v_render_state.set_state(D3DRS_ALPHATESTENABLE, FALSE);
				v_render_state.set_state(D3DRS_ALPHABLENDENABLE, TRUE);
				v_render_state.set_state(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				v_render_state.set_state(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

				v_render_state.set_state(D3DRS_ZENABLE, TRUE);
				v_render_state.set_state(D3DRS_ZWRITEENABLE, TRUE);

				texture_state v_ts(args->m_device);
				v_ts.set_state(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
				v_ts.set_state(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);

				v_ts.set_state(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
				v_ts.set_state(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);

				render_object_array::iterator itr = m_current_render_array->begin();
				for (; itr != m_current_render_array->end(); itr++)
				{
					if ((*itr)->get_blend_color() != 0x00000000)
					{
						render_state v_child_render_state(args->m_device);
						v_child_render_state.set_state(D3DRS_TEXTUREFACTOR, (*itr)->get_blend_color());

						texture_state v_child_texture_state(args->m_device);
						v_child_texture_state.set_state(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
						v_child_texture_state.set_state(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
						v_child_texture_state.set_state(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

						v_child_texture_state.set_state(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
						v_child_texture_state.set_state(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
						v_child_texture_state.set_state(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
					}

					switch ((*itr)->get_rtti())
					{
					case US_RTTI_GEOMETRY_TEXTURED_MESH:
						{
						//((render_object_geometry_textured_mesh*)(*itr))->draw_figure( args );
					}
					break;
				case US_RTTI_GEOMETRY_MESH:
					{
						((render_object_geometry_mesh*)(*itr))->draw_figure(args);
					}
					break;
				}
			}
			}

			{
				render_state v_render_state(args->m_device);
				v_render_state.set_state(D3DRS_ALPHATESTENABLE, FALSE);
				v_render_state.set_state(D3DRS_ALPHABLENDENABLE, TRUE);
				v_render_state.set_state(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				v_render_state.set_state(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

				v_render_state.set_state(D3DRS_ZENABLE, TRUE);
				v_render_state.set_state(D3DRS_ZWRITEENABLE, FALSE);

				texture_state v_ts(args->m_device);
				v_ts.set_state(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
				v_ts.set_state(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
				v_ts.set_state(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);

				v_ts.set_state(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
				v_ts.set_state(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
				v_ts.set_state(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

				render_object_array::iterator itr = m_current_render_array->begin();
				for (; itr != m_current_render_array->end(); itr++)
				{
					if ((*itr)->get_blend_color() != 0x00000000)
					{
						render_state v_child_render_state(args->m_device);
						v_child_render_state.set_state(D3DRS_TEXTUREFACTOR, (*itr)->get_blend_color());

						texture_state v_child_texture_state(args->m_device);
						v_child_texture_state.set_state(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
						v_child_texture_state.set_state(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
						v_child_texture_state.set_state(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

						v_child_texture_state.set_state(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
						v_child_texture_state.set_state(1, D3DTSS_COLORARG1, D3DTA_CURRENT);
						v_child_texture_state.set_state(1, D3DTSS_COLORARG2, D3DTA_TEXTURE);

						v_child_texture_state.set_state(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
						v_child_texture_state.set_state(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
						v_child_texture_state.set_state(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

						v_child_texture_state.set_state(1, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
						v_child_texture_state.set_state(1, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
						v_child_texture_state.set_state(1, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);
					}

					switch ((*itr)->get_rtti())
					{
					case US_RTTI_GEOMETRY_TEXTURED_MESH:
					{
						((render_object_geometry_textured_mesh*)(*itr))->draw(args);
					}
					break;
					case US_RTTI_GEOMETRY_MESH:
					{
						((render_object_geometry_mesh*)(*itr))->draw_without_figure(args);
					}
					break;
					default:
					{
						(*itr)->draw(args);
					}
					break;
					}
				}
			}

			{
				render_state v_render_state(args->m_device);
				if (system_environment::s_all_model_cull_none)
				{
					v_render_state.set_state(D3DRS_CULLMODE, D3DCULL_NONE);
				}
				v_render_state.set_state(D3DRS_ALPHATESTENABLE, FALSE);
				v_render_state.set_state(D3DRS_ALPHABLENDENABLE, TRUE);
				v_render_state.set_state(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				v_render_state.set_state(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

				v_render_state.set_state(D3DRS_ZENABLE, TRUE);
				v_render_state.set_state(D3DRS_ZWRITEENABLE, TRUE);

				texture_state v_ts(args->m_device);
				v_ts.set_state(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
				v_ts.set_state(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
				v_ts.set_state(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);

				v_ts.set_state(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
				v_ts.set_state(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
				v_ts.set_state(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

				render_object_array::iterator itr = m_current_render_array->begin();
				for (; itr != m_current_render_array->end(); itr++)
				{
					if ((*itr)->get_blend_color() != 0x00000000)
					{
						render_state v_child_render_state(args->m_device);
						v_child_render_state.set_state(D3DRS_TEXTUREFACTOR, (*itr)->get_blend_color());

						texture_state v_child_texture_state(args->m_device);
						v_child_texture_state.set_state(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
						v_child_texture_state.set_state(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
						v_child_texture_state.set_state(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

						v_child_texture_state.set_state(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
						v_child_texture_state.set_state(1, D3DTSS_COLORARG1, D3DTA_CURRENT);
						v_child_texture_state.set_state(1, D3DTSS_COLORARG2, D3DTA_TEXTURE);

						v_child_texture_state.set_state(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
						v_child_texture_state.set_state(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
						v_child_texture_state.set_state(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

						v_child_texture_state.set_state(1, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
						v_child_texture_state.set_state(1, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
						v_child_texture_state.set_state(1, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);
					}

					switch ((*itr)->get_rtti())
					{
					case US_RTTI_GEOMETRY_TEXTURED_MESH:
					{
						((render_object_geometry_textured_mesh*)(*itr))->draw(args);
					}
					break;
					case US_RTTI_GEOMETRY_MESH:
					{
						((render_object_geometry_mesh*)(*itr))->draw_without_figure(args);
					}
					break;
					default:
					{
						(*itr)->draw(args);
					}
					break;
					}

					//((render_object_geometry_mesh*)(*itr))->draw_without_figure( args );
				}
			}

		}

		args->m_render = v_render;
		render_node::draw(args);
	}

	void geometry_render_blend::clear_background(void)
	{
		m_background_render_array->clear();
	}



}