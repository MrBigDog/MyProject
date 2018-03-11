///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_projection_parser.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : projection_parser class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_projection_parser.h"

namespace uniscope_globe
{
	projection_parser::projection_parser( void )
	{
	}
	projection_parser::~projection_parser( void )
	{
	}

	object_base* projection_parser::forward_parse( object_base* v_container, TiXmlNode* v_xml_node, object_base* parent_object )
	{
		uniscope_projection* v_proj = (uniscope_projection*)parent_object;
		v_proj = NULL;
		// parse_projection: Datum, CoordinateType, (ZoneIndex / CentralMeridian& ReferanceLatitude), Trans, 
		TiXmlNode* v_child_node;
		TiXmlElement* v_child_elem;

		// proj_type
		v_child_node = v_xml_node->FirstChild("ProjectionType");
		if (v_child_node == NULL) return NULL;
		v_child_elem = (TiXmlElement*)v_child_node;
		int v_proj_type = string_ext::to_int(v_child_elem->GetText());

		switch(v_proj_type)
		{
		case 0:
			return NULL;
		case 1:
			v_proj = new gauss_projection();
			((gauss_projection*)v_proj)->set_zone_wide(3);
			break;
		case 2:
			v_proj = new gauss_projection();
			((gauss_projection*)v_proj)->set_zone_wide(6);
			break;
		case 3:
			v_proj = new mercator_projection();
			break;
		case 4:
			v_proj = new utm_projection();
			break;
		default:
			return NULL;
		}

		// datum;
		v_child_node = v_xml_node->FirstChild("Datum");
		if (v_child_node == NULL) return NULL;
		v_child_elem = (TiXmlElement*)v_child_node;
		int v_datum = string_ext::to_int(v_child_elem->GetText());

		switch(v_datum)
		{
		case 0:
			v_proj->set_semi(6378137, 6356752.3142451793);
			break;
		case 1:
			v_proj->set_semi(6378140, 6356755.2881575283);
			break;
		case 2:
			v_proj->set_semi(6378245, 6356863.01887730473);
			break;
		case 3:
			v_proj->set_semi(6378137, 6356752.31414);
			break;
		default:
			return NULL;
		}

		// other proj parameter:
		if (v_proj_type == 1 || v_proj_type == 2 )
		{
			v_child_node = v_xml_node->FirstChild("ZoneIndex");
			if (v_child_node == NULL) return NULL;
			v_child_elem = (TiXmlElement*)v_child_node;
			((gauss_projection*)v_proj)->m_zone_index = string_ext::to_int(v_child_elem->GetText());
		}
		else if (v_proj_type == 3)
		{
			v_child_node = v_xml_node->FirstChild("CentralMeridian");
			if (v_child_node == NULL) return NULL;
			v_child_elem = (TiXmlElement*)v_child_node;
			double v_central_meridian = string_ext::to_double(v_child_elem->GetText());
			v_child_node = v_xml_node->FirstChild("ReferanceLatitude");
			if (v_child_node == NULL) return NULL;
			v_child_elem = (TiXmlElement*)v_child_node;
			double v_ref_lat = string_ext::to_double(v_child_elem->GetText());
			((mercator_projection*)v_proj)->set_central(v_central_meridian, v_ref_lat);
		}
		else if (v_proj_type == 4)
		{
			v_child_node = v_xml_node->FirstChild("ZoneIndex");
			if (v_child_node == NULL) return NULL;
			v_child_elem = (TiXmlElement*)v_child_node;
			((utm_projection*)v_proj)->m_zone_index = string_ext::to_int(v_child_elem->GetText());
		}

		return v_proj;
	}


	bool projection_parser::forward_parse(TiXmlNode* v_xml_node, uniscope_projection*& parent_object)
	{
		parent_object = NULL;
		// parse_projection: Datum, CoordinateType, (ZoneIndex / CentralMeridian& ReferanceLatitude), Trans, 
		TiXmlNode* v_child_node;
		TiXmlElement* v_child_elem;

		// proj_type
		v_child_node = v_xml_node->FirstChild("ProjectionType");
		if (v_child_node == NULL) return false;
		v_child_elem = (TiXmlElement*)v_child_node;
		int v_proj_type = string_ext::to_int(v_child_elem->GetText());

		switch(v_proj_type)
		{
		case 0:
			return false;
		case 1:
			parent_object = new gauss_projection();
			((gauss_projection*)parent_object)->set_zone_wide(3);
			break;
		case 2:
			parent_object = new gauss_projection();
			((gauss_projection*)parent_object)->set_zone_wide(6);
			break;
		case 3:
			parent_object = new mercator_projection();
			break;
		case 4:
			parent_object = new utm_projection();
			break;
		default:
			return false;
		}

		// datum;
		v_child_node = v_xml_node->FirstChild("Datum");
		if (v_child_node == NULL) return false;
		v_child_elem = (TiXmlElement*)v_child_node;
		int v_datum = string_ext::to_int(v_child_elem->GetText());

		switch(v_datum)
		{
		case 0:
			parent_object->set_semi(6378137, 6356752.31424518);
			break;
		case 1:
			parent_object->set_semi(6378140, 6356755.2881575283);
			break;
		case 2:
			parent_object->set_semi(6378245, 6356863.018877305);
			break;
		case 3:
			parent_object->set_semi(6378137, 6356752.31414);
			break;
		default:
			return false;
		}

		// other proj parameter:
		if (v_proj_type == 1 || v_proj_type == 2 )
		{
			v_child_node = v_xml_node->FirstChild("ZoneIndex");
			if (v_child_node == NULL) return false;
			v_child_elem = (TiXmlElement*)v_child_node;
			((gauss_projection*)parent_object)->m_zone_index = string_ext::to_int(v_child_elem->GetText());
		}
		else if (v_proj_type == 3)
		{
			v_child_node = v_xml_node->FirstChild("CentralMeridian");
			if (v_child_node == NULL) return false;
			v_child_elem = (TiXmlElement*)v_child_node;
			double v_central_meridian = string_ext::to_double(v_child_elem->GetText());
			v_child_node = v_xml_node->FirstChild("ReferanceLatitude");
			if (v_child_node == NULL) return false;
			v_child_elem = (TiXmlElement*)v_child_node;
			double v_ref_lat = string_ext::to_double(v_child_elem->GetText());
			((mercator_projection*)parent_object)->set_central(v_central_meridian, v_ref_lat);
		}
		else if (v_proj_type == 4)
		{
			v_child_node = v_xml_node->FirstChild("ZoneIndex");
			if (v_child_node == NULL) return false;
			v_child_elem = (TiXmlElement*)v_child_node;
			((utm_projection*)parent_object)->m_zone_index = string_ext::to_int(v_child_elem->GetText());
		}

		return true;
	}
}


 