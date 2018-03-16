///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_input_device_base.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : input_device_base class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_INPUT_DEVICE_BASE_H_
#define _US_INPUT_DEVICE_BASE_H_
#include <usGfx/Export.h>
#include <usGfx/us_input_argument.h>
#include <usUtil/us_common_file.h>
namespace uniscope_globe
{
	class input_impl;

	class input_device_state
	{
	public:
		double x_move;
		double y_move;
		double z_move;

		double x_rot;
		double y_rot;
		double z_rot;

		byte keys[256];

		byte stick_buttons[128];

	public:
		input_device_state()
		{
			x_move = 0.0;
			y_move = 0.0;
			z_move = 0.0;

			x_rot = 0.0;
			y_rot = 0.0;
			z_rot = 0.0;

			memset((void*)keys, 0, 256);

			memset((void*)stick_buttons, 0, 128);
		}
	};

	class USGFX_EXPORT input_device_argument
		: public input_argument
	{
	public:
		input_device_argument(void);

		virtual ~input_device_argument(void);

	public:
		virtual void reset(void);

	public:
		input_device_state m_state;
	};

	class USGFX_EXPORT input_device_base
	{
	public:
		input_device_base(void);

		virtual ~input_device_base(void);

	public:
		virtual bool refresh(void) = 0;

	public:
		void set_enable(bool enable_value) { m_enable = enable_value; }

	protected:
		bool			m_enable;
	};
}

#endif // _US_INPUT_DEVICE_BASE_H_