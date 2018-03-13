///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_cartesian_object.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : cartesian_object class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_CARTESIAN_OBJECT_H_
#define _US_CARTESIAN_OBJECT_H_

#include <usCore\Export.h>
#include <usCore\us_spatial_object.h>
#include <usCore\us_shared_data.h>
#include <usUtil\us_cartesian_transform.h>

namespace uniscope_globe
{
#define US_OCTREE_NODE_FLAG_DEFAULT			0
#define US_OCTREE_NODE_FLAG_ROOT			1

	class USCORE_EXPORT cartesian_object
		: public cartesian_transform
		, virtual public spatial_object
		, virtual public shared_data
	{
	public:
		cartesian_object(void);

		virtual ~cartesian_object(void);

	public:
		virtual void release(void);

		virtual matrix_4d get_matrix_cart2geo(void);

	};
}

#endif // _US_CARTESIAN_OBJECT_H_