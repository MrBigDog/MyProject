#ifndef GENERATE_WELL_OPERATOR_H
#define GENERATE_WELL_OPERATOR_H 1

#include "SlicePlane.h"
#include <osg/Array>
#include <osg/Group>

class WellGenerator
{
public:
	WellGenerator(bool isOnEarth) : _isOnEarth(isOnEarth) {}
	osg::Node* operator()(osg::Node* geoModel, double well_radius, const osg::Vec3d& pos, float hatHieght, unsigned int segmentNum = 16);
	static void generateSlicePlanes(double well_radius, const osg::Vec3d& pos, bool isOnEarth, SlicePLaneVector& out_slice_planes, unsigned int segmentNum = 16);

private:
	bool _isOnEarth;
};


#endif // generateWellOperator_h__
