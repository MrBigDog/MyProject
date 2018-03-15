///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_link_base.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : link_base class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "Stdafx.h"
#include "us_link_base.h"
#include <usCore/us_system_environment.h>


namespace uniscope_globe
{
	link_base::link_base(const LPCTSTR url, int type)
	{
		m_link_type = type;
		m_link_url = url;
		if (m_link_type == US_LINK_TYPE_ROOT)
		{
			m_link_url = singleton_system_environment::instance().m_root_path + m_link_url.substr(7, m_link_url.size() - 7);
		}
	}

	link_base::~link_base(void)
	{
	}

	int link_base::get_link_type(void)
	{
		return m_link_type;
	}

	ustring link_base::get_link_url(void)
	{
		return m_link_url;
	}

	void link_base::set_link_manager(link_manager* v_link_mngr)
	{
		m_link_mngr = v_link_mngr;
	}



}