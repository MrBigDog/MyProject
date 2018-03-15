///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_render_argument.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : render_argument class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_RENDER_ARGUMENT_H_
#define _US_RENDER_ARGUMENT_H_

#include <usCore\Export.h>
#include <usCore\us_shadow_parameter.h>
#include <vector>

namespace uniscope_globe
{
	class render_node;
	class render_device;
	class observer_base;
	class sound_device;
	class frame_counter;
	class render_manager_base;
	class render_effect;

#define US_MATERIAL_TYPE_NONE				0x0000
#define US_MATERIAL_TYPE_NORMAL				0x0001
#define US_MATERIAL_TYPE_ALPHA_TEST			0x0010
#define US_MATERIAL_TYPE_ALPHA_BLEND		0x0020
#define US_MATERIAL_TYPE_DYNAMIC			0x0040
#define US_MATERIAL_TYPE_WATER				0x0080
#define US_MATERIAL_TYPE_ERROR				0x0100


	class USCORE_EXPORT render_argument
	{
	public:
		render_argument(void);

		render_argument(render_device* device, observer_base* in_observer);

		render_argument(render_device* device, observer_base* in_observer, frame_counter* in_counter);

		render_argument(render_device* device, sound_device* in_sound_device, observer_base* in_observer, frame_counter* in_counter);

		render_argument(render_device* device, sound_device* in_sound_device, observer_base* in_observer, frame_counter* in_counter, shadow_parameter& in_shadow_param);

		render_argument(render_device* device, sound_device* in_sound_device, observer_base* in_observer, frame_counter* in_counter, shadow_parameter& in_shadow_param, render_effect* in_render);

		virtual ~render_argument(void);

		typedef std::vector<render_effect*> render_effect_array;

	public:
		render_device* m_device;

		sound_device*  m_sound_device;

		observer_base* m_observer;

		frame_counter* m_frame_counter;

		shadow_parameter m_shadow_param;

		render_effect* m_render;

		render_effect_array m_effect_array;

		render_manager_base* m_render_manager;

		int m_render_flag;
	};
}

#endif // _US_RENDER_ARGUMENT_H_