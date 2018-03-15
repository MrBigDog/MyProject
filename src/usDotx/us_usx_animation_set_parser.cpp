///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_usx_animation_set_parser.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : usx_animation_set_parser class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_usx_animation_set_parser.h"
#include <usDotx/us_usx_parser_object_container.h>
#include <usDotx/us_usx_animation_set.h>
#include <usDotx/us_usx_parser_assistant.h>
#include <usUtil/us_singleton.h>
#include <d3dx9xof.h>
#include <rmxfguid.h>
#include <guiddef.h>

namespace uniscope_globe
{
	usx_animation_set_parser::usx_animation_set_parser(void)
	{
		m_type = L"3d82ab50-62da-11cf-ab39-0020af71e433";
	}

	usx_animation_set_parser::~usx_animation_set_parser(void)
	{

	}

	bool usx_animation_set_parser::parse_object(ID3DXFileData* dotx_file_data, ID3DXFileData* parent_dotx_file_data, ulong depth, void** data, usx_parser_object_container* in_container)
	{
		GUID guid_type;
		dotx_file_data->GetType(&guid_type);

		if (guid_type != TID_D3DRMAnimationSet ||
			dotx_file_data->IsReference())
			return false;

		usx_animation_set* v_animation_set = new usx_animation_set();
		v_animation_set->m_name = singleton_usx_parser_assistant::instance().get_name(dotx_file_data);

		in_container->m_animation_set_array.push_back(v_animation_set);

		return parse_child_object(dotx_file_data, depth, (void**)&v_animation_set, in_container);
	}
}