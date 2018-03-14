//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_node_base.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : node_base class
//	Reference : 
//
//////////////////////////////////////////////////////////////////////////
//#include "StdAfx.h"
#include "us_node_base.h"

namespace uniscope_globe
{
	node_base::node_base(void)
	{
		m_parent_node = NULL;

		m_level = 0;
	}

	node_base::~node_base(void)
	{
		// remove from parent node
		if( m_parent_node )
		{
			m_parent_node->remove_child( this );
		}

		// remove all children and their descendants
		remove_all_children();
	}

	// Set this node's parent.
	void node_base::set_parent( node_base* parent_node )
	{
		m_parent_node = parent_node;
	}

	// Gets this node's parent (NULL if this is the root).
	node_base* node_base::get_parent( void )
	{
		return m_parent_node;
	}

	void node_base::notify_level( ulong parent_level )
	{
		m_level = parent_level + 1;

		for ( int i = 0; i < (int)m_child_node_array.size(); i++ )
		{
			node_base* v_node = m_child_node_array[i];
			if ( v_node )
			{
				v_node->notify_level( m_level );
			}
		}
	}

	ulong node_base::get_level( void )
	{
		return m_level;
	}

	// Adds a (precreated) child scene node to this node. If it is attached to another node,it must be detached first.
	void node_base::add_child(node_base* child_node)
	{
		child_node->notify_level( m_level );

		child_node->set_parent(this);

		m_child_node_array.push_back( child_node );		
	}

	node_base* node_base::remove_at( ulong index )
	{
		if ( index < m_child_node_array.size() )
		{
			node_base* v_node = m_child_node_array[index];

			// detach
			v_node->set_parent(NULL);
			v_node->notify_level(-1);

			// remove
			child_node_array::iterator itr = find( m_child_node_array.begin(), m_child_node_array.end(), v_node );
			m_child_node_array.erase( itr );

			return v_node;
		}
		
		return NULL;
	}

	void node_base::remove_child(node_base* child_node)
	{
		// detach
		child_node->set_parent(NULL);
		child_node->notify_level( -1 );

		// remove
		child_node_array::iterator itr = find( m_child_node_array.begin(), m_child_node_array.end(), child_node );
		if ( itr != m_child_node_array.end() )
		{			
			m_child_node_array.erase( itr );
		}
	}

	// Drops all the children from this node.
	void node_base::remove_all_children(void)
	{
		child_node_array::iterator itr;
		for (itr = m_child_node_array.begin(); itr != m_child_node_array.end(); ++itr)
		{
			node_base* v_node = (*itr);
			if (v_node)
			{
				v_node->set_parent(NULL);
				v_node->notify_level( -1 );
				AUTO_DELETE( v_node );
			}
		}

		m_child_node_array.clear();
	}

	// Gets the size of the children to this node
	int node_base::get_children_number(void)
	{
		return (int)m_child_node_array.size();
	}

	node_base* node_base::get_child( ulong index )
	{
		if ( index < m_child_node_array.size() )
		{
			return m_child_node_array[index];
		}

		return NULL;
	}
}