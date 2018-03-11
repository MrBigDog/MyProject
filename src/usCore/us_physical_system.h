///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_physical_system.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : physical_system class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_PHYSICAL_SYSTEM_H_
#define _US_PHYSICAL_SYSTEM_H_
#include <usCore\Export.h>
namespace uniscope_globe
{
	class USCORE_EXPORT physical_system
	{
	public:
		typedef std::map< ustring, rigid_body* > rigid_body_map;

	public:
		physical_system(void);

		virtual ~physical_system(void);

	public:
		virtual void update(time_value in_frame_time);

		bool add_body(cpstr name, object_base* in_object);

		void remove_body(cpstr name);

		void remove_body(rigid_body* in_object);

		void on_rigid_body_delete(event_argument* args);

	protected:
		rigid_body_map m_rigid_body_map;

		US_AUTO_MUTEX
	};
}

#endif // _US_PHYSICAL_SYSTEM_H_