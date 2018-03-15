//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_render_state.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : render_state class
//	Reference : 
//
//////////////////////////////////////////////////////////////////////////
#ifndef _US_RENDER_STATE_H_
#define _US_RENDER_STATE_H_
#include <usCore\Export.h>
#include <usUtil\us_common_file.h>

#include <stack>
namespace uniscope_globe
{
	class render_device;

	class USCORE_EXPORT render_state
	{
		struct render_state_block
		{
			ulong state_type;
			ulong state_value;
		};

	private:
		render_state(void) {};

	public:
		render_state(render_device* device);

		virtual ~render_state(void);

	public:
		void set_state(ulong state_type, ulong state_value);

		void restore_state(void);

	protected:
		render_device* m_render_device;

		std::stack<render_state_block> m_state_stack;

	};
}

#endif // _US_RENDER_STATE_H_
