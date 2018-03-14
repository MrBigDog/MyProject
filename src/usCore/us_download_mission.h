///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_download_mission.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : download_mission class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_DOWNLOAD_MISSION_H_
#define _US_DOWNLOAD_MISSION_H_

#include <usCore\Export.h>
#include <usCore\us_mission_base.h>
#include <usCore\us_event_base.h>
#include <usUtil\us_common_file.h>

#include <wtypes.h>

namespace uniscope_globe
{
	class downloader;
	class mission_executant;

	class USCORE_EXPORT download_mission : public mission_base
	{
	protected:
		download_mission(void) {}

	public:
		download_mission(const LPCTSTR str_url, downloader* v_downloader);

		virtual ~download_mission(void);

		enum enum_request_state
		{
			US_DEFAULT = 0,
			US_WAITING,
			US_LOADING,
			US_LOADED,
			US_ERROR,
			US_QUEUE
		};

	public:
		event_base	event_download_start;
		event_base	event_download_finish;

	public:
		void dispose();

		// override from mission_base
	public:
		virtual bool update(void);

		virtual void execute(void);

		virtual void reset(void);

		virtual void startup(void);

		virtual void terminate(void);

		// own status
	public:
		void set_executant(mission_executant* v_executant);

		void set_index(interior_index v_id);

		void set_url(const LPCTSTR str_url);

	protected:
		void on_succeed(byte* v_buf, int v_size);

		void on_failed(int load_result);

	protected:
		void case_default(void);

		void case_waiting(void);

		void case_loading(void);

		void case_loaded(void);

		void case_error(void);

		void case_queue(void);

	protected:
		int					m_error_count;

		ustring				m_request_url;

		interior_index		m_index;

		enum_request_state	m_request_state;

		downloader*			m_downloader;

		mission_executant*	m_mission_executant;

		bool				m_dispose;
	};
}

#endif // _US_COMMON_REQUEST_H_