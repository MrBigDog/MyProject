///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_octree_node.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : octree_node class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_octree_node.h"

namespace uniscope_globe
{
	octree_node::octree_node( void )
	{
		m_child_node_array.resize(8);
		for ( int i = 0; i < 8; i++ )
		{
			m_child_node_array[i] = NULL;
		}

		m_batch_mode = false;
	}

	octree_node::~octree_node( void )
	{
		//for ( int i = 0; i < 8; i++ )
		//{
		//	AUTO_DELETE( m_child_node_array[i] );
		//}
		//m_child_node_array.clear();
	}

	//bool octree_node::add_to_map( spatial_object* object )
	//{
	//	spatial_object_map::iterator itr = m_spatial_object_map.find( object->get_guid() );
	//	if ( itr != m_spatial_object_map.end() )
	//	{
	//		// 已存在
	//		return false;
	//	}

	//	m_spatial_object_map.insert( make_pair( object->get_guid(), object ) );	

	//	return true;
	//}

	bool octree_node::add_to_array( spatial_object* object )
	{
		m_spatial_object_array.push_back( object );

		return true;
	}

	bool octree_node::attach_object( spatial_object* object )
	{
		cartesian_object* cart_obj = dynamic_cast<cartesian_object*>( object );
		if ( cart_obj == NULL ) return false;

		spatial_object* v_spatial_obj = dynamic_cast<spatial_object*>( cart_obj );
		if ( v_spatial_obj == NULL ) return false;

		axis_aligned_box<double> bound_box = object->get_aabb();
		bound_box.transform(cart_obj->get_matrix_d());

		if ( cart_obj->get_object_flags() == US_OCTREE_NODE_FLAG_ROOT )
		{
			return add_to_array( object );
		}

		if ( !m_node_aabb.is_valid() || !bound_box.is_valid() )
		{
			return add_to_array( object );
		}

		if ( bound_box.is_in( m_node_aabb ) )
		{	
			return octree_node_attach_object( object );				
		}
		else if( m_parent_node )
		{
			return (dynamic_cast<octree_node*>(m_parent_node))->add_to_array( object );
		}
		else
		{
			return add_to_array( object );
		}		
		
		return false; 
	}

	bool octree_node::octree_node_attach_object( spatial_object* object )
	{
		axis_aligned_box<double> obj_bound_box = object->get_aabb();

		if ( m_level < 7 && is_twice_lager( obj_bound_box ) )
		{			
			int quadrant = find_appropriate_quadrant( obj_bound_box );
			

			if ( m_child_node_array[quadrant] == NULL )
			{				
				octree_node* v_node = create_octree_node();
				axis_aligned_box<double> child_bound_box = get_child_axis_aligned_box( quadrant );
				v_node->set_node_aabb( child_bound_box );
				v_node->m_level = m_level + 1;
				v_node->m_parent_node = this;
				m_child_node_array[quadrant] = v_node;				
			}			

			(dynamic_cast<octree_node*>( m_child_node_array[quadrant] ))->attach_object( object );
		}
		else
		{			
			return add_to_array( object );
		}

		return false;
	}

	octree_node* octree_node::create_octree_node( void )
	{
		return new octree_node;
	}

	int octree_node::find_appropriate_quadrant( axis_aligned_box<double>& obj_bound_box )
	{
		vector3<double> node_max_vec = m_node_aabb.get_max();
		vector3<double> obj_min_vec = obj_bound_box.get_min();

		vector3<double> node_center_vec = m_node_aabb.center();
		vector3<double> obj_center_vec  = obj_bound_box.center();

		int x,y,z;
		if ( obj_center_vec.x > node_center_vec.x )
			x = 0;
		else
			x = 1;

		if ( obj_center_vec.y > node_center_vec.y )
			y = 0;
		else
			y = 1;

		if ( obj_center_vec.z > node_center_vec.z )
			z = 0;
		else
			z = 1;

		return x + y * 2 + 4 * z;

	}

//////////////////////////////////////////////////////////////////////////
/*
   5-----4
  /|    /|
 / |   / |
1-----2  |
|  6--|--7
| /   | /
|/    |/
0-----3
*/
//////////////////////////////////////////////////////////////////////////
	axis_aligned_box<double> octree_node::get_child_axis_aligned_box( int index )
	{
		axis_aligned_box<double> box;

		const vector3<double>* corners = m_node_aabb.get_corners();

		vector3<double> center_vec = m_node_aabb.center();

		switch( index)
		{
		case 0:
			box.set_extent( center_vec , corners[4] );
			break;
		case 1:
			box.set_extent( ( corners[0] + corners[5] ) * 0.5, ( corners[4] + corners[5] ) * 0.5 );
			break;
		case 2:
			box.set_extent( ( corners[0] + corners[7] ) * 0.5, ( corners[4] + corners[7] ) * 0.5 );
			break;
		case 3:
			box.set_extent( ( corners[0] + corners[6] ) * 0.5, ( corners[4] + corners[6] ) * 0.5 );
			break;
		case 4:
			box.set_extent( ( corners[0] + corners[2] ) * 0.5, ( corners[2] + corners[4] ) * 0.5 );
			break;
		case 5:
			box.set_extent( ( corners[0] + corners[1] ) * 0.5, ( corners[1] + corners[4] ) * 0.5 );
			break;
		case 6:
			box.set_extent( ( corners[0] + corners[3] ) * 0.5, ( corners[3] + corners[4] ) * 0.5 );
			break;
		case 7:
			box.set_extent( corners[0], center_vec );
			break;
		}

		return box;
	}

	spatial_object* octree_node::get_attached_object( cpstr name )
	{
		for ( int i = 0; i < m_spatial_object_array.size(); i++ )
		{
			spatial_object* v_so = m_spatial_object_array[i];
			if( v_so->get_guid() == name )
			{	
				return v_so;
			}
		}

		for ( int i = 0; i < 8; i++ )
		{
			if( m_child_node_array[i] == NULL ) continue;

			spatial_object* v_so = (dynamic_cast<octree_node*>( m_child_node_array[i] ))->get_attached_object( name );
			if( v_so != NULL ) return v_so;
		}

		return NULL;
	}

	spatial_object* octree_node::get_attached_object( long index )
	{
		assert( NULL );

		return NULL;
	}

	bool octree_node::detach_object( spatial_object* obj )
	{
		spatial_object_array::iterator itr = std::find( m_spatial_object_array.begin(), m_spatial_object_array.end(), obj );
		if( itr != m_spatial_object_array.end() )
		{
			m_spatial_object_array.erase( itr );
			return true;
		}

		for ( int i = 0; i < 8; i++ )
		{
			if( m_child_node_array[i] == NULL ) continue;

			if( (dynamic_cast<octree_node*>( m_child_node_array[i] ))->detach_object( obj ) )
				return true;
		}

		return false;
	}

	spatial_object* octree_node::detach_object( cpstr guid_string )
	{
		for ( int i = 0; i < m_spatial_object_array.size(); i++ )
		{
			spatial_object* v_so = m_spatial_object_array[i];
			if( v_so->get_guid() == guid_string )
			{				
				m_spatial_object_array.erase(m_spatial_object_array.begin() + i);
				return v_so;
			}
		}

		for ( int i = 0; i < 8; i++ )
		{
			if( m_child_node_array[i] == NULL ) continue;

			spatial_object* ret = (dynamic_cast<octree_node*>( m_child_node_array[i] ))->detach_object( guid_string );
			if( ret ) return ret;
		}

		return NULL;
	}

	void octree_node::detach_all_object(void)
	{
		m_spatial_object_array.clear();

		for ( int i = 0; i < 8; i++ )
		{
			if( m_child_node_array[i] == NULL ) continue;

			(dynamic_cast<octree_node*>( m_child_node_array[i] ))->detach_all_object();
		}
	}

	bool octree_node::intersect( const ray<double>& in_ray, intersect_result& in_result )
	{
		if( !m_node_aabb.intersect( in_ray ) )
			return false;

		// 当前结点对象求交
		bool ret = false;		
		for ( int i = 0; i < m_spatial_object_array.size(); i++ )
		{
			spatial_object *sp_obj = m_spatial_object_array[i];

			if ( sp_obj )
			{
				//cartesian_object* cart_obj = dynamic_cast<cartesian_object*>( sp_obj );
				intersect_base* v_intersect_base = dynamic_cast<intersect_base*>( sp_obj );
				//intersect_base* v_intersect_base = (intersect_base*)( sp_obj );
				if( v_intersect_base && v_intersect_base->intersect( in_ray,in_result ) )
				{
					in_result.m_object = sp_obj;
					ret = true;
				}
			}
		}

		for ( int i = 0; i < 8; i++ )
		{
			if( m_child_node_array[i] == NULL ) continue;

			octree_node* v_node = dynamic_cast<octree_node*>( m_child_node_array[i] );
			if( v_node && v_node->intersect( in_ray, in_result ) )
			{
				ret = true;
			}
		}
		
		return ret;
	}

	bool octree_node::intersect( part_sphere& pt_sphr, intersect_result& out_result )
	{
		// 第一步
		// 通过距离来判定
		if ( m_node_aabb.nearest_distance_to_point( pt_sphr.v_src_point ) > pt_sphr.v_radian )
			return false;

		// 第二步
		// 将裁剪框
		long cull_result = aabb2d_intersect_part_sphere( m_node_aabb, pt_sphr );
		if ( cull_result == -1 )
			return false;

		// 第三步
		// 遍历子对象
		bool ret = false;		
		for ( int i = 0; i < m_spatial_object_array.size(); i++ )
		{
			spatial_object *sp_obj = m_spatial_object_array[i];

			if ( sp_obj )
			{
				intersect_base* v_intersect_base = dynamic_cast<intersect_base*>( sp_obj );
				if ( v_intersect_base && v_intersect_base->intersect( pt_sphr, out_result ) )
				{
					ret = true;
				}
			}
		}

		// 第四步
		// 遍历子节点
		for ( int i = 0; i < 8; i++ )
		{
			if( m_child_node_array[i] == NULL ) continue;

			octree_node* v_node = dynamic_cast<octree_node*>( m_child_node_array[i] );
			if( v_node && v_node->intersect( pt_sphr, out_result ) )
			{
				ret = true;
			}
		}

		return ret;
	}

	//bool octree_node::intersect( const axis_aligned_box<double>& box )
	//{
	//	return false;
	//}

	// 返回true时，所有结点访问过
	bool octree_node::traverse( node_visitor* in_visitor )
	{
		in_visitor->begin_visit();

		bool val = traverse_recursive( in_visitor );

		in_visitor->end_visit();

		return val;
	}

	bool octree_node::traverse_recursive( node_visitor* in_visitor )
	{
		if ( !in_visitor->intersect( m_node_aabb ) )
		{		
			return false;
		}

		for ( int i = 0; i < m_spatial_object_array.size(); i++ )
		{
			in_visitor->visit( m_spatial_object_array[i] );
		}

		for ( int i = 0; i < 8; i++ )
		{
			if( m_child_node_array[i] == NULL ) continue;
			octree_node* v_node = dynamic_cast<octree_node*>( m_child_node_array[i] );
			v_node->traverse_recursive( in_visitor );
		}

		return true;
	}

	//void octree_node::clipping( observer_base* in_observer, frustum<double>* in_frustum )
	//{
	//	if ( !in_frustum->intersect( m_node_aabb ) )
	//	{		
	//		return;
	//	}

	//	spatial_object_map::iterator itr = m_spatial_object_map.begin();
	//	for ( ; itr != m_spatial_object_map.end(); itr ++ )
	//	{
	//		spatial_object* obj = dynamic_cast<spatial_object*>( itr->second );

	//		if( obj )
	//		{
	//			clipping_object( in_observer, in_frustum, obj );
	//		}
	//	}

	//	for ( int i = 0; i < 8; i++ )
	//	{
	//		if( m_child_node_array[i] == NULL ) continue;
	//		octree_node* v_node = dynamic_cast<octree_node*>( m_child_node_array[i] );
	//		if( v_node )
	//			v_node->clipping( in_observer, in_frustum );
	//	}
	//}

	//void octree_node::clipping_object( observer_base* in_observer, frustum<double>* in_frustum, spatial_object* in_object )
	//{

	//}

	//void octree_node::collision_detect( collision_detector* in_detector, collision_node* in_node )
	//{
	//	if ( !in_detector->collision( m_node_aabb ) )
	//	{		
	//		return;
	//	}

	//	spatial_object_map::iterator itr = m_spatial_object_map.begin();
	//	for ( ; itr != m_spatial_object_map.end(); itr ++ )
	//	{
	//		spatial_object* obj = dynamic_cast<spatial_object*>( itr->second );
	//		intersect_base* v_collision_obj = dynamic_cast<intersect_base*>( obj );
	//		if( v_collision_obj )
	//		{
	//			axis_aligned_box<double>& v_box = obj->get_aabb();
	//			if( in_detector->collision( v_box ) )
	//			{
	//				collision_object( obj );
	//				in_node->push( v_collision_obj );
	//			}
	//		}			
	//	}

	//	for ( int i = 0; i < 8; i++ )
	//	{
	//		if( m_child_node_array[i] == NULL ) continue;
	//		octree_node* v_node = dynamic_cast<octree_node*>( m_child_node_array[i] );
	//		v_node->collision_detect( in_detector, in_node );
	//	}
	//}

	//void octree_node::collision_object( spatial_object* obj )
	//{

	//}

	bool octree_node::is_twice_lager( axis_aligned_box<double>& box )
	{
		const vector3<double>* node_corners = m_node_aabb.get_corners();
		const vector3<double>* obj_corners = box.get_corners();

		return  ( ( obj_corners[ 4 ].x - obj_corners[ 0 ].x ) <= ( node_corners[ 4 ].x - node_corners[ 0 ].x ) / 2 ) &&
				( ( obj_corners[ 4 ].y - obj_corners[ 0 ].y ) <= ( node_corners[ 4 ].y - node_corners[ 0 ].y ) / 2 ) &&
				( ( obj_corners[ 4 ].z - obj_corners[ 0 ].z ) <= ( node_corners[ 4 ].z - node_corners[ 0 ].z ) / 2 ) ;
	}
}