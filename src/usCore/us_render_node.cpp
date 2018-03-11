///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_render_node.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : render_node class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_render_node.h"

namespace uniscope_globe
{
	render_node::render_node( void )
	{
		m_priority = 0;
	}

	render_node::~render_node( void )
	{

	}

	void render_node::draw( render_argument* args )
	{
		US_LOCK_MUTEX( base_mutex )

		for ( int i = 0; i < (int)m_child_node_array.size(); i++ )
		{
			m_child_node_array[i]->draw( args );
		}
	}


	void render_node::draw_alpha( render_argument* args )
	{
		US_LOCK_MUTEX( base_mutex )

		for ( int i = 0; i < (int)m_child_node_array.size(); i++ )
		{
			m_child_node_array[i]->draw_alpha( args );
		}
	}

	void render_node::draw( render_argument* args, void* in_mesh )
	{

	}

	void render_node::set_texture( render_argument* args, void* in_texture, uint stage_index )
	{
		
	}

	void render_node::occlusion( render_argument* args )
	{
		if ( m_child_node_array.size() == 0 )
			return;

		US_LOCK_MUTEX( base_mutex )

		for ( int i = 0; i < (int)m_child_node_array.size(); i++ )
		{
			m_child_node_array[i]->occlusion( args );
		}
	}

	void render_node::flush( void )
	{
		if ( m_child_node_array.size() == 0 )
			return;

		US_LOCK_MUTEX( base_mutex )

		for ( int i = 0; i < (int)m_child_node_array.size(); i++ )
		{
			m_child_node_array[i]->flush();
		}
	}

	void render_node::add_child( render_node* in_render )
	{
		US_LOCK_MUTEX( base_mutex )

		m_child_node_array.push_back( in_render );

	}

	render_node* render_node::get_child( int in_index )
	{
		US_LOCK_MUTEX( base_mutex )

		if ( m_child_node_array.size() > in_index )
		{
			return m_child_node_array[in_index];
		}

		return NULL;
	}

	int render_node::get_child_count( void )
	{
		US_LOCK_MUTEX( base_mutex )

		return m_child_node_array.size();
	}

	void render_node::remove_child( render_node* in_render )
	{
		US_LOCK_MUTEX( base_mutex )

		render_node_array::iterator itr = m_child_node_array.begin();
		for ( ; itr != m_child_node_array.end() ; itr++ )
		{
			if ( in_render == (*itr) )
			{
				m_child_node_array.erase( itr );
				break;
			}
		}
	}

	void render_node::remove_all( void )
	{
		US_LOCK_MUTEX( base_mutex )

		m_child_node_array.clear();
	}

	bool render_node_priority_ascending( render_node* bs1, render_node* bs2 )
	{
		return bs1->get_priority() < bs2->get_priority();
	}

	bool render_node_priority_descending( render_node* bs1, render_node* bs2 )
	{
		return bs1->get_priority() > bs2->get_priority();
	}

	void render_node::sort( bool is_ascending )
	{
		US_LOCK_MUTEX( base_mutex )

		if(is_ascending)
		{
			std::sort(m_child_node_array.begin(), m_child_node_array.end(), render_node_priority_ascending);
		}
		else
		{
			std::sort(m_child_node_array.begin(), m_child_node_array.end(), render_node_priority_descending);
		}

		for ( int i = 0; i < (int)m_child_node_array.size(); i++ )
		{
			m_child_node_array[i]->sort(is_ascending);
		}
	}



	
}