///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_usx_material.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : usx_material class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_USX_MATERIAL_H_
#define _US_USX_MATERIAL_H_

#include <usDotx/Export.h>
#include <usDotx/us_usx_data_struct.h>
#include <usUtil/us_common_file.h>
#include <vector>

namespace uniscope_globe
{
	class USDOTX_EXPORT  usx_material
	{
	public:
		usx_material(void);

		~usx_material(void);

		typedef std::vector<usx_texture_file> texture_file_array;

	public:
		usx_color_value	m_diffuse_color_rgba;
		usx_color_value m_ambient_color_rgb;
		usx_color_value	m_specular_color_rgb;
		usx_color_value	m_emissive_color_rgb;
		float			m_power;

		texture_file_array m_texture_files;

		ustring m_name;
	};
}

#endif // _US_USX_MATERIAL_H_