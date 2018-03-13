///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_event_argument.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : event_argument class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_EVENT_ARGUMENT_H_
#define _US_EVENT_ARGUMENT_H_

#include <usCore\Export.h>

namespace uniscope_globe
{
	class USCORE_EXPORT event_argument
	{
	public:
		event_argument( void );

		~event_argument( void );
	
	public:
		int m_type;
		
		bool m_return_value;
	};
}

#endif // _US_EVENT_ARGUMENT_H_