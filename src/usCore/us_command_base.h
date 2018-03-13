///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_command_base.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : command_base class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_COMMAND_BASE_H_
#define _US_COMMAND_BASE_H_

#include <usCore\Export.h>

namespace uniscope_globe
{
	class USCORE_EXPORT command_base
	{
	public:
		command_base( void );

		virtual ~command_base( void );

	public:
		virtual bool execute( void );

		virtual bool reverse_execute( void );
	};
}

#endif // _US_COMMAND_BASE_H_