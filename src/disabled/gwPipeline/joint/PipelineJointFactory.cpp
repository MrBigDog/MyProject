#include <gwPipeline/PipelineJointFactory.h>
#include <gwPipeline/PipelineGenerateHelper.h>
#include <gwPipeline/PipelineMeshGenerateHelper.h>

#include <osg/Geode>
#include <osg/Geometry>

#include <assert.h>

namespace gwPipeline
{
	UdgJointEntry::UdgJointEntry()
	{
		m_id = -1;
		m_joint_thickness = 0.02;
		m_generate_tooth = true;

		m_location = osg::Vec3d(0.0, 0.0, 0.0);

		m_texture_scl = 1.0f;

		m_jointType = CIRCLE;
	}

	UdgJointEntry::UdgJointEntry(const UdgJointEntry& v_entry)
	{
		m_id = v_entry.m_id;
		m_str_point_id = v_entry.m_str_point_id;
		m_guid = v_entry.m_guid;
		m_generate_tooth = v_entry.m_generate_tooth;

		m_location = v_entry.m_location;
		m_material = v_entry.m_material;

		m_joint_thickness = v_entry.m_joint_thickness;
		m_joint_param = v_entry.m_joint_param;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	JointEntryGenerator::JointEntryGenerator(void) { }

	JointEntryGenerator::~JointEntryGenerator(void) { }

	bool compare_large_joint_part_radius1(JointPartInfo& pt1, JointPartInfo& pt2)
	{
		return pt1.m_radius > pt2.m_radius;
	}

	void JointEntryGenerator::checkPointType(UdgJointEntry* us_element_joint, const Vector3dArray& in_relate_direction, const DoubleValueArray& radius, int relate_num)
	{
		if (in_relate_direction.size() < 2) return;
		if (!us_element_joint) us_element_joint = new UdgJointEntry;


		// 逐点判断;
		// 遍历关联线，获取有效关联线和最大管径;
		double max_joint_deep = -DBL_MAX;

		JointPartInfoArray v_part_array;

		for (int ni = 0; ni < relate_num; ni++)
		{
			JointPartInfo v_part_info;
			v_part_info.m_direction = in_relate_direction[ni];
			v_part_info.m_direction.normalize();
			v_part_info.m_radius = radius[ni] * 0.5;
			v_part_array.push_back(v_part_info);

			max_joint_deep = std::max(max_joint_deep, v_part_info.m_radius);
		}

		// 当点关联的线少于2时，不处理
		if (v_part_array.size() < 2)
			return;

		// 按管径大小排序;
		std::sort(v_part_array.begin(), v_part_array.end(), compare_large_joint_part_radius1);

		// 当点关联的线为2时，生成弯头
		// 当两条线的夹角小于60度时，不生成弯头
		if (v_part_array.size() == 2)
		{
			JointPartInfo& v_line0 = v_part_array[0];
			JointPartInfo& v_line1 = v_part_array[1];

			double v_angle = v_line0.m_direction*v_line1.m_direction;

			if (v_angle <= 0.5)
			{
				us_element_joint->m_joint_param.m_valid = true;
				us_element_joint->m_joint_param.m_main_radius = max_joint_deep;
				us_element_joint->m_joint_param.m_curvature_radius = us_element_joint->m_joint_param.m_main_radius * 2;
				us_element_joint->m_joint_param.m_main_angle = acos(v_angle);
				us_element_joint->m_joint_param.m_main_in_part = v_line0;
				us_element_joint->m_joint_param.m_main_out_part = v_line1;
			}
			return;
		}
		else
		{
			// 当点关联的线大于2时，生成多通接头，如三通、四通等
			double included_angle = 0.0;
			int in_part = -1, out_part = -1;
			if (!calculateMultiJointParam(v_part_array, in_part, out_part, included_angle))
			{
				return;
			}

			us_element_joint->m_joint_param.m_valid = true;
			us_element_joint->m_joint_param.m_main_radius = max_joint_deep;
			us_element_joint->m_joint_param.m_curvature_radius = us_element_joint->m_joint_param.m_main_radius * 2;
			us_element_joint->m_joint_param.m_main_angle = included_angle;
			us_element_joint->m_joint_param.m_main_in_part = v_part_array[in_part];
			us_element_joint->m_joint_param.m_main_in_part.m_radius = max_joint_deep;
			us_element_joint->m_joint_param.m_main_out_part = v_part_array[out_part];
			us_element_joint->m_joint_param.m_main_out_part.m_radius = max_joint_deep;

			v_part_array.erase(v_part_array.begin() + in_part);
			v_part_array.erase(v_part_array.begin() + out_part - 1);
			us_element_joint->m_joint_param.m_lateral_part = v_part_array;
		}
	}

	void JointEntryGenerator::checkPointType(UdgJointEntry* us_element_joint, const Vector3dArray& in_relate_direction, const DoubleValueArray& widths, const DoubleValueArray& heights, int relate_num)
	{
		if (in_relate_direction.size() < 2)
		{
			return;
		}

		if (us_element_joint == NULL || !us_element_joint)
		{
			us_element_joint = new UdgJointEntry;
		}

		// 逐点判断;
		// 遍历关联线，获取有效关联线和最大管径;
		//double max_joint_deep = -DBL_MAX;
		double max_joint_width = -DBL_MAX;
		double max_joint_height = -DBL_MAX;

		JointPartInfoArray v_part_array;

		for (int ni = 0; ni < relate_num; ni++)
		{
			JointPartInfo v_part_info;
			v_part_info.m_direction = in_relate_direction[ni];
			v_part_info.m_direction.normalize();
			//v_part_info.m_radius = radius[ni] * 0.5;
			v_part_info.m_width = widths[ni];
			v_part_info.m_height = heights[ni];
			v_part_array.push_back(v_part_info);

			//max_joint_deep = std::max( max_joint_deep, (v_part_info.m_width + v_part_info.m_height)*0.5 );
			max_joint_width = std::max(max_joint_width, v_part_info.m_width);
			max_joint_height = std::max(max_joint_height, v_part_info.m_height);
		}

		// 当点关联的线少于2时，不处理
		if (v_part_array.size() < 2)
			return;

		// 按管径大小排序;
		std::sort(v_part_array.begin(), v_part_array.end(), compare_large_joint_part_radius1);

		// 当点关联的线为2时，生成弯头
		// 当两条线的夹角小于60度时，不生成弯头
		if (v_part_array.size() == 2)
		{
			JointPartInfo& v_line0 = v_part_array[0];
			JointPartInfo& v_line1 = v_part_array[1];

			double v_angle = v_line0.m_direction*v_line1.m_direction;

			if (v_angle <= 0.5)
			{
				us_element_joint->m_joint_param.m_valid = true;
				//us_element_joint->m_joint_param.m_main_radius = max_joint_deep;
				us_element_joint->m_joint_param.m_main_width = max_joint_width;
				us_element_joint->m_joint_param.m_main_height = max_joint_height;
				us_element_joint->m_joint_param.m_curvature_radius = us_element_joint->m_joint_param.m_main_width + us_element_joint->m_joint_param.m_main_height;
				us_element_joint->m_joint_param.m_main_angle = acos(v_angle);
				us_element_joint->m_joint_param.m_main_in_part = v_line0;
				us_element_joint->m_joint_param.m_main_out_part = v_line1;
			}
			return;
		}
		else
		{
			// 当点关联的线大于2时，生成多通接头，如三通、四通等
			double included_angle = 0.0;
			int in_part = -1, out_part = -1;
			if (!calculateMultiJointParam(v_part_array, in_part, out_part, included_angle))
			{
				return;
			}

			us_element_joint->m_joint_param.m_valid = true;
			//us_element_joint->m_joint_param.m_main_radius = max_joint_deep;
			us_element_joint->m_joint_param.m_main_width = max_joint_width;
			us_element_joint->m_joint_param.m_main_height = max_joint_height;
			us_element_joint->m_joint_param.m_curvature_radius = us_element_joint->m_joint_param.m_main_width + us_element_joint->m_joint_param.m_main_height;
			us_element_joint->m_joint_param.m_main_angle = included_angle;
			us_element_joint->m_joint_param.m_main_in_part = v_part_array[in_part];
			//us_element_joint->m_joint_param.m_main_in_part.m_radius = max_joint_deep;
			us_element_joint->m_joint_param.m_main_in_part.m_width = max_joint_width;
			us_element_joint->m_joint_param.m_main_in_part.m_height = max_joint_height;
			us_element_joint->m_joint_param.m_main_out_part = v_part_array[out_part];
			//us_element_joint->m_joint_param.m_main_out_part.m_radius = max_joint_deep;
			us_element_joint->m_joint_param.m_main_out_part.m_width = max_joint_width;
			us_element_joint->m_joint_param.m_main_out_part.m_height = max_joint_height;

			v_part_array.erase(v_part_array.begin() + in_part);
			v_part_array.erase(v_part_array.begin() + out_part - 1);
			us_element_joint->m_joint_param.m_lateral_part = v_part_array;
		}
	}


	bool JointEntryGenerator::calculateMultiJointParam(JointPartInfoArray& v_part_array, int& in_id, int& out_id, double& v_angle)
	{
		// 以最大管径为主管的情况3
		int out_part = 1;
		double out_angle_cos = 2.0;

		// 记录夹角非180度的主管
		int in_part_ex = 0, out_part_ex = 1;
		double max_angle_cos = 2.0;

		bool v_main_part_ok = false;
		for (int ni = 0; ni < (int)v_part_array.size(); ni++)
		{
			JointPartInfo& v_line0 = v_part_array[ni];

			for (int nj = ni + 1; nj < (int)v_part_array.size(); nj++)
			{
				JointPartInfo& v_line1 = v_part_array[nj];
				double tmp_angle = v_line0.m_direction*v_line1.m_direction;//vector_3d::dot( v_line0.m_direction, v_line1.m_direction );

				if (tmp_angle <= PI_LIMEN_COS)
				{
					in_id = ni;
					out_id = nj;
					v_angle = acos(tmp_angle);

					return true;
				}

				if (tmp_angle < max_angle_cos)
				{
					in_part_ex = ni;
					out_part_ex = nj;
					max_angle_cos = tmp_angle;

					if (ni == 0)
					{
						out_part = nj;
						out_angle_cos = tmp_angle;
					}
				}
			}
		}

		if (out_angle_cos <= 0)
		{
			if (max_angle_cos < out_angle_cos)
			{
				in_id = in_part_ex;
				out_id = out_part_ex;
				v_angle = acos(max_angle_cos);

				return true;
			}
			else
			{
				in_id = 0;
				out_id = out_part;
				v_angle = acos(out_angle_cos);

				return true;
			}
		}

		if (max_angle_cos <= 0)
		{
			in_id = in_part_ex;
			out_id = out_part_ex;
			v_angle = acos(max_angle_cos);

			return true;
		}
		return false;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	osg::Group* JointNodeFactory::generateJointNode(UdgJointEntry& v_entry)
	{
		osg::ref_ptr<osg::Group> jointGroup = new osg::Group;

		osg::ref_ptr<osg::Geode> v_main_geode = generateJointMainGeode(v_entry);
		if (!v_main_geode) return 0L;

		jointGroup->addChild(v_main_geode);

		for (int ni = 0; ni < (int)v_entry.m_joint_param.m_lateral_part.size(); ni++)
		{
			osg::Geode* v_lateral_geom = generateJointLateralGeode(v_entry, ni);
			if (!v_lateral_geom) continue;
			jointGroup->addChild(v_lateral_geom);
		}
		return jointGroup.release();
	}

	osg::Geode* JointNodeFactory::generateJointMainGeode(UdgJointEntry& v_entry)
	{
		osg::ref_ptr<osg::Geode> joint_geode = new osg::Geode;

		// 主管半径;
		double v_max_radius = v_entry.m_joint_param.m_main_radius;
		double vMaxWidth = v_entry.m_joint_param.m_main_width;
		double vMaxHeight = v_entry.m_joint_param.m_main_height;
		// 曲率半径;
		double v_curvature_radius = v_entry.m_joint_param.m_curvature_radius;
		// 主管夹角;
		double v_angle = v_entry.m_joint_param.m_main_angle;
		// 分管长度;

		double v_joint_length;
		if (v_entry.m_jointType == UdgJointEntry::CIRCLE)
		{
			v_joint_length = v_max_radius + v_curvature_radius * (1.0 / std::tan(v_angle * 0.5));
		}
		else if (v_entry.m_jointType == UdgJointEntry::SQUARE)
		{
			v_joint_length = (vMaxWidth + vMaxHeight)*0.5 + v_curvature_radius * (1.0 / std::tan(v_angle * 0.5));
		}

		// 计算局部的坐标集合;
		osg::ref_ptr<osg::Vec3Array> vertexArray = new osg::Vec3Array;
		vertexArray->push_back(osg::Vec3d(v_entry.m_joint_param.m_main_in_part.m_direction * v_joint_length));

		if (v_angle < PI_LIMEN)
		{
			// 计算弯头坐标到主坐标;
			double v_distance = v_curvature_radius / std::sin(v_angle * 0.5);

			osg::Vec3d vec_z = -(v_entry.m_joint_param.m_main_in_part.m_direction + v_entry.m_joint_param.m_main_out_part.m_direction);
			vec_z.normalize();

			osg::Vec3d v_radian_center = -vec_z * v_distance;

			osg::Vec3d vec_y = v_entry.m_joint_param.m_main_out_part.m_direction^v_entry.m_joint_param.m_main_in_part.m_direction;
			vec_y.normalize();

			osg::Vec3d vec_x = vec_y^ vec_z;
			vec_x.normalize();

			osg::Vec3d vec_b = v_radian_center;
			osg::Matrixd v_mat;
			v_mat(0, 0) = vec_x[0]; v_mat(0, 1) = vec_x[1]; v_mat(0, 2) = vec_x[2]; v_mat(0, 3) = 0.0;
			v_mat(1, 0) = vec_y[0]; v_mat(1, 1) = vec_y[1]; v_mat(1, 2) = vec_y[2]; v_mat(1, 3) = 0.0;
			v_mat(2, 0) = vec_z[0]; v_mat(2, 1) = vec_z[1]; v_mat(2, 2) = vec_z[2]; v_mat(2, 3) = 0.0;
			v_mat(3, 0) = vec_b[0]; v_mat(3, 1) = vec_b[1]; v_mat(3, 2) = vec_b[2]; v_mat(3, 3) = 1.0;

			// 计算弯头分段;
			double v_arc_angle = osg::PI - v_angle;
			int v_piece_num = int(v_arc_angle / ONE_PIECE_RADIAN_OF_CYCLE) + 2;

			double v_start_angle = -v_arc_angle * 0.5;
			double v_angle_step = v_arc_angle / v_piece_num;
			for (int ni = 0; ni < v_piece_num + 1; ni++)
			{
				double tmp_angle = v_start_angle + ni * v_angle_step;
				osg::Vec3d vec_point(v_curvature_radius * std::sin(tmp_angle), 0.0, v_curvature_radius * std::cos(tmp_angle));
				vec_point = vec_point * v_mat;

				assert(!vec_point.isNaN());
				vertexArray->push_back(vec_point);
			}
		}
		vertexArray->push_back(v_entry.m_joint_param.m_main_out_part.m_direction * v_joint_length /*+ v_entry.m_location*/);

		// 生成模型;
		osg::ref_ptr<osg::Geometry> joint_part_geom = NULL;
		if (v_entry.m_jointType == UdgJointEntry::CIRCLE)
		{
			joint_part_geom = generateCircleJointPartGeometry(v_max_radius, *vertexArray, v_entry.m_texture_scl);
		}
		else if (v_entry.m_jointType == UdgJointEntry::SQUARE)
		{
			joint_part_geom = generateSquareJointPartGeometry(vMaxWidth, vMaxHeight, *vertexArray, v_entry.m_texture_scl);
		}

		if (joint_part_geom&&joint_part_geom != NULL)
		{
			joint_geode->addDrawable(joint_part_geom);
		}

		// 返回;
		if (v_entry.m_generate_tooth)
		{
			osg::Geometry* cap1_mesh = NULL;
			osg::Geometry* cap2_mesh = NULL;

			if (v_entry.m_jointType == UdgJointEntry::CIRCLE)
			{
				osg::Vec3d cap1_start_pt = v_entry.m_joint_param.m_main_in_part.m_direction * v_joint_length /*+ v_entry.m_location*/;
				osg::Vec3d cap1_end_pt = v_entry.m_joint_param.m_main_in_part.m_direction * (v_joint_length + v_max_radius * 0.618) /*+ v_entry.m_location*/;
				cap1_mesh = PipelineMeshGenerateHelper::createEncloseCircularTube(cap1_start_pt, cap1_end_pt,
					-v_entry.m_joint_param.m_main_in_part.m_direction, v_entry.m_joint_param.m_main_in_part.m_direction,
					v_max_radius * 1.15, v_entry.m_texture_scl/*, v_entry.m_material*/);

				osg::Vec3d cap2_start_pt = v_entry.m_joint_param.m_main_out_part.m_direction * v_joint_length/*+ v_entry.m_location*/;
				osg::Vec3d cap2_end_pt = v_entry.m_joint_param.m_main_out_part.m_direction * (v_joint_length + v_max_radius * 0.618)/*+ v_entry.m_location*/;
				cap2_mesh = PipelineMeshGenerateHelper::createEncloseCircularTube(cap2_start_pt, cap2_end_pt,
					-v_entry.m_joint_param.m_main_out_part.m_direction, v_entry.m_joint_param.m_main_out_part.m_direction,
					v_max_radius * 1.15, v_entry.m_texture_scl/*, v_entry.m_material*/);
			}
			else if (v_entry.m_jointType == UdgJointEntry::SQUARE)
			{
				osg::Vec3d cap1_start_pt = v_entry.m_joint_param.m_main_in_part.m_direction * v_joint_length;
				osg::Vec3d cap1_end_pt = v_entry.m_joint_param.m_main_in_part.m_direction * (v_joint_length + (vMaxWidth + vMaxHeight)*0.5 * 0.618);
				cap1_mesh = PipelineMeshGenerateHelper::create_enclose_square_tube(cap1_start_pt, cap1_end_pt,
					-v_entry.m_joint_param.m_main_in_part.m_direction, v_entry.m_joint_param.m_main_in_part.m_direction,
					vMaxWidth * 1.15, vMaxHeight*1.15, v_entry.m_texture_scl);

				osg::Vec3d cap2_start_pt = v_entry.m_joint_param.m_main_out_part.m_direction * v_joint_length;
				osg::Vec3d cap2_end_pt = v_entry.m_joint_param.m_main_out_part.m_direction * (v_joint_length + (vMaxWidth + vMaxHeight)*0.5 * 0.618);
				cap2_mesh = PipelineMeshGenerateHelper::create_enclose_square_tube(cap2_start_pt, cap2_end_pt,
					-v_entry.m_joint_param.m_main_out_part.m_direction, v_entry.m_joint_param.m_main_out_part.m_direction,
					vMaxWidth * 1.15, vMaxHeight*1.15, v_entry.m_texture_scl);
			}

			if (cap1_mesh&&cap1_mesh != NULL) joint_geode->addDrawable(cap1_mesh);
			if (cap2_mesh&&cap2_mesh != NULL) joint_geode->addDrawable(cap2_mesh);
		}

		return joint_geode.release();
	}

	osg::Geometry* JointNodeFactory::generateCircleJointPartGeometry(double v_max_radius, const osg::Vec3Array& v_point_array, float texScl)
	{
		if (v_point_array.size() < 2) return 0L;

		//int cycleSegmentPlus1 = cycleSegment+1;
		//计算纹理坐标用的;
		int v_point_size = v_point_array.size();
		double sum_len = 0.0;
		for (int ni = 0; ni < v_point_size - 1; ni++)
		{
			sum_len += (v_point_array[ni + 1] - v_point_array[ni]).length();
		}

		// 获取模版;
		osg::ref_ptr<osg::Vec3Array> vec_template = new osg::Vec3Array;
		PipelineMeshGenerateHelper::createCircleClockD(vec_template, osg::Vec3d(0.0, 0.0, 0.0), v_max_radius, CYCLE_SEGMENT);
		vec_template->push_back((*vec_template)[0]);

		// 生成顶点;
		int vert_idx = 0;

		// 第一个面;
		osg::Vec3d vec_dir = v_point_array[1] - v_point_array[0];
		vec_dir.normalize();

		osg::Matrixd v_mat1 = PipelineGenerateHelper::generateTbnMatrix(vec_dir, v_point_array[0]);

		float u_step = 1.0f*2.0f*osg::PI*v_max_radius / ((float)CYCLE_SEGMENT)*texScl;
		float v_step = 1.0f / sum_len*texScl;

		int vertexSize = v_point_size*CYCLE_SEGMENT_PLUS1;
		osg::ref_ptr<osg::Vec3Array> va = new osg::Vec3Array;
		va->reserve(vertexSize);
		osg::ref_ptr<osg::Vec3Array> na = new osg::Vec3Array;
		na->reserve(vertexSize);
		osg::ref_ptr<osg::Vec2Array> ta = new osg::Vec2Array;
		ta->reserve(vertexSize);
		//////////////////////////////////////////////////////////////////////////
		for (int ni = 0; ni < CYCLE_SEGMENT_PLUS1; ni++)
		{
			(*vec_template)[ni] = (*vec_template)[ni] * v_mat1;
			va->push_back((*vec_template)[ni]);

			osg::Vec3d nor = (*vec_template)[ni] - v_point_array[0];
			nor.normalize();
			na->push_back(nor);

			osg::Vec2 v_tex_coord = osg::Vec2(((float)ni)*u_step, 0.0);
			ta->push_back(v_tex_coord);
		}

		//中间的面;
		for (int ni = 1; ni < v_point_size - 1; ni++)
		{
			//计算射线方向;
			osg::Vec3d ray_dir = v_point_array[ni] - v_point_array[ni - 1];
			ray_dir.normalize();

			osg::Plane v_plane = PipelineGenerateHelper::calculateNormalPlane(v_point_array[ni - 1], v_point_array[ni], v_point_array[ni + 1]);
			for (int nj = 0; nj < CYCLE_SEGMENT_PLUS1; nj++)
			{
				osg::Vec3d plane_normal = v_plane.getNormal();
				plane_normal.normalize();

				double cos_angle = plane_normal*ray_dir;
				double dis = v_plane.distance(vec_template->at(nj));
				double len = dis / cos_angle;

				osg::Vec3d v_vec = (vec_template->at(nj) - ray_dir*len);

				assert(!v_vec.isNaN());
				va->push_back(v_vec);

				osg::Vec3d nor = v_vec - v_point_array[ni];
				nor.normalize();
				assert(!nor.isNaN());
				na->push_back(nor);

				osg::Vec2 v_tex_coord = osg::Vec2(((float)nj)*u_step, ((float)ni)*v_step);
				ta->push_back(v_tex_coord);

				vec_template->at(nj) = v_vec;
			}
		}

		// 最后一个面;
		osg::Vec3d ray_dir = v_point_array[v_point_size - 1] - v_point_array[v_point_size - 2];
		ray_dir.normalize();

		osg::Plane v_plane(ray_dir, v_point_array[v_point_size - 1]);
		for (int nj = 0; nj < CYCLE_SEGMENT_PLUS1; nj++)
		{
			osg::Vec3d plane_normal = v_plane.getNormal();
			double cos_angle = plane_normal*ray_dir;
			double dis = v_plane.distance(vec_template->at(nj));
			double len = dis / cos_angle;

			osg::Vec3d v_vec = vec_template->at(nj) - ray_dir*len;//v_vec是v_plane与ray的交点
			assert(!v_vec.isNaN());
			va->push_back(v_vec);

			osg::Vec3d nor = v_vec - v_point_array[v_point_size - 1];
			nor.normalize();
			assert(!nor.isNaN());
			na->push_back(nor);

			osg::Vec2 v_tex_coord = osg::Vec2(((float)nj)*u_step, texScl);
			ta->push_back(v_tex_coord);
		}

		osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
		geom->setVertexArray(va);
		geom->setNormalArray(na);
		geom->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);
		geom->setTexCoordArray(0, ta);

		for (int ni = 0; ni < v_point_size - 1; ni++)
		{
			osg::DrawElementsUInt* cylinderFaceOne = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLE_STRIP, 0);
			for (int nj = 0; nj < CYCLE_SEGMENT_PLUS1; nj++)
			{
				unsigned int index_1 = nj + ni*CYCLE_SEGMENT_PLUS1;
				unsigned int index_2 = index_1 + CYCLE_SEGMENT_PLUS1;

				cylinderFaceOne->push_back(index_1);
				cylinderFaceOne->push_back(index_2);
			}
			geom->addPrimitiveSet(cylinderFaceOne);
		}

		return geom.release();
	}

	osg::Geometry* JointNodeFactory::generateSquareJointPartGeometry(double vWidth, double vHeight, const osg::Vec3Array& v_point_array, float texScl)
	{
		if (v_point_array.size() < 2) return NULL;

		//计算纹理坐标用的;
		int v_point_size = v_point_array.size();
		double sum_len = 0.0;
		for (int ni = 0; ni < v_point_size - 1; ni++)
		{
			sum_len += (v_point_array[ni + 1] - v_point_array[ni]).length();
		}

		// 获取模版;
		osg::ref_ptr<osg::Vec3Array> vec_template = new osg::Vec3Array;
		PipelineMeshGenerateHelper::createRectangleD(vec_template, osg::Vec3d(0.0, 0.0, 0.0), vWidth, vHeight);
		vec_template->push_back((*vec_template)[0]);

		// 生成顶点;
		int vert_idx = 0;

		// 第一个面;
		osg::Vec3d vec_dir = v_point_array[1] - v_point_array[0];
		vec_dir.normalize();

		osg::Matrixd v_mat1 = PipelineGenerateHelper::generateTbnMatrix(vec_dir, v_point_array[0]);

		float u_step = 1.0f*2.0f*(vWidth + vHeight) / ((float)BLOCK_SEGMENT)*texScl;
		float v_step = 1.0f / sum_len*texScl;

		int vertexSize = v_point_size*BLOCK_SEGMENT_PLUS1;
		osg::ref_ptr<osg::Vec3Array> va = new osg::Vec3Array;
		va->reserve(vertexSize);
		osg::ref_ptr<osg::Vec3Array> na = new osg::Vec3Array;
		na->reserve(vertexSize);
		osg::ref_ptr<osg::Vec2Array> ta = new osg::Vec2Array;
		ta->reserve(vertexSize);
		//////////////////////////////////////////////////////////////////////////
		for (int ni = 0; ni < BLOCK_SEGMENT_PLUS1; ni++)
		{
			(*vec_template)[ni] = (*vec_template)[ni] * v_mat1;
			va->push_back((*vec_template)[ni]);

			osg::Vec3d nor = (*vec_template)[ni] - v_point_array[0];
			nor.normalize();
			na->push_back(nor);

			osg::Vec2 v_tex_coord = osg::Vec2(((float)ni)*u_step, 0.0);
			ta->push_back(v_tex_coord);
		}

		//中间的面;
		for (int ni = 1; ni < v_point_size - 1; ni++)
		{
			//计算射线方向;
			osg::Vec3d ray_dir = v_point_array[ni] - v_point_array[ni - 1];
			ray_dir.normalize();

			osg::Plane v_plane = PipelineGenerateHelper::calculateNormalPlane(v_point_array[ni - 1], v_point_array[ni], v_point_array[ni + 1]);
			for (int nj = 0; nj < BLOCK_SEGMENT_PLUS1; nj++)
			{
				osg::Vec3d plane_normal = v_plane.getNormal();
				plane_normal.normalize();

				double cos_angle = plane_normal*ray_dir;
				double dis = v_plane.distance(vec_template->at(nj));
				double len = dis / cos_angle;

				osg::Vec3d v_vec = (vec_template->at(nj) - ray_dir*len);

				assert(!v_vec.isNaN());
				va->push_back(v_vec);

				osg::Vec3d nor = v_vec - v_point_array[ni];
				nor.normalize();
				assert(!nor.isNaN());
				na->push_back(nor);

				osg::Vec2 v_tex_coord = osg::Vec2(((float)nj)*u_step, ((float)ni)*v_step);
				ta->push_back(v_tex_coord);

				vec_template->at(nj) = v_vec;
			}
		}

		// 最后一个面;
		osg::Vec3d ray_dir = v_point_array[v_point_size - 1] - v_point_array[v_point_size - 2];
		ray_dir.normalize();

		osg::Plane v_plane(ray_dir, v_point_array[v_point_size - 1]);
		for (int nj = 0; nj < BLOCK_SEGMENT_PLUS1; nj++)
		{
			osg::Vec3d plane_normal = v_plane.getNormal();
			double cos_angle = plane_normal*ray_dir;
			double dis = v_plane.distance(vec_template->at(nj));
			double len = dis / cos_angle;

			osg::Vec3d v_vec = vec_template->at(nj) - ray_dir*len;//v_vec是v_plane与ray的交点
			assert(!v_vec.isNaN());
			va->push_back(v_vec);

			osg::Vec3d nor = v_vec - v_point_array[v_point_size - 1];
			nor.normalize();
			assert(!nor.isNaN());
			na->push_back(nor);

			osg::Vec2 v_tex_coord = osg::Vec2(((float)nj)*u_step, texScl);
			ta->push_back(v_tex_coord);
		}

		osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
		geom->setVertexArray(va);
		geom->setNormalArray(na);
		geom->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);
		geom->setTexCoordArray(0, ta);

		for (int ni = 0; ni < v_point_size - 1; ni++)
		{
			osg::DrawElementsUInt* cylinderFaceOne = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLE_STRIP, 0);
			for (int nj = 0; nj < BLOCK_SEGMENT_PLUS1; nj++)
			{
				unsigned int index_1 = nj + ni*BLOCK_SEGMENT_PLUS1;
				unsigned int index_2 = index_1 + BLOCK_SEGMENT_PLUS1;

				cylinderFaceOne->push_back(index_1);
				cylinderFaceOne->push_back(index_2);
			}
			geom->addPrimitiveSet(cylinderFaceOne);
		}

		return geom.release();
	}


	osg::Geode* JointNodeFactory::generateJointLateralGeode(UdgJointEntry& v_entry, int v_index)
	{
		osg::ref_ptr<osg::Geode> joint_lateral_geode = new osg::Geode;

		JointPartInfo& part_info = v_entry.m_joint_param.m_lateral_part[v_index];

		// 分管长度;
		double v_joint_length = part_info.m_radius + v_entry.m_joint_param.m_main_radius;

		osg::ref_ptr<osg::Vec3Array> v_point_array = new osg::Vec3Array;
		v_point_array->push_back(osg::Vec3d(0.0, 0.0, 0.0));
		v_point_array->push_back(part_info.m_direction*v_joint_length);

		// 生成模型;
		osg::ref_ptr<osg::Geometry> geom = NULL;
		if (v_entry.m_jointType == UdgJointEntry::CIRCLE)
		{
			geom = JointNodeFactory::generateCircleJointPartGeometry(part_info.m_radius, *v_point_array, v_entry.m_texture_scl);
		}
		else if (UdgJointEntry::SQUARE)
		{
			geom = JointNodeFactory::generateSquareJointPartGeometry(part_info.m_width, part_info.m_height, *v_point_array, v_entry.m_texture_scl);
		}
		if (geom && geom != NULL)
		{
			joint_lateral_geode->addDrawable(geom);
		}

		osg::ref_ptr<osg::Geometry> cap1_mesh = NULL;
		if (v_entry.m_generate_tooth)
		{
			if (v_entry.m_jointType == UdgJointEntry::CIRCLE)
			{
				osg::Vec3d cap1_start_pt = part_info.m_direction * v_joint_length;
				osg::Vec3d cap1_end_pt = cap1_start_pt + part_info.m_direction * part_info.m_radius * 0.618;

				cap1_mesh = PipelineMeshGenerateHelper::createEncloseCircularTube(cap1_start_pt, cap1_end_pt,
					part_info.m_direction, part_info.m_direction,
					part_info.m_radius * 1.15, v_entry.m_texture_scl);
			}
			else if (v_entry.m_jointType == UdgJointEntry::SQUARE)
			{
				osg::Vec3d cap1_start_pt = part_info.m_direction * v_joint_length;
				osg::Vec3d cap1_end_pt = cap1_start_pt + part_info.m_direction * (part_info.m_width + part_info.m_height)*0.5 * 0.618;

				cap1_mesh = PipelineMeshGenerateHelper::create_enclose_square_tube(cap1_start_pt, cap1_end_pt,
					part_info.m_direction, part_info.m_direction,
					part_info.m_width * 1.15, part_info.m_height*1.15, v_entry.m_texture_scl);
			}
			if (cap1_mesh&&cap1_mesh != NULL)
			{
				joint_lateral_geode->addDrawable(cap1_mesh);
			}
		}

		return joint_lateral_geode.release();
	}

	//osg::Geometry* PipelineJointFactory::generateCircleJointScrewCapGeometry( osg::Vec3d& s_point, osg::Vec3d& e_point, double v_radius, float texScl )
	//{
	//	osg::Vec3d s_dir = e_point - s_point;
	//	s_dir.normalize();
	//
	//	osg::Vec3d e_dir = s_dir;
	//
	//	return PipelineMeshGenerateHelper::createEncloseCircularTube( s_point, e_point, s_dir, e_dir, v_radius, texScl );
	//}
	//
	//osg::Geometry* PipelineJointFactory::generateSquareJointScrewCapGeometry( osg::Vec3d& s_point, osg::Vec3d& e_point, double vWidth, double vHeigh, float texScl )
	//{
	//	osg::Vec3d s_dir = e_point - s_point;
	//	s_dir.normalize();
	//
	//	osg::Vec3d e_dir = s_dir;
	//
	//	return PipelineMeshGenerateHelper::create_enclose_square_tube( s_point, e_point, s_dir, e_dir, vWidth, vHeigh, texScl );
	//}
}