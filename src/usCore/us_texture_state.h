///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_textue_state.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : texture_state class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_TEXTUE_STATE_H_
#define _US_TEXTUE_STATE_H_
#include <usCore\Export.h>
namespace uniscope_globe
{
#ifndef WIN64
	typedef unsigned long  texture_handle;
#else
	typedef ULONG_PTR  texture_handle;
#endif

	class render_device;

	class USCORE_EXPORT texture_state
	{
		struct texture_state_block
		{
			ulong stage_index;
			ulong state_type;
			ulong state_value;
		};

	private:
		texture_state(void);

	public:
		texture_state(render_device* device);

		virtual ~texture_state(void);

	public:
		void set_state(ulong stage_index, ulong state_type, ulong state_value);

	private:
		void restore_state(void);

	private:
		render_device* m_render_device;

		std::stack<texture_state_block> m_state_stack;

	};
}

#endif // _US_TEXTUE_STATE_H_
