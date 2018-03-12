

///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_threadsafe_memory_pool.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : threadsafe_memory_pool class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "us_threadsafe_memory_pool.h"

namespace uniscope_globe
{

threadsafe_memory_pool::threadsafe_memory_pool(void)
{
	m_aligned_thread_buffer_size = 2048*2048*4;
}

threadsafe_memory_pool::~threadsafe_memory_pool(void)
{
	std::map<interior_index,byte*>::iterator itr = m_aligned_thread_buffer_map.begin();
	for ( ; itr != m_aligned_thread_buffer_map.end(); itr++ )
	{
		AUTO_ALIGNED_FREE_MEMORY( itr->second );
	}
	m_aligned_thread_buffer_map.clear();
}

byte* threadsafe_memory_pool::get_aligned_thread_buffer( int buf_size, int align )
{
	unsigned int th_id = ::GetCurrentThreadId();
	
	interior_index v_key = align;
	v_key = (v_key<<32) | th_id;
	std::map<interior_index, byte*>::iterator itr = m_aligned_thread_buffer_map.find( v_key );
	if ( itr != m_aligned_thread_buffer_map.end() )
	{
		return itr->second;
	}
	
	byte* v_buf = AUTO_ALIGNED_ALLOC_MEMORY( buf_size, align );
	m_aligned_thread_buffer_map.insert( make_pair( v_key, v_buf ) );

	return v_buf;
}

byte* threadsafe_memory_pool::alloc_aligned_buffer( int buf_size, int align )
{
	return AUTO_ALIGNED_ALLOC_MEMORY( buf_size, align );
}

void threadsafe_memory_pool::free_alloc_aligned_buffer( byte* buf )
{
	AUTO_ALIGNED_FREE_MEMORY( buf );
}

}