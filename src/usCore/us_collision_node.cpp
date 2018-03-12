///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_clipper_node.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : clipper_node class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_collision_node.h"

namespace uniscope_globe
{
	collision_node::collision_node( void )
	{
		m_current_array = new intersect_object_array;

		m_back_array = new intersect_object_array;
	}

	collision_node::~collision_node( void )
	{
		AUTO_DELETE( m_current_array );

		AUTO_DELETE( m_back_array );
	}

	void collision_node::push( intersect_base* in_obj )
	{
		m_back_array->push_back( in_obj );
	}

	void collision_node::flush( void )
	{
		// flip
		{
			US_LOCK_AUTO_MUTEX

			intersect_object_array* temp_array = m_current_array;

			m_current_array = m_back_array;

			m_back_array = temp_array;

		}

		m_back_array->clear();		
	}

	bool collision_node::collision( ray<double>& in_ray, intersect_result& result )
	{
		US_LOCK_AUTO_MUTEX

		bool intersected = false;
		for ( int i = 0; i < m_current_array->size(); i++ )
		{
			intersect_base* v_obj = (*m_current_array)[i];

			if( v_obj && v_obj->intersect( in_ray, result ) )
			{
				intersected = true;
			}
		}

		return intersected;
	}
}