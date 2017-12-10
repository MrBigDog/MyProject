#include "WellGenerator.h"
#include "SliceOperator.h"
#include "Cutter.h"
#include "GeneratorHelp.h"
#include "MeshConsolidator.h"

#include <osg/PolygonMode>
#include <osg/TriangleIndexFunctor>
#include <osgUtil/Optimizer>

#include <iostream>

namespace
{
	struct CutPlnae
	{
		CutPlnae(const osg::Plane& plane, const osg::Polytope& polytope)
			: _plane(plane)
			, _polytope(polytope)
		{}
		osg::Plane _plane;
		osg::Polytope _polytope;
	};
	typedef std::vector<CutPlnae> CutPlaneVector;
	typedef std::vector<CutPlnae>::const_iterator CutPlaneVectorConstIt;
}


//-----------------------------------------------------------------------------------
WellGenerator::WellGenerator(bool isOnEarth, unsigned int segmentNum)
	: _isOnEarth(isOnEarth)
	, _segmentNum(segmentNum)
{ }

osg::Node * WellGenerator::operator()(osg::Group * geoModel, double well_radius, const osg::Vec3d & pos)
{
	osg::Vec3d up = _isOnEarth ? normalize(pos) : osg::Vec3d(0, 0, 1);
	osg::Vec3d currentDir = _isOnEarth ? normalize(osg::Vec3d(0, 0, 1) ^ up) : osg::Vec3d(1, 0, 0);
	osg::Quat rot(osg::PI*2.0 / (double)_segmentNum, up);

	osg::ref_ptr<osg::Vec3dArray> va = new osg::Vec3dArray;
	for (unsigned int i = 0; i < _segmentNum; ++i)
	{
		osg::Vec3d currentPos = pos + currentDir*well_radius;
		va->push_back(currentPos);
		currentDir = normalize(rot*currentDir);
	}
	va->push_back(va->at(0));

	std::vector<CutPlnae> planeVector; planeVector.reserve(va->size() - 1);
	for (osg::Vec3dArray::const_iterator it = va->begin() + 1; it != va->end(); ++it)
	{
		osg::Vec3d planenormal = normalize(normalize(*it - pos) + normalize(*(it - 1) - pos));
		osg::Vec3d polytopePlaneNormal = normalize(*it - *(it - 1));

		osg::Polytope polytope;
		polytope.add(osg::Plane(polytopePlaneNormal, *(it - 1)));
		polytope.add(osg::Plane(-polytopePlaneNormal, *(it)));

		planeVector.push_back(CutPlnae(osg::Plane(planenormal, *it), polytope));
	}

	osg::ref_ptr<osg::Group> profileRoot = new osg::Group;
	for (CutPlaneVectorConstIt it = planeVector.begin(); it != planeVector.end(); ++it)
	{
		SliceOperator sliceer(_isOnEarth);
		//osg::ref_ptr<osg::Node> profileNode = sliceer.slice(geoModel, (*it)._plane, 0L);
		osg::ref_ptr<osg::Node> profileNode = sliceer.slice_closed(geoModel, (*it)._plane);
		if (profileNode)
		{
			Cutter cutter(it->_polytope);
			profileNode->accept(cutter);

			profileRoot->addChild(cutter.getResult());
		}
	}

	ColumnGeometryOptimizer wgo(pos, osg::Vec3d(0, 0, 1), osg::Vec3d(1, 0, 0), well_radius, _isOnEarth);
	profileRoot->accept(wgo);
	osg::ref_ptr<osg::Node> result = wgo.optimize();

	{//test
		osg::ref_ptr<osg::PolygonMode> pm = new osg::PolygonMode;
		pm->setMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::FILL);
		result->getOrCreateStateSet()->setAttributeAndModes(pm, osg::StateAttribute::OVERRIDE | osg::StateAttribute::ON);
		result->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	}

	return result.release();
}
