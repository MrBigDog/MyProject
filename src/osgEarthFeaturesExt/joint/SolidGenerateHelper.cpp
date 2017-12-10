#include <mdPipeline/PipelineGenerateHelper.h>

namespace mdPipeline
{
	osg::Matrixd PipelineGenerateHelper::generateTbnMatrix( const osg::Vec3d& vec_n, const osg::Vec3d& vec_trans )
	{
		if ( fabs(vec_n.y() - 1.0) < DBL_EPSILON )
		{
			osg::Matrixd mat = osg::Matrixd::identity();
			mat(3,0) = vec_trans.x();
			mat(3,1) = vec_trans.y();
			mat(3,2) = vec_trans.z();

			return mat;
		}

		if ( fabs( vec_n.y() + 1.0) < DBL_EPSILON )
		{
			osg::Matrixd mat = osg::Matrixd::identity()*osg::Matrixd::rotate(osg::PI, osg::X_AXIS);
			mat(3,0) = vec_trans.x();
			mat(3,1) = vec_trans.y();
			mat(3,2) = vec_trans.z();

			return mat;
		}

		osg::Vec3d vec_b(0.0, 1.0, 0.0);

		double v_angle = asin(vec_b*vec_n);

		osg::Vec3d vec_t = vec_b^vec_n;
		vec_t.normalize();

		vec_b = vec_t^vec_n;
		vec_b.normalize();

		osg::Matrixd mat;
		mat(0,0) = vec_t.x()	; mat(0,1) = vec_t.y()	  ; mat(0,2) = vec_t.z()	; mat(0,3) = 0.0;
		mat(1,0) = vec_n.x()	; mat(1,1) = vec_n.y()	  ; mat(1,2) = vec_n.z()	; mat(1,3) = 0.0;
		mat(2,0) = vec_b.x()	; mat(2,1) = vec_b.y()	  ; mat(2,2) = vec_b.z()	; mat(2,3) = 0.0;
		mat(3,0) = vec_trans.x(); mat(3,1) = vec_trans.y(); mat(3,2) = vec_trans.z(); mat(3,3) = 1.0;

		return mat;
	}

	osg::Matrixd PipelineGenerateHelper::generateTbnMatrix( const osg::Vec3d& vec_n, const osg::Vec3d& vec_trans, const osg::Vec3d& vec_scl )
	{
		if ( fabs(vec_n.y() - 1.0) < DBL_EPSILON )
		{
			osg::Matrixd mat = osg::Matrixd::identity();
			mat(3,0) = vec_trans.x();
			mat(3,1) = vec_trans.y();
			mat(3,2) = vec_trans.z();
			return mat;
		}

		if ( fabs( vec_n.y() + 1.0) < DBL_EPSILON )
		{
			osg::Matrixd mat = osg::Matrixd::identity()*osg::Matrixd::rotate(osg::PI, osg::X_AXIS);
			mat(3,0) = vec_trans.x();
			mat(3,1) = vec_trans.y();
			mat(3,2) = vec_trans.z();
			return mat;
		}

		osg::Vec3d vec_b( 0.0, 1.0, 0.0 );
		double v_angle = acos( vec_b*vec_n );

		osg::Vec3d vec_t = vec_b^vec_n;
		vec_t.normalize();
		vec_t = vec_t*vec_scl.x();

		vec_b = vec_t^vec_n;
		vec_b.normalize();
		vec_b = vec_b*vec_scl.z();

		osg::Vec3d vec_p = vec_trans;

		//matrix4<double> mat = matrix4<double>::from_tnbp( vec_t, vec_n, vec_b, vec_trans );
		osg::Matrixd mat;
		mat(0,0) = vec_t.x();  mat(0,1) = vec_t.y();   mat(0,2) = vec_t.z();  mat(0,3) = 0.0;
		mat(1,0) = vec_n.x();  mat(1,1) = vec_n.y();   mat(1,2) = vec_n.z();  mat(1,3) = 0.0;
		mat(2,0) = vec_b.x();  mat(2,1) = vec_b.y();   mat(2,2) = vec_b.z();  mat(2,3) = 0.0;
		mat(3,0) = vec_p.x();  mat(3,1) = vec_p.y();   mat(3,2) = vec_p.z();  mat(3,3) = 1.0;

		return mat;
	}

	osg::Plane PipelineGenerateHelper::calculateNormalPlane( const osg::Vec3d& pre_pt, const osg::Vec3d& cur_pt, const osg::Vec3d& nxt_pt )
	{
		// 平面定义的点;
		osg::Vec3d v_normal( 0.0, 0.0, 0.0 );

		// 计算normal;
		osg::Vec3d vec_n1 = pre_pt - cur_pt; vec_n1.normalize();
		osg::Vec3d vec_n2 = nxt_pt - cur_pt; vec_n2.normalize();

		osg::Vec3d v_binormal = vec_n1 + vec_n2; v_binormal.normalize();
		osg::Vec3d v_tangent  = vec_n1 ^ vec_n2; v_tangent.normalize();

		v_normal =  v_binormal ^ v_tangent; v_normal.normalize();

		// 构建平面;
		osg::Plane v_plane( v_normal, cur_pt );

		return v_plane;
	}

	//osg::Vec3Array* PipelineGenerateHelper::generate_cylinder_vertex( double v_raidus, double s_len, double e_len, long v_segment )
	//{
	//	// 顺时针;
	//	int v_segment_plus1 = v_segment + 1;
	//	int v_double_segment_plus1 = v_segment_plus1<<1;
	//
	//	osg::ref_ptr<osg::Vec3Array> vertex = new osg::Vec3Array;
	//	vertex->reserve(v_double_segment_plus1);
	//
	//	create_circle_d( vertex, vector3<double>( 0.0, s_len, 0.0 ), v_raidus, v_segment );
	//	vertex[v_segment] = vertex[0];
	//
	//	create_circle_d( vertex+v_segment_plus1, vector3<double>( 0.0, e_len, 0.0 ), v_raidus, v_segment );
	//	vertex[v_double_segment_plus1-1] = vertex[v_segment_plus1];
	//
	//	return vertex;
	//}
	//
	//
	//void PipelineGenerateHelper::calculate_orient( vector3<double> start_point, vector3<double> end_point, double& v_heading, vector3<double>& v_direct )
	//{
	//	// 初始化线参数
	//	spherical_transform sphr_trans;
	//	sphr_trans.create_from_sphr( start_point.x, start_point.y, start_point.z * 0.001 + SEMI_MAJOR_GEO, 0.0, 0.0, 0.0 );
	//
	//	vector_3d start_geo( start_point.x, start_point.y, start_point.z * 0.001 + SEMI_MAJOR_GEO );
	//	start_geo = geocentric_coords::from_spherical_d( start_geo );
	//
	//	vector_3d end_geo( end_point.x, end_point.y, end_point.z * 0.001 + SEMI_MAJOR_GEO );
	//	end_geo = geocentric_coords::from_spherical_d( end_geo );
	//
	//	v_direct= end_geo * sphr_trans.m_mat_tnb_inverse - start_geo * sphr_trans.m_mat_tnb_inverse;
	//
	//	v_heading = PipelineGenerateHelper::calculate_line_heading( vector3<double>( 0.0, 0.0, 0.0 ), v_direct );
	//	v_direct.normalize();
	//}
	//
	//
	//
	//double PipelineGenerateHelper::calculate_line_heading( vector3<double>& s_pt, vector3<double>& e_pt )
	//{
	//	vector3<double> vec = vector3<double>::normalize( e_pt - s_pt );
	//
	//	if ( math_d::fabs_( vec.z ) < math_d::s_zero_tolerance )
	//	{
	//		if ( vec.x > 0.0 )
	//		{
	//			return math_d::s_half_pi;
	//		}
	//
	//		if ( vec.x < 0.0 )
	//		{
	//			return math_d::s_half_pi * 3.0;
	//		}
	//
	//		return 0.0;
	//	}
	//
	//	if ( math_d::fabs_( vec.x ) < math_d::s_zero_tolerance )
	//	{
	//		if ( vec.z < 0.0 )
	//		{
	//			return math_d::s_pi;
	//		}
	//
	//		return 0.0;
	//	}
	//
	//	if ( vec.x > 0.0 )
	//	{
	//		return math_d::s_half_pi - math_d::atan_( vec.z / vec.x );
	//	}
	//
	//	if ( vec.x < 0.0  )
	//	{
	//		return math_d::s_half_pi * 3 - math_d::atan_( vec.z / vec.x );
	//	}
	//
	//	return 0.0;
	//}
	//
	//
	//double PipelineGenerateHelper::get_pipeline_thickness( double v_radius )
	//{
	//	double v_thickness = v_radius * 0.15;
	//	v_thickness = max( v_thickness, 0.002);
	//	v_thickness = min( v_thickness, 0.18 );
	//	return v_thickness;
	//}
	//
	//bool PipelineGenerateHelper::material_equal( usx_material* mat1, usx_material* mat2 )
	//{
	//	if ( mat1->m_texture_files.size() != mat2->m_texture_files.size() )
	//		return false;
	//
	//	int v_tex_size = (int)mat1->m_texture_files.size();
	//	for ( int ni = 0; ni < v_tex_size; ni++ )
	//	{
	//		if ( mat1->m_texture_files[ni].m_texture_file != mat2->m_texture_files[ni].m_texture_file )
	//			return false;
	//	}
	//
	//	if ( mat1->m_power != mat2->m_power )
	//		return false;
	//
	//	if ( mat1->m_ambient_color_rgb != mat2->m_ambient_color_rgb )
	//		return false;
	//
	//	if ( mat1->m_diffuse_color_rgba != mat2->m_diffuse_color_rgba )
	//		return false;
	//
	//	if ( mat1->m_specular_color_rgb != mat2->m_specular_color_rgb )
	//		return false;
	//
	//	if ( mat1->m_emissive_color_rgb != mat2->m_emissive_color_rgb )
	//		return false;
	//
	//	return true;
	//}
	//
	//
	//usx_mesh* PipelineGenerateHelper::combine_mesh( usx_mesh_array& v_mesh_array )
	//{
	//	if ( v_mesh_array.size() == 0 )
	//		return NULL;
	//
	//	usx_mesh* v_mesh = new usx_mesh;
	//	v_mesh->m_fvf = v_mesh_array[0]->m_fvf;
	//
	//	int v_sum_vertex_num = 0;
	//	int v_sum_face_num = 0;
	//	for ( int ni = 0; ni < (int)v_mesh_array.size(); ni++ )
	//	{
	//		usx_mesh* tmp_mesh = v_mesh_array[ni];
	//
	//		v_sum_vertex_num += tmp_mesh->m_number_of_vertices;
	//		v_sum_face_num += tmp_mesh->m_number_of_faces;
	//	}
	//
	//	// 顶点
	//	if ( v_mesh->m_fvf & USFVF_XYZ )
	//	{
	//		v_mesh->m_number_of_vertices = v_sum_vertex_num;
	//		v_mesh->m_vertex_data = new vector_3f[v_mesh->m_number_of_vertices];
	//
	//		int vertex_off = 0;
	//		for ( int ni = 0; ni < (int)v_mesh_array.size(); ni++ )
	//		{
	//			usx_mesh* tmp_mesh = v_mesh_array[ni];
	//			memcpy( v_mesh->m_vertex_data + vertex_off, tmp_mesh->m_vertex_data, tmp_mesh->m_number_of_vertices * sizeof( vector_3f ) );
	//
	//			vertex_off += tmp_mesh->m_number_of_vertices;
	//		}
	//	}
	//
	//	// Box
	//	v_mesh->m_bbox.make_invalid();
	//	for ( int ni = 0; ni < (int)v_mesh_array.size(); ni++ )
	//	{
	//		usx_mesh* tmp_mesh = v_mesh_array[ni];
	//		v_mesh->m_bbox.combine( tmp_mesh->m_bbox );
	//	}
	//
	//	// 法向量
	//	if ( v_mesh->m_fvf & USFVF_NORMAL )
	//	{
	//		v_mesh->m_normals = new vector_3f[v_mesh->m_number_of_vertices];
	//
	//		int normal_off = 0;
	//		for ( int ni = 0; ni < (int)v_mesh_array.size(); ni++ )
	//		{
	//			usx_mesh* tmp_mesh = v_mesh_array[ni];
	//			memcpy( v_mesh->m_normals + normal_off, tmp_mesh->m_normals, tmp_mesh->m_number_of_vertices * sizeof( vector_3f ) );
	//
	//			normal_off += tmp_mesh->m_number_of_vertices;
	//		}
	//	}
	//
	//	// 颜色
	//	if ( v_mesh->m_fvf & USFVF_DIFFUSE )
	//	{
	//		//todo
	//		v_mesh->m_vertex_color = new usx_color_value[v_mesh->m_number_of_vertices];
	//
	//		int color_off = 0;
	//		for ( int ni = 0; ni < (int)v_mesh_array.size(); ni++ )
	//		{
	//			usx_mesh* tmp_mesh = v_mesh_array[ni];
	//			memcpy( v_mesh->m_vertex_color + color_off, tmp_mesh->m_vertex_color, tmp_mesh->m_number_of_vertices * sizeof( ulong ) );
	//
	//			color_off += tmp_mesh->m_number_of_vertices;
	//		}
	//	}
	//
	//	// 纹理坐标
	//	int v_texture_level = ( v_mesh->m_fvf & USFVF_TEXCOUNT_MASK ) >> 8;
	//	if ( v_texture_level > 0 )
	//	{
	//		int v_texture_coods_num = v_mesh->m_number_of_vertices * 2;
	//		int v_texture_coords_buffer_size = sizeof( float ) * v_texture_coods_num;
	//
	//		for ( int ii = 0; ii < v_texture_level; ii++ )
	//		{
	//			float* v_texture_coords = new float[v_texture_coods_num];
	//
	//			int tex_coord_off = 0;
	//			for ( int ni = 0; ni < (int)v_mesh_array.size(); ni++ )
	//			{
	//				float* tmp_tex_coords = v_mesh_array[ni]->m_vertex_texture_coords[ii];
	//				int v_coords_size = v_mesh_array[ni]->m_number_of_vertices * 2 ;
	//				memcpy( v_texture_coords + tex_coord_off, tmp_tex_coords, v_coords_size *  sizeof( float ) );
	//
	//				tex_coord_off += v_coords_size;
	//			}
	//
	//			v_mesh->m_vertex_texture_coords.push_back( v_texture_coords );
	//		}
	//	}
	//
	//	// 顶点索引
	//	{
	//		v_mesh->m_number_of_faces = v_sum_face_num;
	//		v_mesh->m_face_index = new usx_tri_face_index[v_mesh->m_number_of_faces];
	//
	//		int vert_off = 0, idx_0ff = 0;
	//		for ( int ni = 0; ni < (int)v_mesh_array.size(); ni++ )
	//		{
	//			usx_mesh* tmp_mesh = v_mesh_array[ni];
	//			for ( int nj = 0; nj < tmp_mesh->m_number_of_faces; nj++ )
	//			{
	//				v_mesh->m_face_index[idx_0ff].a = tmp_mesh->m_face_index[nj].a + vert_off;
	//				v_mesh->m_face_index[idx_0ff].b = tmp_mesh->m_face_index[nj].b + vert_off;
	//				v_mesh->m_face_index[idx_0ff].c = tmp_mesh->m_face_index[nj].c + vert_off;
	//
	//				idx_0ff++;
	//			}
	//
	//			vert_off += tmp_mesh->m_number_of_vertices;
	//		}
	//
	//		int k = 0;
	//	}
	//
	//	// 先合并材质，再合并attribute
	//	v_mesh->m_attribute_data = new ulong[v_mesh->m_number_of_faces];
	//
	//	int idx = 0;
	//	for ( int ni = 0; ni < (int)v_mesh_array.size(); ni++ )
	//	{
	//		usx_mesh* tmp_mesh = v_mesh_array[ni];
	//
	//		std::vector<int> new_attribute_value;
	//		new_attribute_value.resize( tmp_mesh->m_materials.size() );
	//
	//		// 合并材质
	//		for ( int nj = 0; nj < (int)tmp_mesh->m_materials.size(); nj++ )
	//		{
	//			usx_material* v_mateiral = tmp_mesh->m_materials[nj];
	//
	//			// 判断是否存在相同的material
	//			bool exsit_mat = false;
	//			for ( int ii = 0; ii < (int)v_mesh->m_materials.size(); ii++ )
	//			{
	//				if ( PipelineGenerateHelper::material_equal( v_mesh->m_materials[ii], v_mateiral ) )
	//				{
	//					exsit_mat = true;
	//					new_attribute_value[nj] = ii;
	//					break;
	//				}
	//			}
	//
	//			// 如果不存在相同的material
	//			if ( !exsit_mat )
	//			{
	//				//usx_material* new_mat = new usx_material(v_mateiral);
	//				usx_material* new_mat = new usx_material();
	//
	//
	//				new_mat->m_diffuse_color_rgba = v_mateiral->m_diffuse_color_rgba;
	//				new_mat->m_ambient_color_rgb = v_mateiral->m_ambient_color_rgb;
	//				new_mat->m_specular_color_rgb = v_mateiral->m_specular_color_rgb;
	//				new_mat->m_emissive_color_rgb = v_mateiral->m_emissive_color_rgb;
	//				new_mat->m_power = v_mateiral->m_power;
	//
	//				new_mat->m_texture_files = v_mateiral->m_texture_files;
	//
	//				v_mesh->m_materials.push_back( new_mat );
	//
	//				new_attribute_value[nj] = (int)(v_mesh->m_materials.size()) - 1;
	//			}
	//		}
	//
	//		// 合并attribute
	//		for ( ulong nj = 0; nj < tmp_mesh->m_number_of_faces; nj++ )
	//		{
	//			v_mesh->m_attribute_data[idx++] = new_attribute_value[ tmp_mesh->m_attribute_data[nj] ];
	//		}
	//	}
	//
	//	return v_mesh;
	//}
	//

	//
	//
	//void PipelineGenerateHelper::read_pipeline_size( char* str_value, double& v_diameter, double& v_width, double& v_height )
	//{
	//	v_diameter = 0.0;
	//	v_width = 0.0;
	//	v_height = 0.0;
	//
	//	double v_decimal = 0;
	//	std::vector<double> decimal_array;
	//
	//	char vv;
	//	int cur_pos = 0;
	//	bool read_status = false;
	//
	//	int char_num = strlen( str_value );
	//	while ( cur_pos < char_num )
	//	{
	//		vv = str_value[cur_pos];
	//		cur_pos++;
	//
	//		if ( vv <= '9' && vv >= '0' )
	//		{
	//			double digit = vv - '0';
	//
	//			v_decimal = v_decimal * 10 + digit;
	//			read_status = true;
	//			continue;
	//		}
	//
	//		if ( !read_status )
	//			continue;
	//
	//		decimal_array.push_back( v_decimal );
	//
	//		v_decimal = 0;
	//		read_status = false;
	//	}
	//
	//	if ( read_status )
	//	{
	//		decimal_array.push_back( v_decimal );
	//	}
	//
	//	if ( decimal_array.size() == 1 )
	//	{
	//		v_diameter = decimal_array[0];
	//	}
	//	else if ( decimal_array.size() > 1 )
	//	{
	//		v_width = decimal_array[0];
	//		v_height = decimal_array[1];
	//	}
	//}
	//
	////void PipelineGenerateHelper::generate_pure_color_texture( LPCTSTR str_path, ulong v_color )
	////{
	////	int v_row = 128, v_col = 128;
	////	int v_quality = 90;
	////	
	////	int v_size = v_row * v_col * 3;
	//
	////	byte* v_rgb = new byte[v_size];
	////	memset( v_rgb, 0, v_size );
	//
	////	byte r = (v_color & 0x00ff0000)>>16;
	////	byte g = (v_color & 0x0000ff00)>>8;
	////	byte b = (v_color & 0x000000ff);
	////	
	////	int v_off = 0;
	////	for ( int ni = 0; ni < v_row; ni++ )
	////	{
	////		for ( int nj = 0; nj < v_col; nj++ )
	////		{
	////			v_rgb[v_off++] = b;
	////			v_rgb[v_off++] = g;
	////			v_rgb[v_off++] = r;
	////		}
	////	}
	//
	////	jpeg_loader::save_as_jpg( v_rgb, v_row, v_col, v_quality, str_path );
	//
	////	AUTO_DELETE( v_rgb );
	////}
}
