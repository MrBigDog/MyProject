///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_common_render.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : common_render class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_COMMON_RENDER_H_
#define _US_COMMON_RENDER_H_
#include <usCore\Export.h>
namespace uniscope_globe
{
	class USCORE_EXPORT common_render : public render_node
	{
	public:
		typedef std::vector<render_object*> render_object_array;

	public:
		common_render( void );

		virtual ~common_render( void );

		// override from render_object
	public:
		virtual void draw( render_argument* args );

		virtual void flush( void );

		virtual void push( render_object* v_obj );

		virtual void clear( void );	

	public:
		virtual int get_count( void );

	protected:
		virtual void clear_background( void );

	protected:
		render_object_array* m_current_render_array;

		render_object_array* m_background_render_array;	

		US_AUTO_MUTEX
	};
}

#endif // _US_COMMON_RENDER_H_