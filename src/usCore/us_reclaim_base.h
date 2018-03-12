///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_reclaimer_base.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : reclaim_base class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_RECLAIMER_BASE_H_
#define _US_RECLAIMER_BASE_H_
#include <usCore\Export.h>
namespace uniscope_globe
{
	class USCORE_EXPORT reclaim_base : public chain_iterator, public shared_data
	{
	protected:
		reclaim_base() {}
	public:
		reclaim_base(document_base* in_doc);
		virtual ~reclaim_base(void);

		friend class resource_reclaimer;

	public:
		void set_reclaim_param(double v_param);

		double get_reclaim_param(void);

		void set_reclaimer(resource_reclaimer* v_reclaimer) { m_resource_reclaimer = v_reclaimer; }

		// method
	protected:
		virtual void update_size(ULONG_PTR v_size);

		// refresh
		virtual void refresh(ULONG_PTR v_size);

		// remove from reclaim list
		virtual void destroy(void);

		// judge time
		virtual bool can_reclaim(int v_life_time);

		// enbale_reclaim
		void enable_reclaim(bool v_enable);

		// property
	protected:
		// memory size
		void set_size(ULONG_PTR v_size) { m_size = v_size; }

		ULONG_PTR get_size(void) { return m_size; }

	protected:
		bool m_can_reclaim;

		ULONG_PTR m_size;

		time_value m_last_call_time;

		double m_param;

		LONGLONG m_update_count;

		document_base* m_document;

		resource_reclaimer* m_resource_reclaimer;
	};
}

#endif // _US_RECLAIMER_BASE_H_

