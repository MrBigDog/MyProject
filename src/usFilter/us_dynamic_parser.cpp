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
#include "us_dynamic_parser.h"

namespace uniscope_globe
{
	object_base* dynamic_layer_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT )
			return NULL;

		document* v_doc = dynamic_cast<document*>( v_container );
		dynamic_system* v_system = v_doc->get_dynamic_system();

		TiXmlElement *v_elem = (TiXmlElement *)v_xml_node;
		ustring str_guid = string_ext::to_wstring( v_elem->Attribute( "id" ) );
		ustring str_name = string_ext::to_wstring( v_elem->Attribute( "name" ) );

		dynamic_layer* v_layer = new dynamic_layer;
		v_layer->initialize( v_doc );
		v_layer->set_name( str_name.c_str() );
		v_layer->set_guid( str_guid.c_str() );

		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT )
				continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			ustring str_label = string_ext::to_wstring( v_child->Value() );

			xml_parser_base *v_parser = m_parser_mngr->get_parser( str_label.c_str() );

			v_parser->forward_parse( v_container, v_elem, v_layer );
			
		}

		v_system->attach_object( v_layer );	
		return v_layer;
	}

	object_base* dynamic_model_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT )
			return NULL;

		document* v_doc = dynamic_cast<document*>( v_container );
		dynamic_layer* v_layer = dynamic_cast<dynamic_layer*>(parent_object);

		TiXmlElement *v_elem = (TiXmlElement *)v_xml_node;
		ustring str_guid = string_ext::to_wstring( v_elem->Attribute( "id" ) );
		ustring str_name = string_ext::to_wstring( v_elem->Attribute( "name" ) );
		ustring str_type;
		if( v_elem->Attribute( "type" ) )
			str_type = string_ext::to_wstring( v_elem->Attribute( "type" ) );
		else str_type = L"common";

		dynamic_model* v_model = NULL;
		
		//if( str_type == L"skinned" )
		//{

		//	v_model = new skinned_dynamic_model( v_layer );
		//}
		//else if( str_type == L"common" )
		//{
		//	v_model = new common_dynamic_model( v_layer );
		//}
		//else return NULL;

		v_model->set_name( str_name.c_str() );
		v_model->set_guid( str_guid.c_str() );

		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT )
				continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			ustring str_label = string_ext::to_wstring( v_child->Value() );

			if( str_label == L"MeshLink" )
			{
				ustring mesh_name = str_name;
				if( v_elem->Attribute( "name" ) )
				{
					mesh_name = string_ext::to_wstring( v_elem->Attribute( "name" )  );	
				}

				ustring mesh_link = string_ext::to_wstring( v_elem->GetText() );

				v_model->set_mesh_link( mesh_link.c_str() );				
			}
			//else if( str_label == L"SkeletonLink" )
			//{
			//	ustring skl_link = string_ext::to_wstring( v_elem->GetText() );

			//	skinned_dynamic_model* skinned_model = dynamic_cast<skinned_dynamic_model*>( v_model );
			//	skinned_model->set_skeleton_link( skl_link.c_str() );
			//}
			else if( str_label == L"Position" )
			{
				vector3<float> position =  xml_parser_ext::to_vector_3f( v_elem->GetText() );
				v_model->set_position( vector3<float>( &position.x ));
			}
			else if ( str_label == L"Rotation" )
			{
				vector3<float> rotation =  xml_parser_ext::to_vector_3f( v_elem->GetText() );
				quaternion<float> quater = quaternion<float>::from_euler(rotation.x, rotation.y, rotation.z);
				v_model->set_rotation( quater );
			}
			else if ( str_label == L"Quaternion" )
			{
				quaternion<float> quater = xml_parser_ext::to_quaternion_4f( v_elem->GetText() );
				v_model->set_rotation( quater );
			}
			else if( str_label == L"Scaling" )
			{
				vector3<float> v_scaling =  xml_parser_ext::to_vector_3f( v_elem->GetText() );
				v_model->set_scale( vector3<float>( &v_scaling.x ) );
			}
			else if ( str_label == L"Visibility" )
			{
				v_model->set_visible( string_ext::to_bool( v_elem->GetText() ) );
			}
			else if ( str_label == L"Intersectable" )
			{
				v_model->set_intersectable( string_ext::to_bool( v_elem->GetText() ) );
			}
			else
			{
				xml_parser_base *v_parser = m_parser_mngr->get_parser( str_label.c_str() );

				v_parser->forward_parse( v_container, v_elem, v_model );
			}
		}

		v_layer->m_clip_tree_root->attach_object( v_model );

		return NULL;
	}


	object_base* dynamic_billboard_set_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT )
			return NULL;

		//document* v_doc = dynamic_cast<document*>( v_container );
		//dynamic_layer* v_layer = dynamic_cast<dynamic_layer*>(parent_object);

		//TiXmlElement *v_elem = (TiXmlElement *)v_xml_node;

		//ustring str_guid = string_ext::to_wstring( v_elem->Attribute( "id" ) );
		//ustring str_name = string_ext::to_wstring( v_elem->Attribute( "name" ) );	

		//double v_width;
		//double v_height;
		//ustring v_link;
		//std::vector<vector_3d> v_point_list;
		//std::vector<double> v_scale_list;
		//bool v_has_scale = false;

		//for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		//{
		//	if ( v_child->Type() != TiXmlNode::ELEMENT )
		//		continue;

		//	TiXmlElement* v_elem = (TiXmlElement*)v_child;

		//	ustring str_label = string_ext::to_wstring( v_child->Value() );

		//	if ( str_label == L"Visibility" )
		//	{
		//	}
		//	else if ( str_label == L"Width" )
		//	{
		//		v_width = string_ext::to_double( v_elem->GetText() );
		//	}
		//	else if ( str_label == L"Height" )
		//	{
		//		v_height = string_ext::to_double( v_elem->GetText() );
		//	}
		//	else if ( str_label == L"Link" )
		//	{
		//		v_link = string_ext::to_wstring( v_elem->GetText() );
		//	}	
		//	else if ( str_label == L"Coordinates" )
		//	{
		//		v_point_list = xml_parser_ext::to_point_list_d( v_elem->GetText() );
		//	}
		//	else if ( str_label == L"Scales" )
		//	{
		//		v_has_scale = true;
		//		v_scale_list = xml_parser_ext::to_double_array( v_elem->GetText() );
		//	}

		//}


		//billboard_description bd;
		//bd.m_height = v_height;
		//bd.m_width  = v_width;
		//bd.m_texture_source = v_link;
		//bd.m_billboard = v_point_list;

		//if ( v_has_scale )
		//{
		//	bd.m_scale_array = v_scale_list;
		//}
		//else{
		//	bd.m_scale_array.resize( bd.m_billboard.size() );
		//	for( int i = 0; i < (int)bd.m_billboard.size(); i++ )
		//	{
		//		bd.m_scale_array[i] = 1.0;
		//	}
		//}


		//dynamic_billboard_set* v_bs = new dynamic_billboard_set( v_doc->get_application()->m_manager_group, bd );
		//v_bs->set_name( str_name.c_str() );
		//v_bs->set_guid( str_guid.c_str() );

		//v_layer->m_clip_tree_root->attach_object( v_bs );

		return NULL;
	}
	

	/*object_base* dynamic_label_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT )
			return NULL;

		document* v_doc = dynamic_cast<document*>( v_container );
		dynamic_layer* v_layer = dynamic_cast<dynamic_layer*>(parent_object);

		TiXmlElement *v_elem = (TiXmlElement *)v_xml_node;

		ustring str_id = string_ext::to_wstring( v_elem->Attribute( "id" ) );
		ustring str_name = string_ext::to_wstring( v_elem->Attribute( "name" ) );
		GUID guid;
		UuidFromString( (RPC_WSTR)str_id.c_str() , &guid );

		dynamic_label* v_label = new dynamic_label();
		v_label->set_name( str_name.c_str() );

		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT )
				continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			ustring str_label = string_ext::to_wstring( v_child->Value() );

			if( str_label == L"Visibility" )
			{
				v_label->set_visible( string_ext::to_bool( v_elem->GetText() ) );
			}
			else if( str_label == L"Position" )
			{
				vector3<float> position =  xml_parser_ext::to_vector_3f( v_elem->GetText() );
				v_label->set_position( vector3<float>( &position.x ) );
			}
			else if ( str_label == L"LabelText" )
			{
				v_label->set_label_text( string_ext::to_wstring( v_elem->GetText() ).c_str() );
			}
			else if( str_label == L"Icon" )
			{
				ustring link = string_ext::to_wstring( v_elem->GetText() );

				ustring::size_type v_pos = link.find( L"root://" );

				if( v_pos != ustring::npos )
				{
					link = singleton_system_environment::instance().m_root_path + link.substr( v_pos + 7, link.size() - 7 );
				}

				v_label->set_icon_link( link.c_str() );
			}
		}


		v_layer->m_clip_tree_root->attach_object( v_label );

		return NULL;
	}*/

	//
	// sound_style
	//
	object_base* dynamic_sound_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT )
			return NULL;

		document* v_doc = dynamic_cast<document*>( v_container );
		dynamic_layer* v_layer = dynamic_cast<dynamic_layer*>(parent_object);

		ustring v_name = L"";
		ustring v_guid = L"";
		ustring v_url = L"";
		bool v_repeat = true;

		TiXmlElement* v_xml_elem = (TiXmlElement*)v_xml_node;
		{
			char* str_temp = NULL;
			//  parse id
			if( (str_temp = (char*)v_xml_elem->Attribute( "id" )) != NULL )
			{
				v_guid = string_ext::to_wstring( str_temp );
			}
			// parse name
			if ( (str_temp = (char*)v_xml_elem->Attribute( "name" )) != NULL )
			{
				v_name = string_ext::to_wstring( str_temp );
			}
		}


		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT )
				continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;
			string str_label = v_child->Value();
			if ( str_label == "Repeat" )
			{				
				v_repeat = string_ext::to_bool( v_elem->GetText() );
			}
			else if ( str_label == "Link" )
			{
				v_url = string_ext::to_wstring( v_elem->GetText() );
			}
		}

		dynamic_sound* v_dynamic_sound = new dynamic_sound( v_url );

		v_dynamic_sound->set_repeat_mode( v_repeat );
		v_dynamic_sound->set_guid( v_guid.c_str() );
		v_dynamic_sound->set_name( v_name.c_str() );

		//v_layer->set_sound( v_dynamic_sound );


		//element_base* v_us_elem = dynamic_cast<element_base*>(parent_object);
		//v_us_elem->set_render_style( v_sound_style );

		return v_dynamic_sound;

	}
}