///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_operator_executant.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : operator_executant class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_operator_executant.h"

namespace uniscope_globe
{
	operator_executant::operator_executant( void )
	{

	}

	operator_executant::~operator_executant( void )
	{
		clear_operator();
	}

	int operator_executant::execute_operator( time_value v_current_time )
	{
		if( m_operators.size() == 0 ) return 0;

		//time_value v_eclipse_time = v_current_time - m_last_time;
		//if ( v_eclipse_time < MIN_TIME_INTERVAL ) return;

		{
			operator_base* v_operator = m_operators.front();
			v_operator->on_execute( v_current_time );
			if( v_operator->is_end() )
			{
				v_operator->on_end( v_current_time );

				AUTO_DELETE(v_operator);
				m_operators.pop_front();
				if( m_operators.size() > 0 )
				{
					operator_base* v_next_operator = m_operators.front();
					v_next_operator->on_begin( v_current_time );
				}
			}
		}

		m_last_time = v_current_time;

		if( m_operators.size() == 0 )
		{
			return -1;
			//m_operator_event( NULL );
		}
		return (int)m_operators.size();
	}

	void operator_executant::start_operator( time_value v_current_time )
	{
		if( m_operators.size() > 0 )
		{
			operator_base* v_operator = m_operators.front();
			v_operator->on_begin( v_current_time );
		}
	}

	void operator_executant::operator () (  time_value v_current_time  )
	{
		execute_operator( v_current_time );
	}

	void operator_executant::operator += ( operator_base* in_operator )
	{
		register_operator( in_operator );
	}

	void operator_executant::operator -= ( operator_base* in_operator )
	{
		unregister_operator( in_operator );
	}

	void operator_executant::register_operator( operator_base* in_operator )
	{
		m_operators.push_back( in_operator );
	}

	void operator_executant::unregister_operator( operator_base* in_operator )
	{
		operator_queue::iterator itr = m_operators.begin();
		for ( ; itr != m_operators.end(); ++itr )
		{
			if ( *itr == in_operator )
			{
				m_operators.erase( itr );
				AUTO_DELETE(in_operator);
				break;
			}
		}
	}

	void operator_executant::clear_operator( void )
	{
		while( m_operators.size() > 0 )
		{
			operator_base* v_operator = m_operators.front();
			AUTO_DELETE(v_operator);
			m_operators.pop_front();
		}
	}

	
}