///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_geometry_mark.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : geometry_mark class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_geometry_mark.h"
#include <usCore/us_render_device.h>
#include <usCore/us_hardware_texture.h>
#include <d3dx9core.h>
namespace uniscope_globe
{
	geometry_mark::geometry_mark(void)
	{
		m_texture = NULL;
		m_mark_color = 0xffffffff;
		m_font_color = 0x00005500;
	}

	geometry_mark::~geometry_mark(void)
	{
		m_texture = NULL;
	}

	void geometry_mark::draw(observer_base* v_obsr, render_device* device)
	{
		LPD3DXSPRITE p_sprite = LPD3DXSPRITE(device->get_sprite());
		LPD3DXFONT   p_font = LPD3DXFONT(device->get_font());

		p_sprite->SetTransform((D3DXMATRIX*)(&m_mark_matrix));
		p_sprite->Draw((LPDIRECT3DTEXTURE9)m_texture->get_handle(), NULL, (D3DXVECTOR3*)&m_mark_cen, (D3DXVECTOR3*)&m_mark_pos, m_mark_color);

		p_sprite->SetTransform((D3DXMATRIX*)(&m_font_matrix));
		p_font->DrawText(p_sprite, m_text.c_str(), -1, (RECT*)(&m_font_rect), DT_LEFT | DT_VCENTER | DT_NOCLIP, m_font_color);
	}

	void geometry_mark::set_texture(hardware_texture* v_texture)
	{
		m_texture = v_texture;
	}

	void geometry_mark::set_mark_info(matrix_4f& v_mat, vector_3f& v_pos, vector_3f& v_cen, ulong v_color)
	{
		m_mark_matrix = v_mat;
		m_mark_pos = v_pos;
		m_mark_cen = v_cen;
		m_mark_color = v_color;
	}

	void geometry_mark::set_font_info(const LPCTSTR str_text, matrix_4f& v_mat, rect& v_rect, ulong v_color)
	{
		m_text = str_text;
		m_font_matrix = v_mat;
		m_font_rect = v_rect;
		m_font_color = v_color;
	}
}