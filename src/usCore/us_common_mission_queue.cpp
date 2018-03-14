///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_common_mission_queue.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : common_mission_queue class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_common_mission_queue.h"
#include <usCore/us_mission_base.h>
#include <usUtil/us_common_file.h>
#include <usUtil/us_mutex.h>

namespace uniscope_globe
{
	common_mission_queue* singleton_common_mission_queue::s_ins = NULL;

	common_mission_queue::common_mission_queue(void)
		: mission_queue(1)
	{

	}

	common_mission_queue::~common_mission_queue(void)
	{

	}

	void common_mission_queue::remove_mission(mission_base* v_mission)
	{
		US_LOCK_AUTO_MUTEX

			m_mission_array.push_back(v_mission);
	}

	void common_mission_queue::clean(void)
	{
		US_LOCK_AUTO_MUTEX

			for (int i = 0; i < (int)m_mission_array.size(); i++)
			{
				AUTO_DELETE(m_mission_array[i]);
			}
		m_mission_array.clear();
	}


}