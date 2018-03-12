///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_xml_mission.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : xml_mission class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_xml_mission.h"

namespace uniscope_globe
{
	xml_mission::xml_mission( void )
	{

	}

	xml_mission::xml_mission( LPCTSTR str_xml, LPCTSTR str_file, LPCTSTR str_type )
	{
		m_mission_args.m_xml_string = str_xml;
		m_mission_args.m_xml_file = str_file;
		m_mission = str_type;
	}

	xml_mission::~xml_mission( void )
	{
		
	}

	void xml_mission::execute( void )
	{
		// do mission
		m_xml_event( &m_mission_args );
		// remove mission
		singleton_common_mission_queue::instance().remove_mission( this );
	}

}