///////////////////////////////////////////////////////////
//  gw_exception_base.h
//  Implementation of the Class gw_exception_base
//  Created on:      10-十一月-2015 11:26:52
//  Original author: felix
///////////////////////////////////////////////////////////
#pragma once

#ifndef struct_exception_base_h__
#define struct_exception_base_h__
#include <usCore\Export.h>
#include "exception_base.h"
#include <windows.h>

namespace gw_exception
{
	///即是工具类又是异常类
	class USCORE_EXPORT struct_exception_base :public exception_base
	{
	private:
		PEXCEPTION_POINTERS exception_;
	public:
		struct_exception_base(PEXCEPTION_POINTERS exception);
		struct_exception_base(UINT code, PEXCEPTION_POINTERS exception);
		static void MapSHEtoCE();


		PEXCEPTION_POINTERS ExceptionPointers();

		DWORD ExceptionCode();

		PVOID ExceptionAddress();

	private:
		static void TranslateSEHtoCE(UINT code, PEXCEPTION_POINTERS pep);

	};

	///可恢复的结构化异常
	class USCORE_EXPORT gw_recover_struct_exception : public struct_exception_base
	{
	public:
		gw_recover_struct_exception(PEXCEPTION_POINTERS exception) :struct_exception_base(exception) {}
		gw_recover_struct_exception(UINT code, PEXCEPTION_POINTERS exception) :struct_exception_base(code, exception) {}
	};

	///不可恢复的结构化异常
	class USCORE_EXPORT gw_unrecover_struct_exception : public struct_exception_base
	{
	public:
		gw_unrecover_struct_exception(PEXCEPTION_POINTERS exception) :struct_exception_base(exception) {}
		gw_unrecover_struct_exception(UINT code, PEXCEPTION_POINTERS exception) :struct_exception_base(code, exception) {}
	};

}


#endif // struct_exception_base_h__
