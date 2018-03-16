///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_server_linker.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : server_linker class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_PACKET_PARSER_H_
#define _US_PACKET_PARSER_H_
#include <usGfx/Export.h>
#include <usGfx/us_packet_taker.h>
#include <usGfx/us_packet_sender.h>
#include <usGfx/us_socket_client.h>
#include <usGfx/us_network_argument.h>
#include <usCore/us_event_base.h>
#include <usUtil/us_mutex.h>
#include <list>

namespace uniscope_globe
{
	class socket_client;
	class USGFX_EXPORT server_linker
	{
	public:
		server_linker(void);
		virtual ~server_linker(void);

		typedef std::list<packet> packet_list;

		event_base m_receiver;

		friend class socket_client;

	public:
		void set_host(const LPCTSTR str_host);
		void set_port(int port);

	public:
		bool login(int server_mark);
		bool logout(void);

	public:
		// dispatch
		void dispatch(void);

		// send
		bool send(network_argument& args);

	private:
		void keep(packet& pck);

		bool is_valid(int msg_id);

	protected:
		socket_client*	m_client;

		packet_list		m_packets;

		packet_taker	m_taker;
		packet_sender	m_sender;

		ustring			m_str_host;
		int				m_port;

	public:
		ustring			m_guid_string;

		US_AUTO_MUTEX
	};

	/*
	class LIB_APP_API singleton_server_linker
		: public singleton<server_linker>
	{
	public:
		singleton_server_linker( void )
		{
		}

		virtual ~singleton_server_linker( void )
		{
		}
	};
	//*/
}

#endif // _US_PACKET_PARSER_H_