///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_icon_style.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : icon_style class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "Stdafx.h"
#include "us_icon_style.h"
#include <usCore/us_downloader.h>
#include <usUtil/us_common_file.h>
#include <usCore/us_hardware_texture.h>
#include <usCore/us_common_manager_group.h>
#include <WinUser.h>

namespace uniscope_globe
{
	icon_style::icon_style(void)
	{
		m_rtti = US_RTTI_ICON_STYLE;
		m_blend_color = 0xffffff55;
		m_icon_scale = 1.0;
		m_texture = NULL;
		m_str_texture_url = /*L*/"";
		m_text_format = DT_LEFT | DT_VCENTER | DT_NOCLIP;
	}

	icon_style::icon_style(const icon_style& v_style)
	{
		m_rtti = US_RTTI_ICON_STYLE;
		m_blend_color = v_style.m_blend_color;
		m_icon_scale = v_style.m_icon_scale;
		m_src_rect = v_style.m_src_rect;
		m_str_texture_url = v_style.m_str_texture_url;
		m_text_format = v_style.m_text_format;
		m_texture = NULL;
	}

	icon_style::~icon_style(void)
	{
		AUTO_RELEASE_SHARED_DATA(m_texture);
	}

	void icon_style::set_icon(cpstr str)
	{
		AUTO_RELEASE_SHARED_DATA(m_texture);
		m_str_texture_url = str;
	}

	hardware_texture* icon_style::get_texture(void)
	{
		if (m_str_texture_url.length() == 0) return NULL;
		if (m_texture == NULL)
		{
			m_texture = (texture_resource_container*)singleton_common_manager_group::instance().get_texture_manager(/*L*/"icon")->get_resource(m_str_texture_url);
			if (m_texture == NULL)
			{
				m_texture = new texture_resource_container(singleton_common_manager_group::instance().get_document(), m_str_texture_url, m_str_texture_url.c_str(), US_DOWNLOAD_IN_HEAP);
				m_texture->add_ref();
				singleton_common_manager_group::instance().get_texture_manager(/*L*/"icon")->add_resource(m_str_texture_url, m_texture);
			}
		}

		return (hardware_texture*)m_texture->get_resource();
	}

}