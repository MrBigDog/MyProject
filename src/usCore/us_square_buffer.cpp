///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_square_buffer.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : square_buffer class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_square_buffer.h"

namespace uniscope_globe
{
	square_buffer::~square_buffer( void )
	{
		
	}

	//square_buffer::square_buffer( byte* p_data, int data_size, interior_index index, const LPCTSTR str_path, double v_height )
	//	:raw_buffer(p_data, data_size)
	//{
	//	m_index = index;
	//	m_str_path = str_path;
	//	m_height_offset = v_height;
	//	m_document = NULL;
	//}

	square_buffer::square_buffer( document_base* in_doc, byte* p_data, int data_size, interior_index index, const LPCTSTR str_path, double v_height )
		:raw_buffer(p_data, data_size)
	{
		m_index = index;
		m_str_path = str_path;
		m_height_offset = v_height;
		m_document = in_doc;
	}
}