///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_mission_heap.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : mission_heap class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_MISSION_HEAP_H_
#define _US_MISSION_HEAP_H_

#include <usCore\Export.h>
#include <usCore\us_mission_executant.h>
#include <usUtil\us_mutex.h>
#include <usUtil\us_heap.h>
#include <usUtil\us_common_file.h>

#include <vector>
#include <deque>

namespace uniscope_globe
{
	class USCORE_EXPORT mission_heap : public mission_executant
	{
	protected:
		mission_heap(void);

	public:
		mission_heap(int executant_num, int max_heap_size);

		virtual ~mission_heap(void);

		friend class mission_base;

		typedef std::vector<mission_executant*> mission_executant_array;
		typedef std::deque<mission_executant*> mission_executant_deque;

		typedef std::vector<mission_base*> download_mission_array;

		// override from mission_executant
	public:
		virtual void receive_mission(mission_base* mission_value);

		virtual void terminate_mission(void);

		virtual void update_mission(mission_base* mission);

		virtual int get_mission_count(void) { return m_missions.size(); }

	public:
		virtual void push_back_buffer(mission_base* v_mission);

		virtual void clear_back_buffer();

	protected:
		virtual void on_mission_complete(mission_executant* executant_value);

		// own
	protected:
		void distribute_mission(void);

		void attenuate_mission(void);

	protected:
		US_AUTO_MUTEX

			int m_max_heap_size;

		heap_base<uint>	m_missions;

		mission_executant_array m_executant_array;
		mission_executant_deque m_leisure_executeants;

		download_mission_array m_mission_array;

		time_value m_lastest_time;
	};

}

#endif // _US_REQUEST_HEAP_H_