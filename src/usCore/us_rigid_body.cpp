///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_rigid_body.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : rigid_body class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_rigid_body.h"

namespace uniscope_globe
{
	rigid_body::rigid_body( void )
	{
		m_rotation_velocity = 0.0;
	}

	rigid_body::~rigid_body( void )
	{
		rigid_body_event_argument args;
		args.m_this = this;
		m_on_rigid_body_delete( &args );
	}

	void rigid_body::release_rigid_body( void )
	{
		rigid_body_event_argument args;
		args.m_this = this;
		m_on_rigid_body_delete( &args );
		m_on_rigid_body_delete.clear();
	}
}