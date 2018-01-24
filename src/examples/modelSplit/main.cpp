#include <iostream>

#include <osg/Point>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/NodeVisitor>
#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>

#include <osgViewer/Viewer>

class NodeAndDrawableVisitor : public osg::NodeVisitor
{
public:
	NodeAndDrawableVisitor(osg::NodeVisitor::TraversalMode tm) : osg::NodeVisitor(tm)
	{
		_geodeNum = 0;
		_geometryNum = 0;
	}
	NodeAndDrawableVisitor(osg::NodeVisitor::VisitorType type, osg::NodeVisitor::TraversalMode tm)
		: osg::NodeVisitor(type, tm)
	{
		_geodeNum = 0;
		_geometryNum = 0;
	}

	virtual ~NodeAndDrawableVisitor() {}

	using osg::NodeVisitor::apply;
	virtual void apply(osg::Node& node)
	{
		traverse(node);
	}

	virtual void apply(osg::Drawable& drawable)
	{
		_geometryNum++;
		std::string path = "D:/gwV1/0P3dModules/model/split";
		std::stringstream ss;
		ss << _geometryNum;
		std::string name = ss.str() + ".ive";
		std::string fullname = osgDB::concatPaths(path, name);

		osg::ref_ptr<osg::Geode> geode = new osg::Geode;
		geode->addDrawable(&drawable);
		//osgDB::writeNodeFile(*geode, fullname);

		osg::Geometry* geom = drawable.asGeometry();
		if (geom)
		{
			int ps = geom->getNumPrimitiveSets();
			std::cout << ps << std::endl;
		}

	}

	void traverse(osg::Node& node)
	{
		TraversalMode tm = getTraversalMode();
		if (tm == TRAVERSE_NONE)
		{
			return;
		}
		else if (tm == TRAVERSE_PARENTS)
		{
			osg::NodeVisitor::traverse(node);
			return;
		}
		osg::Geode* geode = dynamic_cast<osg::Geode*>(&node);
		if (geode)
		{
			_geodeNum++;
			std::string name = geode->getName() + ".ive";
			std::string path = "D:/gwV1/0P3dModules/model/split";
			std::string fullname = osgDB::concatPaths(path, name);
			//osgDB::writeNodeFile(*geode, fullname);

			unsigned numDrawables = geode->getNumDrawables();
			for (unsigned i = 0; i < numDrawables; ++i)
			{
				apply(*geode->getDrawable(i));
			}
		}
		else
		{
			osg::NodeVisitor::traverse(node);
		}
	}

	int getGeodeNum() { return _geodeNum; }
	int getGeometryNum() { return _geometryNum; }


private:
	int _geodeNum;
	int _geometryNum;
};


//
//class TextureAndImageVisitor : public osg::NodeVisitor
//{
//public:
//	TextureAndImageVisitor(const std::string& rootPath)
//		: osg::NodeVisitor()
//		, _outpath(rootPath)
//	{
//		setNodeMaskOverride(~0L);
//		setTraversalMode(TRAVERSE_ALL_CHILDREN);
//	}
//	virtual ~TextureAndImageVisitor() { }
//
//public:
//	virtual void apply(osg::Texture& texture)
//	{
//		for (unsigned k = 0; k < texture.getNumImages(); ++k)
//		{
//			osg::Image* image = texture.getImage(k);
//			if (image)
//			{
//				apply(*image);
//			}
//		}
//	}
//
//	virtual void apply(osg::Image& image)
//	{
//		std::string imageName = image.getFileName();
//		std::string sname = osgDB::getSimpleFileName(imageName);
//		image.setFileName(sname);
//		std::string filename = osgDB::concatPaths(_outpath, sname);
//		if (!osgDB::fileExists(filename))
//		{
//			//gwUtil::makeDirectoryForFile(filename);
//			osgDB::writeImageFile(image, filename);
//		}
//	}
//
//public:
//	virtual void apply(osg::Node& node)
//	{
//		if (node.getStateSet())
//			apply(*node.getStateSet());
//
//		traverse(node);
//	}
//
//	virtual void apply(osg::Geode& geode)
//	{
//		if (geode.getStateSet())
//			apply(*geode.getStateSet());
//
//		for (unsigned i = 0; i < geode.getNumDrawables(); ++i)
//		{
//			apply(*geode.getDrawable(i));
//			//if (geode.getDrawable(i) && geode.getDrawable(i)->getStateSet())
//			//    apply(*geode.getDrawable(i)->getStateSet());
//		}
//		//traverse(geode);
//	}
//
//	virtual void apply(osg::Drawable& drawable)
//	{
//		if (drawable.getStateSet())
//			apply(*drawable.getStateSet());
//		//traverse(drawable);
//	}
//
//	virtual void apply(osg::StateSet& stateSet)
//	{
//		osg::StateSet::TextureAttributeList& a = stateSet.getTextureAttributeList();
//		for (osg::StateSet::TextureAttributeList::iterator i = a.begin(); i != a.end(); ++i)
//		{
//			osg::StateSet::AttributeList& b = *i;
//			for (osg::StateSet::AttributeList::iterator j = b.begin(); j != b.end(); ++j)
//			{
//				osg::StateAttribute* sa = j->second.first.get();
//				if (sa)
//				{
//					osg::Texture* tex = dynamic_cast<osg::Texture*>(sa);
//					if (tex)
//					{
//						apply(*tex);
//					}
//				}
//			}
//		}
//	}
//
//private:
//	std::string _outpath;
//};

osg::Node* createLine(osg::Vec3Array* va, const osg::Vec4& color)
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

osg::Node* createPoint(const osg::Vec3& pos, const osg::Vec4& color)
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
	geom->getOrCreateStateSet()->setAttributeAndModes(point, 1);

	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	geode->addDrawable(geom);

	return geode.release();
}

osg::Node* createUnitPipe(const osg::Vec4& color)
{
	osg::ref_ptr<osg::Cylinder> cylin = new osg::Cylinder;
	osg::ref_ptr <osg::ShapeDrawable> sd = new osg::ShapeDrawable(cylin);
	sd->setColor(color);

	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	geode->addDrawable(sd);

	return geode.release();
}

osg::Node* creatPipeline(osg::Vec3Array* va, const osg::Vec4& color, float radius)
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


#include <osg/ComputeBoundsVisitor>
#include <osg/MatrixTransform>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/StateSetManipulator>

// class to handle events with a pick
class PickHandler : public osgGA::GUIEventHandler
{
public:
	PickHandler() { _va = new osg::Vec3Array; _root = new osg::Group; }
	~PickHandler() {}

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
				osg::ref_ptr<osg::Node> nn = creatPipeline(_va, osg::Vec4(1, 1, 0, 1), 14.40);
				osgDB::writeNodeFile(*nn, "tunel.ive");
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
			_va->push_back(p);

			std::cout << p[0] << " " << p[1] << " " << p[2] << std::endl;

			_root->addChild(createPoint(p, osg::Vec4(1, 1, 0, 1)));
		}
	}

	osg::Node* getRoot() { return _root.get(); }

protected:
	osg::ref_ptr<osg::Vec3Array> _va;
	osg::ref_ptr<osg::Group> _root;
};


int main(int argc, char** argv)
{
	//osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("D:/gwV1/0P3dModules/model/split/dm_-290zdjx001-GEODE.ive");
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("E:\\DATA\\GeoData\\Workspace\\geoModel-800.osg");
	//osg::ref_ptr<osg::Node> node1 = osgDB::readNodeFile("E:\\DATA\\GeoData\\Workspace\\22.osgb");

	//osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
	//mt->addChild(node1);
	//mt->setMatrix(osg::Matrixd::scale(100, 100, 100));

	osg::ref_ptr<osg::Group> root = new osg::Group;
	root->addChild(node);

	osg::ref_ptr<PickHandler> pickhandler = new PickHandler;
	root->addChild(pickhandler->getRoot());
	//root->addChild(mt);

	//osg::ComputeBoundsVisitor cbv;
	//node->accept(cbv);

	//osg::ComputeBoundsVisitor cbv1;
	//node1->accept(cbv1);

	//const osg::BoundingBox& bb = cbv.getBoundingBox();
	//const osg::BoundingBox& bb1 = cbv1.getBoundingBox();

	osgViewer::Viewer viewer;
	viewer.addEventHandler(new osgGA::StateSetManipulator(viewer.getCamera()->getOrCreateStateSet()));
	viewer.addEventHandler(new osgViewer::StatsHandler());
	viewer.addEventHandler(pickhandler);
	viewer.setSceneData(root);
	viewer.realize();
	viewer.run();

	return (0);
}
