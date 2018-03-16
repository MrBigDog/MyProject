
///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_render_object_line_volume.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : render_object_line_volume class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_RENDER_OBJECT_LINE_VOLUME_H_
#define _US_RENDER_OBJECT_LINE_VOLUME_H_

#include <usGfx/Export.h>
#include <usGfx/us_d3d9_vertex_declear.h>
#include <usCore/us_render_object.h>
#include <usCore/us_shadow_volume_creator.h>
//#include <usUtil/us_common_file.h>

namespace uniscope_globe
{
	class render_object_factory;

	class USGFX_EXPORT render_object_line_volume
		: public render_object
		, public shadow_volume_creator
		//, public intersect_base
	{
	public:
		render_object_line_volume( void );

		virtual ~render_object_line_volume( void );

		friend class render_object_line_factory;

	public:
		static render_object_line_volume* create_shared_instance( void );
		// override from render_object
	public:
		virtual void draw( render_argument* args );

		// override from shadow_volume_creator
	public:
		virtual render_object* get_shadow_object( void ) { return this;  }

		virtual int get_pass1( void ) { return 5; }

		virtual int get_pass2( void ) { return 3; }

		// override from intersect_base
	public:
		virtual bool intersect( const ray<double>& a_ray, intersect_result& result );

	public:
		virtual void set_world_matrix( matrix4<double>& mat ) { m_trans_matrix = mat; }

		virtual void set_collapse_matrix( matrix_4d& mat ) { m_collapse_matrix = m_collapse_matrix * mat; }

		virtual void set_line_width( float line_width ) { m_line_width = line_width; }

		virtual float get_line_width( void ) { return m_line_width; }

		virtual void set_line_color( ulong line_color ) { m_line_color = line_color;}

		virtual ulong get_line_color( void ) { return m_line_color; }

	public:
		void clear();

	public:
		std::vector<position_normal_color>	m_vertex_array;
		std::vector<ushort>					m_index_array;

	protected:
		float		m_line_width;
		ulong		m_line_color;

		matrix_4d	m_trans_matrix;
		matrix_4d	m_collapse_matrix;
	};
}

#endif // _US_RENDER_OBJECT_LINE_VOLUME_H_ 