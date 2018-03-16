///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_message_argument.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : message_argument class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_MESSAGE_ARGUMENT_H_
#define _US_MESSAGE_ARGUMENT_H_

#include <usGfx/Export.h>
#include <usGfx/us_input_argument.h>
#include <usUtil/us_common_file.h>

namespace uniscope_globe
{
	class USGFX_EXPORT message_argument
		: public input_argument
	{
	public:
		message_argument(void);

		virtual ~message_argument(void);

	public:
		virtual void reset(void);

	public:
		uint   m_message;
		WPARAM m_w_param;
		LPARAM m_l_param;
		HHOOK  m_hook;
	};
}

#endif // _US_MESSAGE_ARGUMENT_H_