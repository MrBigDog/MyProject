///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_octree_node.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : octree_node class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_OCTREE_NODE_H_
#define _US_OCTREE_NODE_H_

#include <usCore\Export.h>
#include <usCore\us_spatial_node.h>
#include <usCore\us_intersect_base.h>
#include <usCore\us_intersect_result.h>

#include <vector>

namespace uniscope_globe
{
	class spatial_object;
	class node_visitor;

	class USCORE_EXPORT octree_node
		: public spatial_node
		, public intersect_base
	{
	public:
		typedef std::vector<spatial_object*> spatial_object_array;

	public:
		octree_node(void);

		virtual ~octree_node(void);

	public:
		virtual bool traverse(node_visitor* in_visitor);

	public:
		// spatial_node
		virtual bool attach_object(spatial_object* object);

		virtual spatial_object* get_attached_object(cpstr name);

		virtual spatial_object* get_attached_object(long index);

		virtual bool detach_object(spatial_object* obj);

		virtual spatial_object* detach_object(cpstr name);

		virtual void detach_all_object(void);

		// intersect_base
	public:
		virtual bool intersect(const ray<double>& a_ray, intersect_result& result);

		virtual bool intersect(part_sphere& pt_sphr, intersect_result& out_result);

	protected:
		//bool add_to_map( spatial_object* object );

		bool add_to_array(spatial_object* object);

		virtual octree_node* create_octree_node(void);

		bool is_twice_lager(axis_aligned_box<double>& in_box);

		bool octree_node_attach_object(spatial_object* object);

		int find_appropriate_quadrant(axis_aligned_box<double>& in_box);

		axis_aligned_box<double> get_child_axis_aligned_box(int index);

		bool traverse_recursive(node_visitor* in_visitor);

	public:
		vector3<float> m_half_size;

		bool m_batch_mode;

		spatial_object_array m_spatial_object_array;
	};
}

#endif // _US_OCTREE_NODE_H_