///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_intersect_result.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : intersect_result class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_INTERSECT_RESULT_H_
#define _US_INTERSECT_RESULT_H_

#include <usCore\Export.h>
#include <usUtil\us_vector3.h>

namespace uniscope_globe
{
	class hardware_mesh;
	class object_base;

	class USCORE_EXPORT intersect_result
	{
	public:
		typedef std::vector<object_base*> object_array;

	public:
		intersect_result(void);

		virtual ~intersect_result(void);

	public:
		double			m_distance;

		ulong			m_face_index;

		float			m_tu;

		float			m_tv;

		ulong			m_hit_count;

		vector3<double>	m_position;

		vector3<double>	m_face_direction;

		vector3<double> m_terrain_position;

		object_base*		m_parent_layer;

		ustring			m_guid;

		object_base*    m_object;

		object_array    m_object_array;

		object_base*    m_inner_object;

		hardware_mesh*  m_inner_mesh;

		ulong			m_draw_order;
	};
}

#endif // _US_INTERSECT_RESULT_H_