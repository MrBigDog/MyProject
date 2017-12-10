#include "GeneratorHelp.h"
#include "MeshConsolidator.h"

void ColumnGeometryOptimizer::apply(osg::Geode& geode)
{
	for (unsigned int i = 0; i < geode.getNumDrawables(); ++i)
	{
		osg::Geometry* geom = dynamic_cast<osg::Geometry*>(geode.getDrawable(i));
		osg::Drawable* d = geode.getDrawable(i);
		GeologicalMaterial* duserdata = dynamic_cast<GeologicalMaterial*>(d->getUserData());
		if (!duserdata) continue;

		bool ishave = false;
		for (GeodeVector::const_iterator it = _geodeVector.begin(); it != _geodeVector.end(); ++it)
		{
			GeologicalMaterial* guserdata = dynamic_cast<GeologicalMaterial*>((*it)->getUserData());
			if (!guserdata)continue;
			if (*guserdata == *duserdata)
			{
				(*it)->addDrawable(d);
				ishave = true;
				break;
			}
		}
		if (!ishave)
		{
			osg::ref_ptr<osg::Geode> geode = new osg::Geode;
			geode->setUserData(d->getUserData());
			geode->addDrawable(d);
			_geodeVector.push_back(geode);
		}
	}
}

osg::Node * ColumnGeometryOptimizer::optimize()
{
	osg::Plane plane(_dir, _wellCenter);
	osg::ref_ptr<osg::Geode> newGeode = new osg::Geode;
	for (GeodeVector::const_iterator it = _geodeVector.begin(); it != _geodeVector.end(); ++it)
	{
		//合并Geometry
		MeshConsolidator mc; mc.run(**it);

		//计算纹理坐标;
		for (unsigned int i = 0; i < (*it)->getNumDrawables(); ++i)
		{
			osg::Geometry* geom = (*it)->getDrawable(i)->asGeometry();
			if (!geom) continue;

			osg::Vec3Array* va = dynamic_cast<osg::Vec3Array*>(geom->getVertexArray());
			if (!va) continue;

			osg::ref_ptr<osg::Vec2Array> ta = new osg::Vec2Array;
			ta->reserve(va->size());
			osg::ref_ptr<osg::Vec3Array> na = new osg::Vec3Array;
			na->reserve(va->size());
			for (osg::Vec3Array::const_iterator vait = va->begin(); vait != va->end(); ++vait)
			{
				osg::Vec3d nc = _wellCenter;
				double dis1 = plane.distance(nc);
				double dis2 = plane.distance(*vait);
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
			geom->setNormalArray(na, osg::Array::BIND_PER_VERTEX);
			newGeode->addDrawable(geom);
		}
	}
	return newGeode.release();
}

void PlaneGeometryOptimizer::apply(osg::Geode & geode)
{
	for (unsigned int i = 0; i < geode.getNumDrawables(); ++i)
	{
		osg::Geometry* geom = dynamic_cast<osg::Geometry*>(geode.getDrawable(i));
		osg::Drawable* d = geode.getDrawable(i);
		GeologicalMaterial* duserdata = dynamic_cast<GeologicalMaterial*>(d->getUserData());
		if (!duserdata) continue;

		bool ishave = false;
		for (GeodeVector::const_iterator it = _geodeVector.begin(); it != _geodeVector.end(); ++it)
		{
			GeologicalMaterial* guserdata = dynamic_cast<GeologicalMaterial*>((*it)->getUserData());
			if (!guserdata)continue;
			if (*guserdata == *duserdata)
			{
				(*it)->addDrawable(d);
				ishave = true;
				break;
			}
		}
		if (!ishave)
		{
			osg::ref_ptr<osg::Geode> geode = new osg::Geode;
			geode->setUserData(d->getUserData());
			geode->addDrawable(d);
			_geodeVector.push_back(geode);
		}
	}
}

osg::Node * PlaneGeometryOptimizer::optimize()
{
	osg::Plane plane_t(_side, _posOnPlane);
	osg::Plane plane_s(_side^_normal, _posOnPlane);
	osg::ref_ptr<osg::Geode> newGeode = new osg::Geode;
	for (GeodeVector::const_iterator it = _geodeVector.begin(); it != _geodeVector.end(); ++it)
	{
		//合并Geometry
		MeshConsolidator mc; mc.run(**it);

		//计算纹理坐标;
		for (unsigned int i = 0; i < (*it)->getNumDrawables(); ++i)
		{
			osg::Geometry* geom = (*it)->getDrawable(i)->asGeometry();
			if (!geom) continue;

			osg::Vec3Array* va = dynamic_cast<osg::Vec3Array*>(geom->getVertexArray());
			if (!va) continue;

			osg::ref_ptr<osg::Vec2Array> ta = new osg::Vec2Array;
			ta->reserve(va->size());
			osg::ref_ptr<osg::Vec3Array> na = new osg::Vec3Array;
			na->reserve(va->size());
			for (osg::Vec3Array::const_iterator vait = va->begin(); vait != va->end(); ++vait)
			{
				double dis_s = plane_s.distance(*vait);
				double dis_t = plane_t.distance(*vait);

				ta->push_back(osg::Vec2(dis_s, dis_t));
				na->push_back(osg::Vec3(_normal));
			}
			geom->setTexCoordArray(0, ta);
			geom->setNormalArray(na, osg::Array::BIND_PER_VERTEX);
			newGeode->addDrawable(geom);
		}
	}
	return newGeode.release();
}
