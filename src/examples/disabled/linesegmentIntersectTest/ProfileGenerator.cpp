#include "ProfileGenerator.h"
#include "SliceOperator.h"
#include "GeneratorHelp.h"
#include "Cutter.h"

osg::Node * ProfileGenerator::operator()(osg::Group * geologicalGroup, const osg::Plane & plane)
{
	if (!geologicalGroup) return 0L;
	if (geologicalGroup->getNumChildren() < 1) return 0L;

	SliceOperator slicer(_isOnEarth);
	//return SLICE_CLOSED ? slicer.slice_closed(geologicalGroup, plane) : slicer.slice(geologicalGroup, plane);
	return  slicer.slice(geologicalGroup, plane);
}

osg::Node * ProfileGenerator::operator()(osg::Group * geologicalGroup, const PlaneVec & planes)
{
	if (planes.empty() || !geologicalGroup) return 0L;
	if (geologicalGroup->getNumChildren() < 1) return 0L;

	SliceOperator slicer(_isOnEarth);
	osg::ref_ptr<osg::Group> profilegroup = new osg::Group;
	for (std::vector<osg::Plane>::const_iterator it = planes.begin(); it != planes.end(); ++it)
	{
		//osg::ref_ptr<osg::Node> profilenode = SLICE_CLOSED ? slicer.slice_closed(geologicalGroup, *it)
		//	: slicer.slice(geologicalGroup, *it);
		osg::ref_ptr<osg::Node> profilenode = slicer.slice(geologicalGroup, *it);

		if (profilenode) profilegroup->addChild(profilenode);
	}
	return profilegroup.release();
}

SlicePlane ProfileGenerator::generateSlicePlanes(const osg::Plane & plane)
{
	return SlicePlane(plane, osg::Polytope());
}

void ProfileGenerator::generateSlicePlanes(const PlaneVec & planes, SlicePLaneVector & out_sliceplanes)
{
	for (PlaneVec::const_iterator it = planes.begin(); it != planes.end(); ++it)
	{
		out_sliceplanes.push_back(SlicePlane(*it, osg::Polytope()));
	}
}

osg::Node * ProfileStripGenerator::operator()(osg::Group * geologicalGroup, const osg::Vec3Array * vva, bool isLoop)
{
	if (vva->size() < 2 || !geologicalGroup) return 0L;
	if (geologicalGroup->getNumChildren() < 1) return 0L;

	osg::ref_ptr<osg::Vec3Array> va = new osg::Vec3Array; va->reserve(vva->size());
	for (osg::Vec3Array::const_iterator it = vva->begin(); it != vva->end(); ++it)
	{
		va->push_back(*it);
	}
	removeRepeat<osg::Vec3Array>(va);
	if (va->size() < 2) return 0L;
	if (isLoop) va->push_back(vva->at(0));

	SliceOperator slicer(_isOnEarth);

	osg::ref_ptr<osg::Group> profileGroup = new osg::Group;
	for (unsigned int i = 0; i < va->size() - 1; ++i)
	{
		osg::Vec3d dir = normalize(va->at(i + 1) - va->at(i));

		osg::Plane s(dir, va->at(i));
		osg::Plane e(-dir, va->at(i + 1));
		osg::Plane p(va->at(i), va->at(i + 1), va->at(i) + osg::Vec3d(0, 0, 1));

		osg::Polytope pt;
		pt.add(s);
		pt.add(e);

		//osg::ref_ptr<osg::Node> profileNode = SLICE_CLOSED ?
		//	slicer.slice_closed(geologicalGroup, p) : slicer.slice(geologicalGroup, p);
		osg::ref_ptr<osg::Node> profileNode = slicer.slice(geologicalGroup, p);

		if (!profileNode) continue;

		Cutter cut(pt);
		profileNode->accept(cut);

		PlaneGeometryOptimizer pgo(va->at(0), p.getNormal(), dir);
		/*cut.getResult()*/profileNode->accept(pgo);

		profileGroup->addChild(/*pgo.optimize()*/profileNode);
	}
	return profileGroup.release();
}

void ProfileStripGenerator::generateSlicePlanes(const osg::Vec3Array * vva, bool isLoop, bool isOnEarth, SlicePLaneVector & out_slice_planes)
{
	if (!vva) return;
	if (vva->size() < 2) return;
	osg::ref_ptr<osg::Vec3Array> va = new osg::Vec3Array; va->reserve(vva->size());
	for (osg::Vec3Array::const_iterator it = vva->begin(); it != vva->end(); ++it)
	{
		va->push_back(*it);
	}
	removeRepeat<osg::Vec3Array>(va);
	if (va->size() < 2) return;
	if (isLoop) va->push_back(vva->at(0));

	out_slice_planes.reserve(va->size());
	for (unsigned int i = 0; i < va->size() - 1; ++i)
	{
		osg::Vec3d dir = normalize(va->at(i + 1) - va->at(i));

		osg::Plane s(dir, va->at(i));
		osg::Plane e(-dir, va->at(i + 1));
		osg::Plane p(va->at(i), va->at(i + 1), va->at(i) + osg::Vec3d(0, 0, 1));

		osg::Polytope pt;
		pt.add(s);
		pt.add(e);

		SlicePlane sliceplane(p, pt);
		out_slice_planes.push_back(sliceplane);
	}
}
