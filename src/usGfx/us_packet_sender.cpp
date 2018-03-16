///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_packet_sender.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : packet_sender class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "Stdafx.h"
#include "us_packet_sender.h"

namespace uniscope_globe
{
	packet_sender::packet_sender( void )
	{
	}

	packet_sender::~packet_sender( void )
	{
	}

	bool packet_sender::send( const network_argument& args, packet& pck )
	{
		bool result = false;
		switch( args.m_msg_id )
		{
		case US_NETWORK_LOGIN:
			result = case_login( args, pck );
			break;
		case US_NETWORK_LOGOUT:
			result = case_logout( args, pck );
			break;
		case US_NETWORK_ENTER:
			result = case_enter( args, pck );
			break;
		case US_NETWORK_LEAVE:
			result = case_leave( args, pck );
			break;
		case US_NETWORK_CHANGE:
			result = case_change( args, pck );
			break;
		}

		return result;
	}

	bool packet_sender::case_login( const network_argument& args, packet& pck )
	{
		int n_size = args.m_user_name.size() + 1;

		pck.v_head.msg_id = args.m_msg_id;
		pck.v_head.msg_len = n_size<<1;
		
		memcpy( pck.v_stream, args.m_user_name.c_str(), n_size );

		memcpy( pck.v_stream + n_size, args.m_user_name.c_str(), n_size );

		return true;
	}

	bool packet_sender::case_logout( const network_argument& args, packet& pck )
	{
		pck.v_head.msg_id = args.m_msg_id;
		pck.v_head.msg_len = 0;
		
		return true;
	}

	bool packet_sender::case_enter( const network_argument& args, packet& pck )
	{
		int str_size = args.m_scene.size() + 1;
		int pos_size = sizeof(args.m_pos);
		int dir_size = sizeof(args.m_rotate_dir);
		
		pck.v_head.msg_id = args.m_msg_id;
		pck.v_head.msg_len = str_size + pos_size + dir_size;

		int off = 0;

		memcpy( pck.v_stream, args.m_scene.c_str(), str_size );
		off += str_size;

		memcpy( pck.v_stream + off, &args.m_pos, pos_size );
		off += pos_size;

		memcpy( pck.v_stream + off, &args.m_rotate_dir, dir_size );

		return true;
	}

	bool packet_sender::case_leave( const network_argument& args, packet& pck )
	{
		pck.v_head.msg_id = args.m_msg_id;
		pck.v_head.msg_len = 0;

		return true;
	}

	bool packet_sender::case_change( const network_argument& args, packet& pck )
	{
		int pos_size = sizeof(args.m_pos);
		int ahe_size = sizeof(args.m_ahead);
		int ahs_size = sizeof(args.m_ahead_speed);
		int dir_size = sizeof(args.m_rotate_dir);
		int rsd_size = sizeof(args.m_rotate_speed);
		int anc_size = sizeof(args.m_animation_code);

		pck.v_head.msg_id = args.m_msg_id;
		pck.v_head.msg_len = pos_size + ahe_size + ahs_size + dir_size + rsd_size + anc_size;

		int off = 0;

		memcpy( pck.v_stream + off, &args.m_pos, sizeof(args.m_pos) );
		off += pos_size;

		memcpy( pck.v_stream + off, &args.m_ahead, sizeof(args.m_ahead) );
		off += ahe_size;

		memcpy( pck.v_stream + off, &args.m_ahead_speed, sizeof(args.m_ahead_speed) );
		off += ahs_size;

		memcpy( pck.v_stream + off, &args.m_rotate_dir, sizeof(args.m_rotate_dir) );
		off += dir_size;

		memcpy( pck.v_stream + off, &args.m_rotate_speed, sizeof(args.m_rotate_speed) );
		off += rsd_size;

		memcpy( pck.v_stream + off, &args.m_animation_code, sizeof(args.m_animation_code) );

		return true;
	}


}