///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_xml_element_parsers.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : xml_element_parsers class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_XML_ELEMENT_PARSERS_H_
#define _US_XML_ELEMENT_PARSERS_H_
#include <usFilter\Export.h>

namespace uniscope_globe
{

	//
	//	xml_element_document_parser
	//
	class USFILTER_EXPORT xml_element_document_parser : public xml_parser_base
	{
	public:
		xml_element_document_parser( void ){}

		virtual ~xml_element_document_parser( void ){}

	public:
		virtual object_base* forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object );
	};

	//
	//	xml_element_folder_parser
	//
	class USFILTER_EXPORT xml_element_folder_parser : public xml_parser_base
	{
	public:
		xml_element_folder_parser( void ){}

		virtual ~xml_element_folder_parser( void ){}

	public:
		virtual object_base* forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object );
	};

	//
	//	xml_element_ground_overlay_parser
	//
	class USFILTER_EXPORT xml_element_ground_overlay_parser : public xml_parser_base
	{
	public:
		xml_element_ground_overlay_parser( void ){}

		virtual ~xml_element_ground_overlay_parser( void ){}

	public:
		virtual object_base* forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object );
	};

	// 
	// icon
	//
	class USFILTER_EXPORT xml_element_icon_parser
		: public xml_parser_base
	{
	public:
		xml_element_icon_parser( void ){}

		virtual ~xml_element_icon_parser(void){}

	public:
		virtual object_base* forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object );
	};

	//
	//	xml_element_points_parser
	//
	class USFILTER_EXPORT xml_element_points_parser : public xml_parser_base
	{
	public:
		xml_element_points_parser( void ){}

		virtual ~xml_element_points_parser( void ){}

	public:
		virtual object_base* forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object );
	};

	//
	//	xml_element_line_parser
	//
	class USFILTER_EXPORT xml_element_line_parser : public xml_parser_base
	{
	public:
		xml_element_line_parser( void ){}

		virtual ~xml_element_line_parser( void ){}

	public:
		virtual object_base* forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object );
	};


	//
	//	xml_element_polygon_parser
	//
	class USFILTER_EXPORT xml_element_polygon_parser : public xml_parser_base
	{
	public:
		xml_element_polygon_parser( void ){}

		virtual ~xml_element_polygon_parser( void ){}

	public:
		virtual object_base* forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object );
	};

	//
	//	xml_element_circle_parser
	//
	class USFILTER_EXPORT xml_element_circle_parser : public xml_parser_base
	{
	public:
		xml_element_circle_parser( void ){}

		virtual ~xml_element_circle_parser( void ){}

	public:
		virtual object_base* forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object );
	};

	//
	//	xml_element_box_parser
	//
	class USFILTER_EXPORT xml_element_box_parser : public xml_parser_base
	{
	public:
		xml_element_box_parser( void ){}

		virtual ~xml_element_box_parser( void ){}

	public:
		virtual object_base* forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object );
	};

	//
	//	xml_element_sphere_parser
	//
	class USFILTER_EXPORT xml_element_sphere_parser : public xml_parser_base
	{
	public:
		xml_element_sphere_parser( void ){}

		virtual ~xml_element_sphere_parser( void ){}

	public:
		virtual object_base* forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object );
	};


	//
	//	xml_element_rectangle_parser
	//
	class USFILTER_EXPORT xml_element_rectangle_parser : public xml_parser_base
	{
	public:
		xml_element_rectangle_parser( void ){}

		virtual ~xml_element_rectangle_parser( void ){}

	public:
		virtual object_base* forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object );
	};

	//
	//	xml_element_mesh_parser
	//
	class USFILTER_EXPORT xml_element_mesh_parser : public xml_parser_base
	{
	public:
		xml_element_mesh_parser( void ){}

		virtual ~xml_element_mesh_parser( void ){}

	public:
		virtual object_base* forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object );

	protected:
		void parse_mesh( TiXmlNode* v_xml_node, point_list_d& v_point_list, index_list_u& v_index_list );

		void parse_box( TiXmlNode* v_xml_node, double& v_length, double& v_width, double& v_height );

		void parse_sphere( TiXmlNode* v_xml_node, double& v_radius );

		void parse_cone( TiXmlNode* v_xml_node, double& v_radius, double& v_height );

		void parse_cylinder( TiXmlNode* v_xml_node, double& v_radius, double& v_height );

		void parse_prism( TiXmlNode* v_xml_node, long& v_sides, double& v_radius, double& v_height );

		void parse_pyramid( TiXmlNode* v_xml_node, long& v_sides, double& v_radius, double& v_height );

		void parse_volume( TiXmlNode* v_xml_node, point_list_d& v_points, double& vol_height );
	};

	// 
	// xml_element_text_parser
	//
	class USFILTER_EXPORT xml_element_text_parser
		: public xml_parser_base
	{
	public:
		xml_element_text_parser( void ){}

		virtual ~xml_element_text_parser(void){}

	public:
		virtual object_base* forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object );
	};

	// 
	// xml_element_billboard_parser
	//
	class USFILTER_EXPORT xml_element_billboard_parser
		: public xml_parser_base
	{
	public:
		xml_element_billboard_parser( void ){}

		virtual ~xml_element_billboard_parser(void){}

	public:
		virtual object_base* forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object );
	};

	// 
	// xml_element_block_parser
	//
	class USFILTER_EXPORT xml_element_block_parser
		: public xml_parser_base
	{
	public:
		xml_element_block_parser( void ){}

		virtual ~xml_element_block_parser(void){}

	public:
		virtual object_base* forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object );

	protected:
		void parse_mesh( TiXmlNode* v_xml_node, point_list_d& v_point_list, index_list_u& v_index_list );

		void parse_box( TiXmlNode* v_xml_node, double& v_length, double& v_width, double& v_height );

		void parse_sphere( TiXmlNode* v_xml_node, double& v_radius );

		void parse_cone( TiXmlNode* v_xml_node, double& v_radius, double& v_height );

		void parse_cylinder( TiXmlNode* v_xml_node, double& v_radius, double& v_height );

		void parse_prism( TiXmlNode* v_xml_node, long& v_sides, double& v_radius, double& v_height );

		void parse_pyramid( TiXmlNode* v_xml_node, long& v_sides, double& v_radius, double& v_height );

		void parse_volume( TiXmlNode* v_xml_node, point_list_d& v_points, double& vol_height );
	};


	//
	// look_at
	//
	class USFILTER_EXPORT xml_lookat_parser
		: public xml_parser_base
	{
	public:
		xml_lookat_parser( void ){}

		virtual ~xml_lookat_parser(void){}

	public:
		virtual object_base* forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object );
	};

	
	//////////////////////////////////////////////////////////////////////////
	// update parsers
	//////////////////////////////////////////////////////////////////////////

	//
	//	xml_element_update_parser
	//
	class USFILTER_EXPORT xml_element_update_parser : public xml_parser_base
	{
	public:
		xml_element_update_parser( void ){}

	public:
		xml_element_update_parser( document* doc ){ m_document = doc; }

		virtual ~xml_element_update_parser( void ){}

	public:
		virtual object_base* forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object );

	public:
		document* m_document;
	};

	//
	//	xml_element_create_parser
	//
	class USFILTER_EXPORT xml_element_create_parser : public xml_parser_base
	{
	public:
		xml_element_create_parser( void ){}

	public:
		xml_element_create_parser( document* doc ){ m_document = doc; }

		virtual ~xml_element_create_parser( void ){}

	public:
		virtual object_base* forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object );

	public:
		document* m_document;
	};

	//
	//	xml_element_delete_parser
	//
	class USFILTER_EXPORT xml_element_delete_parser : public xml_parser_base
	{
	public:
		xml_element_delete_parser( void ){}

	public:
		xml_element_delete_parser( document* doc ){ m_document = doc; }

		virtual ~xml_element_delete_parser( void ){}

	public:
		virtual object_base* forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object );

	public:
		document* m_document;
	};

	//
	//	xml_element_change_parser
	//
	class USFILTER_EXPORT xml_element_change_parser : public xml_parser_base
	{
	public:
		xml_element_change_parser( void ){}

		virtual ~xml_element_change_parser( void ){}

	public:
		virtual object_base* forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object );

	private:
		void change_element_parameter( object_base* v_container, TiXmlNode* v_xml_node, object_base* elem_object );
	};


	//////////////////////////////////////////////////////////////////////////
	// style parsers
	//////////////////////////////////////////////////////////////////////////

	//
	// icon_style
	//
	class USFILTER_EXPORT icon_style_parser
		: public xml_parser_base
	{
	public:
		icon_style_parser(void){}

		virtual ~icon_style_parser(void){}

	public:
		virtual object_base* forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object );
	};

	//
	// font_style
	//
	class USFILTER_EXPORT font_style_parser
		: public xml_parser_base
	{
	public:
		font_style_parser(void){}

		virtual ~font_style_parser(void){}

	public:
		virtual object_base* forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object );
	};

	//
	// shadow_style
	//
	class USFILTER_EXPORT shadow_style_parser
		: public xml_parser_base
	{
	public:
		shadow_style_parser(void){}

		virtual ~shadow_style_parser(void){}

	public:
		virtual object_base* forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object );
	};


	//
	// render_style
	//
	class USFILTER_EXPORT render_style_parser
		: public xml_parser_base
	{
	public:
		render_style_parser(void){}

		virtual ~render_style_parser(void){}

	public:
		virtual object_base* forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object );

	public:
		static pen_style* parse_pen_style( TiXmlNode* v_xml_node, pen_style* v_render_style );

		static brush_style* parse_brush_style( TiXmlNode* v_xml_node, brush_style* v_render_style );
		
		static icon_style* parse_icon_style( TiXmlNode* v_xml_node, icon_style* v_render_style );

		static font_style* parse_font_style( TiXmlNode* v_xml_node, font_style* v_render_style );

		static shadow_style* parse_shadow_style( TiXmlNode* v_xml_node, shadow_style* v_render_style );
	};

	//
	// spherical_tranform 
	//
	class USFILTER_EXPORT spherical_transform_parser
		: public xml_parser_base
	{
	public:
		spherical_transform_parser(void){}

		virtual ~spherical_transform_parser(void){}

	public:
		virtual object_base* forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object );
	};
}

#endif // _US_XML_ELEMENT_PARSERS_H_