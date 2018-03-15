
///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_sorted_render_queue.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : sorted_render_queue class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_SORTED_RENDER_QUEUE_H_
#define _US_SORTED_RENDER_QUEUE_H_

namespace uniscope_globe
{
	bool render_object_compare_function( render_object* ro1, render_object* ro2 );

	bool render_object_compare_function_ex( render_object* ro1, render_object* ro2 );

	class sorted_render_queue : public generic_render_queue
	{
	public:
		sorted_render_queue( void );

		virtual ~sorted_render_queue( void );

	public:
		virtual void draw( render_argument* args );

		virtual void draw_alpha( render_argument* args );

		virtual void flush( void );	

	protected:
		void draw_alpha_test( render_argument* args );

		void draw_alpha_blend( render_argument* args );
	};

	typedef sorted_render_queue alphe_blend_model_render_queue;
}

#endif // _US_SORTED_RENDER_QUEUE_H_ 