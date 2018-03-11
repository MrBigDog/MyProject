///////////////////////////////////////////////////////////
//  gw_exception_base.cpp
//  Implementation of the Class gw_exception_base
//  Created on:      10-十一月-2015 11:26:52
//  Original author: felix
///////////////////////////////////////////////////////////
#include "stdafx.h"
#include "struct_exception_base.h"

namespace gw_exception
{
	struct_exception_base::struct_exception_base(PEXCEPTION_POINTERS exception)
		:exception_base("")
		,exception_(exception)
	{

	}

	struct_exception_base::struct_exception_base(UINT code,PEXCEPTION_POINTERS exception)
		:exception_base("")
		,exception_(exception)
	{
		switch ( code ) {
		case EXCEPTION_ACCESS_VIOLATION:
			msg_.append( " 线程试图访问未分配或非法内存的异常!" );
			break;
		case EXCEPTION_DATATYPE_MISALIGNMENT:
			msg_.append( " 线程试图读或写不支持对齐的硬件上的未对齐的数据!" );
			break;
		case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
			msg_.append( " 线程试图存取一个越界的数组元素!" );
			break;
		case EXCEPTION_IN_PAGE_ERROR:
			msg_.append( " 由于文件系统或一个设备启动程序返回一个读错误，造成不能满足要求的页故障!" );
			break;
		case EXCEPTION_GUARD_PAGE:
			msg_.append( " 线程试图读取一个带有PAGE_GUARD保护属性的内存页!" );
			break;
		case EXCEPTION_ILLEGAL_INSTRUCTION:
			msg_.append( " 线程执行了一个无效的指令!" );
			break;
		case EXCEPTION_PRIV_INSTRUCTION:
			msg_.append( " 线程执行了一个当前机器模式不允许的指令!" );
			break;
		case EXCEPTION_INVALID_DISPOSITION:
			msg_.append( " 异常过滤器返回了错误的值!" );
			break;
		case EXCEPTION_NONCONTINUABLE_EXCEPTION:
			msg_.append( " 异常过滤器对一个不能继续的异常返回的EXCEPTION_CONTINUE_EXCEPTION!" );
			break;

		case EXCEPTION_INT_DIVIDE_BY_ZERO:
			msg_.append( " 整型数除零异常!" );
			break;
		case EXCEPTION_INT_OVERFLOW:
			msg_.append( " 一个整数操作的结果超过了整数值规定的范围!" );
			break;

		case EXCEPTION_FLT_DIVIDE_BY_ZERO:
			msg_.append( " 浮点数除零异常" );
			break;
		case EXCEPTION_FLT_DENORMAL_OPERAND:
			msg_.append( " 浮点操作中的一个操作数不正常!" );
			break;
		case EXCEPTION_FLT_INEXACT_RESULT:
			msg_.append( " 浮点操作的结构不能精确表示成十进制小数!" );
			break;
		case EXCEPTION_FLT_INVALID_OPERATION:
			msg_.append( " 表示任何没有在此列出的其它浮点数异常!" );
			break;
		case EXCEPTION_FLT_OVERFLOW:
			msg_.append( " 浮点操作的结构超过了允许的值!" );
			break;
		case EXCEPTION_FLT_STACK_CHECK:
			msg_.append( " 由于浮点操作造成栈溢出!" );
			break;
		case EXCEPTION_FLT_UNDERFLOW:
			msg_.append( " 浮点操作的结果小于允许的值!" );
			break;

		case EXCEPTION_STACK_OVERFLOW:
			msg_.append( " 栈溢出异常!" );
			break;
		default:
			msg_.append( " 其它未知类型的结构化异常!" );
			break;
		}

	}
	void struct_exception_base::MapSHEtoCE()
	{
		_set_se_translator(TranslateSEHtoCE);
	}

	PEXCEPTION_POINTERS struct_exception_base::ExceptionPointers()
	{
		return exception_;
	}

	DWORD struct_exception_base::ExceptionCode()
	{
		if ( NULL != exception_ && NULL != exception_->ExceptionRecord )
		{
			return exception_->ExceptionRecord->ExceptionCode;
		}
		else
			return 0;
	}

	PVOID struct_exception_base::ExceptionAddress()
	{
		if (NULL != exception_ && NULL != exception_->ExceptionRecord )
		{
			return exception_->ExceptionRecord->ExceptionAddress;
		}
		else
			return NULL;
	}

	void struct_exception_base::TranslateSEHtoCE(UINT code, PEXCEPTION_POINTERS pep )
	{
		throw struct_exception_base(code,pep);
	}
}
