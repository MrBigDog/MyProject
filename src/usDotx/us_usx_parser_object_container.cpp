///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_usx_parser_object_container.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : usx_parser_object_container class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_usx_parser_object_container.h"
#include <usDotx/us_usx_mesh.h>

namespace uniscope_globe
{
	usx_parser_object_container::usx_parser_object_container(void)
	{
		m_root_frame = NULL;
	}

	usx_parser_object_container::~usx_parser_object_container(void)
	{
		clear();
	}

	void usx_parser_object_container::clear()
	{
		usx_mesh_frame_map::iterator itr = m_mesh_frame_map.begin();
		for (; itr != m_mesh_frame_map.end(); itr++)
		{
			usx_mesh* v_mesh = itr->first;
			AUTO_DELETE(v_mesh);
		}
		m_mesh_frame_map.clear();

		AUTO_DELETE(m_root_frame);

		for (int i = 0; i < (int)m_animation_set_array.size(); i++)
		{
			AUTO_DELETE(m_animation_set_array[i]);
		}
		m_animation_set_array.clear();
	}

	void usx_parser_object_container::set_path(cpstr in_path)
	{
		m_path = in_path;

		ustring::size_type index = m_path.find_last_of('/');
		if (index != ustring::npos)
		{
			m_path = m_path.substr(0, index + 1);
		}

		index = m_path.find_last_of('\\');
		if (index != ustring::npos)
		{
			m_path = m_path.substr(0, index + 1);
		}

	}
}