//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_sphere.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : sphere template
//	Reference : http://www.geometrictools.com ( Wild Magic Library )
//				http://www.ogre3d.org/		  ( Ogre Object-Oriented Graphics Rendering Engine )
//				http://mathworld.wolfram.com/Matrix.html
//
//////////////////////////////////////////////////////////////////////////
#include "us_rect.h"
#include <usUtil/us_point.h>
#include <usUtil/us_common_file.h>

namespace uniscope_globe
{
	rect::rect(void)
	{
		m_left = 0;

		m_top = 0;

		m_right = 0;

		m_bottom = 0;
	}

	rect::~rect(void)
	{
	}

#ifdef WIN32
	rect::rect(RECT& api_rc)
	{
		m_left = api_rc.left;

		m_top = api_rc.top;

		m_right = api_rc.right;

		m_bottom = api_rc.bottom;
	}
#endif //WIN32

	rect::rect(int left, int top, int right, int bottom)
	{
		m_left = left;

		m_top = top;

		m_right = right;

		m_bottom = bottom;
	}

	void rect::set_rect(int left, int top, int right, int bottom)
	{
		m_left = left;

		m_top = top;

		m_right = right;

		m_bottom = bottom;
	}

	void rect::make_identity(void)
	{
		m_left = m_top = m_right = m_bottom = 0;
	}

	void rect::make_invalid(void)
	{
		m_left = 1 << 31 - 1;
		m_right = -m_left;
		m_top = m_left;
		m_bottom = -m_left;
	}

	int rect::get_width(void)
	{
		return (m_right - m_left);
	}

	int rect::get_height(void)
	{
		return (m_bottom - m_top);
	}

	point rect::get_center(void) const
	{
		return point((m_left + m_right) * 0.5, (m_top + m_bottom) * 0.5);
	}

	bool rect::is_valid(void)
	{
		return (m_right >= m_left) &&
			(m_bottom >= m_top);
	}

	void rect::offset(int cx, int cy)
	{
		m_left += cx;
		m_right += cx;

		m_top += cy;
		m_bottom += cy;
	}

	bool rect::contain(const rect& rc_check) const
	{
		if (rc_check.m_left < m_left ||
			rc_check.m_right > m_right ||
			rc_check.m_top < m_top ||
			rc_check.m_bottom > m_bottom)
			return false;

		return true;
	}

	bool rect::contain(const point& pt) const
	{
		if (pt.x < m_left ||
			pt.x > m_right ||
			pt.y < m_top ||
			pt.y > m_bottom)
			return false;

		return true;
	}

	bool rect::intersect(const rect& in_rect) const
	{
		bool b_ret = (m_bottom >= in_rect.m_top && m_top <= in_rect.m_bottom &&
			m_right >= in_rect.m_left  && m_left <= in_rect.m_right);

		return b_ret;
	}

	bool rect::intersect(region& in_region) const
	{
		for (ulong ni = 0; ni < in_region.get_size(); ni++)
		{
			if (contain(in_region.get_at(ni)))
				return true;
		}
		for (int ni = 0; ni < 4; ni++)
		{
			if (in_region.contain(this->get_conner(ni)))
				return true;
		}

		return false;
	}

	void rect::deflate(int l, int t, int r, int b)
	{
		m_left += l;
		m_top += t;
		m_right -= r;
		m_bottom -= b;
	}

	void rect::inflate(int l, int t, int r, int b)
	{
		m_left -= l;
		m_top -= t;
		m_right += r;
		m_bottom += b;
	}

	point rect::restrict(const point& pt)
	{
		point out_pt = pt;

		if (out_pt.x < m_left) out_pt.x = m_left;
		if (out_pt.x > m_right) out_pt.x = m_right;
		if (out_pt.y < m_top) out_pt.y = m_top;
		if (out_pt.y > m_bottom) out_pt.y = m_bottom;

		return out_pt;
	}

	rect rect::combine(const rect& rc1, const rect& rc2)
	{
		rect ret;

		ret.m_left = min(rc1.m_left, rc2.m_left);
		ret.m_top = min(rc1.m_top, rc2.m_top);
		ret.m_right = max(rc1.m_right, rc2.m_right);
		ret.m_bottom = max(rc1.m_bottom, rc2.m_bottom);


		return ret;
	}

	void rect::combine(const rect& rc)
	{
		m_left = min(m_left, rc.m_left);
		m_top = min(m_top, rc.m_top);
		m_right = max(m_right, rc.m_right);
		m_bottom = max(m_bottom, rc.m_bottom);
	}

	void rect::combine(const point& pt)
	{
		m_left = min(m_left, pt.x);
		m_top = min(m_top, pt.y);
		m_right = max(m_right, pt.x);
		m_bottom = max(m_bottom, pt.y);
	}

	bool rect::operator == (rect& rect_test)
	{
		if (m_left == rect_test.m_left &&
			m_top == rect_test.m_top &&
			m_right == rect_test.m_right &&
			m_bottom == rect_test.m_bottom)
		{
			return true;
		}

		return false;
	}

	point rect::get_conner(int type) const
	{
		switch (type)
		{
		case 0:
			return point(m_left, m_top);
			break;
		case 1:
			return point(m_right, m_top);
			break;
		case 2:
			return point(m_right, m_bottom);
			break;
		case 3:
			return point(m_left, m_bottom);
			break;
		}
		return point(m_left, m_top);
	}
}
