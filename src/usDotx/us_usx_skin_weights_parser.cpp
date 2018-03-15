///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_usx_skin_weights_parser.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : usx_skin_weights_parser class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_usx_skin_weights_parser.h"
#include <usDotx/us_usx_mesh.h>
#include <usDotx/us_usx_skin_info.h>
#include <usDotx/us_usx_parser_object_container.h>

#include <usUtil/us_string_ext.h>
#include <usUtil/us_matrix4.h>

#include <d3dx9xof.h>

namespace uniscope_globe
{
	usx_skin_weights_parser::usx_skin_weights_parser(void)
	{
		m_type = L"6f0d123b-bad2-4167-a0d0-80224f25fabb";
	}

	usx_skin_weights_parser::~usx_skin_weights_parser(void)
	{

	}

	bool usx_skin_weights_parser::parse_object(ID3DXFileData* dotx_file_data, ID3DXFileData* parent_dotx_file_data, ulong depth, void** data, usx_parser_object_container* in_container)
	{
		if (dotx_file_data->IsReference())
			return false;

		usx_mesh* v_mesh = (usx_mesh*)(*data);

		void* data_pointer = NULL;
		SIZE_T size_of_data = 0;
		dotx_file_data->Lock(&size_of_data, (const void **)&data_pointer);

		usx_weights_info* v_info = new usx_weights_info;

		char* char_ptr = (char*)data_pointer;

		v_info->m_frame_name = string_ext::to_wstring(char_ptr);

		char_ptr += v_info->m_frame_name.size() + 1;

		ulong* ulong_ptr = (ulong*)char_ptr;
		ulong num_of_weights = *ulong_ptr;
		ulong_ptr++;

		for (ulong i = 0; i < num_of_weights; i++)
		{
			v_info->m_indices.push_back(*ulong_ptr);
			ulong_ptr++;
		}

		float* float_ptr = (float*)ulong_ptr;

		for (ulong i = 0; i < num_of_weights; i++)
		{
			v_info->m_weights.push_back(*float_ptr);
			float_ptr++;
		}

		v_info->m_offset_matrix = matrix4<double>(float_ptr);

		dotx_file_data->Unlock();


		v_mesh->m_skin_info->m_weights_array.push_back(v_info);
		return true;
	}
}