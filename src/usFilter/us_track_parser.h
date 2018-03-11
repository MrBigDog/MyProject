///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_track_parser.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : track_parser class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_TRACK_PARSER_H_
#define _US_TRACK_PARSER_H_
#include <usFilter\Export.h>

namespace uniscope_globe
{
	class LIB_FILTER_API track_parser : public xml_parser_base
	{
	public:
		track_parser( void ){}

		virtual ~track_parser( void ){}
		
	public:
		virtual object_base* forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object );
	};

	class LIB_FILTER_API station_route_parser : public xml_parser_base
	{
	public:
		station_route_parser( void ){}

		virtual ~station_route_parser( void ){}

	public:
		virtual object_base* forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object );
	};
	
	class LIB_FILTER_API station_pass_parser : public xml_parser_base
	{
	public:
		station_pass_parser( void ){}

		virtual ~station_pass_parser( void ){}

	public:
		virtual object_base* forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object );
	};

	class LIB_FILTER_API station_stamp_parser : public xml_parser_base
	{
	public:
		station_stamp_parser( void ){}

		virtual ~station_stamp_parser( void ){}

	public:
		virtual object_base* forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object );
	};

	class LIB_FILTER_API station_lookat_parser : public xml_parser_base
	{
	public:
		station_lookat_parser( void ){}

		virtual ~station_lookat_parser( void ){}

	public:
		virtual object_base* forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object );
	};

	class LIB_FILTER_API station_surround_parser : public xml_parser_base
	{
	public:
		station_surround_parser( void ){}

		virtual ~station_surround_parser( void ){}

	public:
		virtual object_base* forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object );
	};

	class LIB_FILTER_API station_stop_parser : public xml_parser_base
	{
	public:
		station_stop_parser( void ){}

		virtual ~station_stop_parser( void ){}

	public:
		virtual object_base* forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object );
	};
}

#endif // _US_TRACK_PARSER_H_