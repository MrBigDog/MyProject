///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_resource_reclaimer.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : resource_reclaimer class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_RESOURCE_RECLAIMER_H_
#define _US_RESOURCE_RECLAIMER_H_

#include <usCore\Export.h>
#include <usUtil\us_bidirectional_chain.h>
#include <usUtil\us_mutex.h>
#include <usCore\us_tls_singleton.h>

namespace uniscope_globe
{
	class USCORE_EXPORT resource_reclaimer
		: public bidirectional_chain
	{
	public:
		resource_reclaimer(void);

		virtual ~resource_reclaimer(void);

		friend class reclaim_base;

	public:
		void update(void);

		ULONG_PTR get_memory_size(void);

		void push_front(reclaim_base* reclaim);

		void remove(reclaim_base* reclaim);

		void increase(ULONG_PTR v_size);

		void decrease(ULONG_PTR v_size);

	private:
		ULONG_PTR showMemoryInfo();

	private:
		ULONG_PTR m_max_memory_size;
		//long m_max_memory_size;

		double m_last_cost_time;

		US_AUTO_MUTEX

	};

	class USCORE_EXPORT singleton_resource_reclaimer
		: public tls_singleton<resource_reclaimer>
	{
	};
}

#endif // _US_RESOURCE_RECLAIMER_H_