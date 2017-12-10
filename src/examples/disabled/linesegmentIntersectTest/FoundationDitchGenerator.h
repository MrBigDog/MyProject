#ifndef FOUNDATIONDITCHGENERATOR_H
#define FOUNDATIONDITCHGENERATOR_H 1
#include "SlicePlane.h"
#include <osg/Group>

//»ù¿Ó;
class FoundationDitchGenerator
{
public:
	FoundationDitchGenerator(bool isOnEarth = false) :_isOnEarth(isOnEarth) {}

	osg::Node* operator()(osg::Node* geologicalGroup, const osg::Vec3Array* va);
	static void generateSlicePlanes(const osg::Vec3Array* va, bool isOnEarth, SlicePLaneVector& out_slice_planes);

private:
	bool _isOnEarth;
};

#endif // FoundationDitchGenerator_h__
