
///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_texture_fill_style.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : texture_fill_style class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_TEXTURE_FILL_STYLE_H_
#define _US_TEXTURE_FILL_STYLE_H_

#include <usCore\Export.h>
#include <usCore\us_style_base.h>
#include <wtypes.h>

namespace uniscope_globe
{
	class USCORE_EXPORT texture_fill_style : public style_base
	{
	public:
		texture_fill_style(void);
		texture_fill_style(const texture_fill_style& v_style);
		virtual ~texture_fill_style(void);

	public:
		//void set_icon( cpstr str );

	public:
		LONG	m_fill_color;
		LONG    m_blend_color;
		float	m_tu_start;
		float	m_tv_start;
		float	m_tu_end;		// max: 8192
		float	m_tv_end;
		ustring m_texture_link;
		float	m_texture_rotation;

		bool    m_flip_texture_y;
		bool    m_two_sided_mode;
	};
}

#endif // _US_TEXTURE_FILL_STYLE_H_ 