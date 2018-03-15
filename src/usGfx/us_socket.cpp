///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_socket.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : us_socket class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "Stdafx.h"
#include "us_socket.h"

namespace uniscope_globe
{
	us_socket::us_socket( void )
		: m_s( INVALID_SOCKET )
	{
		start();
	}

	us_socket::~us_socket( void )
	{
		end();
	}

	void us_socket::start()
	{
		WSADATA info;
		WSAStartup( MAKEWORD( 2, 2 ), &info );
	}

	void us_socket::end()
	{
		WSACleanup();
	}

	bool us_socket::connect( const char * addr, int port )
	{
		struct hostent *he = gethostbyname( addr );
		if ( he == 0 )
		{
			return false;
		}

		struct sockaddr_in v_addr;
		v_addr.sin_port = htons( port );
		v_addr.sin_family = AF_INET;
		v_addr.sin_addr = *( (struct in_addr*)he->h_addr );
		memset( &(v_addr.sin_zero), 0, 8 ); 

		if ( m_s && m_s != -1 )
		{
			closesocket( m_s );
		}

		m_s = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

		if ( m_s == -1 )
		{
			return false;
		}

		return ( -1 != ::connect( m_s, (struct sockaddr*)&v_addr, sizeof(v_addr) ) );
	}

	void us_socket::close()
	{
		if ( m_s != INVALID_SOCKET ) 
		{
			shutdown( m_s, 0x02 );
			closesocket( m_s );
			m_s = INVALID_SOCKET;
		}
	}

	bool us_socket::receive( char* stream, int size ) volatile
	{
		int sum_recv = 0;
		while ( size != 0 )
		{
			int num_recv = ::recv( m_s, stream + sum_recv, size, 0 );
			switch( num_recv )
			{
			case -1:
			case  0:
				return false;
				break;
			}

			size -= num_recv;
			sum_recv += num_recv;
		}

		return true;
	}

	bool us_socket::send ( char* stream, int size ) volatile
	{
		int sum_send = 0;
		while ( size != 0 )
		{
			int num_send = ::send( m_s, stream + sum_send, size, 0 );
			switch( num_send )
			{
			case -1:
			case  0:
				return false;
				break;
			}

			size -= num_send;
			sum_send += num_send;
		}

		return true;
	}
	
}