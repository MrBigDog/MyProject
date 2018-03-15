///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_usx_frame_parser.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : usx_frame_parser class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "Stdafx.h"
#include "us_usx_frame_parser.h"
#include <usDotx/us_usx_parser_object_container.h>
#include <usDotx/us_usx_animation_set.h>
#include <usDotx/us_usx_parser_assistant.h>
#include <usDotx/us_usx_frame.h>
#include <usUtil/us_singleton.h>
#include <usUtil/us_string_ext.h>
#include <d3dx9xof.h>
#include <rmxfguid.h>
#include <guiddef.h>

namespace uniscope_globe
{
	usx_frame_parser::usx_frame_parser(void)
	{
		m_type = L"3d82ab46-62da-11cf-ab39-0020af71e433";

		m_frame_counter = 0;
	}

	usx_frame_parser::~usx_frame_parser(void)
	{

	}

	bool usx_frame_parser::parse_object(ID3DXFileData* dotx_file_data, ID3DXFileData* parent_dotx_file_data, ulong depth, void** data, usx_parser_object_container* in_container)
	{
		GUID guid_type;
		dotx_file_data->GetType(&guid_type);

		if (guid_type != TID_D3DRMFrame ||
			dotx_file_data->IsReference())
			return false;


		usx_frame* cur_frame = new usx_frame();

		cur_frame->m_name = singleton_usx_parser_assistant::instance().get_name(dotx_file_data);
		if (cur_frame->m_name == L"")
		{
			cur_frame->m_name = L"default_frame_";
			cur_frame->m_name = cur_frame->m_name + string_ext::wfrom_int(m_frame_counter++);
		}

		if (data == NULL)
		{
			cur_frame->m_frame_sibling = in_container->m_root_frame;
			in_container->m_root_frame = cur_frame;

			data = (void**)&cur_frame;
		}
		else
		{
			usx_frame* parent_frame = (usx_frame*)*data;
			cur_frame->m_frame_sibling = parent_frame->m_frame_first_child;
			parent_frame->m_frame_first_child = cur_frame;
			cur_frame->m_frame_parent = parent_frame;

			data = (void**)&cur_frame;
		}

		return usx_parser::parse_child_object(dotx_file_data, depth, data, in_container);
	}



	usx_frame_transform_matrix_parser::usx_frame_transform_matrix_parser(void)
	{
		m_type = L"f6f23f41-7686-11cf-8f52-0040333594a3";
	}

	usx_frame_transform_matrix_parser::~usx_frame_transform_matrix_parser(void)
	{

	}

	bool usx_frame_transform_matrix_parser::parse_object(ID3DXFileData* dotx_file_data, ID3DXFileData* parent_dotx_file_data, ulong depth, void** data, usx_parser_object_container* in_container)
	{
		GUID guid_type;
		dotx_file_data->GetType(&guid_type);

		if (guid_type != TID_D3DRMFrameTransformMatrix ||
			dotx_file_data->IsReference())
			return false;

		usx_frame* frame = (usx_frame*)*data;

		if (frame != NULL)
		{
			SIZE_T data_size = 0;

			void *data_pointer = NULL;

			dotx_file_data->Lock(&data_size, (const void **)&data_pointer);
			{
				D3DXMATRIX mat = *(D3DXMATRIX *)data_pointer;

				frame->m_original_mat = matrix4<double>((float*)&mat);

				frame->m_transform_mat = frame->m_original_mat;
			}

			dotx_file_data->Unlock();
		}

		return true;
	}

}