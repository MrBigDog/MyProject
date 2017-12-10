#include "GeneratorHelp.h"
#include "MeshConsolidator.h"

#include <osg/TriangleIndexFunctor>

#include <sstream>

//--------------------------------------------------------------------------------------
void ColumnGeometryOptimizer::apply(osg::Geode& geode)
{
	//计算纹理坐标、法线;
	for (unsigned int i = 0; i < geode.getNumDrawables(); ++i)
	{
		osg::Geometry* ogeom = geode.getDrawable(i)->asGeometry();
		if (!ogeom) continue;

		osg::ref_ptr<osg::Geometry> geom = new osg::Geometry(*ogeom, osg::CopyOp::DEEP_COPY_ALL);
		geom->setUseVertexBufferObjects(true);
		geom->setUseDisplayList(false);
		osg::Vec3Array* va = dynamic_cast<osg::Vec3Array*>(geom->getVertexArray());
		if (!va) continue;

		osg::ref_ptr<osg::Vec2Array> ta = new osg::Vec2Array;
		ta->reserve(va->size());
		osg::ref_ptr<osg::Vec3Array> na = new osg::Vec3Array;
		na->reserve(va->size());
		for (osg::Vec3Array::const_iterator vait = va->begin(); vait != va->end(); ++vait)
		{
			osg::Vec3d nc = _wellCenter;
			double dis1 = _plane.distance(nc);
			double dis2 = _plane.distance(*vait);
			nc += _dir*(dis2 - dis1);

			osg::Vec3d normal = normalize(*vait - nc);
			na->push_back(-normal);

			double angle = std::abs(getAngle(_sideDir, normal, _dir));
			double strideNum = angle / _strideAngle;

			double tc_s = (strideNum*_d);
			double tc_t = dis2;
			ta->push_back(osg::Vec2(tc_s, tc_t));
		}
		geom->setTexCoordArray(0, ta);
		geom->setNormalArray(na);
		geom->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);

		geode.replaceDrawable(ogeom, geom);
	}
}

//------------------------------------------------------------------------------------------------
void PlaneGeometryOptimizer::apply(osg::Geode & geode)
{
	//计算纹理坐标、法线;
	for (unsigned int i = 0; i < geode.getNumDrawables(); ++i)
	{
		osg::Geometry* ogeom = dynamic_cast<osg::Geometry*>(geode.getDrawable(i));
		if (!ogeom) continue;

		osg::ref_ptr<osg::Geometry> geom = new osg::Geometry(*ogeom, osg::CopyOp::DEEP_COPY_ALL);
		geom->setUseVertexBufferObjects(true);
		geom->setUseDisplayList(false);

		osg::Vec3Array* va = dynamic_cast<osg::Vec3Array*>(geom->getVertexArray());
		if (!va) continue;

		osg::ref_ptr<osg::Vec2Array> ta = new osg::Vec2Array;
		ta->reserve(va->size());
		osg::ref_ptr<osg::Vec3Array> na = new osg::Vec3Array(va->size());
		na->assign(va->size(), osg::Vec3(_normal));
		for (osg::Vec3Array::const_iterator vait = va->begin(); vait != va->end(); ++vait)
		{
			double dis_s = _plane_s.distance(*vait);
			double dis_t = _plane_t.distance(*vait);

			ta->push_back(osg::Vec2(dis_s, dis_t));
		}
		geom->setTexCoordArray(0, ta);
		geom->setNormalArray(na);
		geom->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);

		geode.replaceDrawable(ogeom, geom);
	}
}

//-------------------------------------------------------------------------------------------------



