///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_clamp_base.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : clamp_base class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_CLAMP_BASE_H_
#define _US_CLAMP_BASE_H_
#include <usCore\Export.h>
namespace uniscope_globe
{
	class USCORE_EXPORT clamp_base
	{
	public:
		clamp_base( void );

		virtual ~clamp_base( void );

	public:
		virtual void clamp_to_ground( void );

	public:
		double get_clamp_factor( void ) { return m_clamp_factor; }

		vector3<double> get_clamp_pivot_sphr( void ) { return m_clamp_pivot_sphr; }

	protected:
		vector3<double> m_clamp_pivot_sphr;

		double m_clamp_factor;
	};
}

#endif // _US_CLAMP_BASE_H_