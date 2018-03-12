
///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_proxy_resource_container.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : proxy_resource_container class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_PROXY_RESOURCE_CONTAINER_H_
#define _US_PROXY_RESOURCE_CONTAINER_H_
#include <usCore\Export.h>
namespace uniscope_globe
{
	template<typename KEY>
	class proxy_resource_container : public resource_container<KEY>
	{
	protected:
		proxy_resource_container(void) {}

	public:
		proxy_resource_container(document_base* in_doc, KEY key, LPCTSTR str_url, int v_type)
			: resource_container<KEY>(in_doc, key)
		{
			m_update_count = 0;
			m_url = str_url;
			m_download_type = 0x53094357;
		}

		virtual ~proxy_resource_container(void)
		{
		}

	public:
		virtual resource_base* get_resource(void)
		{
			if (m_resource == NULL)
			{
				square_buffer v_buffer(NULL, 0, 0, 0, m_url.c_str(), 0);
				filter_base* v_filter = singleton_filter_manager::instance().get_filter(m_download_type);
				m_resource = dynamic_cast<resource_base*>(v_filter->parse(&v_buffer));
				m_resource->add_ref();

			}
			return resource_container<KEY>::get_resource();

			//return m_resource;
		}

		// override from reclaim_base
		virtual void destroy(void)
		{
			//m_mission_flow->reset();

			DEFERRED_RELEASE(m_resource);

			reclaim_base::destroy();
		}

		virtual void add_ref(void)
		{
			__use_count__++;
			if (__use_count__ == 2)
			{
				if (m_resource != NULL)
					m_resource->set_resource_status(1);
			}
		}


		virtual void release(void)
		{
			if (__use_count__ == 2)
			{
				if (m_resource != NULL)
					m_resource->set_resource_status(0);
			}
			return resource_container<KEY>::release();
		}

	protected:
		ustring m_url;

		int m_download_type;

	};


	typedef proxy_resource_container<ustring> flash_resource_container;
}

#endif // _US_PROXY_RESOURCE_CONTAINER_H_ 