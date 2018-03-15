///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_pen_style.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : pen_style class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_PEN_STYLE_H_
#define _US_PEN_STYLE_H_

#include <usCore\Export.h>
#include <usCore\us_style_base.h>
#include <usUtil\us_common_file.h>

namespace uniscope_globe
{
	class USCORE_EXPORT pen_style : public style_base
	{
	public:
		pen_style(void);
		pen_style(const pen_style& v_style);
		virtual ~pen_style(void);

	public:
		float m_pen_width;
		ulong m_pen_color;
	};


	class USCORE_EXPORT simple_line_style : public style_base
	{
	public:
		simple_line_style(void);
		simple_line_style(const simple_line_style& v_style);
		virtual ~simple_line_style(void);

	public:
		float m_line_width;
		ulong m_line_color;
		ulong  m_line_width_type;
	};
}

#endif // _US_PEN_STYLE_H_ 