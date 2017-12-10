/************************************************************************/
/*      From osgEarth                                                   */
/************************************************************************/
#ifndef MESHCONSOLIDATOR_H
#define MESHCONSOLIDATOR_H 1

#include <osg/Geode>
#include <osg/Geometry>

class  MeshConsolidator
{
public:
	static void convertToTriangles(osg::Geometry& geom, bool force = false);
	static void run(osg::Geode& geode);
};

#endif // MESHCONSOLIDATOR_H
