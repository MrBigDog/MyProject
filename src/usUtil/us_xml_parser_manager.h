///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_xml_parser_manager.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : xml_parser_manager class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_XML_PARSER_MANAGER_H_
#define _US_XML_PARSER_MANAGER_H_

#include <usUtil\Export.h>
#include <usUtil\us_common_file.h>
#include <usUtil\us_singleton.h>

#include <map>

namespace uniscope_globe
{
	class xml_parser_base;

	class USUTIL_EXPORT xml_parser_manager
	{
	public:
		typedef std::map<ustring, xml_parser_base*> xml_parser_map;

	public:
		xml_parser_manager(void);

		virtual ~xml_parser_manager(void);

	public:
		xml_parser_base* get_parser(cpstr str_key);

		void register_parser(cpstr str_key, xml_parser_base* v_parser);

	protected:
		xml_parser_map m_parser_map;

	};

	class USUTIL_EXPORT singleton_xml_parser_manager : public singleton<xml_parser_manager>
	{
	public:
		singleton_xml_parser_manager(void)
		{
		}

		virtual ~singleton_xml_parser_manager(void)
		{
		}
	};

#ifdef LINUX26
	template<class xml_parser_manager>	xml_parser_manager* singleton<xml_parser_manager>::s_ins = NULL;
#endif //LINUX26
}

#endif // _US_XML_PARSER_MANAGER_H_