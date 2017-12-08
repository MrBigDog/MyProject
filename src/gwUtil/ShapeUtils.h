#ifndef GWUTIL_SHAPEUTILS_H
#define GWUTIL_SHAPEUTILS_H 1

#include <gwUtil/Export.h>
#include <Compiler.h>
#include <osg/Vec3d>
#include <osg/Array>

namespace gwUtil
{
	class GWUTIL_EXPORT ShapeUtil
	{
	public:
		static osg::Vec3Array* create_circle(double radius, long sides);

		// ���ɼ�ͷ����Ρ�����double������Ϊ��������Χ0~1֮�䣻
		static osg::Vec3Array* create_polygon_from_arrow(
			osg::Vec3d pnt_begin
			, osg::Vec3d pnt_end
			, double tail_length
			, double tail_width
			, double head_length
			, double head_width
			, double head_root_length
			, double head_root_width);

		static osg::Vec3Array* create_polygon_from_arrow_geo(
			osg::Vec3d pnt_begin,
			osg::Vec3d pnt_end,
			double tail_length,
			double tail_width,
			double head_length,
			double head_width,
			double head_root_length,
			double head_root_width);

		static osg::Vec3Array* create_polygon_from_arrow_geo(
			osg::Vec3d pnt_begin,
			osg::Vec3d pnt_mid,
			osg::Vec3d pnt_end,
			double tail_length,
			double tail_width,
			double head_length,
			double head_width,
			double head_root_length,
			double head_root_width,
			bool end_is_set = true);


		static void ComputeBezier2(osg::Vec3Array* cp, int numberOfPoints, osg::Vec3Array* curve);

		static osg::Vec3d PointOnCubicBezier2(osg::Vec3Array* cp, float t);


	};
}
#endif // GWUTIL_SHAPEUTILS_H
