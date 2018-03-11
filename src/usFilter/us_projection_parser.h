///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_projection_parser.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : projection_parser class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_PROJECTION_PARSER_H_
#define _US_PROJECTION_PARSER_H_
#include <usFilter\Export.h>

namespace uniscope_globe
{
	class projection_parser	: public xml_parser_base
	{
	public:
		projection_parser( void );
		virtual ~projection_parser( void );

	public:
		virtual object_base* forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object );
		bool forward_parse(TiXmlNode* v_xml_node, uniscope_projection*& parent_object);
	};
}

#endif // _US_PROJECTION_PARSER_H_ 