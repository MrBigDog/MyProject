///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_cartesian_transform.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : cartesian_transform class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////

#include "us_cartesian_transform.h"

namespace uniscope_globe
{
	cartesian_transform::cartesian_transform( void )
	{
		m_is_dirty = true;

		m_is_identity = false;

		m_scale = vector3<float>( 1.0f, 1.0f, 1.0f );

		m_pivot = vector3<float>( 0.0f, 0.0f, 0.0f );

		m_position = vector3<float>( 0.0f, 0.0f, 0.0f );

		m_rotation = quaternion<float>( 0.0f, 0.0f, 0.0f, 1.0f );

		m_transform_matrix_d = matrix4<double>::s_identity;

		m_transform_matrix_f = matrix4<float>::s_identity;
	}

	cartesian_transform::cartesian_transform( const cartesian_transform& new_transform )
	{
		m_is_dirty = new_transform.m_is_dirty;

		m_is_identity = new_transform.m_is_identity;

		m_scale = new_transform.m_scale;

		m_pivot = new_transform.m_pivot;

		m_position = new_transform.m_position;

		m_rotation = new_transform.m_rotation;

		m_transform_matrix_d = new_transform.m_transform_matrix_d;

		m_transform_matrix_f = new_transform.m_transform_matrix_f;

	}
	cartesian_transform::~cartesian_transform( void )
	{

	}

	void cartesian_transform::copy_from(cartesian_transform* v_transform)
	{
		m_is_dirty = v_transform->m_is_dirty;

		m_is_identity = v_transform->m_is_identity;

		m_scale = v_transform->m_scale;

		m_pivot = v_transform->m_pivot;

		m_position = v_transform->m_position;

		m_rotation = v_transform->m_rotation;

		m_transform_matrix_d = v_transform->m_transform_matrix_d;

		m_transform_matrix_f = v_transform->m_transform_matrix_f;
	}

	matrix_4f cartesian_transform::get_matrix_f( void )
	{
		update_matrix();
		return m_transform_matrix_f;
	}

	matrix_4d cartesian_transform::get_matrix_d( void )
	{
		update_matrix();
		return m_transform_matrix_d;
	}

	void cartesian_transform::set( vector3<float>& in_pos, vector3<float> in_pivot, vector3<float>& in_scale, quaternion<float>& in_rotation )
	{
		m_position = in_pos;
		m_pivot = in_pivot;
		m_scale = in_scale;
		m_rotation = in_rotation;
		m_is_dirty = true;
		update_matrix();
	}

	void cartesian_transform::update_matrix( void )
	{
		if( !m_is_dirty ) return;

		matrix4<float> scale_matrix = matrix4<float>::scaling( m_scale.x, m_scale.y, m_scale.z );
		matrix4<float> positive_pivot_matrix = matrix4<float>::translation( m_pivot.x, m_pivot.y, m_pivot.z );
		matrix4<float> negative_pivot_matrix = matrix4<float>::translation( -m_pivot.x, -m_pivot.y, -m_pivot.z );
		matrix4<float> translation_matrix = matrix4<float>::translation( m_position.x, m_position.y, m_position.z );
		matrix4<float> rotation_matrix = matrix4<float>::from_quaternion( m_rotation );
		m_transform_matrix_f = negative_pivot_matrix * scale_matrix * rotation_matrix * positive_pivot_matrix * translation_matrix;		
		m_transform_matrix_d = matrix4<double>(m_transform_matrix_f.m);

		m_is_dirty = false;
		m_is_identity = ( m_transform_matrix_f.is_identity()? true : false );
	}


	
}