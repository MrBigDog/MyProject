///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_hierarchy_texture.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_hierarchy_texture class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_D3D9_HIERARCHY_TEXTURE_H_
#define _US_D3D9_HIERARCHY_TEXTURE_H_

#include <usGfx/Export.h>
#include <usUtil/us_matrix4.h>
#include <usUtil/us_common_file.h>
#include <usCore/us_hardware_texture.h>

#include <d3d9.h>
#include <vector>

namespace uniscope_globe
{

	class USGFX_EXPORT d3d9_texture_frame : public hardware_texture
	{
	protected:
		d3d9_texture_frame(void);

	public:
		d3d9_texture_frame(raw_buffer* buffer, texture_info& tex_info);

		virtual ~d3d9_texture_frame(void);

	public:
		virtual long create(render_device* device);

		virtual bool destroy(void);

		virtual void draw(render_device* device, uint stage_index);

		void set_key_time(int time) { m_key_time = time; }

		int get_key_time(void) { return m_key_time; }

		virtual ULONG_PTR get_handle(void);

	public:
		ulong			   m_key_time;

		LPDIRECT3DTEXTURE9  m_d3d_texture;

	};

	class USGFX_EXPORT d3d9_hierarchy_texture
		: public hardware_texture
	{
	public:
		typedef std::vector<d3d9_texture_frame*> texture_frame_array;

	public:
		d3d9_hierarchy_texture(void);

		virtual ~d3d9_hierarchy_texture(void);

	public:
		void set_active_frame(long v_active_frame);

	public:
		virtual long create(render_device* device);

		virtual bool destroy(render_device* device);

		virtual ULONG_PTR get_handle(void);

		virtual bool refresh(void);

	public:
		virtual void insert_frame(d3d9_texture_frame* in_frame);

		virtual void set_matrix(render_device* device, ulong stage_index, matrix4<float>* texture_matrix);

		virtual void draw(render_device* device, uint stage_index);

		void set_total_time(ulong total_time) { m_total_time = total_time; }

		ulong get_total_time(void) { return m_total_time; }

	private:
		texture_frame_array	m_texture_frame_array;

		ulong				m_total_time;

		ulong				m_start_time;

		ulong				m_last_time;

		ulong				m_memory_size;

		long				m_active_frame;
	};
}

#endif // _US_D3D9_HIERARCHY_TEXTURE_H_