///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_render_helper.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : render_helper class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_render_helper.h"
#include <usGfx/us_d3d9_effect_common_mesh.h>
#include <usGfx/us_d3d9_render_target_manager.h>
#include <usGfx/us_d3d9_effect_reflection_surface_ex.h>
#include <usGfx/us_d3d9_effect_manager.h>
#include <usGfx/us_d3d9_vertex_declear.h>
#include <usGfx/us_d3d9_hardware_texture.h>

#include <usCore/us_render_argument.h>
#include <usCore/us_render_device.h>
#include <usCore/us_render_target.h>
#include <usCore/us_system_environment.h>
#include <usCore/us_observer_base.h>
#include <usCore/us_render_state.h>
#include <usCore/us_hardware_texture.h>
#include <usCore/us_data_loader.h>
#include <usCore/us_filter_manager.h>

#include <usUtil/us_spherical_coords.h>
#include <usUtil/us_cartesian_coords.h>
#include <usUtil/us_geocentric_coords.h>
#include <usUtil/us_managed_buffer.h>

namespace uniscope_globe
{
	render_helper::render_helper(void)
	{

	}

	render_helper::~render_helper(void)
	{

	}

	void render_helper::draw_points(render_device* device, vector3<double>* vertices_buffer, ulong vertices_count, ulong vertices_color, ulong vertices_size, bool is_ztest)
	{
		position_color* vertex_array = new position_color[vertices_count];
		for (int ni = 0; ni < (int)vertices_count; ni++)
		{
			vector3<float> vec = cartesian_coords::from_geocentric_f(vertices_buffer[ni]);
			vertex_array[ni].x = vec.x;
			vertex_array[ni].y = vec.y;
			vertex_array[ni].z = vec.z;
			vertex_array[ni].color = vertices_color;
		}

		float v_point_size = (float)vertices_size;

		render_state v_render_state(device);
		v_render_state.set_state(D3DRS_ALPHATESTENABLE, FALSE);
		v_render_state.set_state(D3DRS_ALPHABLENDENABLE, TRUE);
		v_render_state.set_state(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		v_render_state.set_state(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		v_render_state.set_state(D3DRS_POINTSIZE, *((DWORD*)&v_point_size));
		v_render_state.set_state(D3DRS_POINTSPRITEENABLE, TRUE);
		v_render_state.set_state(D3DRS_ZENABLE, is_ztest);

		device->set_world_matrix(matrix4<float>::s_identity);
		device->set_vertex_declaration(position_color::fvf);
		device->draw_point_list((void*)vertex_array, vertices_count, sizeof(position_color));

		delete[] vertex_array;
	}

	void render_helper::draw_line_list(render_device* device, vector3<double>* vertices_buffer, ulong vertices_count, ulong vertices_color, bool is_ztest)
	{
		position_color* vertex_array = new position_color[vertices_count];
		for (int ni = 0; ni < (int)vertices_count; ni++)
		{
			vector3<float> vec = cartesian_coords::from_geocentric_f(vertices_buffer[ni]);
			vertex_array[ni].x = vec.x;
			vertex_array[ni].y = vec.y;
			vertex_array[ni].z = vec.z;
			vertex_array[ni].color = vertices_color;
		}

		render_state v_render_state(device);
		v_render_state.set_state(D3DRS_ALPHATESTENABLE, FALSE);
		v_render_state.set_state(D3DRS_ALPHABLENDENABLE, TRUE);
		v_render_state.set_state(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		v_render_state.set_state(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		v_render_state.set_state(D3DRS_ZENABLE, is_ztest);

		device->set_world_matrix(matrix4<float>::s_identity);
		device->set_vertex_declaration(position_color::fvf);
		device->draw_line_list((void*)vertex_array, vertices_count, sizeof(position_color));

		delete[] vertex_array;
	}

	void render_helper::draw_line(render_device* device, vector3<double>& vert1, vector3<double>& vert2, ulong vertices_color, bool is_ztest)
	{
		position_color vertex_array[2];
		vector3<float> vec = cartesian_coords::from_geocentric_f(vert1);
		vertex_array[0].x = vec.x;
		vertex_array[0].y = vec.y;
		vertex_array[0].z = vec.z;
		vertex_array[0].color = vertices_color;

		vec = cartesian_coords::from_geocentric_f(vert2);
		vertex_array[1].x = vec.x;
		vertex_array[1].y = vec.y;
		vertex_array[1].z = vec.z;
		vertex_array[1].color = vertices_color;

		render_state v_render_state(device);
		v_render_state.set_state(D3DRS_ALPHATESTENABLE, FALSE);
		v_render_state.set_state(D3DRS_ALPHABLENDENABLE, TRUE);
		v_render_state.set_state(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		v_render_state.set_state(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		v_render_state.set_state(D3DRS_ZENABLE, is_ztest);

		device->set_world_matrix(matrix4<float>::s_identity);
		device->set_vertex_declaration(position_color::fvf);
		device->draw_line_list((void*)vertex_array, 2, sizeof(position_color));

		//delete[] vertex_array;
	}

	void render_helper::draw_view_shed_frame(render_device* device, vector3<double>& start_point, vector3<double>& end_point, double vertical_angle, double horizon_angle, ulong vertices_color, bool is_ztest)
	{
		vector_3d dir_up = vector_3d::normalize(start_point);
		vector_3d dir_target = vector_3d::normalize(end_point - start_point);
		vector_3d dir_b = vector_3d::cross(dir_up, dir_target);
		dir_up = vector_3d::cross(dir_target, dir_b);
		dir_up.normalize();

		matrix_4d mat_view = matrix4<double>::look_at(start_point, end_point, dir_up);
		mat_view = mat_view.inverse();

		double v_range = (end_point - start_point).length();
		double v_x_value = v_range * math_d::tan_(horizon_angle * 0.5);
		double v_y_value = v_range * math_d::tan_(vertical_angle * 0.5);
		double v_z_value = v_range;

		position_color vertex_array[5];
		vertex_array[0].x = 0.0; vertex_array[0].y = 0.0; vertex_array[0].z = 0.0;
		vertex_array[0].color = vertices_color;

		vertex_array[1].x = -v_x_value; vertex_array[1].y = v_y_value; vertex_array[1].z = v_z_value;
		vertex_array[1].color = vertices_color;

		vertex_array[2].x = v_x_value; vertex_array[2].y = v_y_value; vertex_array[2].z = v_z_value;
		vertex_array[2].color = vertices_color;

		vertex_array[3].x = v_x_value; vertex_array[3].y = -v_y_value; vertex_array[3].z = v_z_value;
		vertex_array[3].color = vertices_color;

		vertex_array[4].x = -v_x_value; vertex_array[4].y = -v_y_value; vertex_array[4].z = v_z_value;
		vertex_array[4].color = vertices_color;

		ushort line_index[16];
		line_index[0] = 0; line_index[1] = 1;
		line_index[2] = 0; line_index[3] = 2;
		line_index[4] = 0; line_index[5] = 3;
		line_index[6] = 0; line_index[7] = 4;
		line_index[8] = 1; line_index[9] = 2;
		line_index[10] = 2; line_index[11] = 3;
		line_index[12] = 3; line_index[13] = 4;
		line_index[14] = 4; line_index[15] = 1;

		matrix_4f v_mat(mat_view.m);
		v_mat.m41 = mat_view.m41 - cartesian_coords::s_reference_position_geo.x;
		v_mat.m42 = mat_view.m42 - cartesian_coords::s_reference_position_geo.y;
		v_mat.m43 = mat_view.m43 - cartesian_coords::s_reference_position_geo.z;

		render_state v_render_state(device);
		v_render_state.set_state(D3DRS_ALPHATESTENABLE, FALSE);
		v_render_state.set_state(D3DRS_ALPHABLENDENABLE, TRUE);
		v_render_state.set_state(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		v_render_state.set_state(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		v_render_state.set_state(D3DRS_ZENABLE, is_ztest);

		device->push_transform();

		device->multiply_transform(v_mat);

		device->pop_transform();

		device->set_vertex_declaration(position_color::fvf);

		LPDIRECT3DDEVICE9 v_device = (LPDIRECT3DDEVICE9)device->get_device();

		v_device->DrawIndexedPrimitiveUP(D3DPT_LINELIST, 0, 5, 8, line_index, D3DFMT_INDEX16, vertex_array, position_color::stride);
	}

	void render_helper::draw_spherical_points_line(render_device* device, vector3<double>* vertices_buffer, ulong vertices_count, ulong point_color, ulong v_point_size, ulong line_color, bool is_ztest)
	{
		vector_3d vec_cen(0.0, 0.0, 0.0);
		for (int ni = 0; ni < (int)vertices_count; ni++)
		{
			vec_cen += vertices_buffer[ni];
		}
		vec_cen /= double(vertices_count);

		spherical_transform v_trans;
		v_trans.create_from_sphr(vec_cen.x, vec_cen.y, vec_cen.z, 0.0, 0.0, 0.0);

		vector_3d vec_tmp(0.0, 0.0, 0.0);
		position_color* vertex_array = new position_color[vertices_count + 1];
		for (int ni = 0; ni < (int)vertices_count; ni++)
		{
			vec_tmp = geocentric_coords::from_spherical_d(vertices_buffer[ni]);
			vec_tmp = vec_tmp * v_trans.m_mat_tnb_inverse;
			vertex_array[ni].x = vec_tmp.x;
			vertex_array[ni].y = vec_tmp.y;
			vertex_array[ni].z = vec_tmp.z;
			vertex_array[ni].color = point_color;
		}
		vertex_array[vertices_count] = vertex_array[0];

		matrix_4f v_mat(v_trans.m_mat_tnb.m);
		v_mat.m41 = v_trans.m_mat_tnb.m41 - cartesian_coords::s_reference_position_geo.x;
		v_mat.m42 = v_trans.m_mat_tnb.m42 - cartesian_coords::s_reference_position_geo.y;
		v_mat.m43 = v_trans.m_mat_tnb.m43 - cartesian_coords::s_reference_position_geo.z;

		render_state v_render_state(device);
		v_render_state.set_state(D3DRS_ALPHATESTENABLE, FALSE);
		v_render_state.set_state(D3DRS_ALPHABLENDENABLE, TRUE);
		v_render_state.set_state(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		v_render_state.set_state(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		float pt_size = (float)v_point_size;
		v_render_state.set_state(D3DRS_POINTSIZE, *((DWORD*)&pt_size));
		v_render_state.set_state(D3DRS_POINTSPRITEENABLE, TRUE);
		v_render_state.set_state(D3DRS_ZENABLE, is_ztest);

		//device->set_world_matrix( v_mat );
		device->push_transform();

		device->multiply_transform(v_mat);

		device->set_vertex_declaration(position_color::fvf);
		device->draw_point_list((void*)vertex_array, vertices_count, sizeof(position_color));

		for (int ni = 0; ni < (int)vertices_count; ni++)
		{
			vertex_array[ni].color = line_color;
		}
		device->draw_line_strip((void*)vertex_array, vertices_count + 1, sizeof(position_color));

		device->pop_transform();

		delete[] vertex_array;
	}

	void render_helper::draw_indexed_primitive_up(render_device* device, int v_type, ulong min_index, void* vertices_buffer, ushort* index_buffer, ulong verteices_count, ulong v_num, ulong vertices_size)
	{
		LPDIRECT3DDEVICE9 v_device = (LPDIRECT3DDEVICE9)device->get_device();
		v_device->DrawIndexedPrimitiveUP((D3DPRIMITIVETYPE)v_type, 0, verteices_count, v_num, index_buffer, D3DFMT_INDEX16, vertices_buffer, vertices_size);
	}

	void render_helper::draw_line_strip(render_device* device, vector3<double>* vertices_buffer, ulong vertices_count, ulong vertices_color, bool is_ztest)
	{
		position_color* vertex_array = new position_color[vertices_count];
		for (int ni = 0; ni < (int)vertices_count; ni++)
		{
			vector3<float> vec = cartesian_coords::from_geocentric_f(vertices_buffer[ni]);
			vertex_array[ni].x = vec.x;
			vertex_array[ni].y = vec.y;
			vertex_array[ni].z = vec.z;
			vertex_array[ni].color = vertices_color;
		}

		render_state v_render_state(device);
		v_render_state.set_state(D3DRS_ALPHATESTENABLE, FALSE);
		v_render_state.set_state(D3DRS_ALPHABLENDENABLE, TRUE);
		v_render_state.set_state(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		v_render_state.set_state(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		v_render_state.set_state(D3DRS_ZENABLE, is_ztest);
		//if(!is_ztest)
		//{
		//	v_render_state.set_state( D3DRS_ZFUNC, D3DCMP_ALWAYS );
		//}

		device->set_world_matrix(matrix4<float>::s_identity);
		device->set_vertex_declaration(position_color::fvf);
		device->draw_line_strip((void*)vertex_array, vertices_count, sizeof(position_color));

		delete[] vertex_array;

	}
	//////////////////////////////////////////////////////////////////////////
	/*
	   5-----4
	  /|    /|
	 / |   / |
	1-----2  |
	|  6--|--7
	| /   | /
	|/    |/
	0-----3
	*/
	//////////////////////////////////////////////////////////////////////////
	void render_helper::draw_box(render_device* device, vector3<double>* vertices_buffer, ulong vertices_color)
	{
		position_color vertex_array[24];

		int index[24] = { 0,1, 1,2, 2,3, 3,0,
							4,5, 5,6, 6,7, 7,4,
							0,6, 3,7, 1,5, 2,4
		};

		for (ulong ni = 0; ni < 24; ni++)
		{
			//vector3<float> vec = cartesian_coords::from_geocentric_f( vertices_buffer[index[ni]]);
			vector3<double> vec = vertices_buffer[index[ni]];
			vertex_array[ni].x = vec.x;
			vertex_array[ni].y = vec.y;
			vertex_array[ni].z = vec.z;
			vertex_array[ni].color = vertices_color;
		}

		render_state v_render_state(device);
		v_render_state.set_state(D3DRS_ALPHATESTENABLE, FALSE);
		v_render_state.set_state(D3DRS_ALPHABLENDENABLE, TRUE);
		v_render_state.set_state(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		v_render_state.set_state(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		v_render_state.set_state(D3DRS_ZENABLE, FALSE);

		//device->set_world_matrix( matrix4<float>::s_identity );
		device->set_vertex_declaration(position_color::fvf);
		device->draw_line_list((void*)vertex_array, 24, sizeof(position_color));

	}

	void render_helper::draw_box(render_device* device, double v_width, double v_length, double v_height, ulong fill_color, bool is_ztest, bool is_lighting)
	{
		position_color v_vertex_array[8];

		double half_x = v_width * 0.5;
		double half_y = v_height * 0.5;
		double half_z = v_length * 0.5;

		v_vertex_array[0].x = -half_x;
		v_vertex_array[0].y = -half_y;
		v_vertex_array[0].z = -half_z;
		v_vertex_array[0].color = fill_color;

		v_vertex_array[1].x = -half_x;
		v_vertex_array[1].y = half_y;
		v_vertex_array[1].z = -half_z;
		v_vertex_array[1].color = fill_color;

		v_vertex_array[2].x = half_x;
		v_vertex_array[2].y = half_y;
		v_vertex_array[2].z = -half_z;
		v_vertex_array[2].color = fill_color;

		v_vertex_array[3].x = half_x;
		v_vertex_array[3].y = -half_y;
		v_vertex_array[3].z = -half_z;
		v_vertex_array[3].color = fill_color;

		v_vertex_array[4].x = half_x;
		v_vertex_array[4].y = half_y;
		v_vertex_array[4].z = half_z;
		v_vertex_array[4].color = fill_color;

		v_vertex_array[5].x = -half_x;
		v_vertex_array[5].y = half_y;
		v_vertex_array[5].z = half_z;
		v_vertex_array[5].color = fill_color;

		v_vertex_array[6].x = -half_x;
		v_vertex_array[6].y = -half_y;
		v_vertex_array[6].z = half_z;
		v_vertex_array[6].color = fill_color;

		v_vertex_array[7].x = half_x;
		v_vertex_array[7].y = -half_y;
		v_vertex_array[7].z = half_z;
		v_vertex_array[7].color = fill_color;

		ushort v_index_array[36] =
		{
			0,1,3,1,2,3,
			3,2,7,2,4,7,
			7,4,6,4,5,6,
			6,5,0,5,1,0,
			1,5,2,5,4,2,
			0,3,6,3,7,6
		};

		render_state v_render_state(device);
		v_render_state.set_state(D3DRS_ALPHATESTENABLE, FALSE);
		v_render_state.set_state(D3DRS_ALPHABLENDENABLE, TRUE);
		v_render_state.set_state(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		v_render_state.set_state(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		v_render_state.set_state(D3DRS_ZENABLE, is_ztest);
		v_render_state.set_state(D3DRS_LIGHTING, is_lighting);

		device->set_vertex_declaration(position_color::fvf);

		device->draw_indexed_triangle_list(D3DFMT_INDEX16, v_vertex_array, 8, v_index_array, 36, position_color::stride);
	}


	//////////////////////////////////////////////////////////////////////////
	/*
	   5-----4
	  /|    /|
	 / |   / |
	1-----2  |
	|  6--|--7
	| /   | /
	|/    |/
	0-----3
	*/
	//////////////////////////////////////////////////////////////////////////
	void render_helper::draw_frustum(render_device* device, observer_base* v_observer, ulong vertices_color)
	{
		vector_3d vectors[8];

		double v_fov = v_observer->get_fov();
		double v_aspect = v_observer->get_aspect();
		double v_near_plane_range = v_observer->get_near_plane();
		double v_far_plane_range = v_observer->get_far_plane();

		vector3<double> v_positive_x = vector3<double>(v_aspect * v_near_plane_range * (double)math<double>::tan_(v_fov * (double)0.5),
			(double)0.0,
			v_near_plane_range);

		vector3<double> v_positive_y = vector3<double>((double)0.0,
			v_near_plane_range * (double)math<double>::tan_(v_fov * (double)0.5),
			v_near_plane_range);


		vector3<double> v_negative_x = v_positive_x * (double)-1.0;
		vector3<double> v_negative_y = v_positive_y * (double)-1.0;
		v_negative_x.z = v_negative_y.z = v_near_plane_range;

		matrix4<double> mat = v_observer->get_view_matrix_geo();
		matrix4<double> inv_view_mat;
		inv_view_mat = mat.inverse();

		//double v_width = v_observer->get_ortho_width();
		//double v_height = v_observer->get_ortho_height();

		//double m_near_plane_range = v_observer->get_near_plane();
		//double m_far_plane_range  = v_observer->get_far_plane();

		//matrix4<double> mat = v_observer->get_view_matrix_geo();

		//vector3<double> v_near_left_up  = vector3<double>(  -v_width * (double)0.5,  v_height * (double)0.5,  m_near_plane_range  );
		//vector3<double> v_near_right_up  = vector3<double>(  v_width * (double)0.5,  v_height * (double)0.5,  m_near_plane_range  );
		//vector3<double> v_near_left_down  = vector3<double>(  -v_width * (double)0.5,  -v_height * (double)0.5,  m_near_plane_range  );
		//vector3<double> v_near_right_down  = vector3<double>(  v_width * (double)0.5,  -v_height * (double)0.5,  m_near_plane_range  );

		//vector3<double> v_far_left_up  = vector3<double>(  -v_width * (double)0.5,  v_height * (double)0.5,  m_far_plane_range  );
		//vector3<double> v_far_right_up  = vector3<double>(  v_width * (double)0.5,  v_height * (double)0.5,  m_far_plane_range  );
		//vector3<double> v_far_left_down  = vector3<double>(  -v_width * (double)0.5,  -v_height * (double)0.5,  m_far_plane_range  );
		//vector3<double> v_far_right_down  = vector3<double>(  v_width * (double)0.5,  -v_height * (double)0.5,  m_far_plane_range  );

		//matrix4<double> inv_view_mat;
		//inv_view_mat = mat.inverse();

		//vectors[1] = v_near_left_up * inv_view_mat;
		//vectors[2]  = v_near_right_up * inv_view_mat;
		//vectors[0]  = v_near_left_down * inv_view_mat;
		//vectors[3]  = v_near_right_down * inv_view_mat;

		//vectors[5]  = v_far_left_up * inv_view_mat;
		//vectors[4]  = v_far_right_up * inv_view_mat;
		//vectors[6]  = v_far_left_down * inv_view_mat;
		//vectors[7]  = v_far_right_down * inv_view_mat;

		render_helper::draw_box(device, vectors, vertices_color);
	}

	void render_helper::draw_filled_frustum(render_device* device, observer_base* v_observer, ulong vertices_color)
	{

		vector_3d vectors[8];

		double v_width = v_observer->get_ortho_width();
		double v_height = v_observer->get_ortho_height();

		double m_near_plane_range = v_observer->get_near_plane() + 0.001;
		//double m_far_plane_range  = v_observer->get_far_plane() - 1;

		double m_far_plane_range = 9;

		matrix4<double> mat = v_observer->get_view_matrix_geo();

		vector3<double> v_near_left_up = vector3<double>(-v_width * (double)0.5, v_height * (double)0.5, m_near_plane_range);
		vector3<double> v_near_right_up = vector3<double>(v_width * (double)0.5, v_height * (double)0.5, m_near_plane_range);
		vector3<double> v_near_left_down = vector3<double>(-v_width * (double)0.5, -v_height * (double)0.5, m_near_plane_range);
		vector3<double> v_near_right_down = vector3<double>(v_width * (double)0.5, -v_height * (double)0.5, m_near_plane_range);

		vector3<double> v_far_left_up = vector3<double>(-v_width * (double)0.5, v_height * (double)0.5, m_far_plane_range);
		vector3<double> v_far_right_up = vector3<double>(v_width * (double)0.5, v_height * (double)0.5, m_far_plane_range);
		vector3<double> v_far_left_down = vector3<double>(-v_width * (double)0.5, -v_height * (double)0.5, m_far_plane_range);
		vector3<double> v_far_right_down = vector3<double>(v_width * (double)0.5, -v_height * (double)0.5, m_far_plane_range);

		matrix4<double> inv_view_mat;
		inv_view_mat = mat.inverse();

		vectors[1] = v_near_left_up * inv_view_mat;
		vectors[2] = v_near_right_up * inv_view_mat;
		vectors[0] = v_near_left_down * inv_view_mat;
		vectors[3] = v_near_right_down * inv_view_mat;

		vectors[5] = v_far_left_up * inv_view_mat;
		vectors[4] = v_far_right_up * inv_view_mat;
		vectors[6] = v_far_left_down * inv_view_mat;
		vectors[7] = v_far_right_down * inv_view_mat;


		position_color vertex_array[8];
		ushort index_array[36] =
		{
			0,1,3,1,2,3,
			3,2,7,2,4,7,
			7,4,6,4,5,6,
			6,5,0,5,1,0,
			1,5,2,5,4,2,
			0,3,6,3,7,6
		};

		for (ulong ni = 0; ni < 8; ni++)
		{
			vector3<float> vec = cartesian_coords::from_geocentric_f(vectors[ni]);
			vertex_array[ni].x = vec.x;
			vertex_array[ni].y = vec.y;
			vertex_array[ni].z = vec.z;
			vertex_array[ni].color = vertices_color;
		}

		device->set_vertex_declaration(position_color::fvf);
		device->draw_indexed_triangle_list(D3DFMT_INDEX16, (void*)vertex_array, 8, index_array, 36, sizeof(position_color));

		//render_helper::draw_filled_box( device, vectors, vertices_color );
	}



	void render_helper::draw_box_ex(render_device* device, const vector3<double>* vertices_buffer, ulong vertices_color)
	{
		position_color vertex_stream[48];
		vector3<double> vertex_array[32];

		int index[48] = { 0, 1,  0, 2,  0, 3,
							 4, 5,  4, 6,  4, 7,
							 8, 9,  8,10,  8,11,
							12,13, 12,14, 12,15,
							16,17, 16,18, 16,19,
							20,21, 20,22, 20,23,
							24,25, 24,26, 24,27,
							28,29, 28,30, 28,31 };

		for (ulong ni = 0; ni < 8; ni++)
		{
			vertex_array[ni * 4].x = vertices_buffer[ni].x;
			vertex_array[ni * 4].y = vertices_buffer[ni].y;
			vertex_array[ni * 4].z = vertices_buffer[ni].z;
		}

		vector3<double> line_0_3 = vector3<double>::normalize(vertex_array[3 * 4] - vertex_array[0]);
		double	length_0_3 = (vertex_array[3 * 4] - vertex_array[0]).length();

		vector3<double> line_0_1 = vector3<double>::normalize(vertex_array[1 * 4] - vertex_array[0]);
		double	length_0_1 = (vertex_array[1 * 4] - vertex_array[0]).length();

		vector3<double> line_0_6 = vector3<double>::normalize(vertex_array[6 * 4] - vertex_array[0]);
		double	length_0_6 = (vertex_array[6 * 4] - vertex_array[0]).length();

		vector3<double> line_1_2 = vector3<double>::normalize(vertex_array[2 * 4] - vertex_array[1 * 4]);
		double	length_1_2 = (vertex_array[2 * 4] - vertex_array[1 * 4]).length();

		vector3<double> line_1_5 = vector3<double>::normalize(vertex_array[5 * 4] - vertex_array[1 * 4]);
		double	length_1_5 = (vertex_array[5 * 4] - vertex_array[1 * 4]).length();

		vector3<double> line_2_3 = vector3<double>::normalize(vertex_array[3 * 4] - vertex_array[2 * 4]);
		double	length_2_3 = (vertex_array[3 * 4] - vertex_array[2 * 4]).length();

		vector3<double> line_2_4 = vector3<double>::normalize(vertex_array[4 * 4] - vertex_array[2 * 4]);
		double	length_2_4 = (vertex_array[4 * 4] - vertex_array[2 * 4]).length();

		vector3<double> line_3_7 = vector3<double>::normalize(vertex_array[7 * 4] - vertex_array[3 * 4]);
		double	length_3_7 = (vertex_array[7 * 4] - vertex_array[3 * 4]).length();

		vector3<double> line_4_5 = vector3<double>::normalize(vertex_array[5 * 4] - vertex_array[4 * 4]);
		double	length_4_5 = (vertex_array[5 * 4] - vertex_array[4 * 4]).length();

		vector3<double> line_4_7 = vector3<double>::normalize(vertex_array[7 * 4] - vertex_array[4 * 4]);
		double	length_4_7 = (vertex_array[7 * 4] - vertex_array[4 * 4]).length();

		vector3<double> line_5_6 = vector3<double>::normalize(vertex_array[6 * 4] - vertex_array[5 * 4]);
		double	length_5_6 = (vertex_array[6 * 4] - vertex_array[5 * 4]).length();

		vector3<double> line_6_7 = vector3<double>::normalize(vertex_array[7 * 4] - vertex_array[6 * 4]);
		double	length_6_7 = (vertex_array[7 * 4] - vertex_array[6 * 4]).length();


		double v_factor = 0.3;

		vertex_array[1] = vertex_array[0] + line_0_3 * v_factor * length_0_3;
		vertex_array[2] = vertex_array[0] + line_0_1 * v_factor * length_0_1;
		vertex_array[3] = vertex_array[0] + line_0_6 * v_factor * length_0_6;

		vertex_array[5] = vertex_array[1 * 4] + line_1_2 * v_factor * length_1_2;
		vertex_array[6] = vertex_array[1 * 4] + -line_0_1 * v_factor * length_0_1;
		vertex_array[7] = vertex_array[1 * 4] + line_1_5 * v_factor * length_1_5;

		vertex_array[9] = vertex_array[2 * 4] + -line_1_2 * v_factor * length_1_2;
		vertex_array[10] = vertex_array[2 * 4] + line_2_3 * v_factor * length_2_3;
		vertex_array[11] = vertex_array[2 * 4] + line_2_4 * v_factor * length_2_4;

		vertex_array[13] = vertex_array[3 * 4] + -line_0_3 * v_factor * length_0_3;
		vertex_array[14] = vertex_array[3 * 4] + -line_2_3 * v_factor * length_2_3;
		vertex_array[15] = vertex_array[3 * 4] + line_3_7 * v_factor * length_3_7;

		vertex_array[17] = vertex_array[4 * 4] + line_4_5 * v_factor * length_4_5;
		vertex_array[18] = vertex_array[4 * 4] + line_4_7 * v_factor * length_4_7;
		vertex_array[19] = vertex_array[4 * 4] + -line_2_4 * v_factor * length_2_4;

		vertex_array[21] = vertex_array[5 * 4] + -line_4_5 * v_factor * length_4_5;
		vertex_array[22] = vertex_array[5 * 4] + line_5_6 * v_factor * length_5_6;
		vertex_array[23] = vertex_array[5 * 4] + -line_1_5 * v_factor * length_1_5;

		vertex_array[25] = vertex_array[6 * 4] + line_6_7 * v_factor * length_6_7;
		vertex_array[26] = vertex_array[6 * 4] + -line_5_6 * v_factor * length_5_6;
		vertex_array[27] = vertex_array[6 * 4] + -line_0_6 * v_factor * length_0_6;

		vertex_array[29] = vertex_array[7 * 4] + -line_6_7 * v_factor * length_6_7;
		vertex_array[30] = vertex_array[7 * 4] + -line_4_7 * v_factor * length_4_7;
		vertex_array[31] = vertex_array[7 * 4] + -line_3_7 * v_factor * length_3_7;

		for (ulong ni = 0; ni < 48; ni++)
		{
			vertex_stream[ni].x = vertex_array[index[ni]].x - cartesian_coords::s_reference_position_geo.x;
			vertex_stream[ni].y = vertex_array[index[ni]].y - cartesian_coords::s_reference_position_geo.y;
			vertex_stream[ni].z = vertex_array[index[ni]].z - cartesian_coords::s_reference_position_geo.z;
			vertex_stream[ni].color = vertices_color;
		}

		render_state v_render_state(device);
		v_render_state.set_state(D3DRS_ALPHATESTENABLE, FALSE);
		v_render_state.set_state(D3DRS_ALPHABLENDENABLE, TRUE);
		v_render_state.set_state(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		v_render_state.set_state(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		v_render_state.set_state(D3DRS_ZENABLE, FALSE);

		device->set_world_matrix(matrix4<float>::s_identity);
		device->set_vertex_declaration(position_color::fvf);
		device->draw_line_list((void*)vertex_stream, 48, sizeof(position_color));
	}

	void render_helper::draw_lon_lat_rect(render_device* device, lon_lat_rect& v_rect, ulong vertices_color)
	{
		vector3<double> v_point[4];
		v_rect.get_corners(v_point);
		for (ulong ni = 0; ni < 4; ni++)
		{
			v_point[ni] = cartesian_coords::from_spherical_d(v_point[ni]);
		}

		position_color vertex_array[5];
		for (ulong ni = 0; ni < 4; ni++)
		{
			vertex_array[ni].x = v_point[ni].x;
			vertex_array[ni].y = v_point[ni].y;
			vertex_array[ni].z = v_point[ni].z;
			vertex_array[ni].color = vertices_color;
		}

		vertex_array[4].x = v_point[0].x;
		vertex_array[4].y = v_point[0].y;
		vertex_array[4].z = v_point[0].z;
		vertex_array[4].color = vertices_color;

		render_state v_render_state(device);
		v_render_state.set_state(D3DRS_ALPHATESTENABLE, FALSE);
		v_render_state.set_state(D3DRS_ALPHABLENDENABLE, TRUE);
		v_render_state.set_state(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		v_render_state.set_state(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		v_render_state.set_state(D3DRS_ZENABLE, FALSE);

		device->set_world_matrix(matrix4<float>::s_identity);
		device->set_vertex_declaration(position_color::fvf);
		device->draw_line_strip((void*)vertex_array, 5, sizeof(position_color));
	}

	//////////////////////////////////////////////////////////////////////////
	/*
	   5-----4
	  /|    /|
	 / |   / |
	1-----2  |
	|  6--|--7
	| /   | /
	|/    |/
	0-----3
	*/
	//////////////////////////////////////////////////////////////////////////
	void render_helper::draw_filled_box(render_device* device, vector3<double>* vertices_buffer, ulong vertices_color)
	{
		position_color vertex_array[8];

		// save current view matrix;
		//matrix4<float> mat_view = device->get_view_matrix();

		// save current projection matrix
		//matrix4<float> mat_proj = device->get_projection_matrix();

		// multiply mat_view and mat_projection
		//matrix4<float> mat_total = mat_view/* * mat_proj*/;

		//float v_far_plane = device->get_far_plane();

		ushort index_array[36] =
		{
			0,1,3,1,2,3,
			3,2,7,2,4,7,
			7,4,6,4,5,6,
			6,5,0,5,1,0,
			1,5,2,5,4,2,
			0,3,6,3,7,6
		};


		for (ulong ni = 0; ni < 8; ni++)
		{
			vector3<float> vec = cartesian_coords::from_geocentric_f(vertices_buffer[ni]);

			//vec = vec * mat_view;

			////if( vec.z >  v_far_plane ) vec.z = v_far_plane;
			//vec = matrix4<float>::transform_normal( vec,mat_proj );

			//if( vec.z > 1 ) vec.z = 1.0;

			//vector4<float> v_temp( vec.x, vec.y, vec.z, 1.0f );

			vertex_array[ni].x = vec.x;
			vertex_array[ni].y = vec.y;
			vertex_array[ni].z = vec.z;
			vertex_array[ni].color = vertices_color;
		}

		render_state v_render_state(device);

		// disable back buffer write
		v_render_state.set_state(D3DRS_ALPHABLENDENABLE, TRUE);
		v_render_state.set_state(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		v_render_state.set_state(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		v_render_state.set_state(D3DRS_ZENABLE, TRUE);
		v_render_state.set_state(D3DRS_CULLMODE, D3DCULL_CCW);

		device->set_world_matrix(matrix4<float>::s_identity);
		//device->set_view_matrix( matrix4<float>::s_identity );
		//device->set_projection_matrix( matrix4<float>::s_identity );

		device->set_vertex_declaration(position_color::fvf);
		device->draw_indexed_triangle_list(D3DFMT_INDEX16, (void*)vertex_array, 8, index_array, 36, sizeof(position_color));

		// restore view matrix and projection matrix
		//device->set_view_matrix( mat_view );
		//device->set_projection_matrix( mat_proj );
	}

	void render_helper::draw_polygon(render_device* device, vector3<double>* vertices_buffer, ulong vertices_count, ulong vertices_color, bool is_ztest)
	{
		position_color* vertex_array = new position_color[vertices_count];
		for (int ni = 0; ni < (int)vertices_count; ni++)
		{
			vector3<float> vec = cartesian_coords::from_geocentric_f(vertices_buffer[ni]);
			vertex_array[ni].x = vec.x;
			vertex_array[ni].y = vec.y;
			vertex_array[ni].z = vec.z;
			vertex_array[ni].color = vertices_color;
		}

		render_state v_render_state(device);
		v_render_state.set_state(D3DRS_ZENABLE, is_ztest);
		v_render_state.set_state(D3DRS_ALPHATESTENABLE, FALSE);
		v_render_state.set_state(D3DRS_ALPHABLENDENABLE, TRUE);
		v_render_state.set_state(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		v_render_state.set_state(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		v_render_state.set_state(D3DRS_CULLMODE, D3DCULL_NONE);

		device->set_world_matrix(matrix4<float>::s_identity);
		device->set_vertex_declaration(position_color::fvf);
		device->draw_triangle_fan((void*)vertex_array, vertices_count, sizeof(position_color));

		delete[] vertex_array;
	}

	void render_helper::draw_line_ring(render_device* device, vector3<double>* vertices_buffer, ulong vertices_count, ulong vertices_color, bool is_ztest)
	{
		position_color* vertex_array = new position_color[vertices_count + 1];
		for (int ni = 0; ni < (int)vertices_count; ni++)
		{
			vector3<float> vec = cartesian_coords::from_geocentric_f(vertices_buffer[ni]);
			vertex_array[ni].x = vec.x;
			vertex_array[ni].y = vec.y;
			vertex_array[ni].z = vec.z;
			vertex_array[ni].color = vertices_color;
		}
		vector3<float> vec = cartesian_coords::from_geocentric_f(vertices_buffer[0]);
		vertex_array[vertices_count].x = vec.x;
		vertex_array[vertices_count].y = vec.y;
		vertex_array[vertices_count].z = vec.z;
		vertex_array[vertices_count].color = vertices_color;

		render_state v_render_state(device);
		v_render_state.set_state(D3DRS_ZENABLE, is_ztest);
		v_render_state.set_state(D3DRS_ALPHATESTENABLE, FALSE);
		v_render_state.set_state(D3DRS_ALPHABLENDENABLE, TRUE);
		v_render_state.set_state(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		v_render_state.set_state(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		v_render_state.set_state(D3DRS_CULLMODE, D3DCULL_NONE);

		device->set_world_matrix(matrix4<float>::s_identity);
		device->set_vertex_declaration(position_color::fvf);
		device->draw_line_strip((void*)vertex_array, vertices_count + 1, sizeof(position_color));

		delete[] vertex_array;
	}

	void render_helper::draw_triangle_strip(render_device* device, vector3<double>* vertices_buffer, ulong vertices_count, ulong vertices_color, bool is_ztest)
	{
		position_color* vertex_array = new position_color[vertices_count];
		for (int ni = 0; ni < (int)vertices_count; ni++)
		{
			vector3<float> vec = cartesian_coords::from_geocentric_f(vertices_buffer[ni]);
			vertex_array[ni].x = vec.x;
			vertex_array[ni].y = vec.y;
			vertex_array[ni].z = vec.z;
			vertex_array[ni].color = vertices_color;
		}

		render_state v_render_state(device);
		v_render_state.set_state(D3DRS_ZENABLE, is_ztest);
		v_render_state.set_state(D3DRS_ALPHATESTENABLE, FALSE);
		v_render_state.set_state(D3DRS_ALPHABLENDENABLE, TRUE);
		v_render_state.set_state(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		v_render_state.set_state(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		v_render_state.set_state(D3DRS_CULLMODE, D3DCULL_NONE);

		device->set_world_matrix(matrix4<float>::s_identity);
		device->set_vertex_declaration(position_color::fvf);
		device->draw_triangle_strip((void*)vertex_array, vertices_count, sizeof(position_color));

		delete[] vertex_array;
	}


	void render_helper::draw_terrain_polygon(render_device* device, vector3<double>* vertices_buffer, ulong vertices_count, ulong fill_color)
	{
		// save current view matrix;
		matrix4<float> mat_view = device->get_view_matrix();

		// save current projection matrix
		matrix4<float> mat_proj = device->get_projection_matrix();

		// multiply mat_view and mat_projection
		matrix4<float> mat_total = mat_view * mat_proj;

		position_color* vertex_array = new position_color[vertices_count];
		for (int ni = 0; ni < (int)vertices_count; ni++)
		{
			vector3<float> vec = cartesian_coords::from_geocentric_f(vertices_buffer[ni]);

			vec = vec * mat_total;

			vertex_array[ni].x = vec.x;
			vertex_array[ni].y = vec.y;
			vertex_array[ni].z = vec.z;
			vertex_array[ni].color = fill_color;
		}

		device->set_world_matrix(matrix4<float>::s_identity);
		device->set_view_matrix(matrix4<float>::s_identity);
		device->set_projection_matrix(matrix4<float>::s_identity);
		device->set_vertex_declaration(position_color::fvf);
		device->draw_triangle_fan((void*)vertex_array, vertices_count - 1, sizeof(position_color));

		device->set_view_matrix(mat_view);
		device->set_projection_matrix(mat_proj);

		delete[] vertex_array;
	}

	void render_helper::draw_rect(render_device* device, rect& v_rect, ulong vertices_color)
	{
		transformed_color vertex_array[5];
		vertex_array[0].x = (float)v_rect.m_left;
		vertex_array[0].y = (float)v_rect.m_top;
		vertex_array[0].z = 0.0f;
		vertex_array[0].rhw = 1.0f;
		vertex_array[0].color = vertices_color;

		vertex_array[1].x = (float)v_rect.m_right;
		vertex_array[1].y = (float)v_rect.m_top;
		vertex_array[1].z = 0.0f;
		vertex_array[1].rhw = 1.0f;
		vertex_array[1].color = vertices_color;

		vertex_array[2].x = (float)v_rect.m_right;
		vertex_array[2].y = (float)v_rect.m_bottom;
		vertex_array[2].z = 0.0f;
		vertex_array[2].rhw = 1.0f;
		vertex_array[2].color = vertices_color;

		vertex_array[3].x = (float)v_rect.m_left;
		vertex_array[3].y = (float)v_rect.m_bottom;
		vertex_array[3].z = 0.0f;
		vertex_array[3].rhw = 1.0f;
		vertex_array[3].color = vertices_color;

		vertex_array[4].x = (float)v_rect.m_left;
		vertex_array[4].y = (float)v_rect.m_top;
		vertex_array[4].z = 0.0f;
		vertex_array[4].rhw = 1.0f;
		vertex_array[4].color = vertices_color;

		render_state v_render_state(device);
		v_render_state.set_state(D3DRS_ALPHATESTENABLE, FALSE);
		v_render_state.set_state(D3DRS_ALPHABLENDENABLE, TRUE);
		v_render_state.set_state(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		v_render_state.set_state(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		v_render_state.set_state(D3DRS_ZENABLE, FALSE);

		device->set_world_matrix(matrix4<float>::s_identity);
		device->set_vertex_declaration(transformed_color::fvf);
		device->draw_line_strip((void*)vertex_array, 5, sizeof(transformed_color));
	}

	void render_helper::draw_transformed_points(render_device* device, vector3<double>* vertices_buffer, ulong vertices_count, ulong vertices_color)
	{
		transformed_color* vertex_array = new transformed_color[vertices_count];
		for (int ni = 0; ni < (int)vertices_count; ni++)
		{
			vertex_array[ni].x = (float)vertices_buffer[ni].x;
			vertex_array[ni].y = (float)vertices_buffer[ni].y;
			vertex_array[ni].z = 0.0f;
			vertex_array[ni].rhw = 1.0f;
			vertex_array[ni].color = vertices_color;
		}

		render_state v_render_state(device);
		v_render_state.set_state(D3DRS_ALPHATESTENABLE, FALSE);
		v_render_state.set_state(D3DRS_ALPHABLENDENABLE, TRUE);
		v_render_state.set_state(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		v_render_state.set_state(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		v_render_state.set_state(D3DRS_ZENABLE, FALSE);

		device->set_world_matrix(matrix4<float>::s_identity);
		device->set_vertex_declaration(transformed_color::fvf);
		device->draw_point_list((void*)vertex_array, vertices_count, sizeof(transformed_color));
	}

	void render_helper::draw_transformed_line(render_device* device, vector3<double>* vertices_buffer, ulong vertices_count, ulong vertices_color)
	{
		transformed_color* vertex_array = new transformed_color[vertices_count];
		for (int ni = 0; ni < (int)vertices_count; ni++)
		{
			vertex_array[ni].x = (float)vertices_buffer[ni].x;
			vertex_array[ni].y = (float)vertices_buffer[ni].y;
			vertex_array[ni].z = 0.0f;
			vertex_array[ni].rhw = 1.0f;
			vertex_array[ni].color = vertices_color;
		}

		render_state v_render_state(device);
		v_render_state.set_state(D3DRS_ALPHATESTENABLE, FALSE);
		v_render_state.set_state(D3DRS_ALPHABLENDENABLE, TRUE);
		v_render_state.set_state(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		v_render_state.set_state(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		v_render_state.set_state(D3DRS_ZENABLE, FALSE);

		device->set_world_matrix(matrix4<float>::s_identity);
		device->set_vertex_declaration(transformed_color::fvf);
		device->draw_line_strip((void*)vertex_array, vertices_count, sizeof(transformed_color));
	}


	void render_helper::draw_text(render_device* device, const LPCTSTR text_string, vector3<double>& position_in_geo, ulong text_color)
	{
		vector3<float> position_cart = cartesian_coords::from_geocentric_f(position_in_geo);
		vector3<float> position_screen = device->projection(position_cart);

		view_port vp = device->get_view_port();

		if (position_screen.x < 0.0 ||
			position_screen.x >(vp.m_x + vp.m_width) ||
			position_screen.y < 0.0 ||
			position_screen.y >(vp.m_y + vp.m_height) ||
			position_screen.z < 0.0 ||
			position_screen.z > 1.0)
		{
			return;
		}

		render_state v_render_state(device);
		v_render_state.set_state(D3DRS_ZENABLE, TRUE);
		v_render_state.set_state(D3DRS_ALPHATESTENABLE, FALSE);
		v_render_state.set_state(D3DRS_ALPHABLENDENABLE, TRUE);
		v_render_state.set_state(D3DRS_CULLMODE, D3DCULL_NONE);

		device->set_world_matrix(matrix4<float>::s_identity);

		device->draw_transformed_text(text_string, point(position_screen.x, position_screen.y), text_color);
	}

	void render_helper::draw_text_ext(render_device* device, const LPCTSTR text_string, vector3<double>& position_in_geo, ulong text_color, double scale)
	{
		vector3<float> position_cart = cartesian_coords::from_geocentric_f(position_in_geo);
		vector3<float> position_screen = device->projection(position_cart);

		view_port vp = device->get_view_port();

		if (position_screen.x < 0.0 ||
			position_screen.x >(vp.m_x + vp.m_width) ||
			position_screen.y < 0.0 ||
			position_screen.y >(vp.m_y + vp.m_height) ||
			position_screen.z < 0.0 ||
			position_screen.z > 1.0)
		{
			return;
		}

		render_state v_render_state(device);
		v_render_state.set_state(D3DRS_ZENABLE, TRUE);
		v_render_state.set_state(D3DRS_ALPHATESTENABLE, FALSE);
		v_render_state.set_state(D3DRS_ALPHABLENDENABLE, TRUE);
		v_render_state.set_state(D3DRS_CULLMODE, D3DCULL_NONE);

		device->set_world_matrix(matrix4<float>::s_identity);

		device->draw_transformed_text_ext(text_string, point(position_screen.x, position_screen.y), text_color, scale);
	}

	void render_helper::draw_icon(render_device* device, hardware_texture* in_texture, LPCTSTR text_string, vector3<double>& position_in_geo, ulong text_color)
	{
		if (in_texture == NULL || in_texture->get_handle() == 0)
			return;

		//
		// prepare rendering data
		//
		vector3<float> v_icon_position = device->projection(vector3<float>((double*)&(position_in_geo - cartesian_coords::s_reference_position_geo)));
		view_port vp = device->get_view_port();

		if (v_icon_position.x < vp.m_x ||
			v_icon_position.x >(vp.m_x + vp.m_width) ||
			v_icon_position.y < vp.m_y ||
			v_icon_position.y >(vp.m_y + vp.m_height) ||
			v_icon_position.z < vp.m_z_min ||
			v_icon_position.z > vp.m_z_max)
			return;

		short v_texture_width_in_memory = 32;
		short v_texture_height_in_memory = 32;


		//
		// text rect
		//
		rect v_text_rect, v_icon_rect;
		v_text_rect.set_rect(0, 0, wcslen(text_string) * 13, 14);
		v_text_rect.offset(v_icon_position.x, v_icon_position.y);
		v_text_rect.offset(v_texture_width_in_memory * 0.5 + 2, -v_text_rect.get_height());

		//
		// icon rect
		//
		v_icon_rect.set_rect(v_icon_position.x - v_texture_width_in_memory  * 0.5,
			v_icon_position.y - v_texture_height_in_memory * 0.5,
			v_icon_position.x + v_texture_width_in_memory  * 0.5,
			v_icon_position.y + v_texture_height_in_memory * 0.5);




		//
		// actual rendering begin
		//

		render_state rs(device);
		rs.set_state(D3DRS_ZENABLE, FALSE);
		rs.set_state(D3DRS_ALPHABLENDENABLE, TRUE);

		LPD3DXSPRITE v_sprite = (LPD3DXSPRITE)device->get_sprite();
		v_sprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
		v_sprite->SetTransform((D3DXMATRIX*)matrix4<float>::s_identity.m);

		D3DXVECTOR3 center = D3DXVECTOR3(v_texture_width_in_memory * 0.5f, v_texture_height_in_memory* 0.5f, 0);

		D3DXVECTOR3 icon_pos;
		icon_pos.x = v_icon_position.x;
		icon_pos.y = v_icon_position.y;
		icon_pos.z = 0;
		LPDIRECT3DTEXTURE9 txt = (LPDIRECT3DTEXTURE9)in_texture->get_handle();
		v_sprite->Draw(txt, NULL, &center, &icon_pos, 0xffffffff);


		LPD3DXFONT d3d_font = (LPD3DXFONT)device->get_font();

		float v_offset = 1;
		v_text_rect.offset(-v_offset, -v_offset);
		d3d_font->DrawText(v_sprite, text_string, -1, (LPRECT)&v_text_rect, DT_LEFT | DT_VCENTER | DT_NOCLIP, 0xff000000);
		v_text_rect.offset(v_offset * 2, 0);
		d3d_font->DrawText(v_sprite, text_string, -1, (LPRECT)&v_text_rect, DT_LEFT | DT_VCENTER | DT_NOCLIP, 0xff000000);
		v_text_rect.offset(0, v_offset * 2);
		d3d_font->DrawText(v_sprite, text_string, -1, (LPRECT)&v_text_rect, DT_LEFT | DT_VCENTER | DT_NOCLIP, 0xff000000);
		v_text_rect.offset(-v_offset * 2, 0);
		d3d_font->DrawText(v_sprite, text_string, -1, (LPRECT)&v_text_rect, DT_LEFT | DT_VCENTER | DT_NOCLIP, 0xff000000);
		v_text_rect.offset(v_offset, -v_offset);

		d3d_font->DrawText(v_sprite, text_string, -1, (LPRECT)&v_text_rect, DT_LEFT | DT_VCENTER | DT_NOCLIP, text_color);

		v_sprite->End();
	}

	void render_helper::draw_transformed_region(render_device* device, vector3<double>* vertices_buffer, ulong vertices_count, ulong vertices_color)
	{
		transformed_color* vertex_array = new transformed_color[vertices_count];
		for (int ni = 0; ni < (int)vertices_count; ni++)
		{
			vertex_array[ni].x = (float)vertices_buffer[ni].x;
			vertex_array[ni].y = (float)vertices_buffer[ni].y;
			vertex_array[ni].z = 0.0f;
			vertex_array[ni].rhw = 1.0f;
			vertex_array[ni].color = vertices_color;
		}

		render_state v_render_state(device);
		v_render_state.set_state(D3DRS_ALPHATESTENABLE, FALSE);
		v_render_state.set_state(D3DRS_ALPHABLENDENABLE, TRUE);
		v_render_state.set_state(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		v_render_state.set_state(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		v_render_state.set_state(D3DRS_ZENABLE, FALSE);

		device->set_world_matrix(matrix4<float>::s_identity);
		device->set_vertex_declaration(transformed_color::fvf);
		device->draw_triangle_strip((void*)vertex_array, vertices_count, sizeof(transformed_color));

	}

	void render_helper::draw_transformed_text(render_device* device, const LPCTSTR text_string, point& position, ulong text_color)
	{

	}

	hardware_texture* render_helper::create_fill_texture(render_device* device, ulong width, ulong height, ulong fill_color)
	{
		int v_size = width * height * 2;
		managed_buffer* v_managed_buffer = new managed_buffer(v_size);
		memset(v_managed_buffer->get_buffer(), 0xff, v_size);

		texture_info tex_info;
		tex_info.width = width;
		tex_info.height = height;
		tex_info.mipmap = 1;
		tex_info.format = (int)D3DFMT_A1R5G5B5;

		hardware_texture* v_hardware_texture = new d3d9_hardware_texture(v_managed_buffer, tex_info);
		v_hardware_texture->create(device);
		//AUTO_DELETE( v_data );

		return v_hardware_texture;
	}

	hardware_texture* render_helper::create_texture_from_file(render_device* device, const LPCTSTR texture_path)
	{
		d3d9_hardware_texture* v_hardware_texture = new d3d9_hardware_texture();
		v_hardware_texture->create_from_file(device, texture_path);

		return (hardware_texture*)v_hardware_texture;
	}

	hardware_texture* render_helper::create_texture_from_file_ex(render_device* device, const LPCTSTR texture_path)
	{
		data_loader v_loader;
		raw_buffer* v_buffer = v_loader.load(texture_path);

		if (v_buffer == NULL) return NULL;
		hardware_texture* v_hardware_texture = dynamic_cast<hardware_texture*>(singleton_filter_manager::instance().parse(v_buffer));
		v_hardware_texture->create(device);

		return (hardware_texture*)v_hardware_texture;
	}

	hardware_texture* render_helper::create_texture_from_resource(render_device* device, ULONG_PTR handle)
	{
		d3d9_hardware_texture* v_hardware_texture = new d3d9_hardware_texture();
		v_hardware_texture->create_from_resource(device, handle);

		return (hardware_texture*)v_hardware_texture;
	}


	void render_helper::line_approximation(vector3<double>* in_pos_array, ulong in_num_of_points,
		vector3<double>** out_pos_array, ulong* out_num_of_points,
		double angle_tolerance)
	{
		if (*out_pos_array != NULL)
		{
			return;
		}

		std::vector<vector3<double>> target_pos_array;

		for (int ni = 0; ni < in_num_of_points - 1; ni++)
		{
			vector3<double> pos_geo1 = in_pos_array[ni];
			vector3<double> pos_geo2 = in_pos_array[ni + 1];

			// 加入第一个点
			target_pos_array.push_back(pos_geo1);

			// 判断是否需要插值
			vector3<double> vec_tmp1 = pos_geo1;
			vector3<double> vec_tmp2 = pos_geo2;
			vec_tmp1.normalize();
			vec_tmp2.normalize();
			double included_angle = math<double>::acos_(vector3<double>::dot(vec_tmp1, vec_tmp2));

			if (included_angle > angle_tolerance * RADIAN)
			{
				// 分裂
				int split_count = math<double>::floor_(included_angle / (angle_tolerance * RADIAN));
				vector3<double> dir = pos_geo2 - pos_geo1;
				double delta_dist = dir.length() / (split_count + 1);
				dir.normalize();

				for (int i = 0; i < split_count; i++)
				{
					vector3<double> pos_split_geo = pos_geo1 + dir * delta_dist * (i + 1);
					pos_split_geo.normalize();
					vector3<double> pos_result_geo = pos_split_geo * spherical_coords::s_semi_major;
					target_pos_array.push_back(pos_result_geo);
				}
			}


		}

		// 加入最后一个点
		target_pos_array.push_back(in_pos_array[in_num_of_points - 1]);


		*out_num_of_points = target_pos_array.size();
		*out_pos_array = new vector3<double>[*out_num_of_points];
		for (int i = 0; i < int(*out_num_of_points); i++)
		{
			(*out_pos_array)[i] = target_pos_array[i];
		}

	}



}