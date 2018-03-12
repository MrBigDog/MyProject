///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_uni_mng_filter.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : uni_mng_filter class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_uni_mng_filter.h"

namespace uniscope_globe
{
#ifdef WIN64
	extern unsigned GetTicks(CxImageMNG *pImg);
	extern int GetDIBSize(CxImageMNG *pImg);
#endif
	object_base* uni_mng_filter::parse( raw_buffer* in_buffer )
	{
		CxImageMNG cx_image_engine;

		//从内存中读取
		byte* mng_buff = in_buffer->get_buffer();
		int v_type = 0x474e4d8a;
		memcpy( mng_buff, &v_type, sizeof(v_type) );
#ifndef WIN64
		cx_image_engine.ReadFromMemory(in_buffer->get_buffer(), in_buffer->get_size());
#else
		// 暂时不会解析mng文件，没有代码 [9/20/2017 mahc]
		return nullptr;
		cx_image_engine.CxImage::Decode(in_buffer->get_buffer(), in_buffer->get_size(),CXIMAGE_FORMAT_MNG);
#endif // CXIMAGE70

		if (!cx_image_engine.IsValid())
		{
			//内存读取失败
			cx_image_engine.Clear();
			return NULL;
		}

		if ( cx_image_engine.GetNumFrames() == 0 ) return NULL;
#ifndef WIN64
		ulong v_dib_size = cx_image_engine.GetDIBSize();
#else
		ulong v_dib_size = GetDIBSize(&cx_image_engine);
#endif
		
		ulong v_width    = cx_image_engine.GetWidth();
		ulong v_height   = cx_image_engine.GetHeight();
		ulong v_time	 = 0;


		//申请空间存储图像
		byte *v_dib_data = (byte*)AUTO_ALIGNED_ALLOC_MEMORY(v_dib_size,16);
		d3d9_hierarchy_texture* v_hierarchy_texture = new d3d9_hierarchy_texture;

		//返回图像个数
#ifndef WIN64
		int v_map_size = cx_image_engine.GetDIBMapSize();
#else
		int v_map_size = cx_image_engine.GetNumFrames();
#endif 
		
		for( int ni = 0; ni < v_map_size; ni++ )
		{
			//返回第ni幅图像缓存指针
#ifndef WIN64
			byte *temp_dib_data = cx_image_engine.GetDIBMapAt( ni );
#else
			byte *temp_dib_data = (byte*)cx_image_engine.GetFrame( ni )->GetDIB();
#endif
			if ( temp_dib_data )
			{
				ZeroMemory( v_dib_data, v_dib_size );
				int pitch_Size = v_width * 4;

				byte* v_temp_data = new byte[ pitch_Size ];
				for( int nj = 0; nj < int( v_height * 0.5 ); nj++ )
				{
					memcpy( v_temp_data, temp_dib_data + nj * pitch_Size, pitch_Size );
					memcpy( v_dib_data + nj * pitch_Size , temp_dib_data + ( v_height - 1 - nj ) * pitch_Size, pitch_Size );
					memcpy( v_dib_data + ( v_height - 1 - nj ) * pitch_Size, v_temp_data , pitch_Size );
				}
				AUTO_DELETE( v_temp_data );

				dds_file v_dds_file;
				v_dds_file.create_dxt5( v_dib_data, v_width, v_height, 0, true );

				//保存纹理并计算延迟时间
				d3d9_texture_frame* v_texture_frame = new d3d9_texture_frame( v_dds_file.m_dxt_buffer, v_dds_file.m_tex_info );
				v_texture_frame->set_key_time( v_time );
				v_hierarchy_texture->insert_frame( v_texture_frame );

#ifndef WIN64
				float v_delay_time = (float)cx_image_engine.GetDIBFrameDelayAt( ni );
				ulong v_ticks = cx_image_engine.GetTicks();
#else
				float v_delay_time = (float)cx_image_engine.GetFrame( ni )->GetFrameDelay();
				ulong v_ticks = GetTicks(&cx_image_engine);
#endif
				
				if ( v_ticks < 0 || v_ticks == 0 ) v_ticks = 1;
				v_time += ( 1000 * v_delay_time / (float)v_ticks );
			}				
		}

		if (v_time != 0)
		{
			v_hierarchy_texture->set_total_time( v_time );
		}
		else
		{
			v_hierarchy_texture->set_total_time( 1 );
		}

		AUTO_ALIGNED_FREE_MEMORY( v_dib_data );

		return (object_base*)v_hierarchy_texture;
	}
	
}