///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_hardware_mesh.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_hardware_mesh class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_D3D9_HARDWARE_MESH_H_
#define _US_D3D9_HARDWARE_MESH_H_

#include <usGfx/Export.h>
#include <usGfx/us_d3d9_hardware_mesh_buffer.h>
#include <usGfx/us_d3d9_material_declear.h>
#include <usGfx/us_d3d9_mesh_subset.h>
#include <usUtil/us_common_file.h>
#include <usCore/us_resource_manager.h>
#include <usCore/us_hardware_mesh.h>

#include <d3dx9mesh.h>
#include <map>

namespace uniscope_globe
{
	class render_device;
	//class d3d9_mesh_subset;

	class USGFX_EXPORT mesh_topology
	{
	public:
		virtual ~mesh_topology(void)
		{
			std::map<ulong, d3d9_mesh_subset*>::iterator it = m_mesh_subset_map.begin();
			for (; it != m_mesh_subset_map.end(); it++)
			{
				AUTO_DELETE(it->second);
			}

			m_mesh_subset_map.clear();
		}

	public:
		std::map<ulong, d3d9_mesh_subset*> m_mesh_subset_map;
	};

	class USGFX_EXPORT d3d9_hardware_mesh
		: public hardware_mesh
		, public d3d9_hardware_mesh_buffer
	{
	public:
		d3d9_hardware_mesh(void);

		virtual ~d3d9_hardware_mesh(void);

		// override d3d9_hardware_mesh_buffer
	public:
		virtual void clear_buffer(void);

		virtual void lock();

		virtual void unlock();

		virtual void generate_adjacency(float v_eclipse, ulong* v_adjacency);

		virtual void generate_point_rep(ulong* v_adj_info, ulong* v_point_rep);

		// override hardware_mesh
	public:
		virtual long create(render_device* device);

		virtual bool destroy(void);

		virtual bool refresh(void);

		virtual void draw(render_argument* args);

		virtual void draw_subset(render_argument* args, int material_index);

		virtual bool intersect(const ray<double>& in_ray, intersect_result& out_result);

	public:
		virtual hardware_mesh* clone(void);

		virtual void update_bound_box(void);

		//virtual void reset( render_device* device );

		virtual vector3<double> get_lowest_point(matrix4<double>& in_mat);

		virtual void generate_topological_data(void);

		mesh_topology * get_topological_data(void) { return m_topology; }

	protected:
		virtual void calculate_topological_data(void);

	public:
		virtual ULONG_PTR get_handle(void) { return (ULONG_PTR)m_d3d_mesh; }



	public:
		resource_manager<ustring>* get_texture_manager(void) { return m_texture_manager; }

		void set_texture_manager(resource_manager<ustring>* v_texture_manager) { m_texture_manager = v_texture_manager; }

	public:
		LPD3DXMESH						m_d3d_mesh;

	public:
		resource_manager<ustring>*		m_texture_manager;

		material_entry_array			m_material_entry_array;

	public:
		render_texture_lod_entry_array	m_texture_array;

		mesh_topology*					m_topology;

		//bool							m_use_material;

		//US_AUTO_MUTEX
	};
}

#endif // _US_D3D9_HARDWARE_MESH_H_