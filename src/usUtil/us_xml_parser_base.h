///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_xml_parser_base.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : xml_parser_base class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_XML_PARSER_BASE_H_
#define _US_XML_PARSER_BASE_H_

#include <usUtil\Export.h>

class TiXmlNode;

namespace uniscope_globe
{
	class object_base;

	class xml_parser_manager;

	class USUTIL_EXPORT xml_parser_base
	{
	public:
		xml_parser_base(void);

		virtual ~xml_parser_base(void);

	public:
		virtual object_base* forward_parse(object_base* v_container, TiXmlNode *v_xml_node, object_base *parent_object);

		virtual TiXmlNode*   backward_parse(object_base* v_container, object_base* ret_object, TiXmlNode* parent_node);

		void set_manager(xml_parser_manager* mngr);

	protected:
		xml_parser_manager* m_parser_mngr;
	};
}

#endif // _US_XML_PARSER_BASE_H_