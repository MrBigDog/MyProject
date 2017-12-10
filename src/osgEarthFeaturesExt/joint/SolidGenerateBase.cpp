#include <mdPipeline/PipelineGenerateBase.h>
#include <algorithm>

namespace mdPipeline
{
	PipelineGenerateBase::PipelineGenerateBase(void)
	{}

	PipelineGenerateBase::~PipelineGenerateBase(void)
	{}

	bool compare_large_joint_part_radius1( JointPartInfo& pt1, JointPartInfo& pt2 )
	{
		return pt1.m_radius > pt2.m_radius;
	}

	void PipelineGenerateBase::checkPointType(UdgJointEntry* us_element_joint,const Vector3dArray& in_relate_direction, const DoubleValueArray& radius,int relate_num)
	{
		if (in_relate_direction.size()<2)
		{
			return;
		}

		if (us_element_joint == NULL||!us_element_joint)
		{
			us_element_joint = new UdgJointEntry;
		}

		// 逐点判断;
		// 遍历关联线，获取有效关联线和最大管径;
		double max_joint_deep = -DBL_MAX;

		JointPartInfoArray v_part_array;

		for ( int ni = 0; ni < relate_num; ni++ )
		{
			JointPartInfo v_part_info;
			v_part_info.m_direction = in_relate_direction[ni];
			v_part_info.m_direction.normalize();
			v_part_info.m_radius = radius[ni] * 0.5;
			v_part_array.push_back( v_part_info );

			max_joint_deep = std::max( max_joint_deep, v_part_info.m_radius );
		}

		// 当点关联的线少于2时，不处理
		if ( v_part_array.size() < 2 )
			return;

		// 按管径大小排序;
		std::sort( v_part_array.begin(), v_part_array.end(), compare_large_joint_part_radius1 );

		// 当点关联的线为2时，生成弯头
		// 当两条线的夹角小于60度时，不生成弯头
		if ( v_part_array.size() == 2 )
		{
			JointPartInfo& v_line0 = v_part_array[0];
			JointPartInfo& v_line1 = v_part_array[1];

			double v_angle = v_line0.m_direction*v_line1.m_direction;

			if ( v_angle <= 0.5 )
			{
				us_element_joint->m_joint_param.m_valid = true;
				us_element_joint->m_joint_param.m_main_radius = max_joint_deep;
				us_element_joint->m_joint_param.m_curvature_radius = us_element_joint->m_joint_param.m_main_radius * 2;
				us_element_joint->m_joint_param.m_main_angle = acos( v_angle );
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
			if ( !calculateMultiJointParam( v_part_array, in_part, out_part, included_angle ) )
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

			v_part_array.erase( v_part_array.begin() + in_part );
			v_part_array.erase( v_part_array.begin() + out_part - 1 );
			us_element_joint->m_joint_param.m_lateral_part = v_part_array;
		}
	}

	void PipelineGenerateBase::checkPointType(UdgJointEntry* us_element_joint, const Vector3dArray& in_relate_direction, const DoubleValueArray& widths, const DoubleValueArray& heights, int relate_num)
	{
		if (in_relate_direction.size()<2)
		{
			return ;
		}

		if (us_element_joint == NULL||!us_element_joint)
		{
			us_element_joint = new UdgJointEntry;
		}

		// 逐点判断;
		// 遍历关联线，获取有效关联线和最大管径;
		//double max_joint_deep = -DBL_MAX;
		double max_joint_width = -DBL_MAX;
		double max_joint_height = -DBL_MAX;

		JointPartInfoArray v_part_array;

		for ( int ni = 0; ni < relate_num; ni++ )
		{
			JointPartInfo v_part_info;
			v_part_info.m_direction = in_relate_direction[ni];
			v_part_info.m_direction.normalize();
			//v_part_info.m_radius = radius[ni] * 0.5;
			v_part_info.m_width = widths[ni];
			v_part_info.m_height = heights[ni];
			v_part_array.push_back( v_part_info );

			//max_joint_deep = std::max( max_joint_deep, (v_part_info.m_width + v_part_info.m_height)*0.5 );
			max_joint_width = std::max(max_joint_width, v_part_info.m_width);
			max_joint_height = std::max(max_joint_height, v_part_info.m_height);
		}

		// 当点关联的线少于2时，不处理
		if ( v_part_array.size() < 2 )
			return;

		// 按管径大小排序;
		std::sort( v_part_array.begin(), v_part_array.end(), compare_large_joint_part_radius1 );

		// 当点关联的线为2时，生成弯头
		// 当两条线的夹角小于60度时，不生成弯头
		if ( v_part_array.size() == 2 )
		{
			JointPartInfo& v_line0 = v_part_array[0];
			JointPartInfo& v_line1 = v_part_array[1];

			double v_angle = v_line0.m_direction*v_line1.m_direction;

			if ( v_angle <= 0.5 )
			{
				us_element_joint->m_joint_param.m_valid = true;
				//us_element_joint->m_joint_param.m_main_radius = max_joint_deep;
				us_element_joint->m_joint_param.m_main_width = max_joint_width;
				us_element_joint->m_joint_param.m_main_height = max_joint_height;
				us_element_joint->m_joint_param.m_curvature_radius = us_element_joint->m_joint_param.m_main_width+us_element_joint->m_joint_param.m_main_height;
				us_element_joint->m_joint_param.m_main_angle = acos( v_angle );
				us_element_joint->m_joint_param.m_main_in_part = v_line0;
				us_element_joint->m_joint_param.m_main_out_part = v_line1;
			}
			return ;
		}
		else
		{
			// 当点关联的线大于2时，生成多通接头，如三通、四通等
			double included_angle = 0.0;
			int in_part = -1, out_part = -1;
			if ( !calculateMultiJointParam( v_part_array, in_part, out_part, included_angle ) )
			{
				return;
			}

			us_element_joint->m_joint_param.m_valid = true;
			//us_element_joint->m_joint_param.m_main_radius = max_joint_deep;
			us_element_joint->m_joint_param.m_main_width = max_joint_width;
			us_element_joint->m_joint_param.m_main_height = max_joint_height;
			us_element_joint->m_joint_param.m_curvature_radius = us_element_joint->m_joint_param.m_main_width+us_element_joint->m_joint_param.m_main_height;
			us_element_joint->m_joint_param.m_main_angle = included_angle;
			us_element_joint->m_joint_param.m_main_in_part = v_part_array[in_part];
			//us_element_joint->m_joint_param.m_main_in_part.m_radius = max_joint_deep;
			us_element_joint->m_joint_param.m_main_in_part.m_width = max_joint_width;
			us_element_joint->m_joint_param.m_main_in_part.m_height = max_joint_height;
			us_element_joint->m_joint_param.m_main_out_part = v_part_array[out_part];
			//us_element_joint->m_joint_param.m_main_out_part.m_radius = max_joint_deep;
			us_element_joint->m_joint_param.m_main_out_part.m_width = max_joint_width;
			us_element_joint->m_joint_param.m_main_out_part.m_height = max_joint_height;

			v_part_array.erase( v_part_array.begin() + in_part );
			v_part_array.erase( v_part_array.begin() + out_part - 1 );
			us_element_joint->m_joint_param.m_lateral_part = v_part_array;
		}
	}


	bool PipelineGenerateBase::calculateMultiJointParam( JointPartInfoArray& v_part_array, int& in_id, int& out_id, double& v_angle )
	{
		// 以最大管径为主管的情况3
		int out_part = 1;
		double out_angle_cos = 2.0;

		// 记录夹角非180度的主管
		int in_part_ex = 0, out_part_ex = 1;
		double max_angle_cos = 2.0;

		bool v_main_part_ok = false;
		for ( int ni = 0; ni < (int)v_part_array.size(); ni++ )
		{
			JointPartInfo& v_line0 = v_part_array[ni];

			for ( int nj = ni + 1; nj < (int)v_part_array.size(); nj++ )
			{
				JointPartInfo& v_line1 = v_part_array[nj];
				double tmp_angle = v_line0.m_direction*v_line1.m_direction;//vector_3d::dot( v_line0.m_direction, v_line1.m_direction );

				if ( tmp_angle <= PI_LIMEN_COS )
				{
					in_id = ni;
					out_id = nj;
					v_angle = acos( tmp_angle );

					return true;
				}

				if ( tmp_angle < max_angle_cos )
				{
					in_part_ex = ni;
					out_part_ex = nj;
					max_angle_cos = tmp_angle;

					if ( ni == 0 )
					{
						out_part = nj;
						out_angle_cos = tmp_angle;
					}
				}
			}
		}

		if ( out_angle_cos <= 0 )
		{
			if ( max_angle_cos < out_angle_cos )
			{
				in_id = in_part_ex;
				out_id = out_part_ex;
				v_angle = acos( max_angle_cos );

				return true;
			}
			else 
			{
				in_id = 0;
				out_id = out_part;
				v_angle = acos( out_angle_cos );

				return true;
			}
		}

		if ( max_angle_cos <= 0 )
		{
			in_id = in_part_ex;
			out_id = out_part_ex;
			v_angle = acos( max_angle_cos );

			return true;
		}
		return false;
	}
}