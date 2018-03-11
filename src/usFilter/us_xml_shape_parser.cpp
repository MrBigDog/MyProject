///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_xml_shape_parser.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : xml_shape_parser class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_xml_shape_parser.h"

namespace uniscope_globe
{
	// 
	// xml_edit_element_block_parser
	//
	object_base* xml_edit_element_block_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT ) return NULL;

		document* v_document = (document*)v_container;
		element_folder* v_parent = dynamic_cast<element_folder*>(parent_object);
		if ( v_parent == NULL )
		{
			v_parent = v_document->get_element_root();
		}

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
	
		TiXmlElement* v_block = NULL;
		spherical_object* v_sphr_obj = NULL;
		
		bool v_visible = true;		
		vector_3d vec_location( 0.0, 0.0, spherical_coords::s_semi_major );
		vector_3d vec_scaling( 1.0, 1.0, 1.0 );
		vector_3d vec_rotation( 0.0, math_d::s_half_pi, 0.0 );
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
			}
			else if ( str_label == "RenderStyle" )
			{
				v_render_style = render_style_parser::parse_brush_style( v_elem, NULL );
				v_shadow_style = render_style_parser::parse_shadow_style( v_elem, NULL );
				v_render_style->m_style_array.push_back(v_shadow_style);
				//xml_parser_base* xml_parser = m_parser_mngr->get_parser( string_ext::to_wstring( str_label ).c_str() );
				//if ( xml_parser )
				//{
				//	v_render_style = (render_style*)xml_parser->forward_parse( NULL, v_elem, NULL );
				//}
			}
			else if ( str_label == "SphericalTransform" )
			{
				parse_spheical_transform( v_elem, vec_location, vec_rotation, vec_scaling );
			}
			else if ( str_label == "Box" )
			{
				v_block = v_elem;
			}
			else if ( str_label == "Sphere" )
			{
				v_block = v_elem;
			}
			else if ( str_label == "Volume" )
			{
				v_block = v_elem;
			}
			else if ( str_label == "Cylinder" )
			{
				v_block = v_elem;
			}
			else if ( str_label == "Cone" )
			{
				v_block = v_elem;
			}
			else if ( str_label == "Prism" )
			{
				v_block = v_elem;
			}
			else if ( str_label == "Pyramid" )
			{	
				v_block = v_elem;
			}
		}

		if ( v_block == NULL )
			return NULL;

		// create object		
		string str_label = v_block->Value();
		if ( str_label == "Box" )
		{
			element_box* v_element_object = new element_box;
			parse_box( v_block, v_element_object->m_length, v_element_object->m_width, v_element_object->m_height);
			v_sphr_obj = dynamic_cast<spherical_object*>(v_element_object);
			v_sphr_obj->set_guid( str_id.c_str() );
			v_sphr_obj->set_name( str_name.c_str() );
			v_sphr_obj->set_visible( v_visible );
			v_sphr_obj->get_spherical_transform()->create_from_sphr( vec_location.x, vec_location.y, vec_location.z, vec_rotation.x, vec_rotation.y, vec_rotation.z );
			v_sphr_obj->set_document( v_document );
			v_sphr_obj->set_render_style( v_render_style );
			v_element_object->create();
		}
		else if ( str_label == "Sphere" )
		{
			element_sphere* v_element_object = new element_sphere;
			parse_sphere( v_block, v_element_object->m_radius );
			v_sphr_obj = dynamic_cast<spherical_object*>(v_element_object);
			v_sphr_obj->set_guid( str_id.c_str() );
			v_sphr_obj->set_name( str_name.c_str() );
			v_sphr_obj->set_visible( v_visible );
			v_sphr_obj->get_spherical_transform()->create_from_sphr( vec_location.x, vec_location.y, vec_location.z, vec_rotation.x, vec_rotation.y, vec_rotation.z );
			v_sphr_obj->set_document( v_document );
			v_sphr_obj->set_render_style( v_render_style );
			v_element_object->create();
		}
		else if ( str_label == "Volume" )
		{
			element_volume* v_element_object = new element_volume;
			
			coordinates_array ring;
			double height;
			parse_volume( v_block, ring, height );
			v_element_object->set_height( height );
			coordinates_part_array ring_array;
			ring_array.push_back( ring );
			v_element_object->set_coordinate( ring_array );

			v_sphr_obj = dynamic_cast<spherical_object*>(v_element_object);
			v_sphr_obj->set_guid( str_id.c_str() );
			v_sphr_obj->set_name( str_name.c_str() );
			v_sphr_obj->set_visible( v_visible );
			v_sphr_obj->get_spherical_transform()->create_from_sphr( vec_location.x, vec_location.y, vec_location.z, vec_rotation.x, vec_rotation.y, vec_rotation.z );
			v_sphr_obj->set_document( v_document );
			v_sphr_obj->set_render_style( v_render_style );
			v_element_object->create();
		}
		else if ( str_label == "Cylinder" )
		{
			element_prism* v_element_object = new element_prism;
			v_element_object->m_sides = 24;
			parse_cylinder( v_block, v_element_object->m_radius, v_element_object->m_height );
			v_sphr_obj = dynamic_cast<spherical_object*>(v_element_object);
			v_sphr_obj->set_guid( str_id.c_str() );
			v_sphr_obj->set_name( str_name.c_str() );
			v_sphr_obj->set_visible( v_visible );
			v_sphr_obj->get_spherical_transform()->create_from_sphr( vec_location.x, vec_location.y, vec_location.z, vec_rotation.x, vec_rotation.y, vec_rotation.z );
			v_sphr_obj->set_document( v_document );
			v_sphr_obj->set_render_style( v_render_style );
			v_element_object->create();
		}
		else if ( str_label == "Cone" )
		{
			element_pyramid* v_element_object = new element_pyramid;
			v_element_object->m_sides = 24;
			parse_cone( v_block, v_element_object->m_radius, v_element_object->m_height );
			v_sphr_obj = dynamic_cast<spherical_object*>(v_element_object);
			v_sphr_obj->set_guid( str_id.c_str() );
			v_sphr_obj->set_name( str_name.c_str() );
			v_sphr_obj->set_visible( v_visible );
			v_sphr_obj->get_spherical_transform()->create_from_sphr( vec_location.x, vec_location.y, vec_location.z, vec_rotation.x, vec_rotation.y, vec_rotation.z );
			v_sphr_obj->set_document( v_document );
			v_sphr_obj->set_render_style( v_render_style );
			v_element_object->create();
		}
		else if ( str_label == "Prism" )
		{
			element_prism* v_element_object = new element_prism;
			parse_prism( v_block, v_element_object->m_sides, v_element_object->m_radius, v_element_object->m_height );
			v_sphr_obj = dynamic_cast<spherical_object*>(v_element_object);
			v_sphr_obj->set_guid( str_id.c_str() );
			v_sphr_obj->set_name( str_name.c_str() );
			v_sphr_obj->set_visible( v_visible );
			v_sphr_obj->get_spherical_transform()->create_from_sphr( vec_location.x, vec_location.y, vec_location.z, vec_rotation.x, vec_rotation.y, vec_rotation.z );
			v_sphr_obj->set_document( v_document );
			v_sphr_obj->set_render_style( v_render_style );
			v_element_object->create();
		}
		else if ( str_label == "Pyramid" )
		{	
			element_pyramid* v_element_object = new element_pyramid;
			parse_pyramid( v_block, v_element_object->m_sides, v_element_object->m_radius, v_element_object->m_height );
			v_sphr_obj = dynamic_cast<spherical_object*>(v_element_object);
			v_sphr_obj->set_guid( str_id.c_str() );
			v_sphr_obj->set_name( str_name.c_str() );
			v_sphr_obj->set_visible( v_visible );
			v_sphr_obj->get_spherical_transform()->create_from_sphr( vec_location.x, vec_location.y, vec_location.z, vec_rotation.x, vec_rotation.y, vec_rotation.z );
			v_sphr_obj->set_document( v_document );
			v_sphr_obj->set_render_style( v_render_style );
			v_element_object->create();
		}
		


		v_document->register_object( v_sphr_obj );
		v_parent->attach_object( v_sphr_obj );

		return v_sphr_obj;
	}

	void xml_edit_element_block_parser::parse_spheical_transform( TiXmlNode* v_xml_node, vector_3d& v_location, vector_3d& v_rotation, vector_3d& v_scaling )
	{
		if ( v_xml_node == NULL || v_xml_node->Type() != TiXmlNode::ELEMENT )
			return;

		v_location.x = 0.0;
		v_location.y = 0.0;
		v_location.z = 0.0;

		v_rotation.x = 0.0;
		v_rotation.y = math_d::s_half_pi;
		v_rotation.z = 0.0;
		
		v_scaling.x = 1.0;
		v_scaling.y = 1.0;
		v_scaling.z = 1.0;
		
		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT )
				continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			string str_label = v_child->Value();

			if ( str_label == "Location" )
			{
				v_location = xml_parser_ext::to_vector_3d( v_elem->GetText() );
				v_location.x *= math_d::s_deg_to_rad;
				v_location.y *= math_d::s_deg_to_rad;
				v_location.z = v_location.z * 0.001 + spherical_coords::s_semi_major;
			}
			else if ( str_label == "Scaling" )
			{
				v_scaling = xml_parser_ext::to_vector_3d( v_elem->GetText() );
			}
			else if ( str_label == "Rotation" )
			{
				v_rotation = xml_parser_ext::to_vector_3d( v_elem->GetText() );
				v_rotation.x *= math_d::s_deg_to_rad;
				v_rotation.y *= math_d::s_deg_to_rad;
				v_rotation.z *= math_d::s_deg_to_rad;
			}
			else if ( str_label == "EulerRotation" )
			{
				v_rotation = xml_parser_ext::to_vector_3d( v_elem->GetText() );
				v_rotation.x *= math_d::s_deg_to_rad;
				v_rotation.y *= math_d::s_deg_to_rad;
				v_rotation.z *= math_d::s_deg_to_rad;
			}
			else if ( str_label == "Quaternion" )
			{
				quaternion<double> quater = xml_parser_ext::to_quaternion_4d( v_elem->GetText() );
				v_rotation = quaternion<double>::to_euler( quater );
				//vec_rotation = xml_parser_ext::to_vector_3d( v_elem->GetText() );
				//vec_rotation.x *= math_d::s_deg_to_rad;
				//vec_rotation.y *= math_d::s_deg_to_rad;
				//vec_rotation.z *= math_d::s_deg_to_rad;
			}
		}
	}

	void xml_edit_element_block_parser::parse_box( TiXmlNode* v_xml_node, double& v_length, double& v_width, double& v_height )
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

	void xml_edit_element_block_parser::parse_sphere( TiXmlNode* v_xml_node, double& v_radius )
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

	void xml_edit_element_block_parser::parse_cone( TiXmlNode* v_xml_node, double& v_radius, double& v_height )
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

	void xml_edit_element_block_parser::parse_cylinder( TiXmlNode* v_xml_node, double& v_radius, double& v_height )
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

	void xml_edit_element_block_parser::parse_prism( TiXmlNode* v_xml_node, long& v_sides, double& v_radius, double& v_height )
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

	void xml_edit_element_block_parser::parse_pyramid( TiXmlNode* v_xml_node, long& v_sides, double& v_radius, double& v_height )
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


	void xml_edit_element_block_parser::parse_volume( TiXmlNode* v_xml_node, point_list_d& v_points, double& vol_height )
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
	
}