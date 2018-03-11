///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_hardware_mesh.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : hardware_mesh class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "Stdafx.h"
#include "us_hardware_mesh.h"

namespace uniscope_globe
{
	hardware_mesh::hardware_mesh( void )
	{
		m_visiable = true;

		m_is_texture_ready = false;

		m_use_material  = false;

	}

	hardware_mesh::hardware_mesh( raw_buffer* buffer )
	{
		m_visiable = true;
		m_raw_buffer = buffer;

		m_use_material  = false;
		m_is_texture_ready = false;
	}

	hardware_mesh::~hardware_mesh( void )
	{		
	}

	long hardware_mesh::create( render_device* device )
	{
		return 0;
	}

	//void hardware_mesh::reset( render_device* device )
	//{
	//}

	bool hardware_mesh::destroy( void )
	{
		return false;
	}

	hardware_mesh* hardware_mesh::clone( void )
	{
		return NULL;
	}
	
	void hardware_mesh::draw( render_argument* args )
	{

	}

	void hardware_mesh::draw_subset( render_argument* args, int v_material_index )
	{
		
	}

	axis_aligned_box<double>& hardware_mesh::get_bound_box( void )
	{
		return m_bound_box;
	}

	bool hardware_mesh::intersect( const ray<double>& in_ray, intersect_result& out_result )
	{
		return true;
	}

}