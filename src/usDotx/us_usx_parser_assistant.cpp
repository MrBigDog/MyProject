///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_usx_parser_assistant.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : usx_parser_assistant class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_usx_template.h"
#include "us_usx_parser_assistant.h"
#include <usDotx/us_usx_mesh_parser.h>
#include <usDotx/us_usx_decl_data_parser.h>
#include <usDotx/us_usx_frame_parser.h>
#include <usDotx/us_usx_animation_parser.h>
#include <usDotx/us_usx_animation_set_parser.h>
#include <usDotx/us_usx_animation_key_parser.h>
#include <usDotx/us_usx_skin_mesh_header_parser.h>
#include <usDotx/us_usx_skin_weights_parser.h>
#include <usDotx/us_usx_mesh.h>
#include <usDotx/us_usx_material.h>
#include <usDotx/us_usx_parser_object_container.h>

#include <usUtil/us_vector3.h>
#include <usUtil/us_string_ext.h>

#include <dxfile.h>
#include <d3dx9xof.h>
#include <rmxfguid.h>

//extern unsigned char D3DRM_XTEMPLATES[];
#define D3DRMXTEMPLATEBYTES 3278

namespace uniscope_globe
{
	usx_parser_assistant* singleton_usx_parser_assistant::s_ins = NULL;

	usx_parser_assistant::usx_parser_assistant(void)
	{
		usx_mesh_parser* mesh_parser = new usx_mesh_parser;
		m_dotx_file_parser_map[mesh_parser->m_type] = mesh_parser;

		usx_decl_data_parser* decl_data_parser = new usx_decl_data_parser;
		m_dotx_file_parser_map[decl_data_parser->m_type] = decl_data_parser;

		usx_frame_parser* frame_parser = new usx_frame_parser;
		m_dotx_file_parser_map[frame_parser->m_type] = frame_parser;

		usx_frame_transform_matrix_parser* frame_transform_matrix_parser = new usx_frame_transform_matrix_parser();
		m_dotx_file_parser_map[frame_transform_matrix_parser->m_type] = frame_transform_matrix_parser;

		usx_animation_set_parser* animation_set_parser = new usx_animation_set_parser;
		m_dotx_file_parser_map[animation_set_parser->m_type] = animation_set_parser;

		usx_animation_parser* animation_parser = new usx_animation_parser;
		m_dotx_file_parser_map[animation_parser->m_type] = animation_parser;

		usx_animation_key_parser* animation_key_parser = new usx_animation_key_parser;
		m_dotx_file_parser_map[animation_key_parser->m_type] = animation_key_parser;

		usx_skin_mesh_header_parser* skin_mesh_header_parser = new usx_skin_mesh_header_parser;
		m_dotx_file_parser_map[skin_mesh_header_parser->m_type] = skin_mesh_header_parser;

		usx_skin_weights_parser* skin_weights_parser = new usx_skin_weights_parser;
		m_dotx_file_parser_map[skin_weights_parser->m_type] = skin_weights_parser;
	}

	usx_parser_assistant::~usx_parser_assistant(void)
	{
		dotx_file_parser_map::iterator itr = m_dotx_file_parser_map.begin();
		for (; itr != m_dotx_file_parser_map.end(); itr++)
		{
			AUTO_DELETE(itr->second);
		}
		m_dotx_file_parser_map.clear();
	}

	usx_parser* usx_parser_assistant::get_usx_parser(const LPCTSTR type)
	{
		dotx_file_parser_map::iterator itr;
		itr = m_dotx_file_parser_map.find(type);
		if (itr == m_dotx_file_parser_map.end())
			return NULL;
		return itr->second;
	}

	bool usx_parser_assistant::parse_x_file(char *file_name, usx_parser_object_container* in_container)
	{
		if (file_name == NULL)
		{
			return false;
		}

		ID3DXFile           *pD3DXFile = NULL;
		ID3DXFileEnumObject *pD3DXEnum = NULL;

		// 创建X文件对象
		if (FAILED(D3DXFileCreate(&pD3DXFile)))
		{
			return false;
		}

		// 注册标准模板
		if (FAILED(pD3DXFile->RegisterTemplates((LPVOID)usx_template::get_default_template(), D3DRMXTEMPLATEBYTES)))
		{
			AUTO_RELEASE(pD3DXFile);
			return false;
		}


		/*// 注册自定义模板
		if ( FAILED( pD3DXFile->RegisterTemplates( (LPVOID)usxTemplates, strlen(usxTemplates) ) ) )
		{
			AUTO_RELEASE( pD3DXFile );
			return false;
		}//*/

		HRESULT hr = 0;
		// 创建枚举对象
		if (FAILED(hr = pD3DXFile->CreateEnumObject((LPVOID)file_name, DXFILELOAD_FROMFILE, &pD3DXEnum)))
		{
			DWORD nError = GetLastError();
			AUTO_RELEASE(pD3DXFile);
			return false;
		}

		//获得子对象个数
		SIZE_T objNum = 0;
		pD3DXEnum->GetChildren(&objNum);

		//遍历子对象(顶级)
		ID3DXFileData *pDXFileData = NULL;
		for (SIZE_T i = 0; i < objNum; i++)
		{
			if (SUCCEEDED(pD3DXEnum->GetChild(i, &pDXFileData)))
			{
				ustring str_guid = get_guid(pDXFileData);

				usx_parser* pParser = get_usx_parser(str_guid.c_str());

				if (pParser != NULL)
				{
					pParser->parse_object(pDXFileData, NULL, 0, NULL, in_container);
				}

				AUTO_RELEASE(pDXFileData);
			}
		}


		// 释放资源 		
		AUTO_RELEASE(pD3DXEnum);
		AUTO_RELEASE(pD3DXFile);

		return true;
	}

	bool usx_parser_assistant::parse_x_buffer(unsigned char *mem_buffer, ulong buffer_size, usx_parser_object_container* in_container)
	{
		if (mem_buffer == NULL)
		{
			return false;
		}

		ID3DXFile           *pD3DXFile = NULL;
		ID3DXFileEnumObject *pD3DXEnum = NULL;

		// 创建X文件对象
		if (FAILED(D3DXFileCreate(&pD3DXFile)))
		{
			return false;
		}

		// 注册标准模板
		if (FAILED(pD3DXFile->RegisterTemplates((LPVOID)usx_template::get_default_template(), D3DRMXTEMPLATEBYTES)))
		{
			AUTO_RELEASE(pD3DXFile);
			return false;
		}

		// 		// 注册自定义模板
		// 		if ( FAILED( pD3DXFile->RegisterTemplates( (LPVOID)usxTemplates, strlen(usxTemplates) ) ) )
		// 		{
		// 			AUTO_RELEASE( pD3DXFile );
		// 			return false;
		// 		}

				// 创建枚举对象
		D3DXF_FILELOADMEMORY xFileMemory;
		xFileMemory.dSize = buffer_size;
		xFileMemory.lpMemory = mem_buffer;
		HRESULT hr = S_OK;
		if (FAILED(hr = pD3DXFile->CreateEnumObject((LPVOID)&xFileMemory, D3DXF_FILELOAD_FROMMEMORY, &pD3DXEnum)))
		{
			AUTO_RELEASE(pD3DXFile);
			return false;
		}

		// 获得子对象个数
		SIZE_T objNum = 0;
		pD3DXEnum->GetChildren(&objNum);

		// 遍历子对象(顶级)
		ID3DXFileData *pDXFileData = NULL;
		for (SIZE_T i = 0; i < objNum; i++)
		{
			if (SUCCEEDED(pD3DXEnum->GetChild(i, &pDXFileData)))
			{
				ustring str_guid = get_guid(pDXFileData);
				usx_parser* pParser = get_usx_parser(str_guid.c_str());

				if (pParser != NULL)
				{
					pParser->parse_object(pDXFileData, NULL, 0, NULL, in_container);
				}

				AUTO_RELEASE(pDXFileData);
			}
		}

		// 释放资源 		
		AUTO_RELEASE(pD3DXEnum);
		AUTO_RELEASE(pD3DXFile);

		return true;
	}


	bool usx_parser_assistant::parse_simple_object(ID3DXFileData *dotx_file_data, ID3DXFileData *parent_dotx_file_data, ulong depth, void** data, usx_parser_object_container* in_container, bool is_reference)
	{
		if (dotx_file_data == NULL || parent_dotx_file_data == NULL || data == NULL)
			return false;

		GUID guid;
		dotx_file_data->GetType(&guid);

		bool is_success = false;

		if (guid == TID_D3DRMMeshNormals)
		{
			is_success = parse_mesh_normals(dotx_file_data, parent_dotx_file_data, depth + 1, data, in_container, is_reference);
		}
		else if (guid == TID_D3DRMMeshMaterialList)
		{
			is_success = parse_material_list(dotx_file_data, parent_dotx_file_data, depth + 1, data, in_container, is_reference);
		}
		else if (guid == TID_D3DRMMeshTextureCoords)
		{
			is_success = parse_mesh_texture_coords(dotx_file_data, parent_dotx_file_data, depth + 1, data, in_container, is_reference);
		}
		else if (guid == TID_D3DRMMaterial)
		{
			is_success = parse_material(dotx_file_data, parent_dotx_file_data, depth + 1, data, in_container, is_reference);
		}
		else if (guid == TID_D3DRMMeshVertexColors)
		{
			is_success = parse_vertex_colors(dotx_file_data, parent_dotx_file_data, depth + 1, data, in_container, is_reference);
		}
		else
		{
			ustring str_guid = get_guid(dotx_file_data);
			usx_parser* parser = singleton_usx_parser_assistant::instance().get_usx_parser(str_guid.c_str());
			if (parser)
			{
				is_success = parser->parse_object(dotx_file_data, parent_dotx_file_data, depth + 1, data, in_container);
			}
		}

		return is_success;
	}

	bool usx_parser_assistant::parse_mesh_normals(ID3DXFileData* dotx_file_data, ID3DXFileData* parent_dotx_file_data, ulong depth, void** data, usx_parser_object_container* in_container, bool is_reference)
	{
		if (dotx_file_data == NULL || parent_dotx_file_data == NULL || data == NULL)
			return false;

		ulong number_of_normals = 0, number_of_face_normals = 0;
		vector3<float>* normal_data = NULL;
		ulong* face_normal_data = NULL;

		int offset = 0;

		SIZE_T data_size = 0;
		BYTE* output_data = NULL;
		dotx_file_data->Lock(&data_size, (LPCVOID*)&output_data);

		// 法向个数
		memcpy((BYTE*)&number_of_normals, output_data, sizeof(number_of_normals));
		offset += sizeof(number_of_normals);

		// 法向数据
		int nNormalDataSize = sizeof(vector3<float>) * number_of_normals;
		normal_data = new vector3<float>[number_of_normals];
		memcpy((BYTE*)normal_data, output_data + offset, nNormalDataSize);
		offset += nNormalDataSize;

		// 面个数
		memcpy((BYTE*)&number_of_face_normals, (BYTE*)output_data + offset, sizeof(number_of_face_normals));
		offset += sizeof(number_of_face_normals);

		// 面法向数据
		int nFaceNormalDataSize = sizeof(ulong) * (number_of_face_normals << 2);
		face_normal_data = new ulong[number_of_face_normals << 2];
		memcpy((BYTE*)face_normal_data, (BYTE*)output_data + offset, nFaceNormalDataSize);
		offset += nFaceNormalDataSize;

		dotx_file_data->Unlock();

		// 解释data
		usx_mesh* mesh = (usx_mesh*)(*data);
		mesh->m_normals = new vector3<float>[mesh->m_number_of_vertices];

		UINT nVertIndex = 0, nNormalIndex = 0;
		for (UINT i = 0; i < mesh->m_number_of_faces; i++)
		{
			nVertIndex = mesh->m_face_index[i].a;
			nNormalIndex = face_normal_data[i * 4 + 1];
			mesh->m_normals[nVertIndex].x = normal_data[nNormalIndex].x;
			mesh->m_normals[nVertIndex].y = normal_data[nNormalIndex].y;
			mesh->m_normals[nVertIndex].z = normal_data[nNormalIndex].z;

			nVertIndex = mesh->m_face_index[i].b;
			nNormalIndex = face_normal_data[i * 4 + 2];
			mesh->m_normals[nVertIndex].x = normal_data[nNormalIndex].x;
			mesh->m_normals[nVertIndex].y = normal_data[nNormalIndex].y;
			mesh->m_normals[nVertIndex].z = normal_data[nNormalIndex].z;

			nVertIndex = mesh->m_face_index[i].c;
			nNormalIndex = face_normal_data[i * 4 + 3];
			mesh->m_normals[nVertIndex].x = normal_data[nNormalIndex].x;
			mesh->m_normals[nVertIndex].y = normal_data[nNormalIndex].y;
			mesh->m_normals[nVertIndex].z = normal_data[nNormalIndex].z;
		}

		// 释放临时数据
		AUTO_DELETE_ARRAY(normal_data);
		AUTO_DELETE(face_normal_data);

		return true;
	}

	bool usx_parser_assistant::parse_material_list(ID3DXFileData* dotx_file_data, ID3DXFileData* parent_dotx_file_data, ulong depth, void** data, usx_parser_object_container* in_container, bool is_reference)
	{
		if (dotx_file_data == NULL || parent_dotx_file_data == NULL || data == NULL)
			return false;

		usx_mesh* mesh = (usx_mesh*)(*data);

		ulong number_of_faces = 0;
		ulong* face_material_index_data = NULL;

		ulong number_of_materials = 0;

		int offset = 0;

		SIZE_T data_size = 0;
		BYTE* output_data = NULL;
		dotx_file_data->Lock(&data_size, (LPCVOID*)&output_data);

		memcpy((BYTE*)&number_of_materials, output_data, sizeof(number_of_materials));
		offset += sizeof(number_of_materials);

		memcpy((BYTE*)&number_of_faces, output_data + offset, sizeof(number_of_faces));
		offset += sizeof(number_of_faces);

		int face_material_index_data_size = sizeof(ulong) * number_of_faces;
		ulong* attribute_data = new ulong[number_of_faces];
		memcpy((BYTE*)attribute_data, output_data + offset, face_material_index_data_size);
		offset += face_material_index_data_size;
		mesh->m_attribute_data = attribute_data;

		dotx_file_data->Unlock();

		// 获取子模板数目
		SIZE_T child_count;
		dotx_file_data->GetChildren(&child_count);
		if (child_count != number_of_materials)
		{
			mesh->m_material_safe = false;

			for (int i = 0; i < number_of_materials; i++)
			{
				usx_material* v_temp = new usx_material;

				// initialize material
				for (int i = 0; i < 4; i++)
				{
					v_temp->m_texture_files.push_back(usx_texture_file(i, L""));
				}

				mesh->m_materials.push_back(v_temp);
			}

			return false;
		}

		// 解析Material
		// 遍历模板并分析
		LPD3DXFILEDATA dotx_sub_file_data = NULL;
		for (SIZE_T i = 0; i < child_count; i++)
		{
			// 取子模板对象
			if (FAILED(dotx_file_data->GetChild(i, &dotx_sub_file_data)))
				break;

			usx_material* v_mat = new usx_material;

			// 分析子模板,它的子模板必为 Material
			bool bResult = parse_material(dotx_sub_file_data, dotx_file_data, depth++, (void**)&v_mat, in_container, dotx_sub_file_data->IsReference() ? true : false);

			// 释放数据对象
			AUTO_RELEASE(dotx_sub_file_data);

			// 出现错误，停止分析
			if (false == bResult)
				break;

			mesh->m_materials.push_back(v_mat);
		}

		return true;
	}

	bool usx_parser_assistant::parse_mesh_texture_coords(ID3DXFileData* dotx_file_data, ID3DXFileData* parent_dotx_file_data, ulong depth, void** data, usx_parser_object_container* in_container, bool is_reference)
	{
		if (dotx_file_data == NULL || parent_dotx_file_data == NULL || data == NULL)
			return false;

		usx_mesh* mesh = (usx_mesh*)(*data);

		ulong number_of_vertices = 0;

		int offset = 0;

		SIZE_T data_size = 0;
		BYTE* output_data = NULL;
		dotx_file_data->Lock(&data_size, (LPCVOID*)&output_data);

		memcpy((BYTE*)&number_of_vertices, output_data, sizeof(number_of_vertices));
		offset += sizeof(number_of_vertices);

		int nTexCoordsDataSize = sizeof(float) * number_of_vertices * 2;
		float* v_texture_coords = new float[number_of_vertices << 1];
		memcpy((BYTE*)v_texture_coords, output_data + offset, nTexCoordsDataSize);
		offset += nTexCoordsDataSize;

		dotx_file_data->Unlock();

		mesh->m_vertex_texture_coords.push_back(v_texture_coords);

		return true;
	}

	bool usx_parser_assistant::parse_material(ID3DXFileData* dotx_file_data, ID3DXFileData* parent_dotx_file_data, ulong depth, void** data, usx_parser_object_container* in_container, bool is_reference)
	{
		if (dotx_file_data == NULL || parent_dotx_file_data == NULL || data == NULL)
			return false;

		// 解释data
		usx_material* a_material = (usx_material*)(*data);

		ulong nColorRGBASize = sizeof(float) * 4;
		ulong nColorRGB = sizeof(float) * 3;

		int offset = 0;




		SIZE_T name_size = 0;
		dotx_file_data->GetName(NULL, &name_size);
		char* name = new char[name_size];
		dotx_file_data->GetName(name, &name_size);

		a_material->m_name = string_ext::to_wstring(name);

		AUTO_DELETE(name);

		SIZE_T data_size = 0;
		BYTE* output_data = NULL;
		dotx_file_data->Lock(&data_size, (LPCVOID*)&output_data);

		memcpy((BYTE*)&a_material->m_diffuse_color_rgba, output_data, nColorRGBASize);
		offset += nColorRGBASize;

		memcpy((BYTE*)&a_material->m_power, output_data + offset, sizeof(a_material->m_power));
		offset += sizeof(a_material->m_power);

		memcpy((BYTE*)&a_material->m_specular_color_rgb, output_data + offset, nColorRGB);
		offset += nColorRGB;

		memcpy((BYTE*)&a_material->m_emissive_color_rgb, output_data + offset, nColorRGB);
		offset += nColorRGB;

		dotx_file_data->Unlock();

		// 获取子模板数目
		SIZE_T child_count;
		dotx_file_data->GetChildren(&child_count);

		// initialize material
		for (int i = 0; i < 4; i++)
		{
			a_material->m_texture_files.push_back(usx_texture_file(i, L""));
		}

		// 遍历模板并分析
		LPD3DXFILEDATA dotx_sub_file_data = NULL;
		for (SIZE_T i = 0; i < child_count; i++)
		{
			// 取子模板对象
			if (FAILED(dotx_file_data->GetChild(i, &dotx_sub_file_data)))
				break;

			// 分析子模板,它的子模板必为 TextureFileName
			char name[32];
			memset(name, 0, 32);

			SIZE_T name_size = 32;
			dotx_sub_file_data->GetName(name, &name_size);

			// 判断类型
			int v_type = -1;
			if (string("Diffuse") == name)
				v_type = 0;
			else if (string("Self_Illumination") == name)
				v_type = 1;
			else if (string("Specular") == name)
				v_type = 2;
			else if (string("Bump") == name)
				v_type = 3;

			SIZE_T file_name_size = 0;
			char* file_name_memory = NULL;
			dotx_sub_file_data->Lock(&file_name_size, (LPCVOID*)&file_name_memory);

			char file_name[MAX_PATH];
			memset(file_name, 0, MAX_PATH);
			memcpy(file_name, file_name_memory, file_name_size);

			dotx_sub_file_data->Unlock();

			if (v_type != -1)
			{
				a_material->m_texture_files[v_type].m_texture_file = in_container->get_path() + string_ext::to_wstring(file_name);
			}
			else
			{
				a_material->m_texture_files[i].m_texture_file = in_container->get_path() + string_ext::to_wstring(file_name);
			}

		}

		return true;
	}

	bool usx_parser_assistant::parse_vertex_colors(ID3DXFileData* dotx_file_data, ID3DXFileData* parent_dotx_file_data, ulong depth, void** data, usx_parser_object_container* in_container, bool is_reference)
	{
		if (dotx_file_data == NULL || parent_dotx_file_data == NULL || data == NULL)
			return false;

		usx_mesh* mesh = (usx_mesh*)(*data);

		ulong number_of_vertices = 0;
		usx_index_color* a_index_data = NULL;

		int offset = 0;

		SIZE_T data_size = 0;
		BYTE* output_data = NULL;
		dotx_file_data->Lock(&data_size, (LPCVOID*)&output_data);

		memcpy((BYTE*)&number_of_vertices, output_data, sizeof(number_of_vertices));
		offset += sizeof(number_of_vertices);

		int vertex_color_data_size = sizeof(usx_index_color) * number_of_vertices;
		a_index_data = new usx_index_color[number_of_vertices];
		memcpy((BYTE*)a_index_data, output_data + offset, vertex_color_data_size);
		offset += vertex_color_data_size;

		dotx_file_data->Unlock();

		mesh->m_vertex_color = new usx_color_value[number_of_vertices];

		for (int i = 0; i < (int)number_of_vertices; i++)
		{
			mesh->m_vertex_color[i] = a_index_data[i].color_value;
		}

		AUTO_DELETE_ARRAY(a_index_data);

		return true;
	}

	ustring usx_parser_assistant::get_name(ID3DXFileData *dotx_file_data)
	{
		char* object_name = NULL;

		SIZE_T name_length = 0;
		if (FAILED(dotx_file_data->GetName(NULL, &name_length)))
			return NULL;

		if (name_length > 1)
		{
			if ((object_name = new char[name_length]) != NULL)
				dotx_file_data->GetName(object_name, &name_length);
		}

		ustring name = L"";
		if (object_name != NULL)
		{
			name = string_ext::to_wstring(string(object_name));
			AUTO_DELETE(object_name);
		}

		return name;
	}

	ustring usx_parser_assistant::get_guid(ID3DXFileData* dotx_file_data)
	{
		GUID guid;
		dotx_file_data->GetType(&guid);
		RPC_WSTR strGUID = NULL;
		UuidToString(&guid, &strGUID);
		ustring guid_string = (LPCTSTR)strGUID;

		US_STRING_LOWER(guid_string);

		RpcStringFree(&strGUID);

		return guid_string;
	}

}