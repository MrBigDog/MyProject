
///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_texture_map_description.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : texture_map_description class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_TEXTURE_MAP_DESCRIPTION_H_
#define _US_TEXTURE_MAP_DESCRIPTION_H_

#include <usCore\Export.h>
#include <usUtil\us_matrix4.h>
#include <usCore\us_event_argument.h>

namespace uniscope_globe
{
	class USCORE_EXPORT texture_map_description
	{
	public:
		texture_map_description(void);

		texture_map_description(const texture_map_description& v_style);

		virtual ~texture_map_description(void);

	public:
		virtual matrix_4f get_texture_matrix(void);

		void set_texture_matrix_dirty(void) { m_is_texture_matrix_dirty = true; }

		void copy_texture_map_desc(texture_map_description* v_tex_map_desc);

	public:
		float	m_tu_start;
		float	m_tv_start;
		//float	m_tw_start;

		float	m_tu_end;		// max: 8192
		float	m_tv_end;
		//float	m_tw_end;

		float   m_rotation_u;
		float	m_rotation_v;
		float	m_rotation_w;

		bool    m_mirror_texture_u;
		bool    m_mirror_texture_v;
		bool    m_two_sided_mode;

		matrix_4f m_texture_matrix;

		bool	m_is_texture_matrix_dirty;
	};

	class texture_map_description_argument : public event_argument
	{
	public:
		texture_map_description_argument()
		{
			m_texture_map_desc = NULL;
		}

		texture_map_description_argument(texture_map_description* v_texture_map_desc)
		{
			m_texture_map_desc = v_texture_map_desc;
		}

		~texture_map_description_argument()
		{

		}

	public:
		texture_map_description* m_texture_map_desc;

	};
}

#endif // _US_TEXTURE_MAP_DESCRIPTION_H_ 