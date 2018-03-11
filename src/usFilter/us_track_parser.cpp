///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_track_parser.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : track_parser class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_track_parser.h"

namespace uniscope_globe
{
	object_base* track_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT )
			return NULL;

		track* v_track = dynamic_cast<track*>(parent_object);

		// get attribute
		{
			TiXmlElement *v_elem = (TiXmlElement *)v_xml_node;
			char* str_temp = NULL;
			//  parse id
			if( (str_temp = (char*)v_elem->Attribute( "name" )) != NULL )
			{
				v_track->set_name( string_ext::to_wstring( str_temp ).c_str() );
			}
		}

		// get child
		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT )
				continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			ustring str_label = string_ext::to_wstring( v_child->Value() );

			xml_parser_base *v_parser = m_parser_mngr->get_parser( str_label.c_str() );

			if( v_parser)
			{
				v_parser->forward_parse( v_container, v_elem, dynamic_cast<object_base*>(v_track) );
			}
		}
	
		return NULL;
	}

	// pass
	object_base* station_route_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT ) return NULL;

		station_route* v_station = new station_route;
		// get attribute
		{
			TiXmlElement *v_elem = (TiXmlElement *)v_xml_node;
			char* str_temp = NULL;
			//  parse id
			if( (str_temp = (char*)v_elem->Attribute( "name" )) != NULL )
			{
				v_station->set_name( string_ext::to_wstring( str_temp ).c_str() );
			}
		}

		// get child
		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT ) continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			ustring str_label = string_ext::to_wstring( v_child->Value() );

			xml_parser_base *v_parser = m_parser_mngr->get_parser( str_label.c_str() );

			if( v_parser)
			{
				v_parser->forward_parse( v_container, v_elem, v_station );
			}

		}
		v_station->commit_changes();

		track* v_track = dynamic_cast<track*>(parent_object);
		if(v_track) 
			v_track->add_station( v_station );

		return NULL;
	}

	// pass
	object_base* station_pass_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT ) return NULL;

		station_pass* v_station = new station_pass;

		// get attribute
		{
			TiXmlElement *v_elem = (TiXmlElement *)v_xml_node;
			char* str_temp = NULL;
			//  parse id
			if( (str_temp = (char*)v_elem->Attribute( "name" )) != NULL )
			{
				v_station->set_name( string_ext::to_wstring( str_temp ).c_str() );
			}
		}

		// get child
		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT ) continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			ustring str_label = string_ext::to_wstring( v_child->Value() );

			if ( str_label == L"Longitude" )
			{
				v_station->m_longitude = string_ext::to_double( v_elem->GetText() ) * RADIAN;
			}
			else if ( str_label == L"Latitude" )
			{
				v_station->m_latitude = string_ext::to_double( v_elem->GetText() ) * RADIAN;
			}
			else if ( str_label == L"Altitude" )
			{
				v_station->m_altitude = string_ext::to_double( v_elem->GetText() ) * 0.001 + spherical_coords::s_semi_major;
			}
			else if ( str_label == L"Heading" )
			{
				v_station->m_heading = string_ext::to_double( v_elem->GetText() ) * RADIAN;
			}
			else if ( str_label == L"Tilt" )
			{
				v_station->m_tilt = string_ext::to_double( v_elem->GetText() ) * RADIAN;
			}
			else if ( str_label == L"FlyHeight" )
			{
				v_station->m_fly_height = string_ext::to_double( v_elem->GetText() ) * 0.001;
			}
			else if ( str_label == L"Speed" )
			{
				v_station->m_speed = string_ext::to_double( v_elem->GetText() ) * 0.001;
			}
		}
		station_route* v_station_route = dynamic_cast<station_route*>(parent_object);
		if(v_station_route)
			v_station_route->add_station( v_station );

		return NULL;
	}

	// stamp
	object_base* station_stamp_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT ) return NULL;

		station_stamp* v_station = new station_stamp;

		// get attribute
		{
			TiXmlElement *v_elem = (TiXmlElement *)v_xml_node;
			char* str_temp = NULL;
			//  parse id
			if( (str_temp = (char*)v_elem->Attribute( "name" )) != NULL )
			{
				v_station->set_name( string_ext::to_wstring( str_temp ).c_str() );
			}
		}

		// get child
		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT ) continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			ustring str_label = string_ext::to_wstring( v_child->Value() );

			if ( str_label == L"Longitude" )
			{
				v_station->m_longitude = string_ext::to_double( v_elem->GetText() ) * RADIAN;
			}
			else if ( str_label == L"Latitude" )
			{
				v_station->m_latitude = string_ext::to_double( v_elem->GetText() ) * RADIAN;
			}
			else if ( str_label == L"Altitude" )
			{
				v_station->m_altitude = string_ext::to_double( v_elem->GetText() ) * 0.001 + spherical_coords::s_semi_major;
			}
			else if ( str_label == L"Heading" )
			{
				v_station->m_heading = string_ext::to_double( v_elem->GetText() ) * RADIAN;
			}
			else if ( str_label == L"Tilt" )
			{
				v_station->m_tilt = string_ext::to_double( v_elem->GetText() ) * RADIAN;
			}
			else if ( str_label == L"FlyHeight" )
			{
				v_station->m_fly_height = string_ext::to_double( v_elem->GetText() ) * 0.001;
			}
		}
		track* v_track = dynamic_cast<track*>(parent_object);
		if(v_track) 
			v_track->add_station( v_station );

		return NULL;
	}

	// LookAt
	object_base* station_lookat_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT ) return NULL;

		station_lookat* v_station = new station_lookat;

		// get attribute
		{
			TiXmlElement *v_elem = (TiXmlElement *)v_xml_node;
			char* str_temp = NULL;
			//  parse id
			if( (str_temp = (char*)v_elem->Attribute( "name" )) != NULL )
			{
				v_station->set_name( string_ext::to_wstring( str_temp ).c_str() );
			}
		}

		// get child
		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT ) continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			ustring str_label = string_ext::to_wstring( v_child->Value() );

			if ( str_label == L"Longitude" )
			{
				v_station->m_longitude = string_ext::to_double( v_elem->GetText() ) * RADIAN;
			}
			else if ( str_label == L"Latitude" )
			{
				v_station->m_latitude = string_ext::to_double( v_elem->GetText() ) * RADIAN;
			}
			else if ( str_label == L"Altitude" )
			{
				v_station->m_altitude = string_ext::to_double( v_elem->GetText() ) * 0.001 + spherical_coords::s_semi_major;
			}
			else if ( str_label == L"Heading" )
			{
				v_station->m_heading = string_ext::to_double( v_elem->GetText() ) * RADIAN;
			}
			else if ( str_label == L"Tilt" )
			{
				v_station->m_tilt = string_ext::to_double( v_elem->GetText() ) * RADIAN;
			}
			else if ( str_label == L"Roll" )
			{
				v_station->m_roll = string_ext::to_double( v_elem->GetText() ) * RADIAN;
			}
			else if ( str_label == L"Range" )
			{
				v_station->m_range = string_ext::to_double( v_elem->GetText() ) * 0.001;
			}
			else if ( str_label == L"Time" )
			{
				v_station->m_time = string_ext::to_double( v_elem->GetText() );
			}
			else if ( str_label == L"StopTime" )
			{
				v_station->m_stop_time = string_ext::to_double( v_elem->GetText() );
			}
		}
		v_station->m_fly_height = v_station->m_range * cos( v_station->m_tilt );
		v_station->commit_changes();

		track* v_track = dynamic_cast<track*>(parent_object);
		if(v_track) 
			v_track->add_station( v_station );

		return NULL;
	}


	// pass
	object_base* station_surround_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT ) return NULL;

		station_surround* v_station = new station_surround;

		// get attribute
		{
			TiXmlElement *v_elem = (TiXmlElement *)v_xml_node;
			char* str_temp = NULL;
			//  parse id
			if( (str_temp = (char*)v_elem->Attribute( "name" )) != NULL )
			{
				v_station->set_name( string_ext::to_wstring( str_temp ).c_str() );
			}
		}

		// get child
		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT ) continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			ustring str_label = string_ext::to_wstring( v_child->Value() );

			if ( str_label == L"Longitude" )
			{
				v_station->m_longitude = string_ext::to_double( v_elem->GetText() ) * RADIAN;
			}
			else if ( str_label == L"Latitude" )
			{
				v_station->m_latitude = string_ext::to_double( v_elem->GetText() ) * RADIAN;
			}
			else if ( str_label == L"Altitude" )
			{
				v_station->m_altitude = string_ext::to_double( v_elem->GetText() ) * 0.001 + spherical_coords::s_semi_major;
			}
			else if ( str_label == L"FlyHeight" )
			{
				v_station->m_fly_height = string_ext::to_double( v_elem->GetText() ) * 0.001;
			}
			else if ( str_label == L"Speed" )
			{
				v_station->m_speed = string_ext::to_double( v_elem->GetText() ) * 0.001;
			}
			else if ( str_label == L"Count" )
			{
				v_station->m_surround_count = string_ext::to_int( v_elem->GetText() );
			}
			else if ( str_label == L"Radius" )
			{
				v_station->m_radius = string_ext::to_double( v_elem->GetText() ) * 0.001;
			}
		}
		v_station->m_tilt = math<double>::atan_( v_station->m_radius / v_station->m_fly_height );
		v_station->commit_changes();

		track* v_track = dynamic_cast<track*>(parent_object);
		if(v_track) 
			v_track->add_station( v_station );

		return NULL;
	}

	object_base* station_stop_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		if ( v_xml_node->Type() != TiXmlNode::ELEMENT ) return NULL;

		station_stop* v_station = new station_stop;

		// get attribute
		{
			TiXmlElement *v_elem = (TiXmlElement *)v_xml_node;
			char* str_temp = NULL;
			//  parse id
			if( (str_temp = (char*)v_elem->Attribute( "name" )) != NULL )
			{
				v_station->set_name( string_ext::to_wstring( str_temp ).c_str() );
			}
		}

		// get child
		for ( TiXmlNode* v_child = v_xml_node->FirstChild(); v_child; v_child = v_child->NextSibling() )
		{
			if ( v_child->Type() != TiXmlNode::ELEMENT ) continue;

			TiXmlElement* v_elem = (TiXmlElement*)v_child;

			ustring str_label = string_ext::to_wstring( v_child->Value() );

			if ( str_label == L"StopTime" )
			{
				v_station->m_stop_time = string_ext::to_double( v_elem->GetText() );
			}
		}

		track* v_track = dynamic_cast<track*>(parent_object);
		if(v_track) 
			v_track->add_station( v_station );

		return NULL;
	}


	//TiXmlNode* station_parser::backward_parse( object_base* ret_object, TiXmlNode* parent_node )
	//{
	//	station* v_station = (station*)ret_object;

	//	TiXmlElement* v_parent_elem = (TiXmlElement*)parent_node;
	//	TiXmlElement elem_station( "Station" );
	//	elem_station.SetAttribute( "name", string_ext::from_wstring( v_station->m_str_name ).c_str() );
	//	TiXmlNode* v_node = v_parent_elem->InsertEndChild( elem_station );
	//	TiXmlElement* e_station = v_node->ToElement();

	//	xml_parser_ext::from_double( v_station->m_longitude, e_station, "Longitude" );
	//	xml_parser_ext::from_double( v_station->m_latitude, e_station, "Latitude" );
	//	xml_parser_ext::from_double( v_station->m_altitude, e_station, "Altitude" );
	//	xml_parser_ext::from_float( v_station->m_heading, e_station, "Heading" );
	//	xml_parser_ext::from_float( v_station->m_tilt, e_station, "Tilt" );
	//	xml_parser_ext::from_float( v_station->m_range, e_station, "Range" );
	//	xml_parser_ext::from_float( v_station->m_time, e_station, "Time" );

	//	return NULL;
	//}
}