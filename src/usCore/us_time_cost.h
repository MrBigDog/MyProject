///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_time_cost.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : time_cost class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_TIME_COST_H_
#define _US_TIME_COST_H_
#include <usCore\Export.h>
namespace uniscope_globe
{
	class USCORE_EXPORT time_cost
	{
	public:
		time_cost( void );

		~time_cost( void );

	public:
		void write_render_time();

		void write_update_time();

		void write_download_time();

	public:
		// for render thread
		long m_render_time;
		long m_create_time;
		long m_destroy_time;

		// for update thread
		long m_update_time;
		long m_reclaim_time;

		// for download
		long m_download_time;
		long m_uncompress_time;
		
		file_buffer m_render_file;
		file_buffer m_update_file;
		file_buffer m_download_file;

	};

	class USCORE_EXPORT singleton_time_cost_checker
		: public singleton<time_cost>
	{
	public:
		singleton_time_cost_checker( void )
		{
		}

		virtual ~singleton_time_cost_checker( void )
		{
		}
	};

}

#endif // _US_TIME_COST_H_