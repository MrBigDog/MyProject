#ifndef PipelineJointFactory_h__
#define PipelineJointFactory_h__



#include <mdBase/export.h>
#include <mdPipeline/PipelineEntryJoint.h>

#include <osg/Vec3d>
#include <osg/Geometry>
#include <osg/Geode>

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

namespace mdPipeline
{
	class MD_EXPORT PipelineJointFactory
	{
	public:
		PipelineJointFactory( void ) { }
		~PipelineJointFactory( void ) { }

	public:
		static osg::Group* generateJointNode( UdgJointEntry& v_entry );

	private:
		static osg::Geode* generateJointMainGeode( UdgJointEntry& v_entry );
		static osg::Geode* generateJointLateralGeode( UdgJointEntry& v_entry, int v_index );

		static osg::Geometry* generateCircleJointPartGeometry(  double v_radius, const osg::Vec3Array& v_point_array, float texScl );
		static osg::Geometry* generateSquareJointPartGeometry(  double vWidth, double vHeight, const osg::Vec3Array& v_point_array, float texScl );

		//static osg::Geometry* generateCircleJointScrewCapGeometry( osg::Vec3d& s_point, osg::Vec3d& e_point, double v_radius, float texScl );
		//static osg::Geometry* generateSquareJointScrewCapGeometry( osg::Vec3d& s_point, osg::Vec3d& e_point, double vWidth, double vHeigh, float texScl );

	};
}

#endif // PipelineJointFactory_h__
