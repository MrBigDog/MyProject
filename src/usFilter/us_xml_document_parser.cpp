///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_xml_document_parser.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : xml_document_parser class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "Stdafx.h"
#include "us_xml_document_parser.h"

namespace uniscope_globe
{
	/////////////////////////////////////////////////////////////////////////////
	//  sphere parser
	object_base * xml_globe_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT )
			return NULL;

		// parser child
		for ( TiXmlNode *v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT )
				continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			ustring str_label = string_ext::to_wstring( v_child->Value() );

			xml_parser_base *v_parser = m_parser_mngr->get_parser( str_label.c_str() );

			v_parser->forward_parse( v_container, v_elem, parent_object );
		}	

		return NULL;
	}

	//////////////////////////////////////////////////////////////////////////
	//	terrain parser
	object_base * xml_dem_system_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT )
			return NULL;

		document* v_document = (document*)v_container;

		// parser child
		for ( TiXmlNode *v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT )
				continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			ustring str_label = string_ext::to_wstring( v_child->Value() );

			xml_parser_base *v_parser = m_parser_mngr->get_parser( str_label.c_str() );

			v_parser->forward_parse( v_container, v_elem, v_document->get_sphere_system()->get_base_dem()->get_connection_set());
		}	

		return NULL;
	}

	//////////////////////////////////////////////////////////////////////////
	//	image parser
	object_base * xml_dom_system_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT )
			return NULL;

		document* v_document = (document*)v_container;
		// parser child
		for ( TiXmlNode *v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT )
				continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			ustring str_label = string_ext::to_wstring( v_child->Value() );

			xml_parser_base *v_parser = m_parser_mngr->get_parser( str_label.c_str() );

			v_parser->forward_parse( v_container, v_elem, v_document->get_sphere_system()->get_base_coverage()->get_connection_set());
		}	

		return NULL;
	}

	//////////////////////////////////////////////////////////////////////////
	//	primary coverage parser
	object_base * xml_primary_coverage_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT )
			return NULL;

		TiXmlElement *v_elem = (TiXmlElement *)v_xml_node;

		document* v_document = (document*)v_container;
		dom_coverage* v_coverage = v_document->get_sphere_system()->get_primary_coverage();

		int v_blend_factor = string_ext::to_int( v_elem->Attribute( "blend" ) );

		v_coverage->set_blend_factor( v_blend_factor );

		// parser child
		for ( TiXmlNode *v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT )
				continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			ustring str_label = string_ext::to_wstring( v_child->Value() );

			xml_parser_base *v_parser = m_parser_mngr->get_parser( str_label.c_str() );

			v_parser->forward_parse( v_container, v_elem, v_coverage->get_connection_set());
		}	

		return NULL;
	}

	//////////////////////////////////////////////////////////////////////////
	//	secondary coverage parser
	object_base * xml_secondary_coverage_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT )
			return NULL;

		TiXmlElement *v_elem = (TiXmlElement *)v_xml_node;

		document* v_document = (document*)v_container;

		dom_coverage* v_coverage = v_document->get_sphere_system()->get_secondary_coverage();

		int v_blend_factor = string_ext::to_int( v_elem->Attribute( "blend" ) );

		v_coverage->set_blend_factor( v_blend_factor );

		// parser child
		for ( TiXmlNode *v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT )
				continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			ustring str_label = string_ext::to_wstring( v_child->Value() );

			xml_parser_base *v_parser = m_parser_mngr->get_parser( str_label.c_str() );

			v_parser->forward_parse( v_container, v_elem, v_coverage->get_connection_set());
		}	

		return NULL;
	}

	//////////////////////////////////////////////////////////////////////////
	//	connnection_chunk parser
	object_base * xml_connection_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT )
			return NULL;

		TiXmlElement *v_elem = (TiXmlElement *)v_xml_node;

		// 读取属性
		ustring str_id = string_ext::to_wstring( v_elem->Attribute( "id" ) );
		ustring str_name = string_ext::to_wstring( v_elem->Attribute( "name" ) );

		// 读取子标签
		ustring str_url;
		int min_level = 0;
		int max_level = 0;
		long v_priority = 0;
		lon_lat_rect v_rect;

		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT )
				continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			ustring str_label = string_ext::to_wstring( v_child->Value() );

			if ( str_label == L"MinLevel" )
			{
				min_level = string_ext::to_int( v_elem->GetText() );
			}
			else if ( str_label == L"MaxLevel" )
			{
				max_level =  string_ext::to_int( v_elem->GetText() );
			}
			else if ( str_label == L"LonLatRect" )
			{
				v_rect = xml_parser_ext::to_lon_lat_rect( v_elem->GetText() );
			}
			else if ( str_label == L"Priority" )
			{
				v_priority = (long)string_ext::to_int( v_elem->GetText() );
			}
			else if ( str_label == L"Link" )
			{
				str_url = string_ext::to_wstring( v_elem->GetText() );
			}
		}

		// new connection
		connection* v_connction = new connection;
		v_connction->set_name( str_name.c_str() );
		v_connction->set_guid( str_id.c_str() );
		v_connction->set_server_connection( str_url.c_str() );
		v_connction->set_layer_rect( v_rect );
		v_connction->set_min_level( min_level );
		v_connction->set_max_level( max_level );
		v_connction->set_order( v_priority );
		v_connction->set_object_flags(US_FLAGS_STATIC_OBJECT);

		((connection_set*)parent_object)->add_connection( v_connction );

		return NULL;
	}

	//
	//	block_layer Parser
	//
	object_base* xml_block_layer_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT )
			return NULL;

		TiXmlElement *v_elem = (TiXmlElement *)v_xml_node;

		// 读取属性
		ustring str_name = string_ext::to_wstring( v_elem->Attribute( "name" ) );
		ustring str_guid = string_ext::to_wstring( v_elem->Attribute( "id" ) );
		US_STRING_LOWER( str_guid );

		ustring	str_url;
		ulong b_flags = 0;
		float height = 0;
		lon_lat_rect v_rect;
		bool b_vis = true;
		block_layer* v_layer = new block_layer;

		double min_height = 0.0;
		double max_height = 1.0;
		int v_transparency = 255;

				
		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT )
				continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			ustring str_label = string_ext::to_wstring( v_child->Value() );

			if ( str_label == L"LonLatRect" )
			{
				v_rect =  xml_parser_ext::to_lon_lat_rect( v_elem->GetText() );
			}
			else if ( str_label == L"Link" )
			{
				str_url =  string_ext::to_wstring( v_elem->GetText() );
			}
			else if ( str_label == L"HeightOffset" )
			{
				height = string_ext::to_float( v_elem->GetText() ) * 0.001;
			}
			else if ( str_label == L"Visibility")
			{
				b_vis = string_ext::to_bool( v_elem->GetText() );
			}
			else if ( str_label == L"Visible")
			{
				b_vis = string_ext::to_bool( v_elem->GetText() );
			}
			else if ( str_label == L"RenderStyle" )
			{
				brush_style* v_render_style = render_style_parser::parse_brush_style( v_elem, NULL );
				v_layer->set_render_style( v_render_style );
			}
			else if ( str_label == L"Underground" )
			{
				b_flags = string_ext::to_bool(v_elem->GetText()) ? US_FLAGS_UNDERGROUND_OBJECT : 0;
			}
			else if ( str_label == L"MinHeight" )
			{
				min_height = string_ext::to_double( v_elem->GetText() );
			}
			else if ( str_label == L"MaxHeight" )
			{
				max_height = string_ext::to_double( v_elem->GetText() );
			}
			else if ( str_label == L"Transparency")
			{
				v_transparency = string_ext::to_int( v_elem->GetText() );
			}
		}

		document* v_document = (document*)v_container;
		v_layer->initialize( v_document );

		v_layer->set_name( str_name.c_str() );		
		v_layer->set_guid( str_guid.c_str() );
		v_layer->set_height_offset( height );
		v_layer->set_server_connection( str_url.c_str() );
		v_layer->set_layer_rect( v_rect );
		v_layer->set_visible(b_vis);
		v_layer->set_object_flags(US_FLAGS_STATIC_OBJECT | b_flags);

		v_layer->set_min_height( min_height );
		v_layer->set_max_height( max_height );
		v_layer->set_custom_transparency( v_transparency );
		
		v_document->add_child_ex( v_document->get_block_system(), v_layer );
		
		return v_layer;
	}
	

	//	bldg_layer parser
	object_base * xml_model_layer_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT )
			return NULL;

		TiXmlElement *v_elem = (TiXmlElement *)v_xml_node;

		// 读取属性
		ustring str_name = string_ext::to_wstring( v_elem->Attribute( "name" ) );
		ustring str_guid = string_ext::to_wstring( v_elem->Attribute( "id" ) );
		US_STRING_LOWER( str_guid );

		ustring	str_url;
		float	v_height_offset = 0;
		dword   b_flags = 0;
		//ulong	v_order = 15;
		lon_lat_rect	v_rect;
		bool b_vis = true;

		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT )
				continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			ustring str_label = string_ext::to_wstring( v_child->Value() );

			if ( str_label == L"LonLatRect" )
			{
				v_rect =  xml_parser_ext::to_lon_lat_rect( v_elem->GetText() );
			}
			else if ( str_label == L"Link" )
			{
				str_url =  string_ext::to_wstring( v_elem->GetText() );
			}
			//else if ( str_label == L"Priority" )
			//{
			//	v_order = (ulong)string_ext::to_int( v_elem->GetText() );
			//}
			else if ( str_label == L"HeightOffset" )
			{
				v_height_offset = string_ext::to_float( v_elem->GetText() ) * 0.001;
			}
			else if ( str_label == L"Visibility")
			{
				b_vis = string_ext::to_bool( v_elem->GetText() );
			}
			else if ( str_label == L"Visible")
			{
				b_vis = string_ext::to_bool( v_elem->GetText() );
			}
			else if ( str_label == L"Underground" )
			{
				b_flags = string_ext::to_bool(v_elem->GetText()) ? US_FLAGS_UNDERGROUND_OBJECT : 0;
			}
		}

		document* v_document = (document*)v_container;
		//if(system_environment::s_new_building)
		{
			//building_layer_ext* v_layer = new building_layer_ext;
			model_layer* v_layer = new model_layer();
			v_layer->initialize( v_document );

			v_layer->set_name( str_name.c_str() );		
			v_layer->set_guid( str_guid.c_str() );
			v_layer->set_height_offset( v_height_offset );
			v_layer->set_server_connection( str_url.c_str() );
			v_layer->set_layer_rect( v_rect );
			//v_layer->set_order( v_order );
			v_layer->set_visible(b_vis);
			v_layer->set_object_flags(US_FLAGS_STATIC_OBJECT | b_flags);
		
			v_document->add_child_ex( v_document->get_building_system(), v_layer );

			return v_layer;

			//building_shadow_layer* v_shadow_layer = new building_shadow_layer;

			//v_shadow_layer->set_name( str_name.c_str() );		
			//v_shadow_layer->set_guid( guid );
			//v_shadow_layer->set_height( height );
			//v_shadow_layer->set_server_connection( str_url.c_str() );
			//v_shadow_layer->set_layer_rect( v_rect );
			//v_shadow_layer->set_order( v_order );
			//v_shadow_layer->set_enable(b_vis);

			//document* v_document = (document*)v_container;
			//v_document->get_shadow_system()->attach_object( v_shadow_layer );
			//v_document->register_object( v_shadow_layer );
		}
		//else
		//{
		//	building_layer* v_layer = new building_layer;

		//	v_layer->set_name( str_name.c_str() );		
		//	v_layer->set_guid( str_guid.c_str() );
		//	v_layer->set_height( height );
		//	v_layer->set_server_connection( str_url.c_str() );
		//	v_layer->set_layer_rect( v_rect );
		//	//v_layer->set_order( v_order );
		//	v_layer->set_enable(b_vis);

		//	v_layer->initialize();

		//	document* v_document = (document*)v_container;
		//	v_document->get_building_system()->attach_object( v_layer );
		//	v_document->register_object( v_layer );

		//	return v_layer;
		//}		

		return NULL;
	}

	//
	//	xml building layer config parser
	//
	object_base* xml_bldg_layer_config_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT )
			return NULL;

		building_layer_ext* v_layer = dynamic_cast<building_layer_ext*>(parent_object);
		if ( v_layer == NULL )
			return NULL;

		TiXmlElement *v_elem = (TiXmlElement *)v_xml_node;
		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT )
				continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			string str_label = v_child->Value();

			if ( str_label == "AverageHeight" )
			{
				v_layer->m_layer_config.v_average_height = string_ext::to_double( v_elem->GetText() );
			}
		}

		return parent_object;
	}

	//
	//	xml building layer performance parser
	//
	object_base* xml_bldg_layer_performance_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT )
			return NULL;

		building_layer_ext* v_layer = dynamic_cast<building_layer_ext*>(parent_object);
		
		TiXmlElement *v_elem = (TiXmlElement *)v_xml_node;

		char* str_attr = (char*)v_elem->Attribute( "style" );
		if ( str_attr )
		{
			ustring str_style = string_ext::to_wstring( str_attr );
			if ( singleton_system_environment::instance().m_computer_performance != str_style )
				return NULL;
		}
		else
		{
			return NULL;
		}

		std::vector<TiXmlNode*> text_conn_nodes;

		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT )
				continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			string str_label = v_child->Value();

			if ( str_label == "TextureConnection" )
			{
				text_conn_nodes.push_back( v_child );
			}
			else if ( str_label == "BuildingConnection" )
			{
				parse_building_connection( v_child, v_layer );
			}
		}
		
		v_layer->m_building_texture_conn_array.resize( text_conn_nodes.size() );
		for ( int ni = 0; ni < (int)text_conn_nodes.size(); ni++ )
		{
			parse_texture_connection( text_conn_nodes[ni], v_layer );
		}
		text_conn_nodes.clear();

		v_layer->m_valid = 1;

		return NULL;
	}

	bool xml_bldg_layer_performance_parser::parse_texture_connection( TiXmlNode* v_xml_node, void* in_object )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT )
			return NULL;

		TiXmlElement *v_elem = (TiXmlElement *)v_xml_node;

		int v_index = 0;
		
		char* str_attr = (char*)v_elem->Attribute( "index" );
		if ( str_attr )
		{
			v_index = string_ext::to_int( str_attr );
		}

		ustring v_prefix;
		
		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT )
				continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			string str_label = v_child->Value();

			if ( str_label == "Prefix" )
			{
				v_prefix = string_ext::to_wstring( v_elem->GetText() );
			}
		}

		building_texture_connection* v_text_connection = new building_texture_connection( v_prefix.c_str(), v_index );
		
		building_layer_ext* v_layer = (building_layer_ext*)(in_object);
		v_text_connection->set_parent_layer( v_layer );
		v_layer->m_building_texture_conn_array[v_index] = v_text_connection;

		return true;
	}

	bool xml_bldg_layer_performance_parser::parse_building_connection( TiXmlNode* v_xml_node, void* in_object )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT )
			return NULL;

		TiXmlElement *v_elem = (TiXmlElement *)v_xml_node;

		building_layer_ext* v_layer = (building_layer_ext*)(in_object);

		std::vector<TiXmlNode*> lod_level_nodes;

		square_connection_description v_square_desp;
		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT )
				continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			string str_label = v_child->Value();

			if ( str_label == "Prefix" )
			{
				v_square_desp.m_prefix = string_ext::to_wstring( v_elem->GetText() );
			}
			else if ( str_label == "BaseLevel" )
			{
				v_square_desp.m_base_level = string_ext::to_int( v_elem->GetText() );
			}
			else if ( str_label == "LodLevel" )
			{
				lod_level_nodes.push_back( v_child );
			}
		}

		v_square_desp.m_lod_desp.resize( lod_level_nodes.size() );
		for ( int ni = 0; ni < (int)lod_level_nodes.size(); ni++ )
		{
			parse_lod_parameter( lod_level_nodes[ni], &v_square_desp.m_lod_desp );
		}

		building_square_connection* v_square_conn = new building_square_connection( v_square_desp, v_layer );
		v_square_conn->add_ref();
		if ( v_square_desp.m_prefix == L"cm" )
		{
			v_layer->m_clarity_square_conn = v_square_conn;
		}
		else
		{
			v_layer->m_standard_square_conn = v_square_conn;
		}

		return true;
	}

	bool xml_bldg_layer_performance_parser::parse_lod_parameter( TiXmlNode* v_xml_node, void* in_object )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT )
			return NULL;

		TiXmlElement *v_elem = (TiXmlElement *)v_xml_node;

		building_lod_description_array& v_lod_desp = *((building_lod_description_array*)in_object);
		
		int v_index;
		char* str_attr = (char*)v_elem->Attribute( "index" );
		if ( str_attr )
		{
			v_index = string_ext::to_int( str_attr );
		}
		else
		{
			return false;
		}

		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT )
				continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			string str_label = v_child->Value();

			if ( str_label == "MinRange" )
			{
				v_lod_desp[v_index].m_min_range = string_ext::to_double( v_elem->GetText() );
			}
			else if ( str_label == "MaxRange" )
			{
				v_lod_desp[v_index].m_max_range = string_ext::to_double( v_elem->GetText() );
			}
			else if ( str_label == "Diffuse" )
			{
				v_lod_desp[v_index].m_texture1 = string_ext::to_int( v_elem->GetText() );
			}
			else if ( str_label == "LightMap" )
			{
				v_lod_desp[v_index].m_texture2 = string_ext::to_int( v_elem->GetText() );
			}
		}

		return true;
	}

	// poi_layer_parser
	object_base* xml_poi_layer_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT )
			return NULL;

		TiXmlElement *v_elem = (TiXmlElement *)v_xml_node;

		// 读取属性
		ustring str_name = string_ext::to_wstring( v_elem->Attribute( "name" ) );
		ustring str_guid = string_ext::to_wstring( v_elem->Attribute( "id" ) );
		US_STRING_LOWER( str_guid );

		ustring	str_url;
		float	min_height = -1;
		float	max_height = -1;
		int     min_level  = -1;
		int     max_level  = -1;
		int  data_level = -1;

		//poi_style v_style;
		style_base* v_render_style = new style_base;
		v_render_style->m_style_array.resize(2);

		lon_lat_rect v_rect;
		bool b_vis = true;

		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT )
				continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			ustring str_label = string_ext::to_wstring( v_child->Value() );

			if ( str_label == L"LonLatRect" )
			{
				v_rect =  xml_parser_ext::to_lon_lat_rect( v_elem->GetText() );
			}
			else if ( str_label == L"Link" )
			{
				str_url =  string_ext::to_wstring( v_elem->GetText() );
			}
			else if ( str_label == L"MinHeight" )
			{
				min_height =  string_ext::to_float( v_elem->GetText() );
			}
			else if ( str_label == L"MaxHeight" )
			{
				max_height =  string_ext::to_float( v_elem->GetText() );
			}
			else if ( str_label == L"MinLevel" )
			{
				min_level =  string_ext::to_int( v_elem->GetText() );
			}
			else if ( str_label == L"MaxLevel" )
			{
				max_level =  string_ext::to_int( v_elem->GetText() );
			}
			else if ( str_label == L"Visibility")
			{
				b_vis = string_ext::to_bool( v_elem->GetText() );
			}
			else if ( str_label == L"Visible")
			{
				b_vis = string_ext::to_bool( v_elem->GetText() );
			}
			else if ( str_label == L"NormalStyle" )
			{
				v_render_style->m_style_array[0] = render_style_parser::parse_icon_style( v_elem, NULL );
				//xml_parser_base* v_parser = m_parser_mngr->get_parser( L"RenderStyle" );
				//if ( v_parser )
				//{
				//	v_parser->forward_parse( v_container, v_elem, v_normal_style ); 
				//}
			}
			else if ( str_label == L"HighlightStyle" )
			{
				v_render_style->m_style_array[1] = render_style_parser::parse_icon_style( v_elem, NULL );
				//xml_parser_base* v_parser = m_parser_mngr->get_parser( L"RenderStyle" );
				//if ( v_parser )
				//{
				//	v_parser->forward_parse( v_container, v_elem, v_highlight_style ); 
				//}
			}
		}

		//if ( data_level != -1 )
		//{
		//	min_level = data_level;
		//	max_level = data_level;
		//}	

		// height
		if( min_height == -1 )
		{
			min_height = PI * spherical_coords::s_semi_major / math<double>::pow_( 2.0, max_level + 1 ) / 256 / math<double>::tan_( 45 * RADIAN * 0.5 ) * 900 * 0.5 ;
		}

		if ( max_height == -1 )
		{
			max_height = PI * spherical_coords::s_semi_major / math<double>::pow_( 2.0, min_level ) / 256 / math<double>::tan_( 45 * RADIAN * 0.5 ) * 900 * 0.5 ;
		}

		document* v_document = (document*)v_container;

		// new layer
		poi_layer* v_layer = new poi_layer;
		v_layer->initialize( v_document );

		v_layer->set_name( str_name.c_str() );		
		v_layer->set_guid( str_guid.c_str() );
		v_layer->set_min_height( min_height );
		v_layer->set_max_height( max_height );		
		v_layer->set_min_level( min_level );
		v_layer->set_max_level( max_level );		
		v_layer->set_server_connection( str_url.c_str() );
		v_layer->set_layer_rect( v_rect );
		v_layer->set_visible(b_vis);
		v_layer->set_object_flags( true );
			
		v_layer->set_render_style( v_render_style );
				
		v_document->add_child_ex( v_document->get_poi_system(), v_layer );

		return v_layer;
	}

	void xml_poi_layer_parser::parse_poi_style( TiXmlNode* v_xml_node, poi_style& v_style )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT )
			return;

		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT )
				continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			string str_label = v_child->Value();

			if ( str_label == "NormalIcon" )
			{
				v_style.str_normal = string_ext::to_wstring( v_elem->GetText() );
			}
			else if ( str_label == "HighlightIcon" )
			{
				v_style.str_high_light = string_ext::to_wstring( v_elem->GetText() );
			}
		}
	}

	//	vector_chunk parser
	object_base* xml_vector_layer_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT )
			return NULL;

		TiXmlElement *v_elem = (TiXmlElement *)v_xml_node;

		// 读取属性
		ustring str_name = string_ext::to_wstring( v_elem->Attribute( "name" ) );
		ustring str_guid = string_ext::to_wstring( v_elem->Attribute( "id" ) );
		US_STRING_LOWER( str_guid );

		ustring	str_url;
		float	v_height_offset = 0;
		float	min_height = -1;
		float	max_height = -1;
		int     min_level = -1;
		int     max_level = -1;
	
		style_base* v_render_style = NULL;

		bool v_fade_in = true;
		bool v_depth_enable = false;

		lon_lat_rect	v_rect;
		bool    b_vis = true;
		ustring v_type = L"common";
		double v_visible_range = 0.0;

		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT )
				continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			ustring str_label = string_ext::to_wstring( v_child->Value() );

			if ( str_label == L"LonLatRect" )
			{
				v_rect =  xml_parser_ext::to_lon_lat_rect( v_elem->GetText() );
			}
			else if ( str_label == L"Link" )
			{
				str_url =  string_ext::to_wstring( v_elem->GetText() );
			}
			else if( str_label == L"MinLevel" )
			{
				min_level = string_ext::to_int( v_elem->GetText() );
			}
			else if( str_label == L"MaxLevel" )
			{
				max_level = string_ext::to_int( v_elem->GetText() );
			}
			else if ( str_label == L"MinHeight" )
			{
				min_height =  string_ext::to_float( v_elem->GetText() );
			}
			else if ( str_label == L"MaxHeight" )
			{
				max_height =  string_ext::to_float( v_elem->GetText() );
			}
			//else if ( str_label == L"DataLevel" )
			//{
			//	data_level =  string_ext::to_int( v_elem->GetText() );
			//}
			else if ( str_label == L"Visibility")
			{
				b_vis = string_ext::to_bool( v_elem->GetText() );
			}
			else if ( str_label == L"Visible")
			{
				b_vis = string_ext::to_bool( v_elem->GetText() );
			}
			else if( str_label == L"VisibleRange" )
			{
				v_visible_range = string_ext::to_double( v_elem->GetText() );
			}
			else if ( str_label == L"HeightOffset" )
			{
				v_height_offset = string_ext::to_float( v_elem->GetText() ) * 0.001;
			}
			else if ( str_label == L"RenderStyle" )
			{
				v_render_style = render_style_parser::parse_brush_style( v_elem, NULL );
				//xml_parser_base* v_parser = m_parser_mngr->get_parser( L"RenderStyle" );
				//if ( v_parser )
				//{
				//	v_parser->forward_parse( v_container, v_elem, v_render_style ); 
				//}
			}
			else if( str_label == L"FadeIn" )
			{
				v_fade_in = string_ext::to_bool( v_elem->GetText() );
			}
			else if( str_label == L"DepthEnable" )
			{
				v_depth_enable = string_ext::to_bool( v_elem->GetText() );
			}
			//else if ( str_label == L"LineWidth" )
			//{
			//	v_line_width = string_ext::to_double( v_elem->GetText() );
			//}
			//else if ( str_label == L"LineColor" )
			//{
			//	v_line_color = string_ext::to_dword( v_elem->GetText() );
			//}
			//else if ( str_label == L"FilledColor" )
			//{
			//	v_fill_color = string_ext::to_dword( v_elem->GetText() );
			//}
			else if( str_label == L"RenderType" )
			{
				v_type = string_ext::to_wstring( v_elem->GetText() );
				
				US_STRING_LOWER( v_type )
			}
		}

		//if ( data_level != -1 )
		//{
		//	min_level = data_level;
		//	max_level = data_level;
		//}	

		// height
		if( min_height == -1 )
		{
			min_height = PI * spherical_coords::s_semi_major / math<double>::pow_( 2.0, max_level + 1 ) / 256 / math<double>::tan_( 45 * RADIAN * 0.5 ) * 900 * 0.5 ;
		}

		if ( max_height == -1 )
		{
			max_height = PI * spherical_coords::s_semi_major / math<double>::pow_( 2.0, min_level ) / 256 / math<double>::tan_( 45 * RADIAN * 0.5 ) * 900 * 0.5 ;
		}

		//v_visible_range = MAX( 2, v_visible_range/(spherical_coords::s_semi_major * interior_index_factory::get_bound_size( max_level )) );
		
		document* v_document = (document*)v_container;


		generic_vector_layer* v_generic_layer = new generic_vector_layer();
		v_generic_layer->initialize( v_document );
		v_generic_layer->set_render_style( v_render_style );

		if ( str_url.find( L"geoserver?" ) != -1 )
		{
			str_url += L"&type=1&si=";

			v_generic_layer->set_request_type( request_type::REQ_TYPE_ARCSDE );
		}

		if ( str_url.find( L"webserver?" ) != -1 )
		{
			str_url += L"&type=0&si=";

			v_generic_layer->set_request_type( request_type::REQ_TYPE_ARCSDE );
		}

		//
		// layer style
		//
		v_generic_layer->set_fade_in( v_fade_in );
		v_generic_layer->set_depth_enable( v_depth_enable );
		v_generic_layer->set_visible_range( v_visible_range );
		
		if( v_type == L"common" )
		{
			v_generic_layer->set_render_type( render_type::COMMON );
		}
		else if( v_type == L"raster" )
		{			
			v_generic_layer->set_render_type( render_type::RASTER );
		}
		else if( v_type == L"stencil" )
		{
			v_generic_layer->set_render_type( render_type::STENCIL );
		}		
		
		v_generic_layer->set_name( str_name.c_str() );
		v_generic_layer->set_guid( str_guid.c_str() );
		v_generic_layer->set_min_level( min_level );
		v_generic_layer->set_max_level( max_level );
		v_generic_layer->set_min_height( min_height );
		v_generic_layer->set_max_height( max_height );		
		v_generic_layer->set_server_connection( str_url.c_str() );
		v_generic_layer->set_layer_rect( v_rect );
		v_generic_layer->set_visible(b_vis);
		v_generic_layer->set_height_offset(v_height_offset);
		v_generic_layer->set_object_flags(US_FLAGS_STATIC_OBJECT);

		if( v_type == L"raster" )
		{
			v_document->get_raster_system()->attach_object( v_generic_layer );
		}
		else
		{
			v_document->get_vector_system()->attach_object( v_generic_layer );
		}

		v_document->add_child_ex( v_document->get_vector_system(), v_generic_layer );
	

		//vector_layer_base* v_layer = NULL;
		//// new layer
		//if( v_type == L"raster" )
		//{			
		//	v_layer = new raster_vector_layer( v_document );

		//	v_layer->set_name( str_name.c_str() );
		//	v_layer->set_guid( str_guid.c_str() );
		//	v_layer->set_min_level( min_level );
		//	v_layer->set_max_level( max_level );
		//	v_layer->set_min_height( min_height );
		//	v_layer->set_max_height( max_height );
		//	
		//	v_style->m_line_width = v_render_style.m_line_width;
		//	v_style->m_line_color = v_render_style.m_line_color;
		//	v_style->m_fill_color = v_render_style.m_fill_color;
		//	v_style->m_fade_in    = v_render_style.m_fade_in;
		//	v_style->m_depth_enable = v_render_style.m_depth_enable;
		//	v_style->m_visible_range = v_visible_range;
		//	v_style->m_render_type = render_type::RASTER;
		//	v_layer->set_layer_style( v_style );
		//	v_layer->set_server_connection( str_url.c_str() );
		//	v_layer->set_layer_rect( v_rect );
		//	v_layer->set_enable(b_vis);

		//	v_document->get_raster_system()->attach_object( v_layer );
		//	v_document->register_object( v_layer );

		//}
		//else if( v_type == L"shadow" )
		//{
		//	v_layer = new shadow_vector_layer( v_document );

		//	v_layer->set_name( str_name.c_str() );	
		//	v_layer->set_guid( str_guid.c_str() );
		//	v_layer->set_min_level( min_level );
		//	v_layer->set_max_level( max_level );
		//	v_layer->set_min_height( min_height );
		//	v_layer->set_max_height( max_height );

		//	v_style->m_line_width = v_render_style.m_line_width;
		//	v_style->m_line_color = v_render_style.m_line_color;
		//	v_style->m_fill_color = v_render_style.m_fill_color;
		//	v_style->m_fade_in    = v_render_style.m_fade_in;
		//	v_style->m_depth_enable = v_render_style.m_depth_enable;
		//	v_style->m_visible_range = v_visible_range;
		//	v_style->m_render_type = render_type::SHADOW_VOLUME;
		//	v_layer->set_layer_style( v_style );
		//	v_layer->set_server_connection( str_url.c_str() );
		//	v_layer->set_layer_rect( v_rect );
		//	v_layer->set_enable(b_vis);

		//	v_document->get_vector_system()->attach_object( v_layer );
		//	v_document->register_object( v_layer );

		//}
		//else if( v_type == L"hardware" )
		//{
		//	v_layer = new common_vector_layer( v_document );

		//	v_layer->set_name( str_name.c_str() );
		//	v_layer->set_guid( str_guid.c_str() );
		//	v_layer->set_min_level( min_level );
		//	v_layer->set_max_level( max_level );
		//	v_layer->set_min_height( min_height );
		//	v_layer->set_max_height( max_height );

		//	v_style->m_line_width = v_render_style.m_line_width;
		//	v_style->m_line_color = v_render_style.m_line_color;
		//	v_style->m_fill_color = v_render_style.m_fill_color;
		//	v_style->m_fade_in    = v_render_style.m_fade_in;
		//	v_style->m_depth_enable = v_render_style.m_depth_enable;
		//	v_style->m_visible_range = v_visible_range;
		//	v_style->m_render_type = render_type::HARDWARE;
		//	v_layer->set_layer_style( v_style );
		//	v_layer->set_server_connection( str_url.c_str() );
		//	v_layer->set_layer_rect( v_rect );
		//	v_layer->set_enable(b_vis);

		//	v_document->get_vector_system()->attach_object( v_layer );
		//	v_document->register_object( v_layer );

		//}
		//else
		//{
		//	v_layer = new common_vector_layer( v_document );

		//	v_layer->set_name( str_name.c_str() );
		//	v_layer->set_guid( str_guid.c_str() );
		//	v_layer->set_min_level( min_level );
		//	v_layer->set_max_level( max_level );
		//	v_layer->set_min_height( min_height );
		//	v_layer->set_max_height( max_height );
		//	
		//	v_style->m_line_width = v_render_style.m_line_width;
		//	v_style->m_line_color = v_render_style.m_line_color;
		//	v_style->m_fill_color = v_render_style.m_fill_color;
		//	v_style->m_fade_in    = v_render_style.m_fade_in;
		//	v_style->m_depth_enable = v_render_style.m_depth_enable;
		//	v_style->m_visible_range = v_visible_range;
		//	v_style->m_render_type = render_type::COMMON;
		//	v_layer->set_layer_style( v_style );
		//	v_layer->set_server_connection( str_url.c_str() );
		//	v_layer->set_layer_rect( v_rect );
		//	v_layer->set_enable(b_vis);

		//	v_document->get_vector_system()->attach_object( v_layer );
		//	v_document->register_object( v_layer );
		//}


	
		return v_generic_layer;
	}

	//	billboard_chunk parser
	object_base* xml_billboard_layer_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT )
			return NULL;

		TiXmlElement *v_elem = (TiXmlElement *)v_xml_node;

		// 读取属性
		ustring str_name = string_ext::to_wstring( v_elem->Attribute( "name" ) );
		ustring str_guid = string_ext::to_wstring( v_elem->Attribute( "id" ) );
		US_STRING_LOWER( str_guid );

		ustring	str_url;
		//ustring str_resource_link;
		float	v_height_offset = 0;
		
		float	min_height = 0.0f;
		float	max_height = 1.0f;
		int     min_level  = 15;
		int     max_level  = 15;
		//int		data_level = -1;

		lon_lat_rect	v_rect;
		bool    b_vis = true;
		double  v_visible_range = 0.5;
		ulong b_flags = 0;

		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT )
				continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			ustring str_label = string_ext::to_wstring( v_child->Value() );

			if ( str_label == L"LonLatRect" )
			{
				v_rect =  xml_parser_ext::to_lon_lat_rect( v_elem->GetText() );
			}
			else if ( str_label == L"Link" )
			{
				str_url =  string_ext::to_wstring( v_elem->GetText() );
			}
			//else if ( str_label == L"ResourceLink" )
			//{
			//	str_resource_link =  string_ext::to_wstring( v_elem->GetText() );
			//}
			else if ( str_label == L"MinHeight" )
			{
				min_height =  string_ext::to_float( v_elem->GetText() );
			}
			else if ( str_label == L"MaxHeight" )
			{
				max_height =  string_ext::to_float( v_elem->GetText() );
			}
			//else if ( str_label == L"DataLevel" )
			//{
			//	data_level =  string_ext::to_int( v_elem->GetText() );
			//}
			//else if( str_label == L"MinLevel" )
			//{
			//	min_level = string_ext::to_int( v_elem->GetText() );
			//}
			//else if( str_label == L"MaxLevel" )
			//{
			//	max_level = string_ext::to_int( v_elem->GetText() );
			//}
			else if ( str_label == L"Visibility")
			{
				b_vis = string_ext::to_bool( v_elem->GetText() );
			}
			else if ( str_label == L"Visible")
			{
				b_vis = string_ext::to_bool( v_elem->GetText() );
			}
			else if ( str_label == L"VisibleRange" )
			{
				v_visible_range = string_ext::to_double( v_elem->GetText() );
			}
			else if ( str_label == L"HeightOffset" )
			{
				v_height_offset = string_ext::to_float( v_elem->GetText() ) * 0.001;
			}
			else if ( str_label == L"Underground" )
			{
				b_flags = string_ext::to_bool(v_elem->GetText()) ? US_FLAGS_UNDERGROUND_OBJECT : 0;
			}
		}

		//if ( data_level != -1 )
		//{
		//	min_level = data_level;
		//	max_level = data_level;
		//}	

		// height
		if( min_height == -1 )
		{
			min_height = PI * spherical_coords::s_semi_major / math<double>::pow_( 2.0, max_level + 1 ) / 256 / math<double>::tan_( 45 * RADIAN * 0.5 ) * 900 * 0.5 ;
		}

		if ( max_height == -1 )
		{
			max_height = PI * spherical_coords::s_semi_major / math<double>::pow_( 2.0, min_level ) / 256 / math<double>::tan_( 45 * RADIAN * 0.5 ) * 900 * 0.5 ;
		}

		// new layer
		document* v_document = (document*)v_container;
		billboard_layer* v_layer = new billboard_layer();
		v_layer->initialize( v_document );
		v_layer->set_name( str_name.c_str() );
		v_layer->set_guid( str_guid.c_str() );
		v_layer->set_min_height( min_height );
		v_layer->set_max_height( max_height );
		v_layer->set_min_level( min_level );
		v_layer->set_max_level( max_level );
		v_layer->set_server_connection( str_url.c_str() );
		v_layer->set_layer_rect( v_rect );
		v_layer->set_visible(b_vis);
		v_layer->set_visible_range( v_visible_range );
		v_layer->set_height_offset(v_height_offset);
		v_layer->set_object_flags(US_FLAGS_STATIC_OBJECT | b_flags );
		
		v_document->add_child_ex( v_document->get_billboard_system(), v_layer );

		return v_layer;
	}
	
	//	annotation layer parser
	object_base* xml_annotation_layer_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT )
			return NULL;

		TiXmlElement *v_elem = (TiXmlElement *)v_xml_node;

		// 读取属性
		ustring str_name = string_ext::to_wstring( v_elem->Attribute( "name" ) );
		ustring str_guid = string_ext::to_wstring( v_elem->Attribute( "id" ) );
		US_STRING_LOWER( str_guid );

		ustring	str_url;
		int     min_level = -1;
		int     max_level = -1;
		//int     data_level = -1;
		float	min_height = -1;
		float	max_height = -1;
		float   v_height_offset = 0;
		
		//ulong   color = 0;
		//ustring typeface;
		//double  font_scale = 1.0;
		style_base* v_render_style = NULL;
		lon_lat_rect	v_rect;
		bool    b_vis = true;		

		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT )
				continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			ustring str_label = string_ext::to_wstring( v_child->Value() );

			if ( str_label == L"LonLatRect" )
			{
				v_rect =  xml_parser_ext::to_lon_lat_rect( v_elem->GetText() );
			}
			else if ( str_label == L"Link" )
			{
				str_url =  string_ext::to_wstring( v_elem->GetText() );
			}
			else if ( str_label == L"MinHeight" )
			{
				min_height =  string_ext::to_float( v_elem->GetText() );
			}
			else if ( str_label == L"MaxHeight" )
			{
				max_height =  string_ext::to_float( v_elem->GetText() );
			}
			else if ( str_label == L"MinLevel" )
			{
				min_level =  string_ext::to_int( v_elem->GetText() );
			}
			else if ( str_label == L"MaxLevel" )
			{
				max_level =  string_ext::to_int( v_elem->GetText() );
			}
			else if ( str_label == L"Visibility")
			{
				b_vis = string_ext::to_bool( v_elem->GetText() );
			}
			else if ( str_label == L"Visible")
			{
				b_vis = string_ext::to_bool( v_elem->GetText() );
			}
			else if ( str_label == L"HeightOffset" )
			{
				v_height_offset = string_ext::to_float( v_elem->GetText() ) * 0.001;
			}
			else if ( str_label == L"RenderStyle" )
			{
				v_render_style = render_style_parser::parse_font_style( v_elem, NULL );
				//xml_parser_base* v_parser = m_parser_mngr->get_parser( L"RenderStyle" );
				//if ( v_parser )
				//{
				//	v_parser->forward_parse( v_container, v_elem, v_render_style ); 
				//}				
			}
			//else if ( str_label == L"Color" )
			//{
			//	color = string_ext::to_dword( v_elem->GetText() );
			//}
			//else if ( str_label == L"FontScale" )
			//{
			//	font_scale = string_ext::to_float( v_elem->GetText() );
			//}
			//else if( str_label == L"Typeface" )
			//{
			//	//typeface = string_ext::to_wstring( v_elem->GetText() );
			//}
		}


		//if ( data_level != -1 )
		//{
		//	min_level = data_level;
		//	max_level = data_level;
		//}	

		// height
		if( min_height == -1 )
		{
			min_height = PI * spherical_coords::s_semi_major / math<double>::pow_( 2.0, max_level + 1 ) / 256 / math<double>::tan_( 45 * RADIAN * 0.5 ) * 900 * 0.5 ;
		}

		if ( max_height == -1 )
		{
			max_height = PI * spherical_coords::s_semi_major / math<double>::pow_( 2.0, min_level ) / 256 / math<double>::tan_( 45 * RADIAN * 0.5 ) * 900 * 0.5 ;
		}

		// new layer
		document* v_document = (document*)v_container;

		annotation_layer* v_layer = new annotation_layer;
		v_layer->initialize( v_document );
		v_layer->set_name( str_name.c_str() );
		v_layer->set_guid( str_guid.c_str() );
		v_layer->set_min_level( min_level );
		v_layer->set_max_level( max_level );
		v_layer->set_min_height( min_height );
		v_layer->set_max_height( max_height );
		v_layer->set_render_style( v_render_style );
		v_layer->set_server_connection( str_url.c_str() );
		v_layer->set_layer_rect( v_rect );
		v_layer->set_visible(b_vis);
		v_layer->set_height_offset(v_height_offset);
		v_layer->set_object_flags(US_FLAGS_STATIC_OBJECT);
		
		v_document->add_child_ex( v_document->get_annotation_system(), v_layer );

		return v_layer;
	}

	object_base* xml_match_model_layer_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT )
			return NULL;

		TiXmlElement *v_elem = (TiXmlElement *)v_xml_node;

		// 读取属性
		ustring str_name = string_ext::to_wstring( v_elem->Attribute( "name" ) );
		ustring str_guid = string_ext::to_wstring( v_elem->Attribute( "id" ) );
		US_STRING_LOWER( str_guid );

		ustring	str_url;
		//ustring str_resource_link;
		float	v_height_offset = 0;

		float	min_height = 0.0f;
		float	max_height = 1.0f;
		int     min_level  = 15;
		int     max_level  = 15;
		//int		data_level = -1;

		lon_lat_rect	v_rect;
		bool    b_vis = true;
		double  v_visible_range = 0.25;
		ulong flags = 0;

		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT )
				continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			ustring str_label = string_ext::to_wstring( v_child->Value() );

			if ( str_label == L"LonLatRect" )
			{
				v_rect =  xml_parser_ext::to_lon_lat_rect( v_elem->GetText() );
			}
			else if ( str_label == L"Link" )
			{
				str_url =  string_ext::to_wstring( v_elem->GetText() );
			}
			else if ( str_label == L"MinHeight" )
			{
				min_height =  string_ext::to_float( v_elem->GetText() );
			}
			else if ( str_label == L"MaxHeight" )
			{
				max_height =  string_ext::to_float( v_elem->GetText() );
			}
			//else if ( str_label == L"DataLevel" )
			//{
			//	data_level =  string_ext::to_int( v_elem->GetText() );
			//}
			//else if( str_label == L"MinLevel" )
			//{
			//	min_level = string_ext::to_int( v_elem->GetText() );
			//}
			//else if( str_label == L"MaxLevel" )
			//{
			//	max_level = string_ext::to_int( v_elem->GetText() );
			//}
			else if ( str_label == L"Visibility")
			{
				b_vis = string_ext::to_bool( v_elem->GetText() );
			}
			else if ( str_label == L"Visible")
			{
				b_vis = string_ext::to_bool( v_elem->GetText() );
			}
			else if ( str_label == L"VisibleRange" )
			{
				v_visible_range = string_ext::to_float( v_elem->GetText() );
			}
			else if ( str_label == L"HeightOffset" )
			{
				v_height_offset = string_ext::to_float( v_elem->GetText() ) * 0.001;
			}
			else if ( str_label == L"Underground" )
			{
				flags = string_ext::to_bool(v_elem->GetText()) ? US_FLAGS_UNDERGROUND_OBJECT : 0;
			}
		}

		//if ( data_level != -1 )
		//{
		//	min_level = data_level;
		//	max_level = data_level;
		//}	

		// height
		if( min_height == -1 )
		{
			min_height = PI * spherical_coords::s_semi_major / math<double>::pow_( 2.0, max_level + 1 ) / 256 / math<double>::tan_( 45 * RADIAN * 0.5 ) * 900 * 0.5 ;
		}

		if ( max_height == -1 )
		{
			max_height = PI * spherical_coords::s_semi_major / math<double>::pow_( 2.0, min_level ) / 256 / math<double>::tan_( 45 * RADIAN * 0.5 ) * 900 * 0.5 ;
		}

		// new layer
		document* v_document = (document*)v_container;
		match_model_layer* v_layer = new match_model_layer();
		v_layer->initialize( v_document );
		v_layer->set_name( str_name.c_str() );
		v_layer->set_guid( str_guid.c_str() );
		v_layer->set_min_height( min_height );
		v_layer->set_max_height( max_height );
		v_layer->set_min_level( min_level );
		v_layer->set_max_level( max_level );
		v_layer->set_server_connection( str_url.c_str() );
		v_layer->set_layer_rect( v_rect );
		v_layer->set_visible(b_vis);
		v_layer->set_visible_range( v_visible_range );
		v_layer->set_height_offset(v_height_offset);
		v_layer->set_object_flags(US_FLAGS_STATIC_OBJECT | flags);

		v_document->add_child_ex( v_document->get_match_model_system(), v_layer );

		return v_layer;
	}

}