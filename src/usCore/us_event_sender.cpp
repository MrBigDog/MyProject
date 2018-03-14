///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_event_sender.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : event_sender class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "Stdafx.h"
#include "us_event_sender.h"
#include <usCore/us_event_base.h>

namespace uniscope_globe
{
	event_sender::event_sender(void)
	{

	}

	event_sender::event_sender(event_sender_ptr v_sender, event_base* v_event)
	{
		m_sender = v_sender;
		m_event = v_event;
	}

	event_sender::~event_sender(void)
	{

	}

	void event_sender::terminate_event(void)
	{

	}


}