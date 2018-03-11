//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_event_base.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : angle template
//	Reference : http://www.geometrictools.com ( Wild Magic Library )
//				http://www.ogre3d.org/		  ( Ogre Object-Oriented Graphics Rendering Engine )
//				http://mathworld.wolfram.com/Matrix.html
//
//////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "us_event_base.h"

namespace uniscope_globe
{
	event_base::event_base(void)
	{
		m_event_type = US_EVENT_REQUEST;
	}

	event_base::~event_base(void)
	{
		//event_handle_array::iterator itr = handle_array.begin();
		//for ( ; itr != handle_array.end(); itr++ )
		//{
		//	(*itr).clear();
		//}
		handle_array.clear();
	}

	int event_base::get_event_type( void )
	{
		return m_event_type;
	}

	void event_base::operator () ( event_argument* event_args )
	{
		event_handle_array::iterator itr;
		
		for ( itr = handle_array.begin(); itr != handle_array.end(); ++itr )
		{
			(*itr)( (event_argument*)event_args );
		}
	}

	event_base& event_base::operator += (const event_handle& handler)
	{
		register_handle( handler) ;
		return *this;
	}

	event_base& event_base::operator -= (const event_handle& handler)
	{
		unregister_handle(handler);
		return *this;
	}

	void event_base::register_handle( const event_handle& handler )
	{
		event_handle_array::iterator itr;
		for ( itr = handle_array.begin(); itr != handle_array.end(); ++itr )
		{
			if ( *itr == handler )
			{				
				return;
			}
		}

		handle_array.push_back(handler);
	}

	void event_base::unregister_handle( const event_handle& handler )
	{
		event_handle_array::iterator itr;
		for ( itr = handle_array.begin(); itr != handle_array.end(); ++itr )
		{
			if ( *itr == handler )
			{
				handle_array.erase( itr );
				break;
			}
		}
	}

	void event_base::clear( void )
	{
		handle_array.clear();
	}
}
