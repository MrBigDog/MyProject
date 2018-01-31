#include <iostream>

#include <osg/Point>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/NodeVisitor>
#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
#include <osg/ComputeBoundsVisitor>
#include <osgGA/StateSetManipulator>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
//
//class NodeAndDrawableVisitor : public osg::NodeVisitor
//{
//public:
//	NodeAndDrawableVisitor(osg::NodeVisitor::TraversalMode tm) : osg::NodeVisitor(tm)
//	{
//		_geodeNum = 0;
//		_geometryNum = 0;
//	}
//	NodeAndDrawableVisitor(osg::NodeVisitor::VisitorType type, osg::NodeVisitor::TraversalMode tm)
//		: osg::NodeVisitor(type, tm)
//	{
//		_geodeNum = 0;
//		_geometryNum = 0;
//	}
//
//	virtual ~NodeAndDrawableVisitor() {}
//
//	using osg::NodeVisitor::apply;
//	virtual void apply(osg::Node& node)
//	{
//		traverse(node);
//	}
//
//	virtual void apply(osg::Drawable& drawable)
//	{
//		_geometryNum++;
//		std::string path = "D:/gwV1/0P3dModules/model/split";
//		std::stringstream ss;
//		ss << _geometryNum;
//		std::string name = ss.str() + ".ive";
//		std::string fullname = osgDB::concatPaths(path, name);
//
//		osg::ref_ptr<osg::Geode> geode = new osg::Geode;
//		geode->addDrawable(&drawable);
//		//osgDB::writeNodeFile(*geode, fullname);
//
//		osg::Geometry* geom = drawable.asGeometry();
//		if (geom)
//		{
//			int ps = geom->getNumPrimitiveSets();
//			std::cout << ps << std::endl;
//		}
//
//	}
//
//	void traverse(osg::Node& node)
//	{
//		TraversalMode tm = getTraversalMode();
//		if (tm == TRAVERSE_NONE)
//		{
//			return;
//		}
//		else if (tm == TRAVERSE_PARENTS)
//		{
//			osg::NodeVisitor::traverse(node);
//			return;
//		}
//		osg::Geode* geode = dynamic_cast<osg::Geode*>(&node);
//		if (geode)
//		{
//			_geodeNum++;
//			std::string name = geode->getName() + ".ive";
//			std::string path = "D:/gwV1/0P3dModules/model/split";
//			std::string fullname = osgDB::concatPaths(path, name);
//			//osgDB::writeNodeFile(*geode, fullname);
//
//			unsigned numDrawables = geode->getNumDrawables();
//			for (unsigned i = 0; i < numDrawables; ++i)
//			{
//				apply(*geode->getDrawable(i));
//			}
//		}
//		else
//		{
//			osg::NodeVisitor::traverse(node);
//		}
//	}
//
//	int getGeodeNum() { return _geodeNum; }
//	int getGeometryNum() { return _geometryNum; }
//
//
//private:
//	int _geodeNum;
//	int _geometryNum;
//};


static osg::Node* createLine(osg::Vec3Array* va, const osg::Vec4& color)
{
	if (!va) return 0L;

	osg::ref_ptr<osg::Vec4Array> ca = new osg::Vec4Array;
	ca->push_back(color);

	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
	geom->setVertexArray(va);
	geom->setColorArray(ca, osg::Array::BIND_OVERALL);
	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP, 0, va->size()));

	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	geode->addDrawable(geom);

	return geode.release();
}

static osg::Node* createPoint(const osg::Vec3& pos, const osg::Vec4& color)
{
	osg::ref_ptr<osg::Vec3Array> va = new osg::Vec3Array;
	va->push_back(pos);

	osg::ref_ptr<osg::Vec4Array> ca = new osg::Vec4Array;
	ca->push_back(color);

	osg::ref_ptr<osg::Point> point = new osg::Point;
	point->setSize(10);

	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
	geom->setVertexArray(va);
	geom->setColorArray(ca, osg::Array::BIND_OVERALL);
	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POINTS, 0, va->size()));
	geom->getOrCreateStateSet()->setAttributeAndModes(point);

	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	geode->addDrawable(geom);

	return geode.release();
}

static osg::Node* createUnitPipe(const osg::Vec4& color)
{
	osg::ref_ptr<osg::Cylinder> cylin = new osg::Cylinder;
	osg::ref_ptr <osg::ShapeDrawable> sd = new osg::ShapeDrawable(cylin);
	sd->setColor(color);

	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	geode->addDrawable(sd);

	return geode.release();
}

static osg::Node* creatPipeline(osg::Vec3Array* va, const osg::Vec4& color, float radius)
{
	osg::ref_ptr<osg::Node> node = createUnitPipe(color);

	osg::ref_ptr<osg::Group> root = new osg::Group;
	for (unsigned int i = 1; i < va->size(); ++i)
	{
		const osg::Vec3& v1 = va->at(i);
		const osg::Vec3& v2 = va->at(i - 1);

		osg::Vec3 dir = v2 - v1;
		float l = (v2 - v1).length();
		dir.normalize();

		osg::Vec3 c = (v1 + v2)*0.5;

		osg::Quat rot;
		rot.makeRotate(osg::Vec3(0, 0, 1), dir);

		osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
		mt->addChild(node);
		mt->setMatrix(osg::Matrix::scale(radius, radius, l)*osg::Matrix::rotate(rot)*osg::Matrix::translate(c));

		root->addChild(mt);
	}

	return root.release();
}


class PickHandler : public osgGA::GUIEventHandler
{
public:
	PickHandler()
	{
		_radius = 15.0f;
		_depth = 500.0f;
		_va = new osg::Vec3Array;
		//_root = new osg::Group;
		_color = osg::Vec4(1, 1, 0, 1);
	}
	~PickHandler() {}

	//osg::Node* getRoot() { return _root.get(); }
	void setRadius(float radius) { _radius = radius; }
	void setDepth(float depth) { _depth = depth; }
	void setColor(const osg::Vec4& color) { _color = color; }

private:
	bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
	{
		switch (ea.getEventType())
		{
		case(osgGA::GUIEventAdapter::PUSH):
		{
			osgViewer::View* view = dynamic_cast<osgViewer::View*>(&aa);
			if (view) pick(view, ea);
			return false;
		}
		case(osgGA::GUIEventAdapter::KEYDOWN):
		{
			if (ea.getKey() == 'c' || ea.getKey() == 'C')
			{
				osg::ref_ptr<osg::Node> nn = creatPipeline(_va, _color, _radius);
				osgDB::writeNodeFile(*nn, "tunel.ive");
			}
			if (ea.getKey() == 'r' || ea.getKey() == 'R')
			{
				reset();
			}
			return false;
		}
		default:
			return false;
		}
	}

	virtual void pick(osgViewer::View* view, const osgGA::GUIEventAdapter& ea)
	{
		osgUtil::LineSegmentIntersector::Intersections intersections;
		if (view->computeIntersections(ea, intersections))
		{
			osg::Vec3d p = intersections.begin()->getWorldIntersectPoint();
			_va->push_back(p - osg::Vec3(0, 0, _depth));
			std::cout << p[0] << " " << p[1] << " " << p[2] << std::endl;

			//_root->addChild(createPoint(p, osg::Vec4(1, 1, 0, 1)));
		}
	}

	void reset() { _va->clear(); }


protected:
	float _radius, _depth;
	osg::Vec4 _color;
	osg::ref_ptr<osg::Vec3Array> _va;
	//osg::ref_ptr<osg::Group> _root;
};


int main(int argc, char** argv)
{
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("E:\\DATA\\GeoData\\Workspace\\geoModel-800.osg");

	osg::ref_ptr<osg::Group> root = new osg::Group;
	root->addChild(node);

	osg::ref_ptr<PickHandler> pickhandler = new PickHandler;
	//root->addChild(pickhandler->getRoot());
	
	osgViewer::Viewer viewer;
	viewer.addEventHandler(new osgGA::StateSetManipulator(viewer.getCamera()->getOrCreateStateSet()));
	viewer.addEventHandler(new osgViewer::StatsHandler());
	viewer.addEventHandler(pickhandler);
	viewer.setSceneData(root);
	viewer.realize();
	viewer.run();

	return (0);
}
