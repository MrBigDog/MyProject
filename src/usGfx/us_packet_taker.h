///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_packet_taker.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : packet_taker class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_PACKET_TAKER_H_
#define _US_PACKET_TAKER_H_
#include <usGfx/Export.h>
#include <usGfx/us_socket_client.h>
#include <usGfx/us_network_argument.h>

#include <usUtil/us_common_file.h>
namespace uniscope_globe
{
	class USGFX_EXPORT packet_taker
	{
	public:
		packet_taker(void);

		virtual ~packet_taker(void);

	public:
		bool take(const packet& pck, network_argument& args);

	private:
		bool case_login(const packet& pck, network_argument& args);

		bool case_logout(const packet& pck, network_argument& args);

		bool case_enter(const packet& pck, network_argument& args);

		bool case_leave(const packet& pck, network_argument& args);

		bool case_change(const packet& pck, network_argument& args);

	};
}

#endif // _US_PACKET_TAKER_H_