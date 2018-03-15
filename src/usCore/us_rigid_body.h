///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_rigid_body.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : rigid_body class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_RIGID_BODY_H_
#define _US_RIGID_BODY_H_
#include <usCore\Export.h>
#include <usCore\us_event_argument.h>
#include <usCore\us_event_base.h>
#include <usUtil\us_vector3.h>

namespace uniscope_globe
{
	class USCORE_EXPORT rigid_body
	{
	public:
		rigid_body(void);

		virtual ~rigid_body(void);

	public:
		virtual void update_motion(time_value in_frame_time) {}

		virtual void set_velocity(vector3<float> in_velocity) { m_velocity = in_velocity; }

		virtual void set_rotation_velocity(float in_rot_velocity) { m_rotation_velocity = in_rot_velocity; }

		vector3<float> get_velocity(void) { return m_velocity; }

		float get_rotation_velocity(void) { return m_rotation_velocity; }

		void release_rigid_body(void);

	protected:
		vector3<float> m_velocity;

		float m_rotation_velocity;

	public:
		event_base m_on_rigid_body_delete;
	};

	class rigid_body_event_argument
		: public event_argument
	{
	public:
		rigid_body* m_this;
	};
}

#endif // _US_RIGID_BODY_H_