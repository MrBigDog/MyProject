///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_resource_manager.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : resource_manager class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_RESOURCE_MANAGER_H_
#define _US_RESOURCE_MANAGER_H_

//#include <usCore\Export.h>
#include <usUtil/us_object_base.h>
#include <usCore/us_resource_container.h>
#include <unordered_map>
namespace uniscope_globe
{
	template<typename KEY>
	class resource_manager : public object_base
	{
	public:
#if _MSC_VER >= 1900
		typedef std::unordered_map<KEY, resource_container<KEY>*> resource_map;
#else
		typedef stdext::hash_map<KEY, resource_container<KEY>*> resource_map;
#endif


	public:
		resource_manager(void) {}

		virtual ~resource_manager(void)
		{
			resource_map::iterator itr = m_resource_map.begin();
			for (; itr != m_resource_map.end(); itr++)
			{
				itr->second->detach();
				//AUTO_RELEASE( itr->second );
			}
			m_resource_map.clear();
		}

	public:
		void add_resource(KEY key, resource_container<KEY>* v_resource)
		{
			resource_map::iterator itr = m_resource_map.find(key);
			if (itr != m_resource_map.end())
			{
				itr->second->detach();
			}

			m_resource_map[key] = v_resource;
			v_resource->attach(this);
			v_resource->add_ref();
		}

		resource_container<KEY>* get_resource(KEY key)
		{
			resource_map::iterator itr = m_resource_map.find(key);
			if (itr != m_resource_map.end())
			{
				itr->second->add_ref();
				return itr->second;
			}

			return NULL;
		}

		void remove_resource(KEY key)
		{
			resource_map::iterator itr = m_resource_map.find(key);
			if (itr != m_resource_map.end())
			{
				itr->second->detach();
				//AUTO_RELEASE( itr->second );
				m_resource_map.erase(itr);
			}
		}

	protected:
		resource_map m_resource_map;
	};
}

#endif // _US_RESOURCE_MANAGER_H_