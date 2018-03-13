///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_xml_parser_base.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : xml_parser_base class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////

#include "us_xml_parser_base.h"
#include <usUtil/us_common_file.h>
#include <usUtil/us_xml_parser_manager.h>
#include <usXml/tinyxml.h>

namespace uniscope_globe
{
	xml_parser_base::xml_parser_base(void)
	{
		m_parser_mngr = NULL;
	}

	xml_parser_base::~xml_parser_base(void)
	{

	}

	object_base* xml_parser_base::forward_parse(object_base* v_container, TiXmlNode *v_xml_node, object_base *parent_object)
	{
		object_base* v_obj = NULL;

		int type = v_xml_node->Type();

		if (type != TiXmlNode::ELEMENT)
			return NULL;

		TiXmlNode* child_node = v_xml_node->FirstChild();

		while (child_node)
		{
			int child_type = child_node->Type();

			if (child_type != TiXmlNode::ELEMENT)
			{
				child_node = child_node->NextSibling();
				continue;
			}

			ustring str;
#ifdef _UNICODE
			str = string_ext::to_wstring(child_node->Value());
#else
			str = child_node->Value();
#endif //_UNICODE

			xml_parser_base* xml_parser = m_parser_mngr->get_parser(str.c_str());

			if (xml_parser)
			{
				v_obj = xml_parser->forward_parse(v_container, (TiXmlElement*)child_node, parent_object);
			}

			child_node = child_node->NextSibling();
		}

		return v_obj;
	}

	TiXmlNode* xml_parser_base::backward_parse(object_base* v_container, object_base* ret_object, TiXmlNode* parent_node)
	{
		return NULL;
	}

	void xml_parser_base::set_manager(xml_parser_manager* mngr)
	{
		m_parser_mngr = mngr;
	}
}