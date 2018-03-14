
///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_observer_helper.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : observer_helper class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_observer_helper.h"
#include <usUtil/us_matrix4.h>
#include <usCore/us_observer_base.h>
#include <usUtil/us_cartesian_coords.h>
#include <usUtil/us_axis_aligned_box.h>

namespace uniscope_globe
{
	vector_3d observer_helper::projection_goecentric(observer_base* in_observer, const vector_3d& vector_geo)
	{
		vector3<double> vector_in_clip = vector_geo * in_observer->m_view_matrix_geo;
		vector_in_clip = matrix4<double>::transform_coordinate(vector_in_clip, in_observer->m_projection_matrix);

		//if( vector_in_clip.x < -1.0 || 
		//	vector_in_clip.x > 1.0  || 
		//	vector_in_clip.y < -1.0 || 
		//	vector_in_clip.y > 1.0 ||
		//	vector_in_clip.z < 0.0 ||
		//	vector_in_clip.z > 1.0)
		//{
		//	return vector3<double>::s_zero;
		//}

		vector_in_clip.x = ((vector_in_clip.x + 1) * in_observer->m_view_port.m_width) / 2.0 + in_observer->m_view_port.m_x;

		vector_in_clip.y = ((vector_in_clip.y - 1) * in_observer->m_view_port.m_height) / -2.0 + in_observer->m_view_port.m_y;

		return vector_in_clip;
	}

	vector_3d observer_helper::projection_world(observer_base* in_observer, const vector_3d& vector_world)
	{
		vector3<double> vector_in_clip = vector_world * in_observer->m_view_matrix_rp;
		vector_in_clip = matrix4<double>::transform_coordinate(vector_in_clip, in_observer->m_projection_matrix);

		//if( vector_in_clip.x < -1.0 || 
		//	vector_in_clip.x > 1.0  || 
		//	vector_in_clip.y < -1.0 || 
		//	vector_in_clip.y > 1.0 ||
		//	vector_in_clip.z < 0.0 ||
		//	vector_in_clip.z > 1.0)
		//{
		//	return vector3<double>::s_zero;
		//}

		vector_in_clip.x = ((vector_in_clip.x + 1) * in_observer->m_view_port.m_width) / 2.0 + in_observer->m_view_port.m_x;

		vector_in_clip.y = ((vector_in_clip.y - 1) * in_observer->m_view_port.m_height) / -2.0 + in_observer->m_view_port.m_y;

		return vector_in_clip;
	}

	vector_3f observer_helper::projection_world_f(observer_base* in_observer, const vector_3d& vector_world)
	{
		vector3<double> vector_in_clip = vector_world * in_observer->m_view_matrix_rp;
		vector_in_clip = matrix4<double>::transform_coordinate(vector_in_clip, in_observer->m_projection_matrix);

		//if( vector_in_clip.x < -1.0 || 
		//	vector_in_clip.x > 1.0  || 
		//	vector_in_clip.y < -1.0 || 
		//	vector_in_clip.y > 1.0 ||
		//	vector_in_clip.z < 0.0 ||
		//	vector_in_clip.z > 1.0)
		//{
		//	return vector3<double>::s_zero;
		//}

		vector_in_clip.x = ((vector_in_clip.x + 1) * in_observer->m_view_port.m_width) / 2.0 + in_observer->m_view_port.m_x;

		vector_in_clip.y = ((vector_in_clip.y - 1) * in_observer->m_view_port.m_height) / -2.0 + in_observer->m_view_port.m_y;

		return vector_3f(vector_in_clip.x, vector_in_clip.y, vector_in_clip.z);
	}

	vector_3d observer_helper::unprojection_geocentric(observer_base* in_observer, const vector_3d& vector_on_viewport)
	{
		vector3<double> vector_in_clip;

		vector_in_clip.x = 2.0 * (vector_on_viewport.x - in_observer->m_view_port.m_x) / in_observer->m_view_port.m_width - 1.0;
		vector_in_clip.y = 2.0 * (in_observer->m_view_port.m_y - vector_on_viewport.y) / in_observer->m_view_port.m_height + 1.0;
		vector_in_clip.z = vector_on_viewport.z;

		vector3<double> vector_in_world = matrix4<double>::transform_coordinate(vector_in_clip, in_observer->m_projection_matrix.inverse() * in_observer->m_view_matrix_geo.inverse());

		return vector_in_world;
	}

	vector_3d observer_helper::unprojection_world(observer_base* in_observer, const vector_3d& vector_on_viewport)
	{
		vector3<double> vector_in_clip;

		vector_in_clip.x = 2.0 * (vector_on_viewport.x - in_observer->m_view_port.m_x) / in_observer->m_view_port.m_width - 1.0;
		vector_in_clip.y = 2.0 * (in_observer->m_view_port.m_y - vector_on_viewport.y) / in_observer->m_view_port.m_height + 1.0;
		vector_in_clip.z = vector_on_viewport.z;

		vector3<double> vector_in_world = matrix4<double>::transform_coordinate(vector_in_clip, in_observer->m_projection_matrix.inverse() * in_observer->m_view_matrix_rp.inverse());

		return vector_in_world;
	}

	void observer_helper::project_line_geo(observer_base* in_observer, coordinates_array_f& out_vec_array_scrn, coordinates_array_d& in_vec_array_geo)
	{
		matrix_4f proj_matrix = matrix_4f((double*)&(in_observer->m_projection_matrix));

		matrix_4f v_mat_world_view = matrix_4f((double*)&(in_observer->m_view_matrix_rp));

		int n_count = in_vec_array_geo.size();
		for (int ni = 0; ni < n_count - 1; ni++)
		{
			vector3<float> vec1 = cartesian_coords::from_geocentric_f(in_vec_array_geo[ni]);
			vector3<float> vec2 = cartesian_coords::from_geocentric_f(in_vec_array_geo[ni + 1]);

			vector3<float> v1 = vec1 * v_mat_world_view;
			vector3<float> v2 = vec2 * v_mat_world_view;
			if (v1.z < 0 && v2.z < 0)
			{
				continue;
			}
			else if (v2.z < 0)
			{
				vector3<float> v3 = vector3<float>::normalize(v2 - v1);
				v2 = v3 * (v2 - v1).length() * fabs((v1.z - in_observer->m_near_plane) / (v1.z - v2.z)) + v1;
			}
			else if (v1.z < 0)
			{
				vector3<float> v3 = vector3<float>::normalize(v1 - v2);
				v1 = v3 * (v1 - v2).length() * fabs((v2.z - in_observer->m_near_plane) / (v2.z - v1.z)) + v2;
			}

			out_vec_array_scrn.push_back(v1);
			out_vec_array_scrn.push_back(v2);

			out_vec_array_scrn[ni] = v1;
			out_vec_array_scrn[ni + 1] = v2;
		}

		for (int ni = 0; ni < out_vec_array_scrn.size(); ni++)
		{
			vector_3f vec_trans = matrix_4f::transform_coordinate(out_vec_array_scrn[ni], proj_matrix);

			out_vec_array_scrn[ni].x = in_observer->m_view_port.m_x + (1.0f + vec_trans.x) * in_observer->m_view_port.m_width / 2.0f;
			out_vec_array_scrn[ni].y = in_observer->m_view_port.m_y + (1.0f - vec_trans.y) * in_observer->m_view_port.m_height / 2.0f;
			//out_vec_array[ni].z = viewport.m_z_min + vec_trans.z * ( viewport.m_z_max - viewport.m_z_min );
			out_vec_array_scrn[ni].z = 0;
		}
	}

	void observer_helper::project_line_cart_strip_ex(observer_base* in_observer, coordinates_array& out_vec_array_scrn, coordinates_array_d& in_vec_array_cart, matrix_4d& tnb_matrix)
	{
		matrix_4d proj_matrix = in_observer->m_projection_matrix;
		matrix_4d v_mat_world_view = in_observer->m_view_matrix_geo;
		matrix_4d v_mat_model = tnb_matrix;

		//v_mat_model.m41 = v_mat_model.m41 - cartesian_coords::s_reference_position_geo.x;
		//v_mat_model.m42 = v_mat_model.m42 - cartesian_coords::s_reference_position_geo.y;
		//v_mat_model.m43 = v_mat_model.m43 - cartesian_coords::s_reference_position_geo.z;

		matrix_4d v_mat;
		v_mat.identity();
		v_mat = v_mat_model * v_mat_world_view;
		v_mat = v_mat * proj_matrix;

		axis_aligned_box_d v_aabb_d(vector_3d(-1.0, -1.0, 0.0), vector_3d(1.0, 1.0, 1.0));

		int n_count = in_vec_array_cart.size();
		for (int ni = 0; ni < n_count - 1; ni++)
		{
			vector_3d vec1 = in_vec_array_cart[ni];
			vector_3d vec2 = in_vec_array_cart[ni + 1];

			vector_3d v1 = matrix_4d::transform_coordinate(vec1, v_mat);
			vector_3d v2 = matrix_4d::transform_coordinate(vec2, v_mat);

			bool bv1 = v_aabb_d.inside(v1);
			bool bv2 = v_aabb_d.inside(v2);

			if (!bv1 && !bv2)
			{
				out_vec_array_scrn.push_back(vector_3d(0.0, 0.0, -1.0));
				continue;
			}

			out_vec_array_scrn.push_back(v1);
			out_vec_array_scrn.push_back(v2);

			ni++;
		}
	}

	void observer_helper::project_line_sphr(observer_base* in_observer, coordinates_array_f& out_vec_array_scrn, coordinates_array_d& in_vec_array_sphr)
	{
		matrix_4f proj_matrix = matrix_4f((double*)&(in_observer->m_projection_matrix));

		matrix_4f v_mat_world_view = matrix_4f((double*)&(in_observer->m_view_matrix_rp));

		int n_count = in_vec_array_sphr.size();
		for (int ni = 0; ni < n_count - 1; ni++)
		{
			vector3<float> vec1 = cartesian_coords::from_spherical_f(in_vec_array_sphr[ni]);
			vector3<float> vec2 = cartesian_coords::from_spherical_f(in_vec_array_sphr[ni + 1]);

			vector3<float> v1 = vec1 * v_mat_world_view;
			vector3<float> v2 = vec2 * v_mat_world_view;
			if (v1.z < 0 && v2.z < 0)
			{
				continue;
			}
			else if (v2.z < 0)
			{
				vector3<float> v3 = vector3<float>::normalize(v2 - v1);
				v2 = v3 * (v2 - v1).length() * fabs((v1.z - in_observer->m_near_plane) / (v1.z - v2.z)) + v1;
			}
			else if (v1.z < 0)
			{
				vector3<float> v3 = vector3<float>::normalize(v1 - v2);
				v1 = v3 * (v1 - v2).length() * fabs((v2.z - in_observer->m_near_plane) / (v2.z - v1.z)) + v2;
			}

			out_vec_array_scrn[ni] = v1;
			out_vec_array_scrn[ni + 1] = v2;
		}

		for (int ni = 0; ni < n_count; ni++)
		{
			vector_3f vec_trans = matrix_4f::transform_coordinate(out_vec_array_scrn[ni], proj_matrix);

			out_vec_array_scrn[ni].x = in_observer->m_view_port.m_x + (1.0f + vec_trans.x) * in_observer->m_view_port.m_width / 2.0f;
			out_vec_array_scrn[ni].y = in_observer->m_view_port.m_y + (1.0f - vec_trans.y) * in_observer->m_view_port.m_height / 2.0f;
			//out_vec_array[ni].z = viewport.m_z_min + vec_trans.z * ( viewport.m_z_max - viewport.m_z_min );
			out_vec_array_scrn[ni].z = 0;
		}
	}

	void observer_helper::project_region_sphr(observer_base* in_observer, region& out_region_scrn, coordinates_array_d& in_vec_array_sphr)
	{
		int n_count = in_vec_array_sphr.size();
		out_region_scrn.m_point_array.resize(n_count);

		coordinates_array_f v_vec_array_scrn;
		v_vec_array_scrn.resize(n_count);
		observer_helper::project_line_sphr(in_observer, v_vec_array_scrn, in_vec_array_sphr);
		for (int ni = 0; ni < n_count; ni++)
		{
			out_region_scrn.m_point_array[ni].x = v_vec_array_scrn[ni].x;
			out_region_scrn.m_point_array[ni].y = v_vec_array_scrn[ni].y;
		}
	}

	void observer_helper::project_region_geo(observer_base* in_observer, region& out_region_scrn, coordinates_array_d& in_vec_array_geo)
	{
		int n_count = in_vec_array_geo.size();
		out_region_scrn.m_point_array.resize(n_count);

		coordinates_array_f v_vec_array_scrn;
		v_vec_array_scrn.resize(n_count);
		observer_helper::project_line_geo(in_observer, v_vec_array_scrn, in_vec_array_geo);
		for (int ni = 0; ni < n_count; ni++)
		{
			out_region_scrn.m_point_array[ni].x = v_vec_array_scrn[ni].x;
			out_region_scrn.m_point_array[ni].y = v_vec_array_scrn[ni].y;
		}
	}

	//void observer_helper::project_region_sphr( observer_base* in_observer, region& out_region_scrn, coordinates_array_d& in_vec_array_sphr )
	//{
	//	out_region_scrn.m_point_array.resize(in_vec_array_sphr.count);
	//	observer_helper::project_line_sphr( observer_base* in_observer, out_region_scrn.m_point_array, in_vec_array_sphr )
	//}


}