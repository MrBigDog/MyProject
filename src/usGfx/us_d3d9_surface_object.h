
///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_surface_object.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_surface_object class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_D3D9_SURFACE_OBJECT_H_
#define _US_D3D9_SURFACE_OBJECT_H_
#include <usGfx/Export.h>
#include <usGfx/us_d3d9_hardware_mesh_buffer.h>
#include <usCore/us_spatial_object.h>
#include <usUtil/us_common_file.h>
namespace uniscope_globe
{
	class render_argument;

	class USGFX_EXPORT d3d9_surface_object
		: public d3d9_hardware_mesh_buffer
		, public spatial_object
	{
	public:
		d3d9_surface_object(void);

		virtual ~d3d9_surface_object(void);

		void create(d3d9_hardware_mesh_buffer* in_parent_mesh_buffer, int mat_index, std::vector<ulong>& in_tri_array);

	public:
		virtual void lock();

		virtual void unlock();

	public:
		virtual void initialize(document_base* v_doc) {}

		virtual void update(void);

		virtual void draw(render_argument* args);

		virtual vector_3f get_center(void) { return m_center; }

		virtual d3d9_hardware_mesh_buffer* get_parent_mesh_buffer(void) { return m_parent_mesh_buffer; }

		virtual spatial_object* get_parent_object(void) { return m_parent_object; }

		virtual int get_material_index(void) { return m_material_index; }


	protected:
		void update_center(void);

		void update_compact_map(void);

	public:
		d3d9_hardware_mesh_buffer*		m_parent_mesh_buffer;

		spatial_object*					m_parent_object;

		std::vector<ulong>				m_face_index_array;

		std::map<ulong, ulong>			m_compact_index_map;

		uint							m_material_index;

		vector_3f						m_center;
	};
}

#endif // _US_D3D9_SURFACE_OBJECT_H_ 