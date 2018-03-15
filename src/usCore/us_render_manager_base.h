///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_render_manager_base.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : render_manager_base class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_RENDER_MANAGER_BASE_H_
#define _US_RENDER_MANAGER_BASE_H_

#include <usCore\Export.h>
#include <usCore\us_render_node.h>

namespace uniscope_globe
{
	enum us_render_id
	{
		US_POST_PROCESS_RENDER_QUEUE = 0,

		US_SCENE_RENDER_QUEUE,

		US_SPHERE_RENDER_QUEUE,
		US_SPHERE_WINDOW_RENDER_QUEUE,

		US_POI_RENDER_QUEUE,
		US_VECTOR_RENDER_QUEUE,
		US_ANNOTATION_RENDER_QUEUE,
		US_GEOMETRY_RENDER_QUEUE,
		US_GEOMETRY_RENDER_BLEND_QUEUE,
		US_BILLBOARD_RENDER_QUEUE,

		US_BUILDING_RENDER_QUEUE,
		US_MATCH_MODEL_RENDER_QUEUE,
		US_SKINNED_MODEL_RENDER_QUEUE,

		US_MODEL_RENDER_QUEUE,
		US_MODEL_ALPHA_TEST_RENDER_QUEUE,
		US_MODEL_ALPHA_BLEND_RENDER_QUEUE,

		US_UNDERGROUND_MODEL_RENDER_QUEUE,
		US_UNDERGROUND_MODEL_ALPHA_TEST_RENDER_QUEUE,
		US_UNDERGROUND_MODEL_ALPHA_BLEND_RENDER_QUEUE,

		US_UNDERGROUND_GEOMETRY_RENDER_QUEUE,
		US_UNDERGROUND_GEOMETRY_RENDER_BLEND_QUEUE,

		US_SKY_RENDER,
		US_ATMOSPHERE_RENDER,
		US_RASTER_RENDER_QUEUE,

		US_REFLECTION_RENDER,

		US_POI_Z_DISABLE_RENDER_QUEUE,

		US_RENDER_COUNT
	};

	class render_argument;

	class USCORE_EXPORT render_manager_base : public render_node
	{
		typedef std::vector<render_node*> render_node_array;
	public:
		render_manager_base(void);

		virtual ~render_manager_base(void);

	public:
		virtual bool initialize(render_device* v_device) { return true; }

		virtual bool finalize(void) { return true; }

		virtual void flush(void) {}

		virtual void draw(render_argument* args) {}

		virtual void occlusion(render_argument* args) {}

		virtual render_node* get_render(long render_handler) { return m_render_array[render_handler]; }

		virtual void set_render(long render_handler, render_node* v_render) { m_render_array[render_handler] = v_render; }

	public:
		render_node* get_post_processs_render(void) { return m_render_array[US_POST_PROCESS_RENDER_QUEUE]; }

		render_node* get_scene_render(void) { return m_render_array[US_SCENE_RENDER_QUEUE]; }

	public:
		render_node* get_sphere_render(void) { return m_render_array[US_SPHERE_RENDER_QUEUE]; }

		render_node* get_sphere_window_render(void) { return m_render_array[US_SPHERE_WINDOW_RENDER_QUEUE]; }

	public:
		render_node* get_poi_render(void) { return m_render_array[US_POI_RENDER_QUEUE]; }

		render_node* get_poi_z_disable_render(void) { return m_render_array[US_POI_Z_DISABLE_RENDER_QUEUE]; }

		render_node* get_vector_render(void) { return m_render_array[US_VECTOR_RENDER_QUEUE]; }

		render_node* get_annotation_render(void) { return m_render_array[US_ANNOTATION_RENDER_QUEUE]; }

		render_node* get_geometry_render(void) { return m_render_array[US_GEOMETRY_RENDER_QUEUE]; }

		render_node* get_geometry_render_blend(void) { return m_render_array[US_GEOMETRY_RENDER_BLEND_QUEUE]; }

		render_node* get_billboard_render(void) { return m_render_array[US_BILLBOARD_RENDER_QUEUE]; }

	public:
		render_node* get_match_model_render(void) { return m_render_array[US_MATCH_MODEL_RENDER_QUEUE]; }

		render_node* get_building_render(void) { return m_render_array[US_BUILDING_RENDER_QUEUE]; }

		render_node* get_skinned_model_render(void) { return m_render_array[US_SKINNED_MODEL_RENDER_QUEUE]; }

	public:
		render_node* get_model_render(void) { return m_render_array[US_MODEL_RENDER_QUEUE]; }

		render_node* get_alpha_test_model_render(void) { return m_render_array[US_MODEL_ALPHA_TEST_RENDER_QUEUE]; }

		render_node* get_alpha_blend_model_render(void) { return m_render_array[US_MODEL_ALPHA_BLEND_RENDER_QUEUE]; }

	public:
		render_node* get_underground_model_render(void) { return m_render_array[US_UNDERGROUND_MODEL_RENDER_QUEUE]; }

		render_node* get_underground_alpha_test_model_render(void) { return m_render_array[US_UNDERGROUND_MODEL_ALPHA_TEST_RENDER_QUEUE]; }

		render_node* get_underground_alpha_blend_model_render(void) { return m_render_array[US_UNDERGROUND_MODEL_ALPHA_BLEND_RENDER_QUEUE]; }

		render_node* get_underground_geometry_render(void) { return m_render_array[US_UNDERGROUND_GEOMETRY_RENDER_QUEUE]; }

		render_node* get_underground_geometry_render_blend(void) { return m_render_array[US_UNDERGROUND_GEOMETRY_RENDER_BLEND_QUEUE]; }

	public:
		render_node* get_raster_render(void) { return m_render_array[US_RASTER_RENDER_QUEUE]; }

	protected:
		render_node_array	m_render_array;
		//	render_node*	m_root_render;

		//	render_node*	m_sphere_render;

		//	render_node*	m_building_render;

		//	render_node*	m_poi_render;

		//	render_node*	m_geometry_render;

		//	render_node*	m_vector_render;

		//	render_node*	m_annotation_render;

		//	render_node*	m_billboard_render;

		//	render_node*    m_match_model_render;

		//	render_node*	m_raster_render;

		//	render_node*	m_sky_render;

		//	render_node*    m_atmosphere_render;

		//	render_node* m_dynamic_common_model_render_alpha_test;

		//	render_node* m_dynamic_common_model_render_alpha_blend;

		//	render_node* m_dynamic_common_model_skinned_render;

		//	render_node* m_dynamic_skinned_model_render;

		//	render_node* m_dynamic_billboard_set_render;

		//	render_node* m_dynamic_label_render;

		//	render_node* m_edit_model_render;

		//	render_node* m_sound_render;
	};
}

#endif // _US_RENDER_MANAGER_BASE_H_