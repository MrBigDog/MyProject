///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_extended_material.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : material_set class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_extended_material.h"

namespace uniscope_globe
{
	material_set::material_set( void )
	{
		float m_power = 1.0;
	}

	material_set::~material_set( void )
	{
		m_texture_files.clear();
		m_materials.clear();
	}
	
}