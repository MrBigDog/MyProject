///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_common_manager_group.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : common_manager_group class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_COMMON_MANAGER_GROUP_H_
#define _US_COMMON_MANAGER_GROUP_H_

#include <usCore\Export.h>
#include <usCore\us_tls_singleton.h>
#include <usUtil\us_common_file.h>
#include <unordered_map>

namespace uniscope_globe
{
	class mesh_creator;
	class texture_creator;
	class hardware_creator;
	class object_base;
	template<typename KEY> class resource_manager;
	class document_base;

	class USCORE_EXPORT common_manager_group
	{
	protected:
		common_manager_group();

	public:
		common_manager_group(document_base* v_document);
		virtual ~common_manager_group(void);

#if _MSC_VER >= 1900
		typedef  std::unordered_map<ustring, resource_manager<ustring>*> manager_map;
		typedef  std::unordered_map<ustring, resource_manager<interior_index>*> index_manager_map;
		typedef  std::unordered_map<ustring, object_base*> object_map;
#else
		typedef  stdext::hash_map<ustring, resource_manager<ustring>*> manager_map;
		typedef  stdext::hash_map<ustring, resource_manager<interior_index>*> index_manager_map;
		typedef  stdext::hash_map<ustring, object_base*> object_map;
#endif


		// object
	public:
		object_base* get_object(const LPCTSTR str_key);
		void register_object(const LPCTSTR str_key, object_base* object);

		// container_manager
	public:
		resource_manager<ustring>* get_texture_manager(const LPCTSTR str_key);

		resource_manager<ustring>* get_mesh_manager(const LPCTSTR str_key);

		resource_manager<interior_index>* get_index_texture_manager(const LPCTSTR str_key);

		resource_manager<interior_index>* get_index_mesh_manager(const LPCTSTR str_key);

		mesh_creator* get_mesh_creator(void) { return m_mesh_creator; }

		texture_creator* get_texture_creator(void) { return m_texture_creator; }

		hardware_creator* get_hardware_creator(void) { return m_hardware_creator; }

		document_base* get_document(void) { return m_document; }

		void clear(void);

	protected:
		manager_map			m_texture_mngr_map;

		manager_map			m_mesh_mngr_map;

		index_manager_map	m_index_texture_mngr_map;

		index_manager_map	m_index_mesh_mngr_map;

		object_map			m_object_map;

		mesh_creator*		m_mesh_creator;

		texture_creator*    m_texture_creator;

		hardware_creator*	m_hardware_creator;

		document_base*		m_document;
	};

	class USCORE_EXPORT singleton_common_manager_group
		: public tls_singleton<common_manager_group>
	{
	};

}

#endif // _US_COMMON_MANAGER_GROUP_H_