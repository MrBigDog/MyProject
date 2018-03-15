///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_input_argument.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : input_argument class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_INPUT_ARGUMENT_H_
#define _US_INPUT_ARGUMENT_H_

namespace uniscope_globe
{
	typedef enum
	{
		US_INPUT_NONE = 100,

		US_INPUT_DEVICE,

		US_INPUT_MOUSE,

		US_INPUT_WINMSG

	} input_argument_type;


	class LIB_GFX_API input_argument
		: public event_argument
	{
	public:
		input_argument( void );

		virtual ~input_argument( void );

	public:
		virtual void reset(void) = 0;

	public:
		HWND   m_hwnd;
	};
}

#endif // _US_INPUT_ARGUMENT_H_