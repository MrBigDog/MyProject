///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_scene_manager_parsers.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : scene_manager_parsers class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_scene_parsers.h"

namespace uniscope_globe
{
	// scene_link parser
	object_base* xml_scene_link_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT )
			return NULL;

		document* v_doc = dynamic_cast<document*>( v_container );		
		dynamic_system* v_system = v_doc->get_dynamic_system();

		TiXmlElement *v_elem = (TiXmlElement *)v_xml_node;		
		ustring str_name = string_ext::to_wstring( v_elem->Attribute( "name" ) );
		ustring str_guid = string_ext::to_wstring( v_elem->Attribute( "id" ) );

		US_STRING_LOWER( str_guid );
		dynamic_layer* v_layer = dynamic_cast<dynamic_layer*>( v_system->get_attached_object( str_guid.c_str() ) );
		if( v_layer ) return NULL;

		v_layer = new dynamic_layer;	
		v_layer->initialize( v_doc );

		TiXmlNode* v_link_node = v_xml_node->FirstChild( "Link" );
		//TiXmlNode* v_conn_node = v_xml_node->FirstChild( "Connection" );
		ustring v_connection;
		if ( v_link_node )
		{
			v_connection = string_ext::to_wstring( ((TiXmlElement*)v_link_node)->GetText() );
			ustring::size_type v_pos = v_connection.find_last_of( '/' );
			if(v_pos != ustring::npos)
			{
				v_connection = v_connection.substr( 0, v_pos + 1 );
			}
			else
			{
				ustring::size_type v_pos = v_connection.find_last_of( '\\' );
				if(v_pos != ustring::npos)
					v_connection = v_connection.substr( 0, v_pos + 1 );
			}
			
		}
		//else if( v_conn_node )
		//{
		//	v_connection = string_ext::to_wstring( ((TiXmlElement*)v_conn_node)->GetText() );
		//	ustring::size_type v_pos = v_connection.find_last_of( '/' );
		//	v_connection = v_connection.substr( 0, v_pos + 1 );
		//}

		v_layer->set_connection( v_connection.c_str() );

		vector_3d vec_location( 0.0, 0.0, 0.0 ), vec_pose( 0.0, 0.0, 0.0 );
		vector_3d v_scaling(1.0,1.0,1.0);
		bool visible = true;
		ustring v_script;
		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT )
				continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			ustring str_label = string_ext::to_wstring( v_child->Value() );

			if( str_label == L"Scene" )
			{
				xml_parser_base* v_parser = m_parser_mngr->get_parser( str_label.c_str() );

				v_parser->forward_parse( v_container, v_elem, v_layer );
			}
			else if ( str_label == L"GeoSpatial" )
			{
				vec_location = xml_parser_ext::to_vector_3d( v_elem->GetText() );				
			}
			else if ( str_label == L"Location" )
			{
				vec_location = xml_parser_ext::to_vector_3d( v_elem->GetText() );
			}
			else if ( str_label == L"Pose" )
			{
				vec_pose = xml_parser_ext::to_vector_3d( v_elem->GetText() );
			}
			else if ( str_label == L"Rotation" )
			{
				vec_pose = xml_parser_ext::to_vector_3d( v_elem->GetText() );
			}
			else if ( str_label == L"EulerRotation" )
			{
				vec_pose = xml_parser_ext::to_vector_3d( v_elem->GetText() );
			}
			else if ( str_label == L"Quaternion" )
			{
				//vec_pose = xml_parser_ext::to_vector_3d( v_elem->GetText() );
			}
			else if( str_label == L"Scaling" )
			{
				v_scaling = xml_parser_ext::to_vector_3d( v_elem->GetText() );
			}
			else if( str_label == L"Visibility" )
			{
				visible = string_ext::to_bool(v_elem->GetText());
			}
			else if( str_label == L"AnimationScript" )
			{
				v_script = string_ext::to_wstring( v_elem->GetText() );
			}
			else if( str_label == L"AnimationLinkList" )
			{
				xml_parser_base* v_parser = m_parser_mngr->get_parser( str_label.c_str() );

				v_parser->forward_parse( v_container, v_elem, v_layer );
			}
		}

		v_layer->set_visible( visible );

		v_layer->get_spherical_transform()->create_from_sphr(	vec_location.x * RADIAN, 
									vec_location.y * RADIAN, 
									vec_location.z * 0.001 + spherical_coords::s_semi_major ,
									vec_pose.x * RADIAN, 
									vec_pose.y * RADIAN, 
									vec_pose.z * RADIAN );

		v_layer->get_spherical_transform()->set_scale(v_scaling);
		v_layer->set_name( str_name.c_str() );
		v_layer->set_guid( str_guid.c_str() );
		v_layer->set_script( v_script.c_str() );

		if( v_doc->register_object( v_layer ) )
		{
			v_system->attach_object( v_layer );	

			physical_system* v_phx_sys = ((application*)(v_doc->get_application()))->get_physical_system();

			v_phx_sys->add_body( str_guid.c_str(), v_layer );

			US_SEND_MESSAGE( v_doc->get_application()->get_hwnd(), WM_USER_SCENE_LOADED, ::SysAllocString( v_layer->get_guid().c_str() ), 0L ); 
		}
		else
		{
			AUTO_DELETE( v_layer );
		}

		return v_layer;
	}

	// scene_parser
	object_base* xml_scene_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT )
			return NULL;

		dynamic_layer* v_layer = dynamic_cast<dynamic_layer*>( parent_object );

		TiXmlElement *v_elem = (TiXmlElement *)v_xml_node;
		ustring str_guid ;
		if(v_elem->Attribute( "id" ) != NULL)
		{
			str_guid = string_ext::to_wstring( v_elem->Attribute( "id" ) );
			v_layer->set_guid( str_guid.c_str() );
		}

		ustring str_name;
		if(v_elem->Attribute( "name" ) != NULL)
		{
			str_name = string_ext::to_wstring( v_elem->Attribute( "name" ) );
			v_layer->set_name( str_name.c_str() );
		}

		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT )
				continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			ustring str_label = string_ext::to_wstring( v_child->Value() );
						
			xml_parser_base* v_parser = m_parser_mngr->get_parser( str_label.c_str() );
			if ( v_parser )
			{
				v_parser->forward_parse( v_container, v_elem, v_layer );
			}
		}	

		//scene_system* v_scene_system = dynamic_cast<scene_system*>( parent_object );
		//v_scene_system->add_scene_manager( v_layer );

		return NULL;
	}


	//	geo_spatial_parser
	object_base* xml_spherical_transform_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT )
			return NULL;

		spherical_object* v_so = dynamic_cast<spherical_object*>( parent_object );
		spherical_transform* v_spherical_transform = v_so->get_spherical_transform();

		TiXmlElement *v_elem = (TiXmlElement *)v_xml_node;

		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT )
				continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			ustring str_label = string_ext::to_wstring( v_child->Value() );

			if ( str_label == L"Position" )
			{
				double_list v_list = xml_parser_ext::to_double_array( v_elem->GetText() );
				if( v_list.size() <= 3 )
				{
					v_spherical_transform->create_from_sphr(	v_list[0] * RADIAN, // longitude
																v_list[1] * RADIAN, // latitude 
																v_list[2] * 0.001 + spherical_coords::s_semi_major, // altitude
																0.0, 
																0.0, 
																0.0 );
				}
				else
				{
					v_spherical_transform->create_from_sphr(v_list[0] * RADIAN, // longitude
															v_list[1] * RADIAN, // latitude 
															v_list[2] * 0.001 + spherical_coords::s_semi_major, // altitude
															v_list[3] * RADIAN, // heading
															v_list[4] * RADIAN, // tilt
															v_list[5] * RADIAN );
				}
			}
		}	

		return NULL;
	}

	//	birth_place_parser
	object_base* xml_birth_place_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT )
			return NULL;

		dynamic_layer* v_layer = dynamic_cast<dynamic_layer*>( parent_object );

		TiXmlElement *v_elem = (TiXmlElement *)v_xml_node;

		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT )
				continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			ustring str_label = string_ext::to_wstring( v_child->Value() );

			if ( str_label == L"Position" )
			{
				vector3<double> position = xml_parser_ext::to_vector_3d( v_elem->GetText() );
				v_layer->set_birth_place( position );
			}
		}	

		return NULL;
	}


	// bound_box_parser
	object_base* xml_bound_box_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT )
			return NULL;

		spatial_object* so = dynamic_cast<spatial_object*>( parent_object );

		vector3<double> min_bound, max_bound;

		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT )
				continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			ustring str_label = string_ext::to_wstring( v_child->Value() );

			if ( str_label == L"MinBoundary")
			{
				min_bound = xml_parser_ext::to_vector_3d( v_elem->GetText() );
			}
			else if (str_label == L"MaxBoundary")
			{
				max_bound = xml_parser_ext::to_vector_3d( v_elem->GetText() );
			}
		}	

		axis_aligned_box<double> box( min_bound, max_bound );

		so->set_aabb( box );

		return NULL;
	}


	//
	//	model_parser
	//
	object_base* xml_model_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT )
			return NULL;

		document* v_doc = dynamic_cast<document*>( v_container );
		dynamic_layer* v_layer = dynamic_cast<dynamic_layer*>( parent_object );

		common_dynamic_model* v_model = new common_dynamic_model( v_layer );

		TiXmlElement *v_elem = (TiXmlElement *)v_xml_node;

		ustring str_name, str_id;

		const char* tmp_value = v_elem->Attribute( "id" );
		if ( tmp_value )
		{
			str_id = string_ext::to_wstring( v_elem->Attribute( "id" ) );
			v_model->set_guid( str_id.c_str() );
		}
		
		tmp_value = v_elem->Attribute("name");
		if ( tmp_value )
		{
			str_name = string_ext::to_wstring( v_elem->Attribute( "name" ) );
			v_model->set_name( str_name.c_str() );
		}

		bool v_alpha_blend = false;
		bool v_visibile = true;

		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT )
				continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			ustring str_label = string_ext::to_wstring( v_child->Value() );

			if ( str_label == L"Position" )
			{
				vector3<float> position =  xml_parser_ext::to_vector_3f( v_elem->GetText() );

				v_model->set_position( position );
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
			else if ( str_label == L"Scaling" )
			{
				vector3<float> v_scaling =  xml_parser_ext::to_vector_3f( v_elem->GetText() );

				v_model->set_scale( v_scaling );
			}
			else if ( str_label == L"Visibility" )
			{
				v_visibile = string_ext::to_bool( v_elem->GetText() );
			}
			else if ( str_label == L"Intersectable" )
			{
				v_model->set_intersectable( string_ext::to_bool( v_elem->GetText() ) );
			}
			else if ( str_label == L"AlphaBlend" )
			{
				v_alpha_blend = string_ext::to_bool( v_elem->GetText() );
			}
			else if ( str_label == L"BBox" )
			{
				xml_parser_base *v_parser = m_parser_mngr->get_parser( str_label.c_str() );

				v_parser->forward_parse( v_container, v_elem, v_model );
			}
			else if ( str_label == L"Link" )
			{
				ustring link = string_ext::to_wstring( v_elem->GetText() );

				ustring::size_type v_pos1 = link.find( L"http://" );
				ustring::size_type v_pos2 = link.find( L"db://" );
				ustring::size_type v_pos3 = link.find( L":\\" );
				ustring::size_type v_pos4 = link.find( L"uds://" );

				if( v_pos1 == ustring::npos && v_pos2 == ustring::npos && v_pos3 == ustring::npos && v_pos4 == ustring::npos )
				{
					link = v_layer->get_connection() + link;
				}

				v_model->set_mesh_link( link.c_str() );
			}
		}

		v_model->set_alpha_blend( v_alpha_blend );
		v_model->set_visible( v_visibile );

		cartesian_transform* v_cartesian_transform = dynamic_cast<cartesian_transform*>( v_model );
		axis_aligned_box<double>& v_aabb = v_model->get_aabb();
		v_aabb.transform( v_cartesian_transform->get_matrix_d() );

		v_layer->m_clip_tree_root->attach_object( v_model );

		return NULL;
	}

	//
	//	volume_parser
	//
	object_base* xml_volume_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT )
			return NULL;

		//document* v_doc = dynamic_cast<document*>( v_container );
		//dynamic_layer* v_layer = dynamic_cast<dynamic_layer*>( parent_object );

		//dynamic_volume* v_volume = new dynamic_volume();

		//TiXmlElement *v_elem = (TiXmlElement *)v_xml_node;

		//ustring str_guid = string_ext::to_wstring( v_elem->Attribute( "id" ) );
		//v_volume->set_guid( str_guid.c_str() );

		//ustring str_name = string_ext::to_wstring( v_elem->Attribute( "name" ) );		
		//v_volume->set_name( str_name.c_str() );

		//ulong v_color = 0xFFFFCC, v_line_color = 0xCCCCCC;
		//char* tmp = (char*)v_elem->Attribute( "color" );
		//if ( tmp )
		//{
		//	v_color = string_ext::to_dword( tmp );
		//}
		//tmp = (char*)v_elem->Attribute( "line_color" );
		//if ( tmp )
		//{
		//	v_line_color = string_ext::to_dword( tmp );
		//}

		//double v_height = 0.0;
		//point_list_d pt_list;
		//volume_mesh* v_mesh = new volume_mesh;

		//for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		//{
		//	if ( v_child->Type() != TiXmlNode::ELEMENT )
		//		continue;

		//	TiXmlElement* v_elem = (TiXmlElement*)v_child;

		//	ustring str_label = string_ext::to_wstring( v_child->Value() );

		//	if ( str_label == L"Position" )
		//	{
		//		vector3<float> position =  xml_parser_ext::to_vector_3f( v_elem->GetText() );

		//		v_volume->set_position( position );
		//	}
		//	else if ( str_label == L"Rotation" )
		//	{
		//		vector3<float> rotation =  xml_parser_ext::to_vector_3f( v_elem->GetText() );
		//		quaternion<float> quater = quaternion<float>::from_euler(rotation.x, rotation.y, rotation.z);
		//		v_volume->set_rotation( quater );
		//	}
		//	else if ( str_label == L"Quaternion" )
		//	{
		//		quaternion<float> quater = xml_parser_ext::to_quaternion_4f( v_elem->GetText() );
		//		v_volume->set_rotation( quater );
		//	}
		//	else if ( str_label == L"Scaling" )
		//	{
		//		vector3<float> v_scaling =  xml_parser_ext::to_vector_3f( v_elem->GetText() );

		//		v_volume->set_scale( v_scaling );
		//	}
		//	else if ( str_label == L"BBox" )
		//	{
		//		xml_parser_base *v_parser = m_parser_mngr->get_parser( str_label.c_str() );

		//		v_parser->forward_parse( v_container, v_elem, v_volume );
		//	}
		//	else if ( str_label == L"Visibility" )
		//	{
		//		v_volume->set_visible( string_ext::to_bool( v_elem->GetText() ) );
		//	}
		//	else if ( str_label == L"Intersectable" )
		//	{
		//		v_volume->set_intersectable( string_ext::to_bool( v_elem->GetText() ) );
		//	}
		//	else if ( str_label == L"Polygon" )
		//	{
		//		pt_list = xml_parser_ext::to_point_list_d( v_elem->GetText() );
		//	}
		//	else if ( str_label == L"Height" )
		//	{
		//		v_height = string_ext::to_double( v_elem->GetText() );
		//	}
		//}

		//if ( pt_list.size() == 0 )
		//{
		//	AUTO_DELETE( v_mesh );
		//	AUTO_DELETE( v_volume );
		//}
		//else
		//{
		//	v_mesh->create_volume_from_volume( pt_list[0], pt_list.size(), v_height, v_color, v_line_color );

		//	v_volume->set_mesh( v_mesh );
		//	v_layer->m_clip_tree_root->attach_object( v_volume );
		//}

		return NULL;	
	}

	//
	//	box_parser
	//
	object_base* xml_box_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT )
			return NULL;

		//document* v_doc = dynamic_cast<document*>( v_container );
		//dynamic_layer* v_layer = dynamic_cast<dynamic_layer*>( parent_object );

		//dynamic_volume* v_volume = new dynamic_volume();

		//TiXmlElement *v_elem = (TiXmlElement *)v_xml_node;

		//ustring str_guid = string_ext::to_wstring( v_elem->Attribute( "id" ) );
		//v_volume->set_guid( str_guid.c_str() );

		//ustring str_name = string_ext::to_wstring( v_elem->Attribute( "name" ) );		
		//v_volume->set_name( str_name.c_str() );

		//ulong v_color = 0xFFFFFFCC, v_line_color = 0xFFCCCCCC;
		//const char* tmp = v_elem->Attribute( "color" );
		//if ( tmp )
		//{
		//	v_color = string_ext::to_dword( tmp );
		//}
		//tmp = (char*)v_elem->Attribute( "line_color" );
		//if ( tmp )
		//{
		//	v_line_color = string_ext::to_dword( tmp );
		//}

		//vector_3d vec_cen;
		//double v_height = 0.0, v_width = 0.0, v_length = 0.0;
		//
		//volume_mesh* v_mesh = new volume_mesh;
		//for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		//{
		//	if ( v_child->Type() != TiXmlNode::ELEMENT )
		//		continue;

		//	TiXmlElement* v_elem = (TiXmlElement*)v_child;

		//	ustring str_label = string_ext::to_wstring( v_child->Value() );

		//	if ( str_label == L"Position" )
		//	{
		//		vector3<float> position =  xml_parser_ext::to_vector_3f( v_elem->GetText() );

		//		v_volume->set_position( position );
		//	}
		//	else if ( str_label == L"Rotation" )
		//	{
		//		vector3<float> rotation =  xml_parser_ext::to_vector_3f( v_elem->GetText() );
		//		quaternion<float> quater = quaternion<float>::from_euler(rotation.x, rotation.y, rotation.z);
		//		v_volume->set_rotation( quater );
		//	}
		//	else if ( str_label == L"Quaternion" )
		//	{
		//		quaternion<float> quater = xml_parser_ext::to_quaternion_4f( v_elem->GetText() );
		//		v_volume->set_rotation( quater );
		//	}
		//	else if ( str_label == L"Scaling" )
		//	{
		//		vector3<float> v_scaling =  xml_parser_ext::to_vector_3f( v_elem->GetText() );

		//		v_volume->set_scale( v_scaling );
		//	}
		//	else if ( str_label == L"BBox" )
		//	{
		//		xml_parser_base *v_parser = m_parser_mngr->get_parser( str_label.c_str() );

		//		v_parser->forward_parse( v_container, v_elem, v_volume );
		//	}
		//	else if ( str_label == L"Visibility" )
		//	{
		//		v_volume->set_visible( string_ext::to_bool( v_elem->GetText() ) );
		//	}
		//	else if ( str_label == L"Intersectable" )
		//	{
		//		v_volume->set_intersectable( string_ext::to_bool( v_elem->GetText() ) );
		//	}
		//	else if ( str_label == L"Center" )
		//	{
		//		vec_cen = xml_parser_ext::to_vector_3d( v_elem->GetText() );
		//	}
		//	else if ( str_label == L"Length" )
		//	{
		//		v_length = string_ext::to_double( v_elem->GetText() );
		//	}
		//	else if ( str_label == L"Width" )
		//	{
		//		v_width = string_ext::to_double( v_elem->GetText() );
		//	}
		//	else if ( str_label == L"Height" )
		//	{
		//		v_height = string_ext::to_double( v_elem->GetText() );
		//	}
		//}

		//v_mesh->create_volume_from_box( vec_cen, v_length, v_width, v_height, v_color, v_line_color );

		//v_volume->set_mesh( v_mesh );
		//v_layer->m_clip_tree_root->attach_object( v_volume );

		return NULL;
	}

	//
	//	sphere_parser
	//
	object_base* xml_sphere_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT )
			return NULL;

	///*	dynamic_layer* v_layer = dynamic_cast<dynamic_layer*>( parent_object );

	//	dynamic_volume* v_volume = new dynamic_volume();

	//	TiXmlElement *v_elem = (TiXmlElement *)v_xml_node;

	//	ustring str_guid = string_ext::to_wstring( v_elem->Attribute( "id" ) );
	//	v_volume->set_guid( str_guid.c_str() );

	//	ustring str_name = string_ext::to_wstring( v_elem->Attribute( "name" ) );		
	//	v_volume->set_name( str_name.c_str() );

	//	ulong v_color = 0xFFFFFFCC, v_line_color = 0xFFCCCCCC;
	//	const char* tmp = v_elem->Attribute( "color" );
	//	if ( tmp )
	//	{
	//		v_color = string_ext::to_dword( tmp );
	//	}
	//	tmp = (char*)v_elem->Attribute( "line_color" );
	//	if ( tmp )
	//	{
	//		v_line_color = string_ext::to_dword( tmp );
	//	}

	//	vector_3d vec_cen;
	//	double v_radius = 0.0;

	//	volume_mesh* v_mesh = new volume_mesh;
	//	for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
	//	{
	//		if ( v_child->Type() != TiXmlNode::ELEMENT )
	//			continue;

	//		TiXmlElement* v_elem = (TiXmlElement*)v_child;

	//		ustring str_label = string_ext::to_wstring( v_child->Value() );

	//		if ( str_label == L"Position" )
	//		{
	//			vector3<float> position =  xml_parser_ext::to_vector_3f( v_elem->GetText() );

	//			v_volume->set_position( position );
	//		}
	//		else if ( str_label == L"Rotation" )
	//		{
	//			vector3<float> rotation =  xml_parser_ext::to_vector_3f( v_elem->GetText() );
	//			quaternion<float> quater = quaternion<float>::from_euler(rotation.x, rotation.y, rotation.z);
	//			v_volume->set_rotation( quater );
	//		}
	//		else if ( str_label == L"Quaternion" )
	//		{
	//			quaternion<float> quater = xml_parser_ext::to_quaternion_4f( v_elem->GetText() );
	//			v_volume->set_rotation( quater );
	//		}
	//		else if ( str_label == L"Scaling" )
	//		{
	//			vector3<float> v_scaling =  xml_parser_ext::to_vector_3f( v_elem->GetText() );

	//			v_volume->set_scale( v_scaling );
	//		}
	//		else if ( str_label == L"BBox" )
	//		{
	//			xml_parser_base *v_parser = m_parser_mngr->get_parser( str_label.c_str() );

	//			v_parser->forward_parse( v_container, v_elem, v_volume );
	//		}
	//		else if ( str_label == L"Visibility" )
	//		{
	//			v_volume->set_visible( string_ext::to_bool( v_elem->GetText() ) );
	//		}
	//		else if ( str_label == L"Intersectable" )
	//		{
	//			v_volume->set_intersectable( string_ext::to_bool( v_elem->GetText() ) );
	//		}
	//		else if ( str_label == L"Center" )
	//		{
	//			vec_cen = xml_parser_ext::to_vector_3d( v_elem->GetText() );
	//		}
	//		else if ( str_label == L"Radius" )
	//		{
	//			v_radius = string_ext::to_double( v_elem->GetText() );
	//		}
	//	}

	//	v_mesh->create_volume_from_sphere( vec_cen, v_radius, v_color, v_line_color );

	//	v_volume->set_mesh( v_mesh );
	//	v_layer->m_clip_tree_root->attach_object( v_volume );*/

		return NULL;
	};
	

	//
	//	cylinder_parser
	//
	object_base* xml_cylinder_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT )
			return NULL;

		//dynamic_layer* v_layer = dynamic_cast<dynamic_layer*>( parent_object );

		//dynamic_volume* v_volume = new dynamic_volume();

		//TiXmlElement *v_elem = (TiXmlElement *)v_xml_node;

		//ustring str_guid = string_ext::to_wstring( v_elem->Attribute( "id" ) );
		//v_volume->set_guid( str_guid.c_str() );

		//ustring str_name = string_ext::to_wstring( v_elem->Attribute( "name" ) );		
		//v_volume->set_name( str_name.c_str() );

		//ulong v_color = 0xFFFFFFCC, v_line_color = 0xFFCCCCCC;
		//const char* tmp = v_elem->Attribute( "color" );
		//if ( tmp )
		//{
		//	v_color = string_ext::to_dword( tmp );
		//}
		//tmp = (char*)v_elem->Attribute( "line_color" );
		//if ( tmp )
		//{
		//	v_line_color = string_ext::to_dword( tmp );
		//}

		//vector_3d vec_cen;
		//double v_radius = 0.0, v_height = 0.0;

		//volume_mesh* v_mesh = new volume_mesh;
		//for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		//{
		//	if ( v_child->Type() != TiXmlNode::ELEMENT )
		//		continue;

		//	TiXmlElement* v_elem = (TiXmlElement*)v_child;

		//	ustring str_label = string_ext::to_wstring( v_child->Value() );

		//	if ( str_label == L"Position" )
		//	{
		//		vector3<float> position =  xml_parser_ext::to_vector_3f( v_elem->GetText() );

		//		v_volume->set_position( position );
		//	}
		//	else if ( str_label == L"Rotation" )
		//	{
		//		vector3<float> rotation =  xml_parser_ext::to_vector_3f( v_elem->GetText() );
		//		quaternion<float> quater = quaternion<float>::from_euler(rotation.x, rotation.y, rotation.z);
		//		v_volume->set_rotation( quater );
		//	}
		//	else if ( str_label == L"Quaternion" )
		//	{
		//		quaternion<float> quater = xml_parser_ext::to_quaternion_4f( v_elem->GetText() );
		//		v_volume->set_rotation( quater );
		//	}
		//	else if ( str_label == L"Scaling" )
		//	{
		//		vector3<float> v_scaling =  xml_parser_ext::to_vector_3f( v_elem->GetText() );

		//		v_volume->set_scale( v_scaling );
		//	}
		//	else if ( str_label == L"BBox" )
		//	{
		//		xml_parser_base *v_parser = m_parser_mngr->get_parser( str_label.c_str() );

		//		v_parser->forward_parse( v_container, v_elem, v_volume );
		//	}
		//	else if ( str_label == L"Visibility" )
		//	{
		//		v_volume->set_visible( string_ext::to_bool( v_elem->GetText() ) );
		//	}
		//	else if ( str_label == L"Intersectable" )
		//	{
		//		v_volume->set_intersectable( string_ext::to_bool( v_elem->GetText() ) );
		//	}
		//	else if ( str_label == L"Center" )
		//	{
		//		vec_cen = xml_parser_ext::to_vector_3d( v_elem->GetText() );
		//	}
		//	else if ( str_label == L"Radius" )
		//	{
		//		v_radius = string_ext::to_double( v_elem->GetText() );
		//	}
		//	else if ( str_label == L"Height" )
		//	{
		//		v_height = string_ext::to_double( v_elem->GetText() );
		//	}
		//}

		//v_mesh->create_volume_from_cylinder( vec_cen, v_radius, v_height, v_color, v_line_color );

		//v_volume->set_mesh( v_mesh );
		//v_layer->m_clip_tree_root->attach_object( v_volume );

		return NULL;
	}
	
	//
	//	cone_parser
	//
	object_base* xml_cone_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT )
			return NULL;

		//dynamic_layer* v_layer = dynamic_cast<dynamic_layer*>( parent_object );

		//dynamic_volume* v_volume = new dynamic_volume();

		//TiXmlElement *v_elem = (TiXmlElement *)v_xml_node;

		//ustring str_guid = string_ext::to_wstring( v_elem->Attribute( "id" ) );
		//v_volume->set_guid( str_guid.c_str() );

		//ustring str_name = string_ext::to_wstring( v_elem->Attribute( "name" ) );		
		//v_volume->set_name( str_name.c_str() );

		//ulong v_color = 0xFFFFFFCC, v_line_color = 0xFFCCCCCC;
		//const char* tmp = v_elem->Attribute( "color" );
		//if ( tmp )
		//{
		//	v_color = string_ext::to_dword( tmp );
		//}
		//tmp = (char*)v_elem->Attribute( "line_color" );
		//if ( tmp )
		//{
		//	v_line_color = string_ext::to_dword( tmp );
		//}

		//vector_3d vec_cen;
		//double v_radius = 0.0, v_height = 0.0;

		//volume_mesh* v_mesh = new volume_mesh;
		//for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		//{
		//	if ( v_child->Type() != TiXmlNode::ELEMENT )
		//		continue;

		//	TiXmlElement* v_elem = (TiXmlElement*)v_child;

		//	ustring str_label = string_ext::to_wstring( v_child->Value() );

		//	if ( str_label == L"Position" )
		//	{
		//		vector3<float> position =  xml_parser_ext::to_vector_3f( v_elem->GetText() );

		//		v_volume->set_position( position );
		//	}
		//	else if ( str_label == L"Rotation" )
		//	{
		//		vector3<float> rotation =  xml_parser_ext::to_vector_3f( v_elem->GetText() );
		//		quaternion<float> quater = quaternion<float>::from_euler(rotation.x, rotation.y, rotation.z);
		//		v_volume->set_rotation( quater );
		//	}
		//	else if ( str_label == L"Quaternion" )
		//	{
		//		quaternion<float> quater = xml_parser_ext::to_quaternion_4f( v_elem->GetText() );
		//		v_volume->set_rotation( quater );
		//	}
		//	else if ( str_label == L"Scaling" )
		//	{
		//		vector3<float> v_scaling =  xml_parser_ext::to_vector_3f( v_elem->GetText() );

		//		v_volume->set_scale( v_scaling );
		//	}
		//	else if ( str_label == L"BBox" )
		//	{
		//		xml_parser_base *v_parser = m_parser_mngr->get_parser( str_label.c_str() );

		//		v_parser->forward_parse( v_container, v_elem, v_volume );
		//	}
		//	else if ( str_label == L"Visibility" )
		//	{
		//		v_volume->set_visible( string_ext::to_bool( v_elem->GetText() ) );
		//	}
		//	else if ( str_label == L"Intersectable" )
		//	{
		//		v_volume->set_intersectable( string_ext::to_bool( v_elem->GetText() ) );
		//	}
		//	else if ( str_label == L"Center" )
		//	{
		//		vec_cen = xml_parser_ext::to_vector_3d( v_elem->GetText() );
		//	}
		//	else if ( str_label == L"Radius" )
		//	{
		//		v_radius = string_ext::to_double( v_elem->GetText() );
		//	}
		//	else if ( str_label == L"Height" )
		//	{
		//		v_height = string_ext::to_double( v_elem->GetText() );
		//	}
		//}

		//v_mesh->create_volume_from_cone( vec_cen, v_radius, v_height, v_color, v_line_color );

		//v_volume->set_mesh( v_mesh );
		//v_layer->m_clip_tree_root->attach_object( v_volume );

		return NULL;
	}

	//
	//	prism_parser
	//
	object_base* xml_prism_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT )
			return NULL;

		//dynamic_layer* v_layer = dynamic_cast<dynamic_layer*>( parent_object );

		//dynamic_volume* v_volume = new dynamic_volume();

		//TiXmlElement *v_elem = (TiXmlElement *)v_xml_node;

		//ustring str_guid = string_ext::to_wstring( v_elem->Attribute( "id" ) );
		//v_volume->set_guid( str_guid.c_str() );

		//ustring str_name = string_ext::to_wstring( v_elem->Attribute( "name" ) );		
		//v_volume->set_name( str_name.c_str() );

		//ulong v_color = 0xFFFFFFCC, v_line_color = 0xFFCCCCCC;
		//const char* tmp = v_elem->Attribute( "color" );
		//if ( tmp )
		//{
		//	v_color = string_ext::to_dword( tmp );
		//}
		//tmp = (char*)v_elem->Attribute( "line_color" );
		//if ( tmp )
		//{
		//	v_line_color = string_ext::to_dword( tmp );
		//}

		//vector_3d vec_cen;
		//double v_radius = 0.0, v_height = 0.0;
		//long v_sides = 0;

		//volume_mesh* v_mesh = new volume_mesh;
		//for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		//{
		//	if ( v_child->Type() != TiXmlNode::ELEMENT )
		//		continue;

		//	TiXmlElement* v_elem = (TiXmlElement*)v_child;

		//	ustring str_label = string_ext::to_wstring( v_child->Value() );

		//	if ( str_label == L"Position" )
		//	{
		//		vector3<float> position =  xml_parser_ext::to_vector_3f( v_elem->GetText() );

		//		v_volume->set_position( position );
		//	}
		//	else if ( str_label == L"Rotation" )
		//	{
		//		vector3<float> rotation =  xml_parser_ext::to_vector_3f( v_elem->GetText() );
		//		quaternion<float> quater = quaternion<float>::from_euler(rotation.x, rotation.y, rotation.z);
		//		v_volume->set_rotation( quater );
		//	}
		//	else if ( str_label == L"Quaternion" )
		//	{
		//		quaternion<float> quater = xml_parser_ext::to_quaternion_4f( v_elem->GetText() );
		//		v_volume->set_rotation( quater );
		//	}
		//	else if ( str_label == L"Scaling" )
		//	{
		//		vector3<float> v_scaling =  xml_parser_ext::to_vector_3f( v_elem->GetText() );

		//		v_volume->set_scale( v_scaling );
		//	}
		//	else if ( str_label == L"BBox" )
		//	{
		//		xml_parser_base *v_parser = m_parser_mngr->get_parser( str_label.c_str() );

		//		v_parser->forward_parse( v_container, v_elem, v_volume );
		//	}
		//	else if ( str_label == L"Visibility" )
		//	{
		//		v_volume->set_visible( string_ext::to_bool( v_elem->GetText() ) );
		//	}
		//	else if ( str_label == L"Intersectable" )
		//	{
		//		v_volume->set_intersectable( string_ext::to_bool( v_elem->GetText() ) );
		//	}
		//	else if ( str_label == L"Center" )
		//	{
		//		vec_cen = xml_parser_ext::to_vector_3d( v_elem->GetText() );
		//	}
		//	else if ( str_label == L"Radius" )
		//	{
		//		v_radius = string_ext::to_double( v_elem->GetText() );
		//	}
		//	else if ( str_label == L"Height" )
		//	{
		//		v_height = string_ext::to_double( v_elem->GetText() );
		//	}
		//	else if ( str_label == L"Sides" )
		//	{
		//		v_sides = string_ext::to_double( v_elem->GetText() );
		//	}
		//}

		//v_mesh->create_volume_from_prism( vec_cen, v_radius, v_height, v_sides, v_color, v_line_color );

		//v_volume->set_mesh( v_mesh );
		//v_layer->m_clip_tree_root->attach_object( v_volume );

		return NULL;
	}
	

	//
	//	pyramid_parser
	//
	object_base* xml_pyramid_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT )
			return NULL;

		//dynamic_layer* v_layer = dynamic_cast<dynamic_layer*>( parent_object );

		//dynamic_volume* v_volume = new dynamic_volume();

		//TiXmlElement *v_elem = (TiXmlElement *)v_xml_node;

		//ustring str_guid = string_ext::to_wstring( v_elem->Attribute( "id" ) );
		//v_volume->set_guid( str_guid.c_str() );

		//ustring str_name = string_ext::to_wstring( v_elem->Attribute( "name" ) );		
		//v_volume->set_name( str_name.c_str() );

		//ulong v_color = 0xFFFFFFCC, v_line_color = 0xFFCCCCCC;
		//const char* tmp = v_elem->Attribute( "color" );
		//if ( tmp )
		//{
		//	v_color = string_ext::to_dword( tmp );
		//}
		//tmp = (char*)v_elem->Attribute( "line_color" );
		//if ( tmp )
		//{
		//	v_line_color = string_ext::to_dword( tmp );
		//}

		//vector_3d vec_cen;
		//double v_radius = 0.0, v_height = 0.0;
		//long v_sides = 0;

		//volume_mesh* v_mesh = new volume_mesh;
		//for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		//{
		//	if ( v_child->Type() != TiXmlNode::ELEMENT )
		//		continue;

		//	TiXmlElement* v_elem = (TiXmlElement*)v_child;

		//	ustring str_label = string_ext::to_wstring( v_child->Value() );

		//	if ( str_label == L"Position" )
		//	{
		//		vector3<float> position =  xml_parser_ext::to_vector_3f( v_elem->GetText() );

		//		v_volume->set_position( position );
		//	}
		//	else if ( str_label == L"Rotation" )
		//	{
		//		vector3<float> rotation =  xml_parser_ext::to_vector_3f( v_elem->GetText() );
		//		quaternion<float> quater = quaternion<float>::from_euler(rotation.x, rotation.y, rotation.z);
		//		v_volume->set_rotation( quater );
		//	}
		//	else if ( str_label == L"Quaternion" )
		//	{
		//		quaternion<float> quater = xml_parser_ext::to_quaternion_4f( v_elem->GetText() );
		//		v_volume->set_rotation( quater );
		//	}
		//	else if ( str_label == L"Scaling" )
		//	{
		//		vector3<float> v_scaling =  xml_parser_ext::to_vector_3f( v_elem->GetText() );

		//		v_volume->set_scale( v_scaling );
		//	}
		//	else if ( str_label == L"BBox" )
		//	{
		//		xml_parser_base *v_parser = m_parser_mngr->get_parser( str_label.c_str() );

		//		v_parser->forward_parse( v_container, v_elem, v_volume );
		//	}
		//	else if ( str_label == L"Visibility" )
		//	{
		//		v_volume->set_visible( string_ext::to_bool( v_elem->GetText() ) );
		//	}
		//	else if ( str_label == L"Intersectable" )
		//	{
		//		v_volume->set_intersectable( string_ext::to_bool( v_elem->GetText() ) );
		//	}
		//	else if ( str_label == L"Center" )
		//	{
		//		vec_cen = xml_parser_ext::to_vector_3d( v_elem->GetText() );
		//	}
		//	else if ( str_label == L"Radius" )
		//	{
		//		v_radius = string_ext::to_double( v_elem->GetText() );
		//	}
		//	else if ( str_label == L"Height" )
		//	{
		//		v_height = string_ext::to_double( v_elem->GetText() );
		//	}
		//	else if ( str_label == L"Sides" )
		//	{
		//		v_sides = string_ext::to_double( v_elem->GetText() );
		//	}
		//}

		//v_mesh->create_volume_from_pyramid( vec_cen, v_radius, v_height, v_sides, v_color, v_line_color );

		//v_volume->set_mesh( v_mesh );
		//v_layer->m_clip_tree_root->attach_object( v_volume );

		return NULL;
	}
	

	//
	// dummy_geometry parser
	//
	object_base* xml_dummy_geometry_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT )
			return NULL;

		document* v_doc = dynamic_cast<document*>( v_container );
		dynamic_layer* v_layer = dynamic_cast<dynamic_layer*>( parent_object );

		common_dynamic_model* v_model = new common_dynamic_model( v_layer );

		TiXmlElement *v_elem = (TiXmlElement *)v_xml_node;

		ustring str_guid = string_ext::to_wstring( v_elem->Attribute( "id" ) );
		ustring str_name = string_ext::to_wstring( v_elem->Attribute( "name" ) );

		v_model->set_guid( str_guid.c_str() );
		v_model->set_name( str_name.c_str() );

		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT )
				continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			ustring str_label = string_ext::to_wstring( v_child->Value() );

			if ( str_label == L"Position" )
			{
				vector3<float> position =  xml_parser_ext::to_vector_3f( v_elem->GetText() );

				v_model->set_position( position );
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
			else if ( str_label == L"Scaling" )
			{
				vector3<float> v_scaling =  xml_parser_ext::to_vector_3f( v_elem->GetText() );

				v_model->set_scale( v_scaling );
			}
			else if ( str_label == L"Visibility" )
			{
				v_model->set_visible( string_ext::to_bool( v_elem->GetText() ) );
			}
			else if ( str_label == L"Intersectable" )
			{
				v_model->set_intersectable( string_ext::to_bool( v_elem->GetText() ) );
			}
			else if ( str_label == L"BBox" )
			{
				xml_parser_base *v_parser = m_parser_mngr->get_parser( str_label.c_str() );

				v_parser->forward_parse( v_container, v_elem, v_model );
			}
			else if ( str_label == L"Link" )
			{
				ustring link = string_ext::to_wstring( v_elem->GetText() );

				ustring::size_type v_pos1 = link.find( L"http://" );
				ustring::size_type v_pos2 = link.find( L"db://" );
				ustring::size_type v_pos3 = link.find( L"uds://" );

				if( v_pos1 == ustring::npos && v_pos2 == ustring::npos && v_pos3 == ustring::npos )
				{
					link = v_layer->get_connection() + link;
				}

				v_model->set_mesh_link( link.c_str() );
			}
		}

		v_layer->m_clip_tree_root->attach_object( v_model );

		return NULL;
	}

	// scene_graph_parser
	object_base* xml_scene_graph_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT )
			return NULL;

		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT )
				continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			ustring str_label = string_ext::to_wstring( v_child->Value() );

			if ( str_label == L"Type" )
			{
			}
			else
			{
				xml_parser_base *v_parser = m_parser_mngr->get_parser( str_label.c_str() );

				v_parser->forward_parse( v_container, v_elem, parent_object );
			}

		}

		return NULL;//*/
	}

	// bill_board_set_parser
	object_base* xml_billboard_set_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT )
			return NULL;

		//document* v_doc = dynamic_cast<document*>( v_container );
		//dynamic_layer* v_layer = dynamic_cast<dynamic_layer*>( parent_object );

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

		//		ustring::size_type v_pos1 = v_link.find( L"http://" );
		//		ustring::size_type v_pos2 = v_link.find( L"db://" );

		//		if( v_pos1 == ustring::npos && v_pos2 == ustring::npos )
		//		{
		//			v_link = v_layer->get_connection() + v_link;
		//		}
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
		//

		//dynamic_billboard_set* v_bs = new dynamic_billboard_set( v_doc->get_application()->get_manager_group(), bd );
		//v_bs->set_name( str_name.c_str() );
		//v_bs->set_guid( str_guid.c_str() );

		//v_layer->m_clip_tree_root->attach_object( v_bs );

		return NULL;
	}


	object_base* xml_label_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
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
	}

	// animation_link_parser
	object_base* xml_animation_link_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT )
			return NULL;

		document* v_doc = dynamic_cast<document*>( v_container );
		dynamic_layer* v_layer = dynamic_cast<dynamic_layer*>(parent_object);

		TiXmlElement *v_elem = (TiXmlElement *)v_xml_node;

		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT )
				continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			ustring str_label = string_ext::to_wstring( v_child->Value() );

			if( str_label == L"AnimationLink" )
			{
				ustring v_link = string_ext::to_wstring( v_elem->GetText() );
				ustring::size_type v_pos1 = v_link.find( L"http://" );
				ustring::size_type v_pos2 = v_link.find( L"db://" );
				ustring::size_type v_pos3 = v_link.find( L":\\" );
				ustring::size_type v_pos4 = v_link.find( L"uds://" );

				if( v_pos1 == ustring::npos && v_pos2 == ustring::npos && v_pos3 == ustring::npos && v_pos4 == ustring::npos )
				{
					v_link = v_layer->get_connection() + v_link;
				}

				//animation_controller* v_controller = new animation_controller( v_doc->get_application()->m_manager_group );
				//v_controller->set_link( v_link.c_str() );

				//animation_manager* v_anim_manager = (animation_manager*)v_doc->get_application()->get_animation_manager();
				//animation_controller_set* v_animation_controller_set = v_anim_manager->get_controller_set( v_layer->get_guid().c_str() );
				//v_animation_controller_set->add_controller( v_link.c_str(), v_controller );
			}			
		}

		return NULL;
	}
}