///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_xml_mission.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : xml_mission class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_XML_MISSION_H_
#define _US_XML_MISSION_H_
#include <usFilter\Export.h>

namespace uniscope_globe
{
	class LIB_FILTER_API xml_mission_argument
		: public event_argument
	{
	public:
		xml_mission_argument( void ){}
		~xml_mission_argument( void ){}

	public:
		ustring m_xml_string;
		ustring m_xml_file;
	};
	

	class LIB_FILTER_API xml_mission
		: public mission_base
	{
	protected:
		xml_mission( void );

	public:
		xml_mission( LPCTSTR str_xml, LPCTSTR str_file, LPCTSTR str_type );

		virtual ~xml_mission( void );

	protected:
		virtual void execute( void );

		virtual bool update( void ){ return true; }

		virtual void reset( void ){ }

		virtual void terminate( void ) { }

	public: 
		event_base m_xml_event;

	protected:
		xml_mission_argument m_mission_args;

		ustring m_mission;
	};
}

#endif // _US_XML_MISSION_H_