///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_pen_style.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : pen_style class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_pen_style.h"

namespace uniscope_globe
{
	pen_style::pen_style(void)
	{
		m_rtti = US_RTTI_PEN_STYLE;
		m_pen_width = 0.0f;
		m_pen_color = 0xffaaaaaa;
	}

	pen_style::pen_style(const pen_style& v_style)
	{
		m_rtti = US_RTTI_PEN_STYLE;
		m_pen_color = v_style.m_pen_color;
		m_pen_width = v_style.m_pen_width;
	}

	pen_style::~pen_style(void)
	{

	}


	simple_line_style::simple_line_style(void)
	{
		m_rtti = US_RTTI_SIMPLE_LINE_STYLE;
		m_line_width = 0.0f;
		m_line_color = 0xffaaaaaa;
		m_line_width_type = 0;
	}

	simple_line_style::simple_line_style(const simple_line_style& v_style)
	{
		m_rtti = US_RTTI_SIMPLE_LINE_STYLE;
		m_line_width = v_style.m_line_width;
		m_line_color = v_style.m_line_color;
		m_line_width_type = v_style.m_line_width_type;
	}

	simple_line_style::~simple_line_style(void)
	{

	}
}