#include "WellGenerator.h"
#include "SliceOperator.h"
#include "Cutter.h"
#include "GeneratorHelp.h"
#include "MeshConsolidator.h"
#include "RayIntersector.h"
#include "LineSegmentIntersector.h"

#include <osg/PolygonMode>
#include <osg/ComputeBoundsVisitor>
#include <osg/TriangleIndexFunctor>
#include <osgUtil/Optimizer>

#include <iostream>

namespace
{
	static osg::Node* generateHat(const osg::Vec3Array* va, const osg::Vec3d& up, float hatHeight)
	{
		osg::Vec3d center = getCenter<osg::Vec3Array, osg::Vec3d>(va);

		osg::Vec3d upvec = normalize(up)*hatHeight;
		osg::ref_ptr<osg::Vec3Array> newva = new osg::Vec3Array;
		newva->reserve(va->size() * 2);
		osg::ref_ptr<osg::Vec3Array> na = new osg::Vec3Array;
		osg::ref_ptr<osg::Vec4Array> ca = new osg::Vec4Array;
		osg::ref_ptr<osg::DrawElementsUInt> polygonele = new osg::DrawElementsUInt(osg::PrimitiveSet::POLYGON);
		for (osg::Vec3Array::const_iterator it = va->begin(); it != va->end(); ++it)
		{
			newva->push_back(*it);
			newva->push_back(*it + upvec);
			polygonele->push_back(newva->size() - 1);

			osg::Vec3d nor = *it - center;
			na->push_back(nor);
			na->push_back(nor);

			ca->push_back(osg::Vec4(1, 1, 1, 1));
			ca->push_back(osg::Vec4(1, 1, 1, 1));
		}
		osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
		geom->setVertexArray(newva);
		geom->setNormalArray(na, osg::Array::BIND_PER_VERTEX);
		geom->setColorArray(ca, osg::Array::BIND_PER_VERTEX);
		geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLE_STRIP, 0, newva->size()));
		geom->addPrimitiveSet(polygonele);

		osg::ref_ptr<osg::Geode> geode = new osg::Geode;
		geode->addDrawable(geom);
		return geode.release();
	}

	osg::Node* generateUnitWell(const osg::Vec3d& center, double radius, double length, const osg::Vec4& color, osg::Texture2D* tex, unsigned int segNum = 17)
	{
		double half_length = length*0.5;
		double delta_angle = osg::PI*2.0 / (double)(segNum - 1);
		double delta_s = delta_angle*radius;

		osg::Vec3d bottomCenter = center - osg::Vec3d(0, 0, 1)*half_length;
		osg::Quat rot(delta_angle, osg::Vec3d(0, 0, 1));
		osg::Vec3d currentDir = osg::Vec3d(1, 0, 0);

		double sumS = 0.0;

		osg::ref_ptr<osg::Vec3Array> va = new osg::Vec3Array;
		osg::ref_ptr<osg::Vec3Array> na = new osg::Vec3Array;
		osg::ref_ptr<osg::Vec2Array> ta = new osg::Vec2Array;
		for (unsigned int i = 0; i < segNum; ++i)
		{
			currentDir = rot*currentDir;

			osg::Vec3d bottomPos = bottomCenter + currentDir*radius;
			osg::Vec3d topPos = bottomPos + osg::Vec3d(0, 0, 1)*length;
			va->push_back(bottomPos);
			va->push_back(topPos);

			osg::Vec3d normal = normalize(currentDir);
			na->push_back(normal);
			na->push_back(normal);

			sumS += delta_s;
			ta->push_back(osg::Vec2(0.0, sumS));
			ta->push_back(osg::Vec2(length, sumS));
		}

		osg::ref_ptr<osg::Vec4Array> ca = new osg::Vec4Array;
		ca->push_back(color);

		osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
		geom->setVertexArray(va);
		geom->setTexCoordArray(0, ta);
		geom->setNormalArray(na, osg::Array::BIND_PER_VERTEX);
		geom->setColorArray(ca, osg::Array::BIND_OVERALL);
		geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLE_STRIP, 0, va->size()));
		if (tex)
		{
			geom->getOrCreateStateSet()->setTextureAttributeAndModes(0, tex);
		}
		if (!geom)
		{
			return 0L;
		}
		osg::ref_ptr<osg::Geode> geode = new osg::Geode;
		geode->addDrawable(geom);

		return geode.release();
	}

	void getGeometryMaterial(osg::Geometry* geom, osg::Vec4& color = osg::Vec4(1, 1, 1, 1), osg::Texture2D* texture = 0L)
	{
		if (!geom) return;
		osg::Vec4Array* ca = dynamic_cast<osg::Vec4Array*>(geom->getColorArray());
		if (ca && !ca->empty())
		{
			color = ca->at(0);
		}
		osg::StateSet* ss = geom->getStateSet();
		if (ss)
		{
			osg::StateSet::TextureAttributeList& a = ss->getTextureAttributeList();
			for (osg::StateSet::TextureAttributeList::iterator i = a.begin(); i != a.end(); ++i)
			{
				osg::StateSet::AttributeList& b = *i;
				for (osg::StateSet::AttributeList::iterator j = b.begin(); j != b.end(); ++j)
				{
					osg::StateAttribute* sa = j->second.first.get();
					if (sa)
					{
						osg::Texture* tex = dynamic_cast<osg::Texture*>(sa);
						if (tex)
						{
							texture = dynamic_cast<osg::Texture2D*>(tex);
						}
					}
				}
			}
		}
	}

	class GeometryVisitor :public osg::NodeVisitor
	{
	public:
		GeometryVisitor(const osg::Vec3d& pos, double wellRadius, double height) :osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN)
			, _wellRadius(wellRadius)
			, _height(height)
			, _pos(pos)
			, _wellRoot(0L)
			, _up(osg::Vec3d(0, 0, 1))
		{}

		void apply(osg::Geode& geode)
		{
			for (unsigned int i = 0; i < geode.getNumDrawables(); ++i)
			{
				osg::Geometry* geom = geode.getDrawable(i)->asGeometry();
				if (!geom) continue;

				const osg::MatrixList& ml = geom->getWorldMatrices();
				osg::Matrixd matrix = getMatrix(ml);
				osg::Matrixd inversematrix = osg::Matrixd::inverse(matrix);

				osg::Vec3d localPos = _pos*inversematrix;

				osg::ref_ptr<LineSegmentIntersector> lsi = new LineSegmentIntersector(localPos, localPos - _up*_height);
				lsi->setPrecisionHint(osgUtil::Intersector::USE_DOUBLE_CALCULATIONS);

				osgUtil::IntersectionVisitor intersectVisitor(lsi.get());
				geom->accept(intersectVisitor);
				if (!lsi->containsIntersections()) continue;
				LineSegmentIntersector::Intersections& intersections = lsi->getIntersections();

				osg::Vec3d topHitPos = intersections.begin()->getWorldIntersectPoint();
				osg::Vec3d bottomHitPos = intersections.rbegin()->getWorldIntersectPoint();

				osg::Vec3d wellCenter = (topHitPos + bottomHitPos)*0.5;
				double wellLength = (topHitPos - bottomHitPos).length();

				osg::Vec4 wellColor;
				osg::ref_ptr<osg::Texture2D> texture;
				getGeometryMaterial(geom, wellColor, texture);

				osg::ref_ptr<osg::Node> well = generateUnitWell(wellCenter, _wellRadius, wellLength, wellColor, texture);
				if (!well.valid()) continue;

				osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
				mt->addChild(well);
				mt->setMatrix(matrix);

				if (!_wellRoot) _wellRoot = new osg::Group;
				_wellRoot->addChild(mt);
			}
		}

		osg::Node* getWell() { return _wellRoot.release(); }

	private:
		osg::Matrixd getMatrix(const osg::MatrixList& ml)
		{
			osg::Matrixd matrix; matrix.makeIdentity();
			for (osg::MatrixList::const_iterator it = ml.begin(); it != ml.end(); ++it)
			{
				matrix *= *it;
			}
			return matrix;
		}

	private:
		double _wellRadius;
		double _height;
		osg::Vec3d _up;
		osg::Vec3d _pos;
		osg::ref_ptr<osg::Group> _wellRoot;
	};
}

//-----------------------------------------------------------------------------------
osg::Node * WellGenerator::operator()(osg::Node * geoModel, double wellRadius, const osg::Vec3d & pos, float hatHieght, unsigned int segmentNum)
{
	osg::ComputeBoundsVisitor cbv;
	geoModel->accept(cbv);
	osg::BoundingBox bb = cbv.getBoundingBox();

	double ll = (bb.zMax() - bb.zMin())*100.0;

	GeometryVisitor gv(pos, wellRadius, ll);
	geoModel->accept(gv);
	return gv.getWell();
}

void WellGenerator::generateSlicePlanes(double well_radius, const osg::Vec3d & pos, bool isOnEarth, SlicePLaneVector & out_slice_planes, unsigned int segmentNum)
{
	osg::Vec3d up = isOnEarth ? normalize(pos) : osg::Vec3d(0, 0, 1);
	osg::Vec3d currentDir = isOnEarth ? normalize(osg::Vec3d(0, 0, 1) ^ up) : osg::Vec3d(1, 0, 0);
	osg::Quat rot(osg::PI*2.0 / (double)segmentNum, up);

	osg::ref_ptr<osg::Vec3dArray> va = new osg::Vec3dArray;
	for (unsigned int i = 0; i < segmentNum; ++i)
	{
		osg::Vec3d currentPos = pos + currentDir*well_radius;
		va->push_back(currentPos);
		currentDir = normalize(rot*currentDir);
	}
	va->push_back(va->at(0));

	SlicePLaneVector planeVector; planeVector.reserve(va->size() - 1);
	for (osg::Vec3dArray::const_iterator it = va->begin() + 1; it != va->end(); ++it)
	{
		osg::Vec3d planenormal = normalize(normalize(*it - pos) + normalize(*(it - 1) - pos));
		osg::Vec3d polytopePlaneNormal = normalize(*it - *(it - 1));

		osg::Polytope polytope;
		polytope.add(osg::Plane(polytopePlaneNormal, *(it - 1)));
		polytope.add(osg::Plane(-polytopePlaneNormal, *(it)));

		planeVector.push_back(SlicePlane(osg::Plane(planenormal, *it), polytope));
	}
}
