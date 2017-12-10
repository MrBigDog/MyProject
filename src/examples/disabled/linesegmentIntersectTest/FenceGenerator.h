#ifndef FENCEGENERATOR_H
#define FENCEGENERATOR_H 1

#include "SlicePlane.h"

class FenceGenerator
{
public:
	static void generateSlicePlanes(const osg::BoundingBox & bb, const osg::Vec3d& slicedir, unsigned int sliceNum, SlicePLaneVector& out_slice_planes);
	static void generateSlicePlanes(const osg::BoundingBox & bb, const osg::Vec3d& slicedir, double deltaDis, SlicePLaneVector& out_slice_planes);
};

#endif // FenceGenerator_h__
