///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_usx_parser.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : usx_parser class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_usx_parser.h"
#include <usDotx/us_usx_parser_assistant.h>

#include <d3dx9xof.h>

namespace uniscope_globe
{
	usx_parser::usx_parser(void)
	{
	}

	usx_parser::~usx_parser(void)
	{

	}

	bool usx_parser::parse_object(ID3DXFileData* dotx_file_data, ID3DXFileData* parent_dotx_file_data, ulong depth, void** data, usx_parser_object_container* in_container)
	{
		return true;
	}

	bool usx_parser::parse_child_object(ID3DXFileData* dotx_file_data, ulong depth, void** data, usx_parser_object_container* in_container)
	{
		// 获取子模板数目
		SIZE_T child_count;
		dotx_file_data->GetChildren(&child_count);

		// 遍历模板并分析
		LPD3DXFILEDATA dotx_sub_file_data = NULL;
		for (SIZE_T i = 0; i < child_count; i++)
		{
			// 取子模板对象
			if (FAILED(dotx_file_data->GetChild(i, &dotx_sub_file_data)))
			{
				continue;
			}

			ustring str_guid = singleton_usx_parser_assistant::instance().get_guid(dotx_sub_file_data);

			usx_parser* parser = singleton_usx_parser_assistant::instance().get_usx_parser(str_guid.c_str());
			if (parser != NULL)
			{
				parser->parse_object(dotx_sub_file_data, dotx_file_data, depth + 1, data, in_container);
			}

			AUTO_RELEASE(dotx_sub_file_data);
		}

		return true;
	}
}