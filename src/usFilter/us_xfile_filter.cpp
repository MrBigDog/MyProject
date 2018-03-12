///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_xfile_filter.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : xfile_filter class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_xfile_filter.h"

namespace uniscope_globe
{
	object_base* xfile_filter::parse( raw_buffer* in_buffer )
	{
		square_buffer* s_b = (square_buffer* )in_buffer;

		usx_parser_object_container v_container;
		v_container.set_path( s_b->get_path().c_str() );

		if ( !singleton_usx_parser_assistant::instance().parse_x_buffer( s_b->get_buffer(), s_b->get_size(), &v_container ) ) 
		{
			return NULL;
		}

		if( v_container.m_mesh_frame_map.size() == 1 )
		{
			usx_mesh_frame_map::iterator itr = v_container.m_mesh_frame_map.begin();
			if( itr->second == NULL )
				return hardware_mesh_factory::create_single_mesh( s_b->get_document(), itr->first, &v_container );
		}

		return hardware_mesh_factory::create_complex_mesh( s_b->get_document(), &v_container );

		//hardware_mesh_buffer_map v_map;

		//usx_mesh_frame_map::iterator itr = v_container.m_mesh_frame_map.begin();
		//for ( ; itr != v_container.m_mesh_frame_map.end(); itr ++ )
		//{
		//	usx_mesh* v_usx_mesh = itr->first;
		//	usx_frame* v_usx_frame = itr->second;		
		//	
		//	hardware_mesh* v_sub_mesh = hardware_mesh_factory::create( v_usx_mesh, v_container.m_root_frame, v_container.get_path().c_str() );
		//	v_map.insert( make_pair( v_sub_mesh, v_usx_frame ) );
		//	AUTO_DELETE( v_usx_mesh );
		//}

		//return hardware_mesh_factory::create_complex_mesh( v_map, v_container.m_root_frame, v_container.m_animation_set_array );

		//return dynamic_cast<object_base*>( new d3d9_complex_hardware_mesh( v_map, v_container.m_root_frame, v_container.m_animation_set_array ) );

		//return NULL;
	}
}