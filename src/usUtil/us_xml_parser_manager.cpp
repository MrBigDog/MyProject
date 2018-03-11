///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_xml_parser_manager.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : xml_parser_manager class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////

#include "us_xml_parser_manager.h"
#include <usUtil/us_xml_parser_base.h>

namespace uniscope_globe
{
#ifdef WIN32
	xml_parser_manager* singleton_xml_parser_manager::s_ins = NULL;
#endif //WIN32

	xml_parser_manager::xml_parser_manager(void)
	{

	}

	xml_parser_manager::~xml_parser_manager(void)
	{
		xml_parser_map::iterator itr = m_parser_map.begin();
		for (; itr != m_parser_map.end(); itr++)
		{
			AUTO_DELETE(itr->second);
		}
		m_parser_map.clear();
	}

	void xml_parser_manager::register_parser(cpstr str_key, xml_parser_base* v_parser)
	{
		m_parser_map[str_key] = v_parser;
		v_parser->set_manager(this);
	}

	xml_parser_base* xml_parser_manager::get_parser(cpstr str_key)
	{
		return m_parser_map[str_key];
	}


}