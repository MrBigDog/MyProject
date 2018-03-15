
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_usx_mesh_parser.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : usx_mesh_parser class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_usx_mesh_parser.h"
#include <usDotx/us_usx_mesh.h>
#include <usDotx/us_usx_frame.h>
#include <usDotx/us_usx_parser_object_container.h>
#include <usDotx/us_usx_animation_set.h>
#include <usDotx/us_usx_parser_assistant.h>
#include <usDotx/us_usx_data_struct.h>
#include <usUtil/us_singleton.h>
#include <usUtil/us_string_ext.h>
#include <d3dx9xof.h>
#include <rmxfguid.h>
#include <guiddef.h>

namespace uniscope_globe
{
	usx_mesh_parser::usx_mesh_parser(void)
	{
		m_type = L"3d82ab44-62da-11cf-ab39-0020af71e433";
	}

	usx_mesh_parser::~usx_mesh_parser(void)
	{

	}

	bool usx_mesh_parser::parse_object(ID3DXFileData* dotx_file_data, ID3DXFileData* parent_dotx_file_data, ulong depth, void** data, usx_parser_object_container* in_container)
	{
		if (dotx_file_data == NULL)
			return false;

		bool is_reference = dotx_file_data->IsReference() ? true : false;

		GUID guid_type;
		dotx_file_data->GetType(&guid_type);

		// 做判断，如果该段不是Mesh，或者该段是引用对象，不解析
		if ((guid_type != TID_D3DRMMesh) || is_reference)
			return false;

		GUID guid_id;
		dotx_file_data->GetId(&guid_id);

		// 定义数据体
		usx_mesh* x_mesh = new usx_mesh;

		char my_name[256];
		memset(my_name, 0, 256);
		SIZE_T my_size = 256;
		dotx_file_data->GetName(my_name, &my_size);
		x_mesh->m_name = string_ext::to_wstring(my_name);

		// 数据读取的偏移
		int offset = 0;

		SIZE_T data_size = 0;
		unsigned char* out_data = NULL;
		dotx_file_data->Lock(&data_size, (LPCVOID*)&out_data);

		// vertex num
		memcpy((unsigned char*)&x_mesh->m_number_of_vertices, out_data, sizeof(x_mesh->m_number_of_vertices));
		offset += sizeof(x_mesh->m_number_of_vertices);

		// vertex data
		int nVertDataSize = sizeof(vector3<float>) * x_mesh->m_number_of_vertices;
		x_mesh->m_vertex_data = new vector3<float>[x_mesh->m_number_of_vertices];

		// do not use memory copy here in order to collect bounding box info.
		//memcpy( (unsigned char*)x_mesh->m_vertex_data, out_data + offset, nVertDataSize );

		x_mesh->m_bbox.make_invalid();
		float* v_float_ptr = (float*)(out_data + offset);
		for (int i = 0; i < x_mesh->m_number_of_vertices; i++)
		{
			x_mesh->m_vertex_data[i].x = *v_float_ptr;
			x_mesh->m_vertex_data[i].y = *(v_float_ptr + 1);
			x_mesh->m_vertex_data[i].z = *(v_float_ptr + 2);

			v_float_ptr += 3;

			x_mesh->m_bbox.combine(x_mesh->m_vertex_data[i]);
		}

		offset += nVertDataSize;

		// face num
		memcpy((unsigned char*)&x_mesh->m_number_of_faces, out_data + offset, sizeof(x_mesh->m_number_of_faces));
		offset += sizeof(x_mesh->m_number_of_faces);

		// face data
		int face_data_size = sizeof(ulong) * (x_mesh->m_number_of_faces << 2);
		x_mesh->m_face_index = new usx_tri_face_index[x_mesh->m_number_of_faces];
		ulong* temp_face_index = new ulong[(x_mesh->m_number_of_faces << 2)];
		memcpy((unsigned char*)temp_face_index, out_data + offset, face_data_size);
		offset += face_data_size;
		for (uint i = 0; i < x_mesh->m_number_of_faces; i++)
		{
			x_mesh->m_face_index[i].a = temp_face_index[(i << 2) + 1];
			x_mesh->m_face_index[i].b = temp_face_index[(i << 2) + 2];
			x_mesh->m_face_index[i].c = temp_face_index[(i << 2) + 3];
		}
		AUTO_DELETE(temp_face_index);

		dotx_file_data->Unlock();

		// 遍历子段
		bool b_result = false;
		int v_error_cnt = 0;

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
				v_error_cnt++;
				continue;
			}

			// 分析子模板
			b_result = singleton_usx_parser_assistant::instance().parse_simple_object(dotx_sub_file_data,
				dotx_file_data,
				depth,
				(void**)&x_mesh,
				in_container,
				dotx_sub_file_data->IsReference() ? true : false);

			// 释放数据对象
			AUTO_RELEASE(dotx_sub_file_data);

			// 出现错误，停止分析
			if (false == b_result)
				v_error_cnt++;
		}

		// fvf
		x_mesh->m_fvf = 0;
		x_mesh->m_vertex_stride = 0;
		if (x_mesh->m_skin_info != NULL)
		{
			x_mesh->m_fvf |= D3DFVF_XYZB5 | D3DFVF_LASTBETA_D3DCOLOR;
			x_mesh->m_vertex_stride += 3 * sizeof(float) + 4 * sizeof(float) + sizeof(DWORD);
		}
		else
		{
			x_mesh->m_fvf |= D3DFVF_XYZ;
			x_mesh->m_vertex_stride += 3 * sizeof(float);
		}

		if (x_mesh->m_normals != NULL)
		{
			x_mesh->m_fvf |= D3DFVF_NORMAL;
			x_mesh->m_vertex_stride += 3 * sizeof(float);
		}

		if (x_mesh->m_vertex_texture_coords.size() > 0)
		{
			x_mesh->m_fvf |= (ulong)x_mesh->m_vertex_texture_coords.size() << 8;
			x_mesh->m_vertex_stride += (ulong)x_mesh->m_vertex_texture_coords.size() * 2 * sizeof(float);
		}

		x_mesh->m_valid = true;

		if (data != NULL && x_mesh->m_skin_info == NULL)
		{
			usx_frame* parent_frame = (usx_frame*)(*data);
			in_container->m_mesh_frame_map.insert(std::make_pair(x_mesh, parent_frame));

			parent_frame->m_mesh = x_mesh;
			x_mesh->m_parent_frame = parent_frame;
		}
		else
		{
			in_container->m_mesh_frame_map.insert(std::make_pair(x_mesh, (usx_frame*)NULL));
			//in_container->m_mesh_array.push_back( x_mesh );
		}

		return true;
	}
}