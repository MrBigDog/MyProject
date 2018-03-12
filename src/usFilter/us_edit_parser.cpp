///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_edit_parser.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : edit_parser class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "us_edit_parser.h"

namespace uniscope_globe
{
	//////////////////////////////////////////////////////////////////////////
	// xml_edit_model_layer_parser
	//////////////////////////////////////////////////////////////////////////
	//object_base* xml_edit_model_layer_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	//{
		//if ( v_xml_node->Type() != TiXmlNode::ELEMENT )
		//	return NULL;

		//document* v_doc = dynamic_cast<document*>( v_container );		
		//edit_system* v_system = v_doc->get_edit_system();

		//TiXmlElement *v_elem = (TiXmlElement *)v_xml_node;
		//ustring str_name = string_ext::to_wstring( v_elem->Attribute( "name" ) );
		//ustring str_guid = string_ext::to_wstring( v_elem->Attribute( "id" ) );

		//US_STRING_LOWER( str_guid );
		//edit_layer* v_layer = dynamic_cast<edit_layer*>( v_system->get_attached_object( str_guid.c_str() ) );
		//bool is_update = false;
		//if( v_layer )
		//{
		//	is_update = true;
		//	v_layer->delete_all_object();
		//	v_layer->set_name( str_name.c_str() );
		//	v_layer->set_guid( str_guid.c_str() );
		//}
		//else
		//{
		//	v_layer = new edit_layer( v_doc );
		//	v_layer->set_name( str_name.c_str() );
		//	v_layer->set_guid( str_guid.c_str()  );			
		//}					


		//ustring conn;
		//TiXmlNode* v_link_node = v_xml_node->FirstChild( "Connection" );
		//ustring v_connection;
		//if ( v_link_node )
		//{
		//	conn = string_ext::to_wstring( ((TiXmlElement*)v_link_node)->GetText() );
		//}
		//v_layer->set_server_connection( conn.c_str() );
		//v_layer->set_link( conn.c_str() );

		//for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		//{
		//	if ( v_child->Type() != TiXmlNode::ELEMENT )
		//		continue;

		//	TiXmlElement* v_elem = (TiXmlElement*)v_child;

		//	ustring str_label = string_ext::to_wstring( v_child->Value() );
		//	if( str_label == L"Connection" )
		//	{				
		//	}
		//	else
		//	{
		//		xml_parser_base* v_parser = m_parser_mngr->get_parser( str_label.c_str() );
		//		if( v_parser )
		//			v_parser->forward_parse( v_container, v_elem, v_layer );
		//	}
		//}

		//if( !is_update )
		//{
		//	if( !v_system->attach_object( v_layer ) )
		//	{
		//		AUTO_DELETE( v_layer );
		//		return NULL;
		//	}

		//	if( !v_doc->register_object( v_layer ) )
		//	{
		//		v_system->detach_object( v_layer );
		//		AUTO_DELETE( v_layer );
		//		return NULL;
		//	}
		//}

		//BSTR v_guid = ::SysAllocString( str_guid.c_str() );
		//US_SEND_MESSAGE( v_doc->get_application()->get_hwnd(), WM_USER_IMPORT_XML_SUCCEEDED, (WPARAM)NULL, (LPARAM)v_guid );

		//return v_layer;

		//return NULL;
	//}

	//////////////////////////////////////////////////////////////////////////
	// xml_edit_model_parser
	//////////////////////////////////////////////////////////////////////////
	//object_base* xml_edit_model_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	//{
	//	return NULL;
		//if ( v_xml_node->Type() != TiXmlNode::ELEMENT )
		//	return NULL;

		//document* v_doc = dynamic_cast<document*>( v_container );
		//element_model_set* v_model_set = dynamic_cast<element_model_set*>( parent_object );
		//if ( v_model_set == NULL )
		//{
		//	return NULL;
		//}

		//ustring str_id, str_name;
		//TiXmlElement *v_elem = (TiXmlElement *)v_xml_node;
		//{
		//	char* str_temp = NULL;

		//	//  parse id
		//	if( (str_temp = (char*)v_elem->Attribute( "id" )) != NULL )
		//	{
		//		str_id = string_ext::to_wstring( str_temp );
		//	}
		//	if( (str_temp = (char*)v_elem->Attribute( "name" )) != NULL )
		//	{
		//		str_name = string_ext::to_wstring( str_temp );
		//	}
		//}

		//element_model* v_model = NULL;
		//v_model = dynamic_cast<element_model*>( v_model_set->get_child( str_id.c_str() ) );

		//bool is_update = false;
		//if( v_model == NULL )
		//{
		//	v_model = new element_model( v_doc );
		//	v_model->set_document(v_doc);
		//	v_model->set_guid( str_id.c_str() );
		//	v_model->set_name( str_name.c_str() );			
		//}
		//else
		//{
		//	is_update = true;
		//	v_model->set_guid( str_id.c_str() );
		//	v_model->set_name( str_name.c_str() );
		//}

		//ustring v_link;
		//bool v_visibility = true;
		//bool v_selectable = true;
		//for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		//{
		//	if ( v_child->Type() != TiXmlNode::ELEMENT )
		//		continue;

		//	TiXmlElement* v_elem = (TiXmlElement*)v_child;

		//	ustring str_label = string_ext::to_wstring( v_child->Value() );

		//	if( str_label == L"BBox" )
		//	{
		//		xml_parser_base* v_parser = m_parser_mngr->get_parser( str_label.c_str() );
		//		v_parser->forward_parse( v_container, v_elem, v_model );
		//	}			
		//	else if( str_label == L"Visibility" )
		//	{
		//		v_visibility = string_ext::to_bool( v_elem->GetText() );
		//	}
		//	else if ( str_label == L"Link" )
		//	{
		//		v_link = string_ext::to_wstring( v_elem->GetText() );
		//	}			
		//}

		//v_model->set_visible( v_visibility );
		//v_model->set_selectable( v_selectable );

		//ustring::size_type v_pos1 = v_link.find( L"http://" );
		//ustring::size_type v_pos2 = v_link.find( L"db://" );
		//ustring::size_type v_pos3 = v_link.find( L":\\" );

		//if( v_pos1 == ustring::npos && v_pos2 == ustring::npos && v_pos3 == ustring::npos )
		//{
		//	v_link = v_model_set->get_link() + v_link;
		//}
		//v_model->set_link( v_link.c_str() );

		//if( !is_update )
		//{
		//	v_model_set->add_child( v_model );
		//}

		//return v_model;
//	}

	//object_base* xml_edit_model_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	//{
	//	if ( v_xml_node->Type() != TiXmlNode::ELEMENT )
	//		return NULL;

	//	document* v_doc = dynamic_cast<document*>( v_container );
	//	edit_layer* v_layer = dynamic_cast<edit_layer*>( parent_object );

	//	TiXmlElement *v_elem = (TiXmlElement *)v_xml_node;
	//	ustring str_id = string_ext::to_wstring( v_elem->Attribute( "id" ) );
	//	GUID guid;
	//	UuidFromString( (RPC_WSTR)str_id.c_str() , &guid );
	//	ustring str_name = string_ext::to_wstring( v_elem->Attribute( "name" ) );

	//	US_STRING_LOWER( str_id )

	//	edit_model* v_model = NULL;
	//	v_model = dynamic_cast<edit_model*>(v_layer->get_attached_object( str_id.c_str() ));
	//	bool is_update = false;
	//	if( v_model == NULL )
	//	{
	//		v_model = new edit_model( v_doc->get_application()->get_view(), v_doc->get_application()->get_manager_group() );
	//		v_model->set_guid( guid );
	//		v_model->set_name( str_name.c_str() );			
	//	}
	//	else
	//	{
	//		is_update = true;
	//		v_model->set_guid( guid );
	//		v_model->set_name( str_name.c_str() );
	//	}
	//	
	//	vector3<double> v_pos_sphr;
	//	vector3<double> v_pivot;
	//	vector3<double> v_scale( 1.0, 1.0, 1.0 );		

	//	double v_heading = 0.0;
	//	double v_tilt = 90.0;
	//	double v_roll = 0.0;
	//	ustring v_link;
	//	bool v_visibility = true;

	//	for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
	//	{
	//		if ( v_child->Type() != TiXmlNode::ELEMENT )
	//			continue;

	//		TiXmlElement* v_elem = (TiXmlElement*)v_child;

	//		ustring str_label = string_ext::to_wstring( v_child->Value() );

	//		if( str_label == L"Location" )
	//		{
	//			v_pos_sphr = xml_parser_ext::to_vector_3d( v_elem->GetText() );
	//		}			
	//		else if ( str_label == L"EulerRotation" )
	//		{
	//			vector3<double> v_Euler_angle = xml_parser_ext::to_vector_3d( v_elem->GetText() );
	//			v_heading = v_Euler_angle.x * RADIAN;
	//			v_tilt    = v_Euler_angle.y * RADIAN;
	//			v_roll    = v_Euler_angle.z * RADIAN;
	//		}
	//		else if( str_label == L"Heading" )
	//		{
	//			v_heading = string_ext::to_double( v_elem->GetText() ) * RADIAN;
	//		}
	//		else if( str_label == L"Tilt" )
	//		{
	//			v_tilt = string_ext::to_double( v_elem->GetText() ) * RADIAN;
	//		}
	//		else if( str_label == L"Roll" )
	//		{
	//			v_roll = string_ext::to_double( v_elem->GetText() ) * RADIAN;
	//		}
	//		else if( str_label == L"Pivot" )
	//		{
	//			v_pivot = xml_parser_ext::to_vector_3d( v_elem->GetText() );
	//		}
	//		else if( str_label == L"Scaling" )
	//		{
	//			v_scale = xml_parser_ext::to_vector_3d( v_elem->GetText() );
	//		}
	//		else if( str_label == L"BBox" )
	//		{
	//			xml_parser_base* v_parser = m_parser_mngr->get_parser( str_label.c_str() );
	//			v_parser->forward_parse( v_container, v_elem, v_model );
	//		}			
	//		else if( str_label == L"Visibility" )
	//		{
	//			v_visibility = string_ext::to_bool( v_elem->GetText() );
	//		}
	//		else if ( str_label == L"Link" )
	//		{
	//			v_link = string_ext::to_wstring( v_elem->GetText() );
	//		}			
	//	}

	//	v_model->set_visible( v_visibility );

	//	v_model->create_from_sphr(	v_pos_sphr.x * RADIAN, 
	//								v_pos_sphr.y * RADIAN,
	//								v_pos_sphr.z * 0.001 + spherical_coords::s_semi_major,
	//								v_heading,
	//								v_tilt, 
	//								v_roll );

	//	v_model->set_pivot( v_pivot );

	//	v_model->set_scale( v_scale );

	//	ustring::size_type v_pos1 = v_link.find( L"http://" );
	//	ustring::size_type v_pos2 = v_link.find( L"db://" );
	//	ustring::size_type v_pos3 = v_link.find( L":\\" );

	//	if( v_pos1 == ustring::npos && v_pos2 == ustring::npos && v_pos3 == ustring::npos )
	//	{
	//		v_link = v_layer->get_server_connection() + v_link;
	//	}

	//	v_model->set_link( v_link.c_str() );

	//	if( !is_update )
	//	{
	//		if( !v_layer->attach_object( v_model ) )
	//		{
	//			AUTO_DELETE( v_model );
	//			return NULL;
	//		}

	//		if( !v_doc->register_object( v_model ) )
	//		{
	//			v_layer->detach_object( v_model );
	//			AUTO_DELETE( v_model );
	//			return NULL;
	//		}
	//	}
	//	return v_model;
	//}

	//object_base* xml_edit_billboard_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	//{
	//	if ( v_xml_node->Type() != TiXmlNode::ELEMENT )
	//		return NULL;

	//	document* v_doc = dynamic_cast<document*>( v_container );
	//	edit_layer* v_layer = dynamic_cast<edit_layer*>( parent_object );

	//	TiXmlElement *v_elem = (TiXmlElement *)v_xml_node;
	//	ustring str_id = string_ext::to_wstring( v_elem->Attribute( "id" ) );
	//	GUID guid;
	//	UuidFromString( (RPC_WSTR)str_id.c_str() , &guid );
	//	ustring str_name = string_ext::to_wstring( v_elem->Attribute( "name" ) );

	//	US_STRING_LOWER( str_id )

	//	edit_billboard* v_billboard = NULL;
	//	v_billboard = dynamic_cast<edit_billboard*>( v_layer->get_attached_object( str_id.c_str() ) );
	//	bool is_update = false;
	//	if( v_billboard )
	//	{
	//		is_update = true;
	//		v_billboard->set_guid( guid );
	//		v_billboard->set_name( str_name.c_str() );
	//	}
	//	else
	//	{
	//		v_billboard = new edit_billboard( v_doc->get_application()->get_view(), v_doc->get_application()->get_manager_group() );
	//		v_billboard->set_guid( guid );
	//		v_billboard->set_name( str_name.c_str() );
	//	}

	//	vector3<double> v_pos_sphr;
	//	vector3<double> v_pivot;
	//	vector3<double> v_scale;

	//	ustring v_link;
	//	bool v_visibility = true;

	//	for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
	//	{
	//		if ( v_child->Type() != TiXmlNode::ELEMENT )
	//			continue;

	//		TiXmlElement* v_elem = (TiXmlElement*)v_child;

	//		ustring str_label = string_ext::to_wstring( v_child->Value() );

	//		if( str_label == L"Location" )
	//		{
	//			v_pos_sphr = xml_parser_ext::to_vector_3d( v_elem->GetText() );
	//		}			
	//		else if( str_label == L"Pivot" )
	//		{
	//			v_pivot = xml_parser_ext::to_vector_3d( v_elem->GetText() );
	//		}
	//		else if( str_label == L"Scaling" )
	//		{
	//			v_scale = xml_parser_ext::to_vector_3d( v_elem->GetText() );
	//		}
	//		else if( str_label == L"Visibility" )
	//		{
	//			v_visibility = string_ext::to_bool( v_elem->GetText() );
	//		}
	//		else if ( str_label == L"Link" )
	//		{
	//			v_link = string_ext::to_wstring( v_elem->GetText() );
	//		}
	//		else if( str_label == L"BBox" )
	//		{
	//			xml_parser_base* v_parser = m_parser_mngr->get_parser( str_label.c_str() );
	//			v_parser->forward_parse( v_container, v_elem, v_billboard );
	//		}
	//	}

	//	v_billboard->set_visible( v_visibility );

	//	v_billboard->create_from_sphr(	v_pos_sphr.x * RADIAN, 
	//									v_pos_sphr.y * RADIAN,
	//									v_pos_sphr.z * 0.001 + spherical_coords::s_semi_major,
	//									0.0,
	//									90.0 * RADIAN, 
	//									0.0 );

	//	v_billboard->set_pivot( v_pivot );

	//	v_billboard->set_scale( v_scale );

	//	ustring::size_type v_pos1 = v_link.find( L"http://" );
	//	ustring::size_type v_pos2 = v_link.find( L"db://" );
	//	ustring::size_type v_pos3 = v_link.find( L":\\" );

	//	if( v_pos1 == ustring::npos && v_pos2 == ustring::npos && v_pos3 == ustring::npos )
	//	{
	//		v_link = v_layer->get_server_connection() + v_link;
	//	}

	//	v_billboard->set_link( v_link.c_str() );

	//	if( !is_update )
	//	{
	//		if( !v_layer->attach_object( v_billboard ) )
	//		{
	//			AUTO_DELETE( v_billboard );
	//			return NULL;
	//		}

	//		if( !v_doc->register_object( v_billboard ) )			 
	//		{
	//			v_layer->detach_object( v_billboard );
	//			AUTO_DELETE( v_billboard );
	//			return NULL;
	//		}
	//	}

	//	return v_billboard;
	//}

	//
	// element_model_set parser
	//
	//object_base* xml_element_model_set_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	//{
	//	return NULL;

		//if ( v_xml_node->Type() != TiXmlNode::ELEMENT )
		//	return NULL;

		//document* v_doc = dynamic_cast<document*>( v_container );
		//element_folder* v_folder = dynamic_cast<element_folder*>( parent_object );
		//if ( v_folder == NULL )
		//{
		//	v_folder = v_doc->get_element_root();
		//}

		//ustring str_id, str_name, v_link;
		//TiXmlElement *v_elem = (TiXmlElement *)v_xml_node;
		//{
		//	char* str_temp = NULL;

		//	//  parse id
		//	if( (str_temp = (char*)v_elem->Attribute( "id" )) != NULL )
		//	{
		//		str_id = string_ext::to_wstring( str_temp );
		//	}
		//	if( (str_temp = (char*)v_elem->Attribute( "name" )) != NULL )
		//	{
		//		str_name = string_ext::to_wstring( str_temp );
		//	}
		//	if( (str_temp = (char*)v_elem->Attribute( "link" )) != NULL )
		//	{
		//		v_link = string_ext::to_wstring( str_temp ) + L"\\";
		//	}
		//}

		//element_model_set* v_model_set = NULL;
		//v_model_set = dynamic_cast<element_model_set*>( v_doc->get_register_object( str_id.c_str() ) );
		//bool is_update = false;
		//if( v_model_set )
		//{
		//	is_update = true;
		//	v_model_set->set_guid( str_id.c_str() );
		//	v_model_set->set_name( str_name.c_str() );
		//	v_model_set->set_link( v_link.c_str() );
		//}
		//else
		//{
		//	v_model_set = new element_model_set();
		//	v_model_set->set_document(v_doc);
		//	v_model_set->set_guid( str_id.c_str() );
		//	v_model_set->set_name( str_name.c_str() );
		//	v_model_set->set_link( v_link.c_str() );
		//}

		//vector3<double> v_pos_sphr;
		//vector3<double> v_scale;

		//bool v_visibility = true;

		//for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		//{
		//	if ( v_child->Type() != TiXmlNode::ELEMENT )
		//		continue;

		//	TiXmlElement* v_elem = (TiXmlElement*)v_child;

		//	ustring str_label = string_ext::to_wstring( v_child->Value() );

		//	if ( str_label == L"SphericalTransform" )
		//	{
		//		spherical_transform_parser v_parser;
		//		v_parser.forward_parse( NULL, v_elem, v_model_set );
		//	}
		//	else if( str_label == L"Visibility" )
		//	{
		//		v_visibility = string_ext::to_bool( v_elem->GetText() );
		//	}
		//	//else if ( str_label == L"SceneGraph" )
		//	//{
		//	//	xml_parser_base* v_parser = m_parser_mngr->get_parser( str_label.c_str() );
		//	//	if( v_parser )
		//	//	{
		//	//		v_parser->forward_parse( v_container, v_elem, v_model_set );
		//	//	}
		//	//}
		//	else
		//	{
		//		xml_parser_base* v_parser = m_parser_mngr->get_parser( str_label.c_str() );
		//		if( v_parser )
		//		{
		//			v_parser->forward_parse( v_container, v_elem, v_model_set );
		//		}
		//	}
		//	//else if ( str_label == L"EditModel" )
		//	//{
		//	//	xml_parser_base* v_parser = m_parser_mngr->get_parser( str_label.c_str() );
		//	//	if( v_parser )
		//	//	{
		//	//		v_parser->forward_parse( v_container, v_elem, v_model_set );
		//	//	}
		//	//}
		//	//else if ( str_label == L"EditBillboard" )
		//	//{
		//	//	xml_parser_base* v_parser = m_parser_mngr->get_parser( str_label.c_str() );
		//	//	if( v_parser )
		//	//	{
		//	//		v_parser->forward_parse( v_container, v_elem, v_model_set );
		//	//	}
		//	//}
		//}

		//v_model_set->set_visible( v_visibility );
		//		
		//if( !is_update )
		//{
		//	if( !v_folder->attach_object( v_model_set ) )
		//	{
		//		AUTO_DELETE( v_model_set );
		//		return NULL;
		//	}

		//	if( !v_doc->register_object( v_model_set ) )			 
		//	{
		//		v_folder->detach_object( v_model_set );
		//		AUTO_DELETE( v_model_set );
		//		return NULL;
		//	}
		//}

		//return v_model_set;
	//};

	//object_base* xml_edit_add_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	//{
	//	document* v_doc = dynamic_cast<document*>( v_container );
	//	edit_system* v_system = v_doc->get_edit_system();

	//	TiXmlElement *v_elem = (TiXmlElement *)v_xml_node;		
	//	ustring str_id = string_ext::to_wstring( v_elem->Attribute( "id" ) );
	//	GUID guid;
	//	UuidFromString( (RPC_WSTR)str_id.c_str() , &guid );

	//	US_STRING_LOWER( str_id )

	//		edit_layer* v_layer = dynamic_cast<edit_layer*>( v_system->get_attached_object( str_id.c_str() ) );
	//	if( v_layer == NULL ) return NULL;

	//	bool v_error = false;

	//	for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
	//	{
	//		if ( v_child->Type() != TiXmlNode::ELEMENT )
	//			continue;

	//		TiXmlElement* v_elem = (TiXmlElement*)v_child;

	//		ustring str_label = string_ext::to_wstring( v_child->Value() );

	//		xml_parser_base* v_parser = m_parser_mngr->get_parser( str_label.c_str() );
	//		if( v_parser->forward_parse( v_container, v_elem, v_layer ) == NULL )
	//			v_error = true;
	//	}

	//	if( v_error ) return NULL;

	//	BSTR v_guid = ::SysAllocString( str_id.c_str() );
	//	US_SEND_MESSAGE( v_doc->get_application()->get_hwnd(), WM_USER_IMPORT_XML_SUCCEEDED, (WPARAM)NULL, (LPARAM)v_guid );

	//	return v_layer;
	//}

}