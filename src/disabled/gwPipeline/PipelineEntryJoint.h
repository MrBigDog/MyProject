#ifndef GWPIPELINE_PIPELINEENTRYJOINT_H
#define GWPIPELINE_PIPELINEENTRYJOINT_H 1

#include <gwPipeline/PipelineGenerateParameter.h>

#include <osg/Vec3d>
#include <vector>
#include <string>

namespace gwPipeline
{
	struct JointPartInfo
	{
		int m_line_id;
		double m_radius;
		double m_width;
		double m_height;

		osg::Vec3d m_direction;
		JointPartInfo()
		{
			m_radius = 0.0;
			m_width = 0.0;
			m_height = 0.0;
			m_direction = osg::Vec3d(0.0,0.0,0.0);
			m_line_id = -1;
		}
	};
	typedef std::vector<JointPartInfo> JointPartInfoArray;

	struct JointParameter
	{
		bool m_valid;
		double m_main_radius;
		double m_main_width;
		double m_main_height;
		double m_curvature_radius;
		double m_main_angle;
		JointPartInfo m_main_in_part;
		JointPartInfo m_main_out_part;
		JointPartInfoArray m_lateral_part;
		JointParameter()
		{
			m_valid = false;
			m_main_radius = 0.0;
			m_main_width = 0.0;
			m_main_height= 0.0;
			m_curvature_radius = 0.0;
			m_main_angle = 0.0;
		}
	};

	class UdgJointEntry
	{
	public:
		UdgJointEntry();
		UdgJointEntry( const UdgJointEntry& v_entry );
		~UdgJointEntry(){ }

	public:
		enum JointType
		{
			CIRCLE,
			SQUARE
		};

	public:
		int m_id;
		std::string m_str_point_id;
		std::wstring m_guid;

		osg::Vec3d m_location;

		float m_texture_scl;

		PplMaterialEntry m_material;

		bool m_generate_tooth;
		double m_joint_thickness;
		JointParameter m_joint_param;

		JointType m_jointType;
	};

	typedef std::vector<UdgJointEntry> UdgJointEntryArray;
}

#endif // GWPIPELINE_PIPELINEENTRYJOINT_H