///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_font_style.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : font_style class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_font_style.h"

namespace uniscope_globe
{
	font_style::font_style( void )
	{
		m_rtti = US_RTTI_FONT_STYLE;
		m_face_name = L"Arial";
		m_font_color = 0xffffffff;
		m_font_back_color = 0x00ffffff;
		m_font_scale = 1.0;

		m_font_size = 14;
		m_bold = false;
		m_italic = false;
	}

	font_style::font_style( const font_style& v_style )
	{
		m_rtti = US_RTTI_FONT_STYLE;
		m_face_name = v_style.m_face_name;
		m_font_color = v_style.m_font_color;
		m_font_back_color = v_style.m_font_back_color;
		m_font_scale = v_style.m_font_scale;

		m_font_size = v_style.m_font_size;
		m_bold = v_style.m_bold;
		m_italic = v_style.m_italic;
	}

	font_style::~font_style( void )
	{

	}

	
}