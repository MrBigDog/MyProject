///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_mission_queue.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : mission_queue class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "Stdafx.h"
#include "us_mission_queue.h"
#include <usUtil/us_common_file.h>
#include <usCore/us_mission_base.h>
#include <usCore/us_mission_executant_async.h>

namespace uniscope_globe
{
	mission_queue::mission_queue(void)
	{
	}

	mission_queue::mission_queue(int executant_num)
	{
		m_executant_array.resize(executant_num);
		m_executant_deque.resize(executant_num);
		for (int i = 0; i < executant_num; i++)
		{
			mission_executant_async* v_executant = new mission_executant_async;
			v_executant->set_parent(this);
			m_executant_array[i] = v_executant;
			m_executant_deque[i] = v_executant;
		}
	}

	mission_queue::~mission_queue(void)
	{
		for (int i = 0; i < (int)m_executant_array.size(); i++)
		{
			AUTO_DELETE(m_executant_array[i]);
		}
		m_executant_array.clear();
		m_executant_deque.clear();
	}

	void mission_queue::receive_mission(mission_base* mission)
	{
		US_LOCK_AUTO_MUTEX

			m_missions.push_back(mission);

		distribute_mission();
	}


	void mission_queue::terminate_mission(void)
	{
		//US_LOCK_AUTO_MUTEX

		mission_list::iterator itr = m_missions.begin();
		for (; itr != m_missions.end(); itr++)
		{
			(*itr)->reset();
		}
		m_missions.clear();

		m_executant_deque.clear();
		for (int i = 0; i < (int)m_executant_array.size(); i++)
		{
			m_executant_array[i]->terminate_mission();
			//m_executant_deque.push_back( m_executant_array[i] );
		}
	}

	void mission_queue::on_mission_complete(mission_executant* executant)
	{
		US_LOCK_AUTO_MUTEX

			// no mission
			m_executant_deque.push_back(executant);

		// sleep
		//::Sleep( 100 );

		distribute_mission();

	}

	void mission_queue::distribute_mission(void)
	{
		if (m_missions.size() == 0)
			return;

		if (m_executant_deque.size() == 0)
			return;

		// get executant
		mission_executant* v_leisure_executant = m_executant_deque.front();
		m_executant_deque.pop_front();

		// get mission
		mission_base* v_mission = (mission_base*)m_missions.front();
		m_missions.pop_front();

		v_mission->startup();
		v_leisure_executant->receive_mission(v_mission);
	}

	void mission_queue::push_back_buffer(mission_base* v_mission)
	{
		US_LOCK_AUTO_MUTEX

			m_missions.push_back(v_mission);

		distribute_mission();
	}
}