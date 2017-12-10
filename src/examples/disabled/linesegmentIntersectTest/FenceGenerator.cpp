#include "FenceGenerator.h"


namespace
{
	//---
	static void getStartAndDir(const osg::BoundingBox& bb, const osg::Vec3d& dir, osg::Vec3d& respos, osg::Vec3d& resdir, double& length)
	{
		osg::Vec3d leftBottom = osg::Vec3d(bb.xMin(), bb.yMin(), bb.zMin());
		osg::Vec3d leftTop = osg::Vec3d(bb.xMin(), bb.yMax(), bb.zMin());
		osg::Vec3d rightBottom = osg::Vec3d(bb.xMax(), bb.yMin(), bb.zMin());
		osg::Vec3d rightTop = osg::Vec3d(bb.xMax(), bb.yMax(), bb.zMin());

		osg::Vec3d xDir1 = rightTop - leftBottom;
		double xLength = xDir1.length();
		xDir1.normalize();

		osg::Vec3d xDir2 = rightBottom - leftTop;
		xDir2.normalize();

		osg::Vec3d d = dir; d.normalize();
		double xCos1 = osg::absolute(d*xDir1);
		double xCos2 = osg::absolute(d*xDir2);

		osg::Vec3d currentPos, currentDir;
		if (xCos1 > xCos2)
		{
			currentPos = leftTop;
			currentDir = xDir2;
		}
		else
		{
			currentPos = leftBottom;
			currentDir = xDir1;
		}
		respos = currentPos;
		resdir = currentDir;
		length = xLength;
	}

	static bool generateFixDisFencePlanes(const osg::BoundingBox & bb, const osg::Vec3d & slicedir, double deltaDis, std::vector<osg::Plane>& planes)
	{
		osg::Vec3d currentPos, currentDir;
		double xLength = 0.0;
		getStartAndDir(bb, slicedir, currentPos, currentDir, xLength);

		double deltaL = deltaDis;

		int sliceNum = xLength / deltaL;
		if (sliceNum < 1) return false;

		for (int i = 0; i < sliceNum; ++i)
		{
			osg::Vec3d currentPosOnXdir = currentPos + currentDir*(double)i*deltaL;

			osg::Vec3d planeNormal = slicedir^osg::Vec3d(0.0, 0.0, 1.0);
			planeNormal.normalize();

			planes.push_back(osg::Plane(planeNormal, currentPosOnXdir));
		}
		return true;
	}

	static bool generateFixNumFencePlanes(const osg::BoundingBox & bb, const osg::Vec3d & slicedir, int sliceNum, std::vector<osg::Plane>& planes)
	{
		if (sliceNum < 1) return false;

		osg::Vec3d currentPos, currentDir;
		double xLength = 0.0;
		getStartAndDir(bb, slicedir, currentPos, currentDir, xLength);
		double deltaL = xLength / (double)sliceNum;
		for (int i = 0; i < sliceNum; ++i)
		{
			osg::Vec3d currentPosOnXdir = currentPos + currentDir*(double)i*deltaL;

			osg::Vec3d planeNormal = slicedir^osg::Vec3d(0.0, 0.0, 1.0);
			planeNormal.normalize();

			planes.push_back(osg::Plane(planeNormal, currentPosOnXdir));
		}
		return true;
	}
}


void FenceGenerator::generateSlicePlanes(const osg::BoundingBox & bb, const osg::Vec3d & slicedir, unsigned int sliceNum, SlicePLaneVector & out_slice_planes)
{
	std::vector<osg::Plane> planes;
	if (!generateFixNumFencePlanes(bb, slicedir, sliceNum, planes))
	{
		return;
	}
	for (std::vector<osg::Plane>::const_iterator it = planes.begin(); it != planes.end(); ++it)
	{
		out_slice_planes.push_back(SlicePlane(*it, osg::Polytope()));
	}
}

void FenceGenerator::generateSlicePlanes(const osg::BoundingBox & bb, const osg::Vec3d & slicedir, double deltaDis, SlicePLaneVector & out_slice_planes)
{
	std::vector<osg::Plane> planes;
	if (!generateFixDisFencePlanes(bb, slicedir, deltaDis, planes))
	{
		return;
	}
	for (std::vector<osg::Plane>::const_iterator it = planes.begin(); it != planes.end(); ++it)
	{
		out_slice_planes.push_back(SlicePlane(*it, osg::Polytope()));
	}
}
