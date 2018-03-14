///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_frame_counter.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : frame_counter class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_FRAME_COUNTER_H_
#define _US_FRAME_COUNTER_H_

#include <usCore\Export.h>
#include <usUtil\us_common_file.h>
#include <usCore\us_tls_singleton.h>
#include <winnt.h>

namespace uniscope_globe
{
	class USCORE_EXPORT frame_counter
	{
	public:
		frame_counter(void);

		virtual ~frame_counter(void);

		void update_frame_time(void);

		void update_logic_time(void);

	public:
		double     m_frame_per_sec;
		double     m_logic_per_sec;

		// frame
		time_value m_frame_time;
		time_value m_last_frame_tick_count;
		//time_value m_last_frame_elapsed_time;


		// logic
		LONGLONG m_update_count;
		time_value m_logic_time;
		time_value m_last_logic_tick_count;


	protected:
		time_value m_frame_time_tick;

		time_value m_logic_time_tick;

	public:
		double get_current_time(void);

		double get_elapsed_frame_time(void);

	protected:
		time_value m_last_frame_time;
		time_value m_last_logic_time;

		LONGLONG   m_ticks_per_sec;


		ulong	   m_update_frame_count;
		ulong	   m_update_logic_count;
	public:
		ulong	  m_frame_count;

	};

	class USCORE_EXPORT tls_singleton_frame_counter : public tls_singleton<frame_counter>
	{
	};
}

#endif // _US_FRAME_COUNTER_H_