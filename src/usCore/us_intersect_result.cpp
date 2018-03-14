///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_intersect_result.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : intersect_result class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_intersect_result.h"

namespace uniscope_globe
{
	intersect_result::intersect_result( void )
	{
		m_distance = math<double>::s_max_real;

		m_object = NULL;

		m_inner_object = NULL;

		m_hit_count = 0;

		m_inner_mesh = NULL;

		m_draw_order = 0;

		m_parent_layer = NULL;
	}

	intersect_result::~intersect_result( void )
	{

	}

	
}