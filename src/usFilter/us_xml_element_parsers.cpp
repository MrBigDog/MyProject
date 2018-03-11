///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_xml_element_parsers.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : xml_element_parsers class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_xml_element_parsers.h"

namespace uniscope_globe
{

	//
	//	xml_element_document_parser
	//
	object_base* xml_element_document_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT || parent_object == NULL )	return NULL;

		document* v_document = (document*)v_container;
		element_folder* v_parent = dynamic_cast<element_folder*>(parent_object);
		if ( v_parent == NULL )
		{
			v_parent = v_document->get_element_root();
		}

		element_folder* v_elem_doc = new element_folder;
		v_elem_doc->initialize( v_document );
		
		TiXmlElement* v_xml_elem = (TiXmlElement*)v_xml_node;
		{
			char* str_temp = NULL;
			//  parse id
			if( (str_temp = (char*)v_xml_elem->Attribute( "id" )) != NULL )
			{
				ustring str_id = string_ext::to_wstring( str_temp );
				v_elem_doc->set_guid( str_id.c_str() );
			}
			// parse name
			if ( (str_temp = (char*)v_xml_elem->Attribute( "name" )) != NULL )
			{
				ustring str_name = string_ext::to_wstring( str_temp );
				v_elem_doc->set_name( str_name.c_str() );
			}
		}

		std::vector<TiXmlElement*> v_element_array;
		bool v_visible = true;
		xml_parser_base* xml_parser = NULL;
		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT )
				continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			string str_label = v_child->Value();

			if ( str_label == "Visible" )
			{
				v_visible = string_ext::to_bool( v_elem->GetText() );
				v_elem_doc->set_visible( v_visible );
			}
			else if ( str_label == "RenderStyle" )
			{
				xml_parser = m_parser_mngr->get_parser( string_ext::to_wstring( str_label ).c_str() );
				if ( xml_parser )
				{
					xml_parser->forward_parse( v_container, v_elem, v_elem_doc ); 
				}
			}
			else
			{
				v_element_array.push_back( v_elem );
			}
		}

		// parse element node
		for ( int i = 0; i < (int)v_element_array.size(); i++ )
		{
			xml_parser = m_parser_mngr->get_parser( string_ext::to_wstring( v_element_array[i]->Value() ).c_str() );
			if ( xml_parser ) 
			{
				xml_parser->forward_parse( v_document, v_element_array[i], v_elem_doc );
			}
		}

		////v_document->register_object( v_elem_doc );
		v_parent->attach_object( v_elem_doc );

		return v_elem_doc;
	}

	//////////////////////////////////////////////////////////////////////////
	object_base* xml_element_folder_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT )	return NULL;

		document* v_document = (document*)v_container;

		element_folder* v_parent = dynamic_cast<element_folder*>(parent_object);
		if ( v_parent == NULL )
		{
			v_parent = v_document->get_element_root();
		}

		element_folder* v_folder = new element_folder;
		v_folder->initialize( v_document );

		TiXmlElement* v_xml_elem = (TiXmlElement*)v_xml_node;
		{
			char* str_temp = NULL;
			//  parse id
			if( (str_temp = (char*)v_xml_elem->Attribute( "id" )) != NULL )
			{
				ustring str_id = string_ext::to_wstring( str_temp );
				v_folder->set_guid( str_id.c_str() );
			}
			// parse name
			if ( (str_temp = (char*)v_xml_elem->Attribute( "name" )) != NULL )
			{
				ustring str_name = string_ext::to_wstring( str_temp );
				v_folder->set_name( str_name.c_str() );
			}
		}

		std::vector<TiXmlElement*> v_element_array;
		bool v_visible = true;
		xml_parser_base* xml_parser = NULL;
		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT )
				continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			string str_label = v_child->Value();

			if ( str_label == "Visible" )
			{
				v_visible = string_ext::to_bool( v_elem->GetText() );
			}
			else if ( str_label == "RenderStyle" )
			{
				xml_parser = m_parser_mngr->get_parser( string_ext::to_wstring( str_label ).c_str() );
				if ( xml_parser )
				{
					xml_parser->forward_parse( v_container, v_elem, v_folder ); 
				}
			}
			else
			{
				v_element_array.push_back( v_elem );
			}
		}

		// parse element node
		for ( int i = 0; i < (int)v_element_array.size(); i++ )
		{
			xml_parser = m_parser_mngr->get_parser( string_ext::to_wstring( v_element_array[i]->Value() ).c_str() );
			if ( xml_parser ) 
			{
				xml_parser->forward_parse( v_document, v_element_array[i], v_folder );
			}
		}

		v_folder->set_visible( v_visible );

		//v_document->register_object( v_folder );
		v_parent->attach_object( v_folder );

		return v_folder;
	}

	//////////////////////////////////////////////////////////////////////////
	object_base* xml_element_ground_overlay_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT ) return NULL;

		document* v_document = (document*)v_container;
		element_folder* v_parent = dynamic_cast<element_folder*>(parent_object);
		if ( v_parent == NULL )
		{
			v_parent = v_document->get_element_root();
		}

		lon_lat_rect v_rect;

		double v_rotation_angle;
		bool v_visible = true;
		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT )
				continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			ustring str_label = string_ext::to_wstring( v_child->Value() );

			if ( str_label == L"LonLatRect" )
			{
				v_rect = xml_parser_ext::to_lon_lat_rect( v_elem->GetText() );
			}
			else if( str_label == L"Angle" )
			{
				v_rotation_angle = string_ext::to_double( v_elem->GetText() );
			}
			else if ( str_label == L"Visible" )
			{
				v_visible = string_ext::to_bool( v_elem->GetText() );
			}
		}

		return NULL;
	}

	// element_icon
	object_base* xml_element_icon_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT ) return NULL;

		document* v_document = (document*)v_container;

		element_folder* v_parent = dynamic_cast<element_folder*>(parent_object);
		if ( v_parent == NULL )
		{
			v_parent = v_document->get_element_root();
		}

		element_icon* v_icon = new element_icon( v_document );
		//v_icon->initialize( v_document );

		// parse attribute
		TiXmlElement *v_elem = (TiXmlElement *)v_xml_node;
		{
			char* str_temp = NULL;

			// parse id
			if( (str_temp = (char*)v_elem->Attribute( "id" )) != NULL )
			{
				ustring str_id = string_ext::to_wstring( str_temp );
				v_icon->set_guid( str_id.c_str() );
			}
			//  parse name
			if( (str_temp = (char*)v_elem->Attribute( "name" )) != NULL )
			{
				ustring str_name = string_ext::to_wstring( str_temp );
				v_icon->set_name( str_name.c_str() );
			}
		}

		vector_3d v_location;
		bool v_visible = true;

		xml_parser_base* v_parser = NULL;
		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT ) continue;

			TiXmlElement* v_sub_elem = (TiXmlElement*)v_child;

			string str_label = v_child->Value();

			if ( str_label == "RenderStyle" )
			{
				xml_parser_base* xml_parser = m_parser_mngr->get_parser( string_ext::to_wstring( str_label ).c_str() );
				if ( xml_parser )
				{
					xml_parser->forward_parse( v_container, v_sub_elem, v_icon->get_render_style() ); 
				}
			}
			else if ( str_label == "Text" )
			{
				char* str_text = (char*)(v_sub_elem->GetText());

				if ( str_text != NULL )
				{
					v_icon->set_text( string_ext::to_wstring( str_text ).c_str() );
				}
			}
			else if ( str_label == "Location" )
			{
				v_location = xml_parser_ext::to_vector_3d( v_sub_elem->GetText() );
			}
			else if ( str_label == "Visible" )
			{
				v_visible = string_ext::to_bool( v_sub_elem->GetText() );
			}
		}

		v_location.x *= math_d::s_deg_to_rad;
		v_location.y *= math_d::s_deg_to_rad;
		v_location.z = v_location.z * 0.001 + spherical_coords::s_semi_major;
		v_icon->create( v_location );
		v_icon->set_visible( v_visible );

		//v_document->register_object( v_icon );
		v_parent->attach_object( v_icon );

		return v_icon;
	};
	//////////////////////////////////////////////////////////////////////////
	object_base* xml_element_points_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT ) return NULL;

		document* v_document = (document*)v_container;
		element_folder* v_parent = dynamic_cast<element_folder*>(parent_object);
		if ( v_parent == NULL )
		{
			v_parent = v_document->get_element_root();
		}


		element_points* v_points = new element_points( v_document );
		//v_points->initialize( v_document );

		ustring str_id;
		// parse attribute
		TiXmlElement *v_elem = (TiXmlElement *)v_xml_node;
		{
			char* str_temp = NULL;

			//  parse id
			if( (str_temp = (char*)v_elem->Attribute( "id" )) != NULL )
			{
				str_id = string_ext::to_wstring( str_temp );
				v_points->set_guid( str_id.c_str() );
			}

			//  parse name
			if( (str_temp = (char*)v_elem->Attribute( "name" )) != NULL )
			{
				ustring str_name = string_ext::to_wstring( str_temp );
				v_points->set_name( str_name.c_str() );
			}

			// parse point size
			if( (str_temp = (char*)v_elem->Attribute( "point_size" )) != NULL )
			{
				int v_point_size = string_ext::to_int( str_temp );
			}
		}

		// parse coordinates
		point_list_d v_point_list;
		bool v_visible = true;
		pen_style* v_pen_style = NULL;

		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT ) continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			string str_label = v_child->Value();

			if ( str_label == "RenderStyle" )
			{
				v_pen_style = render_style_parser::parse_pen_style( v_elem, NULL );
			}
			else if ( str_label == "Coordinates" )
			{
				const char* temp_char = v_elem->GetText();
				if( temp_char != NULL )
				{
					v_point_list = xml_parser_ext::to_point_list_d( temp_char );
				}
			}
			else if ( str_label == "Visible" )
			{
				v_visible = string_ext::to_bool( v_elem->GetText() );
			}
		}

		for ( int i = 0; i < (int)v_point_list.size(); i++ )
		{
			v_point_list[i].x *= math_d::s_deg_to_rad;
			v_point_list[i].y *= math_d::s_deg_to_rad;
			v_point_list[i].z = v_point_list[i].z * 0.001 + spherical_coords::s_semi_major;
		}

		v_points->set_render_style( v_pen_style );
		v_points->create_from_coordinates( v_point_list );
		v_points->set_visible( v_visible );

		if ( str_id.size() > 0 )
		{
			//v_document->register_object( v_points );
		}
		v_parent->attach_object( v_points );

		return v_points;
	}

	//////////////////////////////////////////////////////////////////////////
	object_base* xml_element_line_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT ) return NULL;

		document* v_document = (document*)v_container;
		element_folder* v_parent = dynamic_cast<element_folder*>(parent_object);
		if ( v_parent == NULL )
		{
			v_parent = v_document->get_element_root();
		}

		// create line
		element_line* v_element_line = new element_line( v_document );

		// parse attribute
		ustring str_id, str_name;
		TiXmlElement *v_elem = (TiXmlElement *)v_xml_node;
		{
			char* str_temp = NULL;

			//  parse id
			if( (str_temp = (char*)v_elem->Attribute( "id" )) != NULL )
			{
				str_id = string_ext::to_wstring( str_temp );
			}

			if( (str_temp = (char*)v_elem->Attribute( "name" )) != NULL )
			{
				str_name = string_ext::to_wstring( str_temp );
			}
		}

		// parse coordinates
		bool v_visible = true;
		double v_geo_line_width = -1.0;
		point_list_d v_point_list;
		brush_style* v_render_style = NULL;
		shadow_style* v_shadow_style = NULL;

		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT ) continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			string str_label = v_child->Value();

			if ( str_label == "RenderStyle" )
			{
				v_render_style = render_style_parser::parse_brush_style( v_elem, NULL );

				TiXmlElement* v_shadow_elem = (TiXmlElement*)(v_elem->FirstChild("ShadowStyle"));
				if ( v_shadow_elem )
				{
					v_shadow_style = render_style_parser::parse_shadow_style( v_elem, NULL );
					v_render_style->m_line_color = v_shadow_style->m_shadow_color;
					v_element_line->m_render_type = v_shadow_style->m_shadow_type;
				}
				//v_render_style->m_style_array.push_back( v_shadow_style );
			}
			else if ( str_label == "RenderType" )
			{
				char* v_render_type = (char*)(v_elem->GetText());
				if ( v_render_type != NULL )
				{
					string str_render_type = v_render_type;
					US_STRING_LOWER( str_render_type );
					if ( str_render_type == "thin" )
					{
						v_element_line->m_render_type = 0;
					}
					else 
					{
						v_element_line->m_render_type = 3;
					}
				}
			}
			else if ( str_label == "GeoLineWidth" )
			{
				v_geo_line_width = string_ext::to_double( v_elem->GetText() ) * 0.001;
			}
			else if ( str_label == "Coordinates" )
			{
				const char* temp_char = v_elem->GetText();
				if( temp_char != NULL )
				{
					v_point_list = xml_parser_ext::to_point_list_d( temp_char );
				}
			}
			else if ( str_label == "Visible" )
			{
				v_visible = string_ext::to_bool( v_elem->GetText() );
			}
		}

		// set parameter
		//v_element_line->initialize( v_document );
		v_element_line->set_guid( str_id.c_str() );
		v_element_line->set_name( str_name.c_str() );
		v_element_line->set_visible( v_visible );
		v_element_line->set_render_style( v_render_style );
				
		// 变换坐标
		v_element_line->m_spherical_coords.resize( v_point_list.size() );

		for ( int ni = 0; ni < (int)v_point_list.size(); ni++ )
		{
			v_element_line->m_spherical_coords[ni].x = v_point_list[ni].x * math_d::s_deg_to_rad;
			v_element_line->m_spherical_coords[ni].y = v_point_list[ni].y * math_d::s_deg_to_rad;
			v_element_line->m_spherical_coords[ni].z = v_point_list[ni].z * 0.001 + spherical_coords::s_semi_major;
		}

		v_element_line->create();

		if ( str_id.size() > 0 )
		{
			//v_document->register_object( v_element_line );
		}
		v_parent->attach_object( v_element_line );

		return v_element_line;

	}

	//////////////////////////////////////////////////////////////////////////
	object_base* xml_element_polygon_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT ) return NULL;

		document* v_document = (document*)v_container;
		element_folder* v_parent = dynamic_cast<element_folder*>(parent_object);
		if ( v_parent == NULL )
		{
			v_parent = v_document->get_element_root();
		}

		element_polygon* v_element_poly = new element_polygon( v_document );

		// parse attribute
		ustring str_id;
		ustring str_name;

		TiXmlElement *v_elem = (TiXmlElement *)v_xml_node;
		{
			char* str_temp = NULL;
			//  parse id
			if ( (str_temp = (char*)v_elem->Attribute( "id" )) != NULL )
			{
				str_id = string_ext::to_wstring( str_temp );
			}
			if ( (str_temp = (char*)v_elem->Attribute( "name" )) != NULL )
			{
				str_name = string_ext::to_wstring( str_temp );
			}
		}

		// parse coordinates
		bool v_visible = true;
		double v_geo_line_width = -1.0;
		point_list_d v_point_list;
		brush_style* v_render_style = NULL;
		shadow_style* v_shadow_style = NULL;

		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT ) continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;
			string str_label = v_child->Value();

			if ( str_label == "RenderStyle" )
			{
				v_render_style = render_style_parser::parse_brush_style( v_elem, NULL );
				TiXmlElement* v_shadow_elem = (TiXmlElement*)(v_elem->FirstChild("ShadowStyle"));
				if ( v_shadow_elem )
				{
					v_shadow_style = render_style_parser::parse_shadow_style( v_elem, NULL );
				
					v_render_style->m_fill_color = v_shadow_style->m_shadow_color;
					v_element_poly->m_render_type = v_shadow_style->m_shadow_type;
				}
				//v_render_style->m_style_array.push_back( v_shadow_style );
			}
			else if ( str_label == "RenderType" )
			{
				char* v_render_type = (char*)(v_elem->GetText());
				if ( v_render_type != NULL )
				{
					string str_render_type = v_render_type;
					US_STRING_LOWER( str_render_type );
					if ( str_render_type == "thin" )
					{
						v_element_poly->m_render_type = 0;
					}
					else 
					{
						v_element_poly->m_render_type = 3;
					}
				}
			}
			else if ( str_label == "GeoLineWidth" )
			{
				v_geo_line_width = string_ext::to_double( v_elem->GetText() ) * 0.001;
			}
			else if ( str_label == "Coordinates" )
			{
				const char* temp_char = v_elem->GetText();
				if( temp_char != NULL )
				{
					v_point_list = xml_parser_ext::to_point_list_d( temp_char );
				}
			}
			else if ( str_label == "Visible" )
			{
				v_visible = string_ext::to_bool( v_elem->GetText() );
			}
		}

		// set parameter
		//v_element_poly->initialize( v_document );
		v_element_poly->set_guid( str_id.c_str() );
		v_element_poly->set_name( str_name.c_str() );
		v_element_poly->set_visible( v_visible );
		v_element_poly->set_render_style( v_render_style );
		
		// 变换坐标
		v_element_poly->m_spherical_coords.resize( v_point_list.size() );

		for ( int ni = 0; ni < (int)v_point_list.size(); ni++ )
		{
			v_element_poly->m_spherical_coords[ni].x = v_point_list[ni].x * math_d::s_deg_to_rad;
			v_element_poly->m_spherical_coords[ni].y = v_point_list[ni].y * math_d::s_deg_to_rad;
			v_element_poly->m_spherical_coords[ni].z = v_point_list[ni].z * 0.001 + spherical_coords::s_semi_major;
		}

		v_element_poly->create();

		if ( str_id.size() > 0 )
		{
			//v_document->register_object( v_element_poly );
		}
		v_parent->attach_object( v_element_poly );

		return v_element_poly;
	}


	//////////////////////////////////////////////////////////////////////////
	object_base* xml_element_circle_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		//	if ( v_xml_node->Type() != TiXmlNode::ELEMENT ) return NULL;

		//	document* v_document = (document*)v_container;
		//	element_folder* v_parent = dynamic_cast<element_folder*>(parent_object);
		//	if ( v_parent == NULL )
		//	{
		//		v_parent = v_document->get_element_root();
		//	}

		//	// parse attribute
		//
		//	ustring str_id, str_name;
		//	TiXmlElement *v_elem = (TiXmlElement *)v_xml_node;
		//	{
		//		char* str_temp = NULL;

		//		//  parse id
		//		if( (str_temp = (char*)v_elem->Attribute( "id" )) != NULL )
		//		{
		//			str_id = string_ext::to_wstring( str_temp );
		//		}
		//		if ( (str_temp = (char*)v_elem->Attribute( "name" )) != NULL )
		//		{
		//			str_name = string_ext::to_wstring( str_temp );
		//		}
		//	}

		//	double v_radius = 0;
		//	vector3<double> v_center;
		//	bool v_visible = true;
		//	render_style* v_render_style = NULL;

		//	for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		//	{
		//		if ( v_child->Type() != TiXmlNode::ELEMENT )
		//			continue;

		//		TiXmlElement* v_elem = (TiXmlElement*)v_child;

		//		ustring str_label = string_ext::to_wstring( v_child->Value() );

		//		if ( str_label == L"Radius" )
		//		{
		//			v_radius = string_ext::to_double( v_elem->GetText() );
		//		}
		//		else if ( str_label == L"Position" )
		//		{
		//			v_center = xml_parser_ext::to_vector_3d( v_elem->GetText() );
		//		}
		//		else if ( str_label == L"Visible" )
		//		{
		//			v_visible = string_ext::to_bool( v_elem->GetText() );
		//		}
		//		if ( str_label == "RenderStyle" )
		//		{
		//			xml_parser_base* xml_parser = m_parser_mngr->get_parser( L"RenderStyle" );
		//			if ( xml_parser )
		//			{
		//				v_render_style = (render_style*)xml_parser->forward_parse( NULL, v_elem, NULL ); 
		//			}
		//		}
		//	}
		//	
		//	int v_segment = 32;
		//	std::vector<vector3<double>> v_corner_points;
		//	v_corner_points.resize( v_segment );

		//	vector3<double> v_center_point_geo = geocentric_coords::from_spherical_d( v_center );
		//	vector3<double> v_direction = vector3<double>::normalize( v_center_point_geo );
		//	vector3<double> v_rigth = v_direction.cross( vector3<double>::s_unit_y );
		//	matrix4<double> mat_rotate = matrix4<double>::rotate_axis_lh( vector3<double>::s_zero, vector3<double>::s_zero + v_rigth, v_radius);
		//	vector3<double> v_north_geo = matrix4<double>::transform_coordinate( v_center_point_geo, mat_rotate);

		//	for( int ni = 0; ni < v_segment; ni++ )
		//	{
		//		matrix4<double> mat_rotate = matrix4<double>::rotate_axis_lh( vector3<double>::s_zero, vector3<double>::s_zero + v_center_point_geo, math<double>::s_two_pi / v_segment * ni );
		//		v_corner_points[ni] = matrix4<double>::transform_coordinate( v_north_geo, mat_rotate );
		//	}
		//	
		//	// create object
		//	element_mesh* v_element_mesh = new element_mesh;
		//	v_element_mesh->set_document( v_document );
		//	v_element_mesh->set_visible( v_visible );
		//	v_element_mesh->set_guid( str_id.c_str() );
		//	v_element_mesh->set_name( str_name.c_str() );
		//	v_element_mesh->set_render_style( v_render_style );

		//	lon_lat_region v_region;
		//	for ( int i = 0 ; i < v_corner_points.size(); i++ )
		//	{
		//		v_region.push_back( v_corner_points[i] );
		//		v_corner_points[i] = geocentric_coords::from_spherical_d( v_corner_points[i] );
		//	}
		//	lon_lat_rect v_rect = v_region.get_bound();

		//	double v_height = 1.5 * spherical_coords::s_semi_major * MAX( v_rect.get_width(), v_rect.get_height() ) * 0.5 / math<double>::tan_( 27 * RADIAN );

		//	geometry_mesh* v_mesh = new geometry_mesh;
		//	render_object_factory::create_mesh_from_polygon( v_mesh, v_corner_points[0],
		//												v_corner_points.size(),
		//												v_render_style->get_fill_color(),
		//												max( v_height, 8.5 ) );
		//	v_element_mesh->create( v_mesh );

		//	if ( str_id.c_str() )
		//	{
		//		//v_document->register_object( v_element_mesh );
		//	}
		//	v_parent->add_child( v_element_mesh );

		//	return v_element_mesh;

		return NULL;
	}


	//////////////////////////////////////////////////////////////////////////
	object_base* xml_element_box_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		//if ( v_xml_node->Type() != TiXmlNode::ELEMENT ) return NULL;

		//document* v_document = (document*)v_container;
		//element_folder* v_parent = dynamic_cast<element_folder*>(parent_object);
		//if ( v_parent == NULL )
		//{
		//	v_parent = v_document->get_element_root();
		//}

		//// parse attribute
		//dword v_color = 0xccff00ff;
		//double v_line_width = 2;
		//double v_line_height = 15000;
		//int v_shadow_cast = 0;
		//ustring str_id;
		//ustring str_name;

		//TiXmlElement *v_elem = (TiXmlElement *)v_xml_node;
		//{
		//	char* str_temp = NULL;
		//	//  parse id
		//	if ( (str_temp = (char*)v_elem->Attribute( "id" )) != NULL )
		//	{
		//		str_id = string_ext::to_wstring( str_temp );
		//	}
		//	if ( (str_temp = (char*)v_elem->Attribute( "name" )) != NULL )
		//	{
		//		str_name = string_ext::to_wstring( str_temp );
		//	}
		//	if ( (str_temp = (char*)v_elem->Attribute( "color" )) != NULL )
		//	{
		//		v_color = string_ext::to_dword( str_temp );
		//	}
		//	if ( (str_temp = (char*)v_elem->Attribute( "line_width" )) != NULL )
		//	{
		//		v_line_width = string_ext::to_double( str_temp );
		//	}
		//	if ( (str_temp = (char*)v_elem->Attribute( "shadow_cast" )) != NULL )
		//	{
		//		v_shadow_cast = string_ext::to_int( str_temp );
		//	}
		//	if ( (str_temp = (char*)v_elem->Attribute( "line_height" )) != NULL )
		//	{
		//		v_line_height = string_ext::to_double( str_temp );
		//	}
		//}

		//// parse coordinates
		//lon_lat_box v_box;
		//bool v_visible = true;
		//TiXmlElement* v_style_elem = NULL;

		//xml_parser_base* v_parser = NULL;
		//for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		//{
		//	if ( v_child->Type() != TiXmlNode::ELEMENT ) continue;

		//	TiXmlElement* v_elem = (TiXmlElement*)v_child;
		//	ustring str_label = string_ext::to_wstring( v_child->Value() );

		//	if ( str_label == L"RenderStyle" )
		//	{
		//		v_style_elem = v_elem;
		//	}
		//	else if ( str_label == L"LonLatBox" )
		//	{
		//		v_box = xml_parser_ext::to_lon_lat_box( v_elem->GetText() );
		//	}
		//	else if ( str_label == L"Visible" )
		//	{
		//		v_visible = string_ext::to_bool( v_elem->GetText() );
		//	}
		//}

		//element_mesh* v_element_mesh = new element_mesh;
		//v_element_mesh->set_document( v_document );
		//if ( v_style_elem == NULL )
		//{
		//	render_style* v_style = new render_style( v_parent->get_render_style() );
		//	v_style->m_line_color = v_color;
		//	v_style->m_fill_color = v_color;
		//	v_element_mesh->set_render_style( v_style );
		//}

		//// create object
		//v_element_mesh->set_guid( str_id.c_str() );
		//v_element_mesh->set_name( str_name.c_str() );
		//v_element_mesh->set_shadow_flag( v_shadow_cast );
		//v_element_mesh->set_visible( v_visible );

		//render_style* v_render_style = v_element_mesh->get_render_style();

		//geometry_mesh* v_mesh = new geometry_mesh;
		//render_object_factory::create_mesh_from_lon_lat_box( v_mesh,  v_box, v_render_style->m_line_color, v_render_style->m_fill_color );		
		//v_element_mesh->create( v_mesh );

		////v_document->register_object( v_element_mesh );
		//v_parent->add_child( v_element_mesh );

		return NULL;
	}


	//////////////////////////////////////////////////////////////////////////
	object_base* xml_element_sphere_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		//if ( v_xml_node->Type() != TiXmlNode::ELEMENT ) return NULL;

		//document* v_document = (document*)v_container;
		//element_folder* v_parent = dynamic_cast<element_folder*>(parent_object);
		//if ( v_parent == NULL )
		//{
		//	v_parent = v_document->get_element_root();
		//}

		//// parse attribute
		//dword v_color = 0xffffffff;
		//int v_shadow_cast = 0;
		//ustring str_id;
		//int v_lon_segment = 16;
		//int v_lat_segment = 16;


		//TiXmlElement *v_elem = (TiXmlElement *)v_xml_node;
		//{
		//	char* str_temp = NULL;

		//	//  parse id
		//	if( (str_temp = (char*)v_elem->Attribute( "id" )) != NULL )
		//	{
		//		str_id = string_ext::to_wstring( str_temp );
		//	}

		//	if( (str_temp = (char*)v_elem->Attribute( "color" )) != NULL )
		//	{
		//		v_color = string_ext::to_dword( str_temp );
		//	}

		//	if( (str_temp = (char*)v_elem->Attribute( "shadow_cast" )) != NULL )
		//	{
		//		v_shadow_cast = string_ext::to_int( str_temp );
		//	}

		//	if( (str_temp = (char*)v_elem->Attribute( "lon_segment" )) != NULL )
		//	{
		//		v_lon_segment = string_ext::to_int( str_temp );
		//	}

		//	if( (str_temp = (char*)v_elem->Attribute( "lat_segment" )) != NULL )
		//	{
		//		v_lat_segment = string_ext::to_int( str_temp );
		//	}
		//}

		//double v_radius = 0;
		//vector3<double> v_center;
		//bool v_visible = true;

		//for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		//{
		//	if ( v_child->Type() != TiXmlNode::ELEMENT )
		//		continue;

		//	TiXmlElement* v_elem = (TiXmlElement*)v_child;

		//	ustring str_label = string_ext::to_wstring( v_child->Value() );

		//	if ( str_label == L"Radius" )
		//	{
		//		v_radius = string_ext::to_double( v_elem->GetText() );
		//	}
		//	else if ( str_label == L"Position" )
		//	{
		//		v_center = xml_parser_ext::to_vector_3d( v_elem->GetText() );
		//	}
		//	else if ( str_label == L"Visible" )
		//	{
		//		v_visible = string_ext::to_bool( v_elem->GetText() );
		//	}
		//}

		//if( v_lon_segment < 4 || v_lat_segment < 4 ) return NULL;

		return NULL;
	}


	//////////////////////////////////////////////////////////////////////////
	object_base* xml_element_rectangle_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		//if ( v_xml_node->Type() != TiXmlNode::ELEMENT ) return NULL;

		//document* v_document = (document*)v_container;
		//element_folder* v_parent = dynamic_cast<element_folder*>(parent_object);
		//if ( v_parent == NULL )
		//{
		//	v_parent = v_document->get_element_root();
		//}

		//// parse attribute
		//dword v_color = 0xffffffff;
		//int v_shadow_cast = 0;
		//ustring str_id;

		//TiXmlElement *v_elem = (TiXmlElement *)v_xml_node;
		//{
		//	char* str_temp = NULL;

		//	//  parse id
		//	if( (str_temp = (char*)v_elem->Attribute( "id" )) != NULL )
		//	{
		//		str_id = string_ext::to_wstring( str_temp );
		//	}

		//	if( (str_temp = (char*)v_elem->Attribute( "color" )) != NULL )
		//	{
		//		v_color = string_ext::to_dword( str_temp );
		//	}


		//	if( (str_temp = (char*)v_elem->Attribute( "shadow_cast" )) != NULL )
		//	{
		//		v_shadow_cast = string_ext::to_int( str_temp );
		//	}
		//}

		//// parse coordinates
		//point_list_d v_point_list;
		//bool v_visible = true;
		//for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		//{
		//	if ( v_child->Type() != TiXmlNode::ELEMENT ) continue;

		//	TiXmlElement* v_elem = (TiXmlElement*)v_child;

		//	ustring str_label = string_ext::to_wstring( v_child->Value() );

		//	if ( str_label == L"Coordinates" )
		//	{
		//		const char* temp_char = v_elem->GetText();
		//		if( temp_char != NULL )
		//		{
		//			v_point_list = xml_parser_ext::to_point_list_d( temp_char );
		//		}

		//	}
		//	else if ( str_label == L"Visible" )
		//	{
		//		v_visible = string_ext::to_bool( v_elem->GetText() );
		//	}
		//}

		//if( v_point_list.size() < 2) return NULL;
		//vector3<double>	v_corner_points[5];
		//v_corner_points[0]   = v_point_list[0];
		//v_corner_points[1]	 = v_point_list[0];
		//v_corner_points[1].x = v_point_list[1].x;
		//v_corner_points[2]   = v_point_list[1];
		//v_corner_points[3]   = v_point_list[1];
		//v_corner_points[3].x = v_point_list[0].x;

		//for( ulong ni = 0; ni < 4; ni++ )
		//{
		//	v_corner_points[ni] = geocentric_coords::from_spherical_d( v_corner_points[ni] );
		//}

		//v_corner_points[4] = v_corner_points[0];

		//// create object
		//element_mesh* v_element_mesh = new element_mesh;
		//v_element_mesh->set_document( v_document );
		//
		//v_element_mesh->set_guid( str_id.c_str() );
		//v_element_mesh->set_shadow_flag( v_shadow_cast );
		//v_element_mesh->set_visible( v_visible );

		//render_style* v_render_style = v_element_mesh->get_render_style();
		//v_render_style->m_fill_color = v_color;

		//lon_lat_region v_region;
		//for ( int i = 0 ; i < 5; i++ )
		//{
		//	v_region.push_back( v_corner_points[i] );
		//	v_corner_points[i] = geocentric_coords::from_spherical_d( v_corner_points[i] );
		//}
		//lon_lat_rect v_rect = v_region.get_bound();

		//double v_height = 8.5;
		//v_height = max( v_height, 1.5 * spherical_coords::s_semi_major * MAX( v_rect.get_width(), v_rect.get_height() ) * 0.5 / math<double>::tan_( 27 * RADIAN ) );

		//geometry_mesh* v_mesh = new geometry_mesh;
		//render_object_factory::create_mesh_from_polygon( v_mesh, v_corner_points, 5, v_color );
		//v_element_mesh->create( v_mesh );
		//
		////v_document->register_object( v_element_mesh );
		//v_parent->add_child( v_element_mesh );

		return NULL;
	}


	//////////////////////////////////////////////////////////////////////////
	object_base* xml_element_text_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		//if ( v_xml_node->Type() != TiXmlNode::ELEMENT ) return NULL;

		//document* v_document = (document*)v_container;
		//element_folder* v_parent = dynamic_cast<element_folder*>(parent_object);
		//if ( v_parent == NULL )
		//{
		//	v_parent = v_document->get_element_root();
		//}

		//element_points* v_points = new element_points;
		//v_points->set_document( v_document );

		//// parse attribute
		//dword v_color = 0xffffffff;
		//TiXmlElement *v_elem = (TiXmlElement *)v_xml_node;
		//{
		//	char* str_temp = NULL;

		//	//  parse id
		//	if( (str_temp = (char*)v_elem->Attribute( "id" )) != NULL )
		//	{
		//		ustring str_id = string_ext::to_wstring( str_temp );
		//		v_points->set_guid( str_id.c_str() );
		//	}
		//	//  parse id
		//	if( (str_temp = (char*)v_elem->Attribute( "name" )) != NULL )
		//	{
		//		ustring str_name = string_ext::to_wstring( str_temp );
		//		v_points->set_name( str_name.c_str() );
		//	}
		//	// parse color
		//	if( (str_temp = (char*)v_elem->Attribute( "color" )) != NULL )
		//	{
		//		v_color = string_ext::to_dword( str_temp );
		//	}
		//}

		//// parse coordinates
		//point_list_d v_point_list;
		//spherical_pose* v_s_p = NULL;
		//bool v_visible = true;

		//xml_parser_base* v_parser = NULL;
		//for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		//{
		//	if ( v_child->Type() != TiXmlNode::ELEMENT ) continue;

		//	TiXmlElement* v_elem = (TiXmlElement*)v_child;

		//	ustring str_label = string_ext::to_wstring( v_child->Value() );

		//	if ( str_label == L"Coordinates" )
		//	{
		//		const char* temp_char = v_elem->GetText();
		//		if( temp_char != NULL )
		//		{
		//			v_point_list = xml_parser_ext::to_point_list_d( temp_char );
		//		}
		//	}
		//	else if ( str_label == L"Visible" )
		//	{
		//		v_visible = string_ext::to_bool( v_elem->GetText() );
		//	}
		//}

		//v_points->create_from_coordinates( v_point_list );
		//v_points->set_visible( v_visible );
		////v_document->register_object( v_points );
		//v_parent->add_child( v_points );

		return NULL;
	}

	object_base* xml_element_mesh_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT ) return NULL;

		document* v_document = (document*)v_container;
		element_folder* v_parent = dynamic_cast<element_folder*>(parent_object);
		if ( v_parent == NULL )
		{
			v_parent = v_document->get_element_root();
		}

		// create object
		element_mesh* v_element_mesh = new element_mesh(v_document);
		//v_element_mesh->initialize( v_document );

		// parse attribute
		ustring str_id, str_name;
		TiXmlElement *v_elem = (TiXmlElement *)v_xml_node;
		{
			char* str_temp = NULL;

			//  parse id
			if( (str_temp = (char*)v_elem->Attribute( "id" )) != NULL )
			{
				str_id = string_ext::to_wstring( str_temp );
			}
			if( (str_temp = (char*)v_elem->Attribute( "name" )) != NULL )
			{
				str_name = string_ext::to_wstring( str_temp );
			}
			
		}
		v_element_mesh->set_guid( str_id.c_str() );
		v_element_mesh->set_name( str_name.c_str() );

		bool v_visible = true;
		vector_3d vec_location( 0.0, 0.0, spherical_coords::s_semi_major );
		vector_3d vec_scaling( 1.0, 1.0, 1.0 );
		vector_3d vec_rotation( 0.0, math_d::s_half_pi, 0.0 );
		matrix_4d v_transform_mat = matrix_4d::s_identity;
		int v_tranform_type = 0;

		render_object_geometry_mesh* v_mesh = NULL;
		TiXmlElement* v_volume = NULL;
		brush_style* v_render_style = NULL;
		shadow_style* v_shadow_style = NULL;

		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT ) continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			string str_label = v_child->Value();

			if ( str_label == "Visible" )
			{
				v_visible = string_ext::to_bool( v_elem->GetText() );
				v_element_mesh->set_visible( v_visible );
			}
			else if ( str_label == "RenderStyle" )
			{
				v_render_style = render_style_parser::parse_brush_style( v_elem, NULL );
				v_shadow_style = render_style_parser::parse_shadow_style( v_elem, NULL );
				v_render_style->m_style_array.push_back( v_shadow_style );
				//xml_parser_base* xml_parser = m_parser_mngr->get_parser( string_ext::to_wstring( str_label ).c_str() );
				//if ( xml_parser )
				//{
				//	xml_parser->forward_parse( v_container, v_elem, v_element_mesh->get_render_style() ); 
				//}
			}
			else if ( str_label == "SphericalTransform" )
			{
				spherical_transform_parser v_parser;
				v_parser.forward_parse( NULL, v_elem, v_element_mesh );
			}
			else if ( str_label == "TransformMatrix" )
			{
				double_list v_list = xml_parser_ext::to_double_array( v_elem->GetText() );
				for( int i = 0; i < 16; i++ )
					v_transform_mat.m[i] = v_list[i];

				v_element_mesh->set_matrix( v_transform_mat );
			}
			else if ( str_label == "Box" )
			{
				v_volume = v_elem;
			}
			else if ( str_label == "Sphere" )
			{
				v_volume = v_elem;
			}
			else if ( str_label == "Volume" )
			{
				v_volume = v_elem;
			}
			else if ( str_label == "Cylinder" )
			{
				v_volume = v_elem;
			}
			else if ( str_label == "Cone" )
			{
				v_volume = v_elem;
			}
			else if ( str_label == "Prism" )
			{
				v_volume = v_elem;
			}
			else if ( str_label == "Pyramid" )
			{	
				v_volume = v_elem;
			}
			else if ( str_label == "Mesh" )
			{	
				v_volume = v_elem;
			}
		}
		
		v_element_mesh->set_render_style( (style_base*)v_render_style );
		// 解析geometry_mesh
		{ 
			string str_label = v_volume->Value();
			brush_style* v_style = (brush_style*)(v_element_mesh->get_render_style());
			v_shadow_style = (shadow_style*)(v_style->m_style_array[0]);

			if ( str_label == "Box" )
			{
				double v_length = 0.0, v_width = 0.0, v_height = 0.0;
				parse_box( v_volume, v_length, v_width, v_height);
				v_mesh = render_object_factory::create_mesh_from_box( v_width, v_height, v_length, v_style->m_line_color, v_style->m_fill_color );
			}
			else if ( str_label == "Sphere" )
			{
				double v_radius = 0.0;
				parse_sphere( v_volume, v_radius );
				v_mesh = render_object_factory::create_mesh_from_sphere( v_radius, v_style->m_line_color, v_style->m_fill_color );
			}
			else if ( str_label == "Volume" )
			{
				point_list_d pt_list;
				double v_height = 0.0;
				parse_volume( v_volume, pt_list, v_height );
				v_mesh = render_object_factory::create_mesh_from_volume( &pt_list[0], pt_list.size(), v_height, v_style->m_line_color, v_style->m_fill_color );
			}
			else if ( str_label == "Cylinder" )
			{
				double v_radius = 0.0, v_height = 0.0;
				parse_cylinder( v_volume, v_radius, v_height );
				v_mesh = render_object_factory::create_mesh_from_cylinder( 32, v_radius, v_height, v_style->m_line_color, v_style->m_fill_color );
			}
			else if ( str_label == "Cone" )
			{
				double v_radius = 0.0, v_height = 0.0;
				parse_cone( v_volume, v_radius, v_height );
				v_mesh = render_object_factory::create_mesh_from_cone( 32, v_radius,v_height, v_style->m_line_color, v_style->m_fill_color );
			}
			else if ( str_label == "Prism" )
			{
				long v_sides = 0;
				double v_radius = 0.0, v_height = 0.0;
				parse_prism( v_volume, v_sides, v_radius, v_height );
				v_mesh = render_object_factory::create_mesh_from_cylinder( v_sides, v_radius, v_height, v_style->m_line_color, v_style->m_fill_color );
			}
			else if ( str_label == "Pyramid" )
			{	
				long v_sides = 0;
				double v_radius = 0.0, v_height = 0.0;
				parse_pyramid( v_volume, v_sides, v_radius, v_height );
				v_mesh = render_object_factory::create_mesh_from_cone( v_sides, v_radius, v_height, v_style->m_line_color, v_style->m_fill_color );
			}
			else if ( str_label == "Mesh" )
			{
				point_list_d v_point_list;
				index_list_u v_index_list;
				parse_mesh( v_volume, v_point_list, v_index_list );
				v_mesh = render_object_factory::create_mesh_from_vertex_and_index_cart ( &v_point_list[0],
					v_point_list.size(),
					v_shadow_style->m_shadow_color,
					&v_index_list[0],
					v_index_list.size() );
			}
		}

		if ( v_mesh == NULL )
		{
			AUTO_DELETE( v_element_mesh );
			return NULL;
		}

		v_mesh->set_shadow_color( v_shadow_style->m_shadow_color );
		v_mesh->set_shadow_type( v_shadow_style->m_shadow_type );
		v_element_mesh->create( v_mesh );

		//v_document->register_object( v_element_mesh );
		v_parent->attach_object( v_element_mesh );

		return v_element_mesh;
	}

	void xml_element_mesh_parser::parse_box( TiXmlNode* v_xml_node, double& v_length, double& v_width, double& v_height )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT ) return;

		double v_north = 0.0, v_south = 0.0, v_east = 0.0, v_west = 0.0, v_top = 0.0, v_bottom = 0.0;
		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT ) continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			string str_label = v_child->Value();
			if ( str_label == "Length" )
			{
				v_length = string_ext::to_double( v_elem->GetText() );
			}
			else if ( str_label == "Width" )
			{
				v_width = string_ext::to_double( v_elem->GetText() );
			}
			else if ( str_label == "Height" )
			{
				v_height = string_ext::to_double( v_elem->GetText() );
			}
		}
	}

	void xml_element_mesh_parser::parse_sphere( TiXmlNode* v_xml_node, double& v_radius )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT ) return;

		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT ) continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			string str_label = v_child->Value();

			if ( str_label == "Radius" )
			{
				v_radius = string_ext::to_double( v_elem->GetText() );
			}
		}
	}

	void xml_element_mesh_parser::parse_cone( TiXmlNode* v_xml_node, double& v_radius, double& v_height )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT ) return;	

		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT ) continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			string str_label = v_child->Value();

			if ( str_label == "Radius" )
			{
				v_radius = string_ext::to_double( v_elem->GetText() );
			}
			else if ( str_label == "Height" )
			{
				v_height = string_ext::to_double( v_elem->GetText() );
			}
		}
	}

	void xml_element_mesh_parser::parse_cylinder( TiXmlNode* v_xml_node, double& v_radius, double& v_height )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT ) return;	

		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT ) continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			string str_label = v_child->Value();

			if ( str_label == "Radius" )
			{
				v_radius = string_ext::to_double( v_elem->GetText() );
			}
			else if ( str_label == "Height" )
			{
				v_height = string_ext::to_double( v_elem->GetText() );
			}
		}
	}

	void xml_element_mesh_parser::parse_prism( TiXmlNode* v_xml_node, long& v_sides, double& v_radius, double& v_height )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT ) return;	

		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT ) continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			string str_label = v_child->Value();

			if ( str_label == "Radius" )
			{
				v_radius = string_ext::to_double( v_elem->GetText() );
			}
			else if ( str_label == "Sides" )
			{
				v_sides = string_ext::to_int( v_elem->GetText() );
			}
			else if ( str_label == "Height" )
			{
				v_height = string_ext::to_double( v_elem->GetText() );
			}
		}
	}

	void xml_element_mesh_parser::parse_pyramid( TiXmlNode* v_xml_node, long& v_sides, double& v_radius, double& v_height )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT ) return;	

		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT ) continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			string str_label = v_child->Value();

			if ( str_label == "Sides" )
			{
				v_sides = string_ext::to_int( v_elem->GetText() );
			}
			else if ( str_label == "Radius" )
			{
				v_radius = string_ext::to_double( v_elem->GetText() );
			}
			else if ( str_label == "Height" )
			{
				v_height = string_ext::to_double( v_elem->GetText() );
			}
		}
	}

	void xml_element_mesh_parser::parse_volume( TiXmlNode* v_xml_node, point_list_d& v_points, double& vol_height )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT ) return;	

		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT ) continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			string str_label = v_child->Value();

			if ( str_label == "Coordinates" )
			{
				v_points = xml_parser_ext::to_point_list_d( v_elem->GetText() );
			}
			else if ( str_label == "Height" )
			{
				vol_height = string_ext::to_double( v_elem->GetText() );
			}
		}
	}

	void xml_element_mesh_parser::parse_mesh( TiXmlNode* v_xml_node, point_list_d& v_point_list, index_list_u& v_index_list )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT ) return;	

		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT ) continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;
			string str_label = v_child->Value();

			if ( str_label == "VertexArray" )
			{
				v_point_list = xml_parser_ext::to_point_list_d( v_elem->GetText() );
			}
			else if ( str_label == "IndexArray" )
			{
				v_index_list = xml_parser_ext::to_index_list_u( v_elem->GetText() );
			}
		}
	}

	// 
	// xml_element_block_parser
	//
	object_base* xml_element_block_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT ) return NULL;

		document* v_document = (document*)v_container;
		element_folder* v_parent = dynamic_cast<element_folder*>(parent_object);
		if ( v_parent == NULL )
		{
			v_parent = v_document->get_element_root();
		}

		// create object
		element_mesh* v_element_model = new element_mesh( v_document );
		//v_element_model->initialize( v_document );

		// parse attribute
		ustring str_id, str_name;
		TiXmlElement *v_elem = (TiXmlElement *)v_xml_node;
		{
			char* str_temp = NULL;

			//  parse id
			if( (str_temp = (char*)v_elem->Attribute( "id" )) != NULL )
			{
				str_id = string_ext::to_wstring( str_temp );
			}
			if( (str_temp = (char*)v_elem->Attribute( "name" )) != NULL )
			{
				str_name = string_ext::to_wstring( str_temp );
			}

		}
		v_element_model->set_guid( str_id.c_str() );
		v_element_model->set_name( str_name.c_str() );

		bool v_visible = true;
		vector_3d vec_location( 0.0, 0.0, spherical_coords::s_semi_major );
		vector_3d vec_scaling( 1.0, 1.0, 1.0 );
		vector_3d vec_rotation( 0.0, math_d::s_half_pi, 0.0 );
		matrix_4d v_transform_mat = matrix_4d::s_identity;
		int v_tranform_type = 0;

		render_object_geometry_mesh* v_mesh = NULL;
		TiXmlElement* v_volume = NULL;
		brush_style* v_render_style = NULL;
		shadow_style* v_shadow_style = NULL;

		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT ) continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			string str_label = v_child->Value();

			if ( str_label == "Visible" )
			{
				v_visible = string_ext::to_bool( v_elem->GetText() );
				v_element_model->set_visible( v_visible );
			}
			else if ( str_label == "RenderStyle" )
			{
				v_render_style = render_style_parser::parse_brush_style( v_elem, NULL );
				v_shadow_style = render_style_parser::parse_shadow_style( v_elem, NULL );
				v_render_style->m_style_array.push_back( v_shadow_style );
			}
			else if ( str_label == "SphericalTransform" )
			{
				spherical_transform_parser v_parser;
				v_parser.forward_parse( NULL, v_elem, v_element_model );
			}
			else if ( str_label == "TransformMatrix" )
			{
				double_list v_list = xml_parser_ext::to_double_array( v_elem->GetText() );
				for( int i = 0; i < 16; i++ )
					v_transform_mat.m[i] = v_list[i];

				v_element_model->set_matrix( v_transform_mat );
			}
			else if ( str_label == "Box" )
			{
				v_volume = v_elem;
			}
			else if ( str_label == "Sphere" )
			{
				v_volume = v_elem;
			}
			else if ( str_label == "Volume" )
			{
				v_volume = v_elem;
			}
			else if ( str_label == "Cylinder" )
			{
				v_volume = v_elem;
			}
			else if ( str_label == "Cone" )
			{
				v_volume = v_elem;
			}
			else if ( str_label == "Prism" )
			{
				v_volume = v_elem;
			}
			else if ( str_label == "Pyramid" )
			{	
				v_volume = v_elem;
			}
			else if ( str_label == "Mesh" )
			{	
				v_volume = v_elem;
			}
		}

		v_element_model->set_render_style( (style_base*)v_render_style );
		// 解析geometry_mesh
		{
			string str_label = v_volume->Value();
			brush_style* v_style = (brush_style*)(v_element_model->get_render_style());
			v_shadow_style = (shadow_style*)(v_style->m_style_array[0]);

			if ( str_label == "Box" )
			{
				double v_length = 0.0, v_width = 0.0, v_height = 0.0;
				parse_box( v_volume, v_length, v_width, v_height);
				v_mesh = render_object_factory::create_mesh_from_box( v_width, v_height, v_length, v_style->m_line_color, v_style->m_fill_color );
			}
			else if ( str_label == "Sphere" )
			{
				double v_radius = 0.0;
				parse_sphere( v_volume, v_radius );
				v_mesh = render_object_factory::create_mesh_from_sphere( v_radius, v_style->m_line_color, v_style->m_fill_color );
			}
			else if ( str_label == "Volume" )
			{
				point_list_d pt_list;
				double v_height = 0.0;
				parse_volume( v_volume, pt_list, v_height );
				v_mesh = render_object_factory::create_mesh_from_volume( &pt_list[0], pt_list.size(), v_height, v_style->m_line_color, v_style->m_fill_color );
			}
			else if ( str_label == "Cylinder" )
			{
				double v_radius = 0.0, v_height = 0.0;
				parse_cylinder( v_volume, v_radius, v_height );
				v_mesh = render_object_factory::create_mesh_from_cylinder( 32, v_radius, v_height, v_style->m_line_color, v_style->m_fill_color );
			}
			else if ( str_label == "Cone" )
			{
				double v_radius = 0.0, v_height = 0.0;
				parse_cone( v_volume, v_radius, v_height );
				v_mesh = render_object_factory::create_mesh_from_cone( 32, v_radius,v_height, v_style->m_line_color, v_style->m_fill_color );
			}
			else if ( str_label == "Prism" )
			{
				long v_sides = 0;
				double v_radius = 0.0, v_height = 0.0;
				parse_prism( v_volume, v_sides, v_radius, v_height );
				v_mesh = render_object_factory::create_mesh_from_cylinder( v_sides, v_radius, v_height, v_style->m_line_color, v_style->m_fill_color );
			}
			else if ( str_label == "Pyramid" )
			{	
				long v_sides = 0;
				double v_radius = 0.0, v_height = 0.0;
				parse_pyramid( v_volume, v_sides, v_radius, v_height );
				v_mesh = render_object_factory::create_mesh_from_cone( v_sides, v_radius, v_height, v_style->m_line_color, v_style->m_fill_color );
			}
			else if ( str_label == "Mesh" )
			{
				point_list_d v_point_list;
				index_list_u v_index_list;
				parse_mesh( v_volume, v_point_list, v_index_list );
				v_mesh = render_object_factory::create_mesh_from_vertex_and_index_cart ( &v_point_list[0],
					v_point_list.size(),
					v_style->m_fill_color,
					&v_index_list[0],
					v_index_list.size() );
			}
		}

		if ( v_mesh == NULL )
		{
			AUTO_DELETE( v_element_model );
			return NULL;
		}

		v_mesh->set_shadow_color( v_shadow_style->m_shadow_color );
		v_mesh->set_shadow_type( v_shadow_style->m_shadow_type );
		v_element_model->create( v_mesh );

		//v_document->register_object( v_element_model );
		v_parent->attach_object( v_element_model );

		return v_element_model;
	}

	void xml_element_block_parser::parse_box( TiXmlNode* v_xml_node, double& v_length, double& v_width, double& v_height )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT ) return;

		double v_north = 0.0, v_south = 0.0, v_east = 0.0, v_west = 0.0, v_top = 0.0, v_bottom = 0.0;
		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT ) continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			string str_label = v_child->Value();
			if ( str_label == "Length" )
			{
				v_length = string_ext::to_double( v_elem->GetText() );
			}
			else if ( str_label == "Width" )
			{
				v_width = string_ext::to_double( v_elem->GetText() );
			}
			else if ( str_label == "Height" )
			{
				v_height = string_ext::to_double( v_elem->GetText() );
			}
		}
	}

	void xml_element_block_parser::parse_sphere( TiXmlNode* v_xml_node, double& v_radius )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT ) return;

		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT ) continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			string str_label = v_child->Value();

			if ( str_label == "Radius" )
			{
				v_radius = string_ext::to_double( v_elem->GetText() );
			}
		}
	}

	void xml_element_block_parser::parse_cone( TiXmlNode* v_xml_node, double& v_radius, double& v_height )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT ) return;	

		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT ) continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			string str_label = v_child->Value();

			if ( str_label == "Radius" )
			{
				v_radius = string_ext::to_double( v_elem->GetText() );
			}
			else if ( str_label == "Height" )
			{
				v_height = string_ext::to_double( v_elem->GetText() );
			}
		}
	}

	void xml_element_block_parser::parse_cylinder( TiXmlNode* v_xml_node, double& v_radius, double& v_height )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT ) return;	

		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT ) continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			string str_label = v_child->Value();

			if ( str_label == "Radius" )
			{
				v_radius = string_ext::to_double( v_elem->GetText() );
			}
			else if ( str_label == "Height" )
			{
				v_height = string_ext::to_double( v_elem->GetText() );
			}
		}
	}

	void xml_element_block_parser::parse_prism( TiXmlNode* v_xml_node, long& v_sides, double& v_radius, double& v_height )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT ) return;	

		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT ) continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			string str_label = v_child->Value();

			if ( str_label == "Radius" )
			{
				v_radius = string_ext::to_double( v_elem->GetText() );
			}
			else if ( str_label == "Sides" )
			{
				v_sides = string_ext::to_int( v_elem->GetText() );
			}
			else if ( str_label == "Height" )
			{
				v_height = string_ext::to_double( v_elem->GetText() );
			}
		}
	}

	void xml_element_block_parser::parse_pyramid( TiXmlNode* v_xml_node, long& v_sides, double& v_radius, double& v_height )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT ) return;	

		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT ) continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			string str_label = v_child->Value();

			if ( str_label == "Sides" )
			{
				v_sides = string_ext::to_int( v_elem->GetText() );
			}
			else if ( str_label == "Radius" )
			{
				v_radius = string_ext::to_double( v_elem->GetText() );
			}
			else if ( str_label == "Height" )
			{
				v_height = string_ext::to_double( v_elem->GetText() );
			}
		}
	}

	void xml_element_block_parser::parse_volume( TiXmlNode* v_xml_node, point_list_d& v_points, double& vol_height )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT ) return;	

		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT ) continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			string str_label = v_child->Value();

			if ( str_label == "Coordinates" )
			{
				v_points = xml_parser_ext::to_point_list_d( v_elem->GetText() );
			}
			else if ( str_label == "Height" )
			{
				vol_height = string_ext::to_double( v_elem->GetText() );
			}
		}
	}

	void xml_element_block_parser::parse_mesh( TiXmlNode* v_xml_node, point_list_d& v_point_list, index_list_u& v_index_list )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT ) return;	

		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT ) continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;
			string str_label = v_child->Value();

			if ( str_label == "VertexArray" )
			{
				v_point_list = xml_parser_ext::to_point_list_d( v_elem->GetText() );
			}
			else if ( str_label == "IndexArray" )
			{
				v_index_list = xml_parser_ext::to_index_list_u( v_elem->GetText() );
			}
		}
	}

	// 
	// xml_element_billboard_parser
	//
	object_base* xml_element_billboard_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT ) return NULL;

		document* v_document = (document*)v_container;
		element_folder* v_parent = dynamic_cast<element_folder*>(parent_object);
		if ( v_parent == NULL )
		{
			v_parent = v_document->get_element_root();
		}

		element_simple_billboard* v_element_bb = new element_simple_billboard( v_document );
		//v_element_bb->initialize( v_document );

		// parse attribute
		ustring str_id, str_name;
		TiXmlElement *v_elem = (TiXmlElement *)v_xml_node;
		{
			char* str_temp = NULL;

			//  parse id
			if( (str_temp = (char*)v_elem->Attribute( "id" )) != NULL )
			{
				str_id = string_ext::to_wstring( str_temp );
			}
			if ( (str_temp = (char*)v_elem->Attribute( "name" )) != NULL )
			{
				str_name = string_ext::to_wstring( str_temp );
			}
		}

		// parse coordinates
		bool v_visible = true;
		double v_max_visible_range = 0.0;

		vector_3d vec_locate;
		point_list_d v_point_list;

		double v_height = 0.003, v_width = 0.003;

		xml_parser_base* v_parser = NULL; 
		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT ) continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			string str_label = v_child->Value();

			if ( str_label == "Visible" )
			{
				v_visible = string_ext::to_bool( v_elem->GetText() );
				v_element_bb->set_visible( v_visible );
			}
			else if ( str_label == "MaxVisibleRange" )
			{
				v_max_visible_range = string_ext::to_double( v_elem->GetText() ) * 0.001;
			}
			else if ( str_label == "RenderStyle" )
			{
				xml_parser_base* xml_parser = m_parser_mngr->get_parser( string_ext::to_wstring( str_label ).c_str() );
				if ( xml_parser )
				{
					xml_parser->forward_parse( v_container, v_elem, v_element_bb->get_render_style() ); 
				}
			}
			else if ( str_label == "Width" )
			{
				v_width = string_ext::to_double( v_elem->GetText() ) * 0.001;
			}
			else if ( str_label == "Height" )
			{
				v_height = string_ext::to_double( v_elem->GetText() ) * 0.001;
			}
			else if ( str_label == "Link" )
			{
				icon_style* v_style = (icon_style*)(v_element_bb->get_render_style());
				if ( v_elem->GetText() )
				{
					v_style->set_icon( string_ext::to_wstring(v_elem->GetText()).c_str() );
				}
			}
			else if ( str_label == "SphericalTransform" )
			{
				spherical_transform_parser v_parser;
				v_parser.forward_parse( NULL, v_elem, v_element_bb );
			}
			else if ( str_label == "Location" )
			{
				vec_locate = xml_parser_ext::to_vector_3d( v_elem->GetText() );
				vec_locate.x = vec_locate.x * math_d::s_deg_to_rad;
				vec_locate.y = vec_locate.y * math_d::s_deg_to_rad;
				vec_locate.z = vec_locate.z * 0.001 + spherical_coords::s_semi_major;
			}
		}

		// create object
		v_element_bb->set_guid( str_id.c_str() );
		v_element_bb->set_visible( v_visible );
		v_element_bb->set_name( str_name.c_str() );
		v_element_bb->set_max_visible_range( v_max_visible_range );

		v_element_bb->get_spherical_transform()->create_from_sphr( vec_locate.x, vec_locate.y, vec_locate.z, 0.0, 0.0, 0.0 );
		v_element_bb->create( v_height, v_width );

		//v_document->register_object( v_element_bb );
		v_parent->attach_object( v_element_bb );

		return v_element_bb;
	}


	//
	//	xml_lookat_parser
	//
	object_base* xml_lookat_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		if ( v_xml_node == NULL || v_xml_node->Type() != TiXmlNode::ELEMENT )
			return NULL;

		spherical_pose* v_lookat = new spherical_pose;

		vector3<double> v_target_sphr;

		double v_longitude, v_latitude, v_altitude, v_heading, v_tilt, v_roll, v_range;

		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT )
				continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			ustring str_label = string_ext::to_wstring( v_child->Value() );

			if ( str_label == L"Longitude" )
			{
				v_longitude = string_ext::to_double( v_elem->GetText() );
			}
			else if ( str_label == L"Latitude" )
			{
				v_latitude = string_ext::to_double( v_elem->GetText() );
			}
			else if ( str_label == L"Altitude" )
			{
				v_altitude = string_ext::to_double( v_elem->GetText() ) + spherical_coords::s_semi_major;
			}
			else if ( str_label == L"Heading" )
			{
				v_heading = string_ext::to_double( v_elem->GetText() );
			}
			else if ( str_label == L"Tilt" )
			{
				v_tilt = string_ext::to_double( v_elem->GetText() );
			}
			else if ( str_label == L"Roll" )
			{
				v_roll = string_ext::to_double( v_elem->GetText() );
			}
			else if ( str_label == L"Range" )
			{
				v_range = string_ext::to_double( v_elem->GetText() );
			}
		}

		v_lookat->create_from_target_sphr( v_longitude, v_latitude, v_altitude, v_heading, v_tilt, v_roll, v_range );

		return NULL;
	}


	//////////////////////////////////////////////////////////////////////////
	// update parsers
	//////////////////////////////////////////////////////////////////////////

	object_base* xml_element_update_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		if ( v_xml_node == NULL || v_xml_node->Type() != TiXmlNode::ELEMENT || parent_object == NULL )
			return NULL;

		object_base* v_ret_obj = NULL;
		std::vector<TiXmlElement*> elem_array;
		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT )
				continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;
			ustring str_label = string_ext::to_wstring( v_child->Value() );

			xml_parser_base* v_parser = singleton_xml_parser_manager::instance().get_parser( str_label.c_str() );
			if ( v_parser )
			{
				v_ret_obj = v_parser->forward_parse( v_container, v_elem, parent_object );
			}
		}

		return v_ret_obj;
	}

	//
	//	xml_element_create_parser
	//
	object_base* xml_element_create_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		if ( v_xml_node == NULL || v_xml_node->Type() != TiXmlNode::ELEMENT || parent_object == NULL )
			return NULL;

		document* v_document = (document*)v_container;
		element_folder* v_parent = dynamic_cast<element_folder*>(parent_object);
		element_folder* v_root = v_document->get_element_root();

		object_base* v_ret_obj = NULL;

		xml_parser_base* v_parser = NULL;
		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT )
				continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;
			ustring str_label = string_ext::to_wstring( v_child->Value() );

			if ( str_label == L"ElementFolder" || str_label == L"ElementDocument" )
			{
				ustring str_targetid;

				char* v_target_id = (char*)v_elem->Attribute( "targetid" );
				if ( v_target_id )
				{
					str_targetid = string_ext::to_wstring( v_target_id );
				}
				else
				{
					v_parser = singleton_xml_parser_manager::instance().get_parser( str_label.c_str() );
					if ( v_parser )
					{
						v_parser->forward_parse( v_container, v_elem, v_root );
					}
					continue;
				}

				if ( str_targetid.size() > 0 )
				{
					element_folder* v_parent_folder = dynamic_cast<element_folder*>( v_document->get_register_object( str_targetid.c_str() ) );

					for ( TiXmlNode* v_folder_child = v_elem->FirstChild(); v_folder_child; v_folder_child = v_folder_child->NextSibling() )
					{
						if ( v_folder_child->Type() != TiXmlNode::ELEMENT )
							continue;

						TiXmlElement* v_folder_elem = (TiXmlElement*)v_folder_child;
						ustring str_folder_label = string_ext::to_wstring( v_folder_child->Value() );

						v_parser = singleton_xml_parser_manager::instance().get_parser( str_folder_label.c_str() );
						if ( v_parser )
						{
							v_ret_obj = v_parser->forward_parse( v_container, v_folder_elem, v_parent_folder );
						}
					}
				}
			}
			else
			{
				v_parser = singleton_xml_parser_manager::instance().get_parser( str_label.c_str() );
				if ( v_parser )
				{
					 v_ret_obj = v_parser->forward_parse( v_container, v_elem, v_root );
				}
			}
		}

		return v_ret_obj;
	}

	//
	//	xml_element_delete_parser
	//
	object_base* xml_element_delete_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT ) return NULL;

		document* v_document = (document*)v_container;

		xml_parser_base* v_parser = NULL;
		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT )
				continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;
			string str_label = v_elem->Value();

			if ( str_label == "TargetID" )
			{
				ustring str_id;
				char* str_target_id = (char*)v_elem->GetText();
				if ( str_target_id )
				{
					str_id = string_ext::to_wstring( str_target_id );
					US_STRING_LOWER( str_id );
				}
				if ( str_id.size() > 0 )
				{
					command_deferred_delete* v_command = new command_deferred_delete( v_document, str_id.c_str() );

					v_document->m_command_executant->push( v_command );
				}
			}
			else if ( str_label == "Layer" )
			{
				ustring str_id;
				char* str_target_id = (char*)v_elem->GetText();
				if ( str_target_id )
				{
					str_id = string_ext::to_wstring( str_target_id );
					US_STRING_LOWER( str_id );
				}

				if ( str_id.size() > 0 )
				{
					command_delete* v_command = new command_delete( v_document, str_id.c_str() );

					v_document->m_command_executant->push( v_command );
				}
			}

		}

		return parent_object;
	}


	//
	//	xml_element_change_parser
	//
	object_base* xml_element_change_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT ) return NULL;

		document* v_document = (document*)v_container;

		xml_parser_base* v_parser = NULL;		
		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT )
				continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;
			ustring str_label =  string_ext::to_wstring( v_child->Value() );

			ustring str_id;
			char* str_target_id = (char*)v_elem->Attribute( "targetid" );
			if ( str_target_id )
			{
				str_id = string_ext::to_wstring( str_target_id );
			}

			if ( str_id.size() > 0 )
			{
				object_base* uni_element = dynamic_cast<object_base*>(v_document->get_register_object( str_id.c_str() ));
				if ( uni_element )
				{
					change_element_parameter( v_container, v_elem, uni_element );					
				}
			}
		}

		return NULL;
	}

	void xml_element_change_parser::change_element_parameter( object_base* v_container, TiXmlNode *v_xml_node, object_base *elem_object )
	{
		if ( v_xml_node == NULL || elem_object == NULL ) return;

		spatial_object* uni_sphr = dynamic_cast<spatial_object*>(elem_object);

		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT )
				continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;
			ustring str_label =  string_ext::to_wstring( v_child->Value() );

			if ( str_label == L"Visible" )
			{
				bool b_vis = string_ext::to_bool( v_elem->GetText() );
				uni_sphr->set_visible( b_vis );
			}
		}
	}


	//////////////////////////////////////////////////////////////////////////
	// style parsers
	//////////////////////////////////////////////////////////////////////////

	//
	// icon_style
	//
	object_base* icon_style_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		return NULL;
		//if ( v_xml_node == NULL || v_xml_node->Type() != TiXmlNode::ELEMENT || parent_object == NULL )
		//	return NULL;

		//icon_style& v_icon_style = ((render_style*)parent_object)->m_icon_style;

		//for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		//{
		//	if ( v_child->Type() != TiXmlNode::ELEMENT )
		//		continue;

		//	TiXmlElement* v_elem = (TiXmlElement*)v_child;

		//	ustring str_label = string_ext::to_wstring( v_child->Value() );

		//	if ( str_label == L"Color" )
		//	{
		//		v_icon_style.m_blend_color = string_ext::to_dword( v_elem->GetText() );
		//	}
		//	else if ( str_label == L"Link" )
		//	{
		//		v_icon_style.m_str_texture_url = string_ext::to_wstring( v_elem->GetText() );
		//	}
		//}

		//return &v_icon_style;
	}

	//
	// icon_style
	//
	object_base* font_style_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		return NULL;
		//if ( v_xml_node == NULL || v_xml_node->Type() != TiXmlNode::ELEMENT || parent_object == NULL )
		//	return NULL;

		//font_style& v_font_style = ((render_style*)parent_object)->m_font_style;

		//for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		//{
		//	if ( v_child->Type() != TiXmlNode::ELEMENT )
		//		continue;

		//	TiXmlElement* v_elem = (TiXmlElement*)v_child;

		//	ustring str_label = string_ext::to_wstring( v_child->Value() );

		//	if ( str_label == L"TypeFace" )
		//	{
		//		v_font_style.m_face_name = string_ext::to_wstring( v_elem->GetText() );
		//	}
		//	else if ( str_label == L"TextColor" )
		//	{
		//		v_font_style.m_font_color = string_ext::to_dword( v_elem->GetText() );
		//	}
		//	else if ( str_label == L"Scale" )
		//	{
		//		v_font_style.m_font_scale = string_ext::to_double( v_elem->GetText() );
		//	}
		//}

		//return &v_font_style;
	}

	//
	// shadow_style
	//
	object_base* shadow_style_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		return NULL;
		//if ( v_xml_node == NULL || v_xml_node->Type() != TiXmlNode::ELEMENT || parent_object == NULL )
		//	return NULL;

		//shadow_style& v_shadow_style = ((render_style*)parent_object)->m_shadow_style;

		//for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		//{
		//	if ( v_child->Type() != TiXmlNode::ELEMENT )
		//		continue;

		//	TiXmlElement* v_elem = (TiXmlElement*)v_child;

		//	ustring str_label = string_ext::to_wstring( v_child->Value() );

		//	if ( str_label == L"ShadowColor" )
		//	{
		//		v_shadow_style.m_shadow_color = string_ext::to_dword( v_elem->GetText() );
		//	}
		//	else if ( str_label == L"ShadowType" )
		//	{
		//		v_shadow_style.m_shadow_type = string_ext::to_int( v_elem->GetText() );
		//	}

		//}

		//return &v_shadow_style;
	}

	//
	// render_style
	//
	object_base* render_style_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		return NULL;

		//if ( v_xml_node == NULL || v_xml_node->Type() != TiXmlNode::ELEMENT )
		//	return NULL;

		//render_style* v_render_style = (render_style*)parent_object;
		//if ( v_render_style == NULL )
		//{
		//	v_render_style = new render_style;
		//}

		//for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		//{
		//	if ( v_child->Type() != TiXmlNode::ELEMENT )
		//		continue;

		//	TiXmlElement* v_elem = (TiXmlElement*)v_child;

		//	string str_label = v_child->Value();

		//	if ( str_label == "PointSize" )
		//	{				
		//		v_render_style->m_line_width = string_ext::to_int( v_elem->GetText() );
		//	}
		//	else if ( str_label == "FillColor" )
		//	{				
		//		v_render_style->m_fill_color = string_ext::to_dword( v_elem->GetText() );
		//	}
		//	else if ( str_label == "LineWidth" )
		//	{
		//		v_render_style->m_line_width = string_ext::to_int( v_elem->GetText() );
		//	}
		//	else if ( str_label == "LineColor" )
		//	{
		//		v_render_style->m_line_color = string_ext::to_dword( v_elem->GetText() );
		//	}	
		//	else if ( str_label == "Icon" )
		//	{
		//		v_render_style->m_icon_style.m_str_texture_url = string_ext::to_wstring( v_elem->GetText() ).c_str();
		//	}
		//	else if ( str_label == "ShadowStyle" )
		//	{
		//		shadow_style_parser v_parser;
		//		v_parser.forward_parse( NULL, v_elem, v_render_style );
		//	}
		//	else if ( str_label == "IconStyle" )
		//	{
		//		icon_style_parser v_parser;
		//		v_parser.forward_parse( NULL, v_elem, v_render_style );
		//	}
		//	else if ( str_label == "FontStyle" )
		//	{
		//		font_style_parser v_parser;
		//		v_parser.forward_parse( NULL, v_elem, v_render_style );
		//	}
		//}

		//return v_render_style;	
	}

	pen_style* render_style_parser::parse_pen_style( TiXmlNode* v_xml_node, pen_style* v_style )
	{
		pen_style* v_render_style = v_style;
		if ( v_render_style == NULL )
		{
			v_render_style = new pen_style;
		}
		
		if ( v_xml_node == NULL || v_xml_node->Type() != TiXmlNode::ELEMENT )
			return v_render_style;

		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT )
				continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			string str_label = v_child->Value();
			if ( str_label == "PointSize" )
			{				
				v_render_style->m_pen_width = string_ext::to_int( v_elem->GetText() );
			}
			else if ( str_label == "LineWidth" )
			{
				v_render_style->m_pen_width = string_ext::to_int( v_elem->GetText() );
			}
			else if ( str_label == "LineColor" )
			{
				v_render_style->m_pen_color = string_ext::to_dword( v_elem->GetText() );
			}
			else if ( str_label == "PointColor" )
			{
				v_render_style->m_pen_color = string_ext::to_dword( v_elem->GetText() );
			}
			else if ( str_label == "PenWidth" )
			{
				v_render_style->m_pen_width = string_ext::to_int( v_elem->GetText() );
			}
			else if ( str_label == "PenColor" )
			{
				v_render_style->m_pen_color = string_ext::to_dword( v_elem->GetText() );
			}
		}

		return v_render_style;
	}

	brush_style* render_style_parser::parse_brush_style( TiXmlNode* v_xml_node, brush_style* v_style )
	{
		brush_style* v_render_style = v_style;
		if ( v_render_style == NULL )
		{
			v_render_style = new brush_style;
		}

		if ( v_xml_node == NULL || v_xml_node->Type() != TiXmlNode::ELEMENT )
			return v_render_style;
		
		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT )
				continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			string str_label = v_child->Value();
			if ( str_label == "FillColor" )
			{				
				v_render_style->m_fill_color = string_ext::to_dword( v_elem->GetText() );
			}
			else if ( str_label == "LineWidth" )
			{
				v_render_style->m_line_width = string_ext::to_int( v_elem->GetText() );
			}
			else if ( str_label == "LineColor" )
			{
				v_render_style->m_line_color = string_ext::to_dword( v_elem->GetText() );
			}
		}

		return v_render_style;
	}

	icon_style* render_style_parser::parse_icon_style( TiXmlNode* v_xml_node, icon_style* v_style )
	{
		icon_style* v_render_style = v_style;
		if ( v_render_style == NULL )
		{
			v_render_style = new icon_style;
		}

		if ( v_xml_node == NULL || v_xml_node->Type() != TiXmlNode::ELEMENT )
			return v_render_style;

		TiXmlNode* icon_style_node = v_xml_node->FirstChild("IconStyle");
		if ( icon_style_node == NULL )
			return v_render_style;

		for ( TiXmlNode* v_child = icon_style_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT )
				continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			string str_label = v_child->Value();

			if ( str_label == "Color" )
			{
				v_render_style->m_blend_color = string_ext::to_dword( v_elem->GetText() );
			}
			else if ( str_label == "Link" )
			{
				v_render_style->m_str_texture_url = string_ext::to_wstring( v_elem->GetText() );
			}
		}

		return v_render_style;
	}

	font_style* render_style_parser::parse_font_style( TiXmlNode* v_xml_node, font_style* v_style )
	{
		font_style* v_render_style = v_style;
		if ( v_render_style == NULL )
		{
			v_render_style = new font_style;
		}

		if ( v_xml_node == NULL || v_xml_node->Type() != TiXmlNode::ELEMENT )
			return v_render_style;

		TiXmlNode* font_style_node = v_xml_node->FirstChild("FontStyle");
		if ( font_style_node == NULL )
			return v_render_style;

		for ( TiXmlNode* v_child = font_style_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT )
				continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			string str_label = v_child->Value();

			if ( str_label == "TypeFace" )
			{
				v_render_style->m_face_name = string_ext::to_wstring( v_elem->GetText() );
			}
			else if ( str_label == "TextColor" )
			{
				v_render_style->m_font_color = string_ext::to_dword( v_elem->GetText() );
			}
			else if ( str_label == "Scale" )
			{
				v_render_style->m_font_scale = string_ext::to_double( v_elem->GetText() );
			}
		}

		return v_render_style;
	}

	shadow_style* render_style_parser::parse_shadow_style( TiXmlNode* v_xml_node, shadow_style* v_style )
	{
		shadow_style* v_render_style = v_style;
		if ( v_render_style == NULL )
		{
			v_render_style = new shadow_style;
		}

		if ( v_xml_node == NULL || v_xml_node->Type() != TiXmlNode::ELEMENT )
			return v_render_style;

		TiXmlNode* shadow_style_node = v_xml_node->FirstChild("ShadowStyle");
		if ( shadow_style_node == NULL )
			return v_render_style;

		for ( TiXmlNode* v_child = shadow_style_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT )
				continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			string str_label = v_child->Value();

			if ( str_label == "ShadowColor" )
			{
				v_render_style->m_shadow_color = string_ext::to_dword( v_elem->GetText() );
			}
			else if ( str_label == "ShadowType" )
			{
				v_render_style->m_shadow_type = string_ext::to_int( v_elem->GetText() );
			}
		}

		return v_render_style;
	}

	//
	// spherical_tranform 
	//
	object_base* spherical_transform_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		if ( v_xml_node == NULL || v_xml_node->Type() != TiXmlNode::ELEMENT )
			return NULL;

		spherical_object* v_so = dynamic_cast<spherical_object*>( parent_object );
		spherical_transform* s_trans = v_so->get_spherical_transform();
		if ( s_trans == NULL )
			return NULL;
		
		vector_3d vec_location( 0.0, 0.0, 0.0 );
		vector_3d vec_scaling( 1.0, 1.0, 1.0 );
		vector_3d vec_rotation( 0.0, math_d::s_half_pi, 0.0 );
		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT )
				continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			string str_label = v_child->Value();

			if ( str_label == "Location" )
			{
				vec_location = xml_parser_ext::to_vector_3d( v_elem->GetText() );
				vec_location.x *= math_d::s_deg_to_rad;
				vec_location.y *= math_d::s_deg_to_rad;
				vec_location.z = vec_location.z * 0.001 + spherical_coords::s_semi_major;
			}
			else if ( str_label == "Scaling" )
			{
				vec_scaling = xml_parser_ext::to_vector_3d( v_elem->GetText() );
			}
			else if ( str_label == "Rotation" )
			{
				vec_rotation = xml_parser_ext::to_vector_3d( v_elem->GetText() );
				vec_rotation.x *= math_d::s_deg_to_rad;
				vec_rotation.y *= math_d::s_deg_to_rad;
				vec_rotation.z *= math_d::s_deg_to_rad;
			}
			else if ( str_label == "EulerRotation" )
			{
				vec_rotation = xml_parser_ext::to_vector_3d( v_elem->GetText() );
				vec_rotation.x *= math_d::s_deg_to_rad;
				vec_rotation.y *= math_d::s_deg_to_rad;
				vec_rotation.z *= math_d::s_deg_to_rad;
			}
			else if ( str_label == "Quaternion" )
			{
				quaternion<double> quater = xml_parser_ext::to_quaternion_4d( v_elem->GetText() );
				vec_rotation = quaternion<double>::to_euler( quater );
				//vec_rotation = xml_parser_ext::to_vector_3d( v_elem->GetText() );
				//vec_rotation.x *= math_d::s_deg_to_rad;
				//vec_rotation.y *= math_d::s_deg_to_rad;
				//vec_rotation.z *= math_d::s_deg_to_rad;
			}
		}

		s_trans->set_scale( vec_scaling );
		s_trans->create_from_sphr( vec_location.x, vec_location.y, vec_location.z, vec_rotation.y, vec_rotation.x, vec_rotation.z );

		return parent_object;
	}

}