///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_hierarchy_texture.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_hierarchy_texture class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_d3d9_hierarchy_texture.h"

namespace uniscope_globe
{
	d3d9_texture_frame::d3d9_texture_frame( void )
	{

	}

	d3d9_texture_frame::d3d9_texture_frame( raw_buffer* buffer, texture_info& tex_info )
	{
		m_raw_buffer = buffer;

		m_texture_info = tex_info;

		m_d3d_texture = NULL;
	}

	d3d9_texture_frame::~d3d9_texture_frame( void )
	{
		destroy();
	}

	long d3d9_texture_frame::create( render_device* device )
	{
		if( m_raw_buffer == NULL ) 
		{
			return 0;
		}

		if ( m_valid )
		{
			return 0;
		}		

		m_mem_size = device->create_texture_from_file_in_memory( m_raw_buffer, (void**)&m_d3d_texture );

		AUTO_DELETE( m_raw_buffer );

		if ( m_mem_size > 0 )
		{
			m_valid = true;
		}		

		return m_mem_size;
	}

	bool d3d9_texture_frame::destroy( void )
	{
		AUTO_RELEASE( m_d3d_texture );
 
		return true;
	}

	void d3d9_texture_frame::draw( render_device* device, uint stage_index )
	{
		device->set_texture( stage_index, (texture_handle)m_d3d_texture );
	}

	ULONG_PTR d3d9_texture_frame::get_handle(void)
	{
		return (ULONG_PTR)m_d3d_texture;
	}

	// d3d9_hierarchy_texture
	d3d9_hierarchy_texture::d3d9_hierarchy_texture( void )
	{
		m_total_time = 1;
		m_start_time = 0;
		m_last_time = 0;
		m_active_frame = -1;
	}

	d3d9_hierarchy_texture::~d3d9_hierarchy_texture( void )
	{
		texture_frame_array::iterator itr = m_texture_frame_array.begin();
		for( ; itr != m_texture_frame_array.end(); itr++ )
		{
			AUTO_DELETE( *itr );
		}
		m_texture_frame_array.clear();
	}

	void d3d9_hierarchy_texture::set_active_frame( long v_active_frame )
	{
		m_active_frame = v_active_frame;
	}

	// Override vgHardwareBuffer
	long d3d9_hierarchy_texture::create( render_device* device )
	{
		if ( m_valid )
		{
			return 0;
		}

		bool is_succeed = true;
		texture_frame_array::iterator itr = m_texture_frame_array.begin();
		for( ; itr != m_texture_frame_array.end(); itr++ )
		{
			long v_size = (*itr)->create( device );
			if ( v_size == 0 )
			{
				is_succeed = false;
			}
			m_mem_size += v_size;
		}

		if ( is_succeed )
		{
			m_valid = true;
		}
		

		return m_mem_size;
	}

	bool d3d9_hierarchy_texture::destroy( render_device* device )
	{
		texture_frame_array::iterator itr = m_texture_frame_array.begin();
		for( ; itr != m_texture_frame_array.end(); itr++ )
		{
			AUTO_DELETE( *itr );
		}

		m_texture_frame_array.clear();

		return true;
	}

	ULONG_PTR d3d9_hierarchy_texture::get_handle(void) 
	{
		//if ( m_active_frame < m_texture_frame_array.size() )
		//{
		//	return ulong(m_texture_frame_array[m_active_frame]); 
		//}
		//
		//return 0;

		//if ( m_active_frame == -1 && m_valid )
		//{
		//	return ulong(this);
		//}

		if ( m_active_frame == -1 )
		{
			ulong v_current_time = timeGetTime();
			m_start_time += v_current_time - m_last_time;
			m_start_time = m_start_time % m_total_time;
			m_last_time  = v_current_time;

			d3d9_texture_frame* v_current_frame = 0;
			int v_frame_count = (int)m_texture_frame_array.size();
			for (int ni = 0; ni < v_frame_count; ni++)
			{
				d3d9_texture_frame* v_texture_frame = m_texture_frame_array[ni];

				if( m_start_time >= v_texture_frame->get_key_time() )
				{
					v_current_frame = v_texture_frame;
				}

			}

			return v_current_frame->get_handle();
		}
		else if ( m_active_frame < m_texture_frame_array.size() )
		{
			d3d9_texture_frame* v_texture_frame = m_texture_frame_array[m_active_frame];			

			return v_texture_frame->get_handle();
		}

		return (ulong)0;
	}

	bool d3d9_hierarchy_texture::refresh( void )
	{
		if ( !m_valid )
		{
			//(*singleton_common_manager_group::instance().get_texture_creator()) += this;
			singleton_common_manager_group::instance().get_hardware_creator()->insert_texture( this );
		}

		return m_valid;
	}

	void d3d9_hierarchy_texture::insert_frame( d3d9_texture_frame* in_frame )
	{
		m_texture_frame_array.push_back(in_frame);
	}

	void d3d9_hierarchy_texture::set_matrix( render_device* device, ulong stage_index,  matrix4<float>* texture_matrix )
	{

	}

	void d3d9_hierarchy_texture::draw( render_device* device, uint stage_index )
	{
		if ( m_active_frame == -1 )
		{
			static ulong v_last_tick = timeGetTime();
			ulong v_elapse = timeGetTime() - v_last_tick;
			ulong v_time = v_elapse % m_total_time;

			d3d9_texture_frame* v_current_frame = 0;
			int v_frame_count = (int)m_texture_frame_array.size();
			for (int ni = 0; ni < v_frame_count; ni++)
			{
				d3d9_texture_frame* v_texture_frame = m_texture_frame_array[ni];

				if( v_time >= v_texture_frame->get_key_time() )
				{
					v_current_frame = v_texture_frame;
				}

			}

			v_current_frame->draw( device, stage_index );
		}
		else if ( m_active_frame < m_texture_frame_array.size() )
		{
			d3d9_texture_frame* v_texture_frame = m_texture_frame_array[m_active_frame];
			v_texture_frame->draw( device, stage_index );
		}

	}	
}