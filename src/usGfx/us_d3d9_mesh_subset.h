
///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2010 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_mesh_subset.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_mesh_subset class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_D3D9_MESH_SUBSET_H_
#define _US_D3D9_MESH_SUBSET_H_
#include <usGfx/Export.h>
#include <usGfx/us_mesh_subset.h>
#include <usUtil/us_common_file.h>

#include <vector>

namespace uniscope_globe
{
	struct material_entry;
	class d3d9_hardware_mesh_buffer;

	class USGFX_EXPORT d3d9_mesh_subset : public mesh_subset
	{
	public:
		d3d9_mesh_subset(void);

		virtual ~d3d9_mesh_subset(void);

	public:
		void create(d3d9_hardware_mesh_buffer* in_parent_mesh_buffer, int mat_index, std::vector<ulong>& in_tri_array);

	public:
		virtual void lock();

		virtual void unlock();

		virtual void update(void);

	public:
		virtual material_entry* get_material_entry(void);

	protected:
		void update_compact_map(void);
	};
}

#endif // _US_D3D9_MESH_SUBSET_H_ 