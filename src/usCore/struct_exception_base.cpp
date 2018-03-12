///////////////////////////////////////////////////////////
//  gw_exception_base.cpp
//  Implementation of the Class gw_exception_base
//  Created on:      10-ʮһ��-2015 11:26:52
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
			msg_.append( " �߳���ͼ����δ�����Ƿ��ڴ���쳣!" );
			break;
		case EXCEPTION_DATATYPE_MISALIGNMENT:
			msg_.append( " �߳���ͼ����д��֧�ֶ����Ӳ���ϵ�δ���������!" );
			break;
		case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
			msg_.append( " �߳���ͼ��ȡһ��Խ�������Ԫ��!" );
			break;
		case EXCEPTION_IN_PAGE_ERROR:
			msg_.append( " �����ļ�ϵͳ��һ���豸�������򷵻�һ����������ɲ�������Ҫ���ҳ����!" );
			break;
		case EXCEPTION_GUARD_PAGE:
			msg_.append( " �߳���ͼ��ȡһ������PAGE_GUARD�������Ե��ڴ�ҳ!" );
			break;
		case EXCEPTION_ILLEGAL_INSTRUCTION:
			msg_.append( " �߳�ִ����һ����Ч��ָ��!" );
			break;
		case EXCEPTION_PRIV_INSTRUCTION:
			msg_.append( " �߳�ִ����һ����ǰ����ģʽ�������ָ��!" );
			break;
		case EXCEPTION_INVALID_DISPOSITION:
			msg_.append( " �쳣�����������˴����ֵ!" );
			break;
		case EXCEPTION_NONCONTINUABLE_EXCEPTION:
			msg_.append( " �쳣��������һ�����ܼ������쳣���ص�EXCEPTION_CONTINUE_EXCEPTION!" );
			break;

		case EXCEPTION_INT_DIVIDE_BY_ZERO:
			msg_.append( " �����������쳣!" );
			break;
		case EXCEPTION_INT_OVERFLOW:
			msg_.append( " һ�����������Ľ������������ֵ�涨�ķ�Χ!" );
			break;

		case EXCEPTION_FLT_DIVIDE_BY_ZERO:
			msg_.append( " �����������쳣" );
			break;
		case EXCEPTION_FLT_DENORMAL_OPERAND:
			msg_.append( " ��������е�һ��������������!" );
			break;
		case EXCEPTION_FLT_INEXACT_RESULT:
			msg_.append( " ��������Ľṹ���ܾ�ȷ��ʾ��ʮ����С��!" );
			break;
		case EXCEPTION_FLT_INVALID_OPERATION:
			msg_.append( " ��ʾ�κ�û���ڴ��г��������������쳣!" );
			break;
		case EXCEPTION_FLT_OVERFLOW:
			msg_.append( " ��������Ľṹ�����������ֵ!" );
			break;
		case EXCEPTION_FLT_STACK_CHECK:
			msg_.append( " ���ڸ���������ջ���!" );
			break;
		case EXCEPTION_FLT_UNDERFLOW:
			msg_.append( " ��������Ľ��С�������ֵ!" );
			break;

		case EXCEPTION_STACK_OVERFLOW:
			msg_.append( " ջ����쳣!" );
			break;
		default:
			msg_.append( " ����δ֪���͵Ľṹ���쳣!" );
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
