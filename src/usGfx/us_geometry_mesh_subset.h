
///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2010 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_geometry_mesh_subset.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : geometry_mesh_subset class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_GEOMETRY_MESH_SUBSET_H_
#define _US_GEOMETRY_MESH_SUBSET_H_

namespace uniscope_globe
{
	class geometry_mesh_subset 
		: public mesh_subset
		, public texture_map_description
	{
	public:
		typedef std::pair<ulong, ulong> mesh_subset_info;

	public:
		geometry_mesh_subset( void );

		virtual ~geometry_mesh_subset( void );

	public:
		void create( d3d9_hardware_mesh_buffer* in_parent_mesh_buffer, int mat_index );

	public:
		virtual void lock();

		virtual void unlock();

		virtual void update( void );

	public:
		void copy_from( geometry_mesh_subset* v_src_data );

	public:
		virtual material_entry* get_material_entry( void );

	protected:
		material_entry m_material_entry;

		//mesh_subset_info m_mesh_subset_info;
	};
}

#endif // _US_GEOMETRY_MESH_SUBSET_H_ 