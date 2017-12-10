#ifndef GWPIPELINE_PIPELINEJOINTFACTORY_H
#define GWPIPELINE_PIPELINEJOINTFACTORY_H 1

//#include <gwPipeline/PipelineEntryJoint.h>

#include <osg/Vec3d>
#include <osg/Geode>
#include <osg/Geometry>

#define MAIN_PIPE_VERTEX_NUM		(CYCLE_SEGMENT_PLUS1*7)
#define MAIN_PIPE_FACE_NUM			(CYCLE_SEGMENT*12-8)
#define MAIN_PIPE_FIGURE_NUM		(CYCLE_SEGMENT*4+32)

#define LATERAL_PIPE_VERTEX_NUM		(CYCLE_SEGMENT_PLUS1*4)
#define LATERAL_PIPE_FACE_NUM		(CYCLE_SEGMENT*6-4)
#define LATERAL_PIPE_FIGURE_NUM		(CYCLE_SEGMENT*2+16)

#define ONE_PIECE_RADIAN_OF_CYCLE	0.19634954084936207740391521145497

#define SEMI_MAJOR_CART		6378137.0
#define SEMI_MAJOR_GEO		6378.137
#define PI_LIMEN			3.1258846903218442722703301663631
#define PI_LIMEN_COS	   -0.99987663248166059863890712773125

namespace gwPipeline
{
	typedef std::vector<double> DoubleValueArray;
	typedef std::vector<osg::Vec3d> Vector3dArray;//


	struct JointPartInfo
	{
		JointPartInfo() :m_radius(0.0), m_width(0.0), m_height(0.0), m_line_id(-1)
		{
			m_direction = osg::Vec3d(0.0, 0.0, 0.0);
		}

		int m_line_id;
		double m_radius, m_width, m_height;
		osg::Vec3d m_direction;
	};
	typedef std::vector<JointPartInfo> JointPartInfoArray;

	struct JointParameter
	{
		JointParameter() :m_valid(false), m_main_radius(0.0), m_main_width(0.0)
			, m_main_height(0.0), m_curvature_radius(0.0), m_main_angle(0.0)
		{ }

		bool m_valid;
		double m_main_radius, m_main_width, m_main_height, m_main_angle;
		double m_curvature_radius;
		JointPartInfo m_main_in_part;
		JointPartInfo m_main_out_part;
		JointPartInfoArray m_lateral_part;
	};

	class UdgJointEntry
	{
	public:
		UdgJointEntry();
		UdgJointEntry(const UdgJointEntry& v_entry);
		~UdgJointEntry() { }

	public:
		enum JointType { CIRCLE, SQUARE };

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
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



	class JointEntryGenerator /*PipelineGenerateBase*/
	{
	public:
		JointEntryGenerator(void);
		~JointEntryGenerator(void);

	public:
		void checkPointType(UdgJointEntry* us_element_joint, const Vector3dArray& in_relate_direction, const DoubleValueArray& radius, int relate_num);
		void checkPointType(UdgJointEntry* us_element_joint, const Vector3dArray& in_relate_direction, const DoubleValueArray& widths, const DoubleValueArray& heights, int relate_num);

	private:
		bool calculateMultiJointParam(JointPartInfoArray& v_part_array, int& in_id, int& out_id, double& v_angle);
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class JointNodeFactory
	{
	public:
		JointNodeFactory(void) { }
		~JointNodeFactory(void) { }

	public:
		static osg::Group* generateJointNode(UdgJointEntry& v_entry);

	private:
		static osg::Geode* generateJointMainGeode(UdgJointEntry& v_entry);
		static osg::Geode* generateJointLateralGeode(UdgJointEntry& v_entry, int v_index);

		static osg::Geometry* generateCircleJointPartGeometry(double v_radius, const osg::Vec3Array& v_point_array, float texScl);
		static osg::Geometry* generateSquareJointPartGeometry(double vWidth, double vHeight, const osg::Vec3Array& v_point_array, float texScl);

		//static osg::Geometry* generateCircleJointScrewCapGeometry( osg::Vec3d& s_point, osg::Vec3d& e_point, double v_radius, float texScl );
		//static osg::Geometry* generateSquareJointScrewCapGeometry( osg::Vec3d& s_point, osg::Vec3d& e_point, double vWidth, double vHeigh, float texScl );

	};
}

#endif // GWPIPELINE_PIPELINEJOINTFACTORY_H
