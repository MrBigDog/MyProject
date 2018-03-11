///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_shp_file.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : shp_file class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_SHP_FILE_H_
#define _US_SHP_FILE_H_

#include <usUtil\Export.h>
#include <usUtil\shapefil.h>
#include <usUtil\us_dbf_file.h>
#include <usUtil\us_vector3.h>
#include <usUtil\us_common_file.h>
#include <map>
#include <wtypes.h>

using namespace std;

namespace uniscope_globe
{
	class lon_lat_rect;

	typedef map<ustring, int> field_index_map;

	class shp_file
	{
	public:
		shp_file(void);
		virtual ~shp_file(void);

	public:
		bool init_shape_file(LPCTSTR str_path);

		bool is_shape_polyline();
		bool is_shape_polygon();
		bool is_shape_point();

		int get_record_count();
		int get_attri_count();
		lon_lat_rect get_shape_rect();

		bool get_shape_object(int n_index, lon_lat_rect* obj_rect, vector_3d** vec_list, int* n_vertex);
		bool get_shape_object(int n_index, lon_lat_rect* obj_rect, vector_3d** vec_list, int* n_vertex, int** part_list, int* n_part);

		dbf_file* get_relative_dbf(void);

		int get_field_index(ustring field_name);
		bool get_field_info(int n_attri_index, char* ptr_text, DBFFieldType& field_type, int& field_width, int& n_decimal);

		bool get_attribute_text(int n_shape_index, int n_attri_index, char* ptr_text);
		bool get_attribute_int(int n_shape_index, int n_attri_index, int* ptr_int);
		bool get_attribute_double(int n_shape_index, int n_attri_index, double* ptr_double);

	private:
		ustring m_str_path;
		SHPHandle m_handle;	// handle of shape file
		dbf_file* m_dbf_file;

		int m_n_record_num;		// shape objects' number
		int m_n_shape_type;			// type of shape data
		lon_lat_rect m_rect;	// rect of shape file

		field_index_map m_field_map;


	};
}

#endif // _US_SHP_FILE_H_ 