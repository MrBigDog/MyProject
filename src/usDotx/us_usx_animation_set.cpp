///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_usx_animation_set.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : usx_animation_set class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_usx_animation_set.h"
#include <usDotx/us_usx_frame.h>

namespace uniscope_globe
{
	usx_animation::usx_animation(usx_animation_set* in_parent_set)
	{
		m_parent_set = in_parent_set;

		m_frame = NULL;
	}

	usx_animation::~usx_animation()
	{
		for (int i = 0; i < (int)m_rotation_key_array.size(); i++)
		{
			AUTO_DELETE(m_rotation_key_array[i]);
		}

		for (int i = 0; i < (int)m_scale_key_array.size(); i++)
		{
			AUTO_DELETE(m_scale_key_array[i]);
		}

		for (int i = 0; i < (int)m_translation_key_array.size(); i++)
		{
			AUTO_DELETE(m_translation_key_array[i]);
		}

		for (int i = 0; i < (int)m_matrix_key_array.size(); i++)
		{
			AUTO_DELETE(m_matrix_key_array[i]);
		}
	}

	void usx_animation::update_keys(ulong in_time)
	{
		if (m_frame == NULL) return;

		matrix4<double> updated_mat;
		updated_mat.identity();

		update_scale_keys(in_time, updated_mat);
		update_rotation_keys(in_time, updated_mat);
		update_translation_keys(in_time, updated_mat);

		update_matrix_keys(in_time, updated_mat);

		//matrix4<double> diff_mat = updated_mat - m_frame->m_original_mat;

		//m_frame->m_transform_mat += diff_mat;

		m_frame->m_transform_mat = updated_mat;
	}

	void usx_animation::update_scale_keys(ulong in_time, matrix4<double>& in_mat)
	{
		if (m_scale_key_array.size() <= 0) return;

		ulong begin_key = 0;
		for (int i = 0; i < (int)m_scale_key_array.size(); i++)
		{
			if (in_time >= m_scale_key_array[i]->m_time)
				begin_key = i;
		}
		ulong end_key = (begin_key >= (m_scale_key_array.size() - 1)) ? begin_key : begin_key + 1;

		ulong begin_time = m_scale_key_array[begin_key]->m_time;
		ulong end_time = m_scale_key_array[end_key]->m_time;

		vector3<double> begin_scale = m_scale_key_array[begin_key]->m_scale;
		vector3<double> end_scale = m_scale_key_array[end_key]->m_scale;

		double time_diff = end_time - begin_time;
		if (time_diff < 1.0)
			time_diff = 1.0;

		double scalar = (in_time - begin_time) / time_diff;

		vector3<double> scale_diff = end_scale - begin_scale;

		vector3<double> final_scale = begin_scale + scale_diff * scalar;

		in_mat = in_mat * matrix4<double>::scaling(final_scale.x, final_scale.y, final_scale.z);
	}

	void usx_animation::update_rotation_keys(ulong in_time, matrix4<double>& in_mat)
	{
		if (m_rotation_key_array.size() <= 0) return;

		ulong begin_key = 0;
		for (int i = 0; i < (int)m_rotation_key_array.size(); i++)
		{
			if (in_time >= m_rotation_key_array[i]->m_time)
				begin_key = i;
		}
		ulong end_key = (begin_key >= (m_rotation_key_array.size() - 1)) ? begin_key : begin_key + 1;

		ulong begin_time = m_rotation_key_array[begin_key]->m_time;
		ulong end_time = m_rotation_key_array[end_key]->m_time;

		quaternion<double> begin_rotation = m_rotation_key_array[begin_key]->m_rotation;
		quaternion<double> end_rotation = m_rotation_key_array[end_key]->m_rotation;

		double time_diff = end_time - begin_time;
		if (time_diff < 1.0)
			time_diff = 1.0;

		double scalar = (in_time - begin_time) / time_diff;

		m_begin_key = begin_key;
		m_end_key = end_key;
		m_scalar = scalar;

		//D3DXQUATERNION quatOut( 0.0, 0.0, 0.0, 1.0 );
		//D3DXQUATERNION quatBegin( begin_rotation.x, begin_rotation.y, begin_rotation.z, begin_rotation.w );
		//D3DXQUATERNION quatEnd( end_rotation.x, end_rotation.y, end_rotation.z, end_rotation.w );
		//D3DXQuaternionSlerp( &quatOut, &quatBegin, &quatEnd, float(scalar) );

		//D3DXMATRIX frmMat( matrix4<float>( m_frame->m_transform_mat.m ).m );
		//D3DXQuaternionRotationMatrix( &quatOut, &frmMat );

		quaternion<double> final_rotation = quaternion<double>::slerp(begin_rotation, end_rotation, scalar);

		in_mat = in_mat * matrix4<double>::from_quaternion(final_rotation);
	}

	void usx_animation::update_translation_keys(ulong in_time, matrix4<double>& in_mat)
	{
		if (m_translation_key_array.size() <= 0) return;

		ulong begin_key = 0;
		for (int i = 0; i < (int)m_translation_key_array.size(); i++)
		{
			if (in_time >= m_translation_key_array[i]->m_time)
				begin_key = i;
		}
		ulong end_key = (begin_key >= (m_translation_key_array.size() - 1)) ? begin_key : begin_key + 1;

		ulong begin_time = m_translation_key_array[begin_key]->m_time;
		ulong end_time = m_translation_key_array[end_key]->m_time;

		vector3<double> begin_translate = m_translation_key_array[begin_key]->m_translate;
		vector3<double> end_translate = m_translation_key_array[end_key]->m_translate;

		double time_diff = end_time - begin_time;
		if (time_diff < 1.0)
			time_diff = 1.0;

		double scalar = (in_time - begin_time) / time_diff;

		vector3<double> translate_diff = end_translate - begin_translate;

		vector3<double> final_translate = begin_translate + translate_diff * scalar;

		in_mat = in_mat * matrix4<double>::translation(final_translate.x, final_translate.y, final_translate.z);
	}

	void usx_animation::update_matrix_keys(ulong in_time, matrix4<double>& in_mat)
	{
		if (m_matrix_key_array.size() <= 0) return;

		ulong begin_key = 0;
		for (int i = 0; i < (int)m_matrix_key_array.size(); i++)
		{
			if (in_time >= m_matrix_key_array[i]->m_time)
				begin_key = i;
		}
		ulong end_key = (begin_key >= (m_matrix_key_array.size() - 1)) ? begin_key : begin_key + 1;

		ulong begin_time = m_matrix_key_array[begin_key]->m_time;
		ulong end_time = m_matrix_key_array[end_key]->m_time;

		matrix4<double> begin_matrix = m_matrix_key_array[begin_key]->m_matrix;
		matrix4<double> end_matrix = m_matrix_key_array[end_key]->m_matrix;

		double time_diff = end_time - begin_time;
		if (time_diff < 1.0)
			time_diff = 1.0;

		double scalar = (in_time - begin_time) / time_diff;

		matrix4<double> matrix_diff = end_matrix - begin_matrix;

		matrix4<double> final_matrix = begin_matrix + matrix_diff * scalar;

		//matrix4<double> final_matrix = begin_matrix;

		in_mat = in_mat * final_matrix;
	}


	//////////////////////////////////////////////////////////////////////////
	usx_animation_set::usx_animation_set(void)
	{
		m_time_length = 0;
	}

	usx_animation_set::~usx_animation_set(void)
	{
		for (int i = 0; i < (int)m_animation_array.size(); i++)
		{
			AUTO_DELETE(m_animation_array[i]);
		}
	}

	void usx_animation_set::attach_to_frame(usx_frame* root_frame)
	{
		for (int i = 0; i < (int)m_animation_array.size(); i++)
		{
			usx_animation* anim = m_animation_array[i];
			anim->m_frame = root_frame->find_frame(anim->m_name.c_str());
			if (anim->m_frame == NULL)
				continue;

			anim->m_frame->m_animation = anim;
		}
	}

	void usx_animation_set::update(ulong time)
	{
		if (time > m_time_length)
			time = m_time_length;

		for (int i = 0; i < (int)m_animation_array.size(); i++)
		{
			usx_animation* anim = m_animation_array[i];

			anim->update_keys(time);
		}
	}

}