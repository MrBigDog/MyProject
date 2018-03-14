//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_event_handle.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : angle template
//	Reference : http://www.geometrictools.com ( Wild Magic Library )
//				http://www.ogre3d.org/		  ( Ogre Object-Oriented Graphics Rendering Engine )
//				http://mathworld.wolfram.com/Matrix.html
//
//////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_event_handle.h"
#include <usUtil/us_common_file.h>

namespace uniscope_globe
{

	event_handle::event_handle(void)
	{
		callback_function = NULL;
	}

	event_handle::~event_handle(void)
	{
		clear();
	}

	event_handle& event_handle::operator = (const event_handle& fun_value)
	{
		clear();

		if (fun_value.callback_function)
		{
			callback_function = fun_value.callback_function->clone();
		}

		return *this;
	}

	//void event_handle::operator() ( event_argument* e )
	//{
	//	if ( callback_function )
	//	{
	//		(*callback_function)( e );
	//	}
	//}

	bool event_handle::operator == (const event_handle& handler)
	{
		//if ( callback_function == NULL || handler.callback_function == NULL )
		//{
		//	return true;
		//}

		//if ( typeid(callback_function) == typeid(handler.callback_function) )
		//{
		//	return (*callback_function) == (*(handler.callback_function));
		//}

		//return false;

		if (callback_function == NULL || handler.callback_function == NULL)
		{
			return true;
		}

		if (typeid(callback_function) == typeid(handler.callback_function))
		{
			return callback_function->equal(handler.callback_function);
		}

		return false;
	}

	void event_handle::clear()
	{
		AUTO_DELETE(callback_function);
	}

}
