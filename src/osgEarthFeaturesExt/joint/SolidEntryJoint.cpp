#include <mdPipeline/PipelineEntryJoint.h>

namespace mdPipeline
{
	UdgJointEntry::UdgJointEntry()
	{
		m_id = -1;
		m_joint_thickness = 0.02;
		m_generate_tooth = true;

		m_location = osg::Vec3d(0.0,0.0,0.0);

		m_texture_scl = 1.0f;

		m_jointType = CIRCLE;
	}

	UdgJointEntry::UdgJointEntry( const UdgJointEntry& v_entry )
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
}
