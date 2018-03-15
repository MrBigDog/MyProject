///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_common_resource_container.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : common_resource_container class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_COMMON_RESOURCE_CONTAINER_H_
#define _US_COMMON_RESOURCE_CONTAINER_H_

//#include <usCore\Export.h>
#include <usUtil\us_common_file.h>
#include <usCore\us_resource_container.h>

namespace uniscope_globe
{
	template<typename KEY>
	class common_resource_container : public resource_container<KEY>
	{
	protected:
		common_resource_container(void) {}

	public:
		common_resource_container(document_base* in_doc, KEY key, LPCTSTR str_url, int v_type)
			: resource_container<KEY>(in_doc, key)
		{
			m_update_count = 0;
			m_raw_buffer = NULL;
			m_can_reach = true;
			m_url = str_url;
			m_mission_flow = NULL;
			m_download_type = v_type;
		}

		virtual ~common_resource_container(void)
		{
			AUTO_DELETE(m_mission_flow);
			AUTO_DELETE(m_raw_buffer);
		}

	public:
		virtual resource_base* get_resource(void)
		{
			if (m_url.size() != 0)
			{
				if (m_mission_flow == NULL)
				{
					m_mission_flow = new mission_flow;
					download_mission* v_download_mission = NULL;
					switch (m_download_type)
					{
					case US_DOWNLOAD_IN_HEAP:
						v_download_mission = US_CREATE_MISSION_IN_HEAP(m_url.c_str());
						break;
					case US_DOWNLOAD_IN_QUEUE:
						v_download_mission = US_CREATE_MISSION_IN_QUEUE(m_url.c_str());
						break;
					}
					v_download_mission->event_download_start += event_handle(this, &common_resource_container<KEY>::on_download_start);
					v_download_mission->event_download_finish += event_handle(this, &common_resource_container<KEY>::on_download_finish);
					m_mission_flow->add_mission(v_download_mission);
				}

				m_mission_flow->update();
			}


			return resource_container<KEY>::get_resource();
		}

		// override from reclaim_base
		virtual void destroy(void)
		{

			if (m_mission_flow != NULL)
			{
				m_mission_flow->reset();

				DEFERRED_RELEASE(m_resource);
			}

			reclaim_base::destroy();
		}

		virtual void reset(void)
		{
			if (m_mission_flow == NULL)
				return;

			m_mission_flow->reset();

			mission_base* v_mission = m_mission_flow->remove_mission(0);
			if (v_mission)
			{
				download_mission* v_load_mission = dynamic_cast<download_mission*>(v_mission);
				if (v_load_mission)
				{
					v_load_mission->dispose();
				}
				else
				{
					AUTO_DELETE(v_mission);
				}
			}

			download_mission* v_download_mission = NULL;
			switch (m_download_type)
			{
			case US_DOWNLOAD_IN_HEAP:
				v_download_mission = US_CREATE_MISSION_IN_HEAP(m_url.c_str());
				break;
			case US_DOWNLOAD_IN_QUEUE:
				v_download_mission = US_CREATE_MISSION_IN_QUEUE(m_url.c_str());
				break;
			}
			v_download_mission->event_download_start += event_handle(this, &common_resource_container<KEY>::on_download_start);
			v_download_mission->event_download_finish += event_handle(this, &common_resource_container<KEY>::on_download_finish);
			m_mission_flow->insert_mission(0, v_download_mission);

			DEFERRED_RELEASE(m_resource);
		}

		// download mission
	protected:
		virtual void on_download_start(event_argument* args)
		{
			enable_reclaim(false);

			shared_data::add_ref();
		}

		virtual void on_download_finish(event_argument* args)
		{
			download_argument* v_download_args = (download_argument*)args;

			if (v_download_args->m_download_status == US_DOWNLOAD_ERROR || v_download_args->m_download_status == US_DOWNLOAD_NOFILE)
			{
				m_can_reach = false;
			}
			else if (v_download_args->m_download_status == US_DOWNLOAD_SUCCEED)
			{
				m_resource = dynamic_cast<resource_base*>(singleton_filter_manager::instance().parse((raw_buffer*)v_download_args->m_stream));
				if (m_resource)
				{
					m_resource->add_ref();
				}
			}

			enable_reclaim(true);

			shared_data::release();
		}

	protected:
		mission_flow* m_mission_flow;

		raw_buffer* m_raw_buffer;

		ustring m_url;

		int m_download_type;

	};

	typedef common_resource_container<ustring> texture_resource_container;
	typedef common_resource_container<interior_index> index_texture_resource_container;
}

#endif // _US_COMMON_RESOURCE_CONTAINER_H_