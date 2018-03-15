///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_xml_document_parser.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : xml_document_parser class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_XML_DOCUMENT_PARSER_H_
#define _US_XML_DOCUMENT_PARSER_H_
#include <usFilter\Export.h>

namespace uniscope_globe
{
	class xml_parser_manager;


	//	XMLGlobe Parser
	class USFILTER_EXPORT xml_globe_parser
		: public xml_parser_base
	{
	public:
		xml_globe_parser(void) {}

		virtual ~xml_globe_parser(void) {}

	public:
		virtual object_base* forward_parse(object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object);
	};

	//
	//	XML Terrain Parser
	//
	class USFILTER_EXPORT xml_dem_system_parser
		: public xml_parser_base
	{
	public:
		xml_dem_system_parser(void) {}

		virtual ~xml_dem_system_parser(void) {}

	public:
		virtual object_base *forward_parse(object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object);
	};

	//
	//	XML Image Parser
	//
	class USFILTER_EXPORT xml_dom_system_parser
		: public xml_parser_base
	{
	public:
		xml_dom_system_parser(void) {}

		virtual ~xml_dom_system_parser(void) {}

	public:
		virtual object_base *forward_parse(object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object);
	};

	//
	//	XML Primary Coverage Parser
	//
	class USFILTER_EXPORT xml_primary_coverage_parser
		: public xml_parser_base
	{
	public:
		xml_primary_coverage_parser(void) {}

		virtual ~xml_primary_coverage_parser(void) {}

	public:
		virtual object_base *forward_parse(object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object);
	};

	//
	//	XML Secondary Coverage Parser
	//
	class USFILTER_EXPORT xml_secondary_coverage_parser
		: public xml_parser_base
	{
	public:
		xml_secondary_coverage_parser(void) {}

		virtual ~xml_secondary_coverage_parser(void) {}

	public:
		virtual object_base *forward_parse(object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object);
	};

	//
	//	XML Connection Parser
	//
	class USFILTER_EXPORT xml_connection_parser
		: public xml_parser_base
	{
	public:
		xml_connection_parser(void) {}

		virtual ~xml_connection_parser(void) {}

	public:
		virtual object_base *forward_parse(object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object);
	};


	//
	//	XMLModelLayer Parser
	//
	class USFILTER_EXPORT xml_model_layer_parser
		: public xml_parser_base
	{
	public:
		xml_model_layer_parser(void) {}

		virtual ~xml_model_layer_parser(void) {}

	public:
		virtual object_base *forward_parse(object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object);
	};

	//
	//	xml building layer performance parser
	//
	class USFILTER_EXPORT xml_bldg_layer_performance_parser
		: public xml_parser_base
	{
	public:
		xml_bldg_layer_performance_parser(void) {}

		virtual ~xml_bldg_layer_performance_parser(void) {}

	public:
		virtual object_base *forward_parse(object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object);

	protected:
		bool parse_texture_connection(TiXmlNode* v_xml_node, void* in_object);

		bool parse_building_connection(TiXmlNode* v_xml_node, void* in_object);

		bool parse_lod_parameter(TiXmlNode* v_xml_node, void* in_object);
	};

	//
	//	xml building layer config parser
	//
	class USFILTER_EXPORT xml_bldg_layer_config_parser
		: public xml_parser_base
	{
	public:
		xml_bldg_layer_config_parser(void) {}

		virtual ~xml_bldg_layer_config_parser(void) {}

	public:
		virtual object_base *forward_parse(object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object);
	};

	//
	//	block_layer Parser
	//
	class USFILTER_EXPORT xml_block_layer_parser
		: public xml_parser_base
	{
	public:
		xml_block_layer_parser(void) {}

		virtual ~xml_block_layer_parser(void) {}

	public:
		virtual object_base *forward_parse(object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object);
	};


	//
	//	XMLPOILayer Parser
	//
	class USFILTER_EXPORT xml_poi_layer_parser
		: public xml_parser_base
	{
	public:
		xml_poi_layer_parser(void) {}

		virtual ~xml_poi_layer_parser(void) {}

	public:
		virtual object_base *forward_parse(object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object);

		void parse_poi_style(TiXmlNode* v_xml_node, poi_style& v_style);
	};

	//
	//	XMLVectorLayer Parser
	//
	class USFILTER_EXPORT xml_vector_layer_parser
		: public xml_parser_base
	{
	public:
		xml_vector_layer_parser(void) {}

		virtual ~xml_vector_layer_parser(void) {}

	public:
		virtual object_base *forward_parse(object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object);
	};

	//
	//	Annotation Layer Parser
	//
	class USFILTER_EXPORT xml_annotation_layer_parser
		: public xml_parser_base
	{
	public:
		xml_annotation_layer_parser(void) {}

		virtual ~xml_annotation_layer_parser(void) {}

	public:
		virtual object_base *forward_parse(object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object);
	};

	//
	//	XMLBillboardChunk Parser
	//
	class USFILTER_EXPORT xml_billboard_layer_parser
		: public xml_parser_base
	{
	public:
		xml_billboard_layer_parser(void) {}

		virtual ~xml_billboard_layer_parser(void) {}

	public:
		virtual object_base *forward_parse(object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object);
	};

	//
	//	Match Model Parser
	//
	class USFILTER_EXPORT xml_match_model_layer_parser
		: public xml_parser_base
	{
	public:
		xml_match_model_layer_parser(void) {}

		virtual ~xml_match_model_layer_parser(void) {}

	public:
		virtual object_base *forward_parse(object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object);
	};


}

#endif // _US_XML_DOCUMENT_PARSERS_H_