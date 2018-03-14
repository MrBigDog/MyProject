///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_mission_executant.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : mission_executant class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_MISSION_EXECUTANT_H_
#define _US_MISSION_EXECUTANT_H_

#include <usCore\Export.h>

namespace uniscope_globe
{
	class USCORE_EXPORT mission_executant
	{
	protected:
		mission_executant( void );

	public:
		virtual ~mission_executant( void ){}
		
	public:
		bool is_busy() { return m_busy; }

		void set_parent( mission_executant* v_parent ) { m_parent = v_parent; }

		virtual int get_mission_count( void ) { return 0; }

	public:
		virtual void receive_mission( mission_base* mission_value ){}

		virtual void update_mission( mission_base* mission_value ){}

		virtual void terminate_mission( void ) { }

		virtual void on_mission_complete( mission_executant* executant_value ){}

	public:
		virtual void push_back_buffer( mission_base* v_mission ) { }

		virtual void clear_back_buffer() { }

	protected:
		bool m_busy;

		mission_executant* m_parent;
	};
}

#endif // _US_EXECUTANT_BASE_H_