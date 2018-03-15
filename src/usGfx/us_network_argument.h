///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_network_argument.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : network_argument class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_NETWORK_ARGUMENT_H_
#define _US_NETWORK_ARGUMENT_H_

namespace uniscope_globe
{

#define 	US_NETWORK_LOGIN		1
#define		US_NETWORK_LOGOUT		2
#define		US_NETWORK_ENTER		3
#define		US_NETWORK_LEAVE		4
#define		US_NETWORK_CHANGE		5

	class LIB_GFX_API network_argument
		: public event_argument
	{
	public:
		network_argument( void );

		network_argument( int msg_id, string scene_name );

		virtual ~network_argument( void );

	public:
		int		m_msg_id;
		string	m_user_name;

		string	m_scene;
		int		m_flag;

		int		m_login;
		int		m_enter;
		uint	m_animation_code;

		vector_3f		m_pos;

		float			m_ahead_speed;
		vector_3f		m_ahead;

		float			m_rotate_speed;
		quaternion_4f	m_rotate_dir;		 
	};
}

#endif // _US_NETWORK_ARGUMENT_H_