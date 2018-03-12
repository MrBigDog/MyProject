///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_region.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : region class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "us_region.h"

namespace uniscope_globe
{
	region::region(void)
	{

	}

	region::~region(void)
	{

	}

	void region::add_point(point& pt)
	{
		m_point_array.push_back(pt);
		m_bound.combine(pt);
	}

	void region::clear(void)
	{
		m_point_array.clear();
		m_bound.make_identity();
	}

	ulong region::get_size(void)
	{
		return (ulong)m_point_array.size();
	}

	point region::get_at(ulong index)
	{
		return m_point_array[index];
	}

	rect region::get_bound(void)
	{
		return m_bound;
	}

	bool region::contain(const point& pt)
	{
		bool inside = false;

		int size = m_point_array.size();
		for (int i = 0; i < size; i++)
		{
			point p1 = m_point_array[(i + size) % size];
			point p2 = m_point_array[(i + 1 + size) % size];

			if (pt.y < p2.y)
			{
				//p1 above ray
				if (pt.y >= p1.y)
				{
					//p1 on or blew ray
					if ((pt.y - p1.y) * (p2.x - p1.x) > (pt.x - p1.x) * (p2.y - p1.y))
					{
						inside = !inside;
					}
				}
			}
			else if (pt.y < p1.y)
			{
				if ((pt.y - p1.y) * (p2.x - p1.x) < (pt.x - p1.x) * (p2.y - p1.y))
				{
					inside = !inside;
				}
			}
		}
		return inside;

	}

	bool region::intersect(const rect& in_rect)
	{
		for (int ni = 0; ni < 4; ni++)
		{
			if (this->contain(in_rect.get_conner(ni)))
				return true;
		}

		for (int ni = 0; ni < (int)(m_point_array.size()); ni++)
		{
			if (in_rect.contain(m_point_array[ni]))
				return true;
		}

		return false;
	}
}