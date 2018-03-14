///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_font_style.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : font_style class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_FONT_STYLE_H_
#define _US_FONT_STYLE_H_

#include <usCore\Export.h>
#include <usCore\us_style_base.h>

namespace uniscope_globe
{
	class USCORE_EXPORT font_style : public style_base
	{
	public:
		font_style(void);
		font_style(const font_style& v_style);
		virtual ~font_style(void);

	public:
		ustring m_face_name;
		ulong m_font_color;
		ulong m_font_back_color;
		long m_font_size;
		bool m_bold;
		bool m_italic;
		float m_font_scale;
	};
}

#endif // _US_FONT_STYLE_H_ 