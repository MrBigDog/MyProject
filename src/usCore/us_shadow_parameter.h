///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_shadow_parameter.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : shadow_parameter class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_SHADOW_PARAMETER_H_
#define _US_SHADOW_PARAMETER_H_
#include <usCore\Export.h>
#include <usUtil\us_common_file.h>
#include <usUtil\us_lon_lat_rect.h>

namespace uniscope_globe
{
	class USCORE_EXPORT shadow_parameter
	{
	public:
		shadow_parameter(void);

		shadow_parameter(double v_azimuth, double v_elevation);

		~shadow_parameter(void);

	public:
		lon_lat_rect m_rect;

		double		 m_elevation_angle;

		double		 m_azimuth_angle;

		bool		 m_enable;

		ulong		 m_stamp;

		vector4<double> m_light;
	};
}

#endif // _US_SHADOW_PARAMETER_H_ 