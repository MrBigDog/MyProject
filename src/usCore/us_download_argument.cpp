///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_download_argument.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : download_argument class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "Stdafx.h"
#include "us_download_argument.h"

namespace uniscope_globe
{
	download_argument::download_argument( void )
	{
		m_stream = NULL;
	}

	download_argument::download_argument( raw_buffer* v_stream, int v_status,  interior_index v_id  )
	{
		m_stream = v_stream;
		m_download_status = v_status;
		m_index = v_id;
	}

	download_argument::~download_argument( void )
	{
		m_stream = NULL;		
	}

	
}