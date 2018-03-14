///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_event_sender.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : event_sender class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_EVENT_SENDER_H_
#define _US_EVENT_SENDER_H_

#include <usCore\Export.h>

namespace uniscope_globe
{
	class event_base;

	typedef void* event_sender_ptr;

	class USCORE_EXPORT event_sender
	{
	protected:
		event_sender(void);

	public:
		event_sender(event_sender_ptr v_sender, event_base* v_event);

		~event_sender(void);

	public:
		void terminate_event(void);

	protected:
		event_sender_ptr m_sender;

		event_base* m_event;
	};
}

#endif // _US_EVENT_SENDER_H_