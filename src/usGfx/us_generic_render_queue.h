
///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_generic_render_queue.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : generic_render_queue class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_GENERIC_RENDER_QUEUE_H_
#define _US_GENERIC_RENDER_QUEUE_H_

namespace uniscope_globe
{
	class generic_render_queue : public render_node
	{
	public:
		typedef std::vector<render_object*> render_object_array;

	public:
		generic_render_queue( void );

		virtual ~generic_render_queue( void );

		// override from render_object
	public:
		virtual void draw( render_argument* args );

		virtual void draw_alpha( render_argument* args );

		virtual void occlusion( render_argument* args );
		
		virtual void flush( void );

		virtual void push( render_object* v_obj );

		virtual void clear( void );	

		// override from intersect_base
	public:
		virtual bool intersect( const ray<double>& in_ray, intersect_result& out_result );

		virtual bool intersect( const ray<double>& in_ray, double v_range, intersect_result& out_result  );

	public:
		virtual int get_count( void );

	protected:
		virtual void clear_background( void );

		bool check_intersectable( spatial_object* v_so);

		void draw_water( render_argument* args );

	protected:
		render_object_array* m_current_render_array;

		render_object_array* m_background_render_array;	

		US_AUTO_MUTEX
	};

	typedef generic_render_queue alphe_test_model_render_queue;
}

#endif // _US_GENERIC_RENDER_QUEUE_H_ 