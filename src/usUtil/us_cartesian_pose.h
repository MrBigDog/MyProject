///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_cartesian_pose.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : cartesian_pose class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_CARTESIAN_POSE_H_
#define _US_CARTESIAN_POSE_H_

#include <usUtil\Export.h>
#include <usUtil\us_vector3.h>
#include <usUtil\us_quaternion.h>

namespace uniscope_globe
{
	class USUTIL_EXPORT cartesian_pose
	{
	public:
		cartesian_pose(void);

		cartesian_pose(vector3<double>& in_dir, vector3<double>& in_up);

		cartesian_pose(double heading, double tilt, double roll);

		cartesian_pose(quaternion<double>& in_orientation);

		virtual ~cartesian_pose(void);

	public:
		void create_from_vector(vector3<double>& in_dir, vector3<double>& in_up);

		void create_from_eular(double heading, double tilt, double roll);

		void create_from_quaternion(quaternion<double>& in_orientation);

	public:
		quaternion<double> m_orientation;

		double m_heading;

		double m_tilt;

		double m_roll;

		vector3<double> m_direction;

		vector3<double> m_up;
	};
}

#endif // _US_CARTESIAN_POSE_H_