///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_http_loader.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : http_loader class
//	Reference : http://topic.csdn.net/t/20021108/10/1159077.html
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_HTTP_LOADER_H_
#define _US_HTTP_LOADER_H_

#include <usCore\Export.h>

namespace uniscope_globe
{
#define TEST_BYTE_COUNT 9
#define MAX_PS_LENGTH	64


#define LOAD_RESULT_SUC		0
#define LOAD_RESULT_ERR		1
#define LOAD_RESULT_NOF		2

	class link_http;
	class byte;

	class USCORE_EXPORT http_loader
	{
	public:
		http_loader(void) { }

		~http_loader(void) { }

	public:
		static int load_from_ude(link_http* v_link, byte* &data_stream, int& v_size);
		static int load_from_uds(link_http* v_link, byte* &data_stream, int& v_size);
		static int load_from_http(link_http* v_link, byte* &data_stream, int& v_size);
	};
}

#endif // _US_HTTP_LOADER_H_