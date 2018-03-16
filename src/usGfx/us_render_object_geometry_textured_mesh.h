
///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_render_object_geometry_textured_mesh.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : render_object_geometry_textured_mesh class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_RENDER_OBJECT_GEOMETRY_TEXTURED_MESH_H_
#define _US_RENDER_OBJECT_GEOMETRY_TEXTURED_MESH_H_

#include <usGfx/Export.h>
#include <usGfx/us_d3d9_hardware_mesh_buffer.h>
#include <usGfx/us_d3d9_material_declear.h>
#include <usCore/us_render_object.h>
#include <usCore/us_hardware_mesh.h>
#include <usCore/us_shadow_volume_creator.h>
#include <usCore/us_cartesian_object.h>
#include <usCore/us_resource_manager.h>
#include <usUtil/us_common_file.h>

namespace uniscope_globe
{
	class render_object_factory;
	class geometry_mesh_subset;
	class spherical_object;

	class USGFX_EXPORT render_object_geometry_textured_mesh
		: public render_object
		, public hardware_mesh
		, public d3d9_hardware_mesh_buffer
		, public shadow_volume_creator
		, public cartesian_object
	{
	public:
		typedef std::pair<ulong, ulong> mesh_subset_info;

		typedef std::vector<mesh_subset_info> mesh_subset_info_array;

		typedef std::vector<geometry_mesh_subset*> material_entry_array;

	public:
		render_object_geometry_textured_mesh(void);

		render_object_geometry_textured_mesh(spherical_object* v_parent_object, int material_count);

		virtual ~render_object_geometry_textured_mesh(void);

	public:
		static render_object_geometry_textured_mesh* create_shared_instance(spherical_object* v_parent_object, int material_count);

		// override from render_object
	public:
		virtual void initialize(document_base* v_doc) { m_document = v_doc; }

		virtual void draw(render_argument* args);

		virtual void copy_from(render_object_geometry_textured_mesh* v_mesh);

		virtual void draw_subset(render_argument* args, int v_material_index);

		virtual void set_world_matrix(matrix4<double>& v_mat) { m_trans_matrix = v_mat; }

		virtual matrix4<double> get_world_matrix(void) { return m_trans_matrix; }

		// override from shadow_volume_creator
	public:
		virtual render_object* get_shadow_object(void);

		// override from intersect_base
	public:
		virtual bool intersect(const ray<double>& a_ray, intersect_result& result);

	public:
		virtual bool intersect_material(const ray<double>& a_ray, intersect_result& result);

	public:
		virtual void clear_buffer(void);

	public:
		//void draw_figure( render_device* args, matrix_4f& v_mat );

		void clear_material();

		void set_collapse(matrix_4d& mat);

		bool refresh();

	public:
		resource_manager<ustring>* get_texture_manager(void) { return m_texture_manager; }

		void set_texture_manager(resource_manager<ustring>* v_texture_manager) { m_texture_manager = v_texture_manager; }

		resource_container<ustring>* get_texture_containner(const ustring& newVal);
	public:
		resource_manager<ustring>*		m_texture_manager;

		material_entry_array			m_material_entry_array;

		render_texture_lod_entry_array	m_texture_array;

		mesh_subset_info_array			m_subset_info_array;

	public:
		matrix_4d					m_trans_matrix;

		matrix_4d					m_collapse_matrix;

		vector3<double>				m_center;

		ustring	m_roof_name;
		//protected:
		//	spherical_object*			m_parent_object;

		//	// figure
		//public:
		//	std::vector<position_color> m_frame_vertex_array;

		//	std::vector<ushort>			m_frame_index_array;
		double m_offset_distance;

	};


}

#endif // _US_RENDER_OBJECT_GEOMETRY_TEXTURED_MESH_H_ 