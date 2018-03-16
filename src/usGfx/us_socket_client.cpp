///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_socket_client.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : socket_client class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "Stdafx.h"
#include "us_socket_client.h"

namespace uniscope_globe
{
	socket_client::socket_client( void )
	{
		m_port	 = 80;
		m_linker = 0;
	}

	socket_client::socket_client( server_linker* linker )
	{
		m_port	 = 80;
		m_linker = linker;
	}

	socket_client::~socket_client( void )
	{
	}

	bool socket_client::connect( string host, int port )
	{
		m_host_address = host;
		m_port = port;

		if ( !m_socket.connect( m_host_address.c_str(), m_port ) )
		{
			return false;
		}

		init_thread();
		
		return true;
	}

	void socket_client::reconnect( string host, int port )
	{
		end();

		m_socket.close();

		if ( m_socket.connect( host.c_str(), port ) )
		{
			start();
		}
	}

	void socket_client::close_con( void )
	{
		fina_thread();

		m_socket.close();
	}

	void socket_client::init_thread( void )
	{
		if ( m_thread_data.thread_event == NULL )
		{
			m_thread_data.thread_event = CreateEvent( NULL, TRUE, FALSE, NULL );
		}

		if ( m_thread_data.thread_handle == NULL )
		{
			m_thread_data.thread_handle = CreateThread( NULL, 0, listening, this, CREATE_SUSPENDED, &m_thread_data.thread_id );
			SetThreadPriority( m_thread_data.thread_handle, THREAD_PRIORITY_NORMAL);

			ResumeThread( m_thread_data.thread_handle );
		}

		start();
	}

	void socket_client::fina_thread( void )
	{
		end();

		TerminateThread( m_thread_data.thread_handle, 0 );

		m_thread_data.destroy();
	}

	void socket_client::start(void)
	{
		SetEvent( m_thread_data.thread_event );
	}

	void socket_client::waiting( void )
	{
		WaitForSingleObject( m_thread_data.thread_event, INFINITE );
	}

	void socket_client::end(void)
	{
		ResetEvent( m_thread_data.thread_event );
	}

	bool socket_client::recv( )
	{
		packet pck;
		
		m_socket.receive( (char*)&pck.v_head, sizeof(pck.v_head) );

		if ( m_linker->is_valid( pck.v_head.msg_id ) )
		{
			if ( pck.v_head.msg_len > 0 )
			{
				m_socket.receive( pck.v_stream, pck.v_head.msg_len );
				m_linker->keep( pck );
			}			
		}
		else
		{
			reconnect( m_host_address, m_port );
		}

		return true;
	}

	bool socket_client::send(  const packet& pck  )
	{
		int len = pck.v_head.msg_len + sizeof( pck.v_head );
		return m_socket.send( (char*)&pck, len );
	}

	unsigned long socket_client::listening( void* param )
	{
		socket_client* v_client = (socket_client *)param;

		while( true )
		{
			v_client->waiting();

			v_client->recv();
		}

		return 0;
	}


	
}