///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_usx_skin_mesh_header_parser.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : usx_skin_mesh_header_parser class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_usx_skin_mesh_header_parser.h"
#include <usDotx/us_usx_mesh.h>
//#include <usDotx/us_usx_frame.h>
#include <usDotx/us_usx_parser_object_container.h>
//#include <usDotx/us_usx_animation_set.h>
#include <usDotx/us_usx_parser_assistant.h>
#include <usDotx/us_usx_data_struct.h>
#include <usDotx/us_usx_skin_info.h>
//#include <usUtil/us_singleton.h>
//#include <usUtil/us_string_ext.h>
#include <d3dx9xof.h>
//#include <rmxfguid.h>
//#include <guiddef.h>

namespace uniscope_globe
{
	usx_skin_mesh_header_parser::usx_skin_mesh_header_parser(void)
	{
		m_type = L"3cf169ce-ff7c-44ab-93c0-f78f62d172e2";
	}

	usx_skin_mesh_header_parser::~usx_skin_mesh_header_parser(void)
	{

	}

	bool usx_skin_mesh_header_parser::parse_object(ID3DXFileData* dotx_file_data, ID3DXFileData* parent_dotx_file_data, ulong depth, void** data, usx_parser_object_container* in_container)
	{
		if (dotx_file_data->IsReference())
			return false;

		usx_mesh* v_mesh = (usx_mesh*)(*data);

		v_mesh->m_skin_info = new usx_skin_info();

		void* data_pointer = NULL;
		SIZE_T size_of_data = 0;
		dotx_file_data->Lock(&size_of_data, (const void **)&data_pointer);

		ushort* cur_data_ptr = (ushort*)data_pointer;

		v_mesh->m_skin_info->m_weights_per_vertex = *cur_data_ptr;

		cur_data_ptr++;

		v_mesh->m_skin_info->m_weights_per_face = *cur_data_ptr;

		cur_data_ptr++;

		v_mesh->m_skin_info->m_num_of_bones = *cur_data_ptr;

		dotx_file_data->Unlock();

		return true;
	}
}