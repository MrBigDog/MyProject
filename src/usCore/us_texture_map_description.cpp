///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_texture_fill_style.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : texture_fill_style class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_texture_map_description.h"

namespace uniscope_globe
{
	texture_map_description::texture_map_description(void)
	{
		//m_rtti = US_RTTI_TEXTURE_MAP_DESCRIPTION;
		//m_style_type = style_type::mat_style;
		m_tu_start = 0.0;
		m_tv_start = 0.0;
		m_tu_end = 1.0;
		m_tv_end = 1.0;

		m_rotation_u = 0.0;
		m_rotation_v = 0.0;
		m_rotation_w = 0.0;

		m_mirror_texture_u = false;
		m_mirror_texture_v = false;
		m_two_sided_mode = false;

		m_is_texture_matrix_dirty = true;

	}

	texture_map_description::texture_map_description(const texture_map_description& v_style)
	{
		//m_rtti = US_RTTI_TEXTURE_MAP_DESCRIPTION;

		m_tu_start = v_style.m_tu_start;
		m_tv_start = v_style.m_tv_start;
		m_tu_end = v_style.m_tu_end;
		m_tv_end = v_style.m_tv_end;

		m_rotation_u = v_style.m_rotation_u;
		m_rotation_v = v_style.m_rotation_v;
		m_rotation_w = v_style.m_rotation_w;

		m_mirror_texture_u = v_style.m_mirror_texture_u;
		m_mirror_texture_v = v_style.m_mirror_texture_v;
		m_two_sided_mode = v_style.m_two_sided_mode;

		m_is_texture_matrix_dirty = true;
	}

	texture_map_description::~texture_map_description(void)
	{

	}

	void texture_map_description::copy_texture_map_desc(texture_map_description* v_tex_map_desc)
	{
		this->m_tu_start = v_tex_map_desc->m_tu_start;
		this->m_tv_start = v_tex_map_desc->m_tv_start;
		//float	m_tw_start;

		this->m_tu_end = v_tex_map_desc->m_tu_end;		// max: 8192
		this->m_tv_end = v_tex_map_desc->m_tv_end;
		//float	m_tw_end;

		this->m_rotation_u = v_tex_map_desc->m_rotation_u;
		this->m_rotation_v = v_tex_map_desc->m_rotation_v;
		this->m_rotation_w = v_tex_map_desc->m_rotation_w;

		this->m_mirror_texture_u = v_tex_map_desc->m_mirror_texture_u;
		this->m_mirror_texture_v = v_tex_map_desc->m_mirror_texture_v;
		this->m_two_sided_mode = v_tex_map_desc->m_two_sided_mode;
		this->m_texture_matrix = v_tex_map_desc->m_texture_matrix;
		this->m_is_texture_matrix_dirty = v_tex_map_desc->m_is_texture_matrix_dirty;
	}

	matrix_4f texture_map_description::get_texture_matrix(void)
	{
		if (m_is_texture_matrix_dirty)
		{
			float scale_u = 1.0f;
			float scale_v = 1.0f;
			float translate_u = 0.0f;
			float translate_v = 0.0f;
			if (m_mirror_texture_u)
			{
				scale_u = m_tu_start - m_tu_end;
				translate_u = m_tu_end;
			}
			else
			{
				scale_u = m_tu_end - m_tu_start;
				translate_u = m_tu_start;
			}

			if (m_mirror_texture_v)
			{
				scale_v = m_tv_start - m_tv_end;
				translate_v = m_tv_end;
			}
			else
			{
				scale_v = m_tv_end - m_tv_start;
				translate_v = m_tv_start;
			}

			float center_u = translate_u + scale_u * 0.5;
			float center_v = translate_v + scale_v * 0.5;

			//float scale_v = m_tv_end - m_tv_start;
			//float scale_w = m_tw_start + m_tu_start * 0.5;
			//float center_u = m_tu_start + scale_u * 0.5;
			//float center_v = m_tv_start + scale_v * 0.5;
			//float center_w = m_tw_start + scale_w * 0.5;


			matrix4<float> mat_trans1 = matrix4<float>::translation(-0.5, -0.5, 0);
			//matrix4<float> mat_trans2 = matrix4<float>::translation(  0.5,  0.5, 0);
			matrix_4f mat_trans = matrix_4f::translation(center_u, center_v, 0);
			matrix_4f mat_rotate = matrix4<float>::rotate_z(m_rotation_w);
			matrix_4f mat_scale = matrix_4f::scaling(scale_u, scale_v, 1);

			//m_texture_matrix = mmat_trans * mat_trans1 * mat_scale * mat_rotate * mat_trans2;

			m_texture_matrix = mat_trans1 * mat_scale * mat_rotate * mat_trans;

			m_texture_matrix.m31 = m_texture_matrix.m41;
			m_texture_matrix.m32 = m_texture_matrix.m42;
			//m_texture_matrix.m33 = m_texture_matrix.m43;
			m_texture_matrix.m41 = 0.f;
			m_texture_matrix.m42 = 0.f;
			//m_texture_matrix.m43 = 0.f;


			m_is_texture_matrix_dirty = false;
		}

		return m_texture_matrix;
	}

}