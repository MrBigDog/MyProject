///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_update_argument.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : update_argument class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_update_argument.h"

namespace uniscope_globe
{
	update_argument::update_argument(void)
	{
		m_document = NULL;

		m_observer = NULL;

		m_render = NULL;

		m_data_ready = 0;
	}

	update_argument::update_argument(document_base* v_document, observer_base* v_observer, render_node* v_render, shadow_parameter& v_shadow_param, int data_ready)
	{
		m_document = v_document;

		m_observer = v_observer;

		m_render = v_render;

		m_shadow_param = v_shadow_param;

		m_data_ready = data_ready;
	}

	update_argument::~update_argument(void)
	{

	}


}