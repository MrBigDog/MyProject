///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_geometry_billboard.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : geometry_billboard class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_geometry_billboard.h"

#include <usCore/us_render_state.h>
#include <usCore/us_texture_state.h>
#include <usCore/us_render_argument.h>
#include <usCore/us_render_device.h>
#include <usCore/us_intersect_result.h>
#include <usCore/us_hardware_texture.h>

#include <usUtil/us_cartesian_coords.h>
#include <usUtil/us_geocentric_coords.h>
#include <usUtil/us_triangle.h>

#include <d3d9types.h>
#include <vector>

namespace uniscope_globe
{
	geometry_billboard::geometry_billboard(void)
	{
		m_mat = matrix_4d::s_identity;
		m_center = vector_3d::s_zero;
		m_alpha_blend = false;
	}

	geometry_billboard::~geometry_billboard(void)
	{

	}

	// override from render_object
	void geometry_billboard::draw(render_argument* args)
	{
		render_state rs(args->m_device);
		rs.set_state(D3DRS_ZENABLE, TRUE);
		rs.set_state(D3DRS_ZWRITEENABLE, TRUE);
		float slope_scale_depth_bias = 1.0f;
		rs.set_state(D3DRS_SLOPESCALEDEPTHBIAS, *((DWORD*)&slope_scale_depth_bias));
		rs.set_state(D3DRS_ALPHATESTENABLE, TRUE);

		rs.set_state(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		rs.set_state(D3DRS_ALPHAREF, 15);
		rs.set_state(D3DRS_CULLMODE, D3DCULL_NONE);

		rs.set_state(D3DRS_ALPHABLENDENABLE, m_alpha_blend);
		rs.set_state(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		rs.set_state(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);



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
			args->m_device->set_transparency_texture(0);
		}

		matrix_4d v_mat = m_mat;
		v_mat.m41 -= cartesian_coords::s_reference_position_geo.x;
		v_mat.m42 -= cartesian_coords::s_reference_position_geo.y;
		v_mat.m43 -= cartesian_coords::s_reference_position_geo.z;

		args->m_device->push_transform();
		args->m_device->multiply_transform(matrix_4f(v_mat.m));

		args->m_device->draw_triangle_strip(m_vertex_array.begin()._Ptr, 4, sizeof(position_texture));

		args->m_device->pop_transform();
	}

	void geometry_billboard::set_matrix(matrix_4d& mat)
	{
		m_mat = mat;
	}

	void geometry_billboard::set_texture(hardware_texture* v_texture)
	{
		m_texture = v_texture;
	}

	void geometry_billboard::create_billboard(int bb_type, vector_3d v_locate, double v_width, double v_height)
	{
		m_center = geocentric_coords::from_spherical_d(v_locate);

		m_vertex_array.resize(4);

		m_vertex_array[0].x = -v_width * 0.5;
		m_vertex_array[0].y = 0.0;
		m_vertex_array[0].z = 0.0;
		m_vertex_array[0].tu1 = 0.0f;
		m_vertex_array[0].tv1 = 1.0f;

		m_vertex_array[1].x = -v_width * 0.5;
		m_vertex_array[1].y = v_height;
		m_vertex_array[1].z = 0.0;
		m_vertex_array[1].tu1 = 0.0f;
		m_vertex_array[1].tv1 = 0.0f;

		m_vertex_array[2].x = v_width * 0.5;
		m_vertex_array[2].y = 0.0;
		m_vertex_array[2].z = 0.0;
		m_vertex_array[2].tu1 = 1.0f;
		m_vertex_array[2].tv1 = 1.0f;

		m_vertex_array[3].x = v_width * 0.5;
		m_vertex_array[3].y = v_height;
		m_vertex_array[3].z = 0.0;
		m_vertex_array[3].tu1 = 1.0f;
		m_vertex_array[3].tv1 = 0.0f;

	}

	bool geometry_billboard::intersect(const ray<double>& a_ray, intersect_result& result)
	{
		bool b_ret = false;
		ray<double> in_ray = a_ray;

		vector_3d pick_point;
		double pick_length = 0.0;
		//for ( int i = 0; i < m_index_array.size(); i+=3 )
		{
			vector_3d vec1(m_vertex_array[0].x, m_vertex_array[0].y, m_vertex_array[0].z);
			vector_3d vec2(m_vertex_array[1].x, m_vertex_array[1].y, m_vertex_array[1].z);
			vector_3d vec3(m_vertex_array[2].x, m_vertex_array[2].y, m_vertex_array[2].z);

			triangle_d tri(vec1, vec2, vec3);
			bool b_pick = triangle_d::intersect(in_ray, tri);
			if (triangle_d::pick(in_ray, tri, pick_point, 10000000.0))
			{
				pick_length = (pick_point - in_ray.ray_origin).length();
				if (result.m_distance > pick_length)
				{
					result.m_distance = pick_length;
					result.m_position = pick_point;
				}

				b_ret = true;
			}
		}
		if (!b_ret)
		{

			vector_3d vec1(m_vertex_array[1].x, m_vertex_array[1].y, m_vertex_array[1].z);
			vector_3d vec3(m_vertex_array[2].x, m_vertex_array[2].y, m_vertex_array[2].z);
			vector_3d vec2(m_vertex_array[3].x, m_vertex_array[3].y, m_vertex_array[3].z);

			triangle_d tri(vec1, vec2, vec3);
			bool b_pick = triangle_d::intersect(in_ray, tri);
			if (triangle_d::pick(in_ray, tri, pick_point, 10000000.0))
			{
				pick_length = (pick_point - in_ray.ray_origin).length();
				if (result.m_distance > pick_length)
				{
					result.m_distance = pick_length;
					result.m_position = pick_point;
				}

				b_ret = true;
			}

		}


		return b_ret;
	}


}