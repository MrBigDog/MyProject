///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_shadow_volume_creator.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : shadow_volume_creator class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_SHADOW_VOLUME_CREATOR_H_
#define _US_SHADOW_VOLUME_CREATOR_H_

#include <usCore\Export.h>
#include <usUtil\us_vector4.h>
#include <usUtil\us_common_file.h>
#include <usUtil\us_sphere.h>
namespace uniscope_globe
{
	class render_object;
	class render_argument;

	class USCORE_EXPORT shadow_volume_creator
	{
	public:
		shadow_volume_creator(void);

		virtual ~shadow_volume_creator(void);

	public:
		virtual render_object* get_shadow_object(void) { return m_shadow_object; }

		virtual bool create_shadow_object(render_argument* args) { return false; }

		virtual bool create_shadow_object(vector4<double>& in_light, ulong v_stamp) { return false; }

		virtual void clear_shadow_object(void) { AUTO_DELETE(m_shadow_object); }

		virtual int get_pass1(void) { return 2; }

		virtual int get_pass2(void) { return 3; }

	public:
		virtual render_object* get_shadow_object_from_point_light(sphere<double> dome) { return NULL; }

	protected:
		render_object* m_shadow_object;
	};
}

#endif // _US_SHADOW_VOLUME_CREATOR_H_