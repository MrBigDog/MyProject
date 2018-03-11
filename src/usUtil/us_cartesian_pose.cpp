///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_cartesian_pose.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : cartesian_pose class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////

#include "us_cartesian_pose.h"

namespace uniscope_globe
{
	cartesian_pose::cartesian_pose( void )
	{

	}

	cartesian_pose::~cartesian_pose( void )
	{

	}

	cartesian_pose::cartesian_pose( vector3<double>& in_dir, vector3<double>& in_up )
	{
		create_from_vector(in_dir, in_up);
	}

	cartesian_pose::cartesian_pose( double heading, double tilt, double roll )
	{
		create_from_eular(heading, tilt, roll);
	}

	cartesian_pose::cartesian_pose( quaternion<double>& in_orientation )
	{
		create_from_quaternion( in_orientation );
	}

	void cartesian_pose::create_from_vector( vector3<double>& in_dir, vector3<double>& in_up )
	{

	}

	void cartesian_pose::create_from_eular( double heading, double tilt, double roll )
	{

	}

	void cartesian_pose::create_from_quaternion( quaternion<double>& in_orientation )
	{

	}
	
}