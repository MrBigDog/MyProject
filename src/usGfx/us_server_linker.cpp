///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_server_linker.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : server_linker class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "Stdafx.h"
#include "us_server_linker.h"

namespace uniscope_globe
{
	//server_linker* singleton_server_linker::s_ins = NULL;

	server_linker::server_linker( void )
	{
		m_port = 0;
		m_client = new socket_client( this );

		GUID guid;   
		HRESULT hr = CoCreateGuid(&guid);

		RPC_WSTR wsGuid = NULL;
		UuidToString( &guid, &wsGuid );

		m_guid_string = (LPCTSTR)wsGuid;

		RpcStringFree( &wsGuid );
	}

	server_linker::~server_linker( void )
	{
		AUTO_DELETE( m_client );
		m_packets.clear();
	}

	void server_linker::set_host( const LPCTSTR str_host )
	{
		m_str_host = str_host;
	}

	void server_linker::set_port( int port )
	{
		m_port = port;
	}

	bool server_linker::login( int server_mark )
	{
		// set host / port 
		m_str_host = singleton_system_environment::instance().m_ai_server;
		m_port = singleton_system_environment::instance().m_ai_server_port;

		m_client->connect( string_ext::from_wstring( m_str_host ), m_port );

		network_argument args;
		args.m_msg_id = US_NETWORK_LOGIN;
		args.m_user_name = string_ext::from_wstring( m_guid_string );
		send( args );

		return true;
	}

	bool server_linker::logout( void )
	{
		network_argument args;
		args.m_msg_id = US_NETWORK_LOGOUT;
		args.m_user_name = string_ext::from_wstring( m_guid_string );
		send( args );

		m_client->close_con();

		return true;
	}

	// dispatch
	void server_linker::dispatch( void )
	{
		US_LOCK_AUTO_MUTEX

		int v_count = (int)m_packets.size();
		for ( int i = 0; i < v_count; i++ )
		{
			packet pck = m_packets.front();

			network_argument args;
			if ( m_taker.take( pck, args ) )
			{
				m_receiver( (event_argument*)&args );
			}

			m_packets.pop_front();
		}
	}

	// send
	bool server_linker::send( network_argument& args )
	{
		packet send_pck;

		if ( !m_sender.send( args , send_pck ) )
		{
			return false;
		}

		/*
		switch( args.m_msg_id )
		{
		case US_NETWORK_CHANGE:
			{
				packet take_pck;
				take_pck.v_head.msg_id = args.m_msg_id;

				raw_buffer stm( (byte*)take_pck.v_stream, 1024 );
				take_pck.v_head.msg_len += stm.write( args.m_user_name );
				take_pck.v_head.msg_len += stm.write( (void*)&args.m_pos, sizeof(args.m_pos) );
				take_pck.v_head.msg_len += stm.write( (void*)&args.m_ahead, sizeof(args.m_ahead) );
				take_pck.v_head.msg_len += stm.write( (void*)&args.m_ahead_speed, sizeof(args.m_ahead_speed) );
				take_pck.v_head.msg_len += stm.write( (void*)&args.m_rotate_dir, sizeof(args.m_rotate_dir) );
				take_pck.v_head.msg_len += stm.write( (void*)&args.m_rotate_speed, sizeof(args.m_rotate_speed) );
				take_pck.v_head.msg_len += stm.write( (void*)&args.m_animation_code, sizeof(args.m_animation_code) );

				keep( take_pck );
			}
			break;
		}
		//*/

		return m_client->send( send_pck );
	}

	void server_linker::keep( packet& pck )
	{
		US_LOCK_AUTO_MUTEX
		m_packets.push_back( pck );
	}

	bool server_linker::is_valid( int msg_id )
	{
		return ( msg_id < 6 ) && ( msg_id > 0 );
	}

	
}