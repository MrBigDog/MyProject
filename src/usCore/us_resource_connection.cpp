///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_resource_connetion.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : resource_connection class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "Stdafx.h"
#include "us_resource_connection.h"
#include <usCore/us_download_argument.h>
#include <usCore/us_downloader.h>
#include <usCore/us_download_mission.h>

namespace uniscope_globe
{
	resource_connection::resource_connection(void)
	{
		m_parent_mngr = 0;
	}

	resource_connection::resource_connection(resource_manager<interior_index>* parent_mngr)
	{
		m_parent_mngr = parent_mngr;
	}

	resource_connection::~resource_connection(void)
	{
		resource_request_map::iterator itr = m_request_map.begin();
		for (; itr != m_request_map.end(); itr++)
		{
			AUTO_DELETE(itr->second);
		}
		m_request_map.clear();
	}

	void resource_connection::parse_raw_buffer(event_argument* args)
	{
		download_argument* v_download_args = (download_argument*)(args);

		remove_request(v_download_args->m_index);
	}

	bool resource_connection::request_data(interior_index index)
	{
		download_mission* v_download_mission = 0;

		resource_request_map::iterator itr = m_request_map.find(index);
		if (itr != m_request_map.end())
		{
			v_download_mission = itr->second;
		}
		else
		{
			//...
			v_download_mission = US_CREATE_MISSION_IN_HEAP(index_to_path(index).c_str());
			v_download_mission->set_index(index);
			v_download_mission->event_download_finish += event_handle(this, &resource_connection::parse_raw_buffer);
			m_request_map[index] = v_download_mission;
		}

		v_download_mission->update();

		return true;
	}

	void resource_connection::remove_request(interior_index index)
	{
		m_request_map.erase(index);
	}

}