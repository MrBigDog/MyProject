///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_mission_base.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : mission_base class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "Stdafx.h"
#include "us_mission_base.h"

namespace uniscope_globe
{
	mission_base::mission_base( void )
	{
		m_priority = 0;
		m_finished = false;
	}

	mission_base::~mission_base( void )
	{

	}

	bool compare_mission_priority( mission_base* mb1, mission_base* mb2 )
	{
		return mb1->m_priority > mb2->m_priority;
	}
}