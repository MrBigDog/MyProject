//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_xml_parser_ext.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : 
//
//////////////////////////////////////////////////////////////////////////
#ifndef _US_XML_PARSER_EXTENSION_H_
#define _US_XML_PARSER_EXTENSION_H_

#include <usUtil\Export.h>
#include <usUtil\us_vector3.h>
#include <usUtil\us_rect.h>
#include <usUtil\us_common_file.h>
#include <usUtil\us_quaternion.h>
#include <vector>
#include <string>

class TiXmlNode;

using namespace std;

namespace uniscope_globe
{
	typedef std::vector<vector_3d> point_list_d;
	typedef std::vector<vector3<ushort> > index_list_u;

	typedef std::vector<double> double_list;
	typedef std::vector<int> int_list;

	class lon_lat_rect;
	class lon_lat_box;

	class USUTIL_EXPORT xml_parser_ext
	{
	public:
		xml_parser_ext(void) { }

		virtual ~xml_parser_ext(void) { }

	public:
		static vector_3f		to_vector_3f(string value);

		static vector_3d		to_vector_3d(string value);

		static rect				to_rect(string  value);

		static double_list		to_double_list(string value);

		static int_list			to_int_list(string value);

		static double_list      to_double_array(string value);

		static point_list_d		to_point_list_d(string value);

		static index_list_u		to_index_list_u(string value);

		static lon_lat_rect		to_lon_lat_rect(string value);

		static lon_lat_box		to_lon_lat_box(string value);

		static quaternion_4f	to_quaternion_4f(string value);

		static quaternion_4d	to_quaternion_4d(string value);

		static long				to_color_value_long(string value);

	public:
#ifdef WIN32
		static bool from_dword(dword val, TiXmlNode* parent_node, string node_name);

		static bool from_int(int val, TiXmlNode* parent_node, string node_name);

		static bool from_float(float val, TiXmlNode* parent_node, string node_name);

		static bool from_double(double val, TiXmlNode* parent_node, string node_name);

		static bool from_wide_string(ustring val, TiXmlNode* parent_node, string node_name);

		static bool from_multi_string(string val, TiXmlNode* parent_node, string node_name);

		static bool from_vector_3d(vector_3d& vec, TiXmlNode* parent_node, string node_name);

		static bool from_point_list(point_list_d& a_point_list, TiXmlNode* parent_node, string node_name);
#endif //WIN32
	};

}

#endif //_US_XML_PARSER_EXTENSION_H_
