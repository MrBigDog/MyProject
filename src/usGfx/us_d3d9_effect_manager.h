///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_effect_manager.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_effect_manager class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_D3D9_EFFECT_MANAGER_H_
#define _US_D3D9_EFFECT_MANAGER_H_

namespace uniscope_globe
{



//typedef enum tagEffectType
//{
//
//};
#define US_EFFECT_VERTEX_BLEND				0
#define US_EFFECT_SKY_SPHERE				1
#define US_EFFECT_TERRAIN_RENDER			2
#define	US_EFFECT_REFLECTION_SURFACE		3
#define US_EFFECT_VERTEX_BLEND_EX			4
#define US_EFFECT_SHADOW_VOLUME				5
#define US_EFFECT_COMMON_MESH_FP			6
#define US_EFFECT_GEOMETRY_LINE_FLAKE		7
#define US_EFFECT_GEOMETRY_LINE_VOLUME		8
#define US_EFFECT_PP_COMBINE_HDR			9
#define US_EFFECT_PP_DOWN_FILTER_4X			10
#define US_EFFECT_PP_UP_FILTER_4X			11
#define US_EFFECT_PP_GAUSSIAN_BLUR_H		12
#define US_EFFECT_PP_GAUSSIAN_BLUR_V		13
#define US_EFFECT_PP_COMBINE_DOF			14
#define US_EFFECT_PP_TONE_MAP				15
#define US_EFFECT_PP_COMBINE_HSL			16
#define US_EFFECT_COMMON_MESH_GPU			17
#define US_EFFECT_REFRACTION_SURFACE		18
#define US_EFFECT_FLOW_DIR					19
#define US_EFFECT_FLOOD_ANALYSIS			20
#define US_EFFECT_COUNT						21


	class d3d9_effect_manager
	{
	protected:
		typedef std::vector<d3d9_effect_base*> render_effect_array;

	protected:
		d3d9_effect_manager( void ) {}

	public:
		d3d9_effect_manager( render_device* rd );

		virtual ~d3d9_effect_manager( void );

	public:
		ULONG_PTR get_effect( long effect_handle );

		long on_lost_device( void );

		long on_reset_device( void );

	protected:
		render_effect_array m_effect_array;

		 
	};
}

#endif // _US_D3D9_EFFECT_MANAGER_H_