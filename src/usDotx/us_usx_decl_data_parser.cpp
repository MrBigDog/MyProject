///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_usx_decl_data_parser.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : usx_decl_data_parser class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_usx_decl_data_parser.h"
#include <usDotx/us_usx_parser_object_container.h>
#include <usDotx/us_usx_animation_set.h>
#include <usDotx/us_usx_parser_assistant.h>
#include <usDotx/us_usx_mesh.h>
#include <usUtil/us_singleton.h>
#include <d3dx9xof.h>
#include <rmxfguid.h>
#include <guiddef.h>

namespace uniscope_globe
{
	usx_decl_data_parser::usx_decl_data_parser(void)
	{
		m_type = L"bf22e553-292c-4781-9fea-62bd554bdd93";
	}

	usx_decl_data_parser::~usx_decl_data_parser(void)
	{

	}

	bool usx_decl_data_parser::parse_object(ID3DXFileData* dotx_file_data, ID3DXFileData* parent_dotx_file_data, ulong depth, void** data, usx_parser_object_container* in_container)
	{
		if (dotx_file_data == NULL || data == NULL)
			return false;

		bool is_reference = dotx_file_data->IsReference() ? true : false;

		GUID guid_type;
		dotx_file_data->GetType(&guid_type);

		// 做判断，如果该段不是Mesh，或者该段是引用对象，不解析
		if ((guid_type != DXFILEOBJ_DeclData || is_reference))
			return false;

		// 定义数据体
		usx_mesh* pMesh = (usx_mesh*)(*data);

		ulong elements = 0;
		usx_vertex_element* element_array = NULL;

		ulong number_dword = 0;
		ulong* dword_data = NULL;

		// 数据读取的偏移
		int offset = 0;

		SIZE_T out_data_size = 0;
		unsigned char* out_data = NULL;
		dotx_file_data->Lock(&out_data_size, (LPCVOID*)&out_data);

		memcpy((unsigned char*)&elements, out_data, sizeof(elements));
		offset += sizeof(elements);

		element_array = new usx_vertex_element[elements];
		int nElemArraySize = sizeof(usx_vertex_element) * elements;
		memcpy((unsigned char*)element_array, out_data + offset, nElemArraySize);
		offset += nElemArraySize;

		memcpy((unsigned char*)&number_dword, out_data + offset, sizeof(number_dword));
		offset += sizeof(number_dword);

		dword_data = new ulong[number_dword];
		int dword_data_size = sizeof(ulong)*number_dword;
		memcpy((unsigned char*)dword_data, out_data + offset, dword_data_size);
		offset += dword_data_size;

		dotx_file_data->Unlock();

		ulong nOffset = 0;
		// 导出数据
		for (UINT i = 0; i < elements; i++)
		{
			if (nOffset > number_dword)
				break;

			switch (element_array[i].usage)
			{
			case D3DDECLUSAGE_TEXCOORD:
				parse_decl_texture_coord(nOffset, dword_data, element_array[i], pMesh);
				break;
			}
		}

		//释放对象
		AUTO_DELETE_ARRAY(element_array);
		AUTO_DELETE(dword_data);

		return true;
	}

	int usx_decl_data_parser::get_element_number(ulong element_type)
	{
		int ret_val = 0;
		switch (element_type)
		{
		case D3DDECLTYPE_FLOAT1:
			ret_val = 1;
			break;
		case D3DDECLTYPE_FLOAT2:
			ret_val = 2;
			break;
		case D3DDECLTYPE_FLOAT3:
			ret_val = 3;
			break;
		case D3DDECLTYPE_FLOAT4:
		case D3DDECLTYPE_D3DCOLOR:
			ret_val = 4;
			break;
		}

		return ret_val;
	}

	void usx_decl_data_parser::parse_decl_texture_coord(ulong& offset, ulong* decl_data, usx_vertex_element& vertex_element, usx_mesh* mesh_data)
	{
		int number_of_elements = get_element_number(vertex_element.type);

		int nValueNum = number_of_elements*mesh_data->m_number_of_vertices;
		float* pTextCoord = new float[nValueNum];


		DWORD* pData = decl_data + offset;
		memcpy(pTextCoord, pData, sizeof(float)*nValueNum);

		offset += nValueNum;

		mesh_data->m_vertex_texture_coords.push_back(pTextCoord);
	}
}