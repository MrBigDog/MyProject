
///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_generic_render_queue.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : generic_render_queue class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_generic_render_queue.h"

namespace uniscope_globe
{
	generic_render_queue::generic_render_queue( void )
	{
		m_current_render_array = new render_object_array;

		m_background_render_array = new render_object_array;

		m_qid = US_DEFAULT_RENDER_NODE;
	}

	generic_render_queue::~generic_render_queue( void )
	{
		clear();
	}

	void generic_render_queue::occlusion( render_argument* args )
	{
		for ( render_object_array::iterator itr = m_current_render_array->begin(); itr!= m_current_render_array->end(); itr++ )
		{
			(*itr)->draw( args );
		}

		for ( render_object_array::iterator itr = m_current_render_array->begin(); itr!= m_current_render_array->end(); itr++ )
		{
			(*itr)->occlusion( args );
		}
	}

	void generic_render_queue::draw( render_argument* args )
	{
		
		args->m_render_flag |= US_MATERIAL_TYPE_NORMAL | US_MATERIAL_TYPE_DYNAMIC;
		{
			US_LOCK_AUTO_MUTEX;

			// set cur render state
			render_state v_render_state( args->m_device );
			//v_render_state.set_state( D3DRS_CULLMODE, D3DCULL_CCW );
			//v_render_state.set_state( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
			if( system_environment::s_all_model_cull_none )
			{
				v_render_state.set_state( D3DRS_CULLMODE, D3DCULL_NONE );
			}

			v_render_state.set_state( D3DRS_ALPHATESTENABLE, TRUE );
			v_render_state.set_state( D3DRS_ALPHAFUNC, D3DCMP_GREATER );
			v_render_state.set_state( D3DRS_ALPHAREF, 15);

			v_render_state.set_state( D3DRS_ALPHABLENDENABLE, TRUE );
			v_render_state.set_state( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
			v_render_state.set_state( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

			//float slopeScaleDepthBias = 0.0f;
			//v_render_state.set_state( D3DRS_SLOPESCALEDEPTHBIAS, *((DWORD*)&slopeScaleDepthBias) );

			texture_state v_ts( args->m_device );
			v_ts.set_state( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
			v_ts.set_state( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
			v_ts.set_state( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );

			v_ts.set_state( 1, D3DTSS_COLOROP, D3DTOP_MODULATE );
			v_ts.set_state( 1, D3DTSS_COLORARG1, D3DTA_TEXTURE );
			v_ts.set_state( 1, D3DTSS_COLORARG2, D3DTA_CURRENT );

			v_ts.set_state( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1 );
			v_ts.set_state( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
			v_ts.set_state( 0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR );

			v_ts.set_state( 1, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
			v_ts.set_state( 1, D3DTSS_ALPHAARG1, D3DTA_TFACTOR );
			v_ts.set_state( 1, D3DTSS_ALPHAARG2, D3DTA_CURRENT );

			render_object_array::iterator itr = m_current_render_array->begin();
			for( ; itr != m_current_render_array->end(); itr ++ )
			{

				if ( (*itr)->get_blend_color() != 0x00000000 )
				{
					render_state v_child_render_state( args->m_device );
					v_child_render_state.set_state( D3DRS_TEXTUREFACTOR, (*itr)->get_blend_color() );	

					texture_state v_child_ts( args->m_device );
					v_child_ts.set_state( 0, D3DTSS_COLORARG2, D3DTA_TFACTOR );

					(*itr)->draw( args );
				}
				else
				{
					(*itr)->draw( args );
				}
				
			}
		}
		render_node::draw( args );
	}

	void generic_render_queue::draw_alpha( render_argument* args )
	{
		draw_water( args );
	}

	void generic_render_queue::draw_water( render_argument* args )
	{
		US_LOCK_AUTO_MUTEX;

		args->m_render_flag |= US_MATERIAL_TYPE_WATER;

		render_object_array::iterator itr = m_current_render_array->begin();
		for( ; itr != m_current_render_array->end(); itr ++ )
		{
			(*itr)->draw( args );
		}
		args->m_render_flag ^= US_MATERIAL_TYPE_WATER;

	}

	void generic_render_queue::flush( void )
	{
		//US_RENDER_TRACE1( L"generic ro : %d\n", (int)m_background_render_array->size() );

		// begin flip render_object
		{
			US_LOCK_AUTO_MUTEX;

			render_object_array* temp_array = m_current_render_array;
			m_current_render_array = m_background_render_array;
			m_background_render_array = temp_array;

		}
		clear_background();

		render_node::flush();
	}

	void generic_render_queue::push( render_object* in_object )
	{
		in_object->add_ref();
		m_background_render_array->push_back( in_object );
	}

	void generic_render_queue::clear_background( void )
	{
		render_object_array::iterator itr = m_background_render_array->begin();
		for( ; itr != m_background_render_array->end(); itr ++ )
		{
			AUTO_RELEASE_SHARED_DATA( *itr );
		}

		m_background_render_array->clear();
	}

	void generic_render_queue::clear( void )
	{
		render_object_array::iterator itr = m_current_render_array->begin();
		for( ; itr != m_current_render_array->end(); itr ++ )
		{
			AUTO_RELEASE_SHARED_DATA( *itr );
		}
		AUTO_DELETE( m_current_render_array );

		itr = m_background_render_array->begin();
		for( ; itr != m_background_render_array->end(); itr ++ )
		{
			AUTO_RELEASE_SHARED_DATA( *itr );
		}

		AUTO_DELETE( m_background_render_array );
	}

	int generic_render_queue::get_count( void ) 
	{ 
		return m_current_render_array->size();
	}

	bool generic_render_queue::check_intersectable( spatial_object* v_so)
	{
		if(v_so == NULL) return true;
		if( v_so->get_object_flags() == US_OBJECT_FLAGS_DUMMY_OBJECT)
		{
			return v_so->is_intersectable();
		}
		else
		{
			return (v_so->is_visible() && v_so->is_intersectable());
		}
	}

	bool generic_render_queue::intersect( const ray<double>& in_ray, intersect_result& out_result )
	{
		bool v_intersected = false;

		US_LOCK_AUTO_MUTEX

		for ( int ni = 0; ni < m_current_render_array->size(); ni++ )
		{
			render_object* v_ro = (*m_current_render_array)[ni];

			if( !this->check_intersectable(dynamic_cast<spatial_object*>(v_ro))) continue;

			v_intersected |= v_ro->intersect( in_ray, out_result );
		}

		return v_intersected;
	}

	bool generic_render_queue::intersect(  const ray<double>& in_ray, double v_range, intersect_result& out_result  )
	{
		bool v_intersected = false;

		US_LOCK_AUTO_MUTEX

		for ( int ni = 0; ni < m_current_render_array->size(); ni++ )
		{
			render_object* v_ro = (*m_current_render_array)[ni];
			if( !this->check_intersectable(dynamic_cast<spatial_object*>(v_ro))) continue;
			v_intersected |= v_ro->intersect( in_ray, v_range, out_result );
		}

		return v_intersected;
	}
	
}