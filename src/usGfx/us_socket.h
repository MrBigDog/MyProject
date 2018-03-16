///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_socket.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : socket
//	Reference : http://www.adp-gmbh.ch/win/misc/sockets.html
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_WIN_SOCKET_H_
#define _US_WIN_SOCKET_H_
#include <usGfx/Export.h>
#include <winsock.h>
//#include <usUtil/us_common_file.h>
namespace uniscope_globe
{
	class USGFX_EXPORT us_socket
	{
	public:
		us_socket(void);
		~us_socket();

	public:
		bool connect(const char * addr, int port);
		void close();

		bool receive(char* stream, int size) volatile;
		bool send(char* stream, int size) volatile;

	private:
		void start();
		void end();

	private:
		SOCKET m_s;
	};

}


#endif