///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_resource_killer.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : resource_killer class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_RESOURCE_KILLER_H_
#define _US_RESOURCE_KILLER_H_

#include <usCore\Export.h>
#include <usUtil\us_common_file.h>
#include <usUtil\us_mutex.h>
#include <usCore\us_tls_singleton.h>

#include <vector>
#include <list>

namespace uniscope_globe
{
	class resource_base;
	// clear hardware
	class USCORE_EXPORT resource_killer
	{
	public:
		resource_killer(void);

		virtual ~resource_killer(void);

		typedef std::vector<resource_base*> destroy_array;
		typedef std::list<resource_base*> destroy_list;

	public:
		void push(resource_base* v_ptr);

		void clear(ulong v_time);

	protected:
		US_AUTO_MUTEX

			destroy_array m_array;

		destroy_list m_list;

	};

	class USCORE_EXPORT tls_singleton_resource_killer : public tls_singleton<resource_killer>
	{
	};

#define DEFERRED_RELEASE(obj)		{ if( obj && tls_singleton_resource_killer::ptr() ) tls_singleton_resource_killer::instance().push( obj ); obj = NULL ; }
	//#define CLEAR_RESOURCE			{ singleton_resource_killer::instance().clear();}


	class render_object;
	class object_base;

	// clear memory
	class USCORE_EXPORT memory_ptr_destroyer
	{
	public:
		memory_ptr_destroyer(void);

		virtual ~memory_ptr_destroyer(void);

		typedef std::vector<object_base*> destroy_array;

	public:
		void push(object_base* v_ptr);

		void clear(void);

	protected:
		//US_AUTO_MUTEX

		destroy_array m_array;



	};

	class USCORE_EXPORT tls_singleton_memory_ptr_destroyer : public tls_singleton<memory_ptr_destroyer>
	{
	};

#define AUTO_DESTROY_MEMORY(obj) { if( obj && tls_singleton_memory_ptr_destroyer::ptr() ) tls_singleton_memory_ptr_destroyer::instance().push( obj ); obj = NULL; }
	//#define AUTO_CLEAR_DESTROY_MEMORY {singleton_memory_ptr_destroyer::instance().clear();}



}

#endif // _US_RESOURCE_KILLER_H_