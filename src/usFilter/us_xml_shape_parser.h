///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_xml_shape_parser.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : xml_shape_parser class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_XML_SHAPE_PARSER_H_
#define _US_XML_SHAPE_PARSER_H_
#include <usFilter\Export.h>

namespace uniscope_globe
{
	// 
	// xml_element_block_parser
	//
	class USFILTER_EXPORT xml_edit_element_block_parser
		: public xml_parser_base
	{
	public:
		xml_edit_element_block_parser( void ){}

		virtual ~xml_edit_element_block_parser(void){}

	public:
		virtual object_base* forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object );

	protected:
		void parse_spheical_transform( TiXmlNode* v_xml_node, vector_3d& v_location, vector_3d& v_rotation, vector_3d& v_scaling );

		void parse_box( TiXmlNode* v_xml_node, double& v_length, double& v_width, double& v_height );

		void parse_sphere( TiXmlNode* v_xml_node, double& v_radius );

		void parse_cone( TiXmlNode* v_xml_node, double& v_radius, double& v_height );

		void parse_cylinder( TiXmlNode* v_xml_node, double& v_radius, double& v_height );

		void parse_prism( TiXmlNode* v_xml_node, long& v_sides, double& v_radius, double& v_height );

		void parse_pyramid( TiXmlNode* v_xml_node, long& v_sides, double& v_radius, double& v_height );

		void parse_volume( TiXmlNode* v_xml_node, point_list_d& v_points, double& vol_height );
	};
}

#endif // _US_XML_SHAPE_PARSER_H_