///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_mission_queue.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : mission_queue class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_MISSION_QUEUE_H_
#define _US_MISSION_QUEUE_H_

#include <usCore\Export.h>
#include <usCore\us_mission_executant.h>

#include <usUtil/us_mutex.h>
#include <deque>

namespace uniscope_globe
{
	class USCORE_EXPORT mission_queue : public mission_executant
	{
	protected:
		mission_queue(void);

	public:
		mission_queue(int executant_num);

		virtual ~mission_queue(void);

		typedef std::list<mission_base*> mission_list;

		typedef std::vector<mission_executant*> mission_executant_array;
		typedef std::deque<mission_executant*> mission_executant_deque;

		// override from mission_executant
	public:
		virtual void receive_mission(mission_base* mission_value);

		virtual void terminate_mission(void);

		virtual int get_mission_count(void) { return (int)m_missions.size(); }

	public:
		virtual void push_back_buffer(mission_base* v_mission);

	protected:
		virtual void on_mission_complete(mission_executant* executant_value);

		// own
	protected:
		void distribute_mission(void);

	protected:
		US_AUTO_MUTEX

			mission_list	m_missions;

		mission_executant_array m_executant_array;
		mission_executant_deque m_executant_deque;
	};

}

#endif // _US_REQUEST_QUEUE_H_