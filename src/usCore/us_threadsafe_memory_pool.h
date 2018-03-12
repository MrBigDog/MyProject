
///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_threadsafe_memory_pool.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : threadsafe_memory_pool class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_THREADSAFE_MEMORY_POOL_H_
#define _US_THREADSAFE_MEMORY_POOL_H_
#include <usCore\Export.h>
namespace uniscope_globe
{
	class USCORE_EXPORT threadsafe_memory_pool
	{
	public:
		threadsafe_memory_pool(void);
		~threadsafe_memory_pool(void);

	public:
		byte*	get_aligned_thread_buffer(int buf_size, int align);

	public:
		byte*	alloc_aligned_buffer(int buf_size, int align);
		void	free_alloc_aligned_buffer(byte* buf);

	protected:
		int m_aligned_thread_buffer_size;
		std::map<interior_index, byte*>  m_aligned_thread_buffer_map;
	};
}

#endif // _US_THREADSAFE_MEMORY_POOL_H_

