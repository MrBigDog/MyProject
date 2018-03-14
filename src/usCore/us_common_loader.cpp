///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_common_loader.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : common_loader class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "Stdafx.h"
#include "us_common_loader.h"
#include <usCore/us_downloader.h>
#include <usCore/us_link_base.h>
#include <usCore/us_http_loader.h>
#include <usCore/us_link_http.h>
#include <usCore/us_native_loader.h>
#include <usCore/us_link_manager.h>

namespace uniscope_globe
{
	common_loader::common_loader(const LPCTSTR str_url, downloader* v_downloader)
	{
		m_downloader = v_downloader;
		m_link = m_downloader->get_link_manager()->get_link(str_url);
	}

	common_loader::~common_loader(void)
	{
		US_AUTO_RELEASE_LINK(m_link)
	}

	int common_loader::load(byte* &data_stream, int& v_size)
	{
		if (m_link)
		{
			int link_type = m_link->get_link_type();

			switch (link_type)
			{
			case US_LINK_TYPE_UDE:
			{
				return http_loader::load_from_ude((link_http*)m_link, data_stream, v_size);
			}
			break;
			case US_LINK_TYPE_UDS:
			{
				return http_loader::load_from_uds((link_http*)m_link, data_stream, v_size);
			}
			break;
			case US_LINK_TYPE_WMTS:
			{
				// ude uds wtms 采用相同的处理方法
				return http_loader::load_from_uds((link_http*)m_link, data_stream, v_size);
			}
			break;
			case US_LINK_TYPE_ROOT:
			{
				return native_loader::load_from_root(m_link, data_stream, v_size);
			}
			break;
			case US_LINK_TYPE_HTTP:
			{
				return http_loader::load_from_http((link_http*)m_link, data_stream, v_size);
			}
			break;
			case US_LINK_TYPE_LOCAL:
			{
				return native_loader::load_from_local(m_link, data_stream, v_size);
			}
			break;
			}
		}

		return LOAD_RESULT_ERR;
	}


}