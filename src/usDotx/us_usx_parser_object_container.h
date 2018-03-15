///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_usx_parser_object_container.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : usx_parser_object_container class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_USX_PARSER_OBJECT_CONTAINER_H_
#define _US_USX_PARSER_OBJECT_CONTAINER_H_

#include <usDotx/Export.h>
#include <usUtil/us_common_file.h>
#include <map>
#include <vector>

namespace uniscope_globe
{
	//typedef std::vector<usx_mesh*> usx_mesh_array;

	class usx_parser;
	class usx_frame;
	class usx_mesh;
	class usx_animation_set;

	typedef std::map<usx_mesh*, usx_frame*> usx_mesh_frame_map;
	typedef std::vector<usx_animation_set*> usx_animation_set_array;

	class USDOTX_EXPORT  usx_parser_object_container
	{
	public:
		usx_parser_object_container(void);

		virtual ~usx_parser_object_container(void);

	public:
		void clear();

		void set_path(cpstr in_path);

		ustring get_path(void) { return m_path; }

	public:
		//usx_mesh_array m_mesh_array;

		usx_mesh_frame_map m_mesh_frame_map;

		usx_animation_set_array m_animation_set_array;

		usx_frame* m_root_frame;

	protected:
		ustring m_path;
	};
}

#endif // _US_USX_PARSER_OBJECT_CONTAINER_H_