//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_xml_parser_ext.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : 
//
//////////////////////////////////////////////////////////////////////////

#include "us_xml_parser_ext.h"
#include <usUtil/us_lon_lat_rect.h>
#include <usUtil/us_lon_lat_box.h>
#include <usUtil/us_spherical_coords.h>
#include <usUtil/us_math.h>
#include <usUtil/us_string_ext.h>

#include <usXml/tinyxml.h>

namespace uniscope_globe
{
	vector_3f xml_parser_ext::to_vector_3f(string value)
	{
		vector_3f return_value_3f;

		sscanf(value.c_str(),
			"%f,%f,%f",
			&return_value_3f.x,
			&return_value_3f.y,
			&return_value_3f.z);

		return return_value_3f;
	}

	vector_3d xml_parser_ext::to_vector_3d(string value)
	{
		vector_3d return_value_3f;

		sscanf(value.c_str(),
			"%lf,%lf,%lf",
			&return_value_3f.x,
			&return_value_3f.y,
			&return_value_3f.z);

		return return_value_3f;
	}


	rect xml_parser_ext::to_rect(string value)
	{
		rect ret_rect;

		sscanf(value.c_str(),
			"%d,%d,%d,%d",
			&ret_rect.m_left,
			&ret_rect.m_top,
			&ret_rect.m_right,
			&ret_rect.m_bottom);

		return ret_rect;
	}


	lon_lat_rect xml_parser_ext::to_lon_lat_rect(string value)
	{
		lon_lat_rect ret_lon_lat_rect;

		sscanf(value.c_str(),
			"%lf,%lf,%lf,%lf",
			&ret_lon_lat_rect.m_north,
			&ret_lon_lat_rect.m_south,
			&ret_lon_lat_rect.m_east,
			&ret_lon_lat_rect.m_west);

		ret_lon_lat_rect.m_north *= math_d::s_deg_to_rad;
		ret_lon_lat_rect.m_south *= math_d::s_deg_to_rad;
		ret_lon_lat_rect.m_east *= math_d::s_deg_to_rad;
		ret_lon_lat_rect.m_west *= math_d::s_deg_to_rad;

		return ret_lon_lat_rect;
	}

	lon_lat_box xml_parser_ext::to_lon_lat_box(string value)
	{

		double north, south, east, west, bottom, top;

		sscanf(value.c_str(),
			"%lf,%lf,%lf,%lf,%lf,%lf",
			&north,
			&south,
			&east,
			&west,
			&top,
			&bottom);

		return lon_lat_box(north*RADIAN, south*RADIAN, east*RADIAN, west*RADIAN,
			bottom * 0.001 + spherical_coords::s_semi_major,
			top * 0.001 + spherical_coords::s_semi_major);
	}

	quaternion_4f xml_parser_ext::to_quaternion_4f(string value)
	{
		quaternion_4f ret_quaternion;

		sscanf(value.c_str(),
			"%f,%f,%f,%f",
			&ret_quaternion.x,
			&ret_quaternion.y,
			&ret_quaternion.z,
			&ret_quaternion.w);

		return ret_quaternion;
	}

	quaternion_4d xml_parser_ext::to_quaternion_4d(string value)
	{
		quaternion_4d ret_quaternion;

		sscanf(value.c_str(),
			"%lf,%lf,%lf,%lf",
			&ret_quaternion.x,
			&ret_quaternion.y,
			&ret_quaternion.z,
			&ret_quaternion.w);

		return ret_quaternion;
	}

	long xml_parser_ext::to_color_value_long(string value)
	{
		float a, r, g, b;
		sscanf(value.c_str(), "%f,%f,%f,%f", &a, &r, &g, &b);

		long v_color = USCOLOR_COLORVALUE(a, r, g, b);

		return v_color;
	}

	point_list_d xml_parser_ext::to_point_list_d(string value)
	{
		point_list_d a_point_list;

		double_list a_double;

		a_double = to_double_list(value);

		int number_of_points = a_double.size();

		number_of_points = number_of_points / 3;

		for (int i = 0; i < number_of_points; i++)
		{
			vector_3d temp_vector;

			temp_vector.x = a_double[3 * i + 0];
			temp_vector.y = a_double[3 * i + 1];
			temp_vector.z = a_double[3 * i + 2];

			a_point_list.push_back(temp_vector);
		}

		return a_point_list;
	}

	index_list_u xml_parser_ext::to_index_list_u(string value)
	{
		index_list_u v_index_list;

		int_list v_int;

		v_int = to_int_list(value);

		int number_of_points = v_int.size();

		number_of_points = number_of_points / 3;

		for (int i = 0; i < number_of_points; i++)
		{
			vector3<ushort> temp_vector;

			temp_vector.x = ushort(v_int[3 * i + 0]);
			temp_vector.y = ushort(v_int[3 * i + 1]);
			temp_vector.z = ushort(v_int[3 * i + 2]);

			v_index_list.push_back(temp_vector);
		}

		return v_index_list;
	}

	int_list xml_parser_ext::to_int_list(string value)
	{
		int current_coordinates = 0;

		int_list v_int;

		long current_position = 0;
		long length_of_string = value.size();

		int v_value = 0.0;
		bool is_negative = false;
		int j = -1;
		char last_char = ' ';

		while (current_position < length_of_string)
		{
			char a_char = value[current_position++];

			if (a_char == '-')
			{
				is_negative = true;
				j = -1;
				last_char = a_char;
				continue;
			}
			else if (a_char == ',')
			{
				v_value = is_negative ? -v_value : v_value;
				v_int.push_back(v_value);
				current_coordinates++;
				is_negative = false;
				v_value = 0.0;
				j = -1;
				last_char = a_char;
				continue;
			}
			else if (a_char == ' ')
			{
				if (last_char != ' ')
				{
					v_value = is_negative ? -v_value : v_value;

					v_int.push_back(v_value);
					current_coordinates++;

					is_negative = false;

					if (current_coordinates % 3 != 0)
					{
						v_int.push_back(0);
						current_coordinates++;
					}
				}
				v_value = 0.0;
				j = -1;
				last_char = a_char;
				continue;
			}
			else if (a_char <= '9' && a_char >= '0')
			{
				int digit = a_char - '0';

				v_value = v_value * 10 + digit;

				last_char = a_char;
			}

		}

		if (current_position == length_of_string)
		{
			v_value = is_negative ? -v_value : v_value;
			v_int.push_back(v_value);
			current_coordinates++;

			is_negative = false;


			if (current_coordinates % 3 != 0)
			{
				v_int.push_back(0);
				current_coordinates++;
			}
		}

		return v_int;
	}

	double_list xml_parser_ext::to_double_list(string value)
	{
		int current_coordinates = 0;

		double_list a_double;

		long current_position = 0;
		long length_of_string = value.size();

		bool is_integer_part = true;
		double total_value = 0.0;
		bool is_negative = false;
		int j = -1;
		char last_char = ' ';

		while (current_position < length_of_string)
		{
			char a_char = value[current_position++];

			if (a_char == '-')
			{
				is_negative = true;
				is_integer_part = true;
				j = -1;
				last_char = a_char;
				continue;
			}
			else if (a_char == '.')
			{
				is_integer_part = false;
				j = -1;
				last_char = a_char;
				continue;
			}
			else if (a_char == ',')
			{
				is_integer_part = true;
				total_value = is_negative ? -total_value : total_value;
				a_double.push_back(total_value);
				current_coordinates++;
				is_negative = false;
				total_value = 0.0;
				j = -1;
				last_char = a_char;
				continue;
			}
			else if (a_char == ' ')
			{
				is_integer_part = true;
				if (last_char != ' ')
				{
					total_value = is_negative ? -total_value : total_value;

					a_double.push_back(total_value);
					current_coordinates++;

					is_negative = false;

					if (current_coordinates % 3 != 0)
					{
						a_double.push_back(0.0);
						current_coordinates++;
					}
				}
				total_value = 0.0;
				j = -1;
				last_char = a_char;
				continue;
			}
			else if (a_char <= '9' && a_char >= '0')
			{
				double digit = a_char - '0';

				if (is_integer_part) {
					total_value = total_value * 10 + digit;
				}
				else if (!is_integer_part)
				{
					total_value = total_value + digit * pow(10.0, j--);
				}


				last_char = a_char;
			}

		}

		if (current_position == length_of_string)
		{
			total_value = is_negative ? -total_value : total_value;
			a_double.push_back(total_value);
			current_coordinates++;

			is_negative = false;


			if (current_coordinates % 3 != 0)
			{
				a_double.push_back(0.0);
				current_coordinates++;
			}
		}

		return a_double;

	}


	double_list xml_parser_ext::to_double_array(string value)
	{
		int current_coordinates = 0;

		double_list a_double;

		long current_position = 0;
		long length_of_string = value.size();

		bool is_integer_part = true;
		double total_value = 0.0;
		bool is_negative = false;
		int j = -1;
		char last_char = ' ';

		while (current_position < length_of_string)
		{
			char a_char = value[current_position++];

			if (a_char == '-')
			{
				is_negative = true;
				is_integer_part = true;
				j = -1;
				last_char = a_char;
				continue;
			}
			else if (a_char == '.')
			{
				is_integer_part = false;
				j = -1;
				last_char = a_char;
				continue;
			}
			else if (a_char == ',')
			{
				is_integer_part = true;
				total_value = is_negative ? -total_value : total_value;
				a_double.push_back(total_value);
				current_coordinates++;
				is_negative = false;
				total_value = 0.0;
				j = -1;
				last_char = a_char;
				continue;
			}
			else if (a_char == ' ')
			{
				is_integer_part = true;
				if (last_char != ' ')
				{
					total_value = is_negative ? -total_value : total_value;

					a_double.push_back(total_value);
					current_coordinates++;

					is_negative = false;
				}
				total_value = 0.0;
				j = -1;
				last_char = a_char;
				continue;
			}
			else if (a_char <= '9' && a_char >= '0')
			{
				double digit = a_char - '0';

				if (is_integer_part) {
					total_value = total_value * 10 + digit;
				}
				else if (!is_integer_part)
				{
					total_value = total_value + digit * pow(10.0, j--);
				}


				last_char = a_char;
			}

		}

		if (current_position == length_of_string)
		{
			total_value = is_negative ? -total_value : total_value;
			a_double.push_back(total_value);
			current_coordinates++;

			is_negative = false;
		}

		return a_double;

	}


#ifdef WIN32
	bool xml_parser_ext::from_dword(DWORD value_dword, TiXmlNode* parent_node, string node_name)
	{
		TiXmlElement* xml_element = (TiXmlElement*)parent_node;

		TiXmlElement xml_element_addtion(node_name.c_str());

		TiXmlNode* xml_node = xml_element->InsertEndChild(xml_element_addtion);

		TiXmlElement* temp_element = xml_node->ToElement();

		TiXmvgxt xml_text_value(string_ext::from_dword(value_dword).c_str());

		temp_element->InsertEndChild(xml_text_value);

		return true;
	}

	bool xml_parser_ext::from_float(float value_float, TiXmlNode* parent_node, string node_name)
	{
		TiXmlElement* xml_element = (TiXmlElement*)parent_node;

		TiXmlElement xml_element_addtion(node_name.c_str());

		TiXmlNode* xml_node = xml_element->InsertEndChild(xml_element_addtion);

		TiXmlElement* temp_element = xml_node->ToElement();

		TiXmvgxt xml_text_value(string_ext::from_float(value_float).c_str());

		temp_element->InsertEndChild(xml_text_value);

		return true;
	}

	bool xml_parser_ext::from_double(double value_double, TiXmlNode* parent_node, string node_name)
	{
		TiXmlElement* xml_element = (TiXmlElement*)parent_node;

		TiXmlElement xml_element_addtion(node_name.c_str());

		TiXmlNode* xml_node = xml_element->InsertEndChild(xml_element_addtion);

		TiXmlElement* temp_element = xml_node->ToElement();

		TiXmvgxt xml_text_value(string_ext::from_double(value_double).c_str());

		temp_element->InsertEndChild(xml_text_value);

		return true;
	}

	bool xml_parser_ext::from_wide_string(ustring value, TiXmlNode* parent_node, string node_name)
	{
		TiXmlElement* xml_element = (TiXmlElement*)parent_node;

		TiXmlElement xml_element_addtion(node_name.c_str());

		TiXmlNode* xml_node = xml_element->InsertEndChild(xml_element_addtion);

		TiXmlElement* temp_element = xml_node->ToElement();

#ifdef _UNICODE
		TiXmvgxt xml_text_value(string_ext::from_wstring(value).c_str());
#else
		TiXmvgxt xml_text_value(value.c_str());
#endif //_UNICODE

		temp_element->InsertEndChild(xml_text_value);

		return true;
	}

	bool xml_parser_ext::from_multi_string(string value, TiXmlNode* parent_node, string node_name)
	{
		TiXmlElement* xml_element = (TiXmlElement*)parent_node;

		TiXmlElement xml_element_addtion(node_name.c_str());

		TiXmlNode* xml_node = xml_element->InsertEndChild(xml_element_addtion);

		TiXmlElement* temp_element = xml_node->ToElement();

		TiXmvgxt xml_text_value(value.c_str());

		temp_element->InsertEndChild(xml_text_value);

		return true;
	}

	bool xml_parser_ext::from_vector_3d(vector_3d& return_value_3f, TiXmlNode* parent_node, string node_name)
	{
		TiXmlElement* xml_element = (TiXmlElement*)parent_node;

		TiXmlElement xml_element_addtion(node_name.c_str());

		TiXmlNode* xml_node = xml_element->InsertEndChild(xml_element_addtion);

		TiXmlElement* temp_element = xml_node->ToElement();

		char value_text[512];
		memset(value_text, 0, 512);
		sprintf(value_text, "%.10f,%.10f,%.10f", return_value_3f.x, return_value_3f.y, return_value_3f.z);
		TiXmvgxt xml_text_value(value_text);

		temp_element->InsertEndChild(xml_text_value);

		return true;
	}

	bool xml_parser_ext::from_point_list(point_list_d& a_point_list, TiXmlNode* parent_node, string node_name)
	{
		TiXmlElement* xml_element = (TiXmlElement*)parent_node;

		TiXmlElement xml_element_addtion(node_name.c_str());

		TiXmlNode* xml_node = xml_element->InsertEndChild(xml_element_addtion);

		TiXmlElement* temp_element = xml_node->ToElement();


		string value;
		for (int i = 0; i < (int)a_point_list.size(); i++)
		{
			char value_text[512];
			memset(value_text, 0, 512);
			sprintf(value_text, "%.10f,%.10f,%.10f", a_point_list[i].x, a_point_list[i].y, a_point_list[i].z);
			if (i == 0)
			{
				value = value + value_text;
				continue;
			}

			value = value + " " + value_text;
		}

		TiXmvgxt xml_text_value(value.c_str());

		temp_element->InsertEndChild(xml_text_value);

		return true;
	}

#endif //WIN32

}
