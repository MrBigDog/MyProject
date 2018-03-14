///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_network_link.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : network_link class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_NETWORK_LINK_H_
#define _US_NETWORK_LINK_H_

#include <usCore\Export.h>
#include <usUtil\us_common_file.h>

namespace uniscope_globe
{
	class event_argument;
	class download_mission;

	class USCORE_EXPORT network_link
	{
	public:
		network_link(void);

		virtual ~network_link(void);

		virtual void set_link(cpstr link);

		virtual ustring get_link(void);

		virtual void load(void);

		virtual void reload(void);

		virtual bool is_loaded(void);

		virtual void on_download_finish(event_argument* args);

	protected:
		bool m_loaded;

		ustring m_link;

		download_mission*	m_download_mission;
	};
}

#endif // _US_NETWORK_LINK_H_