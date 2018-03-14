///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_extended_material.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : material_set class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_EXTENDED_MATERIAL_H_
#define _US_EXTENDED_MATERIAL_H_

#include <usCore\Export.h>
#include <usUtil\us_common_file.h>
#include <vector>

namespace uniscope_globe
{
	struct texture_file
	{
		ustring m_diffuse_texture;
		ustring m_self_illuminant_texture;
		ustring m_specular_texture;
		ustring m_bump_texture;
		~texture_file()
		{
			m_diffuse_texture.clear();
			m_self_illuminant_texture.clear();
			m_specular_texture.clear();
			m_bump_texture.clear();
		}
	};

	struct color_value
	{
		float r;
		float g;
		float b;
		float a;
		color_value()
		{
			r = g = b = a = 1.0;
		}

		color_value(float* p)
		{
			r = p[0];
			g = p[1];
			b = p[2];
			a = p[3];
		}

		bool operator == (const color_value& cv) const
		{
			return !(*this != cv);
		}

		bool operator != (const color_value& cv) const
		{
			return (this->a != cv.a || this->r != cv.r || this->g != cv.g || this->b != cv.b);
		}
	};

	class USCORE_EXPORT material
	{
	public:
		material(void) {}

		material(const material& v_mat)
		{
			m_diffuse_rgba = v_mat.m_diffuse_rgba;
			m_ambient_rgb = v_mat.m_ambient_rgb;
			m_specular_rgb = v_mat.m_specular_rgb;
			m_emissive_rgb = v_mat.m_emissive_rgb;
			m_power = v_mat.m_power;
			m_name = v_mat.m_name;
		}

		~material(void) {}

	public:
		color_value	m_diffuse_rgba;
		color_value m_ambient_rgb;
		color_value	m_specular_rgb;
		color_value	m_emissive_rgb;
		float		m_power;

		ustring m_name;
	};

	class USCORE_EXPORT material_set
	{
	public:
		material_set(void);

		~material_set(void);

		typedef std::vector<texture_file> texture_file_array;
		typedef std::vector<material> material_array;

	public:
		material_array m_materials;

		texture_file_array m_texture_files;
	};
}

#endif // _US_EXTENDED_MATERIAL_H_