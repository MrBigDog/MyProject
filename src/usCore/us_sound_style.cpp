///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_sound_style.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : sound_style class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_sound_style.h"
#include <usCore/us_system_environment.h>
#include <usCore/us_sound_base.h>
namespace uniscope_globe
{
	sound_style::sound_style(void)
	{
		m_repeat_mode = true;

		m_str_sound_url = singleton_system_environment::instance().m_root_path + L"icon\\icon.png";

		m_sound = NULL;
	}

	sound_style::sound_style(sound_style& v_style)
	{
		m_repeat_mode = v_style.get_repeat_mode();

		m_str_sound_url = v_style.get_url();

		m_sound = NULL;
	}

	sound_style::sound_style(sound_style* v_style)
	{
		if (v_style == NULL) return;

		m_repeat_mode = v_style->get_repeat_mode();

		m_str_sound_url = v_style->get_url();

		m_sound = NULL;
	}

	sound_style::~sound_style(void)
	{
		AUTO_DELETE(m_sound);
	}

	void sound_style::set_repeat_mode(bool v_repeat_mode)
	{
		m_repeat_mode = v_repeat_mode;
	}

	bool sound_style::get_repeat_mode(void)
	{
		return m_repeat_mode;
	}

	void sound_style::set_url(const LPCTSTR str_url)
	{
		m_str_sound_url = str_url;
	}

	ustring sound_style::get_url(void)
	{
		return m_str_sound_url;
	}

	sound_base* sound_style::get_sound(void)
	{
		return m_sound;
	}
}