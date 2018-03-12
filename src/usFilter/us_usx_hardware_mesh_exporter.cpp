///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_usx_hardware_mesh_exporter.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : usx_hardware_mesh_exporter class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_usx_hardware_mesh_exporter.h"

namespace uniscope_globe
{
	bool usx_hardware_mesh_exporter::do_export( ID3DXFileSaveObject* save_obj, d3d9_hardware_mesh* v_mesh, cpstr dest_path, axis_aligned_box<float>& out_box )
	{
		//d3d9_hardware_mesh_buffer* v_buffer = (d3d9_hardware_mesh_buffer*)v_mesh->get_raw_buffer();
		v_mesh->lock();

		d3d9_hardware_mesh_buffer* v_buffer = dynamic_cast<d3d9_hardware_mesh_buffer*>(v_mesh);

		// get count;
		ulong v_face_number = v_buffer->m_num_of_triangles;
		ulong v_verts_number = v_buffer->m_num_of_vertices;

		// get stride;
		ulong v_index_stride  = v_buffer->m_index_buffer_size / v_buffer->m_num_of_triangles;
		ulong v_vertex_stride = v_buffer->m_vertex_stride;

		// lock vertices;
		byte* v_vertices= (byte*)v_buffer->m_vertex_buffer;
		byte* v_indices = (byte*)v_buffer->m_index_buffer;

		// get attribute
		DWORD* v_attribute = (DWORD*)v_buffer->m_attribute_buffer;

		int v_texture_count = ((int)(v_buffer->m_fvf) & D3DFVF_TEXCOUNT_MASK) >> 8;

		int v_src_stride_offset = 0;


		//////////////////////////////////////////////////////////////////////////////////////////
		// Save Index
		//////////////////////////////////////////////////////////////////////////////////////////
		io_stream index_stream;
		index_stream.write( v_face_number );
		if( v_buffer->m_32bit_index )
		{
			ulong dw_triangle_index[4];
			dw_triangle_index[0] = 3;
			for( int ni = 0; ni < (int)v_face_number; ni++ ) 
			{
				dw_triangle_index[1] = *((ulong*)(v_indices + v_index_stride * ni));
				dw_triangle_index[2] = *((ulong*)(v_indices + v_index_stride * ni + 4));
				dw_triangle_index[3] = *((ulong*)(v_indices + v_index_stride * ni + 8));

				index_stream.write( dw_triangle_index );
			}
		}
		else
		{

			ulong dw_triangle_index[4];
			dw_triangle_index[0] = 3;
			for( int ni = 0; ni < (int)v_face_number; ni++ ) 
			{
				dw_triangle_index[1] = *((ushort*)(v_indices + v_index_stride * ni));
				dw_triangle_index[2] = *((ushort*)(v_indices + v_index_stride * ni + 2));
				dw_triangle_index[3] = *((ushort*)(v_indices + v_index_stride * ni + 4));

				index_stream.write( dw_triangle_index );
			}
		}

		//////////////////////////////////////////////////////////////////////////////////////////
		// Save Vertex Segment
		//////////////////////////////////////////////////////////////////////////////////////////
		io_stream verts_stream;
		verts_stream.write( v_verts_number );

		//////////////////////////////////////////////////////////////////////////////////////////
		// Save Vertex
		//////////////////////////////////////////////////////////////////////////////////////////
		{
			int dest_stride = sizeof(float) * 3;
			for (int ni = 0; ni < (int)v_verts_number; ni++)
			{
				verts_stream.write(v_vertices + v_vertex_stride * ni, dest_stride);

				out_box.combine( vector3<float>( (float*)(v_vertices + v_vertex_stride * ni) ) );
			}
			v_src_stride_offset += dest_stride;
		}

		// 保存面索引
		verts_stream.write(index_stream.get_data(), (size_t)index_stream.get_size());

		// Add to SaveObject
		string str_name = string_ext::from_wstring(v_mesh->get_name());
		CComPtr<ID3DXFileSaveData> dx_save_data;
		if ( FAILED( save_obj->AddDataObject(TID_D3DRMMesh, (char*)str_name.c_str(), NULL, verts_stream.get_size(), verts_stream.get_data(), &dx_save_data) ) )
		{
			return false;
		}


		//////////////////////////////////////////////////////////////////////////////////////////
		// Save Normal
		//////////////////////////////////////////////////////////////////////////////////////////
		if( v_buffer->m_fvf & D3DFVF_NORMAL )
		{
			// 保存法向量
			io_stream stream;
			stream.write( v_verts_number );

			int dest_stride = sizeof(float) * 3;
			for (int ni = 0; ni < (int)v_verts_number; ni++)
			{
				stream.write( v_vertices + v_vertex_stride * ni + v_src_stride_offset, dest_stride );
			}
			v_src_stride_offset += dest_stride;

			// 保存面索引
			stream.write(index_stream.get_data(), index_stream.get_size());

			CComPtr<ID3DXFileSaveData> normal_data;
			if ( FAILED( dx_save_data->AddDataObject(TID_D3DRMMeshNormals, "normals", 0, stream.get_size(), stream.get_data(), &normal_data) ) )
			{
				return false;
			}
		}

		//////////////////////////////////////////////////////////////////////////////////////////
		// Save Diffuse
		//////////////////////////////////////////////////////////////////////////////////////////
		if( v_buffer->m_fvf & D3DFVF_DIFFUSE )
		{
			// 保存法向量
			io_stream stream;
			stream.write( v_verts_number );

			int dest_stride = sizeof(ulong);
			for (int ni = 0; ni < (int)v_verts_number; ni++)
			{
				stream.write( ni );
				
				color_value v_color_value;
				ulong v_color = *((ulong*)(v_vertices + v_vertex_stride * ni + v_src_stride_offset));
				USCOLOR_ULONG_TO_COLORVALUE(v_color_value, v_color);
				stream.write( (byte*)&v_color_value, dest_stride * 4 );
			}
			v_src_stride_offset += dest_stride;

			CComPtr<ID3DXFileSaveData> diffuse_data;
			if ( FAILED( dx_save_data->AddDataObject(TID_D3DRMMeshVertexColors, "col0", 0, stream.get_size(), stream.get_data(), &diffuse_data) ) )
			{
				return false;
			}

		}


		
		//////////////////////////////////////////////////////////////////////////////////////////
		// Save Texture0
		//////////////////////////////////////////////////////////////////////////////////////////
		if ( v_texture_count > 0)
		{
			// 保存第一层纹理坐标
			io_stream stream;
			stream.write( v_verts_number );

			int dest_stride = sizeof(float) * 2;
			for (int ni = 0; ni < (int)v_verts_number; ni++)
			{
				stream.write( v_vertices + v_vertex_stride * ni + v_src_stride_offset, dest_stride);
			}
			v_src_stride_offset += dest_stride;

			CComPtr<ID3DXFileSaveData> color_data;
			if ( FAILED( dx_save_data->AddDataObject(TID_D3DRMMeshTextureCoords, "tc0", 0, stream.get_size(), stream.get_data(), &color_data) ) )
			{
				return false;
			}
		}

		//////////////////////////////////////////////////////////////////////////////////////////
		// Save Texture1
		//////////////////////////////////////////////////////////////////////////////////////////
		if( v_texture_count > 1 )
		{
			D3DVERTEXELEMENT9 elems[4] = {  {0xFF,0,D3DDECLTYPE_UNUSED, 0,0,0},
			{0xFF,0,D3DDECLTYPE_UNUSED, 0,0,0},
			{0xFF,0,D3DDECLTYPE_UNUSED, 0,0,0},
			{0xFF,0,D3DDECLTYPE_UNUSED, 0,0,0} };
			int ni = 0;
			WORD dw_offset = 0;
			{
				D3DVERTEXELEMENT9 ve = { 0, dw_offset, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 };
				elems[ni] = ve;
				ni++;
				dw_offset += 8;
			}

			// 写入头信息
			io_stream stream;
			stream.write( ni );

			for( int nj = 0; nj < ni; nj++ ) 
			{
				stream.write( (ulong)elems[nj].Type);
				stream.write( (ulong)elems[nj].Method);
				stream.write( (ulong)elems[nj].Usage);
				stream.write( (ulong)elems[nj].UsageIndex);
			}

			ulong dw_size = dw_offset / 4 * v_verts_number;
			stream.write( dw_size );

			// 写入数据
			int dest_stride = sizeof(float) * 2;
			for (int ni = 0; ni < (int)v_verts_number; ni++)
			{
				stream.write(v_vertices + v_vertex_stride * ni + v_src_stride_offset, dest_stride);
			}
			v_src_stride_offset += dest_stride;


			CComPtr<ID3DXFileSaveData> dcls_data;
			if ( FAILED( dx_save_data->AddDataObject(DXFILEOBJ_DeclData, 0, 0, stream.get_size(), stream.get_data(), &dcls_data) ) )
			{
				return false;
			}
		}

		//////////////////////////////////////////////////////////////////////////////////////////
		// Save Materials
		//////////////////////////////////////////////////////////////////////////////////////////
		int v_material_count = v_mesh->m_material_entry_array.size();
		if (v_material_count > 0)
		{
			io_stream stream;
			stream.write( v_material_count );
			stream.write( v_face_number );
			stream.write( v_attribute, v_face_number );

			CComPtr<ID3DXFileSaveData> attribute_data;
			if ( FAILED( dx_save_data->AddDataObject(TID_D3DRMMeshMaterialList, "mtls", 0, stream.get_size(), stream.get_data(), &attribute_data) ) )
			{
				return false;
			}

			// export material
			for (int ni = 0; ni < v_material_count; ni++)
			{
				// save light material

				io_stream material_stream;
				material_stream.write( v_mesh->m_material_entry_array[ni].m_diffuse_color );
				material_stream.write( v_mesh->m_material_entry_array[ni].m_power );
				material_stream.write( ((byte*)&v_mesh->m_material_entry_array[ni].m_specular_color), 12 );
				material_stream.write( ((byte*)&v_mesh->m_material_entry_array[ni].m_emissive_color), 12 );


				string mat_name = string_ext::from_wstring(v_mesh->m_material_entry_array[ni].m_name);
				CComPtr<ID3DXFileSaveData> mat_data;
				if ( FAILED( attribute_data->AddDataObject(TID_D3DRMMaterial, mat_name.c_str(), 0, material_stream.get_size(), material_stream.get_data(), &mat_data) ) )
				{
					return false;
				}

				// save texture file name
				for ( int nj = 0; nj < v_texture_count; nj++ )
				{
					if( nj == 0 )
					{
						if (v_mesh->m_material_entry_array[ni].m_diffuse != NULL)
						{
							ustring wstr_src_file_path = v_mesh->m_material_entry_array[ni].m_diffuse->get_key();
							ustring wstr_src_file_name = file_directory::get_file_name(wstr_src_file_path.c_str());
							ustring wstr_dest_directory = file_directory::get_directory(dest_path);
							ustring wstr_dest_file_path = usx_exporter_assistant_ex::export_resource(wstr_src_file_path.c_str(), wstr_dest_directory.c_str(), wstr_src_file_name.c_str());
							string str_file = string_ext::from_wstring(wstr_dest_file_path);


							CComPtr<ID3DXFileSaveData> text_name;
							if ( FAILED( mat_data->AddDataObject(TID_D3DRMTextureFilename, "Diffuse", 0, str_file.size() + 1, str_file.c_str(), &text_name) ) )
							{
								return false;
							}
						}
					}
					else if( nj = 1)
					{
						if (v_mesh->m_material_entry_array[ni].m_self_illuminant != NULL)
						{
							ustring wstr_src_file_path = v_mesh->m_material_entry_array[ni].m_self_illuminant->get_key();
							ustring wstr_src_file_name = file_directory::get_file_name(wstr_src_file_path.c_str());
							ustring wstr_dest_directory = file_directory::get_directory(dest_path);
							ustring wstr_dest_file_path = usx_exporter_assistant_ex::export_resource(wstr_src_file_path.c_str(), wstr_dest_directory.c_str(), wstr_src_file_name.c_str());
							string str_file = string_ext::from_wstring(wstr_dest_file_path);

							CComPtr<ID3DXFileSaveData> text_name;
							if ( FAILED( mat_data->AddDataObject(TID_D3DRMTextureFilename, "Self_Illumination", 0, str_file.size() + 1, str_file.c_str(), &text_name) ) )
							{
								return false;
							}
						}
					}

				}

			}
		}

		v_mesh->unlock();

		return true;
	}

	
}