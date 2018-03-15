///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_shadow_render_queue.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : shadow_render_queue class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_SHADOW_RENDER_QUEUE_H_
#define _US_SHADOW_RENDER_QUEUE_H_

namespace uniscope_globe
{
	class LIB_GFX_API shadow_render_queue
	{
	public :
		shadow_render_queue( void );

		~shadow_render_queue( void );
	
		typedef std::vector<render_object*> render_object_array;

	public:
		void push( render_object* v_ro );
		
		void clear();
		
	public:
		render_object_array m_render_object_queue;
	};
}

#endif // _US_SHADOW_RENDER_QUEUE_H_ 