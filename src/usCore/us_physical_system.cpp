///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_physical_system.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : physical_system class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_physical_system.h"

namespace uniscope_globe
{
	physical_system::physical_system( void )
	{

	}

	physical_system::~physical_system( void )
	{
		m_rigid_body_map.clear();
	}

	void physical_system::update( time_value in_frame_time )
	{
		US_LOCK_AUTO_MUTEX
		rigid_body_map::iterator itr = m_rigid_body_map.begin();
		for ( ; itr != m_rigid_body_map.end(); itr++ )
		{
			itr->second->update_motion( in_frame_time );
		}
	}

	bool physical_system::add_body( cpstr name, object_base* in_object )
	{
		US_LOCK_AUTO_MUTEX
		rigid_body_map::iterator itr = m_rigid_body_map.find( name );
		if( itr != m_rigid_body_map.end() )
		{
			return false;
		}

		rigid_body* body = dynamic_cast<rigid_body*>( in_object );
		if( body == NULL ) return false;

		m_rigid_body_map.insert( make_pair( name, body ) );

		body->m_on_rigid_body_delete += event_handle( this, &physical_system::on_rigid_body_delete );

		return true;
	}

	void physical_system::remove_body( cpstr name )
	{
		US_LOCK_AUTO_MUTEX

		rigid_body_map::iterator itr = m_rigid_body_map.find( name );
		if( itr != m_rigid_body_map.end() )
		{
			m_rigid_body_map.erase( itr );
		}
	}

	void physical_system::remove_body( rigid_body* in_object )
	{
		US_LOCK_AUTO_MUTEX

		rigid_body_map::iterator itr = m_rigid_body_map.begin();
		for ( ; itr != m_rigid_body_map.end(); itr++ )
		{
			if( itr->second == in_object )
			{
				m_rigid_body_map.erase( itr );
				return;
			}
		}
	}

	void physical_system::on_rigid_body_delete( event_argument* args )
	{
		rigid_body_event_argument* rigid_args = (rigid_body_event_argument*)args;

		this->remove_body( rigid_args->m_this );
	}
}