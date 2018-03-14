///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_texture_creator.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : texture_pool class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_TEXTURE_CREATOR_H_
#define _US_TEXTURE_CREATOR_H_

#include <usCore\Export.h>
#include <usUtil\us_mutex.h>
#include <vector>

namespace uniscope_globe
{
	class hardware_base;
	class render_device;

	class USCORE_EXPORT texture_creator
	{
	public:
		texture_creator(void) { }
		~texture_creator(void) { clear(); }

		typedef std::vector<hardware_base*> hardware_array;

	public:
		void operator += (hardware_base* hardware);

		void update(render_device* device);

		void clear(void);

	public:
		hardware_array m_create_array;

		US_AUTO_MUTEX
	};

	//class USCORE_EXPORT singleton_texture_creator
	//	: public singleton<texture_creator>
	//{
	//public:
	//	singleton_texture_creator( void )
	//	{
	//	}

	//	virtual ~singleton_texture_creator( void )
	//	{
	//	}
	//};

}

#endif // _US_TEXTURE_POOL_H_