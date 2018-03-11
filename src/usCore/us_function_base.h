//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_function_base.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : angle template
//	Reference : http://www.geometrictools.com ( Wild Magic Library )
//				http://www.ogre3d.org/		  ( Ogre Object-Oriented Graphics Rendering Engine )
//				http://mathworld.wolfram.com/Matrix.html
//
//////////////////////////////////////////////////////////////////////////
#ifndef _US_FUNCTION_BASE_H_
#define _US_FUNCTION_BASE_H_
//#include <usCore\Export.h>
namespace uniscope_globe
{
	// parameter none
	class function_base
	{
	public:
		function_base(void){}
		virtual ~function_base(void){}

	public:
		virtual void operator()( event_argument* v_event ) = 0;

		virtual bool operator == ( const function_base& fun ) = 0;

		virtual bool equal( function_base* v_fun ) = 0;

		virtual function_base* clone() = 0;
	};

	// parameter one
	class function_parameter_one
	{
	public:
		function_parameter_one(void){}
		virtual ~function_parameter_one(void){}

	public:
		virtual ulong operator()( ulong v_param ) = 0;

		virtual bool operator == ( const function_parameter_one& fun ) = 0;

		virtual function_parameter_one* clone() = 0;
	};

}

#endif // #define _US_ANGLE_H_
