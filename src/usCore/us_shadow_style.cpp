///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_shadow_style.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : shadow_style class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_shadow_style.h"

namespace uniscope_globe
{
	shadow_style::shadow_style( void )
	{
		m_rtti = US_RTTI_SHADOW_STYLE;
		m_shadow_type = US_SHADOW_NONE;
		m_shadow_color = 0xff000000;
	}

	shadow_style::shadow_style( const shadow_style& v_style )
	{
		m_rtti = US_RTTI_SHADOW_STYLE;
		m_shadow_type = v_style.m_shadow_type;
		m_shadow_color = v_style.m_shadow_color;
	}

	shadow_style::~shadow_style( void )
	{

	}

	
}