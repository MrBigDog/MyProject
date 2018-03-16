///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_mouse_argument.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : mouse_argument class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_MOUSE_ARGUMENT_H_
#define _US_MOUSE_ARGUMENT_H_

#include <usGfx/Export.h>
#include <usGfx/us_input_argument.h>
#include <usUtil/us_common_file.h>

namespace uniscope_globe
{
	// struct mouse_state
	struct USGFX_EXPORT mouse_state
	{
		long    l_x;
		long    l_y;
		long    l_z;
		byte    button[8];
	};

	// class mouse_argument
	class USGFX_EXPORT mouse_argument
		: public input_argument
	{
	public:
		mouse_argument(void);

		virtual ~mouse_argument(void);

	public:
		virtual void reset(void);

	public:
		mouse_state m_state;
	};
}

#endif // _US_MOUSE_ARGUMENT_H_