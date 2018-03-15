///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_usx_frame.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : usx_frame class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_usx_frame.h"

namespace uniscope_globe
{
	usx_frame::usx_frame( void )
	{
		m_frame_parent = NULL;

		m_frame_first_child = NULL;

		m_frame_sibling = NULL;

		m_original_mat.identity();

		m_transform_mat.identity();

		m_combined_mat.identity();

		m_mesh = NULL;

		m_animation = NULL;
	}

	usx_frame::~usx_frame( void )
	{
		AUTO_DELETE( m_frame_sibling );

		AUTO_DELETE( m_frame_first_child );
	}

	usx_frame* usx_frame::clone( void )
	{
		return NULL;
	}

	usx_frame* usx_frame::find_frame( cpstr frame_name )
	{
		if( m_name == frame_name )
			return this;

		if( m_frame_sibling != NULL )
		{
			usx_frame* frame = m_frame_sibling->find_frame( frame_name );
			if( frame != NULL )
				return frame;
		}
		
		if( m_frame_first_child != NULL )
		{
			usx_frame* frame = m_frame_first_child->find_frame( frame_name );
			if( frame != NULL )
				return frame;
		}		

		return NULL;
	}

	void usx_frame::reset( void )
	{
		m_transform_mat = m_original_mat;

		if( m_frame_sibling != NULL )
			m_frame_sibling->reset();

		if( m_frame_first_child != NULL )
			m_frame_first_child->reset();
	}

	void usx_frame::update_hierarchy( matrix4<double> in_mat )
	{
		m_combined_mat = m_transform_mat * in_mat;

		if( m_frame_sibling != NULL )
			m_frame_sibling->update_hierarchy( in_mat );

		if( m_frame_first_child != NULL )
			m_frame_first_child->update_hierarchy( m_combined_mat );
	}

}