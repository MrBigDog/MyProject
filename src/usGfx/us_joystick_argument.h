///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_joystick_argument.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : joystick_argument class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_JOYSTICK_ARGUMENT_H_
#define _US_JOYSTICK_ARGUMENT_H_
#include <usGfx/Export.h>
#include <usUtil/us_common_file.h>

#include <windows.h>

namespace uniscope_globe
{
	//struct USGFX_EXPORT joystick_state
	//{
	//	int		m_axis_x;
	//	int		m_axis_y;
	//	int		m_axis_z;
	//	int		m_axis_x_rotate;
	//	int		m_axis_y_rotate;
	//	int		m_axis_z_rotate;
	//	byte    button[8];
	//};

	struct USGFX_EXPORT joystick_state
	{
		LONG    lX;                     /* x-axis position              */
		LONG    lY;                     /* y-axis position              */
		LONG    lZ;                     /* z-axis position              */
		LONG    lRx;                    /* x-axis rotation              */
		LONG    lRy;                    /* y-axis rotation              */
		LONG    lRz;                    /* z-axis rotation              */
		LONG    rglSlider[2];           /* extra axes positions         */
		DWORD   rgdwPOV[4];             /* POV directions               */
		BYTE    rgbButtons[128];        /* 128 buttons                  */
		LONG    lVX;                    /* x-axis velocity              */
		LONG    lVY;                    /* y-axis velocity              */
		LONG    lVZ;                    /* z-axis velocity              */
		LONG    lVRx;                   /* x-axis angular velocity      */
		LONG    lVRy;                   /* y-axis angular velocity      */
		LONG    lVRz;                   /* z-axis angular velocity      */
		LONG    rglVSlider[2];          /* extra axes velocities        */
		LONG    lAX;                    /* x-axis acceleration          */
		LONG    lAY;                    /* y-axis acceleration          */
		LONG    lAZ;                    /* z-axis acceleration          */
		LONG    lARx;                   /* x-axis angular acceleration  */
		LONG    lARy;                   /* y-axis angular acceleration  */
		LONG    lARz;                   /* z-axis angular acceleration  */
		LONG    rglASlider[2];          /* extra axes accelerations     */
		LONG    lFX;                    /* x-axis force                 */
		LONG    lFY;                    /* y-axis force                 */
		LONG    lFZ;                    /* z-axis force                 */
		LONG    lFRx;                   /* x-axis torque                */
		LONG    lFRy;                   /* y-axis torque                */
		LONG    lFRz;                   /* z-axis torque                */
		LONG    rglFSlider[2];          /* extra axes forces            */
	};

	//class joystick_argument
	//	: public input_argument
	//{
	//public:
	//	joystick_argument( void );

	//	virtual ~joystick_argument( void );

	//public:
	//	virtual void reset(void);

	//public:
	//	//joystick_state m_joystick_state;

	//	continuous_device_state m_state;

	//	//ustring		   m_vid;

	//	//ustring		   m_instance_id;



	//};
}

#endif // _US_JOYSTICK_ARGUMENT_H_