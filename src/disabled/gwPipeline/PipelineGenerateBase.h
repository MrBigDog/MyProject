#ifndef GWPIPELINE_PIPELINEGENERATEBASE_H
#define GWPIPELINE_PIPELINEGENERATEBASE_H 1

#include <gwPipeline/PipelineEntryJoint.h>

#include <vector>
#include <osg/Vec3d>

#define PI_LIMEN_COS		-0.99987663248166059863890712773125

namespace gwPipeline
{
	typedef std::vector<double> DoubleValueArray;
	typedef std::vector<osg::Vec3d> Vector3dArray;//

	class PipelineGenerateBase
	{
	public:
		PipelineGenerateBase(void);
		~PipelineGenerateBase(void);

	public:
		void checkPointType(UdgJointEntry* us_element_joint, const Vector3dArray& in_relate_direction, const DoubleValueArray& radius, int relate_num);
		void checkPointType(UdgJointEntry* us_element_joint, const Vector3dArray& in_relate_direction, const DoubleValueArray& widths, const DoubleValueArray& heights, int relate_num);

	private:
		bool calculateMultiJointParam(JointPartInfoArray& v_part_array, int& in_id, int& out_id, double& v_angle);

		//public:
		//	// ת�ֲ�����
		//	void geo_to_cart( coordinates_array& coords, vector_3d vec_center );
		//public:
		//	void PolygonToInner( IUSVector3s* inPoly, coordinates_array& outPolyInner );

		//	void PolygonToInner( vector_3d* inPoly, int nCount,coordinates_array& outPolyInner );

		//public:
		//	void get_section_circle( double radius, int seg_num, coordinates_array& section_vertex, vector<vector_2d>& section_uv );

		//	//void get_section_ex(double radius, int seg_num, int inPos,coordinates_array& section_vertex, vector<vector_2d>& section_uv );

		//	void get_section_rect( double width, double height, coordinates_array& section_vertex, vector<vector_2d>& section_uv );	
		//	void get_tunnel_vertex( coordinates_array& polyline, coordinates_array& section_vertex, coordinates_array& tunnel_vetex );

		//	//void get_vertex( coordinates_array& polyline, coordinates_array& section_vertex, coordinates_array& tunnel_vetex );

		//	d3d9_hardware_mesh* generate_tunnel_mesh( coordinates_array& tunnel_vertex_outer, coordinates_array& tunnel_vertex_inner, vector<vector_2d>& section_uv, int seg_num, ustring texPath );

	};
}

#endif // GWPIPELINE_PIPELINEGENERATEBASE_H
