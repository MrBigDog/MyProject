///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_filter_base.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : filter_base class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "Stdafx.h"
#include "us_filter_base.h"
#include <usCore/us_filter_manager.h>

namespace uniscope_globe
{
	filter_base::filter_base(void)
	{
		m_filter_manager = NULL;
	}

	filter_base::~filter_base(void)
	{

	}

	void filter_base::set_manager(filter_manager *manager)
	{
		m_filter_manager = manager;
	}


}