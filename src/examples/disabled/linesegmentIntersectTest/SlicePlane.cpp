#include "SlicePlane.h"
#include "Cutter.h"
#include "SliceOperator.h"

osg::Node * SlicePlaneNodeGenerator::operator()(osg::Node * single_hull_node, bool isOnEarth, const SlicePLaneVector & slice_planes)
{
	if (!single_hull_node) return 0L;

	osg::ref_ptr<osg::Group> geologicalGroup = new osg::Group;
	geologicalGroup->addChild(single_hull_node);

	osg::ref_ptr<osg::Group> root = new osg::Group;
	for (SlicePlaneVectorConstIt it = slice_planes.begin(); it != slice_planes.end(); ++it)
	{
		const osg::Plane& slicePlane = (*it)._plane;
		const osg::Polytope& polytope = (*it)._polytope;

		SliceOperator slicer(isOnEarth);
		//osg::ref_ptr<osg::Node> profileNode = slicer.slice_closed(geologicalGroup, slicePlane);
		osg::ref_ptr<osg::Node> profileNode = slicer.slice(geologicalGroup, slicePlane);
		if (!profileNode)
		{
			continue;
		}

		if (polytope.getPlaneList().size() > 0)
		{
			Cutter cutter(polytope);
			profileNode->accept(cutter);

			//osg::ref_ptr<osg::Node> cutResult = cutter.getResult();
			if (/*cutResult*/profileNode)
			{
				root->addChild(profileNode);
			}
		}
		root->addChild(profileNode);
	}
	return root.release();
}
