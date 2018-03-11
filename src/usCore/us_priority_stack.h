///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_priority_stack.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : priority_stack class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_PRIORITY_STACK_H_
#define _US_PRIORITY_STACK_H_
#include <usCore\Export.h>
namespace uniscope_globe
{
	class USCORE_EXPORT priority_stack
	{
	public:
		priority_stack(void);

		virtual ~priority_stack(void);

	public:
		void push(ulong in_priority) { m_rank = in_priority; }

		void pop(void) { m_rank = 0; }

		ulong get_priority(void) { return m_rank; }

	protected:
		ulong m_rank;
	};

	class USCORE_EXPORT singleton_priority_stack : public tls_singleton<priority_stack>
	{

	};
}

#endif // _US_PRIORITY_STACK_H_