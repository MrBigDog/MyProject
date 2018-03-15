///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_usx_animation_parser.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : usx_animation_parser class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_usx_animation_parser.h"
#include <usDotx/us_usx_parser_object_container.h>
#include <usDotx/us_usx_animation_set.h>
#include <usDotx/us_usx_parser_assistant.h>
#include <usUtil/us_singleton.h>
#include <d3dx9xof.h>
#include <rmxfguid.h>
#include <guiddef.h>

namespace uniscope_globe
{
	usx_animation_parser::usx_animation_parser(void)
	{
		m_type = L"3d82ab4f-62da-11cf-ab39-0020af71e433";
	}

	usx_animation_parser::~usx_animation_parser(void)
	{

	}

	bool usx_animation_parser::parse_object(ID3DXFileData* dotx_file_data, ID3DXFileData* parent_dotx_file_data, ulong depth, void** data, usx_parser_object_container* in_container)
	{
		GUID guid_type;
		dotx_file_data->GetType(&guid_type);

		if (guid_type != TID_D3DRMAnimation ||
			dotx_file_data->IsReference())
			return false;

		usx_animation_set* animation_set = (usx_animation_set*)(*data);

		usx_animation* animation = new usx_animation(animation_set);

		animation_set->m_animation_array.push_back(animation);

		// parse referenced frame
		SIZE_T child_count;
		dotx_file_data->GetChildren(&child_count);
		LPD3DXFILEDATA dotx_sub_file_data = NULL;
		for (SIZE_T i = 0; i < child_count; i++)
		{
			if (SUCCEEDED(dotx_file_data->GetChild(i, &dotx_sub_file_data)))
			{
				GUID sub_guid;
				dotx_sub_file_data->GetType(&sub_guid);

				if (sub_guid == TID_D3DRMFrame && dotx_sub_file_data->IsReference())
				{
					animation->m_name = singleton_usx_parser_assistant::instance().get_name(dotx_sub_file_data);
				}

				AUTO_RELEASE(dotx_sub_file_data);
			}
		}

		return parse_child_object(dotx_file_data, depth, (void**)&animation, in_container);
	}
}