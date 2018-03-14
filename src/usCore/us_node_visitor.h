///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_node_visitor.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : node_visitor class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_NODE_VISITOR_H_
#define _US_NODE_VISITOR_H_

#include <usCore\Export.h>
#include <usCore\us_intersect_base.h>

namespace uniscope_globe
{
	class spatial_object;
	class node_visitor : public intersect_base
	{
	public:
		node_visitor(void);

		virtual ~node_visitor(void);

	public:
		virtual bool intersect(const axis_aligned_box<double>& box) { return true; }

	public:
		virtual bool begin_visit(void) { return true; }

		virtual bool end_visit(void) { return true; }

		// 返回true继续遍历，false停止遍历
		virtual bool visit(spatial_object* in_object) = 0;
	};
}

#endif // _US_NODE_VISITOR_H_