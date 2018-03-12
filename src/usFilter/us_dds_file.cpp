///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_dds_file.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : dds_file class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////

#ifdef WIN64
#include "fastdxt/libdxt.h"
#endif // WIN64

#include "us_dds_file.h"
namespace uniscope_globe
{
#ifdef WIN64
	extern void gz_comp_mbr_ext(z_img_region_s* pDst,const z_img_region_s* pSrc);
#endif // WIN64
	dds_file::dds_file( void )
	{
		//create_from_file( dib_buffer, width, height, mipmap_level, file_header  );
	}

	dds_file::~dds_file( void )
	{

	}

	int dds_file::calculate_levl(int src_col)
	{
		int col_n = 0;
		while( (src_col>>1) > 0 )
		{
			col_n++;
			src_col = src_col>>1;
		}
		return col_n;
	}

	int dds_file::calculate_size(int in_row, int in_col, int mip_level)
	{
		int v_size = 0;
		for ( int i = 0; i < mip_level; i++ )
		{
			v_size += (in_row>>i) * (in_col>>i);
		}
		return v_size;
	}

	void dds_file::create_dxt1( byte* dib_buffer, long width, long height, long mipmap_level, bool file_header )
	{
		m_mipmap_level = max( min( calculate_levl( width ), calculate_levl( height ) ) - 2, 1 );
		if( mipmap_level > 0 && mipmap_level < m_mipmap_level )
		{
			m_mipmap_level = mipmap_level;
		}
		m_memory_size = calculate_size( width, height, m_mipmap_level ) >> 1;
		byte* v_cur_ptr;
		if( file_header )
		{
			// 压缩到DXT5
			char file_code[4] = {'D','D','S',' '};

			DDSURFACEDESC2 ddsd;
			memset(&ddsd,0,sizeof(ddsd));
			ddsd.dwSize			 = sizeof(ddsd);
			ddsd.dwFlags         = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT | DDSD_MIPMAPCOUNT;
			ddsd.dwWidth         = width;
			ddsd.dwHeight        = height;
			ddsd.ddpfPixelFormat.dwSize			 = 32;
			ddsd.ddpfPixelFormat.dwFlags		 = DDPF_FOURCC;
			ddsd.ddpfPixelFormat.dwRGBBitCount   = 32;
			ddsd.ddpfPixelFormat.dwFourCC        = FOURCC_DXT1;
#ifndef WIN64
			ddsd.ddsCaps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_3DDEVICE | DDSCAPS_MIPMAP | DDSCAPS_COMPLEX;
#else
			ddsd.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_3DDEVICE | DDSCAPS_MIPMAP | DDSCAPS_COMPLEX;
#endif

			ddsd.dwMipMapCount  = m_mipmap_level;

			m_dxt_buffer = new managed_buffer( m_memory_size + sizeof( ddsd ) + 4 );

			// write head
			memcpy( m_dxt_buffer->get_buffer(), file_code, 4 );
			memcpy( m_dxt_buffer->get_buffer() + 4, &ddsd, sizeof(ddsd) );	

			v_cur_ptr = m_dxt_buffer->get_buffer() + sizeof( ddsd ) + 4;
		}
		else
		{
			m_dxt_buffer = new managed_buffer( m_memory_size );
			v_cur_ptr = m_dxt_buffer->get_buffer();
		}

		// compress level_0
		int tmp_size = 0, v_sum_size = 0, v_size = width * height / 2;
#ifndef WIN64
		gz_comp_so( dib_buffer, width, height, v_cur_ptr, &tmp_size );
#else
		CompressDXT(dib_buffer,v_cur_ptr,width,height,FORMAT_DXT1);
#endif
		
		v_sum_size += v_size;

		// compress mip_chain
		z_img_region src_reg;
		src_reg.pdata = (z_argb_32*)dib_buffer;
		src_reg.width = width;
		src_reg.height = height;
		src_reg.byte_width = width<<2;

		byte* tmp_byte = AUTO_ALIGNED_ALLOC_MEMORY( (width*height), 16 );
		z_img_region des_reg;
		des_reg.pdata = (z_argb_32*)tmp_byte;

		z_argb_32* v_change_byte = NULL;
		for ( int i = 1; i < m_mipmap_level; i++ )
		{
			des_reg.width = width>>i;
			des_reg.height = height>>i;
			des_reg.byte_width = des_reg.width<<2;
			v_size = (des_reg.width * des_reg.height) / 2;

#ifndef WIN64
			gz_comp_mbr(&des_reg, &src_reg);
			gz_comp_so((byte*)des_reg.pdata, des_reg.width, des_reg.height, v_cur_ptr + v_sum_size, &tmp_size);
#else
			gz_comp_mbr_ext(&des_reg, &src_reg);
			CompressDXT((byte*)des_reg.pdata, v_cur_ptr + v_sum_size, des_reg.width, des_reg.height,FORMAT_DXT1);
#endif
			v_sum_size += v_size;

			v_change_byte = src_reg.pdata;
			src_reg.pdata = des_reg.pdata;
			src_reg.width = des_reg.width;
			src_reg.height = des_reg.height;
			src_reg.byte_width = des_reg.byte_width;
			des_reg.pdata = v_change_byte;
		}

		// 设置texture_info
		memset( &m_tex_info, 0, sizeof(m_tex_info) );
		m_tex_info.width  = width;
		m_tex_info.height = height;
		m_tex_info.mipmap = m_mipmap_level;
		m_tex_info.format = (int)D3DFMT_DXT1;

		// 清除临时数据
		AUTO_ALIGNED_FREE_MEMORY( tmp_byte );

	}

	void dds_file::create_dxt5( byte* dib_buffer, long width, long height, long mipmap_level, bool file_header  )
	{
		m_mipmap_level = max( min( calculate_levl( width ), calculate_levl( height ) ) - 2, 1 );
		if( mipmap_level > 0 && mipmap_level < m_mipmap_level )
		{
			m_mipmap_level = mipmap_level;
		}
		m_memory_size = calculate_size( width, height, m_mipmap_level );
		byte* v_cur_ptr;

		if( file_header )
		{
			// 压缩到DXT5
			char file_code[4] = {'D','D','S',' '};

			DDSURFACEDESC2 ddsd;
			memset(&ddsd,0,sizeof(ddsd));
			ddsd.dwSize			 = sizeof(ddsd);
			ddsd.dwFlags         = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT | DDSD_MIPMAPCOUNT;
			ddsd.dwWidth         = width;
			ddsd.dwHeight        = height;
			ddsd.ddpfPixelFormat.dwSize			 = 32;
			ddsd.ddpfPixelFormat.dwFlags		 = DDPF_FOURCC;
			ddsd.ddpfPixelFormat.dwRGBBitCount   = 32;
			ddsd.ddpfPixelFormat.dwFourCC        = FOURCC_DXT5;
#ifndef WIN64
			ddsd.ddsCaps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_3DDEVICE | DDSCAPS_MIPMAP | DDSCAPS_COMPLEX;
#else
			ddsd.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_3DDEVICE | DDSCAPS_MIPMAP | DDSCAPS_COMPLEX;
#endif

			ddsd.dwMipMapCount  = m_mipmap_level;

			m_dxt_buffer = new managed_buffer( m_memory_size + sizeof( ddsd ) + 4 );

			// write head
			memcpy( m_dxt_buffer->get_buffer(), file_code, 4 );
			memcpy( m_dxt_buffer->get_buffer() + 4, &ddsd, sizeof(ddsd) );	

			v_cur_ptr = m_dxt_buffer->get_buffer() + sizeof( ddsd ) + 4;
		}
		else
		{
			m_dxt_buffer = new managed_buffer( m_memory_size );
			v_cur_ptr = m_dxt_buffer->get_buffer();
		}

		// compress level_0
		int tmp_size = 0, v_sum_size = 0, v_size = width * height;
#ifndef WIN64
		gz_comp_sf(dib_buffer, width, height, v_cur_ptr, &tmp_size);
#else
		CompressDXT(dib_buffer,v_cur_ptr,width,height,FORMAT_DXT5);
#endif
		
		v_sum_size += v_size;


		// compress mip_chain
		z_img_region src_reg;
		src_reg.pdata = (z_argb_32*)dib_buffer;
		src_reg.width = width;
		src_reg.height = height;
		src_reg.byte_width = width<<2;

		byte* tmp_byte = AUTO_ALIGNED_ALLOC_MEMORY( (width*height), 16 );
		z_img_region des_reg;
		des_reg.pdata = (z_argb_32*)tmp_byte;

		z_argb_32* v_change_byte = NULL;
		for ( int i = 1; i < m_mipmap_level; i++ )
		{
			des_reg.width = width>>i;
			des_reg.height = height>>i;
			des_reg.byte_width = des_reg.width<<2;
			v_size = des_reg.width * des_reg.height;
#ifndef WIN64
			gz_comp_mbr(&des_reg, &src_reg);
			gz_comp_sf((byte*)des_reg.pdata, des_reg.width, des_reg.height, v_cur_ptr + v_sum_size, &tmp_size);
#else
			gz_comp_mbr_ext(&des_reg, &src_reg);
			CompressDXT((byte*)des_reg.pdata,v_cur_ptr + v_sum_size, des_reg.width, des_reg.height,FORMAT_DXT5);
#endif


			v_sum_size += v_size;

			v_change_byte = src_reg.pdata;
			src_reg.pdata = des_reg.pdata;
			src_reg.width = des_reg.width;
			src_reg.height = des_reg.height;
			src_reg.byte_width = des_reg.byte_width;
			des_reg.pdata = v_change_byte;
		}


		//int t_size = width * height + (width * height) >> 2;
		//memset( v_cur_ptr, 0, t_size );

		// 设置texture_info
		memset( &m_tex_info, 0, sizeof(m_tex_info) );
		m_tex_info.width  = width;
		m_tex_info.height = height;
		m_tex_info.mipmap = m_mipmap_level;
		m_tex_info.format = (int)D3DFMT_DXT5;

		// 清除临时数据
		AUTO_ALIGNED_FREE_MEMORY( tmp_byte );
	}

	bool dds_file::create_argb( byte* dib_buffer, long width, long height, long mipmap_level, bool file_header )
	{
		m_mipmap_level = max( min( calculate_levl( width ), calculate_levl( height ) ) - 2, 1 );
		if( mipmap_level > 0 && mipmap_level < m_mipmap_level )
		{
			m_mipmap_level = mipmap_level;
		}

		//m_mipmap_level = 1;
		m_memory_size = calculate_size( width, height, m_mipmap_level );
		m_memory_size *= 4;

		byte* v_cur_ptr;

		if( file_header )
		{
			// 压缩到DXT5
			char file_code[4] = {'D','D','S',' '};

			DDSURFACEDESC2 ddsd;
			memset(&ddsd,0,sizeof(ddsd));
			ddsd.dwSize			 = sizeof(ddsd);
			ddsd.dwFlags         = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT | DDSD_MIPMAPCOUNT;
			ddsd.dwWidth         = width;
			ddsd.dwHeight        = height;
#ifndef WIN64
			ddsd.dwLinearSize	 = width * 4;
#else
			ddsd.dwPitchOrLinearSize = width * 4;
#endif

			ddsd.ddpfPixelFormat.dwSize			 = 32;
			ddsd.ddpfPixelFormat.dwFlags		 = DDPF_RGB | DDPF_ALPHAPIXELS;
			ddsd.ddpfPixelFormat.dwRGBBitCount   = 32;
			ddsd.ddpfPixelFormat.dwRBitMask		 = 0x00ff0000;
			ddsd.ddpfPixelFormat.dwGBitMask		 = 0x0000ff00;
			ddsd.ddpfPixelFormat.dwBBitMask		 = 0x000000ff;
#ifndef WIN64
			ddsd.ddpfPixelFormat.dwRGBAlphaBitMask	 = 0xff000000;

			//ddsd.ddpfPixelFormat.dwFourCC        = FOURCC_DXT1;

			ddsd.ddsCaps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_3DDEVICE | DDSCAPS_MIPMAP | DDSCAPS_COMPLEX;	
#else
			ddsd.ddpfPixelFormat.dwABitMask	 = 0xff000000;

			//ddsd.ddpfPixelFormat.dwFourCC        = FOURCC_DXT1;

			ddsd.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_3DDEVICE | DDSCAPS_MIPMAP | DDSCAPS_COMPLEX;	
#endif
		
			ddsd.dwMipMapCount  = m_mipmap_level;

			m_dxt_buffer = new managed_buffer( m_memory_size + sizeof( ddsd ) + 4 );

			// write head
			memcpy( m_dxt_buffer->get_buffer(), file_code, 4 );
			memcpy( m_dxt_buffer->get_buffer() + 4, &ddsd, sizeof(ddsd) );	

			v_cur_ptr = m_dxt_buffer->get_buffer() + sizeof( ddsd ) + 4;
		}
		else
		{
			m_dxt_buffer = new managed_buffer( m_memory_size );
			v_cur_ptr = m_dxt_buffer->get_buffer();
		}

		// compress level_0
		int v_size = width * height * 4;
		memcpy( v_cur_ptr, dib_buffer, v_size  );
		int v_sum_size = v_size;
		v_cur_ptr = v_cur_ptr + v_size;

		// compress mip_chain
		z_img_region src_reg;
		src_reg.pdata = (z_argb_32*)dib_buffer;
		src_reg.width = width;
		src_reg.height = height;
		src_reg.byte_width = width<<2;

		byte* tmp_byte = AUTO_ALIGNED_ALLOC_MEMORY( (width*height*4), 16 );
		if ( tmp_byte == NULL )
		{
			AUTO_DELETE( m_dxt_buffer );
			return false;
		}

		z_img_region des_reg;
		des_reg.pdata = (z_argb_32*)tmp_byte;

		z_argb_32* v_change_byte = NULL;
		for ( int i = 1; i < m_mipmap_level; i++ )
		{
			des_reg.width = width>>i;
			des_reg.height = height>>i;
			des_reg.byte_width = des_reg.width * 4;
			v_size = des_reg.width * des_reg.height * 4;
#ifndef WIN64
			gz_comp_mbr(&des_reg, &src_reg);
#else
			gz_comp_mbr_ext(&des_reg, &src_reg);
#endif
			

			memcpy( v_cur_ptr, des_reg.pdata, v_size );
			v_sum_size += v_size;
			v_cur_ptr = v_cur_ptr + v_size;

			v_change_byte = src_reg.pdata;
			src_reg.pdata = des_reg.pdata;
			src_reg.width = des_reg.width;
			src_reg.height = des_reg.height;
			src_reg.byte_width = des_reg.byte_width;
			des_reg.pdata = v_change_byte;
		}

		// 设置texture_info
		memset( &m_tex_info, 0, sizeof(m_tex_info) );
		m_tex_info.width  = width;
		m_tex_info.height = height;
		m_tex_info.mipmap = m_mipmap_level;
		m_tex_info.format = (int)D3DFMT_A8B8G8R8;

		// 清除临时数据
		AUTO_ALIGNED_FREE_MEMORY( tmp_byte );

		return true;
	}
	
}