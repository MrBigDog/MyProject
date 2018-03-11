///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_application_base.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : application_base class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_application_base.h"

namespace uniscope_globe
{
	application_base::application_base( HWND hwnd, HWND parent_hwnd, HINSTANCE hinstance )
	{
		// �ؼ�����
		m_hwnd = hwnd;

		// �ؼ���������
		m_parent_hwnd = parent_hwnd;

		// ģ��instance
		m_hinstance = hinstance;


		m_document = NULL;

		m_view = NULL;

		m_manager_group = NULL;
	}

	application_base::~application_base( void )
	{		
	}

	
}