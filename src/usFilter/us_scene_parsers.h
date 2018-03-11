///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_xml_scene_parser.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : scene_parsers class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_SCENE_PARSERS_H_
#define _US_SCENE_PARSERS_H_
#include <usFilter\Export.h>

namespace uniscope_globe
{
	//
	// scene link
	//
	class LIB_FILTER_API xml_scene_link_parser : public xml_parser_base
	{
	public:
		xml_scene_link_parser(void){}

		virtual ~xml_scene_link_parser(void){}

		virtual object_base* forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object );

	};

	//
	// scene parser
	//
	class LIB_FILTER_API xml_scene_parser : public xml_parser_base
	{
	public:
		xml_scene_parser(void){}
	
		virtual ~xml_scene_parser(void){}

		virtual object_base* forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object );

	};

	//
	// billboard_set_parser
	//
	class LIB_FILTER_API xml_billboard_set_parser : public xml_parser_base
	{
	public:
		xml_billboard_set_parser( void ) {}
		
		virtual ~xml_billboard_set_parser( void ) {}

		virtual object_base* forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object );

	};

	//
	//	geo_spatial_parser
	//
	class LIB_FILTER_API xml_spherical_transform_parser : public xml_parser_base
	{
	public:
		xml_spherical_transform_parser( void ){}

		virtual ~xml_spherical_transform_parser( void ){}

		virtual object_base* forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object );

	};

	//
	//	birth_place_parser
	//
	class LIB_FILTER_API xml_birth_place_parser : public xml_parser_base
	{
	public:
		xml_birth_place_parser( void ){}

		virtual ~xml_birth_place_parser( void ){}

		virtual object_base* forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object );

	};


	//
	//	model_parser
	//
	class LIB_FILTER_API xml_model_parser : public xml_parser_base
	{
	public:
		xml_model_parser( void ){}

		virtual ~xml_model_parser( void ){}

		virtual object_base* forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object );

	};

	//
	//	volume_parser
	//
	class LIB_FILTER_API xml_volume_parser : public xml_parser_base
	{
	public:
		xml_volume_parser( void ){}

		virtual ~xml_volume_parser( void ){}

		virtual object_base* forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object );

	};

	//
	//	box_parser
	//
	class LIB_FILTER_API xml_box_parser : public xml_parser_base
	{
	public:
		xml_box_parser( void ){}

		virtual ~xml_box_parser( void ){}

		virtual object_base* forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object );
	};

	//
	//	sphere_parser
	//
	class LIB_FILTER_API xml_sphere_parser : public xml_parser_base
	{
	public:
		xml_sphere_parser( void ){}

		virtual ~xml_sphere_parser( void ){}

		virtual object_base* forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object );
	};

	//
	//	cylinder_parser
	//
	class LIB_FILTER_API xml_cylinder_parser : public xml_parser_base
	{
	public:
		xml_cylinder_parser( void ){}

		virtual ~xml_cylinder_parser( void ){}

		virtual object_base* forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object );
	};

	//
	//	cone_parser
	//
	class LIB_FILTER_API xml_cone_parser : public xml_parser_base
	{
	public:
		xml_cone_parser( void ){}

		virtual ~xml_cone_parser( void ){}

		virtual object_base* forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object );
	};

	//
	//	prism_parser
	//
	class LIB_FILTER_API xml_prism_parser : public xml_parser_base
	{
	public:
		xml_prism_parser( void ){}

		virtual ~xml_prism_parser( void ){}

		virtual object_base* forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object );
	};

	//
	//	pyramid_parser
	//
	class LIB_FILTER_API xml_pyramid_parser : public xml_parser_base
	{
	public:
		xml_pyramid_parser( void ){}

		virtual ~xml_pyramid_parser( void ){}

		virtual object_base* forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object );
	};


	//
	//	dummy_geometry_parser
	//
	class LIB_FILTER_API xml_dummy_geometry_parser : public xml_parser_base
	{
	public:
		xml_dummy_geometry_parser( void ){}

		virtual ~xml_dummy_geometry_parser( void ){}

		virtual object_base* forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object );

	};

	//
	//	scene_graph_parser
	//
	class LIB_FILTER_API xml_scene_graph_parser : public xml_parser_base
	{
	public:
		xml_scene_graph_parser( void ){}

		virtual ~xml_scene_graph_parser( void ){}

		virtual object_base* forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object );

	};

	//
	//	bound_box_parser
	//
	class LIB_FILTER_API xml_bound_box_parser : public xml_parser_base
	{
	public:
		xml_bound_box_parser( void ){}

		virtual ~xml_bound_box_parser( void ){}

		virtual object_base* forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object );
	};

	//
	//	label_parser
	//
	class LIB_FILTER_API xml_label_parser : public xml_parser_base
	{
	public:
		xml_label_parser( void ){}

		virtual ~xml_label_parser( void ){}

		virtual object_base* forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object );
	};

	//
	//	animation_link_parser
	//
	class LIB_FILTER_API xml_animation_link_parser : public xml_parser_base
	{
	public:
		xml_animation_link_parser( void ){}

		virtual ~xml_animation_link_parser( void ){}

		virtual object_base* forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object );
	};
}

#endif // _US_SCENE_PARSERS_H_