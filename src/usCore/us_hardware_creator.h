///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_hardware_creator.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : hardware_creator class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_HARDWARE_CREATOR_H_
#define _US_HARDWARE_CREATOR_H_

#include <usCore\Export.h>
#include <usUtil/us_mutex.h>
#include <vector>

namespace uniscope_globe
{
	class hardware_base;
	class render_device;

	class USCORE_EXPORT hardware_creator
	{
	public:
		hardware_creator(void);

		~hardware_creator(void);

		typedef std::vector<hardware_base*> hardware_array;

	public:
		void insert_mesh(hardware_base* hardware);

		void insert_texture(hardware_base* hardware);

		void update(render_device* device, ulong v_time);

		void clear(void);

	public:
		hardware_array m_mesh_create_array;

		hardware_array m_text_create_array;

		US_AUTO_MUTEX

	};
}

#endif // _US_HARDWARE_CREATOR_H_