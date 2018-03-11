///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_sampler_state.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : sampler_state class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_SAMPLER_STATE_H_
#define _US_SAMPLER_STATE_H_
#include <usCore\Export.h>
namespace uniscope_globe
{
	class USCORE_EXPORT sampler_state
	{
	public:
		struct sampler_state_block
		{
			ulong stage_index;
			ulong state_type;
			ulong state_value;
		};

	private:
		sampler_state( void );

	public:
		sampler_state( render_device* device );

		virtual ~sampler_state( void );

	public:
		void set_state( ulong stage_index, ulong state_type, ulong state_value );

	private:
		void restore_state( void );

	private:
		render_device* m_render_device;

		std::stack<sampler_state_block> m_state_stack;
	};
}

#endif // _US_SAMPLER_STATE_H_