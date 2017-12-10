#include "FoundationDitchGenerator.h"
#include "SliceOperator.h"
#include "Cutter.h"
#include "GeneratorHelp.h"
//#include "clipper.hpp"

#include <osg/PolygonMode>
#include <osg/TriangleFunctor>
#include <osgUtil/Tessellator>

namespace
{
	//struct PolygonCliper :public osg::NodeVisitor
	//{
	//	PolygonCliper(const osg::Vec3d& normal, double h, const osg::Vec3Array* va)
	//		: osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN)
	//		, _normal(normal)
	//		, _h(h)
	//	{
	//		_clipPath.resize(va->size());
	//		for (osg::Vec3Array::const_iterator it = va->begin(); it != va->end(); ++it)
	//		{
	//			_clipPath.push_back(ClipperLib::IntPoint((*it)[0], (*it)[1]));
	//		}
	//		_result = new osg::Geode;
	//	}

	//	void apply(osg::Geode& polygonGeode)
	//	{
	//		for (unsigned int i = 0; i < polygonGeode.getNumDrawables(); ++i)
	//		{
	//			osg::Geometry* geom = polygonGeode.getDrawable(i)->asGeometry();
	//			if (!geom) continue;
	//			osg::Vec3Array* va = dynamic_cast<osg::Vec3Array*>(geom->getVertexArray());
	//			if (!va) continue;

	//			osg::ref_ptr<osg::Vec4Array> ca = dynamic_cast<osg::Vec4Array*>(geom->getColorArray());
	//			osg::Vec4 color = (ca && !ca->empty()) ? ca->at(0) : osg::Vec4(1, 1, 1, 1);

	//			ClipperLib::Path path; path.resize(va->size());
	//			for (unsigned int j = 0; j < va->size(); ++j)
	//			{
	//				path[j].X = va->at(j)[0];
	//				path[j].Y = va->at(j)[1];
	//			}

	//			ClipperLib::Clipper cliper;
	//			cliper.AddPath(_clipPath, ClipperLib::ptClip, true);
	//			cliper.AddPath(path, ClipperLib::ptSubject, true);

	//			ClipperLib::Paths sol;
	//			cliper.Execute(ClipperLib::ctIntersection, sol, ClipperLib::pftEvenOdd, ClipperLib::pftEvenOdd);
	//			for (ClipperLib::Paths::const_iterator it = sol.begin(); it != sol.end(); ++it)
	//			{
	//				osg::ref_ptr<osg::Vec3Array> subva = new osg::Vec3Array;
	//				osg::ref_ptr<osg::Vec4Array> subca = new osg::Vec4Array;
	//				for (unsigned int k = 0; k < (*it).size(); ++k)
	//				{
	//					subva->push_back(_rot*osg::Vec3d((double)(*it)[k].X, (double)(*it)[k].Y, _h));
	//					subca->push_back(color);
	//				}
	//				osg::ref_ptr<osg::Geometry> subGeom = new osg::Geometry;
	//				subGeom->setStateSet(geom->getStateSet());
	//				subGeom->setUserData(geom->getUserData());
	//				subGeom->setVertexArray(subva);
	//				subGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP, 0, subva->size()));

	//				osgUtil::Tessellator tscx;
	//				tscx.setTessellationNormal(_normal);
	//				tscx.setTessellationType(osgUtil::Tessellator::TESS_TYPE_GEOMETRY);
	//				tscx.setBoundaryOnly(false);
	//				tscx.setWindingType(osgUtil::Tessellator::TESS_WINDING_ODD);
	//				tscx.retessellatePolygons(*subGeom);

	//				_result->addDrawable(subGeom);
	//			}
	//		}
	//	}

	//	osg::Node* getResult() { return _result.release(); }
	//private:
	//	ClipperLib::Path _clipPath;
	//	osg::Vec3d _normal;
	//	osg::Quat _rot;
	//	double _h;
	//	osg::ref_ptr<osg::Geode> _result;
	//};
}

osg::Node * FoundationDitchGenerator::operator()(osg::Node * geologicalGroup, const osg::Vec3Array * vva)
{
	if (vva->size() < 3 || !geologicalGroup) return 0L;
	//if (geologicalGroup->getNumChildren() < 1) return 0L;

	osg::ref_ptr<osg::Vec3Array> va = new osg::Vec3Array;
	va->reserve(vva->size() + 1);
	for (osg::Vec3Array::const_iterator it = vva->begin(); it != vva->end(); ++it)
	{
		va->push_back(*it);
	}
	removeRepeat<osg::Vec3Array>(va);
	va->push_back(vva->at(0));

	if (va->size() < 3) return 0L;

	SliceOperator slice(_isOnEarth);
	osg::ref_ptr<osg::Group> profileGroup = new osg::Group;
	for (osg::Vec3Array::const_iterator it = va->begin() + 1; it != va->end(); ++it)
	{
		osg::Vec3d dir = normalize(*it - *(it - 1));
		osg::Vec3d updir = _isOnEarth ? normalize(*(it - 1)) : osg::Vec3d(0, 0, 1);

		osg::Plane s(dir, *(it - 1));
		osg::Plane e(-dir, *it);
		osg::Plane p(*(it - 1), *it, *(it - 1) + updir);

		osg::Polytope pt;
		pt.add(s);
		pt.add(e);

		//osg::ref_ptr<osg::Node> profileNode = SLICE_CLOSED ? slice.slice/*_closed*/(geologicalGroup, p) :
		//	slice.slice(geologicalGroup, p);
		osg::ref_ptr<osg::Node> profileNode = slice.slice(geologicalGroup, p);
		if (!profileNode)
		{
			continue;
		}

		Cutter cut(pt);
		profileNode->accept(cut);

		PlaneGeometryOptimizer pgo(*it, p.getNormal(), dir);
		/*cut.getResult()*/profileNode->accept(pgo);

		profileGroup->addChild(/*pgo.optimize()*/profileNode);
	}

	osg::Vec3d polyCenter = getCenter<osg::Vec3Array, osg::Vec3d>(va);
	osg::Plane bottomPlane(_isOnEarth ? normalize(polyCenter) : osg::Vec3d(0, 0, 1), polyCenter);

	//bottom
	//osg::ref_ptr<osg::Node> bottomNode = SLICE_CLOSED ? slice.slice_closed(geologicalGroup, bottomPlane) :
	//	slice.slice(geologicalGroup, bottomPlane);
	osg::ref_ptr<osg::Node> bottomNode = slice.slice(geologicalGroup, bottomPlane);
	if (bottomNode)
	{
		profileGroup->addChild(bottomNode);
	}

	{//test
		osg::ref_ptr<osg::PolygonMode> pm = new osg::PolygonMode;
		pm->setMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::FILL);
		profileGroup->getOrCreateStateSet()->setAttributeAndModes(pm, osg::StateAttribute::OVERRIDE | osg::StateAttribute::ON);
		profileGroup->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	}

	return profileGroup.release();
}

void FoundationDitchGenerator::generateSlicePlanes(const osg::Vec3Array * vva, bool isOnEarth, SlicePLaneVector & out_slice_planes)
{
	if (!vva) return;
	if (vva->size() < 3) return;

	osg::ref_ptr<osg::Vec3Array> va = new osg::Vec3Array;
	va->reserve(vva->size() + 1);
	for (osg::Vec3Array::const_iterator it = vva->begin(); it != vva->end(); ++it)
	{
		va->push_back(*it);
	}
	removeRepeat<osg::Vec3Array>(va);
	va->push_back(vva->at(0));

	if (va->size() < 3) return;

	for (osg::Vec3Array::const_iterator it = va->begin() + 1; it != va->end(); ++it)
	{
		osg::Vec3d dir = normalize(*it - *(it - 1));
		osg::Vec3d updir = isOnEarth ? normalize(*(it - 1)) : osg::Vec3d(0, 0, 1);

		osg::Plane s(dir, *(it - 1));
		osg::Plane e(-dir, *it);
		osg::Plane p(*(it - 1), *it, *(it - 1) + updir);

		osg::Polytope pt;
		pt.add(s);
		pt.add(e);
		out_slice_planes.push_back(SlicePlane(p, pt));
	}

	osg::Vec3d polyCenter = getCenter<osg::Vec3Array, osg::Vec3d>(va);
	osg::Plane bottomPlane(isOnEarth ? normalize(polyCenter) : osg::Vec3d(0, 0, 1), polyCenter);
	out_slice_planes.push_back(SlicePlane(bottomPlane, osg::Polytope()));
}
