//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_member_function.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : angle template
//	Reference : http://www.geometrictools.com ( Wild Magic Library )
//				http://www.ogre3d.org/		  ( Ogre Object-Oriented Graphics Rendering Engine )
//				http://mathworld.wolfram.com/Matrix.html
//
//////////////////////////////////////////////////////////////////////////
#ifndef _US_MEMBER_FUNCTION_H_
#define _US_MEMBER_FUNCTION_H_

#include <usCore\Export.h>

namespace uniscope_globe
{
	// parameter none
	template <typename object_pointer, typename member_function_pointer>
	class member_function : public function_base
	{
	private:
		member_function(void) { }

		typedef member_function<object_pointer, member_function_pointer> this_type;

	public:
		member_function(const object_pointer& obj_ptr, member_function_pointer mem_fun_ptr)
			: object_ptr(obj_ptr)
			, member_function_ptr(mem_fun_ptr)
		{
		}

		virtual ~member_function(void) { }

	public:
		virtual void operator()(event_argument* param)
		{
			((*object_ptr).*member_function_ptr)(param);
		}

		//virtual bool operator == ( const function_base& fun )
		//{
		//	if ( typeid(this) == typeid( fun ) )
		//	{
		//		const this_type& fun_value = dynamic_cast<const this_type& >( fun );

		//		return ( object_ptr == fun_value.object_ptr && member_function_ptr == fun_value.member_function_ptr );
		//	}

		//	return false;
		//}

		virtual bool operator == (const function_base& fun)
		{
			const this_type& fun_value = dynamic_cast<const this_type&>(fun);
			if (&fun_value)
			{
				return (object_ptr == fun_value.object_ptr && member_function_ptr == fun_value.member_function_ptr);
			}

			return false;
		}

		virtual bool equal(function_base* v_fun)
		{
			//this_type* fun_value = dynamic_cast<this_type* >( v_fun );
			member_function* fun_value = (member_function*)(v_fun);
			if (fun_value)
			{
				return (object_ptr == fun_value->object_ptr && member_function_ptr == fun_value->member_function_ptr);
			}

			return false;
		}

		virtual function_base* clone()
		{
			return new this_type(object_ptr, member_function_ptr);
		}

	protected:
		object_pointer object_ptr;
		member_function_pointer member_function_ptr;
	};


	// parameter one
	template <typename object_pointer, typename member_function_pointer>
	class member_function_one : public function_parameter_one
	{
	private:
		member_function_one(void) { }

		typedef member_function_one<object_pointer, member_function_pointer> this_type;

	public:
		member_function_one(const object_pointer& obj_ptr, member_function_pointer mem_fun_ptr)
			: object_ptr(obj_ptr)
			, member_function_ptr(mem_fun_ptr)
		{
		}

		virtual ~member_function_one(void) { }

	public:
		virtual ulong operator()(ulong param)
		{
			return ((*object_ptr).*member_function_ptr)(param);
		}

		virtual bool operator == (const function_parameter_one& fun)
		{
			if (typeid(this) == typeid(fun))
			{
				const this_type& fun_value = dynamic_cast<const this_type&>(fun);

				return (object_ptr == fun_value.object_ptr && member_function_ptr == fun_value.member_function_ptr);
			}

			return false;
		}

		virtual function_parameter_one* clone()
		{
			return new this_type(object_ptr, member_function_ptr);
		}

	protected:
		object_pointer object_ptr;
		member_function_pointer member_function_ptr;
	};

}


#endif
