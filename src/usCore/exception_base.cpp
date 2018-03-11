///////////////////////////////////////////////////////////
//  exception_base.cpp
//  Implementation of the Class exception_base
//  Created on:      09-Ê®Ò»ÔÂ-2015 11:26:52
//  Original author: felix
///////////////////////////////////////////////////////////
#include "stdafx.h"
#include "exception_base.h"
#include "SimpleSymbolEngine.h"
#include <sstream>
//#include <iostream>

namespace gw_exception
{
	exception_base::exception_base(const std::string& msg) throw()
		: msg_( msg )
		, current_file_("<unknown file>")
		, current_func_("<unknown func>")
		, current_line_(-1)
	{
		
	}

	exception_base::~exception_base() throw()
	{

	}


	void exception_base::init(const char* file, const char* func, int line)
	{
		current_file_ = file;
		current_func_ = func;
		current_line_ = line;
	}

	std::string exception_base::get_class_name() const 
	{

		return  "exception_base";
	}


	std::string exception_base::get_error_message() const
	{
		return  msg_;
	}


	std::string exception_base::get_stack_trace() const
	{
		CONTEXT context = {CONTEXT_FULL};
		::GetThreadContext( GetCurrentThread(), &context );
		/*_asm call $+5
		_asm pop eax
		_asm mov context.Eip, eax
		_asm mov eax, esp
		_asm mov context.Esp, eax
		_asm mov context.Ebp, ebp*/

		std::stringstream os("");
		SimpleSymbolEngine::instance().StackTrace( &context, os );
		return  os.str();
	}

	const std::string& exception_base::to_string() const
	{
		if ( what_.empty() )
		{
			std::stringstream sstr("");
			if (current_line_ > 0)
			{
				sstr << current_file_<<" ("<<current_line_<<") ";
			}
			sstr<<": "<<get_class_name();

			if ( !get_error_message().empty() )
			{
				sstr<<": "<<get_error_message();
			}
			sstr<<" THREAD["<<GetCurrentThreadId()<<"]";
			sstr<<"\nStack Trace:\n";
			sstr<<get_stack_trace();
			what_ = sstr.str();
		}
		return  what_;
	}


	const char* exception_base::what() const throw()
	{
		return  to_string().c_str();
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
