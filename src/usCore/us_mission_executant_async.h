///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_mission_executant_async.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : mission_executant_async class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_MISSION_EXECUTANT_ASYNCHRONOUS_H_
#define _US_MISSION_EXECUTANT_ASYNCHRONOUS_H_

#include <usCore\Export.h>
#include <usCore\us_thread_base.h>
#include <usCore\us_mission_executant.h>

namespace uniscope_globe
{
	class mission_base;

	class USCORE_EXPORT mission_executant_async
		: public mission_executant
		, public thread_base
	{
	public:
		mission_executant_async(void);

		virtual ~mission_executant_async(void);

		// override from mission_executant
	public:
		virtual void receive_mission(mission_base* mission_value);

		virtual void terminate_mission(void);

	public:
		// override from thread_base
		virtual bool on_waiting(void);

		virtual void on_running(void);

	protected:
		mission_base* m_current_mission;
	};
}

#endif // _US_ASYNCHRONOUS_EXECUTANT_H_