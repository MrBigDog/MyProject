///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_xml_filter.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : xml_filter class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_xml_filter.h"

namespace uniscope_globe
{
	object_base* xml_filter::parse( raw_buffer* in_buffer )
	{
		filter_buffer* buf = (filter_buffer*)in_buffer;

		xml_parser_base* v_parser = singleton_xml_parser_manager::instance().get_parser( L"XML" );

		// load xml buffer
		TiXmlDocument xml_doc;
		xml_doc.Parse( (const char *)buf->get_buffer() );

		// parse
		TiXmlElement *v_elem = xml_doc.RootElement();
		return v_parser->forward_parse( buf->m_container, v_elem, buf->m_parent );
	}
}