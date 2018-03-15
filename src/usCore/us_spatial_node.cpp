///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_spatial_node.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : spatial_node class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_spatial_node.h"

namespace uniscope_globe
{
	spatial_node::spatial_node( void )
	{
	}

	spatial_node::~spatial_node( void )
	{
		//delete_all_object();
	}

	// Adds an instance of a scene object to this node.
	bool spatial_node::attach_object(spatial_object* object)
	{
		spatial_object_map::iterator itr = m_spatial_object_map.find( object->get_guid() );
		if ( itr != m_spatial_object_map.end() )
		{
			// ÒÑ´æÔÚ
			return false;
		}

		if ( object->is_attached() )
		{
			// already attached to a node
			object->dissolve_attachment();
		}

		m_spatial_object_map.insert( make_pair( object->get_guid(), object ) );

		object->notify_attached( this );
			
		return true;
	}

	// Reports the number of objects attached to this node and all its descendants.
	unsigned short spatial_node::get_attached_object_number(void) const
	{
		return (ushort)m_spatial_object_map.size();
	}

	// Retrieves a pointer to an attached object of this node
	spatial_object* spatial_node::get_attached_object( cpstr name )
	{
		spatial_object_map::iterator itr = m_spatial_object_map.find( name );
		if ( itr != m_spatial_object_map.end() )
		{
			return itr->second;
		}

		return NULL;
	}

	// Retrieves a pointer to an attached object of this node
	spatial_object* spatial_node::get_attached_object(long index)
	{
		// Look up
		int n_size = (int)m_spatial_object_map.size();
		if( index >= n_size ) return NULL;

		spatial_object_map::iterator itr = m_spatial_object_map.begin();
		for( int ni = 0; ni < index; ni++ )
		{
			itr++;
		}

		return itr->second;
	}

	// Detaches an object by pointer.
	bool spatial_node::detach_object( spatial_object* obj )
	{
		spatial_object_map::iterator itr = m_spatial_object_map.find( obj->get_guid() );
		if( itr != m_spatial_object_map.end() )
		{
			m_spatial_object_map.erase( itr );		

			obj->notify_attached(NULL);

			return true;
		}

		return false;
	}

	// Detaches the named object from this node and returns a pointer to it.
	spatial_object* spatial_node::detach_object(cpstr name)
	{
		spatial_object_map::iterator itr = m_spatial_object_map.find(name);
		if (itr != m_spatial_object_map.end())
		{
			spatial_object* ret = itr->second;
			m_spatial_object_map.erase(itr);

			ret->notify_attached(NULL);		

			return ret;
		}

		return NULL;
	}

	// Detaches all objects attached to this node.
	void spatial_node::detach_all_object(void)
	{
		spatial_object_map::iterator itr;
		for ( itr = m_spatial_object_map.begin(); itr != m_spatial_object_map.end(); itr++ )
		{
			itr->second->notify_attached( NULL );
		}
		m_spatial_object_map.clear();
	}

	//void spatial_node::delete_all_object( void )
	//{
	//	spatial_object_map::iterator itr;
	//	for ( itr = m_spatial_object_map.begin(); itr != m_spatial_object_map.end(); itr++ )
	//	{			
	//		itr->second->notify_attached( NULL );
	//		//AUTO_DESTROY_MEMORY( itr->second );
	//		memory_ptr_destroyer* v_destroyer = tls_singleton_memory_ptr_destroyer::ptr();
	//		if( v_destroyer != NULL )
	//			v_destroyer->push( itr->second );
	//		else
	//			AUTO_DELETE( itr->second );
	//	}
	//	m_spatial_object_map.clear();
	//}	

	axis_aligned_box<double>& spatial_node::get_node_aabb()
	{
		return m_node_aabb;
	}

	void spatial_node::set_node_aabb( axis_aligned_box<double>& box )
	{
		m_node_aabb = box;
	}

}