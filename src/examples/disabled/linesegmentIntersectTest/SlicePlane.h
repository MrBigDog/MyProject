#ifndef SLICEPLANE_H
#define SLICEPLANE_H 1

#include <osg/Plane>
#include <osg/Polytope>
#include <vector>

typedef std::vector<osg::Plane> PlaneVec;

struct SlicePlane
{
	SlicePlane(const osg::Plane& plane, const osg::Polytope& polytope)
		:_plane(plane), _polytope(polytope)
	{}

	osg::Plane _plane;
	osg::Polytope _polytope;
};
typedef std::vector<SlicePlane> SlicePLaneVector;
typedef std::vector<SlicePlane>::const_iterator SlicePlaneVectorConstIt;

//////////////////////////////////////////////////////////////////////////
class SlicePlaneNodeGenerator :public osg::Referenced
{
public:
	SlicePlaneNodeGenerator() {}
	osg::Node* operator()(osg::Node* single_hull_node, bool isOnEarth, const SlicePLaneVector& slice_planes);
};

#endif // SlicePlane_h__