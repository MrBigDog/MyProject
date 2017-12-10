#ifndef PipelineMeshGenerateHelper_h__
#define PipelineMeshGenerateHelper_h__

#include <mdBase/export.h>
#include <osg/Vec3d>
#include <osg/Geometry>

namespace mdPipeline
{
	class MD_EXPORT PipelineMeshGenerateHelper
	{
	public:
		PipelineMeshGenerateHelper( void ){}
		~PipelineMeshGenerateHelper( void ){}

	public:
		static osg::Geometry* createEncloseCircularTube( const osg::Vec3d& s_point, const osg::Vec3d& e_point, const osg::Vec3d& s_dir, const osg::Vec3d& e_dir, double v_radius, float texScl );
		static osg::Geometry* create_enclose_square_tube(const osg::Vec3d& s_point, const osg::Vec3d& e_point, const osg::Vec3d& s_dir, const osg::Vec3d& e_dir, double v_width, double v_height, float texScl);

		static void createCircleClockD( osg::Vec3Array* outCirclePoints, const osg::Vec3d& inCenterPoint, double inRadiusRadians, int inSegment );
		static void createRectangleD( osg::Vec3Array* out_rec_points, const osg::Vec3d& in_center_point, double width, double height );

		//static usx_material* create_material_from_entry( PplMaterialEntry& v_entry );
	};
}

#endif // PipelineMeshGenerateHelper_h__