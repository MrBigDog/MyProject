///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_link_base.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : link_base class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_LINK_BASE_H_
#define _US_LINK_BASE_H_

#include <usCore\Export.h>
#include <usUtil\us_common_file.h>

#include <wtypes.h>

namespace uniscope_globe
{
#define US_LINK_TYPE_UDE		0x0000
#define US_LINK_TYPE_ROOT		0x0001
#define US_LINK_TYPE_HTTP		0x0002
#define US_LINK_TYPE_LOCAL		0x0004
#define US_LINK_TYPE_UDS		0x0008
#define US_LINK_TYPE_WMTS		0x0010

	class link_manager;
	class USCORE_EXPORT link_base
	{
	protected:
		link_base(void) {}

	public:
		link_base(const LPCTSTR url, int type);

		virtual ~link_base(void);

	public:
		void set_link_manager(link_manager* v_link_mngr);

		int get_link_type(void);

		ustring get_link_url(void);

	protected:
		ustring	m_link_url;

		int	m_link_type;

		link_manager* m_link_mngr;

	};
}

#endif // _US_LINK_BASE_H_