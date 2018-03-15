///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_usx_animation_key_parser.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : usx_animation_key_parser class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_usx_animation_key_parser.h"
#include <usDotx/us_usx_parser_object_container.h>
#include <usDotx/us_usx_animation_set.h>
#include <d3dx9xof.h>
#include <rmxfguid.h>
#include <guiddef.h>
namespace uniscope_globe
{
	usx_animation_key_parser::usx_animation_key_parser(void)
	{
		m_type = L"10dd46a8-775b-11cf-8f52-0040333594a3";
	}

	usx_animation_key_parser::~usx_animation_key_parser(void)
	{

	}

	bool usx_animation_key_parser::parse_object(ID3DXFileData* dotx_file_data, ID3DXFileData* parent_dotx_file_data, ulong depth, void** data, usx_parser_object_container* in_container)
	{
		GUID guid_type;
		dotx_file_data->GetType(&guid_type);

		if (guid_type != TID_D3DRMAnimationKey ||
			dotx_file_data->IsReference())
			return false;


		usx_animation* animation = (usx_animation*)(*data);

		void* data_pointer = NULL;
		SIZE_T size_of_data = 0;
		dotx_file_data->Lock(&size_of_data, (const void **)&data_pointer);

		//
		// reading data
		//
		ulong* cur_data_pointer = (ulong*)data_pointer;

		// frame type
		ulong file_data_type = *cur_data_pointer++;

		// number of frames
		ulong number_of_frames = *cur_data_pointer++;

		// reading actual frame data
		switch (file_data_type)
		{
		case 0:
		{
			// rotation
			for (ulong ni = 0; ni < number_of_frames; ni++)
			{
				usx_animation_key* key = new usx_animation_key;

				key->m_time = *cur_data_pointer++;

				if (key->m_time > animation->m_parent_set->m_time_length)
					animation->m_parent_set->m_time_length = key->m_time;

				cur_data_pointer++;

				float *float_pointer = (float*)cur_data_pointer;
				key->m_rotation.w = *float_pointer++;
				key->m_rotation.x = *float_pointer++;
				key->m_rotation.y = *float_pointer++;
				key->m_rotation.z = *float_pointer++;

				animation->m_rotation_key_array.push_back(key);

				cur_data_pointer += 4;
			}
		}
		break;
		case 1:
		{
			for (ulong ni = 0; ni < number_of_frames; ni++)
			{
				usx_animation_key* key = new usx_animation_key;

				key->m_time = *cur_data_pointer++;

				if (key->m_time > animation->m_parent_set->m_time_length)
					animation->m_parent_set->m_time_length = key->m_time;

				cur_data_pointer++;

				D3DXVECTOR3* vector_pointer = (D3DXVECTOR3*)cur_data_pointer;
				key->m_scale.x = (*vector_pointer).x;
				key->m_scale.y = (*vector_pointer).y;
				key->m_scale.z = (*vector_pointer).z;

				animation->m_scale_key_array.push_back(key);

				cur_data_pointer += 3;
			}
		}
		break;
		case 2:
		{
			for (ulong ni = 0; ni < number_of_frames; ni++)
			{
				usx_animation_key* key = new usx_animation_key;

				key->m_time = *cur_data_pointer++;

				if (key->m_time > animation->m_parent_set->m_time_length)
					animation->m_parent_set->m_time_length = key->m_time;

				cur_data_pointer++;

				D3DXVECTOR3* vector_pointer = (D3DXVECTOR3*)cur_data_pointer;
				key->m_translate.x = (*vector_pointer).x;
				key->m_translate.y = (*vector_pointer).y;
				key->m_translate.z = (*vector_pointer).z;

				animation->m_translation_key_array.push_back(key);

				cur_data_pointer += 3;
			}
		}
		break;
		case 4:
		{
			for (ulong ni = 0; ni < number_of_frames; ni++)
			{
				usx_animation_key* key = new usx_animation_key;

				key->m_time = *cur_data_pointer++;

				if (key->m_time > animation->m_parent_set->m_time_length)
					animation->m_parent_set->m_time_length = key->m_time;

				cur_data_pointer++;

				// 					D3DXMATRIX* matrix_pointer = (D3DXMATRIX *)cur_data_pointer;

				key->m_matrix = matrix4<double>((float*)cur_data_pointer);

				animation->m_matrix_key_array.push_back(key);

				cur_data_pointer += 16;
			}
		}
		break;
		}

		dotx_file_data->Unlock();

		return true;
	}
}