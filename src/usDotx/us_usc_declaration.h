//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_usc_declaration.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : usx_data_struct class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_USC_DECLARATION_H_
#define _US_USC_DECLARATION_H_

#include <usUtil/us_common_file.h>
#include <map>
#include <vector>

namespace uniscope_globe
{
	// Flexible vertex format bits
	//
#define USFVF_RESERVED0        0x001
#define USFVF_POSITION_MASK    0x400E
#define USFVF_XYZ              0x002
#define USFVF_XYZRHW           0x004
#define USFVF_XYZB1            0x006
#define USFVF_XYZB2            0x008
#define USFVF_XYZB3            0x00a
#define USFVF_XYZB4            0x00c
#define USFVF_XYZB5            0x00e
#define USFVF_XYZW             0x4002

#define USFVF_NORMAL           0x010
#define USFVF_PSIZE            0x020
#define USFVF_DIFFUSE          0x040
#define USFVF_SPECULAR         0x080

#define USFVF_TEXCOUNT_MASK    0xf00
#define USFVF_TEXCOUNT_SHIFT   8
#define USFVF_TEX0             0x000
#define USFVF_TEX1             0x100
#define USFVF_TEX2             0x200
#define USFVF_TEX3             0x300
#define USFVF_TEX4             0x400
#define USFVF_TEX5             0x500
#define USFVF_TEX6             0x600
#define USFVF_TEX7             0x700
#define USFVF_TEX8             0x800

#define USFVF_LASTBETA_UBYTE4   0x1000
#define USFVF_LASTBETA_D3DCOLOR 0x8000

#define USFVF_RESERVED2         0x6000  // 2 reserved bits


// ust 

// vertex
#define US_BLDG_BUILDER_XYZ              0x0001
#define US_BLDG_BUILDER_NORMAL           0x0002
#define US_BLDG_BUILDER_BINORMAL         0x0003
#define US_BLDG_BUILDER_TANGENT          0x0004
#define US_BLDG_BUILDER_DIFFUSE          0x0005
#define US_BLDG_BUILDER_SPECULAR         0x0006

#define US_BLDG_BUILDER_TEX0             0x0010
#define US_BLDG_BUILDER_TEX1             0x0011
#define US_BLDG_BUILDER_TEX2             0x0012
#define US_BLDG_BUILDER_TEX3             0x0013
#define US_BLDG_BUILDER_TEX4             0x0014
#define US_BLDG_BUILDER_TEX5             0x0015
#define US_BLDG_BUILDER_TEX6             0x0016
#define US_BLDG_BUILDER_TEX7             0x0017
#define US_BLDG_BUILDER_TEX8             0x0018

// indice
#define US_BLDG_BUILDER_INDICES			 0x0100
#define US_BLDG_BUILDER_ATTRIBUTE		 0x0101
#define US_BLDG_BUILDER_MATERIAL		 0x0102
#define US_BLDG_BUILDER_AABB			 0x0103

	struct bldg_builder_head
	{
		short m_version;
		short m_type;
		bldg_builder_head(short t = 0, short v = 0)
		{
			m_type = t;
			m_version = v;
		}
	};

	struct bldg_builder_vertex_head
	{
		ulong m_num;
		ulong m_size;
		bldg_builder_vertex_head(ulong n = 0, ulong s = 0)
		{
			m_num = n;
			m_size = s;
		}
	};


	struct bldg_data_segment_head
	{
		short m_version;
		short m_type;
		bldg_data_segment_head(short t = 0, short v = 0)
		{
			m_type = t;
			m_version = v;
		}
	};

	struct bldg_data_common_meta
	{
		ulong m_num;
		ulong m_size;
		bldg_data_common_meta(ulong n = 0, ulong s = 0)
		{
			m_num = n;
			m_size = s;
		}
	};

	typedef bldg_data_common_meta bldg_data_vertex_meta;
	typedef bldg_data_common_meta bldg_data_index_meta;
	typedef bldg_data_common_meta bldg_data_attr_meta;

	// data type
#define US_USC_TYPE			0x0100
#define US_USM_TYPE			0x0101
#define US_UGP_TYPE			0x0102

// version
#define US_BUILDING_RAW		0x0001
#define US_BUILDING_ZIP		0x0002
#define US_BUILDING_7Z		0x0003


	// vertex
#define US_USC_VERTEX		0x0101
#define US_USC_INDICE		0x0102
#define US_USC_MATERIAL		0x0103
#define US_USC_BOUNDBOX		0x0104
#define US_USC_FIGURE		0x0105
#define US_USC_TOPOLOGIC	0x0106

// texture
#define US_TEXTURE_TYPE_NORMAL				0x0000
#define US_TEXTURE_TYPE_ANIMATION			0x0010
#define US_TEXTURE_TYPE_WATER				0x0020
#define US_TEXTURE_TYPE_ALPHA				0x0100
#define US_TEXTURE_TYPE_BLEND				0x0200
#define US_TEXTURE_TYPE_DYNAMIC				0x0300
#define US_TEXTURE_TYPE_UNKNOWN				0x0d00
#define US_TEXTURE_TYPE_ERROR				0x0e00


#define US_TEXTURE_TYPE_SELF_ILLUMINATION	0x0001
#define US_TEXTURE_TYPE_ACCURACY_SI			0x0011
#define US_TEXTURE_TYPE_SELF_ILLUMINATION_ERROR		0x0e01

#define US_TEXTURE_TYPE_SPECILAR			0x0002
#define US_TEXTURE_TYPE_BUMP				0x0003


	// usc_vertex_head
	struct usc_vertex_head
	{
		ulong m_num;
		ulong m_fvf;
		ulong m_stride;
		usc_vertex_head(ulong n = 0, ulong f = 0, ulong s = 0)
		{
			m_num = n;
			m_fvf = f;
			m_stride = s;
		}
	};

	// usc_index_head
	struct usc_index_head
	{
		ulong m_face_num;
		ulong m_bit_count;
		usc_index_head(ulong n = 0, ulong b = 16)
		{
			m_face_num = n;
			m_bit_count = b;
		}
	};

	// usc_figure_head
	struct usc_figure_head
	{
		ulong m_num;
		ulong m_bit_count;
		usc_figure_head(ulong n = 0, ulong b = 16)
		{
			m_num = n;
			m_bit_count = b;
		}
	};

	// usc_figure_vertex_head
	struct usc_figure_vertex_head
	{
		ulong m_num;
		ulong m_fvf;
		ulong m_stride;
		usc_figure_vertex_head(ulong n = 0, ulong f = 0, ulong s = 0)
		{
			m_num = n;
			m_fvf = f;
			m_stride = s;
		}
	};


	struct usc_material_head
	{
		ulong m_num;
		ulong m_texture_stage_count;
		usc_material_head(ulong n = 0, ulong tl = 1)
		{
			m_num = n;
			m_texture_stage_count = tl;
		}
	};

	struct usd_material_head
	{
		ulong m_num;
		ulong m_texture_stage_count;
		ulong m_material_segment;
		usd_material_head(ulong n = 0, ulong tl = 1, ulong ms = 1)
		{
			m_num = n;
			m_texture_stage_count = tl;
			m_material_segment = ms;
		}
	};


	struct edge
	{
		long start_point, end_point;
		long line_index;
		long left_poly, right_poly;
		edge(void)
		{
			start_point = 0;
			end_point = 0;
			line_index = 0;
			left_poly = -1;
			right_poly = -1;
		};
	};

	struct face_normal
	{
		float a, b, c, d;
		face_normal(void)
		{
			a = b = c = d = 0.0;
		};
	};

	typedef std::vector<edge> edge_array;
	typedef std::pair<int, int> bldg_point_pair;
	typedef std::map<bldg_point_pair, int> bldg_line_map;
	typedef std::vector<face_normal> face_normal_array;

}

#endif