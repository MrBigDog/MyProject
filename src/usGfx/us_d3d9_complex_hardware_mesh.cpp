///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_complex_hardware_mesh.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_complex_hardware_mesh class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
////#include "stdafx.h"
#include "us_d3d9_complex_hardware_mesh.h"
#include <usGfx/us_d3d9_hardware_mesh.h>
#include <usGfx/us_d3d9_effect_common_mesh.h>
#include <usDotx/us_usx_animation_set.h>
#include <usDotx/us_usx_frame.h>

#include <windows.h>

namespace uniscope_globe
{
	simple_animation_player::simple_animation_player(usx_animation_set* in_anim_set)
	{
		m_animation_set = in_anim_set;

		m_current_time = 0;

		m_last_time_tick = 0;
	}

	simple_animation_player::~simple_animation_player(void)
	{
		AUTO_DELETE(m_animation_set);
	}

	void simple_animation_player::attach(usx_frame* in_frame)
	{
		m_animation_set->attach_to_frame(in_frame);
	}

	void simple_animation_player::update(void)
	{
		//ulong cur_time_tick = timeGetTime();

		//if ( m_current_time < m_animation_set->m_time_length )
		//{
		//	m_current_time += ( cur_time_tick - m_last_time_tick ) * 2.0;
		//	m_animation_set->update( m_current_time );
		//}
		//else
		//{
		//	m_animation_set->update( m_animation_set->m_time_length );
		//	m_current_time = 0;
		//}

		//m_last_time_tick = cur_time_tick;

		ulong cur_time_tick = timeGetTime();
		m_current_time = m_current_time + cur_time_tick - m_last_time_tick;

		if (m_current_time > m_animation_set->m_time_length)
		{
			m_current_time = m_current_time % m_animation_set->m_time_length;
		}

		m_animation_set->update(m_current_time);

		m_last_time_tick = cur_time_tick;
	}


	//////////////////////////////////////////////////////////////////////////
	d3d9_complex_hardware_mesh::d3d9_complex_hardware_mesh(void)
	{
		m_rtti = US_RTTI_D3D9_COMPLEX_HARDWARE_MESH;

		m_root_frame = NULL;
	}

	//d3d9_complex_hardware_mesh::d3d9_complex_hardware_mesh( hardware_mesh_buffer_map& in_buffer_map, usx_frame* in_root_frame, usx_animation_set_array& in_animation_set_array )
	//{
	//	m_rtti = US_RTTI_D3D9_COMPLEX_HARDWARE_MESH;

	//	m_is_texture_ready = true;

	//	m_root_frame = in_root_frame;

	//	if( m_root_frame != NULL )
	//	{			
	//		m_root_frame->reset();
	//		m_root_frame->update_hierarchy( matrix4<double>::s_identity );
	//	}

	//	if( m_root_frame != NULL )
	//	{
	//		for( int i = 0; i < (int)in_animation_set_array.size(); i++ )
	//		{
	//			usx_animation_set* anim_set = in_animation_set_array[i];	

	//			simple_animation_player* v_player = new simple_animation_player( anim_set );
	//			v_player->attach( m_root_frame );

	//			m_animation_player_map.insert( make_pair(anim_set->m_name, v_player ) );
	//		}			
	//	}

	//	m_bound_box.make_invalid();
	//	hardware_mesh_buffer_map::iterator itr = in_buffer_map.begin();
	//	for( ; itr != in_buffer_map.end(); itr ++ )
	//	{
	//		hardware_mesh_buffer* v_buffer = itr->first;
	//		usx_frame* v_frame = itr->second;

	//		if ( v_buffer->m_num_of_vertices == 0 )
	//			continue;

	//		m_fvf = v_buffer->m_fvf;

	//		d3d9_hardware_mesh* v_mesh = d3d9_hardware_mesh::create_from_buffer( v_buffer );

	//		m_bound_box.combine( v_buffer->m_aabb );

	//		m_mesh_frame_map.insert( make_pair( v_mesh,  v_frame  ) );
	//	}		
	//}

	d3d9_complex_hardware_mesh::~d3d9_complex_hardware_mesh(void)
	{
		destroy();
	}

	void d3d9_complex_hardware_mesh::generate_topological_data(void)
	{
		mesh_frame_map::iterator itr = m_mesh_frame_map.begin();
		for (; itr != m_mesh_frame_map.end(); itr++)
		{
			itr->first->generate_topological_data();
		}
	}

	void d3d9_complex_hardware_mesh::update_bound_box(void)
	{
		m_bound_box.make_invalid();

		mesh_frame_map::iterator itr = m_mesh_frame_map.begin();
		for (; itr != m_mesh_frame_map.end(); itr++)
		{
			itr->first->update_bound_box();
			m_bound_box.combine(itr->first->get_bound_box());
		}
	}

	//void d3d9_complex_hardware_mesh::reset( render_device* device )
	//{
	//	mesh_frame_map::iterator itr = m_mesh_frame_map.begin();
	//	for( ; itr != m_mesh_frame_map.end(); itr ++ )
	//	{
	//		itr->first->reset(device);
	//	}
	//}

	hardware_mesh* d3d9_complex_hardware_mesh::clone(void)
	{
		d3d9_complex_hardware_mesh* v_cloned_mesh = new d3d9_complex_hardware_mesh();

		mesh_frame_map::iterator itr = m_mesh_frame_map.begin();
		for (; itr != m_mesh_frame_map.end(); itr++)
		{
			v_cloned_mesh->m_mesh_frame_map.insert(std::make_pair(dynamic_cast<d3d9_hardware_mesh*>(itr->first->clone()), itr->second->clone()));
		}

		v_cloned_mesh->m_root_frame = this->m_root_frame->clone();

		v_cloned_mesh->m_fvf = this->m_fvf;

		// animation_player

		return v_cloned_mesh;
	}

	// create synchronously
	long d3d9_complex_hardware_mesh::create(render_device* device)
	{
		mesh_frame_map::iterator itr = m_mesh_frame_map.begin();
		for (; itr != m_mesh_frame_map.end(); itr++)
		{
			d3d9_hardware_mesh* v_mesh = itr->first;

			v_mesh->create(device);
		}

		return 0;
	}

	bool d3d9_complex_hardware_mesh::destroy(void)
	{
		AUTO_DELETE(m_root_frame);

		mesh_frame_map::iterator itr = m_mesh_frame_map.begin();
		for (; itr != m_mesh_frame_map.end(); itr++)
		{
			d3d9_hardware_mesh* v_mesh = itr->first;
			AUTO_DELETE(v_mesh);
		}

		m_mesh_frame_map.clear();

		animation_player_map::iterator itr2 = m_animation_player_map.begin();
		for (; itr2 != m_animation_player_map.end(); itr2++)
		{
			simple_animation_player* anim_player = itr2->second;
			AUTO_DELETE(anim_player);
		}

		m_animation_player_map.clear();

		return true;
	}

	bool d3d9_complex_hardware_mesh::refresh(void)
	{
		mesh_frame_map::iterator itr = m_mesh_frame_map.begin();
		for (; itr != m_mesh_frame_map.end(); itr++)
		{
			d3d9_hardware_mesh* v_mesh = itr->first;
			if (!v_mesh->refresh())
				return false;
		}

		return true;
	}

	void d3d9_complex_hardware_mesh::update_animation(void)
	{
		if (m_root_frame == NULL) return;
		if (m_animation_player_map.size() == 0) return;

		m_root_frame->reset();

		animation_player_map::iterator itr = m_animation_player_map.begin();
		if (itr != m_animation_player_map.end())
		{
			simple_animation_player* player = itr->second;

			player->update();
		}

		m_root_frame->update_hierarchy(matrix4<double>::s_identity);
	}

	void d3d9_complex_hardware_mesh::draw(render_argument* args)
	{
		update_animation();

		d3d9_effect_common_mesh* v_render = (d3d9_effect_common_mesh*)(args->m_render);

		mesh_frame_map::iterator itr = m_mesh_frame_map.begin();
		for (; itr != m_mesh_frame_map.end(); itr++)
		{
			d3d9_hardware_mesh* v_mesh = itr->first;
			usx_frame* v_frame = itr->second;

			v_render->push_transform();

			matrix4<float> mat = matrix4<float>::s_identity;
			if (v_frame != NULL)
			{
				//std::vector<usx_frame*> usx_frame_array;
				//usx_frame* cyc_frame = v_frame;
				//while( cyc_frame )
				//{
				//	usx_frame_array.push_back( cyc_frame );
				//	cyc_frame = cyc_frame->m_frame_parent;
				//}

				//matrix4<double> v_trans_mat = matrix4<double>::s_identity;
				//matrix4<double> v_trans_mat1 = matrix4<double>::s_identity;
				//for( int ni = 0; ni < (int)usx_frame_array.size(); ni++ )
				//{
				//	cyc_frame = usx_frame_array[ni];
				//	
				//	if ( cyc_frame->m_animation == NULL )
				//	{
				//		v_trans_mat = v_trans_mat * cyc_frame->m_transform_mat;
				//		v_trans_mat1 = v_trans_mat1 * cyc_frame->m_transform_mat;
				//		continue;
				//	}

				//	matrix4<double> v_begin_mat = matrix4<double>::s_identity;
				//	matrix4<double> v_end_mat =  matrix4<double>::s_identity;

				//	D3DXQUATERNION quatOut( 0.0, 0.0, 0.0, 1.0 );
				//	D3DXMATRIX frmMat( matrix4<float>( v_trans_mat.m ).m );
				//	D3DXQuaternionRotationMatrix( &quatOut, &frmMat );
				//	quaternion<double> quat_tmp( quatOut.x, quatOut.y, quatOut.z, quatOut.w );

				//	//rotation
				//	{
				//		quaternion<double> begin_rotation = cyc_frame->m_animation->m_rotation_key_array[cyc_frame->m_animation->m_begin_key]->m_rotation;
				//		//begin_rotation.normalize();
				//		quaternion<double> end_rotation   = cyc_frame->m_animation->m_rotation_key_array[cyc_frame->m_animation->m_end_key]->m_rotation;
				//		//end_rotation.normalize();

				//		v_begin_mat = v_begin_mat * matrix4<double>::from_quaternion( begin_rotation );
				//		v_end_mat = v_end_mat * matrix4<double>::from_quaternion( end_rotation );

				//		if ( begin_rotation != end_rotation )
				//		{
				//			//begin_rotation = quat_tmp * begin_rotation;
				//			//end_rotation = quat_tmp * end_rotation;
				//			//begin_rotation = begin_rotation * quat_tmp;
				//			//end_rotation = end_rotation * quat_tmp;
				//		}

				//		quaternion<double> final_rotation = quaternion<double>::slerp( begin_rotation, end_rotation, cyc_frame->m_animation->m_scalar );
				//		//final_rotation.normalize();
				//		//quaternion<double> final_rotation = begin_rotation + ( end_rotation - begin_rotation ) * cyc_frame->m_animation->m_scalar;

				//		v_trans_mat = v_trans_mat * matrix4<double>::from_quaternion( final_rotation );
				//	}
				//	
				//	//scaling
				//	{
				//		vector3<double> begin_scale = cyc_frame->m_animation->m_scale_key_array[cyc_frame->m_animation->m_begin_key]->m_scale;
				//		vector3<double> end_scale   = cyc_frame->m_animation->m_scale_key_array[cyc_frame->m_animation->m_end_key]->m_scale;

				//		v_begin_mat = v_begin_mat * matrix4<double>::scaling( begin_scale.x, begin_scale.y, begin_scale.z );
				//		v_end_mat = v_end_mat * matrix4<double>::scaling( end_scale.x, end_scale.y, end_scale.z );


				//		vector3<double> scale_diff = end_scale - begin_scale;
				//		vector3<double> final_scale = begin_scale + scale_diff * cyc_frame->m_animation->m_scalar;

				//		v_trans_mat = v_trans_mat * matrix4<double>::scaling( final_scale.x, final_scale.y, final_scale.z );
				//	}



				//	//translate
				//	{
				//		vector3<double> begin_translate = cyc_frame->m_animation->m_translation_key_array[cyc_frame->m_animation->m_begin_key]->m_translate;
				//		vector3<double> end_translate   = cyc_frame->m_animation->m_translation_key_array[cyc_frame->m_animation->m_end_key]->m_translate;

				//		v_begin_mat = v_begin_mat * matrix4<double>::translation( begin_translate.x, begin_translate.y, begin_translate.z );
				//		v_end_mat = v_end_mat * matrix4<double>::translation( end_translate.x, end_translate.y, end_translate.z );

				//		vector3<double> translate_diff = end_translate - begin_translate;

				//		vector3<double> final_translate = begin_translate + translate_diff * cyc_frame->m_animation->m_scalar;

				//		v_trans_mat = v_trans_mat * matrix4<double>::translation( final_translate.x, final_translate.y, final_translate.z );
				//	}

				//	v_trans_mat1 = v_trans_mat1 * ( v_begin_mat + ( v_end_mat - v_begin_mat ) * cyc_frame->m_animation->m_scalar );

				//}
				//mat = matrix4<float>( v_trans_mat.m );

				mat = matrix4<float>(v_frame->m_combined_mat.m);

			}

			v_render->multiply_transform(mat);

			v_mesh->draw(args);

			v_render->pop_transform();
		}

	}

	bool d3d9_complex_hardware_mesh::intersect(const ray<double>& in_ray, intersect_result& out_result)
	{
		bool is_intersected = false;

		mesh_frame_map::iterator itr = m_mesh_frame_map.begin();
		for (; itr != m_mesh_frame_map.end(); itr++)
		{
			d3d9_hardware_mesh* v_mesh = itr->first;
			usx_frame* v_frame = itr->second;

			matrix4<double> mat = matrix4<double>::s_identity;
			matrix4<double> mat_inv = matrix4<double>::s_identity;
			if (v_frame != NULL)
			{
				mat = v_frame->m_combined_mat;
				mat_inv = matrix4<double>::inverse(v_frame->m_combined_mat);
			}

			ray<double> v_ray;
			v_ray.ray_origin = in_ray.ray_origin * mat_inv;
			v_ray.ray_direction = matrix4<double>::rotate_normal(in_ray.ray_direction, mat_inv);
			v_ray.ray_direction.normalize();

			intersect_result result;
			if (v_mesh->intersect(v_ray, result))
			{
				if (out_result.m_distance > result.m_distance)
				{
					out_result.m_distance = result.m_distance;
					out_result.m_position = result.m_position * mat;
					out_result.m_inner_mesh = v_mesh;
					out_result.m_face_index = result.m_face_index;
					is_intersected = true;
				}
			}
		}

		return is_intersected;
	}

	vector3<double> d3d9_complex_hardware_mesh::get_lowest_point(matrix4<double>& in_mat)
	{
		vector3<double> v_lowest_point;

		mesh_frame_map::iterator itr = m_mesh_frame_map.begin();
		for (; itr != m_mesh_frame_map.end(); itr++)
		{
			d3d9_hardware_mesh* v_mesh = itr->first;

			vector3<double> v_point = v_mesh->get_lowest_point(in_mat);
			if (v_lowest_point.y > v_point.y)
			{
				v_lowest_point = v_point;
			}
		}

		return v_lowest_point;
	}
}