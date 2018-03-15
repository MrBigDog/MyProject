///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_spatial_node.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : spatial_node class
//	Reference : Ogre OgreSceneNode.h
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_SPATIAL_NODE_H_
#define _US_SPATIAL_NODE_H_

#include <usCore\Export.h>
#include <usCore\us_node_base.h>
#include <usCore\us_spatial_object.h>
#include <map>
namespace uniscope_globe
{
	class USCORE_EXPORT spatial_node : public node_base
	{
	public:
		typedef std::map<ustring, spatial_object* > spatial_object_map;

	public:
		spatial_node(void);

		virtual ~spatial_node(void);

	public:
		// Adds a scene object to this node.
		virtual bool attach_object(spatial_object* object);

		// Reports the number of objects attached to this node.
		virtual unsigned short get_attached_object_number(void) const;

		// Retrieves a pointer to an attached object.
		virtual spatial_object* get_attached_object(cpstr name);

		virtual spatial_object* get_attached_object(long index);

		// Detaches an object by pointer.
		virtual bool detach_object(spatial_object* obj);

		// Detaches the named object from this node and returns a pointer to it.
		virtual spatial_object* detach_object(cpstr name);

		// Detaches all objects attached to this node.
		virtual void detach_all_object(void);

	public:
		virtual axis_aligned_box<double>& get_node_aabb();

		virtual void set_node_aabb(axis_aligned_box<double>& in_box);

	public:
		spatial_object_map m_spatial_object_map;

		axis_aligned_box<double> m_node_aabb;
	};
}

#endif // _US_SPATIAL_NODE_H_