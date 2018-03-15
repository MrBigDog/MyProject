///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_geometry_model.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : geometry_model class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_GEOMETRY_MODEL_H_
#define _US_GEOMETRY_MODEL_H_

namespace uniscope_globe
{
	class LIB_GFX_API geometry_render_model
		: public render_object
	{
	protected:
		geometry_render_model( void ) { }

	public:
		geometry_render_model( d3d9_hardware_mesh* v_mesh );

		~geometry_render_model( void );

	public:
		void set_render_mesh( hardware_mesh* v_mesh );

		void set_matrix( matrix_4d& v_mat );

		void update( void );

	// override from render_object
	public:
		virtual void draw( render_argument* args );

	public:
		matrix_4d m_mat;

		//int m_texture_level;

		d3d9_hardware_mesh* m_mesh;

		//render_texture_container_array m_texture_array;

	};


	class LIB_GFX_API geometry_model
		: public common_resource_container<ustring>
	{
	protected:
		geometry_model( void ){}

	public:
		geometry_model( LPCTSTR str_link );

		virtual ~geometry_model( void );

	public:
		// override from reclaim_base
		virtual void destroy( void )
		{
			m_mission_flow->reset();

			DEFERRED_RELEASE( m_resource );
			AUTO_DELETE( m_render_model );

			reclaim_base::destroy();
		}


	public:		
		geometry_render_model* get_render_model( void );

		bool intersect( const ray<double>& a_ray, intersect_result& result );

		// download mission
	protected:
		virtual void on_download_finish( event_argument* args );

	protected:
		ustring m_link;

		geometry_render_model* m_render_model;
	};

}

#endif // _US_GEOMETRY_RENDER_OBJECT_H_