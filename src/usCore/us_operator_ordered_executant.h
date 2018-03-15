///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_operator_ordered_executant.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : operator_ordered_executant class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_OPERATOR_ORDERED_EXECUTANT_H_
#define _US_OPERATOR_ORDERED_EXECUTANT_H_

#include <usCore\Export.h>
#include <usCore\us_operator_base.h>

#include <deque>

namespace uniscope_globe
{
	class event_base;

	class USCORE_EXPORT operator_ordered_executant : public operator_base
	{
	public:
		typedef std::deque<operator_base*> operator_queue;

	protected:
		operator_ordered_executant(void);

	public:
		virtual ~operator_ordered_executant(void);

		// member function
	public:
		virtual int execute_operator(time_value v_current_time);

		virtual void start_operator(time_value v_current_time);

		virtual void register_operator(operator_base* in_operator);

		virtual void unregister_operator(operator_base* in_operator);

		virtual void clear_operator(void);

	public:
		void operator += (operator_base* in_operator);

		void operator -= (operator_base* in_operator);

		void operator () (time_value v_current_time);

	public:
		operator_queue	m_operators;

		time_value	    m_last_time;

		event_base		m_operator_event;
	};
}

#endif // _US_OPERATOR_ORDERED_EXECUTANT_H_