#pragma warning(disable: 4996)

/************************************************************************/
///本实例证明，面交的时候只能一次计算一个Geometry。
/************************************************************************/

#include <osg/Group>
#include <osg/PolygonMode>
#include <osg/ComputeBoundsVisitor>
#include <osgDB/ReadFile>
#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>
#include <osgGA/StateSetManipulator>
#include <osgUtil/Tessellator>
#include <osgUtil/PlaneIntersector>

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <algorithm>
#include <iostream>
#include <ostream>
#include <fstream>

typedef std::vector<osg::ref_ptr<osg::Vec3dArray> >Vec3dArrayVector;

static bool isVec3dEqual(const osg::Vec3d& p1, const osg::Vec3d& p2, double epsilon)
{
	if (!osg::equivalent(p1.x(), p2.x(), epsilon)) return false;
	if (!osg::equivalent(p1.y(), p2.y(), epsilon)) return false;
	if (!osg::equivalent(p1.z(), p2.z(), epsilon)) return false;
	return true;
}

static void removeRepeat(osg::Vec3dArray* va, double epsilon = FLT_EPSILON)
{
	osg::Vec3dArray::iterator it, it1;
	for (it = ++va->begin(); it != va->end();)
	{
		bool isRepeat = false;
		for (it1 = va->begin(); it1 != it; ++it1)
		{
			if (isVec3dEqual(*it, *it1, epsilon))
			{
				isRepeat = true;
				break;
			}
		}
		if (isRepeat) it = va->erase(it);
		else it++;
	}
}

int main()
{
	osg::ref_ptr<osg::Group> root = new osg::Group;

	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("D:/MongoDB/mongodbcxxtest/GeologicalTool/GeologicalTool/ttt/1_0_0.OSGB");
	root->addChild(node);

	osg::ComputeBoundsVisitor cbv;
	node->accept(cbv);
	osg::BoundingBox bb = cbv.getBoundingBox();

	osg::Plane slicePlane = osg::Plane(osg::Vec3d(0, 0, 1), bb.center());

	osg::ref_ptr<osgUtil::PlaneIntersector> intersector = new osgUtil::PlaneIntersector(slicePlane);
	intersector->setRecordHeightsAsAttributes(true);
	osgUtil::IntersectionVisitor intersectionVisitor;
	intersectionVisitor.reset();
	intersectionVisitor.setIntersector(intersector.get());
	node->accept(intersectionVisitor);

	osgUtil::PlaneIntersector::Intersections& intersections = intersector->getIntersections();
	if (!intersections.empty())
	{
		Vec3dArrayVector vaVector;
		osgUtil::PlaneIntersector::Intersections::iterator itr;
		for (itr = intersections.begin(); itr != intersections.end(); ++itr)
		{
			osgUtil::PlaneIntersector::Intersection& intersection = *itr;
			if (intersection.matrix.valid())
			{
				osg::ref_ptr<osg::Vec3dArray> vva = new osg::Vec3dArray;
				for (auto pitr = intersection.polyline.begin(); pitr != intersection.polyline.end(); ++pitr)
				{
					osg::Vec3d p = intersection.matrix.valid() ? (*pitr) * (*intersection.matrix) : (*pitr);
					vva->push_back(p);
				}
				intersection.matrix = 0;
				removeRepeat(vva);
				if (vva->size() > 2)
				{
					vaVector.push_back(vva);
				}
			}
		}

		if (!vaVector.empty())
		{
			osg::ref_ptr<osg::Geode> geode = new osg::Geode;
			osg::ref_ptr<osg::Vec3Array> va = new osg::Vec3Array;
			osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
			geode->addDrawable(geom);
			geom->setVertexArray(va);

			unsigned int totNum = 0;
			for (Vec3dArrayVector::const_iterator it = vaVector.begin(); it != vaVector.end(); ++it)
			{
				unsigned int vetNum = 0;
				for (osg::Vec3dArray::const_iterator vaIt = (*it)->begin(); vaIt != (*it)->end(); ++vaIt)
				{
					va->push_back((*vaIt));
					totNum++; vetNum++;
				}
				geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POLYGON, totNum - vetNum, vetNum));
			}

			osg::ref_ptr<osg::Vec3Array> na = new osg::Vec3Array(va->size());
			na->assign(va->size(), slicePlane.getNormal());
			geom->setNormalArray(na, osg::Array::BIND_PER_VERTEX);

			osgUtil::Tessellator tscx;
			tscx.setTessellationNormal((slicePlane.getNormal()));
			tscx.setTessellationType(osgUtil::Tessellator::TESS_TYPE_GEOMETRY);
			tscx.setBoundaryOnly(false);
			tscx.setWindingType(osgUtil::Tessellator::TESS_WINDING_NONZERO);
			tscx.retessellatePolygons(*geom);

			{//test
				osg::ref_ptr<osg::PolygonMode> pm = new osg::PolygonMode;
				pm->setMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::FILL);
				geode->getOrCreateStateSet()->setAttributeAndModes(pm, osg::StateAttribute::OVERRIDE | osg::StateAttribute::ON);
				geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
			}

			root->addChild(geode);
		}
	}

	osgViewer::Viewer viewer;
	viewer.addEventHandler(new osgGA::StateSetManipulator(viewer.getCamera()->getOrCreateStateSet()));
	viewer.addEventHandler(new osgViewer::StatsHandler());
	viewer.setSceneData(root);
	viewer.realize();
	viewer.run();

	return 0;
}
