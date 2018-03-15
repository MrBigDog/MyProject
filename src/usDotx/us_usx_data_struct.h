///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_usx_data_struct.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : usx_data_struct class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_USX_DATA_STRUCT_H_
#define _US_USX_DATA_STRUCT_H_

#include <usDotx/Export.h>
#include <usUtil/us_common_file.h>

#include <string>
#include <wtypes.h>

using namespace std;

namespace uniscope_globe
{
#define USCOLOR_COLORVALUE_TO_ULONG(cv) \
	USCOLOR_ARGB( (ulong)((cv.a)*255.f), (ulong)((cv.r)*255.f), (ulong)((cv.g)*255.f), (ulong)((cv.b)*255.f) )

#define USCOLOR_ULONG_TO_COLORVALUE(cv, cvl) \
	{\
	cv.a = float(((cvl)>>24)&(0xff)) / 255.0f; \
	cv.r = float(((cvl)>>16)&(0xff)) / 255.0f; \
	cv.g = float(((cvl)>>8)&(0xff)) / 255.0f; \
	cv.b = float((cvl) & 0xff) / 255.0f; \
}\

#define USCOLOR_COLORVALUE_TO_ARGB(cv, alpha, red, green, blue) \
	{\
	alpha = (ulong)((cv.a)*255.f); \
	red   = (ulong)((cv.r)*255.f); \
	green = (ulong)((cv.g)*255.f); \
	blue  = (ulong)((cv.b)*255.f); \
}\

#define USCOLOR_IS_TRANSPARENCY( cvl ) ( ( ((cvl)>>24) & (0xff)) != 255 )

#define USCOLOR_ALPHA_IS_TRANSPARENCY(cvl) (cvl!= 255)

#define USCOLOR_RGB(clv) ( cvl | 0xff000000 )
#define USCOLOR_ALPHA(clv) ( (clv & 0xff000000) >> 24 )
#define USCOLOR_ALPHA_NO_SHIFT(clv) ( clv & 0xff000000 );

	// Color
	struct  usx_color_value
	{
		float r;
		float g;
		float b;
		float a;
		usx_color_value()
		{
			r = g = b = a = 1.0;
		}

		bool operator == (const usx_color_value& cv) const
		{
			return !(*this != cv);
		}

		bool operator != (const usx_color_value& cv) const
		{
			return (this->a != cv.a || this->r != cv.r || this->g != cv.g || this->b != cv.b);
		}
	};

	//////////////////////////////////////////////////////////////////////////
	// usx_color_blend
	//////////////////////////////////////////////////////////////////////////
	class USDOTX_EXPORT usx_color_blend
	{
	public:
		usx_color_blend(void);
		~usx_color_blend(void) {}

	public:
		void set_blend(float v_rate, usx_color_value* v_des);

		void set_color(usx_color_value* v_color);

		uint get_color(void);

	public:
		float m_rate;

		usx_color_value m_cur_value;
		usx_color_value m_src_value;
		usx_color_value m_des_value;
	};

	struct  usx_tri_face_index
	{
		uint a, b, c;
		usx_tri_face_index()
		{
			a = b = c = 0;
		}

		usx_tri_face_index(uint v_a, uint v_b, uint v_c)
		{
			a = v_a;
			b = v_b;
			c = v_c;
		}
	};

	struct  usx_vertex_element
	{
		ulong type;
		ulong method;
		ulong usage;
		ulong usage_index;
	};

	struct usx_texture_file
	{
		int v_type;
		wstring m_texture_file;
		usx_texture_file(int type, const LPCTSTR str)
		{
			v_type = type;
			m_texture_file = str;
		};
	};

	// 内部使用
	struct usx_index_color
	{
		ulong index;
		usx_color_value color_value;
		usx_index_color()
		{
			index = 0;
		}
	};

}

#endif // _US_USX_DATA_STRUCT_H_