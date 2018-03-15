///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_hardware_mesh.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : hardware_mesh class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_HARDWARE_MESH_H_
#define _US_HARDWARE_MESH_H_

#include <usCore\Export.h>
#include <usCore\us_hardware_base.h>
#include <usUtil\us_axis_aligned_box.h>
#include <usUtil\us_matrix4.h>
#include <usCore\us_intersect_result.h>

namespace uniscope_globe
{
	class render_argument;

	class USCORE_EXPORT hardware_mesh : public hardware_base
	{
	protected:
		hardware_mesh(void);

	public:
		hardware_mesh(raw_buffer* buffer);

		virtual ~hardware_mesh(void);

		// override from hardware_base
	public:
		virtual long create(render_device* device);

		virtual bool destroy(void);

		virtual hardware_mesh* clone(void);

		// own
	public:
		virtual void draw(render_argument* args);

		virtual void draw_subset(render_argument* args, int v_material_index);

		virtual void update_bound_box(void) {}

		axis_aligned_box<double>& get_bound_box(void);

		void set_bound_box(axis_aligned_box<double>& v_aabb) { m_bound_box = v_aabb; }

		virtual bool intersect(const ray<double>& in_ray, intersect_result& out_result);

		bool is_texture_ready(void) { return m_is_texture_ready; }

	public:
		void set_visiable(bool in_visiable) { m_visiable = in_visiable; }

		bool get_visiable(void) { return m_visiable; }

		virtual vector3<double> get_lowest_point(matrix4<double>& in_mat) { return vector3<double>::s_zero; }

		virtual void generate_topological_data(void) {}

	protected:

		axis_aligned_box<double> m_bound_box;

		bool m_visiable;

		bool m_is_texture_ready;

	public:
		bool m_use_material;
	};
}

#endif // _US_HARDWARE_MESH_H_