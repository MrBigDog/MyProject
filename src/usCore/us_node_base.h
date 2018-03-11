//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_node_base.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : node_base class
//	Reference : Ogre node
//
//////////////////////////////////////////////////////////////////////////
#ifndef _US_NODE_BASE_H_
#define _US_NODE_BASE_H_
#include <usCore\Export.h>
namespace uniscope_globe
{
	class USCORE_EXPORT node_base
	{
	public:
		typedef std::vector<node_base*> child_node_array;

	public:
		node_base(void);

		virtual ~node_base(void);

		// Set this node's parent.
		virtual void set_parent(node_base* parent_node);

		// Gets this node's parent (NULL if this is the root).
		virtual node_base* get_parent(void);

		// Adds a (precreated) child scene node to this node. If it is attached to another node,it must be detached first.
		virtual void add_child(node_base* child_node);

		virtual node_base* remove_at(ulong index);

		// Drops the specified child from this node.
		virtual void remove_child(node_base* child_node);

		// Drops all the children from this node.
		virtual void remove_all_children(void);

		// Gets the size of the children
		virtual int get_children_number(void);

		// Gets a pointer to a child node.
		virtual node_base* get_child(ulong index);

		void notify_level(ulong parent_level);

		ulong get_level(void);

	protected:
		ulong m_level;

		child_node_array m_child_node_array;

		node_base*	   m_parent_node;
	};

}

#endif //_US_NODE_BASE_H_

