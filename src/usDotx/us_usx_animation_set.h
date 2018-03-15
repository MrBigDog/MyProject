///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_usx_animation_set.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : usx_animation_set class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_USX_ANIMATION_SET_H_
#define _US_USX_ANIMATION_SET_H_

#include <usDotx/Export.h>
#include <usUtil/us_common_file.h>
#include <usUtil/us_matrix4.h>

namespace uniscope_globe
{
	class usx_frame;

	class USDOTX_EXPORT usx_animation_key
	{
	public:
		ulong       m_time;

		quaternion<double> m_rotation;

		vector3<double> m_scale;

		vector3<double> m_translate;

		matrix4<double> m_matrix;
	};

	typedef std::vector<usx_animation_key*> animation_key_array;

	class usx_animation_set;

	class USDOTX_EXPORT usx_animation
	{
	protected:
		usx_animation() {}

	public:
		usx_animation(usx_animation_set* in_parent_set);

		virtual ~usx_animation();

		void update_keys(ulong in_time);

	protected:
		void update_scale_keys(ulong in_time, matrix4<double>& in_mat);

		void update_rotation_keys(ulong in_time, matrix4<double>& in_mat);

		void update_translation_keys(ulong in_time, matrix4<double>& in_mat);

		void update_matrix_keys(ulong in_time, matrix4<double>& in_mat);

	public:
		ustring m_name;

		usx_frame* m_frame;

		animation_key_array m_rotation_key_array;
		animation_key_array m_scale_key_array;
		animation_key_array m_translation_key_array;
		animation_key_array m_matrix_key_array;

		ulong m_begin_key;
		ulong m_end_key;
		double m_scalar;

		usx_animation_set* m_parent_set;
	};

	class USDOTX_EXPORT usx_animation_set
	{
	public:
		typedef std::vector<usx_animation*> animation_array;

	public:
		usx_animation_set(void);

		virtual ~usx_animation_set(void);

		void attach_to_frame(usx_frame* root_frame);

		void update(ulong time);

	public:
		ustring m_name;

		ulong m_time_length;

		animation_array m_animation_array;
	};
}

#endif // _US_USX_ANIMATION_SET_H_