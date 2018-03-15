//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_event_handle.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : angle template
//	Reference : http://www.geometrictools.com ( Wild Magic Library )
//				http://www.ogre3d.org/		  ( Ogre Object-Oriented Graphics Rendering Engine )
//				http://mathworld.wolfram.com/Matrix.html
//
//////////////////////////////////////////////////////////////////////////
#ifndef _US_EVENT_HANDLE_H_
#define _US_EVENT_HANDLE_H_

#include <usCore\Export.h>
#include <usCore\us_function_base.h>
#include <usCore\us_event_argument.h>
#include <usCore\us_member_function.h>

namespace uniscope_globe
{
	class USCORE_EXPORT event_handle
	{
	protected:
		event_handle(void);

	public:
		event_handle(const event_handle& fun_value)
		{
			if (fun_value.callback_function)
			{
				callback_function = fun_value.callback_function->clone();
			}
		}

		virtual ~event_handle(void);

		// 普通函数
		//template <class fun>
		//event_handle( const fun& fun_value )
		//	: callback_function( new static_function<fun>(fun_value) )
		//{
		//}

		//// 仿函数
		//event_handle( const event_handle& fun_value )
		//	: callback_function( 0 )
		//{
		//	*this = fun_value;
		//}

		// 成员函数
		template <typename object_pointer, typename member_function_pointer>
		event_handle(const object_pointer& p_obj, const member_function_pointer& p_mem_fun)
		{
			callback_function = (new member_function<object_pointer, member_function_pointer>(p_obj, p_mem_fun));
		}

		event_handle& operator = (const event_handle& fun_value);

		void operator() (event_argument* e)
		{
			if (callback_function)
			{
				(*callback_function)(e);
			}
		}

		bool operator == (const event_handle& handler);

	public:
		void clear();

	protected:
		function_base* callback_function;
	};

}

#endif // #define _US_ANGLE_H_
