///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_document_base.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : document_base class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_DOCUMENT_BASE_H_
#define _US_DOCUMENT_BASE_H_
#include <usCore\Export.h>
namespace uniscope_globe
{
#define US_INTERSECT_TERRAIN		0x0001
#define US_INTERSECT_BUILDING		0x0002
#define US_INTERSECT_BLOCK			0x0004
#define US_INTERSECT_MATCH_MODEL	0x0008
#define US_INTERSECT_ELEMENT		0x0010

#define	US_INTERSECT_ALL_WITHOUT_TERRAIN	0x001e
#define	US_INTERSECT_ALL_OBJECT				0x001f
	class USCORE_EXPORT document_base: public object_base
	{
	protected:
		document_base( void );

	public:
		document_base( application_base* in_app, view_base* in_view );

		virtual ~document_base( void );

#if _MSC_VER >= 1900
		typedef std::unordered_map< ustring, spatial_object* > object_map;
#else
		typedef stdext::hash_map< ustring, spatial_object* > object_map;
#endif



	public:
		virtual bool initialize( void ){ return true; }

		virtual bool finalize( void ){ return true; }

	public:
		virtual render_manager_base* get_render_manager( void ){ return m_view->get_render_manager(); }

		virtual common_manager_group* get_common_manager_group( void ) { return m_common_manager_group; }
		virtual void set_common_manager_group(common_manager_group* v_group) { m_common_manager_group = v_group; }

		virtual resource_reclaimer* get_resource_reclaimer() { return m_resource_reclaimer; }

		virtual frame_counter* get_frame_counter() { return m_frame_counter; }

	public:
		static document_base* ptr( void ) { return s_document_ptr; }

		static document_base& instance( void ) { return *s_document_ptr; }

		// object map
	public:
		bool register_object( spatial_object* in_object );

		bool unregister_object( spatial_object* in_object );

		spatial_object* get_register_object( cpstr id );

		void delete_object( cpstr id );

	public:
		virtual bool get_dem_height( vector3<double>& in_pos_sphr, double& v_out_height ) = 0;

		virtual bool intersect( ray<double>& in_ray, intersect_result& out_result, ulong v_flags = US_INTERSECT_ALL_OBJECT  ) = 0;

		virtual bool intersect( ray<double>& in_ray, double in_range, intersect_result& out_vec, ulong v_flags = US_INTERSECT_ALL_OBJECT ) = 0;

		virtual bool pick_vector( point& pt, vector3<double>& out_vec, ulong v_flags = US_INTERSECT_ALL_OBJECT ) = 0;

		virtual bool pick_vector( ray<double>& in_ray, vector3<double>& out_vec, ulong v_flags = US_INTERSECT_ALL_OBJECT  ) = 0;

		virtual bool pick_vector( ray<double>& in_ray, double in_range, vector3<double>& out_vec, ulong v_flags = US_INTERSECT_ALL_OBJECT ) = 0;

		virtual bool pick_object( point& pt, intersect_result& out_result) = 0;

		virtual bool pick_object( lon_lat_rect& v_rect, intersect_result& out_result ) = 0;

		virtual bool pick_object( rect& in_rect, intersect_result& out_result ) = 0;

		virtual ustring select_spherical_object_by_rectangle( lon_lat_rect& select_rect ) = 0;

	public:
		virtual vector_3d get_pick_terrain_position_geo( void ) { return m_pick_terrain_positon_geo; }

	public:
		view_base* get_view( void ) { return m_view; }

		application_base* get_application( void ) { return m_application; }

		void set_map_render_mode( bool enable ) { m_map_render_mode = enable;}

		bool get_map_render_mode( void ) { return m_map_render_mode;}

	public:
		__declspec(thread) static document_base* s_document_ptr;

		object_map m_object_map;

		// tls singleton objects
		resource_reclaimer* m_resource_reclaimer;

		frame_counter*		m_frame_counter;

	protected:
		application_base* m_application;

		view_base* m_view; 

		common_manager_group* m_common_manager_group;
	
		US_AUTO_MUTEX

		vector_3d m_pick_terrain_positon_geo;

		bool m_map_render_mode;
	};
}

#endif // _US_DOCUMENT_BASE_H_