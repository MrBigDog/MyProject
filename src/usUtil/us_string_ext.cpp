///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_string_ext.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : value_convert class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////

#include "us_string_ext.h"
#include <usUtil/us_math.h>
#include <usUtil/us_lon_lat_rect.h>

namespace uniscope_globe
{

	// to
	bool string_ext::to_bool(string value)
	{
		if ("True" == value || "true" == value)
		{
			return true;
		}

		return false;
	}

	int	string_ext::to_int(string value)
	{
		return atoi(value.c_str());
	}

	dword	string_ext::to_dword(string value)
	{
		unsigned long total_value = 0;

		for (int i = 0; i < (int)value.size(); i++)
		{
			char a_char = value[i];

			long inter_result = 0;

			char upper_a = 'A';

			char lower_a = 'a';

			if (!isdigit(a_char))
			{
				if (!islower(a_char))
				{
					inter_result = a_char + 10;

					inter_result -= upper_a;
				}
				else
				{
					inter_result = a_char + 10;

					inter_result -= lower_a;
				}
			}
			else
			{
				inter_result = a_char - '0';
			}

			total_value *= 16;

			total_value += inter_result;
		}

		return total_value;
	}

	float	string_ext::to_float(string value)
	{
		return (float)atof(value.c_str());
	}

	double	string_ext::to_double(string value)
	{
		return atof(value.c_str());
	}

	interior_index string_ext::to_int64(string value)
	{
		interior_index ret;

		string str_tmp = "0x" + value;

#ifdef LINUX26
		sscanf(str_tmp.c_str(), "%llx", &ret);
#else
		sscanf(str_tmp.c_str(), "%I64x", &ret);
#endif //LINUX26

		return ret;
	}

	wstring	string_ext::to_wstring(string value)
	{

		/*
		int char_number = MultiByteToWideChar( CP_ACP, 0, value.c_str(), -1, NULL, 0 );

		wchar_t *wide_string = new wchar_t[char_number];
		MultiByteToWideChar( CP_ACP, 0, value.c_str(), -1, wide_string, char_number );

		ustring ret_string = wide_string;

		AUTO_DELETE(wide_string);

		return ret_string;
		*/

		setlocale(LC_ALL, ".ACP");
		//setlocale ( LC_ALL, "" );
		int required_size = mbstowcs(NULL, value.c_str(), 0);
		wchar_t* wide_string = new wchar_t[required_size + 1];
		mbstowcs(wide_string, value.c_str(), required_size + 1);

		wstring ret_string = wide_string;

		AUTO_DELETE(wide_string);

		return ret_string;
	}

#ifndef _UNICODE
	void string_ext::to_vector3_array(string value, std::vector< vector3<double> >& vec3_array)
	{
		vec3_array.clear();

		cpstr ptr = value.c_str();
		int i = 0;
		std::vector<string> double_array;
		while (ptr[i] != '\0')
		{
			while (ptr[i] == ' ' || ptr[i] == ',')
				i++;

			char buf[128];
			int j = 0;
			while ((ptr[i] >= '0' && ptr[i] <= '9') || ptr[i] == '.')
			{
				buf[j] = ptr[i];
				i++;
				j++;
			}
			buf[j] = '\0';

			double_array.push_back(buf);

			if (double_array.size() == 3)
			{
				vector3<double> vec;
				vec.x = atof(double_array[0].c_str());
				vec.y = atof(double_array[1].c_str());
				vec.z = atof(double_array[2].c_str());

				double_array.clear();

				vec3_array.push_back(vec);
			}

		}
	}
#endif //_UNICODE

	// from
	string	string_ext::from_bool(bool value)
	{
		if (value)
		{
			return string("true");
		}

		return string("false");
	}

	string	string_ext::from_int(int value)
	{
		char value_text[16];
		memset(value_text, 0, 16);

#ifdef LINUX26
		sprintf(value_text, "%d", value);

		return string(value_text);
#else
		return ltoa(value, value_text, 10);
#endif //LINUX26
	}

	string	string_ext::from_dword(dword value)
	{
		char value_text[16];
		memset(value_text, 0, 16);

#ifdef LINUX26
		sprintf(value_text, "%x", value);

		return string(value_text);
#else
		return ultoa(value, value_text, 16);
#endif //LINUX26
	}

	string	string_ext::from_float(float value)
	{
		char value_text[64];
		memset(value_text, 0, 64);

		sprintf(value_text, "%f", value);

		return string(value_text);
	}

	string	string_ext::from_double(double value)
	{
		char value_text[128];
		memset(value_text, 0, 128);

		sprintf(value_text, "%.16f", value);

		return string(value_text);
	}

	string string_ext::from_int64(interior_index value)
	{
		char str_i64[64];
		memset(str_i64, 0, 64);

#ifdef LINUX26
		sprintf(str_i64, "%llx", value);

		return string(str_i64);
#else
		return _i64toa(value, str_i64, 16);
#endif //LINUX26
	}

	string string_ext::from_vector3f(vector_3f& vec)
	{
		char value_text[256];
		memset(value_text, 0, 256);

		sprintf(value_text, "%.6f,%.6f,%.6f", vec.x, vec.y, vec.z);

		return string(value_text);
	}

	string string_ext::from_vector3d(vector_3d& vec)
	{
		char value_text[1024];
		memset(value_text, 0, 1024);

		sprintf(value_text, "%.16f,%.16f,%.16f", vec.x, vec.y, vec.z);

		return string(value_text);
	}

	string string_ext::from_lon_lat_rect(lon_lat_rect& v_rect)
	{
		char value_text[1024];
		memset(value_text, 0, 1024);

		double v_north = v_rect.m_north * math_d::s_rad_to_deg;
		double v_south = v_rect.m_south * math_d::s_rad_to_deg;
		double v_east = v_rect.m_east * math_d::s_rad_to_deg;
		double v_west = v_rect.m_west * math_d::s_rad_to_deg;

		sprintf(value_text, "%.16f,%.16f,%.16f,%.16f", v_north, v_south, v_east, v_west);

		return string(value_text);
	}

	string string_ext::from_quaternion_4d(quaternion_4d& v_quater)
	{
		char value_text[1024];
		memset(value_text, 0, 1024);

		sprintf(value_text, "%.16f,%.16f,%.16f,%.16f", v_quater.x, v_quater.y, v_quater.z, v_quater.w);

		return string(value_text);
	}

	string string_ext::from_quaternion_4f(quaternion_4f& v_quater)
	{
		char value_text[1024];
		memset(value_text, 0, 1024);

		sprintf(value_text, "%.6f,%.6f,%.6f,%.6f", v_quater.x, v_quater.y, v_quater.z, v_quater.w);

		return string(value_text);
	}

	string string_ext::from_vector_3d_array(std::vector<vector_3d>& in_vec_array)
	{
		string ret_str;
		if (in_vec_array.size() < 1) return ret_str;
		for (int ni = 0; ni < (INT_PTR)in_vec_array.size() - 1; ni++)
		{
			ret_str += string_ext::from_double(in_vec_array[ni].x) + ","
				+ string_ext::from_double(in_vec_array[ni].y) + ","
				+ string_ext::from_double(in_vec_array[ni].z) + ",";
		}

		ret_str += string_ext::from_double(in_vec_array[in_vec_array.size() - 1].x) + ","
			+ string_ext::from_double(in_vec_array[in_vec_array.size() - 1].y) + ","
			+ string_ext::from_double(in_vec_array[in_vec_array.size() - 1].z);

		return ret_str;
	}

	string string_ext::from_vector_3f_array(std::vector<vector_3f>& in_vec_array)
	{
		string ret_str;
		if (in_vec_array.size() < 1) return ret_str;
		for (int ni = 0; ni < (INT_PTR)in_vec_array.size() - 1; ni++)
		{
			ret_str += string_ext::from_float(in_vec_array[ni].x) + ","
				+ string_ext::from_float(in_vec_array[ni].y) + ","
				+ string_ext::from_float(in_vec_array[ni].z) + ",";
		}

		ret_str += string_ext::from_float(in_vec_array[in_vec_array.size() - 1].x) + ","
			+ string_ext::from_float(in_vec_array[in_vec_array.size() - 1].y) + ","
			+ string_ext::from_float(in_vec_array[in_vec_array.size() - 1].z);

		return ret_str;
	}

	string	string_ext::from_wstring(wstring value)
	{
		/*
		int char_number = WideCharToMultiByte( CP_ACP, 0, value.c_str(), -1, NULL, 0, NULL, NULL );

		char *multi_string = new char[char_number];
		WideCharToMultiByte( CP_ACP, 0, value.c_str(), -1, multi_string, char_number, NULL, NULL );

		string ret_string = multi_string;

		AUTO_DELETE(multi_string);

		return ret_string;
		*/

		setlocale(LC_ALL, ".ACP");
		//setlocale ( LC_ALL, "" );

		int required_size = wcstombs(NULL, value.c_str(), 0);
		char* mb_str = new char[required_size + 1];
		wcstombs(mb_str, value.c_str(), required_size + 1);

		string ret_string = mb_str;

		AUTO_DELETE(mb_str);

		return ret_string;
	}

	// w to
	bool string_ext::wto_bool(wstring value)
	{
		if (L"True" == value || L"true" == value)
		{
			return true;
		}

		return false;
	}

	int string_ext::wto_int(wstring value)
	{
		string mb_str = string_ext::from_wstring(value);

		return string_ext::to_int(mb_str);
	}

	dword string_ext::wto_dword(wstring value)
	{
		string mb_str = string_ext::from_wstring(value);

		return string_ext::to_dword(mb_str);
	}

	float string_ext::wto_float(wstring value)
	{
		string mb_str = string_ext::from_wstring(value);

		return string_ext::to_float(mb_str);
	}

	double string_ext::wto_double(wstring value)
	{
		string mb_str = string_ext::from_wstring(value);

		return string_ext::to_double(mb_str);
	}

	interior_index	string_ext::wto_int64(wstring value)
	{
		//string mb_str = string_ext::from_wstring ( value );

		//return string_ext::to_int64( mb_str );
		__int64 idx = 0;

		const wchar_t* buf = value.c_str();

		int size = value.size();
		unsigned short tmp = 0;
		for (int ni = 0; ni < size; ni++)
		{
			tmp = (unsigned short)(buf[ni]);
			if (tmp > 47 && tmp < 58)
			{
				tmp -= '0';
			}
			else if (tmp > 64 && tmp < 71)
			{
				tmp -= 55;
			}
			else if (tmp > 96 && tmp < 103)
			{
				tmp -= 87;
			}
			else
			{
				return 0;
			}

			idx += ((__int64)tmp) << ((size - ni - 1) * 4);
		}

		return idx;
	}

	// w from
	wstring	string_ext::wfrom_bool(bool value)
	{
		if (value)
		{
			return wstring(L"true");
		}

		return wstring(L"false");
	}

	wstring	string_ext::wfrom_int(int value)
	{
		string mb_str = string_ext::from_int(value);

		return string_ext::to_wstring(mb_str);
	}

	wstring string_ext::wfrom_dword(dword value)
	{
		string mb_str = string_ext::from_dword(value);

		return string_ext::to_wstring(mb_str);
	}

	wstring string_ext::wfrom_float(float value)
	{
		string mb_str = string_ext::from_float(value);

		return string_ext::to_wstring(mb_str);
	}

	wstring	string_ext::wfrom_double(double value)
	{
		string mb_str = string_ext::from_double(value);

		return string_ext::to_wstring(mb_str);
	}

	wstring string_ext::wfrom_int64(interior_index value)
	{
		string mb_str = string_ext::from_int64(value);

		return string_ext::to_wstring(mb_str);
	}

	wstring string_ext::wfrom_vector3d(vector_3d& vec)
	{
		string mb_str = string_ext::from_vector3d(vec);

		return string_ext::to_wstring(mb_str);
	}

	wstring	string_ext::wfrom_vector_3d_array(std::vector<vector_3d>& in_vec_array)
	{
		wstring mb_str;
		if (in_vec_array.size() < 1) return mb_str;
		for (int ni = 0; ni < (INT_PTR)in_vec_array.size() - 1; ni++)
		{
			mb_str += string_ext::wfrom_double(in_vec_array[ni].x) + L","
				+ string_ext::wfrom_double(in_vec_array[ni].y) + L","
				+ string_ext::wfrom_double(in_vec_array[ni].z) + L",";
		}

		mb_str += string_ext::wfrom_double(in_vec_array[in_vec_array.size() - 1].x) + L","
			+ string_ext::wfrom_double(in_vec_array[in_vec_array.size() - 1].y) + L","
			+ string_ext::wfrom_double(in_vec_array[in_vec_array.size() - 1].z);

		return mb_str;
	}

	wstring string_ext::wfrom_vector_3d_array_2d(std::vector<vector_3d>& in_vec_array)
	{
		wstring mb_str;
		if (in_vec_array.size() < 1) return mb_str;
		for (int ni = 0; ni < (INT_PTR)in_vec_array.size() - 1; ni++)
		{
			mb_str += string_ext::wfrom_double(in_vec_array[ni].x) + L","
				+ string_ext::wfrom_double(in_vec_array[ni].y) + L",";
		}

		mb_str += string_ext::wfrom_double(in_vec_array[in_vec_array.size() - 1].x) + L","
			+ string_ext::wfrom_double(in_vec_array[in_vec_array.size() - 1].y);

		return mb_str;
	}

}