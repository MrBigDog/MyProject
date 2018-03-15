///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_usx_skin_info.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : usx_skin_info class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_usx_skin_info.h"

namespace uniscope_globe
{
	usx_weights_info::usx_weights_info( usx_weights_info* info )
	{
		m_frame_name = info->m_frame_name;

		m_offset_matrix = info->m_offset_matrix;

		m_indices.resize( info->m_indices.size() );
		std::copy( info->m_indices.begin(), info->m_indices.end(), m_indices.begin() );

		m_weights.resize( info->m_weights.size() );
		std::copy( info->m_weights.begin(), info->m_weights.end(), m_weights.begin() );
	}

	//////////////////////////////////////////////////////////////////////////
	usx_skin_info::usx_skin_info( void )
	{
		m_weights_per_vertex = 0;

		m_weights_per_face = 0;

		m_num_of_bones = 0;
	}

	usx_skin_info::~usx_skin_info( void )
	{	
		for ( int i = 0; i < (int)m_weights_array.size(); i ++ )
		{ 
			AUTO_DELETE( m_weights_array[i] );
		}

		m_weights_array.clear();
	}

	usx_skin_info::usx_skin_info( usx_skin_info* in_info )
	{
		usx_skin_info* new_info = new usx_skin_info();

		this->m_weights_per_vertex = in_info->m_weights_per_vertex;
		this->m_weights_per_face   = in_info->m_weights_per_face;

		this->m_weights_array.resize( in_info->m_weights_array.size() );

		for ( int i = 0; i < (int)m_weights_array.size(); i ++ )
		{ 
			this->m_weights_array[i] = in_info->m_weights_array[i];
 		}	
	}
	
}