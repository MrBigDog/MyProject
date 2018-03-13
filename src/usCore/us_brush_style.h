///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_brush_style.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : brush_style class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_BRUSH_STYLE_H_
#define _US_BRUSH_STYLE_H_

#include <usCore\Export.h>
#include <usCore\us_style_base.h>
#include <usUtil\us_common_file.h>

namespace uniscope_globe
{
	class USCORE_EXPORT brush_style : public style_base
	{
	public:
		brush_style(void);
		brush_style(const brush_style& v_style);
		virtual ~brush_style(void);

	public:
		ulong m_line_width;
		ulong m_line_color;
		ulong m_fill_color;
	};

	class USCORE_EXPORT simple_polygon_style : public style_base
	{
	public:
		simple_polygon_style(void);
		simple_polygon_style(const simple_polygon_style& v_style);
		virtual ~simple_polygon_style(void);

	public:
		float m_line_width;
		ulong m_line_color;
		ulong m_line_width_type;
		ulong m_fill_color;
	};
}

#endif // _US_BRUSH_STYLE_H_ 