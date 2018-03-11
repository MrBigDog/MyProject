                                                                                                                                                                                                                              ///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_downloader.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : downloader class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_DOWNLOADER_H_
#define _US_DOWNLOADER_H_
#include <usCore\Export.h>
namespace uniscope_globe
{
	enum download_type
	{
		US_DOWNLOAD_IN_HEAP = 0,
		US_DOWNLOAD_IN_QUEUE
	};

	class USCORE_EXPORT downloader
	{
	public:
		downloader( void );

		virtual ~downloader( void );

	public:
		link_manager* get_link_manager( void );

		int get_mission_count( void );

	public:
		mission_base* create_mission_in_heap( const LPCTSTR str_url );

		mission_base* create_mission_in_queue( const LPCTSTR str_url );
		
		void destroy_mission( mission_base* v_mission );

		void update();

		void terminate( void );

		void write( const LPCTSTR str_log );

	private:
		mission_heap* m_mission_heap;
		mission_queue* m_mission_queue;

		link_manager* m_link_mngr;

		file_buffer m_file;
	};

	class USCORE_EXPORT tls_singleton_downloader: public tls_singleton<downloader>
	{
	public:
		tls_singleton_downloader( void ) { }
		virtual ~tls_singleton_downloader( void ) { }
	};
	
#define US_CREATE_MISSION_IN_HEAP(url)	(download_mission*)tls_singleton_downloader::instance().create_mission_in_heap(url);
#define US_CREATE_MISSION_IN_QUEUE(url)	(download_mission*)tls_singleton_downloader::instance().create_mission_in_queue(url);
#define US_DESTROY_MISSION(mssn)		tls_singleton_downloader::instance().create_mission_in_queue(mssn);

#define US_AUTO_GET_HTTP_LINK(link_info) tls_singleton_downloader::instance().get_link_manager()->get_http_link( link_info );

#define US_AUTO_CREATE_LINK(url) tls_singleton_downloader::instance().get_link_manager()->get_link( url );
#define US_AUTO_RELEASE_LINK(link) AUTO_DELETE(link)

}

#endif // _US_DOWNLOADER_H_