///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_sound_style.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : sound_style class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_SOUND_STYLE_H_
#define _US_SOUND_STYLE_H_
#include <usCore\Export.h>
#include <usCore\us_style_base.h>
#include <wtypes.h>
namespace uniscope_globe
{
	class sound_base;

	class USCORE_EXPORT sound_style : public style_base
	{
	public:
		sound_style(void);

		sound_style(sound_style& v_style);

		sound_style(sound_style* v_style);

		virtual ~sound_style(void);

	public:
		void	set_repeat_mode(bool color);

		bool	get_repeat_mode(void);

		void	set_url(const LPCTSTR str_url);

		ustring get_url(void);

		sound_base* get_sound(void);

	protected:
		bool	m_repeat_mode;

		ustring	m_str_sound_url;

		sound_base*	m_sound;
	};
}

#endif // _US_SOUND_STYLE_H_