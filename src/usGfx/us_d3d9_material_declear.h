///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_material_declear.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : us_d3d9_material_declear class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_D3D9_MATERIAL_DECLEAR_H_
#define _US_D3D9_MATERIAL_DECLEAR_H_
#include <usGfx/Export.h>
#include <usCore/us_extended_material.h>
#include <usCore/us_render_argument.h>
#include <usCore/us_resource_container.h>
#include <usUtil/us_common_file.h>

#include <windows.h>

namespace uniscope_globe
{
	struct material_entry
	{
		color_value m_diffuse_color;
		color_value m_ambient_color;
		color_value m_specular_color;
		color_value m_emissive_color;
		float m_power;

		ustring m_name;
		resource_container<ustring>* m_diffuse;
		resource_container<ustring>* m_self_illuminant;
		resource_container<ustring>* m_specular;
		resource_container<ustring>* m_bump;
		material_entry()
		{
			ZeroMemory((void*)&m_emissive_color, sizeof(color_value));
			ZeroMemory((void*)&m_specular_color, sizeof(color_value));

			m_diffuse = NULL;
			m_self_illuminant = NULL;
			m_specular = NULL;
			m_bump = NULL;
			m_power = 1;
		}
	};
	typedef std::vector<material_entry> material_entry_array;

	struct material_index_entry
	{
		color_value m_diffuse_color;
		color_value m_ambient_color;
		color_value m_specular_color;
		color_value m_emissive_color;
		float m_power;

		ustring m_name;

		int m_texture_type;
		resource_container<interior_index>* m_diffuse;
		resource_container<interior_index>* m_self_illuminant;
		resource_container<interior_index>* m_specular;
		resource_container<interior_index>* m_bump;
		material_index_entry()
		{
			m_texture_type = US_MATERIAL_TYPE_NORMAL;
			ZeroMemory((void*)&m_emissive_color, sizeof(color_value));
			ZeroMemory((void*)&m_specular_color, sizeof(color_value));

			m_diffuse = NULL;
			m_self_illuminant = NULL;
			m_specular = NULL;
			m_bump = NULL;
			m_power = 1;
		}
	};
	typedef std::vector<material_index_entry> material_index_entry_array;

	class hardware_texture;

	struct render_texture_entry
	{
		hardware_texture* m_diffuse;
		hardware_texture* m_self_illuminant;
		hardware_texture* m_specular;
		hardware_texture* m_bump;
		render_texture_entry()
		{
			m_diffuse = NULL;
			m_self_illuminant = NULL;
			m_specular = NULL;
			m_bump = NULL;
		}
	};
	typedef std::vector<render_texture_entry> render_texture_array;

	struct render_texture_lod_entry
	{
		int m_texture_type;
		hardware_texture* m_diffuse;
		int m_diffuse_lod;
		hardware_texture* m_self_illuminant;
		int m_self_illuminant_lod;
		hardware_texture* m_specular;
		int m_specular_lod;
		hardware_texture* m_bump;
		int m_bump_lod;
		render_texture_lod_entry()
		{
			m_texture_type = US_MATERIAL_TYPE_NORMAL;
			m_diffuse = NULL;
			m_self_illuminant = NULL;
			m_specular = NULL;
			m_bump = NULL;
			m_diffuse_lod = 0;
			m_self_illuminant_lod = 0;
			m_specular_lod = 0;
			m_bump_lod = 0;
		}
	};
	typedef std::vector<render_texture_lod_entry> render_texture_lod_entry_array;
}

#endif // _US_D3D9_MATERIAL_DECLEAR_H_