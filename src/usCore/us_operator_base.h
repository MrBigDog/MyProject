///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_operator_base.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : operator_base class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_OPERATOR_BASE_H_
#define _US_OPERATOR_BASE_H_
#include <usCore\Export.h>
namespace uniscope_globe
{
	class render_device;
	class operator_executant;

	class USCORE_EXPORT operator_base : public object_base
	{
	protected:
		operator_base( void );

	public:
		virtual ~operator_base( void );

	public:
		virtual void on_begin( time_value current_time );

		virtual void on_execute( time_value current_time );

		virtual	void on_draw( render_device* device );

		virtual void on_end( time_value current_time );

		virtual void unregister_operator( void );

	public:
		bool is_end( void ) { return m_is_end; }

		void finish_operator( void ) { m_is_end = true; }

		void set_enable( bool is_enable ) {	m_is_enable = is_enable; }

		bool get_enable( void ){ return m_is_enable; }

		void set_target_object( object_base* object );

		object_base* get_target_object( void );

	protected:
		operator_executant* m_executant;

		object_base* m_target_object;

		time_value	 m_begin_time;

		bool		 m_is_end;

		bool		 m_is_enable;

	public:
		event_base   m_finish_event;

		event_base   m_begin_event;
	};
}

#endif // _US_OPERATOR_BASE_H_