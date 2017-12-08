#include <gwUtil/ShapeUtils.h>

namespace gwUtil
{
	osg::Vec3Array* ShapeUtil::create_circle(double radius, long sides)
	{
		osg::ref_ptr<osg::Vec3Array> v_circle = new osg::Vec3Array;
		v_circle->resize(sides);

		double angle_step = osg::PI * 2 / sides;
		double angle_curr = 0;
		for (int i = 0; i < sides; i++)
		{
			angle_curr = angle_step * i;
			v_circle->at(i).x() = radius * std::sin(angle_curr);
			v_circle->at(i).y() = 0.0;
			v_circle->at(i).z() = radius * std::cos(angle_curr);
		}
		return v_circle;
	}


	osg::Vec3Array* ShapeUtil::create_polygon_from_arrow(
		osg::Vec3d pnt_begin
		, osg::Vec3d pnt_end
		, double tail_length
		, double tail_width
		, double head_length
		, double head_width
		, double head_root_length
		, double head_root_width)
	{
		osg::ref_ptr<osg::Vec3Array> poly = new osg::Vec3Array;

		// 转geocentric;
		osg::Vec3d pnt_begin_geoc/* = geocentric_coords::from_spherical_d(pnt_begin)*/;
		osg::Vec3d pnt_end_geoc  /* = geocentric_coords::from_spherical_d(pnt_end)*/;

		// 获取基准值;
		osg::Vec3d vec_arrow = pnt_end_geoc - pnt_begin;
		osg::Vec3d pnt_tail = pnt_begin_geoc + vec_arrow * tail_length;
		osg::Vec3d pnt_head_base = pnt_begin_geoc + vec_arrow * (1 - head_length);
		osg::Vec3d pnt_head_root_base = pnt_begin_geoc + vec_arrow * (1 - head_root_length);

		// 获取相关顶点;
		double length = (pnt_begin_geoc - pnt_end_geoc).length();
		osg::Vec3d normal = pnt_begin_geoc ^ pnt_end_geoc;
		normal.normalize();

		osg::Vec3d pnt_tail_left = pnt_begin_geoc + normal * length * tail_width;
		osg::Vec3d pnt_tail_right = pnt_begin_geoc - normal * length * tail_width;

		osg::Vec3d pnt_head_left = pnt_head_base + normal * length * head_width;
		osg::Vec3d pnt_head_right = pnt_head_base - normal * length * head_width;

		osg::Vec3d pnt_head_root_left = pnt_head_root_base + normal * length * head_root_width;
		osg::Vec3d pnt_head_root_right = pnt_head_root_base - normal * length * head_root_width;

		////
		//// 顺序：tail;tail_left;head_root_left;head_left;end;head_right;head_root_right;tail_right;
		////
		//poly->push_back(spherical_coords::from_geocentric_d(pnt_tail));
		//poly->push_back(spherical_coords::from_geocentric_d(pnt_tail_left));
		//poly->push_back(spherical_coords::from_geocentric_d(pnt_head_root_left));
		//poly->push_back(spherical_coords::from_geocentric_d(pnt_head_left));
		//poly->push_back(spherical_coords::from_geocentric_d(pnt_end_geoc));
		//poly->push_back(spherical_coords::from_geocentric_d(pnt_head_right));
		//poly->push_back(spherical_coords::from_geocentric_d(pnt_head_root_right));
		//poly->push_back(spherical_coords::from_geocentric_d(pnt_tail_right));

		return poly;
	}
}