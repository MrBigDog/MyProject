//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_rect.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : rect template
//	Reference : http://www.geometrictools.com ( Wild Magic Library )
//				http://www.ogre3d.org/		  ( Ogre Object-Oriented Graphics Rendering Engine )
//				http://mathworld.wolfram.com/Matrix.html
//
//////////////////////////////////////////////////////////////////////////
#ifndef _US_RECT_H_
#define _US_RECT_H_

#include <usUtil\Export.h>
#include <wtypes.h>

namespace uniscope_globe
{
	class point;
	class region;

	class USUTIL_EXPORT rect
	{
	public:
		rect(void);

#ifdef WIN32
		rect(RECT& api_rc);
#endif //WIN32

		rect(int left, int top, int right, int bottom);

		~rect(void);

		void set_rect(int left, int top, int right, int bottom);

		void make_identity(void);

		void make_invalid(void);

		int get_width(void);

		int get_height(void);

		point get_center(void) const;

		bool is_valid(void);

		void offset(int cx, int cy);

		bool contain(const rect& rc_check) const;

		bool contain(const point& pt) const;

		bool intersect(const rect& in_rect) const;

		bool intersect(region& in_region) const;

		void deflate(int l, int t, int r, int b);

		void inflate(int l, int t, int r, int b);

		point restrict(const point& pt);

		bool operator == (rect& rect_test);

		static rect combine(const rect& rc1, const rect& rc2);

		void combine(const rect& rc);

		void combine(const point& pt);

		int get_left(void) { return m_left; }

		int get_top(void) { return m_top; }

		int get_right(void) { return m_right; }

		int get_bottom(void) { return m_bottom; }

		void set_left(int left) { m_left = left; }

		void set_top(int top) { m_top = top; }

		void set_right(int right) { m_right = right; }

		void set_bottom(int bottom) { m_bottom = bottom; }

		point get_conner(int type) const;

	public:
		int m_left;
		int m_top;
		int m_right;
		int m_bottom;

	};// class rect

} // uniscope_globe

#endif // _US_RECT_H_