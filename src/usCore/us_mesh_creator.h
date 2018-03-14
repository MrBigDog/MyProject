///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_mesh_creator.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose :
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_MESH_CREATOR_H_
#define _US_MESH_CREATOR_H_

#include <usCore\Export.h>
#include <usUtil\us_mutex.h>

#include <vector>

namespace uniscope_globe
{
	class hardware_base;
	class render_device;

	class USCORE_EXPORT mesh_creator
	{
	public:
		mesh_creator(void) { }
		~mesh_creator(void) { clear(); }

		typedef std::vector<hardware_base*> hardware_array;

	public:
		void operator += (hardware_base* hardware);

		void update(render_device* device);

		void clear(void);

	public:
		hardware_array m_create_array;

		US_AUTO_MUTEX
	};

	//class LIB_CORE_API singleton_mesh_creator
	//	: public singleton<mesh_creator>
	//{
	//public:
	//	singleton_mesh_creator( void )
	//	{
	//	}

	//	virtual ~singleton_mesh_creator( void )
	//	{
	//	}
	//};
}

#endif // _US_MESH_POOL_H_