///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_downloader.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : downloader class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_downloader.h"
#include <usCore/us_link_manager.h>
#include <usCore/us_mission_heap.h>
#include <usCore/us_mission_queue.h>
#include <usCore/us_system_environment.h>
#include <usCore/us_download_mission.h>

#include <wtypes.h>

namespace uniscope_globe
{
	DWORD tls_singleton_downloader::m_tls_index = TlsAlloc();

	downloader::downloader()
	{
		m_link_mngr = new link_manager;
		m_mission_heap = new mission_heap(system_environment::s_download_thread_count, 128);
		m_mission_queue = new mission_queue(1);
		//m_file.file_open( L"C:\\download.txt", L"w" );
	}

	downloader::~downloader()
	{
		//m_file.file_close();
		terminate();
	}

	void downloader::write(const LPCTSTR str_log)
	{
		ustring str = str_log;
		str += /*L*/"\n";
		m_file.write(str);
	}

	link_manager* downloader::get_link_manager(void)
	{
		return m_link_mngr;
	}

	int downloader::get_mission_count(void)
	{
		return m_mission_heap->get_mission_count() + m_mission_queue->get_mission_count();
	}

	mission_base* downloader::create_mission_in_heap(const LPCTSTR str_url)
	{
		if (/*wcslen*/strlen(str_url) == 0)
			return NULL;

		download_mission* v_ret = new download_mission(str_url, this);
		v_ret->set_executant(m_mission_heap);

		return v_ret;
	}

	mission_base* downloader::create_mission_in_queue(const LPCTSTR str_url)
	{
		if (/*wcslen*/strlen(str_url) == 0)
			return NULL;

		download_mission* v_ret = new download_mission(str_url, this);
		v_ret->set_executant(m_mission_queue);

		return v_ret;
	}

	void downloader::destroy_mission(mission_base* v_mission)
	{
		AUTO_DELETE(v_mission);
	}

	void downloader::terminate(void)
	{
		m_mission_heap->terminate_mission();
		AUTO_DELETE(m_mission_heap);

		//m_mission_queue->terminate_mission();
		AUTO_DELETE(m_mission_queue);

		AUTO_DELETE(m_link_mngr);
	}

	void downloader::update()
	{
		m_mission_heap->clear_back_buffer();

		static int k = 0;

		if (k % 100 == 0)
		{
			m_link_mngr->update();
		}

		k++;
	}

}