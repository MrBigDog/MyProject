///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_common_loader.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : common_loader class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_COMMON_LOADER_H_
#define _US_COMMON_LOADER_H_

#include <usCore\Export.h>
#include <usUtil\us_common_file.h>
#include <wtypes.h>

namespace uniscope_globe
{
	class link_base;
	class downloader;

	class USCORE_EXPORT common_loader
	{
	protected:
		common_loader(void) { m_link = 0; }

	public:
		common_loader(const LPCTSTR str_url, downloader* v_downloader);

		virtual ~common_loader(void);

	public:
		int load(byte* &data_stream, int& v_size);

	private:
		link_base*	m_link;

		downloader* m_downloader;

	};
}

#endif // _US_COMMON_LOADER_H_