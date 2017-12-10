#include "AssistPlaneGenerator.h"
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/LineWidth>
#include <osgUtil/PlaneIntersector>

namespace
{
	static osg::Vec3Array* getIntersectPlaneWithBox(const osg::Plane& plane, const osg::BoundingBox& bb)
	{
		osg::ref_ptr<osg::Vec3Array> va = new osg::Vec3Array;
		va->push_back(osg::Vec3d(bb.xMin(), bb.yMin(), bb.zMin()));
		va->push_back(osg::Vec3d(bb.xMax(), bb.yMin(), bb.zMin()));
		va->push_back(osg::Vec3d(bb.xMax(), bb.yMax(), bb.zMin()));
		va->push_back(osg::Vec3d(bb.xMin(), bb.yMax(), bb.zMin()));

		va->push_back(osg::Vec3d(bb.xMin(), bb.yMin(), bb.zMax()));
		va->push_back(osg::Vec3d(bb.xMax(), bb.yMin(), bb.zMax()));
		va->push_back(osg::Vec3d(bb.xMax(), bb.yMax(), bb.zMax()));
		va->push_back(osg::Vec3d(bb.xMin(), bb.yMax(), bb.zMax()));

		osg::ref_ptr<osg::Geometry> pGeometry = new osg::Geometry;
		pGeometry->setVertexArray(va);
		osg::DrawElementsUShort * indexarray = new osg::DrawElementsUShort(osg::PrimitiveSet::QUADS);
		//底面
		indexarray->push_back(0); indexarray->push_back(3);
		indexarray->push_back(2); indexarray->push_back(1);
		//右面
		indexarray->push_back(6); indexarray->push_back(5);
		indexarray->push_back(1); indexarray->push_back(2);
		//上面
		indexarray->push_back(4); indexarray->push_back(5);
		indexarray->push_back(6); indexarray->push_back(7);
		//左面
		indexarray->push_back(3); indexarray->push_back(0);
		indexarray->push_back(4); indexarray->push_back(7);
		//前面
		indexarray->push_back(0); indexarray->push_back(1);
		indexarray->push_back(5); indexarray->push_back(4);
		//后面
		indexarray->push_back(2); indexarray->push_back(3);
		indexarray->push_back(7); indexarray->push_back(6);
		pGeometry->addPrimitiveSet(indexarray);

		osg::ref_ptr<osgUtil::PlaneIntersector> intersector = new osgUtil::PlaneIntersector(plane);
		intersector->setRecordHeightsAsAttributes(false);
		osgUtil::IntersectionVisitor intersectionVisitor;
		intersectionVisitor.reset();
		intersectionVisitor.setIntersector(intersector.get());
		pGeometry->accept(intersectionVisitor);

		osgUtil::PlaneIntersector::Intersections& intersections = intersector->getIntersections();
		if (intersections.empty())
		{
			return NULL;
		}
		typedef osgUtil::PlaneIntersector::Intersection::Polyline Polyline;
		osgUtil::PlaneIntersector::Intersections::iterator itr;
		osg::ref_ptr<osg::Vec3Array> vva = new osg::Vec3Array;
		std::set<osg::Vec3>  vvarray;
		for (itr = intersections.begin(); itr != intersections.end(); ++itr)
		{
			osgUtil::PlaneIntersector::Intersection& intersection = *itr;
			if (intersection.matrix.valid())
			{
				for (auto pitr = intersection.polyline.begin(); pitr != intersection.polyline.end(); ++pitr)
				{
					*pitr = (*pitr) * (*intersection.matrix);
					if (/*m_boundintBox.contains(*pitr) &&*/ vvarray.count(*pitr) == 0)//在包围盒内且无重复点
					{
						vvarray.insert(*pitr);
						vva->push_back(*pitr);
					}
				}
				intersection.matrix = 0;
			}
			else
			{
				for (auto pitr = intersection.polyline.begin(); pitr != intersection.polyline.end(); ++pitr)
				{
					if (/*m_boundintBox.contains(*pitr) &&*/ vvarray.count(*pitr) == 0)
					{
						vva->push_back(*pitr);
						vvarray.insert(*pitr);
					}
				}
			}
		}
		osg::ref_ptr<osg::Vec3Array > newArray = new osg::Vec3Array;
		float epslion = 9.999e-4f;
		if (!vva->empty())//删除非边界点
		{
			for (unsigned int k = 0; k < vva->size(); k++)
			{
				osg::Vec3 sp, ep, dir1, dir2;
				int flag = 0;
				for (unsigned int i = 0; i < indexarray->size(); i++)
				{
					flag++;
					sp = va->at(indexarray->at(i));
					if (flag % 4 == 0 || i == indexarray->size() - 1)
					{
						ep = va->at(indexarray->at(i - 3));
						flag = 0;
					}
					else
					{
						ep = va->at(indexarray->at(i + 1));
					}
					dir1 = sp - vva->at(k);		dir1.normalize();
					dir2 = ep - vva->at(k);		dir2.normalize();
					float res = dir1 * dir2;
					if (osg::equivalent(res, 1.0f, epslion) || osg::equivalent(res, -1.0f, epslion))//如果平行，在边界上
					{
						newArray->push_back(vva->at(k));
						break;
					}
				}
			}
		}
		return newArray.release();
	}

	static osg::Geometry * createBoundary(osg::Vec3Array* vArray)
	{
		osg::Geometry *pGeometry = new osg::Geometry();
		osg::Vec4Array * ca = new osg::Vec4Array;
		pGeometry->setVertexArray(vArray);
		ca->push_back(osg::Vec4(1, 0, 0, 1));
		pGeometry->setColorArray(ca);
		pGeometry->setColorBinding(osg::Geometry::AttributeBinding::BIND_OVERALL);
		osg::DrawArrays *pLine = new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP, 0, vArray->size());
		pGeometry->addPrimitiveSet(pLine);
		osg::LineWidth *pLineWidth = new osg::LineWidth();
		pLineWidth->setWidth(2.0);
		pGeometry->getOrCreateStateSet()->setAttributeAndModes(pLineWidth, osg::StateAttribute::ON);
		return pGeometry;
	}

	static osg::Geometry * createClipPlane(osg::Vec3Array* vArray)
	{
		osg::Geometry *pGeometry = new osg::Geometry();
		osg::Vec4Array * ca = new osg::Vec4Array;
		pGeometry->setVertexArray(vArray);
		ca->push_back(osg::Vec4(1, 1, 0, 1));
		pGeometry->setColorArray(ca);
		pGeometry->setColorBinding(osg::Geometry::AttributeBinding::BIND_OVERALL);
		osg::DrawArrays *pTRIANGLE_FAN = new osg::DrawArrays(osg::PrimitiveSet::TRIANGLE_FAN, 0, vArray->size());
		pGeometry->addPrimitiveSet(pTRIANGLE_FAN);
		pGeometry->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);
		pGeometry->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
		/*osg::ref_ptr<osg::PolygonMode> polymode = new osg::PolygonMode;
		polymode->setMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE);
		pGeometry->getOrCreateStateSet()->setAttributeAndModes(polymode, osg::StateAttribute::ON);*/
		return pGeometry;
	}

	static osg::Geode* createGWClipPlane(const osg::Plane& plane, const osg::BoundingBox& bb)
	{
		osg::ref_ptr<osg::Vec3Array> pVarray = getIntersectPlaneWithBox(plane, bb);
		if (pVarray.valid() && !pVarray->empty())
		{
			osg::Geometry * pGeometryB = NULL;
			osg::Geometry * pGeometryP = NULL;
			osg::ref_ptr<osg::Geode > pGeode = new osg::Geode;
			pGeometryB = createBoundary(pVarray.get());
			if (pGeometryB)
			{
				pGeode->addDrawable(pGeometryB);
			}
			pGeometryP = createClipPlane(pVarray.get());
			if (pGeometryP)
			{
				pGeode->addDrawable(pGeometryP);
				if (pGeode->getNumDrawables() != 0)
				{
					return pGeode.release();
				}
			}
		}
		return  NULL;
	}

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

//-------------------------------------------------------------------------------------------------------------
osg::Node * AssistPlaneGenerator::operator()(const osg::BoundingBox & box, const osg::Plane & plane)
{
	return createGWClipPlane(plane, box);
}

osg::Node * AssistPlaneGenerator::operator()(const osg::BoundingBox & box, const std::vector<osg::Plane> planes)
{
	if (planes.empty()) return 0L;
	osg::ref_ptr<osg::Group> root = new osg::Group;
	for (std::vector<osg::Plane>::const_iterator it = planes.begin(); it != planes.end(); ++it)
	{
		osg::ref_ptr<osg::Node> profilenode = createGWClipPlane(*it, box);
		if (profilenode) root->addChild(profilenode);
	}
	return root.release();
}

//------------------------------------
osg::Node* FencePlaneGenerator::operator()(const osg::BoundingBox & bb, const osg::Vec3d & slicedir, double deltaDis)
{
	std::vector<osg::Plane> planes;
	if (!generateFixDisFencePlanes(bb, slicedir, deltaDis, planes))
	{
		return 0L;
	}
	AssistPlaneGenerator apg(_isOnEarth);
	return apg(bb, planes);
}

osg::Node* FencePlaneGenerator::operator()(const osg::BoundingBox & bb, const osg::Vec3d & slicedir, int sliceNum)
{
	std::vector<osg::Plane> planes;
	if (!generateFixNumFencePlanes(bb, slicedir, sliceNum, planes))
	{
		return 0L;
	}
	AssistPlaneGenerator apg(_isOnEarth);
	return apg(bb, planes);
}