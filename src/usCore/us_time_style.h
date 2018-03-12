///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_time_style.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : time_style class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_TIME_STYLE_H_
#define _US_TIME_STYLE_H_
#include <usCore\Export.h>
namespace uniscope_globe
{
	class USCORE_EXPORT time_style
	{
	public:
		time_style( void );

		time_style( time_style& st );

		~time_style( void );

	public:
		void start( double v_start );

		void set_parameter( double v_duration, double v_interval );

		bool valid_cursor( double v_cur_time );

	public:
		double m_start_time;
		double m_duration;
		double m_interval;
		ulong m_cursor;
	};
}

#endif // _US_TIME_STYLE_H_