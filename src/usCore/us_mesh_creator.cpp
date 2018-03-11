///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_mesh_creator.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : mesh_pool class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "Stdafx.h"
#include "us_mesh_creator.h"

namespace uniscope_globe
{
	//mesh_creator* singleton_mesh_creator::s_ins = NULL;

	void mesh_creator::operator += ( hardware_base* hardware )
	{
		US_LOCK_AUTO_MUTEX

		if ( hardware )
		{
			m_create_array.push_back( hardware );
		}
	}

	void mesh_creator::clear( void )
	{
		US_LOCK_AUTO_MUTEX

		m_create_array.clear();
	}

	void mesh_creator::update( render_device* device )
	{
		US_LOCK_AUTO_MUTEX

		int cur_create_num = system_environment::s_create_mesh_num;
		int create_num = (int)m_create_array.size();
		cur_create_num = min( create_num, cur_create_num );
		
		//std::sort( m_create_array.begin(), m_create_array.end(), compare_mesh_rank );

		int cur_create_size = 0;
		for ( int i = 0; (i < cur_create_num) && (cur_create_size < system_environment::s_create_mesh_size) ; i++ )
		{
			hardware_base* hardware = m_create_array[i] ;
			assert( NULL != hardware );
			cur_create_size += hardware->create( device );
		}

		m_create_array.clear();
	}

}