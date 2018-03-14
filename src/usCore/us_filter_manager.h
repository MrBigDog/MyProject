///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_filter_manager.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : filter_manager class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_FILTER_MANAGER_H_
#define _US_FILTER_MANAGER_H_

#include <usCore\Export.h>
#include <usUtil\us_common_file.h>
#include <usUtil\us_singleton.h>
#include <unordered_map>

namespace uniscope_globe
{
	class filter_base;

	class USCORE_EXPORT filter_manager
	{
#if _MSC_VER >= 1900
		typedef  std::unordered_map<ulong, filter_base*> filter_map;
#else
		typedef  stdext::hash_map<ulong, filter_base*> filter_map;
#endif


	public:
		filter_manager(void);

		virtual ~filter_manager(void);

	public:
		object_base* parse(raw_buffer* in_buffer);

		filter_base* get_filter(ulong dwDataType);

		void register_filter(ulong data_type, filter_base* filter);

	public:
		void clear(void);

	protected:
		filter_map		m_filter_map;

	};

	class USCORE_EXPORT singleton_filter_manager : public singleton<filter_manager>
	{
	public:
		singleton_filter_manager(void)
		{
		}

		virtual ~singleton_filter_manager(void)
		{
		}
	};
}

#endif // _US_FILTER_MANAGER_H_