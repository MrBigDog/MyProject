///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_dbf_file.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : dbf_file class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_DBF_FILE_H_
#define _US_DBF_FILE_H_

#include <usUtil\Export.h>
#include <usUtil\shapefil.h>

#include <vector>
#include <wtypes.h>

using namespace std;

namespace uniscope_globe
{
	struct dbf_field_info
	{
		int m_index;
		char m_field_name[12];
		DBFFieldType m_field_type;
		int m_field_width;
		int m_decimal;
		dbf_field_info()
		{
			m_index = -1;
			memset(m_field_name, 0, 12);
			m_field_type = FTString;
			m_field_width = 0;
			m_decimal = 0;
		}
	};
	typedef std::vector<dbf_field_info> dbf_field_info_array;

	class USUTIL_EXPORT dbf_file
	{
	public:
		dbf_file(void);
		virtual ~dbf_file(void);
	public:
		// read about
		bool init_dbf_file(LPCTSTR str_path);

		int get_record_count();
		int get_attri_count();

		bool get_field_info(int n_attri_index, char* ptr_text, DBFFieldType& field_type, int& field_width, int& n_decimal);
		bool get_attribute_text(int n_shape_index, int n_attri_index, char* ptr_text);
		bool get_attribute_int(int n_shape_index, int n_attri_index, int* ptr_int);
		bool get_attribute_double(int n_shape_index, int n_attri_index, double* ptr_double);

		bool set_attribute_text(int n_shape_index, int n_attri_index, char* ptr_text);
		bool set_attribute_int(int n_shape_index, int n_attri_index, int ptr_int);
		bool set_attribute_double(int n_shape_index, int n_attri_index, double ptr_double);
		bool set_attribute(int n_shape_index, int n_attri_index, void* ptr_value);

		// write about
		bool create_dbf(LPCTSTR str_path);
		bool add_dbf_field(const char* field_name, DBFFieldType field_type, int field_width, int n_decimal);

		bool add_dbf_fields(dbf_field_info_array& v_dbf_field_info_array);

		bool write_attribute(int shape_index, int field_index, const char* field_data);

	private:
		string m_str_file;
		DBFHandle m_h_dbf;
		int m_n_attri_num;
		int m_n_record_num;		// shape objects' number

	};
}

#endif // _US_DBF_FILE_H_ 