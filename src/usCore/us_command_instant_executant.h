///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_command_instant_executant.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : command_instant_executant class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_COMMAND_INSTANT_EXECUTANT_H_
#define _US_COMMAND_INSTANT_EXECUTANT_H_

#include <usCore\Export.h>
#include <usCore\us_command_base.h>
#include <usUtil\us_mutex.h>
#include <deque>

namespace uniscope_globe
{
	class USCORE_EXPORT command_instant_executant : public command_base
	{
	public:
		typedef std::deque<command_base*> command_deque;

	public:
		command_instant_executant(void);

		virtual ~command_instant_executant(void);

	public:
		virtual void push(command_base* cmd);

		virtual void clear(void);

		virtual bool execute(void);

		virtual bool reverse_execute(void);

	protected:
		command_deque m_command_deque;

		US_AUTO_MUTEX
	};
}

#endif // _US_COMMAND_INSTANT_EXECUTANT_H_