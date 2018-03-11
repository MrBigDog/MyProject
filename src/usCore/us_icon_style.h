///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_icon_style.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : icon_style class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_ICON_STYLE_H_
#define _US_ICON_STYLE_H_
#include <usCore\Export.h>
namespace uniscope_globe
{
	class USCORE_EXPORT icon_style : public style_base
	{
	public:
		icon_style( void );
		icon_style( const icon_style& v_style );
		virtual ~icon_style( void );

	public:
		void set_icon( cpstr str );

		hardware_texture* get_texture( void );

	public:
		dword	m_blend_color;
		double	m_icon_scale;
		rect	m_src_rect;
		ustring	m_str_texture_url;
		int		m_text_format;

	protected:
		texture_resource_container*	m_texture;
	};
}

#endif // _US_ICON_STYLE_H_