///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_mission_flow.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : mission_flow class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_mission_flow.h"

namespace uniscope_globe
{
	mission_flow::mission_flow( void )
	{
		m_cursor = 0;
	}

	mission_flow::~mission_flow( void )
	{
		for ( int i = 0 ; i < (int)m_mission_array.size(); i++ )
		{
			AUTO_DELETE( m_mission_array[i] );
		}
		m_mission_array.clear();
	}

	bool mission_flow::update( void )
	{
		if ( m_cursor < m_mission_array.size() )
		{
			if ( m_mission_array[m_cursor]->update() )
			{
				m_cursor++;

				return true;
			}
		}

		return false;
	}

	void mission_flow::reset( void )
	{
		m_cursor = 0;
		for ( int i = 0 ; i < (int)m_mission_array.size(); i++ )
		{
			m_mission_array[i]->reset();
		}
	}

	void mission_flow::execute( void )
	{

	}

	void mission_flow::add_mission( mission_base* v_mission )
	{
		m_mission_array.push_back( v_mission );
	}

	bool mission_flow::insert_mission( int v_id, mission_base* v_mission )
	{
		if ( v_id < 0 || v_id > m_mission_array.size() )
			return false;

		m_mission_array.insert( m_mission_array.begin() + v_id, 1, v_mission );

		return true;
	}

	mission_base* mission_flow::remove_mission( int v_id )
	{
		if ( v_id < 0 || v_id >= m_mission_array.size() )
			return NULL;

		mission_base* v_mission = m_mission_array[v_id];
		m_mission_array.erase( m_mission_array.begin() + v_id );

		return v_mission;
	}
	
}