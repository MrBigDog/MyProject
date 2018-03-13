///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_command_instant_executant.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : command_instant_executant class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_command_instant_executant.h"
#include <usUtil/us_common_file.h>

namespace uniscope_globe
{
	command_instant_executant::command_instant_executant(void)
	{

	}

	command_instant_executant::~command_instant_executant(void)
	{
		clear();
	}

	void command_instant_executant::push(command_base* cmd)
	{
		US_LOCK_AUTO_MUTEX
			m_command_deque.push_back(cmd);
	}

	bool command_instant_executant::execute(void)
	{
		US_LOCK_AUTO_MUTEX

			bool ret_val = true;
		int n_size = m_command_deque.size();
		for (int ni = 0; ni < n_size; ni++)
		{
			command_base* v_command = m_command_deque.front();
			m_command_deque.pop_front();
			if (v_command->execute())
			{
				AUTO_DELETE(v_command);
			}
			else
			{
				m_command_deque.push_back(v_command);
				ret_val = false;
			}
		}
		return ret_val;
	}

	bool command_instant_executant::reverse_execute(void)
	{
		US_LOCK_AUTO_MUTEX
			//command_base* v_command = m_command_deque.front();
			//if( v_command->reverse_execute() )
			//{
			//	AUTO_DELETE( v_command );
			//	m_command_deque.pop_front();
			//	return true;
			//}
			return false;
	}

	void command_instant_executant::clear(void)
	{
		command_deque::iterator itr = m_command_deque.begin();
		for (; itr != m_command_deque.end(); itr++)
		{
			AUTO_DELETE(*itr);
		}
		m_command_deque.clear();
	}


}