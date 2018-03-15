
///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_render_object_orthographic_volume.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : render_object_orthographic_volume class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_RENDER_OBJECT_ORTHOGRAPHIC_VOLUME_H_
#define _US_RENDER_OBJECT_ORTHOGRAPHIC_VOLUME_H_

namespace uniscope_globe
{
	class LIB_GFX_API render_object_orthographic_volume
		: public render_object
		, public shadow_volume_creator
	{
	public:
		render_object_orthographic_volume( void );

		virtual ~render_object_orthographic_volume( void );

		friend class render_object_factory;

		friend class render_object_line_factory;


	public:
		static render_object_orthographic_volume* create_shared_instance();

	public:
		virtual render_object* get_shadow_object( void ) { return this; }

		virtual int get_pass1( void ) { return 8; }

		virtual int get_pass2( void ) { return 3; }

		// override from render_object
	public:
		virtual void draw( render_argument* args );
	};
}

#endif // _US_RENDER_OBJECT_ORTHOGRAPHIC_VOLUME_H_ 