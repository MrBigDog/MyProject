///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_common_manager_group.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : common_manager_group class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "Stdafx.h"
#include "us_common_manager_group.h"

namespace uniscope_globe
{
	DWORD singleton_common_manager_group::m_tls_index = TlsAlloc();

	common_manager_group::common_manager_group( document_base* v_document )
	{
		m_mesh_creator = new mesh_creator;

		m_texture_creator = new texture_creator;

		m_hardware_creator = new hardware_creator;

		m_document = v_document;
	}

	common_manager_group::~common_manager_group( void )
	{
		clear();
	}

	object_base* common_manager_group::get_object( const LPCTSTR str_key )
	{
		return m_object_map[str_key];
	}

	void common_manager_group::register_object( const LPCTSTR str_key, object_base* object )
	{
		m_object_map[str_key] = object;
	}

	void common_manager_group::clear( void )
	{
		manager_map::iterator itr = m_mesh_mngr_map.begin();
		for ( ; itr != m_mesh_mngr_map.end(); itr ++ )
		{
			AUTO_DELETE( itr->second );
		}
		m_mesh_mngr_map.clear();

		itr = m_texture_mngr_map.begin();
		for ( ; itr != m_texture_mngr_map.end(); itr ++ )
		{
			AUTO_DELETE( itr->second );
		}
		m_texture_mngr_map.clear();

		index_manager_map::iterator iitr = m_index_mesh_mngr_map.begin();
		for ( ; iitr != m_index_mesh_mngr_map.end(); iitr ++ )
		{
			AUTO_DELETE( iitr->second );
		}
		m_index_mesh_mngr_map.clear();

		iitr = m_index_texture_mngr_map.begin();
		for ( ; iitr != m_index_texture_mngr_map.end(); iitr ++ )
		{
			AUTO_DELETE( iitr->second );
		}
		m_index_texture_mngr_map.clear();


		object_map::iterator object_itr = m_object_map.begin();
		for ( ; object_itr != m_object_map.end(); object_itr ++ )
		{
			AUTO_DELETE( object_itr->second );
		}

		m_object_map.clear();

		AUTO_DELETE( m_mesh_creator );
		AUTO_DELETE( m_texture_creator );
		AUTO_DELETE( m_hardware_creator );
	}

	resource_manager<ustring>* common_manager_group::get_texture_manager( const LPCTSTR str_key )
	{
		resource_manager<ustring>* ret_mngr = 0;

		manager_map::iterator itr = m_texture_mngr_map.find( str_key );
		if ( itr != m_texture_mngr_map.end() )
		{
			ret_mngr = (resource_manager<ustring>*)itr->second;
		}
		else
		{
			ret_mngr = new resource_manager<ustring>;
			m_texture_mngr_map[str_key] = ret_mngr;
		}

		return ret_mngr;
	}

	resource_manager<ustring>* common_manager_group::get_mesh_manager( const LPCTSTR str_key )
	{
		resource_manager<ustring>* ret_mngr = 0;

		manager_map::iterator itr = m_mesh_mngr_map.find( str_key );
		if ( itr != m_mesh_mngr_map.end() )
		{
			ret_mngr = (resource_manager<ustring>*)itr->second;
		}
		else
		{
			ret_mngr = new resource_manager<ustring>;
			m_mesh_mngr_map[str_key] = ret_mngr;
		}

		return ret_mngr;
	}

	resource_manager<interior_index>* common_manager_group::get_index_texture_manager(  const LPCTSTR str_key )
	{
		resource_manager<interior_index>* ret_mngr = 0;

		index_manager_map::iterator itr = m_index_texture_mngr_map.find( str_key );
		if ( itr != m_index_texture_mngr_map.end() )
		{
			ret_mngr = (resource_manager<interior_index>*)itr->second;
		}
		else
		{
			ret_mngr = new resource_manager<interior_index>;
			m_index_texture_mngr_map[str_key] = ret_mngr;
		}

		return ret_mngr;
	}

	resource_manager<interior_index>* common_manager_group::get_index_mesh_manager(  const LPCTSTR str_key )
	{
		resource_manager<interior_index>* ret_mngr = 0;

		index_manager_map::iterator itr = m_index_mesh_mngr_map.find( str_key );
		if ( itr != m_index_mesh_mngr_map.end() )
		{
			ret_mngr = (resource_manager<interior_index>*)itr->second;
		}
		else
		{
			ret_mngr = new resource_manager<interior_index>;
			m_index_mesh_mngr_map[str_key] = ret_mngr;
		}

		return ret_mngr;
	}

}