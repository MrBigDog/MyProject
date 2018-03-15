///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename:
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose :
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_DYNAMIC_PARSER_H_
#define _US_DYNAMIC_PARSER_H_
#include <usFilter\Export.h>

namespace uniscope_globe
{
	class USFILTER_EXPORT dynamic_layer_parser: public xml_parser_base
	{
	public:
		dynamic_layer_parser( void ){}

		virtual ~dynamic_layer_parser( void ){}

	public:
		virtual object_base* forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object ); 
	};

	class USFILTER_EXPORT dynamic_model_parser: public xml_parser_base
	{
	public:
		dynamic_model_parser( void ){}

		virtual ~dynamic_model_parser( void ){}

	public:
		virtual object_base* forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object ); 
	};

	class USFILTER_EXPORT dynamic_billboard_set_parser: public xml_parser_base
	{
	public:
		dynamic_billboard_set_parser( void ){}

		virtual ~dynamic_billboard_set_parser( void ){}

	public:
		virtual object_base* forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object ); 
	};

	class USFILTER_EXPORT dynamic_label_parser: public xml_parser_base
	{
	public:
		dynamic_label_parser( void ){}

		virtual ~dynamic_label_parser( void ){}

	public:
		virtual object_base* forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object ); 
	};

	//
	// sound_style
	//
	class USFILTER_EXPORT dynamic_sound_parser
		: public xml_parser_base
	{
	public:
		dynamic_sound_parser(void){}

		virtual ~dynamic_sound_parser(void){}

	public:
		virtual object_base* forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object );
	};

}

#endif // _US_DYNAMIC_PARSER_H_