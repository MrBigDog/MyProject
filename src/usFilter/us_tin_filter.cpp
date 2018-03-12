///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_tin_filter.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : tin_filter class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_tin_filter.h"

namespace uniscope_globe
{
	object_base* tin_filter::parse( raw_buffer* in_buffer )
	{
		interior_index v_index = ((square_buffer*)in_buffer)->get_index();

		return parse_from_compressed_stream( in_buffer, v_index );
	}

	object_base* tin_filter::parse_from_compressed_stream( raw_buffer* stream, interior_index index )
	{
		dem_mesh* v_dem_mesh = new dem_mesh( index );
		// read file head	
		us_square_head v_file_head;
		stream->read( &v_file_head, sizeof(v_file_head) );

		// read zipped segment
		us_zipped_segment v_tin_segment;
		memset( &v_tin_segment, 0, sizeof(v_tin_segment) );
		stream->read( &v_tin_segment, sizeof( v_tin_segment ) );

		ulong v_unzip_data_size = v_tin_segment.unzipped_data_size;
		int v_temp_data_size = v_tin_segment.zipped_data_size + v_unzip_data_size;
		byte* v_temp_data = new byte[v_temp_data_size];
		byte* v_zip_data = v_temp_data;
		byte* v_unzip_data = v_temp_data + v_tin_segment.zipped_data_size;

		// read vertex segment ex
		us_vertex_segment_ex v_vertex_segment_ex;
		stream->read( &v_vertex_segment_ex, sizeof( v_vertex_segment_ex ) );
		double v_elevation_scale = (v_vertex_segment_ex.elevation_max - v_vertex_segment_ex.elevation_min) / (double)v_vertex_segment_ex.divide_number;
		double v_horizonal_scale = (v_dem_mesh->m_sub_mesh[0]->get_bound().get_width() * 2 ) / (double)v_vertex_segment_ex.divide_number;
		double v_west = v_dem_mesh->m_sub_mesh[0]->get_bound().m_west;
		double v_south = v_dem_mesh->m_sub_mesh[0]->get_bound().m_south;

		// read zipped data
		stream->read( v_zip_data, v_tin_segment.zipped_data_size );

		// uncompress data
		uncompress( (byte*)v_unzip_data, &v_unzip_data_size, v_zip_data, v_tin_segment.zipped_data_size );

		//square_description v_square_desc( index );
		// process unzipped data
		{
			raw_buffer v_data_buffer( v_unzip_data, v_unzip_data_size );

			us_index_segment v_index_segment;
			short* v_vertex_data = new short[35 * 35 * 3];

			us_vertex_segment v_vertex_segment;
			short* v_index_data = new short[35 * 35 * 3 ];
			for( int ni = 0;ni < square_quadrant::QUADRANT_COUNT; ni++ )
			{
				// read vertex segment
				v_data_buffer.read( &v_vertex_segment, sizeof( v_vertex_segment ) );

				// read vertex data
				v_data_buffer.read( v_vertex_data, v_vertex_segment.data_size );

				// read index segment
				v_data_buffer.read( &v_index_segment, sizeof( v_index_segment ) );

				// read index data
				v_data_buffer.read( v_index_data, v_index_segment.data_size );
				
				//create sub mesh
				{
					// generate vertex buffer
					int v_indices_count = v_index_segment.data_size / 2;
					int v_verts_count = v_vertex_segment.data_size / 6;
					int v_verts_size = sizeof( position_color_texture2 ) * v_verts_count;
					managed_buffer* v_raw_vertices_buffer = new managed_buffer( v_verts_size );
					position_color_texture2* v_vertex_buffer = (position_color_texture2*)v_raw_vertices_buffer->get_buffer();

					lon_lat_rect v_bound = v_dem_mesh->m_sub_mesh[ni]->get_bound();
					vector3<double> v_center_sphr = v_bound.get_center();
					vector3<double> v_center_geo = geocentric_coords::from_spherical_d( v_center_sphr.x, v_center_sphr.y, spherical_coords::s_semi_major );

					double v_longitude, v_latitude, v_altitude;
					for(int nj = 0; nj < v_verts_count; nj++)
					{
						// 
						v_longitude = v_vertex_data[nj*3] * v_horizonal_scale + v_west;
						v_latitude  = v_vertex_data[nj*3+1] * v_horizonal_scale + v_south;
						v_altitude  = (v_vertex_data[nj*3+2] * v_elevation_scale + v_vertex_segment_ex.elevation_min) * 0.001;	

						vector3<double> v_out = geocentric_coords::from_spherical_d( v_longitude, v_latitude, spherical_coords::s_semi_major + v_altitude );

						v_vertex_buffer[nj].x = (float)(v_out.x - v_center_geo.x);
						v_vertex_buffer[nj].y = (float)(v_out.y - v_center_geo.y);
						v_vertex_buffer[nj].z = (float)(v_out.z - v_center_geo.z);

						
						float tu = (v_longitude - v_bound.m_west) / v_bound.get_width();
						float tv = 1 - (v_latitude - v_bound.m_south) /v_bound.get_height();
						v_vertex_buffer[nj].tu1 = tu;
						v_vertex_buffer[nj].tv1 = tv;
						v_vertex_buffer[nj].tu2 = tu;
						v_vertex_buffer[nj].tv2 = tv;
						v_vertex_buffer[nj].color = 0xffffffff;
					}

					// generate index buffer
					managed_buffer* v_raw_index_buffer = new managed_buffer( (byte*)v_index_data, v_index_segment.data_size );

					v_dem_mesh->m_sub_mesh[ni]->create_tin( v_raw_vertices_buffer, v_raw_index_buffer, v_vertex_segment_ex.elevation_min * 0.001, v_vertex_segment_ex.elevation_max * 0.001 );

				}
				
			}
			AUTO_DELETE_ARRAY( v_vertex_data );
		}
		AUTO_DELETE_ARRAY( v_temp_data );

		return (object_base*) v_dem_mesh;
	}


	object_base* tin_filter_v2::parse( raw_buffer* in_buffer )
	{
		interior_index v_index = ((square_buffer*)in_buffer)->get_index();

		return parse_from_compressed_stream( in_buffer, v_index );
	}

	object_base* tin_filter_v2::parse_from_compressed_stream( raw_buffer* stream, interior_index index )
	{
		dem_mesh* v_dem_mesh = new dem_mesh( index );
		// read file head	
		us_square_head v_file_head;
		stream->read( &v_file_head, sizeof(v_file_head) );

		// 读取顶点序列
		us_vertex_segment v_ver_segment;
		memset(&v_ver_segment, 0, sizeof(v_ver_segment));
		stream->read(&v_ver_segment, sizeof(v_ver_segment));
		us_grid_segment_ex v_ver_seg_ex;
		memset(&v_ver_seg_ex, 0, sizeof(v_ver_seg_ex));
		stream->read(&v_ver_seg_ex, sizeof(v_ver_seg_ex));

		int v_verts_count = v_ver_segment.data_size / sizeof(unsigned short) / 3;
		// read vertex data
		unsigned short* v_vertex_data = new unsigned short[v_verts_count * 3];
		stream->read( v_vertex_data, v_ver_segment.data_size );

		lon_lat_rect v_bound = v_dem_mesh->m_sub_mesh[0]->get_bound();
		double v_elevation_scale = (v_ver_seg_ex.elevation_max - v_ver_seg_ex.elevation_min) / (double)v_ver_seg_ex.divide_number;
		double v_horizonal_scale = (v_bound.get_width() * 2 ) / (double)v_ver_seg_ex.divide_number;
		double v_west = v_bound.m_west;
		double v_south = v_bound.m_south;
		int v_verts_size = sizeof( position_color_texture2 ) * v_verts_count;

		// 读取索引
		for( int ni = 0;ni < 4; ni++ )
		{
			// 读索引
			us_index_segment v_index_segment;
			memset(&v_index_segment, 0, sizeof(v_index_segment));
			stream->read(&v_index_segment, sizeof(v_index_segment));
			short* v_index_data = new short[v_index_segment.data_size / sizeof(short)];
			stream->read(v_index_data, v_index_segment.data_size);

			// 创建submesh
			// generate vertex buffer
			int v_index_count = v_index_segment.data_size / 2;
			managed_buffer* v_raw_vertices_buffer = new managed_buffer( v_index_count * position_color_texture2::stride );
			position_color_texture2* v_vertex_buffer = (position_color_texture2*)v_raw_vertices_buffer->get_buffer();

			lon_lat_rect v_bound = v_dem_mesh->m_sub_mesh[ni]->get_bound();
			vector3<double> v_center_sphr = v_bound.get_center();
			vector3<double> v_center_geo = geocentric_coords::from_spherical_d( v_center_sphr.x, v_center_sphr.y, spherical_coords::s_semi_major );

			double v_longitude, v_latitude, v_altitude;
			for(int nj = 0; nj < v_index_count; nj++)
			{
				// 
				v_longitude = v_vertex_data[v_index_data[nj]*3] * v_horizonal_scale + v_west;
				v_latitude  = v_vertex_data[v_index_data[nj]*3+1] * v_horizonal_scale + v_south;
				if (v_vertex_data[v_index_data[nj]*3+2] == v_ver_seg_ex.invalid_value)
					v_altitude = v_ver_seg_ex.elevation_min * 0.001;
				else 
					v_altitude  = (v_vertex_data[v_index_data[nj]*3+2] * v_elevation_scale + v_ver_seg_ex.elevation_min) * 0.001;	

				vector3<double> v_out = geocentric_coords::from_spherical_d( v_longitude, v_latitude, spherical_coords::s_semi_major + v_altitude );

				v_vertex_buffer[nj].x = (float)(v_out.x - v_center_geo.x);
				v_vertex_buffer[nj].y = (float)(v_out.y - v_center_geo.y);
				v_vertex_buffer[nj].z = (float)(v_out.z - v_center_geo.z);


				float tu = (v_longitude - v_bound.m_west) / v_bound.get_width();
				float tv = 1 - (v_latitude - v_bound.m_south) /v_bound.get_height();
				v_vertex_buffer[nj].tu1 = tu;
				v_vertex_buffer[nj].tv1 = tv;
				v_vertex_buffer[nj].tu2 = tu;
				v_vertex_buffer[nj].tv2 = tv;
				v_vertex_buffer[nj].color = 0xffffffff;
				v_index_data[ni] = nj;
			}

			// generate index buffer
			managed_buffer* v_raw_index_buffer = new managed_buffer( (byte*)v_index_data, v_index_segment.data_size );

			v_dem_mesh->m_sub_mesh[ni]->create_tin( v_raw_vertices_buffer, v_raw_index_buffer, v_ver_seg_ex.elevation_min * 0.001, v_ver_seg_ex.elevation_max * 0.001);
			AUTO_DELETE_ARRAY(v_index_data);
		}
		AUTO_DELETE_ARRAY(v_vertex_data);

		return (object_base*) v_dem_mesh;
	}


	object_base* tin_filter_v4::parse( raw_buffer* in_buffer )
	{
		interior_index v_index = ((square_buffer*)in_buffer)->get_index();

		return parse_from_compressed_stream( in_buffer, v_index );
	}

	object_base* tin_filter_v4::parse_from_compressed_stream( raw_buffer* stream, interior_index index )
	{
		dem_mesh* v_dem_mesh = new dem_mesh( index );
		// read file head	
		us_square_head v_file_head;
		stream->read( &v_file_head, sizeof(v_file_head) );

		// 读取顶点序列
		us_vertex_segment v_ver_segment;
		memset(&v_ver_segment, 0, sizeof(v_ver_segment));
		stream->read(&v_ver_segment, sizeof(v_ver_segment));
		us_grid_segment_ex v_ver_seg_ex;
		memset(&v_ver_seg_ex, 0, sizeof(v_ver_seg_ex));
		stream->read(&v_ver_seg_ex, sizeof(v_ver_seg_ex));

		int v_verts_count = v_ver_segment.data_size / sizeof(unsigned short) / 3;
		// read vertex data
		unsigned short* v_vertex_data = new unsigned short[v_verts_count * 3];
		stream->read( v_vertex_data, v_ver_segment.data_size );

		lon_lat_rect v_bound = v_dem_mesh->m_sub_mesh[0]->get_bound();
		double v_elevation_scale = (v_ver_seg_ex.elevation_max - v_ver_seg_ex.elevation_min) / (double)v_ver_seg_ex.divide_number;
		double v_horizonal_scale = (v_bound.get_width() * 2 ) / (double)v_ver_seg_ex.divide_number;
		double v_west = v_bound.m_west;
		double v_south = v_bound.m_south;
		int v_verts_size = sizeof( position_color_texture2 ) * v_verts_count;

		// 读取索引
		for( int ni = 0;ni < 4; ni++ )
		{

			lon_lat_rect v_bound = v_dem_mesh->m_sub_mesh[ni]->get_bound();
			vector3<double> v_center_sphr = v_bound.get_center();
			vector3<double> v_center_geo = geocentric_coords::from_spherical_d( v_center_sphr.x, v_center_sphr.y, spherical_coords::s_semi_major );
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// 读索引
			us_index_segment v_index_segment;
			memset(&v_index_segment, 0, sizeof(v_index_segment));
			stream->read(&v_index_segment, sizeof(v_index_segment));
			int v_index_count = v_index_segment.data_size / sizeof(short);
			short* v_index_data = new short[v_index_count];
			stream->read(v_index_data, v_index_segment.data_size);

			// generate vertex buffer
			managed_buffer* v_raw_vertices_buffer = new managed_buffer( v_index_count * position_color_texture2::stride );
			managed_buffer* v_raw_index_buffer = new managed_buffer( v_index_segment.data_size );
			{
				position_color_texture2* v_vertex_buffer = (position_color_texture2*)v_raw_vertices_buffer->get_buffer();

				double v_longitude, v_latitude, v_altitude;
				for(int ni = 0; ni < v_index_count; ni++)
				{
					// 
					v_longitude = v_vertex_data[v_index_data[ni]*3] * v_horizonal_scale + v_west;
					v_latitude  = v_vertex_data[v_index_data[ni]*3+1] * v_horizonal_scale + v_south;
					if (v_vertex_data[v_index_data[ni]*3+2] == v_ver_seg_ex.invalid_value)
						v_altitude = v_ver_seg_ex.elevation_min * 0.001;
					else 
						v_altitude  = (v_vertex_data[v_index_data[ni]*3+2] * v_elevation_scale + v_ver_seg_ex.elevation_min) * 0.001;	

					vector3<double> v_out = geocentric_coords::from_spherical_d( v_longitude, v_latitude, spherical_coords::s_semi_major + v_altitude );

					v_vertex_buffer[ni].x = (float)(v_out.x - v_center_geo.x);
					v_vertex_buffer[ni].y = (float)(v_out.y - v_center_geo.y);
					v_vertex_buffer[ni].z = (float)(v_out.z - v_center_geo.z);


					float tu = (v_longitude - v_bound.m_west) / v_bound.get_width();
					float tv = 1 - (v_latitude - v_bound.m_south) /v_bound.get_height();
					v_vertex_buffer[ni].tu1 = tu;
					v_vertex_buffer[ni].tv1 = tv;
					v_vertex_buffer[ni].tu2 = tu;
					v_vertex_buffer[ni].tv2 = tv;
					v_vertex_buffer[ni].color = 0xffffffff;
					v_index_data[ni] = ni;
				}
				v_raw_index_buffer->copy((byte*)v_index_data, v_index_segment.data_size);
			}
			
	
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// 读取SkirtIndex头
			us_index_segment v_skirt_index_segment;
			memset(&v_skirt_index_segment, 0, sizeof(v_skirt_index_segment));
			stream->read(&v_skirt_index_segment, sizeof(v_skirt_index_segment));
			short* v_skirt_index_data = new short[v_skirt_index_segment.data_size / sizeof(short)];
			stream->read(v_skirt_index_data, v_skirt_index_segment.data_size);

			// generate vertex buffer
			int v_skirt_index_count = v_skirt_index_segment.data_size / 2;
			managed_buffer* v_skirt_raw_vertices_buffer = new managed_buffer( v_skirt_index_count * position_color_texture2::stride * 2 );
			managed_buffer* v_skirt_raw_index_buffer = new managed_buffer( v_skirt_index_count * 12 );
			{
				position_color_texture2* v_vertex_buffer = (position_color_texture2*)v_skirt_raw_vertices_buffer->get_buffer();
				double v_longitude, v_latitude, v_altitude;
				int index = 0;
				int ni = 0;
				for(ni = 0; ni < v_skirt_index_count; ni++)
				{
					// 
					v_longitude = v_vertex_data[v_skirt_index_data[ni]*3] * v_horizonal_scale + v_west;
					v_latitude  = v_vertex_data[v_skirt_index_data[ni]*3+1] * v_horizonal_scale + v_south;
					if (v_vertex_data[v_skirt_index_data[ni]*3+2] == v_ver_seg_ex.invalid_value)
						v_altitude = v_ver_seg_ex.elevation_min * 0.001;
					else 
						v_altitude  = (v_vertex_data[v_skirt_index_data[ni]*3+2] * v_elevation_scale + v_ver_seg_ex.elevation_min) * 0.001;	

					float tu = (v_longitude - v_bound.m_west) / v_bound.get_width();
					float tv = 1 - (v_latitude - v_bound.m_south) /v_bound.get_height();

					vector3<double> v_out1 = geocentric_coords::from_spherical_d( v_longitude, v_latitude, spherical_coords::s_semi_major + v_altitude );
					vector3<double> v_out2 = geocentric_coords::from_spherical_d( v_longitude, v_latitude, spherical_coords::s_semi_major + v_ver_seg_ex.elevation_min * 0.001);

					v_vertex_buffer[index].x = (float)(v_out1.x - v_center_geo.x);
					v_vertex_buffer[index].y = (float)(v_out1.y - v_center_geo.y);
					v_vertex_buffer[index].z = (float)(v_out1.z - v_center_geo.z);
					v_vertex_buffer[index].tu1 = tu;
					v_vertex_buffer[index].tv1 = tv;
					v_vertex_buffer[index].tu2 = tu;
					v_vertex_buffer[index].tv2 = tv;
					v_vertex_buffer[index].color = 0xffffffff;

					index++;
					v_vertex_buffer[index].x = (float)(v_out2.x - v_center_geo.x);
					v_vertex_buffer[index].y = (float)(v_out2.y - v_center_geo.y);
					v_vertex_buffer[index].z = (float)(v_out2.z - v_center_geo.z);
					v_vertex_buffer[index].tu1 = tu;
					v_vertex_buffer[index].tv1 = tv;
					v_vertex_buffer[index].tu2 = tu;
					v_vertex_buffer[index].tv2 = tv;
					v_vertex_buffer[index].color = 0xffffffff;
					index++;
				}
				
				short* v_index_buffer = (short*)v_skirt_raw_index_buffer->get_buffer();
				index = 0;
				int index_count = v_skirt_index_count * 2;
				for(ni = 0; ni < index_count - 2; ni+=2)
				{
					v_index_buffer[index++] = ni;
					v_index_buffer[index++] = ni+2;
					v_index_buffer[index++] = ni+1;

					v_index_buffer[index++] = ni+1;
					v_index_buffer[index++] = ni+2;
					v_index_buffer[index++] = ni+3;
				}

				v_index_buffer[index++] = ni;
				v_index_buffer[index++] = 0;
				v_index_buffer[index++] = ni+1;

				v_index_buffer[index++] = ni+1;
				v_index_buffer[index++] = 0;
				v_index_buffer[index++] = 1;

				int xxxx = 0;

			}


			v_dem_mesh->m_sub_mesh[ni]->create_tin_with_skirt( v_raw_vertices_buffer, v_raw_index_buffer, v_skirt_raw_vertices_buffer, v_skirt_raw_index_buffer, v_ver_seg_ex.elevation_min * 0.001, v_ver_seg_ex.elevation_max * 0.001 );
			AUTO_DELETE_ARRAY(v_index_data);
			AUTO_DELETE_ARRAY(v_skirt_index_data);
		}
		AUTO_DELETE_ARRAY(v_vertex_data);

		return (object_base*) v_dem_mesh;
	}

	object_base* dem_mesh_filter::parse( raw_buffer* in_buffer )
	{
		interior_index v_index = ((square_buffer*)in_buffer)->get_index();

		return parse_from_compressed_stream( in_buffer, v_index );
	}

	object_base* dem_mesh_filter::parse_from_compressed_stream( raw_buffer* stream, interior_index index )
	{
		square_buffer* s_b = (square_buffer* )stream;

		dem_mesh* v_dem_mesh = new dem_mesh( );
		v_dem_mesh->m_interior_index = index;
		
		// read file head	
		us_square_head v_file_head;
		stream->read( &v_file_head, sizeof(v_file_head) );

		// 读取顶点序列
		//us_vertex_segment v_ver_segment;
		//memset(&v_ver_segment, 0, sizeof(v_ver_segment));
		//stream->read(&v_ver_segment, sizeof(v_ver_segment));
		//us_grid_segment_ex v_ver_seg_ex;
		//memset(&v_ver_seg_ex, 0, sizeof(v_ver_seg_ex));
		//stream->read(&v_ver_seg_ex, sizeof(v_ver_seg_ex));

		////////////////////////////////////////////////////////////////////////////
		//int v_verts_count = v_ver_segment.data_size / sizeof(unsigned short) / 3;
		//// read vertex data
		//unsigned short* v_vertex_data = new unsigned short[v_verts_count * 3];
		//stream->read( v_vertex_data, v_ver_segment.data_size );

		//lon_lat_rect v_bound = v_dem_mesh->m_sub_mesh[0]->get_bound();
		////double v_elevation_scale = (v_ver_seg_ex.elevation_max - v_ver_seg_ex.elevation_min) / (double)v_ver_seg_ex.divide_number;
		////double v_horizonal_scale = (v_bound.get_width() * 2 ) / (double)v_ver_seg_ex.divide_number;
		//double v_west = v_bound.m_west;
		//double v_south = v_bound.m_south;
		//int v_verts_size = sizeof( position_color_texture2 ) * v_verts_count;

		// 读取索引
		for( int ni = 0;ni < 4; ni++ )
		{
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// 读文件头
			us_vertex_segment v_ver_segment;
			memset(&v_ver_segment, 0, sizeof(v_ver_segment));
			stream->read(&v_ver_segment, sizeof(v_ver_segment));
			us_grid_segment_ex v_ver_seg_ex;
			memset(&v_ver_seg_ex, 0, sizeof(v_ver_seg_ex));
			stream->read(&v_ver_seg_ex, sizeof(v_ver_seg_ex));

			if(v_ver_segment.data_size == 0) continue;

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////		
			dem_usx_sub_mesh* v_sub_mesh= new dem_usx_sub_mesh( index, ni);
			v_dem_mesh->m_sub_mesh[ni]  = v_sub_mesh;

			lon_lat_rect v_bound = v_dem_mesh->m_sub_mesh[ni]->get_bound();
			vector3<double> v_center_sphr = v_bound.get_center();
			vector3<double> v_center_geo = geocentric_coords::from_spherical_d( v_center_sphr.x, v_center_sphr.y, spherical_coords::s_semi_major );

			managed_buffer* v_mesh_buffer = new managed_buffer();
			v_mesh_buffer->resize(v_ver_segment.data_size);
			stream->read(v_mesh_buffer->get_buffer(), v_ver_segment.data_size);
			
			//////////////////////////////////////////////////////////////////////////
			usx_parser_object_container v_container;
			v_container.set_path( s_b->get_path().c_str() );

			ustring str_path = v_container.get_path();
			str_path += string_ext::wfrom_dword(interior_index_factory::get_level(s_b->get_index())) + L"\\tex\\";
			v_container.set_path(str_path.c_str());

			if ( !singleton_usx_parser_assistant::instance().parse_x_buffer( v_mesh_buffer->get_buffer(), v_mesh_buffer->get_size(), &v_container ) ) 
			{
				AUTO_DELETE(v_mesh_buffer);
				continue;
			}

			usx_mesh_frame_map::iterator itr = v_container.m_mesh_frame_map.begin();
			if( v_container.m_mesh_frame_map.size() == 1 && itr->second == NULL  )
			{
				v_sub_mesh->m_hardware_mesh =  hardware_mesh_factory::create_single_mesh( s_b->get_document(), itr->first, &v_container );
				v_sub_mesh->m_hardware_mesh->add_ref();

				// 读取纹理
				//d3d9_hardware_mesh* v_hardware_mesh = (d3d9_hardware_mesh*)(v_sub_mesh->m_hardware_mesh);

				//s_b->get_path();
				//v_hardware_mesh->m_material_entry_array
			}
			else
			{
				v_sub_mesh->m_hardware_mesh = hardware_mesh_factory::create_complex_mesh( s_b->get_document(), &v_container );
				v_sub_mesh->m_hardware_mesh->add_ref();
			}

			AUTO_DELETE(v_mesh_buffer);
		}
		//AUTO_DELETE_ARRAY(v_vertex_data);
		v_dem_mesh->add_ref();
		return (object_base*) v_dem_mesh;
	}

	object_base* dem_mesh_filter_v2::parse( raw_buffer* in_buffer )
	{
		interior_index v_index = ((square_buffer*)in_buffer)->get_index();

		return parse_from_compressed_stream( in_buffer, v_index );
	}

	object_base* dem_mesh_filter_v2::parse_from_compressed_stream( raw_buffer* stream, interior_index index )
	{
		//square_buffer* s_b = (square_buffer* )stream;

		//dem_usx_mesh* v_dem_mesh = new dem_usx_mesh();

		//// read file head	
		//us_square_head v_file_head;
		//stream->read( &v_file_head, sizeof(v_file_head) );

		//segment_head_aux v_aux_head;
		//stream->read( &v_aux_head, sizeof(v_aux_head) );

		//if(v_aux_head.segment_child_count == 0 ) return NULL;
		//v_dem_mesh->m_sub_mesh_array.resize(v_aux_head.segment_child_count, 0);

		//// 读取索引
		//for( int ni = 0;ni < v_aux_head.segment_child_count; ni++ )
		//{
		//	dem_usx_sub_mesh* v_sub_mesh = NULL;
		//	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//	// 读node
		//	segment_head_node v_node_head;
		//	memset(&v_node_head, 0, sizeof(v_node_head));
		//	stream->read(&v_node_head, sizeof(v_node_head));

		//	ustring str_hild_node_name;
		//	lon_lat_rect v_bound;
		//	vector_3d v_center_sphr;
		//	vector_3d v_min_boundary;
		//	vector_3d v_max_boundary;
		//	stream->read( str_hild_node_name );
		//	stream->read( &v_bound, sizeof(v_bound));
		//	stream->read( &v_center_sphr, sizeof(v_center_sphr));
		//	stream->read( &v_min_boundary, sizeof(v_min_boundary));
		//	stream->read( &v_max_boundary, sizeof(v_max_boundary));


		//	// 读mesh data
		//	segment_head_mesh_data v_segment_head_mesh;
		//	memset(&v_segment_head_mesh, 0, sizeof(v_segment_head_mesh));
		//	stream->read(&v_segment_head_mesh, sizeof(v_segment_head_mesh));

		//	if(v_segment_head_mesh.data_length > 0)
		//	{
		//		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////		
		//		v_sub_mesh= new dem_usx_sub_mesh( index, ni);
		//		v_sub_mesh->add_ref();
		//		v_dem_mesh->m_sub_mesh_array[ni] = v_sub_mesh;
		//		v_sub_mesh->m_next_source = str_hild_node_name;
		//		v_sub_mesh->m_bound = v_bound;
		//		v_sub_mesh->m_center_geo = geocentric_coords::from_spherical_d(v_center_sphr.x, v_center_sphr.y, v_center_sphr.z * 0.001 + spherical_coords::s_semi_major);
		//		v_sub_mesh->m_aabb.set_extent(v_min_boundary, v_max_boundary);

		//		//lon_lat_rect v_bound = v_dem_mesh->m_sub_mesh_array[ni]->get_bound();
		//		//vector3<double> v_center_sphr = v_bound.get_center();
		//		//vector3<double> v_center_geo = geocentric_coords::from_spherical_d( v_center_sphr.x, v_center_sphr.y, spherical_coords::s_semi_major );

		//		managed_buffer* v_mesh_buffer = new managed_buffer();
		//		v_mesh_buffer->resize(v_segment_head_mesh.data_length);
		//		stream->read(v_mesh_buffer->get_buffer(), v_segment_head_mesh.data_length);

		//		//////////////////////////////////////////////////////////////////////////
		//		usx_parser_object_container v_container;
		//		v_container.set_path( s_b->get_path().c_str() );

		//		//ustring str_path = v_container.get_path();
		//		//str_path += string_ext::wfrom_dword(interior_index_factory::get_level(s_b->get_index())) + L"\\tex\\";
		//		//v_container.set_path(str_path.c_str());

		//		if ( singleton_usx_parser_assistant::instance().parse_x_buffer( v_mesh_buffer->get_buffer(), v_mesh_buffer->get_size(), &v_container ) ) 
		//		{
		//			usx_mesh_frame_map::iterator itr = v_container.m_mesh_frame_map.begin();
		//			if( v_container.m_mesh_frame_map.size() == 1 && itr->second == NULL  )
		//			{
		//				v_sub_mesh->m_hardware_mesh =  hardware_mesh_factory::create_single_mesh( s_b->get_document(), itr->first, &v_container );
		//				v_sub_mesh->m_hardware_mesh->add_ref();
		//			}
		//			else
		//			{
		//				v_sub_mesh->m_hardware_mesh = hardware_mesh_factory::create_complex_mesh( s_b->get_document(), &v_container );
		//				v_sub_mesh->m_hardware_mesh->add_ref();
		//			}
		//		}

		//		AUTO_DELETE(v_mesh_buffer);
		//	}

		//	

		//	//////////////////////////////////////////////////////////////////////////
		//	// 读texture data
		//	segment_head_texture_data v_segment_head_texture;
		//	memset(&v_segment_head_texture, 0, sizeof(v_segment_head_texture));
		//	stream->read(&v_segment_head_texture, sizeof(v_segment_head_texture));

		//	if(v_segment_head_texture.data_length > 0)
		//	{
		//		managed_buffer* v_texture_buffer = new managed_buffer();
		//		v_texture_buffer->resize(v_segment_head_texture.data_length);
		//		stream->read(v_texture_buffer->get_buffer(), v_segment_head_texture.data_length);

		//		if(v_sub_mesh != NULL)
		//		{
		//			v_sub_mesh->m_hardware_texture = dynamic_cast<hardware_texture*>(m_filter_manager->parse(v_texture_buffer));
		//		}
		//		AUTO_DELETE(v_texture_buffer);

		//	}
		//}
		////AUTO_DELETE_ARRAY(v_vertex_data);
		//v_dem_mesh->add_ref();
		//return (object_base*) v_dem_mesh;
		return false;
	}


	object_base* dem_mesh_filter_v3::parse( raw_buffer* in_buffer )
	{
		interior_index v_index = ((square_buffer*)in_buffer)->get_index();

		return parse_from_compressed_stream( in_buffer, v_index );
	}

	object_base* dem_mesh_filter_v3::parse_from_compressed_stream( raw_buffer* stream, interior_index index )
	{
		square_buffer* s_b = (square_buffer* )stream;

		dem_usx_mesh* v_dem_mesh = new dem_usx_mesh();
		axis_aligned_box_d all_box;
		all_box.make_invalid();

		// read file head	
		us_square_head v_file_head;
		stream->read( &v_file_head, sizeof(v_file_head) );

		segment_head_aux v_aux_head;
		stream->read( &v_aux_head, sizeof(v_aux_head) );

		if(v_aux_head.segment_child_count == 0 ) return NULL;
		v_dem_mesh->m_sub_mesh_array.resize(v_aux_head.segment_child_count, 0);

		// 读取索引
		for( int ni = 0;ni < v_aux_head.segment_child_count; ni++ )
		{
			dem_usx_sub_mesh* v_sub_mesh = NULL;
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// 读node
			segment_head_node v_node_head;
			memset(&v_node_head, 0, sizeof(v_node_head));
			stream->read(&v_node_head, sizeof(v_node_head));

			ustring str_hild_node_name;
			lon_lat_rect v_bound;
			vector_3d v_center_sphr;
			vector_3d v_min_boundary;
			vector_3d v_max_boundary;
			int v_texture_index;
			stream->read( str_hild_node_name );
			stream->read( &v_bound, sizeof(v_bound));
			stream->read( &v_center_sphr, sizeof(v_center_sphr));
			stream->read( &v_min_boundary, sizeof(v_min_boundary));
			stream->read( &v_max_boundary, sizeof(v_max_boundary));
			stream->read( &v_texture_index, sizeof(v_texture_index));

			all_box.combine( v_min_boundary );
			all_box.combine( v_max_boundary );

			// 读mesh data
			segment_head_mesh_data v_segment_head_mesh;
			memset(&v_segment_head_mesh, 0, sizeof(v_segment_head_mesh));
			stream->read(&v_segment_head_mesh, sizeof(v_segment_head_mesh));

			if(v_segment_head_mesh.data_length > 0)
			{
				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////		
				v_sub_mesh= new dem_usx_sub_mesh( index, ni);
				v_sub_mesh->add_ref();
				v_dem_mesh->m_sub_mesh_array[ni] = v_sub_mesh;
				v_sub_mesh->m_next_source = str_hild_node_name;
				v_sub_mesh->m_bound = v_bound;
				v_sub_mesh->m_center_geo = geocentric_coords::from_spherical_d(v_center_sphr.x, v_center_sphr.y, v_center_sphr.z * 0.001 + spherical_coords::s_semi_major + 0.1);
				v_sub_mesh->m_aabb.set_extent(v_min_boundary, v_max_boundary);
				v_sub_mesh->m_texture_index = v_texture_index;
				//lon_lat_rect v_bound = v_dem_mesh->m_sub_mesh_array[ni]->get_bound();
				//vector3<double> v_center_sphr = v_bound.get_center();
				//vector3<double> v_center_geo = geocentric_coords::from_spherical_d( v_center_sphr.x, v_center_sphr.y, spherical_coords::s_semi_major );

				managed_buffer* v_mesh_buffer = new managed_buffer();
				v_mesh_buffer->resize(v_segment_head_mesh.data_length);
				stream->read(v_mesh_buffer->get_buffer(), v_segment_head_mesh.data_length);

				//////////////////////////////////////////////////////////////////////////
				usx_parser_object_container v_container;
				v_container.set_path( s_b->get_path().c_str() );

				//ustring str_path = v_container.get_path();
				//str_path += string_ext::wfrom_dword(interior_index_factory::get_level(s_b->get_index())) + L"\\tex\\";
				//v_container.set_path(str_path.c_str());

				if ( singleton_usx_parser_assistant::instance().parse_x_buffer( v_mesh_buffer->get_buffer(), v_mesh_buffer->get_size(), &v_container ) ) 
				{
					usx_mesh_frame_map::iterator itr = v_container.m_mesh_frame_map.begin();
					if( v_container.m_mesh_frame_map.size() == 1 && itr->second == NULL  )
					{
						v_sub_mesh->m_hardware_mesh =  hardware_mesh_factory::create_single_mesh( s_b->get_document(), itr->first, &v_container );
						v_sub_mesh->m_hardware_mesh->add_ref();
					}
					else
					{
						v_sub_mesh->m_hardware_mesh = hardware_mesh_factory::create_complex_mesh( s_b->get_document(), &v_container );
						v_sub_mesh->m_hardware_mesh->add_ref();
					}
				}

				AUTO_DELETE(v_mesh_buffer);
			}
		}

		segment_head_aux v_aux_texture_head;
		stream->read( &v_aux_texture_head, sizeof(v_aux_head) );

		if(v_aux_texture_head.segment_child_count == 0 ) return NULL;
		v_dem_mesh->m_sub_texture_array.resize(v_aux_texture_head.segment_child_count, 0);
		for( int ni = 0;ni < v_aux_texture_head.segment_child_count; ni++ )
		{
			//////////////////////////////////////////////////////////////////////////
			// 读texture data
			segment_head_texture_data v_segment_head_texture;
			memset(&v_segment_head_texture, 0, sizeof(v_segment_head_texture));
			stream->read(&v_segment_head_texture, sizeof(v_segment_head_texture));

			if(v_segment_head_texture.data_length > 0)
			{
				managed_buffer* v_texture_buffer = new managed_buffer();
				v_texture_buffer->resize(v_segment_head_texture.data_length);
				stream->read(v_texture_buffer->get_buffer(), v_segment_head_texture.data_length);
				hardware_texture* v_hardware_texture = dynamic_cast<hardware_texture*>(m_filter_manager->parse(v_texture_buffer));
				v_dem_mesh->m_sub_texture_array[ni] = v_hardware_texture;
				AUTO_DELETE(v_texture_buffer);
			}
		}

		for(int ni = 0; ni < v_dem_mesh->m_sub_mesh_array.size(); ni++ )
		{
			dem_usx_sub_mesh* v_sub_mesh = v_dem_mesh->m_sub_mesh_array[ni];
			if(v_sub_mesh != NULL)
			{
				v_sub_mesh->m_hardware_texture = v_dem_mesh->m_sub_texture_array[v_sub_mesh->m_texture_index];
			}

		}
		//AUTO_DELETE_ARRAY(v_vertex_data);

		v_dem_mesh->set_bound_box( all_box );
		
		v_dem_mesh->add_ref();
		return (object_base*) v_dem_mesh;
	}

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	object_base* dem_mesh_filter_v4::parse( raw_buffer* in_buffer )
	{
		interior_index v_index = ((square_buffer*)in_buffer)->get_index();

		return parse_from_compressed_stream( in_buffer, v_index );
	}

	object_base* dem_mesh_filter_v4::parse_from_compressed_stream( raw_buffer* stream, interior_index index )
	{
		square_buffer* s_b = (square_buffer* )stream;

		dem_usx_mesh* v_dem_mesh = new dem_usx_mesh();
		
		axis_aligned_box_d all_box;
		all_box.make_invalid();

		// read file head	
		us_square_head v_file_head;
		stream->read( &v_file_head, sizeof(v_file_head) );

		int segment_child_count;
		stream->read( &segment_child_count, sizeof(segment_child_count) );

		if(segment_child_count == 0 ) return NULL;
		v_dem_mesh->m_sub_mesh_array.resize(segment_child_count, 0);

		// 读取索引
		for( int ni = 0;ni < segment_child_count; ni++ )
		{
			dem_usx_sub_mesh* v_sub_mesh = NULL;
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// 读node
			//segment_head_node v_node_head;
			//memset(&v_node_head, 0, sizeof(v_node_head));
			//stream->read(&v_node_head, sizeof(v_node_head));

			
			string str_child_node_name;
			lon_lat_rect v_bound;
			vector_3d v_center_sphr;
			vector_3d v_min_boundary;
			vector_3d v_max_boundary;
			int v_texture_index;
			__int64 patch_offset = 0;
			int patch_cursor = stream->get_cursor();
			stream->read( &patch_offset, sizeof(patch_offset));
			stream->read( &v_bound, sizeof(v_bound));
			stream->read( &v_center_sphr, sizeof(v_center_sphr));
			stream->read( &v_min_boundary, sizeof(v_min_boundary));
			stream->read( &v_max_boundary, sizeof(v_max_boundary));
			stream->read( &v_texture_index, sizeof(v_texture_index));

			all_box.combine( v_min_boundary );
			all_box.combine( v_max_boundary );
			{ int temp = v_min_boundary.y; v_min_boundary.y = v_min_boundary.z; v_min_boundary.z = temp; }
			{ int temp = v_max_boundary.y; v_max_boundary.y = v_max_boundary.z; v_max_boundary.z = temp; }

			// 读mesh data
			segment_head_mesh_data v_segment_head_mesh;
			memset(&v_segment_head_mesh, 0, sizeof(v_segment_head_mesh));
			stream->read(&v_segment_head_mesh, sizeof(v_segment_head_mesh));

			int data_cursor = stream->get_cursor();
			int data_offset = 0;
			stream->read(&data_offset, sizeof(data_offset));
			

			if(v_segment_head_mesh.data_length > 0)
			{
				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////		
				v_sub_mesh= new dem_usx_sub_mesh( index, ni);
				v_sub_mesh->add_ref();
				v_dem_mesh->m_sub_mesh_array[ni] = v_sub_mesh;
				v_sub_mesh->m_bound = v_bound;
				v_sub_mesh->m_center_geo = geocentric_coords::from_spherical_d(v_center_sphr.x, v_center_sphr.y, v_center_sphr.z * 0.001 + spherical_coords::s_semi_major + 0.1);
				v_sub_mesh->m_aabb.set_extent(v_min_boundary, v_max_boundary);
				v_sub_mesh->m_texture_index = v_texture_index;

				//////////////////////////////////////////////////////////////////////////
				int VertexNumber = 0;
				int FaceNumber = 0;
				stream->read( &VertexNumber, sizeof(VertexNumber));
				stream->read( &FaceNumber, sizeof(FaceNumber));

				d3d9_hardware_mesh* in_hardware_mesh = new d3d9_hardware_mesh();
				in_hardware_mesh->set_name( L"" );

				in_hardware_mesh->m_fvf = USFVF_XYZ | USFVF_TEX1;
				in_hardware_mesh->m_vertex_stride = 20;

				in_hardware_mesh->m_mesh_aabb.set_extent( v_min_boundary, v_max_boundary );
				in_hardware_mesh->set_bound_box( in_hardware_mesh->m_mesh_aabb );
				in_hardware_mesh->set_32bit_index( false );

				in_hardware_mesh->m_num_of_vertices = VertexNumber;
				in_hardware_mesh->m_vertex_buffer_size = in_hardware_mesh->m_vertex_stride * in_hardware_mesh->m_num_of_vertices;
				in_hardware_mesh->m_vertex_buffer = new unsigned char[in_hardware_mesh->m_vertex_buffer_size];

				// position buffer
				int offset = in_hardware_mesh->get_offset(USFVF_XYZ);
				vector_3f* vertexBuffer = (vector_3f*)(stream->get_buffer() + stream->get_cursor());
				for(int nj = 0; nj < VertexNumber; nj++)
				{
					in_hardware_mesh->set_vector3_rh( offset, nj, vertexBuffer + nj );
				}
				stream->seek_to(stream->get_cursor() + VertexNumber * sizeof(vector_3f) );

				// texture coords buffer
				offset = in_hardware_mesh->get_offset(USFVF_TEX0);
				vector_2f* texcoordBuffer = (vector_2f*)(stream->get_buffer() + stream->get_cursor());
				for(int nj = 0; nj < VertexNumber; nj++)
				{
					in_hardware_mesh->set_vector2( offset, nj, texcoordBuffer + nj );
				}
				stream->seek_to(stream->get_cursor() + VertexNumber * sizeof(vector_2f) );



				// indices buffer
				in_hardware_mesh->m_num_of_triangles = FaceNumber;
				in_hardware_mesh->m_index_buffer = new ushort[ in_hardware_mesh->m_num_of_triangles * 3 ];
				ushort* v_buffer = (ushort*)in_hardware_mesh->m_index_buffer;
				in_hardware_mesh->m_index_buffer_size = sizeof(ushort) * in_hardware_mesh->m_num_of_triangles * 3;
				stream->read(in_hardware_mesh->m_index_buffer, FaceNumber * sizeof(ushort) * 3 );

				for(unsigned int cur_face = 0; cur_face < FaceNumber;cur_face++)
				{
					ushort temp = v_buffer[cur_face * 3 + 1];
					v_buffer[cur_face * 3 + 1] = v_buffer[cur_face * 3 + 2];
					v_buffer[cur_face * 3 + 2] = temp;
				}
			
				short string_count = 0;
				string str_texture_name = "";
				stream->read( &string_count, sizeof(string_count));
				stream->read( str_texture_name );

				stream->seek_to(data_cursor + data_offset);
				stream->read( &string_count, sizeof(string_count));
				stream->read( str_child_node_name );
				stream->seek_to(patch_cursor+ patch_offset);

				v_sub_mesh->m_next_source = string_ext::to_wstring(str_child_node_name);

				// attribute tabel
				in_hardware_mesh->m_attribute_buffer = new ulong[in_hardware_mesh->m_num_of_triangles];
				in_hardware_mesh->m_attribute_buffer_size = sizeof( ulong ) * in_hardware_mesh->m_num_of_triangles;
				memset( in_hardware_mesh->m_attribute_buffer, 0, in_hardware_mesh->m_attribute_buffer_size );

				// texture
				in_hardware_mesh->m_material_entry_array.resize(1);
				in_hardware_mesh->m_texture_array.resize( 1 );
				

				in_hardware_mesh->add_ref();
				v_sub_mesh->m_hardware_mesh = in_hardware_mesh;





				//lon_lat_rect v_bound = v_dem_mesh->m_sub_mesh_array[ni]->get_bound();
				//vector3<double> v_center_sphr = v_bound.get_center();
				//vector3<double> v_center_geo = geocentric_coords::from_spherical_d( v_center_sphr.x, v_center_sphr.y, spherical_coords::s_semi_major );

				//managed_buffer* v_mesh_buffer = new managed_buffer();
				//v_mesh_buffer->resize(v_segment_head_mesh.data_length);
				//stream->read(v_mesh_buffer->get_buffer(), v_segment_head_mesh.data_length);

				//////////////////////////////////////////////////////////////////////////
				//usx_parser_object_container v_container;
				//v_container.set_path( s_b->get_path().c_str() );

				//ustring str_path = v_container.get_path();
				//str_path += string_ext::wfrom_dword(interior_index_factory::get_level(s_b->get_index())) + L"\\tex\\";
				//v_container.set_path(str_path.c_str());

				//if ( singleton_usx_parser_assistant::instance().parse_x_buffer( v_mesh_buffer->get_buffer(), v_mesh_buffer->get_size(), &v_container ) ) 
				//{
				//	usx_mesh_frame_map::iterator itr = v_container.m_mesh_frame_map.begin();
				//	if( v_container.m_mesh_frame_map.size() == 1 && itr->second == NULL  )
				//	{
				//		v_sub_mesh->m_hardware_mesh =  hardware_mesh_factory::create_single_mesh( s_b->get_document(), itr->first, &v_container );
				//		v_sub_mesh->m_hardware_mesh->add_ref();
				//	}
				//	else
				//	{
				//		v_sub_mesh->m_hardware_mesh = hardware_mesh_factory::create_complex_mesh( s_b->get_document(), &v_container );
				//		v_sub_mesh->m_hardware_mesh->add_ref();
				//	}
				//}

				//AUTO_DELETE(v_mesh_buffer);
			}
		}

		segment_head_aux v_aux_texture_head;
		stream->read( &v_aux_texture_head, sizeof(v_aux_texture_head) );

		if(v_aux_texture_head.segment_child_count == 0 ) return NULL;
		v_dem_mesh->m_sub_texture_array.resize(v_aux_texture_head.segment_child_count, 0);
		for( int ni = 0;ni < v_aux_texture_head.segment_child_count; ni++ )
		{
			//////////////////////////////////////////////////////////////////////////
			// 读texture data
			segment_head_texture_data v_segment_head_texture;
			memset(&v_segment_head_texture, 0, sizeof(v_segment_head_texture));
			stream->read(&v_segment_head_texture, sizeof(v_segment_head_texture));

			if(v_segment_head_texture.data_length > 0)
			{
				managed_buffer* v_texture_buffer = new managed_buffer();
				v_texture_buffer->resize(v_segment_head_texture.data_length);
				stream->read(v_texture_buffer->get_buffer(), v_segment_head_texture.data_length);
				hardware_texture* v_hardware_texture = dynamic_cast<hardware_texture*>(m_filter_manager->parse(v_texture_buffer));
				v_dem_mesh->m_sub_texture_array[ni] = v_hardware_texture;
				AUTO_DELETE(v_texture_buffer);
			}
		}

		for(int ni = 0; ni < v_dem_mesh->m_sub_mesh_array.size(); ni++ )
		{
			dem_usx_sub_mesh* v_sub_mesh = v_dem_mesh->m_sub_mesh_array[ni];
			if(v_sub_mesh != NULL)
			{
				v_sub_mesh->m_hardware_texture = v_dem_mesh->m_sub_texture_array[v_sub_mesh->m_texture_index];
			}

		}
		//AUTO_DELETE_ARRAY(v_vertex_data);
		v_dem_mesh->set_bound_box( all_box );
		v_dem_mesh->add_ref();
		return (object_base*) v_dem_mesh;
	}	
}