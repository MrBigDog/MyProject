///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_geometry_mark.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : geometry_mark class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_GEOMETRY_MARK_H_
#define _US_GEOMETRY_MARK_H_

#include <usGfx/Export.h>
#include <usUtil/us_rect.h>
#include <usUtil/us_matrix4.h>
#include <usUtil/us_common_file.h>

namespace uniscope_globe
{
	class observer_base;
	class render_device;
	class hardware_texture;

	class USGFX_EXPORT geometry_mark
	{
	public:
		geometry_mark(void);

		virtual ~geometry_mark(void);

		// override from render_object
	public:
		virtual void draw(observer_base* v_obsr, render_device* device);

	public:
		void set_texture(hardware_texture* v_texture);
		void set_mark_info(matrix_4f& v_mat, vector_3f& v_pos, vector_3f& v_cen, ulong v_color);
		void set_font_info(const LPCTSTR str_text, matrix_4f& v_mat, rect& v_rect, ulong v_color);

	protected:
		hardware_texture* m_texture;

		matrix_4f	m_mark_matrix;
		vector_3f	m_mark_pos;
		vector_3f	m_mark_cen;
		ulong		m_mark_color;

		ustring		m_text;
		matrix_4f	m_font_matrix;
		rect		m_font_rect;
		ulong		m_font_color;

	};
}

#endif // _US_GEOMETRY_MARK_H_