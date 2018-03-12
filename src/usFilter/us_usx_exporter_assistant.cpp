///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_usx_exporter_assistant.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : usx_exporter_assistant class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_usx_exporter_assistant.h"
// 
// extern unsigned char D3DRM_XTEMPLATES[];
#define D3DRMXTEMPLATEBYTES 3278


namespace uniscope_globe
{

	usx_exporter_assistant* singleton_usx_exporter_assistant::s_ins = NULL;

	usx_exporter_assistant::usx_exporter_assistant( void )
	{

	}

	usx_exporter_assistant::~usx_exporter_assistant( void )
	{

	}


	bool usx_exporter_assistant::save_usx( hardware_mesh* v_mesh, cpstr str_path )
	{	
		if(v_mesh == NULL) return false;
		if( v_mesh->get_rtti() == US_RTTI_D3D9_COMPLEX_HARDWARE_MESH )
		{
			bool ret_val = true;
			d3d9_complex_hardware_mesh* v_complex_mesh = (d3d9_complex_hardware_mesh*)v_mesh;

			export_complex_hardware_mesh( v_complex_mesh, str_path );

			return true;
			//v_complex_mesh->m_mesh_frame_map
			//export_single_mesh(v_hardware_mesh, str_path );
		}
		else
		{
			return export_single_mesh( dynamic_cast<d3d9_hardware_mesh*>(v_mesh), str_path );
		}
		return false;
	}

	bool usx_exporter_assistant::export_complex_hardware_mesh( d3d9_complex_hardware_mesh* v_complex_mesh, cpstr str_path )
	{
		// 创建File
		CComPtr<ID3DXFile> d3dx_file;
		if ( FAILED( D3DXFileCreate(&d3dx_file) ) )
		{
			return false;
		}

		// 注册模版
		//static char const * str_extensions = XEXTENSIONS_TEMPLATES;
		if ( FAILED( d3dx_file->RegisterTemplates((void*)usx_template::get_default_template(), D3DRMXTEMPLATEBYTES) ) )
		{
			return false;
		}
		if( FAILED(d3dx_file->RegisterTemplates((void*)XEXTENSIONS_TEMPLATES, strlen(XEXTENSIONS_TEMPLATES)) ) )
		{
			return false;
		}

		// 使用二进制模式保存
		D3DXF_FILEFORMAT file_format = D3DXF_FILEFORMAT_TEXT;

		bool use_binary_mode = true;
		if ( use_binary_mode )
		{
			file_format = D3DXF_FILEFORMAT_BINARY | D3DXF_FILEFORMAT_COMPRESSED;
		}

		// 删除原文件
		string str_file = string_ext::from_wstring(str_path);
		DeleteFileA(str_file.c_str());

		// 创建对象
		CComPtr<ID3DXFileSaveObject> file_save_obj;
		if( FAILED( d3dx_file->CreateSaveObject( str_file.c_str(), D3DXF_FILESAVE_TOFILE, file_format, &file_save_obj) ) )
		{
			return false;
		}


		d3d9_complex_hardware_mesh::mesh_frame_map::iterator itr = v_complex_mesh->m_mesh_frame_map.begin();
		for ( ; itr != v_complex_mesh->m_mesh_frame_map.end(); itr++ )
		{
			export_mesh( file_save_obj.p, itr->first, str_path );
		}

		if ( FAILED( file_save_obj->Save() ) )
		{
			return false;
		}

		return true;
	}

	bool usx_exporter_assistant::export_single_mesh( d3d9_hardware_mesh* v_mesh, cpstr str_path )
	{
		// 创建File
		CComPtr<ID3DXFile> d3dx_file;
		if ( FAILED( D3DXFileCreate(&d3dx_file) ) )
		{
			return false;
		}

		// 注册模版
		//static char const * str_extensions = XEXTENSIONS_TEMPLATES;
		if ( FAILED( d3dx_file->RegisterTemplates((void*)usx_template::get_default_template(), D3DRMXTEMPLATEBYTES) ) )
		{
			return false;
		}
		if( FAILED(d3dx_file->RegisterTemplates((void*)XEXTENSIONS_TEMPLATES, strlen(XEXTENSIONS_TEMPLATES)) ) )
		{
			return false;
		}

		// 使用二进制模式保存
		D3DXF_FILEFORMAT file_format = D3DXF_FILEFORMAT_TEXT;

		bool use_binary_mode = false;
		if ( use_binary_mode )
		{
			file_format = D3DXF_FILEFORMAT_BINARY | D3DXF_FILEFORMAT_COMPRESSED;
		}

		// 删除原文件
		string str_file = string_ext::from_wstring(str_path);
		DeleteFileA(str_file.c_str());

		// 创建对象
		CComPtr<ID3DXFileSaveObject> file_save_obj;
		if( FAILED( d3dx_file->CreateSaveObject( str_file.c_str(), D3DXF_FILESAVE_TOFILE, file_format, &file_save_obj) ) )
		{
			return false;
		}

		export_mesh( file_save_obj.p, v_mesh, str_path );

		if ( FAILED( file_save_obj->Save() ) )
		{
			return false;
		}

		return true;
	}

	bool usx_exporter_assistant::export_mesh( ID3DXFileSaveObject* save_obj, d3d9_hardware_mesh* v_mesh, cpstr str_mesh_path )
	{
		LPD3DXMESH v_d3d_mesh = v_mesh->m_d3d_mesh;
		if(v_d3d_mesh == NULL) return false;
		// get buffer
		LPDIRECT3DVERTEXBUFFER9 v_d3d_vertex_buffer;
		LPDIRECT3DINDEXBUFFER9  v_d3d_index_buffer;
		v_d3d_mesh->GetVertexBuffer( &v_d3d_vertex_buffer );
		v_d3d_mesh->GetIndexBuffer( &v_d3d_index_buffer );
		
		// get desc
		D3DVERTEXBUFFER_DESC vertex_desc;
		D3DINDEXBUFFER_DESC index_desc;
		v_d3d_vertex_buffer->GetDesc(&vertex_desc);
		v_d3d_index_buffer->GetDesc(&index_desc);
		
		// get count;
		ulong v_face_number = v_d3d_mesh->GetNumFaces();
		ulong v_verts_number = v_d3d_mesh->GetNumVertices();

		// get stride;
		ulong v_index_stride  = index_desc.Size  / v_face_number;
		//ulong v_vertex_stride = vertex_desc.Size / v_verts_number;
		ulong v_vertex_stride = v_d3d_mesh->GetNumBytesPerVertex();

		// lock vertices;
		byte* v_vertices= NULL;
		byte* v_indices = NULL;
		v_d3d_vertex_buffer->Lock( 0, 0, (void**)&v_vertices, 0 );
		v_d3d_index_buffer->Lock( 0, 0, (void**)&v_indices, 0 );

		int v_src_stride_offset = 0;
		//////////////////////////////////////////////////////////////////////////////////////////
		// Save Index
		//////////////////////////////////////////////////////////////////////////////////////////
		io_stream index_stream;
		index_stream.write( v_face_number );
		if( index_desc.Format == D3DFMT_INDEX16 )
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
		else if(  index_desc.Format == D3DFMT_INDEX32  )
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
		if( vertex_desc.FVF & D3DFVF_NORMAL )
		{
			// 保存法向量
			io_stream stream;
			stream.write( v_verts_number );

			int dest_stride = sizeof(float) * 3;
			for (int ni = 0; ni < (int)v_verts_number; ni++)
			{
				stream.write( v_vertices + v_vertex_stride * ni + v_src_stride_offset, dest_stride);
			}
			v_src_stride_offset += dest_stride;

			// 保存面索引
			stream.write(index_stream.get_data(), index_stream.get_size());

			CComPtr<ID3DXFileSaveData> normal_data;
			if ( FAILED( dx_save_data->AddDataObject(TID_D3DRMMeshNormals, "normals", 0, stream.get_size(), stream.get_data(), &normal_data) ) )
			{
				return false;
			}

			return true;
		}

		//////////////////////////////////////////////////////////////////////////////////////////
		// Save Diffuse
		//////////////////////////////////////////////////////////////////////////////////////////
		if( vertex_desc.FVF & D3DFVF_DIFFUSE )
		{
			// 保存顶点颜色
			io_stream stream;
			stream.write( v_verts_number );

			int dest_stride = sizeof(int);
			for (int ni = 0; ni < (int)v_verts_number; ni++)
			{
				stream.write( ni );
				stream.write( v_vertices + v_vertex_stride * ni + v_src_stride_offset, dest_stride);
			}
			v_src_stride_offset += dest_stride;

			CComPtr<ID3DXFileSaveData> color_data;
			if ( FAILED( dx_save_data->AddDataObject(TID_D3DRMMeshVertexColors, "col0", 0, stream.get_size(), stream.get_data(), &color_data) ) )
			{
				return false;
			}
		}
	
		int v_texture_count = ((int)(vertex_desc.FVF) & D3DFVF_TEXCOUNT_MASK) >> 8;
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
			stream.write(v_material_count);
			stream.write(v_face_number);

			DWORD* v_attribute;
			v_d3d_mesh->LockAttributeBuffer( 0, &v_attribute );
			stream.write( v_attribute, v_face_number );
			v_d3d_mesh->UnlockAttributeBuffer();

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
						ustring wstr_src_file_path = v_mesh->m_material_entry_array[ni].m_diffuse->get_key();
						ustring wstr_src_file_name = file_directory::get_file_name(wstr_src_file_path.c_str());
						ustring wstr_dest_directory = file_directory::get_directory(str_mesh_path);
						ustring wstr_dest_file_path = usx_exporter_assistant::export_resource(wstr_src_file_path.c_str(), wstr_dest_directory.c_str(), wstr_src_file_name.c_str());
						string str_file = string_ext::from_wstring(wstr_dest_file_path);

						CComPtr<ID3DXFileSaveData> text_name;
						if ( FAILED( mat_data->AddDataObject(TID_D3DRMTextureFilename, "Diffuse", 0, str_file.size() + 1, str_file.c_str(), &text_name) ) )
						{
							return false;
						}
					}
					else if( nj == 1)
					{
						ustring wstr_src_file_path = v_mesh->m_material_entry_array[ni].m_self_illuminant->get_key();
						ustring wstr_src_file_name = file_directory::get_file_name(wstr_src_file_path.c_str());
						ustring wstr_dest_directory = file_directory::get_directory(str_mesh_path);
						ustring wstr_dest_file_path = usx_exporter_assistant::export_resource(wstr_src_file_path.c_str(), wstr_dest_directory.c_str(), wstr_src_file_name.c_str());
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

		v_d3d_index_buffer->Unlock();
		v_d3d_vertex_buffer->Unlock();

		AUTO_RELEASE(v_d3d_index_buffer);
		AUTO_RELEASE(v_d3d_vertex_buffer);

		return true;
	}

	bool usx_exporter_assistant::export_skinning_hardware_mesh( d3d9_skinning_hardware_mesh* v_mesh, cpstr str_path )
	{
		return false;
	}

	ustring usx_exporter_assistant::export_resource(cpstr src_path, cpstr dest_directory, cpstr dest_file_name )
	{
		ustring src_link = src_path;
		ustring::size_type v_pos1 = src_link.find( L"http://" );
		ustring::size_type v_pos2 = src_link.find( L"db://" );
		ustring::size_type v_pos3 = src_link.find( L"root://" );

// 		if( v_pos1 == ustring::npos && v_pos2 == ustring::npos && v_pos3 == ustring::npos )
		{
			ustring str_dest_file_path = dest_directory;
			str_dest_file_path += dest_file_name;
			data_loader v_loader;
			raw_buffer* v_buffer = v_loader.load(src_path);
			if(v_buffer != NULL )
			{
				// save_raw_buffer;
				file_buffer v_file_buffer;
				v_file_buffer.file_open(str_dest_file_path.c_str(), L"wb");
				v_file_buffer.write(v_buffer->get_buffer(), v_buffer->get_size());
				v_file_buffer.file_close();
			}

			return dest_file_name;
		}

		return src_link;
	}

	
}