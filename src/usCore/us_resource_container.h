///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_resource_container.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : resource_container class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_RESOURCE_CONTAINER_H_
#define _US_RESOURCE_CONTAINER_H_

#include <usCore\Export.h>
#include <usCore\us_reclaim_base.h>
#include <usCore\us_resource_base.h>

namespace uniscope_globe
{
	template<typename KEY>
	class resource_manager;

	template<typename KEY>
	class resource_container : public reclaim_base
	{
	protected:
		resource_container() {}

	public:
		resource_container(document_base* in_doc, KEY key)
			: reclaim_base(in_doc)
		{
			m_resource_manager = NULL;
			m_can_reach = true;

			m_key = key;
			m_resource = NULL;
			refresh(0);
		}

		virtual ~resource_container(void) { AUTO_RELEASE_SHARED_DATA(m_resource); }

	public:
		KEY& get_key(void) { return m_key; }

		bool can_reach(void) { return m_can_reach; }

		virtual resource_base* get_resource(void)
		{
			if (m_resource)
			{
				refresh(m_resource->get_mem_size());
			}
			else
			{
				refresh(0);
			}

			return m_resource;
		};

		// override from reclaim_base
		virtual void destroy(void)
		{
			DEFERRED_RELEASE(m_resource);

			reclaim_base::destroy();
		}

		virtual void reset(void)
		{

		}

		virtual void release(void)
		{
			//__use_count__--;
			//if ( __use_count__ == 0 )
			//{
			//	if( m_resource_manager )
			//	{
			//		m_resource_manager->remove_resource( m_key );
			//	}
			//}
			//if ( __use_count__ == -1 )
			//{
			//	delete this;
			//}

			__use_count__--;
			if (__use_count__ < 1)
			{
				if (m_resource_manager)
				{
					m_resource_manager->remove_resource(m_key);
				}

				delete this;

				return;
			}

			//if ( __use_count__ == 1 )
			//{
			//	if( m_resource_manager )
			//	{
			//		m_resource_manager->remove_resource( m_key );
			//	}
			//}

		}

		virtual void attach(resource_manager<KEY>* in_resource_manager)
		{
			m_resource_manager = in_resource_manager;
		}

		virtual void detach(void)
		{
			m_resource_manager = NULL;
		}

	protected:
		resource_manager<KEY>* m_resource_manager;

		resource_base* m_resource;

		bool m_can_reach;

		KEY m_key;
	};
}

#endif // _US_RESOURCE_CONTAINER_H_