///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_shp_file.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : shp_file class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////

#include "us_shp_file.h"
#include <usUtil/us_lon_lat_rect.h>
#include <usUtil/us_string_ext.h>
#include <usUtil/us_file_directory.h>

namespace uniscope_globe
{
	shp_file::shp_file(void)
	{
		m_dbf_file = new dbf_file();
		m_handle = NULL;
	}
	shp_file::~shp_file(void)
	{
		AUTO_DELETE(m_dbf_file);
		SHPClose(m_handle);
		m_handle = NULL;
	}

	bool shp_file::init_shape_file(LPCTSTR str_path)
	{
		// init shape path
		m_str_path = str_path;

		// open shape file
		m_handle = SHPOpen(string_ext::from_wstring(m_str_path).data(), "rb");
		if (m_handle == NULL)
		{
			return false;
		}

		ustring str_dbf_file = file_directory::get_path_without_ext(str_path) + L".dbf";
		if (!m_dbf_file->init_dbf_file(str_dbf_file.c_str()))
		{
			SHPClose(m_handle);
			m_handle = NULL;
			return false;
		}

		// get shape count, objects number and rect
		double v_max_bound[4], v_min_bound[4];
		SHPGetInfo(m_handle, &m_n_record_num, &m_n_shape_type, &v_min_bound[0], &v_max_bound[0]);

		m_rect.m_east = v_max_bound[0];
		m_rect.m_north = v_max_bound[1];
		m_rect.m_west = v_min_bound[0];
		m_rect.m_south = v_min_bound[1];

		// init attribute map
		m_field_map.clear();
		int attri_cnt = m_dbf_file->get_attri_count();
		char* ptr_text = new char[12];
		DBFFieldType field_type;
		int field_width;
		int n_decimal;
		for (int i = 0; i < attri_cnt; i++)
		{
			if (get_field_info(i, ptr_text, field_type, field_width, n_decimal))
			{
				m_field_map.insert(make_pair(string_ext::to_wstring(ptr_text), i));
			}
		}
		return true;
	}

	dbf_file* shp_file::get_relative_dbf(void)
	{
		return m_dbf_file;
	}

	bool shp_file::is_shape_point()
	{
		if (m_n_shape_type == SHPT_POINT || m_n_shape_type == SHPT_POINTZ || m_n_shape_type == SHPT_POINTM || m_n_shape_type == SHPT_MULTIPOINT)
			return true;
		else
			return false;
	}

	bool shp_file::is_shape_polygon()
	{
		if (m_n_shape_type == SHPT_POLYGON || m_n_shape_type == SHPT_POLYGONZ || m_n_shape_type == SHPT_POLYGONM)
			return true;
		else
			return false;
	}

	bool shp_file::is_shape_polyline()
	{
		if (m_n_shape_type == SHPT_ARC || m_n_shape_type == SHPT_ARCZ || m_n_shape_type == SHPT_ARCM)
			return true;
		else
			return false;
	}

	int shp_file::get_record_count()
	{
		return m_n_record_num;
	}

	int shp_file::get_attri_count()
	{
		return m_dbf_file->get_attri_count();
	}

	lon_lat_rect shp_file::get_shape_rect()
	{
		return m_rect;
	}

	bool shp_file::get_shape_object(int n_index, lon_lat_rect* obj_rect, vector_3d** vec_list, int* n_vertex, int** part_list, int* n_part)
	{
		// get shape object
		SHPObject *v_shp_object;
		v_shp_object = SHPReadObject(m_handle, n_index);

		if (v_shp_object == NULL)
		{
			return false;
		}
		if (v_shp_object->nSHPType == SHPT_NULL || v_shp_object->nVertices == 0)
		{
			SHPDestroyObject(v_shp_object);
			return false;
		}

		// get entity rect
		obj_rect->m_west = v_shp_object->dfXMin;
		obj_rect->m_east = v_shp_object->dfXMax;
		obj_rect->m_south = v_shp_object->dfYMin;
		obj_rect->m_north = v_shp_object->dfYMax;

		// get object vertex
		*n_part = v_shp_object->nParts;
		*part_list = new int[v_shp_object->nParts];
		for (int i = 0; i < v_shp_object->nParts; i++)
		{
			(*part_list)[i] = v_shp_object->panPartStart[i];
		}

		*n_vertex = v_shp_object->nVertices;
		*vec_list = new vector3<double>[v_shp_object->nVertices];
		vector3<double> v_vertex;
		for (int i = 0; i < v_shp_object->nVertices; i++)
		{
			v_vertex.x = v_shp_object->padfX[i];
			v_vertex.y = v_shp_object->padfY[i];
			if (m_n_shape_type == SHPT_POINTZ || m_n_shape_type == SHPT_POLYGONZ || m_n_shape_type == SHPT_ARCZ)
			{
				v_vertex.z = v_shp_object->padfZ[i];
			}
			(*vec_list)[i] = v_vertex;
		}

		if (v_shp_object->nVertices == 0)
		{
			SHPDestroyObject(v_shp_object);
			return false;
		}

		SHPDestroyObject(v_shp_object);
		return true;
	}

	bool shp_file::get_shape_object(int n_index, lon_lat_rect* obj_rect, vector_3d** vec_list, int* n_vertex)
	{
		// get shape object
		SHPObject *v_shp_object;
		v_shp_object = SHPReadObject(m_handle, n_index);

		if (v_shp_object == NULL)
		{
			return false;
		}
		if (v_shp_object->nSHPType == SHPT_NULL || v_shp_object->nVertices == 0)
		{
			SHPDestroyObject(v_shp_object);
			return false;
		}

		// get entity rect
		obj_rect->m_west = v_shp_object->dfXMin;
		obj_rect->m_east = v_shp_object->dfXMax;
		obj_rect->m_south = v_shp_object->dfYMin;
		obj_rect->m_north = v_shp_object->dfYMax;

		// get object vertex
		int v_n_part_num = v_shp_object->nParts;
		int v_n_outring_ver_num = 0;
		if (v_n_part_num <= 1)
		{
			v_n_outring_ver_num = v_shp_object->nVertices;
		}
		else
		{
			v_n_outring_ver_num = v_shp_object->panPartStart[1];
		}
		*n_vertex = v_n_outring_ver_num;
		*vec_list = new vector3<double>[v_n_outring_ver_num];
		vector3<double> v_vertex;
		for (int i = 0; i < v_n_outring_ver_num; i++)
		{
			v_vertex.x = v_shp_object->padfX[i];
			v_vertex.y = v_shp_object->padfY[i];
			if (m_n_shape_type == SHPT_POINTZ || m_n_shape_type == SHPT_POLYGONZ || m_n_shape_type == SHPT_ARCZ)
			{
				v_vertex.z = v_shp_object->padfZ[i];
			}
			(*vec_list)[i] = v_vertex;
		}

		if (v_shp_object->nVertices == 0)
		{
			SHPDestroyObject(v_shp_object);
			return false;
		}

		SHPDestroyObject(v_shp_object);
		return true;
	}

	bool shp_file::get_field_info(int n_attri_index, char* ptr_text, DBFFieldType& field_type, int& field_width, int& n_decimal)
	{
		return m_dbf_file->get_field_info(n_attri_index, ptr_text, field_type, field_width, n_decimal);
	}

	int shp_file::get_field_index(ustring field_name)
	{
		field_index_map::iterator itr = m_field_map.find(field_name);
		if (itr == m_field_map.end())
		{
			return -1;
		}
		return itr->second;
	}

	bool shp_file::get_attribute_text(int n_shape_index, int n_attri_index, char* ptr_text)
	{
		return m_dbf_file->get_attribute_text(n_shape_index, n_attri_index, ptr_text);
	}

	bool shp_file::get_attribute_int(int n_shape_index, int n_attri_index, int* ptr_int)
	{
		return m_dbf_file->get_attribute_int(n_shape_index, n_attri_index, ptr_int);
	}

	bool shp_file::get_attribute_double(int n_shape_index, int n_attri_index, double* ptr_double)
	{
		return m_dbf_file->get_attribute_double(n_shape_index, n_attri_index, ptr_double);
	}
}


