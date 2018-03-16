///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_geometry_textured_face.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : geometry_textured_face class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_geometry_textured_face.h"

#include <usCore/us_render_state.h>
#include <usCore/us_texture_state.h>
#include <usCore/us_render_argument.h>
#include <usCore/us_render_device.h>
#include <usCore/us_hardware_texture.h>
#include <usUtil/us_cartesian_coords.h>
#include <d3d9types.h>

namespace uniscope_globe
{
	geometry_textured_face::geometry_textured_face(void)
	{
		m_center = vector_3d::s_zero;
	}

	geometry_textured_face::~geometry_textured_face(void)
	{

	}

	// override from render_object
	void geometry_textured_face::draw(render_argument* args)
	{
		render_state rs(args->m_device);
		rs.set_state(D3DRS_ZENABLE, TRUE);
		rs.set_state(D3DRS_ZWRITEENABLE, TRUE);
		float slope_scale_depth_bias = 1.0f;
		rs.set_state(D3DRS_SLOPESCALEDEPTHBIAS, *((DWORD*)&slope_scale_depth_bias));
		rs.set_state(D3DRS_ALPHATESTENABLE, TRUE);
		rs.set_state(D3DRS_ALPHABLENDENABLE, TRUE);
		rs.set_state(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		rs.set_state(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		rs.set_state(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		rs.set_state(D3DRS_ALPHAREF, 15);
		rs.set_state(D3DRS_CULLMODE, D3DCULL_NONE);

		texture_state ts(args->m_device);
		ts.set_state(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		ts.set_state(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

		ts.set_state(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
		ts.set_state(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

		args->m_device->set_vertex_declaration(position_texture::fvf);

		if (m_texture)
		{
			args->m_device->set_texture(0, m_texture->get_handle());
		}
		else
		{
			args->m_device->set_texture(0, NULL);
		}

		matrix_4d v_mat = m_mat;

		v_mat.m41 -= cartesian_coords::s_reference_position_geo.x;
		v_mat.m42 -= cartesian_coords::s_reference_position_geo.y;
		v_mat.m43 -= cartesian_coords::s_reference_position_geo.z;

		args->m_device->push_transform();
		args->m_device->multiply_transform(matrix_4f(v_mat.m));

		args->m_device->draw_triangle_strip(m_vertex_array.begin()._Ptr, m_vertex_array.size(), sizeof(position_texture));

		args->m_device->pop_transform();
	}

	void geometry_textured_face::set_matrix(matrix_4d& mat)
	{
		m_mat = mat;
	}

	void geometry_textured_face::set_texture(hardware_texture* v_texture)
	{
		m_texture = v_texture;
	}
	// 
	// 	void geometry_textured_face::set_location( vector_3d vec_center )
	// 	{
	// 		m_center = vec_center;
	// 	}

		// 平铺纹理：width方向用grid，length方向用scale；
	void geometry_textured_face::create_textured_face_vertical(std::vector<vector_3d> vers, double width, double width_grid, double length_scale, double length_begin)
	{
		float tv_bottom = width_grid;
		float length_curr = length_begin;
		// 计算顶点和纹理坐标，构建triangle strip；
		int ver_num = vers.size();
		for (int i = 0; i < ver_num; i++)
		{
			if (i != 0)
				length_curr += (vers[i] - vers[i - 1]).length();
			float tu = length_curr / length_scale;

			position_texture pos;
			pos.x = vers[i].x;
			pos.y = vers[i].y;
			pos.z = vers[i].z;
			pos.tu1 = tu;
			pos.tv1 = tv_bottom;
			m_vertex_array.push_back(pos);

			pos.y += width;
			pos.tv1 = 0;
			m_vertex_array.push_back(pos);
		}
	}
}