//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_event_base.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : angle template
//	Reference : http://www.geometrictools.com ( Wild Magic Library )
//				http://www.ogre3d.org/		  ( Ogre Object-Oriented Graphics Rendering Engine )
//				http://mathworld.wolfram.com/Matrix.html
//
//////////////////////////////////////////////////////////////////////////
#ifndef _US_EVENT_BASE_H_
#define _US_EVENT_BASE_H_

#include <usCore\Export.h>
#include <vector>

namespace uniscope_globe
{
	class event_handle;
	class event_argument;

	enum USCORE_EXPORT event_type
	{
		US_EVENT_REQUEST = 0,
		US_EVENT_TERRAIN = 1,
		US_EVENT_COUNT
	};

	class USCORE_EXPORT event_base
	{
	public:
		event_base(void);
		virtual ~event_base(void);

		typedef std::vector<event_handle> event_handle_array;

	public:
		int get_event_type(void);

		virtual void operator () (event_argument* event_args);

		// register
		event_base& operator += (const event_handle& handler);

		void register_handle(const event_handle& handler);

		// unregister
		event_base& operator -= (const event_handle& handler);

		void unregister_handle(const event_handle& handler);

		void clear();

	protected:
		event_handle_array handle_array;

		event_type m_event_type;

	};

}

#endif
