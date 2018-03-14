///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_download_argument.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : download_argument class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_DOWNLOAD_ARGUMENT_H_
#define _US_DOWNLOAD_ARGUMENT_H_

#include <usCore\Export.h>
#include <usCore\us_event_argument.h>
#include <usUtil\us_common_file.h>
#include <wtypes.h>

namespace uniscope_globe
{
	class raw_buffer;

	enum donwload_status
	{
		US_DOWNLOAD_SUCCEED = 0,
		US_DOWNLOAD_FAILED,
		US_DOWNLOAD_ERROR,
		US_DOWNLOAD_NOFILE,
		US_DOWNLOAD_RESET
	};

	class USCORE_EXPORT download_argument : public event_argument
	{
	public:
		download_argument(void);

		download_argument(raw_buffer* v_stream, int v_status, interior_index v_id = 0);

		virtual ~download_argument(void);

	public:
		raw_buffer* m_stream;

		int m_download_status;

		LPCTSTR m_str_url;

		interior_index m_index;
	};
}

#endif // _US_REQUEST_EVENT_ARGUMENT_H_