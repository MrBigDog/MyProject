///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_dbf_file.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : dbf_file class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////

#include "us_dbf_file.h"
#include <usUtil/us_string_ext.h>

#include <string>

using namespace std;

namespace uniscope_globe
{
	dbf_file::dbf_file(void)
	{
		m_n_record_num = 0;
		m_n_attri_num = 0;
		m_h_dbf = NULL;
	}
	dbf_file::~dbf_file(void)
	{
		if (m_h_dbf)
		{
			DBFClose(m_h_dbf);
		}
	}

	bool dbf_file::init_dbf_file(LPCTSTR str_path)
	{

		m_str_file = string_ext::from_wstring(wstring(str_path));
		m_h_dbf = DBFOpen(m_str_file.data(), "rb+");
		if (m_h_dbf == NULL)
		{
			return false;
		}
		m_n_attri_num = DBFGetFieldCount(m_h_dbf);
		m_n_record_num = DBFGetRecordCount(m_h_dbf);
		return true;
	}

	int dbf_file::get_record_count()
	{
		return m_n_record_num;
	}

	int dbf_file::get_attri_count()
	{
		return m_n_attri_num;
	}

	bool dbf_file::get_field_info(int n_attri_index, char* ptr_text, DBFFieldType& field_type, int& field_width, int& n_decimal)
	{
		char v_char_field_name[12];
		field_type = DBFGetFieldInfo(m_h_dbf, n_attri_index, v_char_field_name, &field_width, &n_decimal);
		memcpy(ptr_text, v_char_field_name, 12);
		return true;
	}

	bool dbf_file::get_attribute_text(int n_shape_index, int n_attri_index, char* ptr_text)
	{
		const char* v_ptr_char_text;
		v_ptr_char_text = DBFReadStringAttribute(m_h_dbf, n_shape_index, n_attri_index);

		int v_len = strlen(v_ptr_char_text);
		memcpy(ptr_text, v_ptr_char_text, v_len);
		ptr_text[v_len] = '\0';

		return true;
	}

	bool dbf_file::get_attribute_int(int n_shape_index, int n_attri_index, int* ptr_int)
	{
		*ptr_int = DBFReadIntegerAttribute(m_h_dbf, n_shape_index, n_attri_index);
		return true;
	}

	bool dbf_file::get_attribute_double(int n_shape_index, int n_attri_index, double* ptr_double)
	{
		*ptr_double = DBFReadDoubleAttribute(m_h_dbf, n_shape_index, n_attri_index);
		return true;
	}

	bool dbf_file::set_attribute_text(int n_shape_index, int n_attri_index, char* ptr_text)
	{
		int ret = DBFWriteStringAttribute(m_h_dbf, n_shape_index, n_attri_index, ptr_text);

		return true;
	}

	bool dbf_file::set_attribute_int(int n_shape_index, int n_attri_index, int ptr_int)
	{
		DBFWriteIntegerAttribute(m_h_dbf, n_shape_index, n_attri_index, ptr_int);

		return true;
	}

	bool dbf_file::set_attribute(int n_shape_index, int n_attri_index, void* ptr_value)
	{
		DBFWriteAttributeDirectly(m_h_dbf, n_shape_index, n_attri_index, ptr_value);

		return true;
	}



	bool dbf_file::set_attribute_double(int n_shape_index, int n_attri_index, double ptr_double)
	{
		DBFWriteDoubleAttribute(m_h_dbf, n_shape_index, n_attri_index, ptr_double);

		return true;
	}

	bool dbf_file::create_dbf(LPCTSTR str_path)
	{
		m_h_dbf = DBFCreate(string(str_path).c_str()/*string_ext::from_wstring(wstring(str_path)).c_str()*/);

		return true;
	}

	bool dbf_file::add_dbf_field(const char* field_name, DBFFieldType field_type, int field_width, int n_decimal)
	{
		if (m_h_dbf == NULL)
			return false;
		DBFAddField(m_h_dbf, field_name, field_type, field_width, n_decimal);
		return true;
	}

	bool dbf_file::add_dbf_fields(dbf_field_info_array& v_dbf_field_info_array)
	{
		if (m_h_dbf == NULL)
			return false;

		m_n_record_num = DBFGetRecordCount(m_h_dbf);
		if (m_n_record_num == 0)
		{
			for (int ni = 0; ni < v_dbf_field_info_array.size(); ni++)
			{
				DBFAddField(m_h_dbf, v_dbf_field_info_array[ni].m_field_name,
					v_dbf_field_info_array[ni].m_field_type,
					v_dbf_field_info_array[ni].m_field_width,
					v_dbf_field_info_array[ni].m_decimal);
			}

			return true;
		}

		int v_pos = m_str_file.rfind(".");
		string str_tmp_dbf = m_str_file;
		str_tmp_dbf.insert(v_pos, "_tmp");

		DBFHandle tmp_handle = DBFCreate(str_tmp_dbf.c_str());
		if (tmp_handle == NULL)
			return false;

		dbf_field_info tmp_info;
		dbf_field_info_array v_old_info_array;
		for (int ni = 0; ni < m_n_attri_num; ni++)
		{
			memset(tmp_info.m_field_name, 0, 12);
			get_field_info(ni, tmp_info.m_field_name, tmp_info.m_field_type, tmp_info.m_field_width, tmp_info.m_decimal);

			v_old_info_array.push_back(tmp_info);

			DBFAddField(tmp_handle, tmp_info.m_field_name,
				tmp_info.m_field_type,
				tmp_info.m_field_width,
				tmp_info.m_decimal);

		}

		for (int ni = 0; ni < v_dbf_field_info_array.size(); ni++)
		{
			DBFAddField(tmp_handle, v_dbf_field_info_array[ni].m_field_name,
				v_dbf_field_info_array[ni].m_field_type,
				v_dbf_field_info_array[ni].m_field_width,
				v_dbf_field_info_array[ni].m_decimal);
		}

		for (int ni = 0; ni < m_n_record_num; ni++)
		{
			for (int nj = 0; nj < m_n_attri_num; nj++)
			{
				switch (v_old_info_array[nj].m_field_type)
				{
				case FTString:
				{
					const char* v_value = NULL;
					v_value = DBFReadStringAttribute(m_h_dbf, ni, nj);

					DBFWriteStringAttribute(tmp_handle, ni, nj, v_value);
				}
				break;
				case FTInteger:
				{
					int v_value = DBFReadIntegerAttribute(m_h_dbf, ni, nj);

					DBFWriteIntegerAttribute(tmp_handle, ni, nj, v_value);
				}
				break;
				case FTDouble:
				{
					double v_value = DBFReadDoubleAttribute(m_h_dbf, ni, nj);

					DBFWriteDoubleAttribute(tmp_handle, ni, nj, v_value);
				}
				break;
				}
			}
		}

		if (m_h_dbf)
		{
			DBFClose(m_h_dbf);
		}
		if (tmp_handle)
		{
			DBFClose(tmp_handle);
		}

		::DeleteFileA((m_str_file + "_bak").c_str());
		rename(m_str_file.c_str(), (m_str_file + "_bak").c_str());
		rename(str_tmp_dbf.c_str(), m_str_file.c_str());

		m_h_dbf = DBFOpen(m_str_file.data(), "rb+");
		if (m_h_dbf == NULL)
		{
			return false;
		}
		m_n_attri_num = DBFGetFieldCount(m_h_dbf);
		m_n_record_num = DBFGetRecordCount(m_h_dbf);
		return true;
	}

	bool dbf_file::write_attribute(int shape_index, int field_index, const char* field_data)
	{
		if (m_h_dbf == NULL)
			return false;

		DBFFieldType field_type = DBFGetFieldInfo(m_h_dbf, field_index, NULL, NULL, NULL);

		switch (field_type)
		{
		case FTString:
			DBFWriteStringAttribute(m_h_dbf, shape_index, field_index, field_data);
			break;
		case FTInteger:
			DBFWriteIntegerAttribute(m_h_dbf, shape_index, field_index, string_ext::to_int(string(field_data)));
			break;
		case FTDouble:
			DBFWriteDoubleAttribute(m_h_dbf, shape_index, field_index, string_ext::to_double(string(field_data)));
			break;
		case FTLogical:
			DBFWriteLogicalAttribute(m_h_dbf, shape_index, field_index, string_ext::to_bool(string(field_data)));
			break;
		default:
			break;
		}
		return true;
	}

}


