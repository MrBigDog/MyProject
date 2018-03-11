///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_misssion_base.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : mission_base class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_MISSSION_BASE_H_
#define _US_MISSSION_BASE_H_
#include <usCore\Export.h>
namespace uniscope_globe
{
	class USCORE_EXPORT mission_base : public heapable<uint>
	{
	public:
		mission_base(void);

		virtual ~mission_base(void);

	public:
		virtual bool update(void) = 0;

		virtual void execute(void) = 0;

		virtual void reset(void) = 0;

		virtual void startup(void) = 0;

		virtual void terminate(void) = 0;

	public:
		uint m_priority;

		bool m_finished;
	};

	bool USCORE_EXPORT compare_mission_priority(mission_base* mb1, mission_base* mb2);
}

#endif // _US_THREAD_MISSSION_H_