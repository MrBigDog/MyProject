#pragma once

class post_process_render_queue
{
public:
	post_process_render_queue(void);
	~post_process_render_queue(void);
};

///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_post_process_render_queue.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : post_process_render_queue class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_POST_PROCESS_RENDER_QUEUE_H_
#define _US_POST_PROCESS_RENDER_QUEUE_H_

#include <usGfx/Export.h>
#include <usCore/us_common_render.h>
#include <usUtil/us_common_file.h>

namespace uniscope_globe
{
	class d3d9_render_target_chain;

	class USGFX_EXPORT post_process_render_queue : public common_render
	{
	protected:
		post_process_render_queue(void) {}

	public:
		post_process_render_queue(render_device* device);

		virtual ~post_process_render_queue(void);

	public:
		virtual void draw(render_argument* args);

	protected:
		void draw_hsl(render_argument* args);

	protected:
		//d3d9_effect_pp_hdr* m_render_effect;
		std::vector<ulong> m_pp_effect_index_array;
		d3d9_render_target_chain* m_pp_render_target;
	};
}

#endif // _US_POST_PROCESS_RENDER_QUEUE_H_ 