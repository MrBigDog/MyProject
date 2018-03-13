/////////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_brush_style.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : brush_style class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_brush_style.h"

namespace uniscope_globe
{
	brush_style::brush_style( void )
	{
		m_rtti = US_RTTI_BRUSH_STYLE;
		m_line_width = 1;
		m_line_color = 0xffaaaaaa;
		m_fill_color = 0xffffffff;
	}

	brush_style::brush_style( const brush_style& v_style )
	{
		m_rtti = US_RTTI_BRUSH_STYLE;
		m_line_width = v_style.m_line_width;
		m_line_color = v_style.m_line_color;
		m_fill_color = v_style.m_fill_color;
	}

	brush_style::~brush_style( void )
	{

	}


	simple_polygon_style::simple_polygon_style( void )
	{
		m_rtti = US_RTTI_SIMPLE_POLYGON_STYLE;
		m_line_width = 0.0f;
		m_line_color = 0xffaaaaaa;
		m_line_width_type = 1;
		m_fill_color = 0xffffffff;
	}

	simple_polygon_style::simple_polygon_style( const simple_polygon_style& v_style )
	{
		m_rtti = US_RTTI_SIMPLE_POLYGON_STYLE;
		m_line_width = v_style.m_line_width;
		m_line_color = v_style.m_line_color;
		m_line_width_type = v_style.m_line_width_type;
		m_fill_color = v_style.m_fill_color;
	}

	simple_polygon_style::~simple_polygon_style( void )
	{

	}

	
}