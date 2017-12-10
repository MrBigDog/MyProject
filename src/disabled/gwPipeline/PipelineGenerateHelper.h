#ifndef GWPIPELINE_PIPELINEGENERATEHELPER_H
#define GWPIPELINE_PIPELINEGENERATEHELPER_H 1

#include <osg/Matrixd>
#include <osg/Vec3d>
#include <osg/Array>
#include <osg/Plane>

#include <vector>

#define SEMI_MAJOR_CART		6378137.0
#define SEMI_MAJOR_GEO		6378.137
#define PI_LIMEN			3.1258846903218442722703301663631
#define PI_LIMEN_COS		-0.99987663248166059863890712773125

namespace gwPipeline
{
	class PipelineGenerateHelper
	{
	public:
		PipelineGenerateHelper( void ){}

		~PipelineGenerateHelper( void ){}

	public:
		static osg::Matrixd generateTbnMatrix( const osg::Vec3d& vec_n, const osg::Vec3d& vec_trans );
		static osg::Matrixd generateTbnMatrix( const osg::Vec3d& vec_n, const osg::Vec3d& vec_trans, const osg::Vec3d& vec_scl );

		static osg::Plane calculateNormalPlane( const osg::Vec3d& pre_pt, const osg::Vec3d& cur_pt, const osg::Vec3d& nxt_pt );
		//
		//static osg::Vec3Array* generate_cylinder_vertex( double v_raidus, double s_len, double e_len, long v_segment );
		//
		//	static double calculate_line_heading( vector3<double>& s_pt, vector3<double>& e_pt );
		//
		//	static void calculate_orient( vector3<double> start_point, vector3<double> end_point, double& v_heading, vector3<double>& v_direct );
		//
		//public:
		//	static double get_pipeline_thickness( double v_radius );
		//
		//public:
		//	static bool material_equal( usx_material* mat1, usx_material* mat2 );
		//
		//	static usx_mesh* combine_mesh( usx_mesh_array& v_mesh_array );
		//
		//public:
		//	static void read_pipeline_size( char* str_value, double& v_diameter, double& v_width, double& v_height );
		//
		//public:
		//	//static void generate_pure_color_texture( LPCTSTR str_path, ulong v_color );

	};
}

#endif // GWPIPELINE_PIPELINEGENERATEHELPER_H