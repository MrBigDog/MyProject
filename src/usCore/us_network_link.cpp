///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_network_link.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : network_link class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_network_link.h"

namespace uniscope_globe
{
	network_link::network_link( void )
	{
		m_loaded = false;	

		m_download_mission = NULL;
	}

	network_link::~network_link( void )
	{
		AUTO_DELETE( m_download_mission );
	}

	void network_link::set_link( cpstr link )
	{
		m_link = link;
	}

	ustring network_link::get_link( void )
	{
		return m_link;
	}

	void network_link::load( void )
	{
		if ( m_loaded == false && m_download_mission == NULL )
		{
			AUTO_DELETE( m_download_mission );
			m_download_mission = US_CREATE_MISSION_IN_QUEUE( m_link.c_str() );
			m_download_mission->event_download_finish += event_handle( this, &network_link::on_download_finish );
		}		

		m_download_mission->update();
	}

	void network_link::reload( void )
	{
		if ( m_loaded == false && m_download_mission == NULL )
		{
			AUTO_DELETE( m_download_mission );
			m_download_mission = US_CREATE_MISSION_IN_QUEUE( m_link.c_str() );
			m_download_mission->event_download_finish += event_handle( this, &network_link::on_download_finish );
		}		

		m_download_mission->reset();

		m_download_mission->update();
	}

	bool network_link::is_loaded( void )
	{
		return m_loaded;
	}

	void network_link::on_download_finish( event_argument* args )
	{
		m_loaded = true;
	}

}