///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_vertex_declear.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose :  
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_D3D9_VERTEX_DECLEAR_H_
#define _US_D3D9_VERTEX_DECLEAR_H_

//////////////////////////////////////////////////////////////////////////
// custom_vertex
//////////////////////////////////////////////////////////////////////////
namespace uniscope_globe
{
	//////////////////////////////////////////////////////////////////////////
	// Position Only
	//////////////////////////////////////////////////////////////////////////
	struct position_only
	{
		float x, y, z;
		static const ulong fvf;
		static const ulong stride;
	};

	//	const ulong position_only::fvf = D3DFVF_XYZ;
	//	const ulong position_only::stride = 12;

	//////////////////////////////////////////////////////////////////////////
	// Position Color
	//////////////////////////////////////////////////////////////////////////
	struct position_color
	{
		float x, y, z;
		ulong color;
		static const ulong fvf;
		static const ulong stride;
	};

//	const ulong position_color::fvf = D3DFVF_XYZ | D3DFVF_DIFFUSE;
//	const ulong position_color::stride = 16;

	//////////////////////////////////////////////////////////////////////////
	// Position Normal
	//////////////////////////////////////////////////////////////////////////
	struct position_normal
	{
		float x, y, z;
		float nx, ny, nz;
		static const ulong fvf;
		static const ulong stride;
	};
	
	//	const ulong position_normal::fvf = D3DFVF_XYZ | D3DFVF_NORMAL;
	//	const ulong position_normal::stride = 24;

	//////////////////////////////////////////////////////////////////////////
	// Position Color Texture
	//////////////////////////////////////////////////////////////////////////
	struct position_color_texture
	{
		float x, y, z;
		ulong color;
		float tu1, tv1;
		static const ulong fvf;
		static const ulong stride;
	};

//	const ulong position_color_texture::fvf = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;
//	const ulong position_color_texture::stride = 24;


	//////////////////////////////////////////////////////////////////////////
	// Position Color Texture2
	//////////////////////////////////////////////////////////////////////////
	struct position_color_texture2
	{
		float x, y, z;
		ulong color;
		float tu1, tv1;
		float tu2, tv2;
		static const ulong fvf;
		static const ulong stride;
	};

//	const ulong position_color_texture2::fvf = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX2;
//	const ulong position_color_texture2::stride = 32;


	//////////////////////////////////////////////////////////////////////////
	// Position Texture
	//////////////////////////////////////////////////////////////////////////
	struct position_texture
	{
		float x, y, z;
		float tu1, tv1;
		static const ulong fvf;
		static const ulong stride;
	};

//	const ulong position_texture::fvf = D3DFVF_XYZ | D3DFVF_TEX1;
//	const ulong position_texture::stride = 20;

	//////////////////////////////////////////////////////////////////////////
	// Position Texture2
	//////////////////////////////////////////////////////////////////////////
	struct position_texture2
	{
		float x, y, z;
		float tu1, tv1;
		float tu2, tv2;
		static const ulong fvf;
		static const ulong stride;
	};

//	const ulong position_texture2::fvf = D3DFVF_XYZ | D3DFVF_TEX2;
//	const ulong position_texture2::stride = 28;

	//////////////////////////////////////////////////////////////////////////
	// Position Texture3
	//////////////////////////////////////////////////////////////////////////
	struct position_texture3
	{
		float x, y, z;
		float tu1, tv1;
		float tu2, tv2;
		float tu3, tv3;
		static const ulong fvf;
		static const ulong stride;
	};

	//	const ulong position_texture3::fvf = D3DFVF_XYZ | D3DFVF_TEX3;
	//	const ulong position_texture3::stride = 36;

	//////////////////////////////////////////////////////////////////////////
	// Position Texture4
	//////////////////////////////////////////////////////////////////////////
	struct position_texture4
	{
		float x, y, z;
		float tu1, tv1;
		float tu2, tv2;
		float tu3, tv3;
		float tu4, tv4;
		static const ulong fvf;
		static const ulong stride;
	};

	//	const ulong position_texture4::fvf = D3DFVF_XYZ | D3DFVF_TEX4;
	//	const ulong position_texture4::stride = 44;

	//////////////////////////////////////////////////////////////////////////
	// Position Weights Bone Texture
	//////////////////////////////////////////////////////////////////////////
	struct position_weights4_bone_texture
	{
		float x, y, z;
		float weight[4];
		DWORD bone_index;
		float tu1, tv1;
		static const ulong fvf;
		static const ulong stride;
	};

	struct position_weights3_bone_normal_texture
	{
		float x, y, z;
		float weight1;
		float weight2;
		DWORD bone_index;
		float n1, n2, n3;
		float tu1, tv1;
		static const ulong fvf;
		static const ulong stride;
	};

	//////////////////////////////////////////////////////////////////////////
	// Position Normal Texture
	//////////////////////////////////////////////////////////////////////////
	struct position_normal_texture
	{
		float		x,y,z;
		float       nx,ny,nz;
		float       tu1, tv1;
		static const ulong fvf;
		static const ulong stride;
	};

//	const ulong position_normal_texture::fvf = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
//	const ulong position_normal_texture::stride = 32;

	//////////////////////////////////////////////////////////////////////////
	// Position Normal Texture2
	//////////////////////////////////////////////////////////////////////////
	struct position_normal_texture2
	{
		float		x,y,z;
		float       nx,ny,nz;
		float       tu1, tv1;
		float       tu2, tv2;
		static const ulong fvf;
		static const ulong stride;
	};

//	const ulong position_normal_texture2::fvf = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX2;
//	const ulong position_normal_texture2::stride = 40;

	//////////////////////////////////////////////////////////////////////////
	// Position Normal Color
	//////////////////////////////////////////////////////////////////////////
	struct position_normal_color
	{
		float		x,y,z;
		float		nx,ny,nz;
		ulong		color;
		static const ulong fvf;
		static const ulong stride;
	};

//	const ulong position_normal_color::fvf = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE;
//	const ulong position_normal_color::stride = 28;


	//////////////////////////////////////////////////////////////////////////
	// Position Normal Color Texture
	//////////////////////////////////////////////////////////////////////////
	struct position_normal_color_texture
	{
		float		x,y,z;
		float       nx,ny,nz;
		ulong		color;
		float       tu1, tv1;
		static const ulong fvf;
		static const ulong stride;
	};

//	const ulong position_normal_color_texture::fvf = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1;
//	const ulong position_normal_color_texture::stride = 36;

	//////////////////////////////////////////////////////////////////////////
	// Position Normal Color Texture2
	//////////////////////////////////////////////////////////////////////////
	struct position_normal_color_texture2
	{
		float		x,y,z;
		float       nx,ny,nz;
		ulong		color;
		float       tu1, tv1;
		float       tu2, tv2;
		static const ulong fvf;
		static const ulong stride;
	};

//	const ulong position_normal_color_texture2::fvf = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX2;
//	const ulong position_normal_color_texture2::stride = 44;

	//////////////////////////////////////////////////////////////////////////
	// Transformed Position Color
	//////////////////////////////////////////////////////////////////////////
	struct transformed_color
	{
		float x, y, z, rhw;
		ulong color;
		static const ulong fvf;
		static const ulong stride;
	};

//	const ulong transformed_color::fvf = D3DFVF_XYZRHW | D3DFVF_DIFFUSE;
//	const ulong transformed_color::stride = 20;

	//////////////////////////////////////////////////////////////////////////
	// Transformed Position Texture
	//////////////////////////////////////////////////////////////////////////
	struct transformed_texture
	{
		float x, y, z, rhw;
		float tu1, tv1;
		static const ulong fvf;
		static const ulong stride;
	};

//	const ulong transformed_texture::fvf = D3DFVF_XYZRHW | D3DFVF_TEX1;
//	const ulong transformed_texture::stride = 24;

	//////////////////////////////////////////////////////////////////////////
	// Transformed Position Color Texture
	//////////////////////////////////////////////////////////////////////////
	struct transformed_color_texture
	{
		float x, y, z, rhw;
		ulong color;
		float tu1, tv1;
		static const ulong fvf;
		static const ulong stride;
	};

//	const ulong transformed_color_texture::fvf = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;
//	const ulong transformed_color_texture::stride = 28;

	class degenerate_quad
	{
	public:
		degenerate_quad()
		{
			m_v1 = m_v2 = m_v3 = m_v4 = 0xffffffff;
		}
	public:
		ulong m_v1;
		ulong m_v2;
		ulong m_v3;
		ulong m_v4;
	};
}

#endif // _US_D3D9_VERTEX_DECLEAR_H_