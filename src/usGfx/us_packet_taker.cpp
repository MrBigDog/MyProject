///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_packet_taker.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : packet_taker class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "Stdafx.h"
#include "us_packet_taker.h"
#include <usUtil/us_raw_buffer.h>

namespace uniscope_globe
{
	packet_taker::packet_taker(void)
	{

	}

	packet_taker::~packet_taker(void)
	{

	}

	bool packet_taker::take(const packet& pck, network_argument& args)
	{
		bool result = false;
		switch (pck.v_head.msg_id)
		{
		case US_NETWORK_LOGIN:
			result = case_login(pck, args);
			break;
		case US_NETWORK_LOGOUT:
			result = case_logout(pck, args);
			break;
		case US_NETWORK_ENTER:
			result = case_enter(pck, args);
			break;
		case US_NETWORK_LEAVE:
			result = case_leave(pck, args);
			break;
		case US_NETWORK_CHANGE:
			result = case_change(pck, args);
			break;
		}

		return result;
	}

	bool packet_taker::case_login(const packet& pck, network_argument& args)
	{
		raw_buffer stm((byte*)pck.v_stream, pck.v_head.msg_len);

		args.m_msg_id = pck.v_head.msg_id;
		stm.read(args.m_user_name);
		stm.read((void*)&args.m_login, sizeof(args.m_login));

		args.m_flag = -1;

		return true;
	}

	bool packet_taker::case_logout(const packet& pck, network_argument& args)
	{
		raw_buffer stm((byte*)pck.v_stream, pck.v_head.msg_len);

		args.m_msg_id = pck.v_head.msg_id;
		stm.read(args.m_user_name);

		args.m_flag = -1;

		return true;
	}

	bool packet_taker::case_enter(const packet& pck, network_argument& args)
	{
		raw_buffer stm((byte*)pck.v_stream, pck.v_head.msg_len);

		args.m_msg_id = pck.v_head.msg_id;
		stm.read(args.m_user_name);
		stm.read((void*)&args.m_enter, sizeof(args.m_enter));

		args.m_flag = -1;

		return true;
	}

	bool packet_taker::case_leave(const packet& pck, network_argument& args)
	{
		raw_buffer stm((byte*)pck.v_stream, pck.v_head.msg_len);

		args.m_msg_id = pck.v_head.msg_id;
		stm.read(args.m_user_name);

		args.m_flag = -1;

		return true;
	}

	bool packet_taker::case_change(const packet& pck, network_argument& args)
	{
		raw_buffer stm((byte*)pck.v_stream, pck.v_head.msg_len);

		args.m_msg_id = pck.v_head.msg_id;
		stm.read(args.m_user_name);
		stm.read((void*)&args.m_pos, sizeof(args.m_pos));
		stm.read((void*)&args.m_ahead, sizeof(args.m_ahead));
		stm.read((void*)&args.m_ahead_speed, sizeof(args.m_ahead_speed));
		stm.read((void*)&args.m_rotate_dir, sizeof(args.m_rotate_dir));
		stm.read((void*)&args.m_rotate_speed, sizeof(args.m_rotate_speed));
		stm.read((void*)&args.m_animation_code, sizeof(args.m_animation_code));

		args.m_flag = -1;

		return true;
	}


}