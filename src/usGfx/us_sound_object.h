///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_sound_object.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : sound_object class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_SOUND_OBJECT_H_
#define _US_SOUND_OBJECT_H_

#include <usGfx/Export.h>
#include <usUtil/us_matrix4.h>

namespace uniscope_globe
{
	class render_argument;

	class USGFX_EXPORT sound_object
	{
	public:
		sound_object(void);

		virtual ~sound_object(void);

	public:
		void set_transform(matrix4<double> tnb_transform);

		void draw(render_argument* args);

	};
}

#endif // _US_SOUND_OBJECT_H_ 