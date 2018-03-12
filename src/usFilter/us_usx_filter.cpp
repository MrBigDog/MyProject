///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_usx_filter.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : usx_filter class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_usx_filter.h"

namespace uniscope_globe
{
	object_base* usx_filter::parse( raw_buffer* in_buffer )
	{
		//square_buffer* s_b = (square_buffer* )in_buffer;

		//unsigned char* v_usx_buffer = ( unsigned char* )( in_buffer->get_buffer() + 4 );
		//ulong v_size = in_buffer->get_size() - 4;

		//usx_parser_object_container v_container;
		//v_container.set_path( s_b->get_path().c_str() );

		//bool b_suc = singleton_usx_parser_assistant::instance().parse_x_buffer(v_usx_buffer , v_size, &v_container );
		//if ( !b_suc ) 
		//{
		//	return NULL;
		//}

		//hardware_mesh_buffer_map v_map;

		//ulong v_fvf = position_texture2::fvf;
		//if( system_environment::s_enable_shadow )
		//	v_fvf = position_normal_texture2::fvf;

		//usx_mesh_frame_map::iterator itr = v_container.m_mesh_frame_map.begin();
		//for ( ; itr != v_container.m_mesh_frame_map.end(); itr ++ )
		//{
		//	usx_mesh* v_mesh = itr->first;
		//	usx_frame* v_frame = NULL;
		//	if( v_mesh->m_skin_info != NULL )
		//	{
		//		v_fvf = position_weights4_bone_texture::fvf;
		//	}
		//	else
		//	{				
		//		v_frame = v_container.m_root_frame->find_frame( itr->second.c_str() );
		//	}

		//	v_mesh->m_fvf = v_fvf;

		//	v_map.insert( make_pair( hardware_mesh_buffer::create_from_usx_mesh( v_mesh , v_container.m_root_frame, v_container.get_path().c_str() ) , v_frame ) );
		//}
		//v_container.m_mesh_frame_map.clear();

		//for ( int i = 0; i < (int)v_container.m_mesh_array.size(); i++ )
		//{
		//	usx_mesh* v_mesh = v_container.m_mesh_array[i];
		//	if( v_mesh->m_skin_info != NULL )
		//	{
		//		v_fvf = position_weights4_bone_texture::fvf;
		//	}			

		//	v_mesh->m_fvf = v_fvf;
		//	v_map.insert( make_pair( hardware_mesh_buffer::create_from_usx_mesh( v_mesh, v_container.m_root_frame, v_container.get_path().c_str() ) , (usx_frame*)NULL ) );
		//}
		//v_container.m_mesh_array.clear();

		//return dynamic_cast<object_base*>( new d3d9_complex_hardware_mesh( v_map, &v_container.m_root_frame, v_container.m_animation_set_array, v_fvf ) );

		return NULL;

	}

	
}