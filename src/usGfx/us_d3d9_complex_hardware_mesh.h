///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_complex_hardware_mesh.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_complex_hardware_mesh class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_D3D9_COMPLEX_HARDWARE_MESH_H_
#define _US_D3D9_COMPLEX_HARDWARE_MESH_H_

namespace uniscope_globe
{
	class simple_animation_player		
	{
	protected:
		simple_animation_player(){}

	public:
		simple_animation_player( usx_animation_set* in_anim_set );

		virtual ~simple_animation_player(void);

		void attach( usx_frame* in_frame );

		virtual void update( void );

	protected:
		usx_animation_set* m_animation_set;

		ulong m_current_time;

		ulong m_last_time_tick;
	};

	class d3d9_complex_hardware_mesh
		: public hardware_mesh
	{
	public:
		typedef std::map<ustring, simple_animation_player*> animation_player_map;

		typedef std::map<d3d9_hardware_mesh*, usx_frame*> mesh_frame_map;

		typedef std::vector<d3d9_hardware_mesh*> mesh_array;

		friend class hardware_mesh_factory;

	public:
		d3d9_complex_hardware_mesh( void );

	//public:
	//	d3d9_complex_hardware_mesh( hardware_mesh_buffer_map& in_buffer_map, usx_frame* in_root_frame, usx_animation_set_array& in_animation_set_array );

		virtual ~d3d9_complex_hardware_mesh( void );

	public:
		virtual long create( render_device* device );

		virtual bool destroy( void );

		virtual bool refresh( void );

		virtual void draw( render_argument* args );

		virtual bool intersect( const ray<double>& in_ray, intersect_result& out_result );

	public:
		virtual hardware_mesh* clone( void );

		virtual void update_bound_box( void );

		//virtual void reset( render_device* device );

		virtual vector3<double> get_lowest_point( matrix4<double>& in_mat );

		virtual void generate_topological_data( void );

		// own
	public:
		usx_frame* get_root_frame( void ) { return m_root_frame; }

		void set_root_frame( usx_frame* v_root_frame ) { m_root_frame = v_root_frame; }

		ulong get_fvf( void ) { return m_fvf; }

		void set_fvf( ulong v_fvf ) { m_fvf = v_fvf; }

	protected:
		void update_animation( void );

		void add_mesh( d3d9_hardware_mesh* v_mesh, usx_frame* v_frame = NULL );

		hardware_mesh* get_mesh( int index );

	public:
		usx_frame* m_root_frame;
		
		mesh_frame_map m_mesh_frame_map;

		animation_player_map m_animation_player_map;

		mesh_array m_mesh_array;

		ulong m_fvf;
	};
}

#endif // _US_D3D9_COMPLEX_HARDWARE_MESH_H_