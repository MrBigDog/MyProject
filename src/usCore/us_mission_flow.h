///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_mission_flow.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : mission_flow class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_MISSION_FLOW_H_
#define _US_MISSION_FLOW_H_

#include <usCore\Export.h>
#include <usCore\us_mission_base.h>

#include <vector>

namespace uniscope_globe
{
	class USCORE_EXPORT mission_flow : public mission_base
	{
	public:
		mission_flow(void);

		virtual ~mission_flow(void);

		typedef std::vector<mission_base*> mission_array;

		// override from mission_base
	public:
		virtual bool update(void);

		virtual void execute(void);

		virtual void reset(void);

		virtual void startup(void) { }

		virtual void terminate(void) { }

		// own
	public:
		void add_mission(mission_base* v_mission);

		bool insert_mission(int v_id, mission_base* v_mission);

		mission_base* remove_mission(int v_id);

	protected:
		mission_array m_mission_array;
		int m_cursor;
	};
}

#endif // _US_MISSION_FLOW_H_