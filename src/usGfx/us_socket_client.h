///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_socket_client.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : socket_client class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_SOCKET_CLIENT_H_
#define _US_SOCKET_CLIENT_H_

namespace uniscope_globe
{
	struct packet_head
	{
		unsigned int msg_id;
		unsigned int msg_len;
		packet_head()
		{
			msg_id = 0;
			msg_len = 0;
		}
	};

	struct packet
	{
		packet_head v_head;
		char		v_stream[1024];
		packet()
		{
			memset( v_stream ,0, 1024 );
		}
	};

	class server_linker;

	class LIB_GFX_API socket_client
	{
	protected:
		socket_client();
	public:
		socket_client( server_linker* linker );
		virtual ~socket_client( void );

		friend class server_linker;

		US_AUTO_MUTEX

	private:
		// socket
		us_socket	m_socket;

		// host addr/post
		string		m_host_address;
		int			m_port;

		// thread info
		thread_data	m_thread_data;

		server_linker* m_linker;

	public:
		// con & close
		bool connect( string host, int port );
		void reconnect( string host, int port );
		void close_con( void );

	public:
		bool recv( void );
		bool send( const packet& pck );

		//	listener
		static unsigned long __stdcall listening( void* param );

	protected:
		void init_thread( void );
		void fina_thread( void );

		// start & end
		void start( void );
		void end( void );
		void waiting( void );
	 
	};
}

#endif // _US_PACKET_LISTENER_H_