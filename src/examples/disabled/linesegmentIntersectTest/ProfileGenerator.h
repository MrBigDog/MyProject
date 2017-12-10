#ifndef PROFILEGENERATOR_H
#define PROFILEGENERATOR_H 1

#include "SlicePlane.h"
#include <osg/Group>
#include <osg/Plane>

//∂‡«–/µ•«–;
class ProfileGenerator
{
public:
	ProfileGenerator(bool isOnEarth = false) :_isOnEarth(isOnEarth) {}
	osg::Node* operator()(osg::Group* geologicalGroup, const osg::Plane& plane);
	osg::Node* operator()(osg::Group* geologicalGroup, const PlaneVec& planes);

	//
	static SlicePlane generateSlicePlanes(const osg::Plane& plane);
	static void generateSlicePlanes(const PlaneVec& planes, SlicePLaneVector& out_sliceplanes);

private:
	bool _isOnEarth;
};

//strip;
class ProfileStripGenerator
{
public:
	ProfileStripGenerator(bool isOnEarth = false) :_isOnEarth(isOnEarth) {}
	osg::Node* operator()(osg::Group* geologicalGroup, const osg::Vec3Array* va, bool isLoop);
	static void generateSlicePlanes(const osg::Vec3Array* va, bool isLoop, bool isOnEarth, SlicePLaneVector& out_slice_planes);

private:
	bool _isOnEarth;
};

#endif // PROFILEGENERATOR_H
