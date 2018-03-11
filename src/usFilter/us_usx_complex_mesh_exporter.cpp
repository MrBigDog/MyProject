
///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_usx_complex_mesh_exporter.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : usx_complex_mesh_exporter class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_usx_complex_mesh_exporter.h"

namespace uniscope_globe
{
	bool usx_complex_mesh_exporter::do_export( ID3DXFileSaveObject* save_obj, d3d9_complex_hardware_mesh* v_mesh, cpstr dest_path, axis_aligned_box<float>& out_box )
	{
		usx_frame* v_frame = v_mesh->get_root_frame();
		if( v_frame != NULL )
		{
			//export_frame( save_obj, v_frame, dest_path, out_box, matrix4<float>::s_identity );
		}

		d3d9_complex_hardware_mesh::mesh_frame_map::iterator itr = v_mesh->m_mesh_frame_map.begin();
		for ( ; itr != v_mesh->m_mesh_frame_map.end(); itr++ )
		{
// 			itr->first->lock();

			d3d9_hardware_mesh_buffer* v_buffer = (d3d9_hardware_mesh_buffer*)itr->first;

			if( itr->first->get_rtti() == US_RTTI_D3D9_SKINNING_HARDWARE_MESH )
			{
				//export_mesh( save_obj, itr->first, dest_path, out_box, matrix4<float>::s_identity );

				//vector3<double> vec_min = v_buffer->m_aabb.get_min();
				//vector3<double> vec_max = v_buffer->m_aabb.get_max();
				//out_box.combine( vector3<float>( (double*)&vec_min.x ) );
				//out_box.combine( vector3<float>( (double*)&vec_max.x ) );
			}
			else if( itr->first->get_rtti() == US_RTTI_D3D9_HARDWARE_MESH && itr->second == NULL )
			{
				usx_hardware_mesh_exporter::do_export( save_obj, dynamic_cast<d3d9_hardware_mesh*>(itr->first), dest_path, out_box );

				//export_mesh( save_obj, itr->first, dest_path, out_box, matrix4<float>::s_identity );
			}
			
// 			itr->first->unlock();
		}

		return true;
	}

	//bool usx_complex_mesh_exporter::export_frame( ID3DXFileSaveObject* save_obj, usx_frame* v_frame, cpstr dest_path, axis_aligned_box<float>& out_box, matrix4<float> in_mat )
	//{
	//	// save current frame
	//	string str_name = string_ext::from_wstring(v_frame->m_name);

	//	// save frame
	//	CComPtr<ID3DXFileSaveData> frame_data;
	//	if ( FAILED( save_obj->AddDataObject(TID_D3DRMFrame, (char*)str_name.c_str(), 0, 0, 0, &frame_data) ) )
	//	{
	//		return false;
	//	}

	//	{
	//		// save matrix;
	//		matrix_4f v_frame_matrix = matrix_4f((double*)&v_frame->m_original_mat);
	//		CComPtr<ID3DXFileSaveData> matrix_data;
	//		if ( FAILED( frame_data->AddDataObject(TID_D3DRMFrameTransformMatrix, "", 0, sizeof(matrix_4f), (void*)&v_frame_matrix, &matrix_data) ) )
	//		{
	//			return false;
	//		}

	//		if(v_frame->m_mesh != NULL )
	//		{
	//			usx_complex_mesh_exporter::export_mesh( frame_data, v_frame->m_mesh, dest_path, out_box, v_frame_matrix * in_mat );
	//		}

	//		if( v_frame->m_frame_first_child != NULL )
	//		{
	//			usx_complex_mesh_exporter::export_frame( frame_data, v_frame->m_frame_first_child, dest_path, out_box, v_frame_matrix * in_mat );
	//		}
	//	}

	//	if( v_frame->m_frame_sibling != NULL )
	//	{
	//		usx_complex_mesh_exporter::export_frame( save_obj, v_frame->m_frame_sibling, dest_path, out_box, in_mat );
	//	}
	//}

	//bool usx_complex_mesh_exporter::export_frame( ID3DXFileSaveData* save_obj, usx_frame* v_frame, cpstr dest_path, axis_aligned_box<float>& out_box, matrix4<float> in_mat )
	//{
	//	// save current frame
	//	string str_name = string_ext::from_wstring(v_frame->m_name);

	//	// save frame
	//	CComPtr<ID3DXFileSaveData> frame_data;
	//	if ( FAILED( save_obj->AddDataObject(TID_D3DRMFrame, (char*)str_name.c_str(), 0, 0, 0, &frame_data) ) )
	//	{
	//		return false;
	//	}

	//	{
	//		// save matrix;
	//		matrix_4f v_frame_matrix = matrix_4f((double*)&v_frame->m_original_mat);
	//		CComPtr<ID3DXFileSaveData> matrix_data;
	//		if ( FAILED( frame_data->AddDataObject(TID_D3DRMFrameTransformMatrix, "", 0, sizeof(matrix_4f), (void*)&v_frame_matrix, &matrix_data) ) )
	//		{
	//			return false;
	//		}

	//		if(v_frame->m_mesh != NULL )
	//		{
	//			usx_complex_mesh_exporter::export_mesh( frame_data, v_frame->m_mesh, dest_path, out_box, v_frame_matrix * in_mat );
	//		}

	//		if( v_frame->m_frame_first_child != NULL )
	//		{
	//			usx_complex_mesh_exporter::export_frame( frame_data, v_frame->m_frame_first_child, dest_path, out_box, v_frame_matrix * in_mat );
	//		}
	//	}

	//	if( v_frame->m_frame_sibling != NULL )
	//	{
	//		usx_complex_mesh_exporter::export_frame( save_obj, v_frame->m_frame_sibling, dest_path, out_box, in_mat );
	//	}
	//}



	bool usx_complex_mesh_exporter::export_mesh( ID3DXFileSaveData* save_obj, usx_mesh* v_mesh, cpstr dest_path, axis_aligned_box<float>& out_box, matrix4<float> in_mat  )
	{
		// 保存顶点
		io_stream stream;
		stream.write( v_mesh->m_number_of_vertices );
		stream.write( v_mesh->m_vertex_data, v_mesh->m_number_of_vertices );

		// 更新BoundingBox
		if( v_mesh->m_skin_info == NULL )
		{
			for ( int i = 0; i < v_mesh->m_number_of_vertices; i++ )
			{
				out_box.combine( v_mesh->m_vertex_data[i] * in_mat );
			}
		}

		// 保存面索引
		stream.write( v_mesh->m_number_of_faces );
		ulong dw_triangle_index[4];
		dw_triangle_index[0] = 3;
		for( int ni = 0; ni < (int)v_mesh->m_number_of_faces; ni++ ) 
		{
			dw_triangle_index[1] = v_mesh->m_face_index[ni].a;
			dw_triangle_index[2] = v_mesh->m_face_index[ni].b;
			dw_triangle_index[3] = v_mesh->m_face_index[ni].c;

			stream.write( dw_triangle_index );
		}

		string str_name = string_ext::from_wstring(v_mesh->m_name);
		CComPtr<ID3DXFileSaveData> dx_save_data;
		if ( FAILED( save_obj->AddDataObject(TID_D3DRMMesh, (char*)str_name.c_str(), 0, stream.get_size(), stream.get_data(), &dx_save_data) ) )
		{
			return false;
		}

		// 保存法向量
		if( v_mesh->m_normals != NULL )
		{
			usx_complex_mesh_exporter::export_normal( dx_save_data, v_mesh );
		}

		// 保存纹理坐标
		if( v_mesh->m_vertex_texture_coords.size() > 0 )
		{
			usx_complex_mesh_exporter::export_texture_coords( dx_save_data, v_mesh );
		}

		// 保存颜色
		if( v_mesh->m_vertex_color ) 
		{
			usx_complex_mesh_exporter::export_color( dx_save_data, v_mesh );
		}

		// 保存第二层纹理坐标
		if( v_mesh->m_vertex_texture_coords.size() > 1 ) 
		{
			usx_complex_mesh_exporter::export_dcls( dx_save_data, v_mesh );
		}

		// 保存材质
		if( v_mesh->m_materials.size() > 0 )
		{
			usx_complex_mesh_exporter::export_attribute( dx_save_data, v_mesh, dest_path);
		}

		if( v_mesh->m_skin_info != NULL )
		{
			usx_complex_mesh_exporter::export_skin( dx_save_data, v_mesh->m_skin_info );
		}

		return true;
	}

	bool usx_complex_mesh_exporter::export_mesh( ID3DXFileSaveObject* save_obj, usx_mesh* v_mesh, cpstr dest_path, axis_aligned_box<float>& out_box, matrix4<float> in_mat )
	{
		// 保存顶点
		io_stream stream;
		stream.write( v_mesh->m_number_of_vertices );
		stream.write( v_mesh->m_vertex_data, v_mesh->m_number_of_vertices );

		if( v_mesh->m_skin_info == NULL )
		{
			for ( int i = 0; i < v_mesh->m_number_of_vertices; i++ )
			{
				out_box.combine( v_mesh->m_vertex_data[i] * in_mat );
			}
		}

		// 保存面索引
		stream.write( v_mesh->m_number_of_faces );
		ulong dw_triangle_index[4];
		dw_triangle_index[0] = 3;
		for( int ni = 0; ni < (int)v_mesh->m_number_of_faces; ni++ ) 
		{
			dw_triangle_index[1] = v_mesh->m_face_index[ni].a;
			dw_triangle_index[2] = v_mesh->m_face_index[ni].b;
			dw_triangle_index[3] = v_mesh->m_face_index[ni].c;

			stream.write( dw_triangle_index );
		}

		string str_name = string_ext::from_wstring(v_mesh->m_name);
		CComPtr<ID3DXFileSaveData> dx_save_data;
		if ( FAILED( save_obj->AddDataObject(TID_D3DRMMesh, (char*)str_name.c_str(), 0, stream.get_size(), stream.get_data(), &dx_save_data) ) )
		{
			return false;
		}

		// 保存法向量
		if( v_mesh->m_normals != NULL )
		{
			usx_complex_mesh_exporter::export_normal( dx_save_data, v_mesh );
		}

		// 保存纹理坐标
		if( v_mesh->m_vertex_texture_coords.size() > 0 )
		{
			usx_complex_mesh_exporter::export_texture_coords( dx_save_data, v_mesh );
		}

		// 保存颜色
		if( v_mesh->m_vertex_color ) 
		{
			usx_complex_mesh_exporter::export_color( dx_save_data, v_mesh );
		}

		// 保存第二层纹理坐标
		if( v_mesh->m_vertex_texture_coords.size() > 1 ) 
		{
			usx_complex_mesh_exporter::export_dcls( dx_save_data, v_mesh );
		}

		// 保存材质
		if( v_mesh->m_materials.size() > 0 )
		{
			usx_complex_mesh_exporter::export_attribute( dx_save_data, v_mesh, dest_path );
		}

		if( v_mesh->m_skin_info != NULL )
		{
			usx_complex_mesh_exporter::export_skin( dx_save_data, v_mesh->m_skin_info );
		}

		return true;
	}


	bool usx_complex_mesh_exporter::export_normal( ID3DXFileSaveData* save_data, usx_mesh* v_mesh )
	{
		// 保存法向量
		io_stream stream;
		stream.write( v_mesh->m_number_of_vertices );
		stream.write( v_mesh->m_normals, v_mesh->m_number_of_vertices );

		// 保存面索引
		stream.write( v_mesh->m_number_of_faces );

		ulong dw_triangle_index[4];
		dw_triangle_index[0] = 3;
		for( int ni = 0; ni < (int)v_mesh->m_number_of_faces; ni++ ) 
		{
			dw_triangle_index[1] = v_mesh->m_face_index[ni].a;
			dw_triangle_index[2] = v_mesh->m_face_index[ni].b;
			dw_triangle_index[3] = v_mesh->m_face_index[ni].c;
			stream.write( dw_triangle_index );
		}

		CComPtr<ID3DXFileSaveData> normal_data;
		if ( FAILED( save_data->AddDataObject(TID_D3DRMMeshNormals, "normals", 0, stream.get_size(), stream.get_data(), &normal_data) ) )
		{
			return false;
		}

		return true;
	}

	bool usx_complex_mesh_exporter::export_texture_coords( ID3DXFileSaveData* save_data, usx_mesh* v_mesh )
	{
		// 保存第一层纹理坐标
		io_stream stream;
		stream.write( v_mesh->m_number_of_vertices );
		stream.write( v_mesh->m_vertex_texture_coords[0], 2*v_mesh->m_number_of_vertices );

		CComPtr<ID3DXFileSaveData> text_coords;
		if ( FAILED( save_data->AddDataObject(TID_D3DRMMeshTextureCoords, "tc0", 0, stream.get_size(), stream.get_data(), &text_coords) ) )
		{
			return false;
		}

		return true;
	}

	bool usx_complex_mesh_exporter::export_color( ID3DXFileSaveData* save_data, usx_mesh* v_mesh )
	{
		// 保存顶点颜色
		io_stream stream;
		stream.write( v_mesh->m_number_of_vertices );

		ulong target = 0;
		ulong color_size = sizeof(usx_color_value);
		for( int ni = 0; ni < (int)v_mesh->m_number_of_vertices; ni++ ) 
		{
			target = ulong(ni);
			stream.write( target );

			usx_color_value usx_color(v_mesh->m_vertex_color[ni]);
			stream.write( usx_color );
		}

		CComPtr<ID3DXFileSaveData> color_data;
		if ( FAILED( save_data->AddDataObject(TID_D3DRMMeshVertexColors, "col0", 0, stream.get_size(), stream.get_data(), &color_data) ) )
		{
			return false;
		}

		return true;
	}

	bool usx_complex_mesh_exporter::export_dcls( ID3DXFileSaveData* save_data, usx_mesh* v_mesh )
	{
		D3DVERTEXELEMENT9 elems[4] = {  {0xFF,0,D3DDECLTYPE_UNUSED, 0,0,0},
		{0xFF,0,D3DDECLTYPE_UNUSED, 0,0,0},
		{0xFF,0,D3DDECLTYPE_UNUSED, 0,0,0},
		{0xFF,0,D3DDECLTYPE_UNUSED, 0,0,0} };
		int k = 0;
		int ni = 1;
		WORD dw_offset = 0;
		for ( ni = 1; ni < (int)v_mesh->m_vertex_texture_coords.size(); ni++ )
		{
			D3DVERTEXELEMENT9 ve = { 0, dw_offset, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 };
			elems[k] = ve;

			k++;
			dw_offset += 8;
		}
		
		// 写入头信息
		io_stream stream;
		stream.write( k );

		for( int nj = 0; nj < k; nj++ ) 
		{
			stream.write( (ulong)elems[nj].Type);
			stream.write( (ulong)elems[nj].Method);
			stream.write( (ulong)elems[nj].Usage);
			stream.write( (ulong)elems[nj].UsageIndex);
		}

		ulong dw_num_vertex = v_mesh->m_number_of_vertices;
		ulong dw_size = dw_offset / 4 * dw_num_vertex;
		stream.write( dw_size );

		ulong text_coord_size = 2 * dw_num_vertex;
		for ( ulong d = 1; d < v_mesh->m_vertex_texture_coords.size(); ++d )
		{
			stream.write( v_mesh->m_vertex_texture_coords[d], text_coord_size );
		}

		CComPtr<ID3DXFileSaveData> dcls_data;
		if ( FAILED( save_data->AddDataObject(DXFILEOBJ_DeclData, 0, 0, stream.get_size(), stream.get_data(), &dcls_data) ) )
		{
			return false;
		}

		return true;
	}

	bool usx_complex_mesh_exporter::export_attribute( ID3DXFileSaveData* save_data, usx_mesh* v_mesh, cpstr dest_path )
	{
		// 导出attribute
		io_stream stream;
		stream.write( v_mesh->m_materials.size() );
		stream.write( v_mesh->m_number_of_faces );
		stream.write( v_mesh->m_attribute_data, v_mesh->m_number_of_faces );

		CComPtr<ID3DXFileSaveData> attribute_data;
		if ( FAILED( save_data->AddDataObject(TID_D3DRMMeshMaterialList, "mtls", 0, stream.get_size(), stream.get_data(), &attribute_data) ) )
		{
			return false;
		}

		// 处理材质
		return export_materials( attribute_data, v_mesh, dest_path );
	}

	bool usx_complex_mesh_exporter::export_materials( ID3DXFileSaveData* save_data, usx_mesh* v_mesh, cpstr dest_path )
	{
		int v_texture_level = v_mesh->m_vertex_texture_coords.size();
		for( size_t ni = 0; ni < v_mesh->m_materials.size(); ni++ )
		{
			usx_material* v_mat = v_mesh->m_materials[ni];

			io_stream stream;
			stream.write( v_mat->m_diffuse_color_rgba );
			stream.write( v_mat->m_power );
			stream.write( ((byte*)&v_mat->m_specular_color_rgb), 12 );
			stream.write( ((byte*)&v_mat->m_emissive_color_rgb), 12 );

			string str_name = string_ext::from_wstring( v_mat->m_name );

			CComPtr<ID3DXFileSaveData> mat_data;
			if ( FAILED( save_data->AddDataObject(TID_D3DRMMaterial, str_name.c_str(), 0, stream.get_size(), stream.get_data(), &mat_data) ) )
			{
				return false;
			}

			for ( int nj = 0; nj < v_texture_level; nj++ )
			{
				//string str_file = string_ext::from_wstring( v_mat->m_texture_files[nj].m_texture_file );

				if( nj == 0 )
				{
					ustring wstr_src_file_path = v_mat->m_texture_files[nj].m_texture_file;
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
				else if( nj == 1)
				{
					ustring wstr_src_file_path = v_mat->m_texture_files[nj].m_texture_file;
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

		return true;
	}

	bool usx_complex_mesh_exporter::export_skin( ID3DXFileSaveData* save_data, usx_skin_info* in_skin )
	{
		{
			// skin header
			io_stream stream;		
			stream.write( (byte*)&in_skin->m_weights_per_vertex, sizeof( ushort ) );
			stream.write( (byte*)&in_skin->m_weights_per_face, sizeof( ushort ) );
			stream.write( (byte*)&in_skin->m_num_of_bones, sizeof( ushort ) );

			CComPtr<ID3DXFileSaveData> skin_header_data;
			if ( FAILED( save_data->AddDataObject(DXFILEOBJ_XSkinMeshHeader, "",  0, stream.get_size(), stream.get_data(), &skin_header_data) ) )
			{
				return false;
			}
		}	

		{
			
			for ( int i = 0; i < in_skin->m_weights_array.size(); i++ )
			{
				usx_weights_info* weights_info = in_skin->m_weights_array[i];

				io_stream stream;

				// bone name
				stream.write( string_ext::from_wstring( weights_info->m_frame_name ) );
				
				// index
				ulong v_num_of_indices = weights_info->m_indices.size();
				stream.write( (byte*)&v_num_of_indices, sizeof(ulong) );
				for ( int j = 0; j < weights_info->m_indices.size(); j++ )
				{
					ulong index = weights_info->m_indices[j];
					stream.write( (byte*)&index, sizeof(ulong) );
				}

				// weights
				for ( int j = 0; j < weights_info->m_weights.size(); j++ )
				{
					float val = weights_info->m_weights[j];
					stream.write( (byte*)&val, sizeof(float) );
				}

				// offset matrix
				matrix_4f v_offset_matrix_f = matrix_4f( (double*)&weights_info->m_offset_matrix );
				stream.write( (byte*)&v_offset_matrix_f, sizeof( matrix4<float> ) );

				// skin weights
				CComPtr<ID3DXFileSaveData> skin_weights_data;
				if ( FAILED( save_data->AddDataObject(DXFILEOBJ_SkinWeights, "",  0, stream.get_size(), stream.get_data(), &skin_weights_data) ) )
				{
					return false;
				}
			}			
		}		

		return true;
	}
}