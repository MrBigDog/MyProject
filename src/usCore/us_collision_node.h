///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_collision_node.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : collision_node class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_COLLISION_NODE_H_
#define _US_COLLISION_NODE_H_
#include <usCore\Export.h>
namespace uniscope_globe
{
	class USCORE_EXPORT collision_node
	{
	public:
		typedef std::vector< intersect_base* > intersect_object_array;

	public:
		collision_node( void );

		virtual ~collision_node( void );

		virtual void push( intersect_base* in_obj );

		virtual void flush( void );

		// override
	public:
		virtual bool collision( ray<double>& a_ray, intersect_result& result );

	protected:
		intersect_object_array* m_back_array;

		intersect_object_array* m_current_array;

		US_AUTO_MUTEX
	};
}

#endif // _US_COLLISION_NODE_H_