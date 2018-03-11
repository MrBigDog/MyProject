///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_common_mission_queue.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : common_mission_queue class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_COMMON_MISSION_QUEUE_H_
#define _US_COMMON_MISSION_QUEUE_H_
#include <usCore\Export.h>
namespace uniscope_globe
{
	
	class USCORE_EXPORT common_mission_queue
		: public mission_queue
	{
	public:
		common_mission_queue( void );

		virtual ~common_mission_queue( void );

		typedef std::vector<mission_base*> remove_mission_array;

	public:
		void remove_mission( mission_base* v_mission );
		
		void clean( void );

	protected:
		remove_mission_array m_mission_array;
	};
	//*/

	class USCORE_EXPORT singleton_common_mission_queue
		: public singleton<common_mission_queue>
	{
	public:
		singleton_common_mission_queue( void )
		{
		}

		virtual ~singleton_common_mission_queue( void )
		{

		}

	};
}

#endif // _US_COMMON_MISSION_QUEUE_H_