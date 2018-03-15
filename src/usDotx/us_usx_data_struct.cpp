///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_usx_data_struct.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : usx_data_struct class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_usx_data_struct.h"

namespace uniscope_globe
{
	//////////////////////////////////////////////////////////////////////////
	// usx_color_blend
	//////////////////////////////////////////////////////////////////////////
	usx_color_blend::usx_color_blend()
	{
		m_rate = 0.05f;
	}

	void usx_color_blend::set_blend( float v_rate, usx_color_value* v_des )
	{
		
		m_des_value = *v_des;
		if ( m_des_value.a < m_cur_value.a )
		{
			m_rate = -v_rate;
		}
		else
		{
			m_rate = v_rate;
		}
	}

	void usx_color_blend::set_color( usx_color_value* v_color )
	{
		m_src_value = *v_color;
	}

	uint usx_color_blend::get_color( void )
	{
		/*
		m_cur_value.r = m_cur_value.r + m_rate * (m_des_value.r - m_cur_value.r);
		m_cur_value.g = m_cur_value.g + m_rate * (m_des_value.g - m_cur_value.g);
		m_cur_value.b = m_cur_value.b + m_rate * (m_des_value.b - m_cur_value.b);
		m_cur_value.a = m_cur_value.a + m_rate * (m_des_value.a - m_cur_value.a);

		return USCOLOR_COLORVALUE(m_cur_value.a, m_cur_value.r, m_cur_value.g, m_cur_value.b );
		//*/

		return 0xffffffff;

	}

}