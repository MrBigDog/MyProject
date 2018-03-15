///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_packet_sender.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : packet_sender class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_PACKET_SENDER_H_
#define _US_PACKET_SENDER_H_

namespace uniscope_globe
{
	class LIB_GFX_API packet_sender
	{
	public:
		packet_sender( void );

		virtual ~packet_sender( void );

	public:
		bool send( const network_argument& args, packet& pck );

	private:
		bool case_login( const network_argument& args, packet& pck );

		bool case_logout( const network_argument& args, packet& pck );

		bool case_enter( const network_argument& args, packet& pck );

		bool case_leave( const network_argument& args, packet& pck );

		bool case_change( const network_argument& args, packet& pck );


	};
}

#endif // _US_PACKET_SENDER_H_