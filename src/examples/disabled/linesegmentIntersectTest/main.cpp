#include "SliceOperator.h"
#include "WellGenerator.h"
#include "TunnelGenerator.h"
#include "MeshConsolidator.h"
#include "FoundationDitchGenerator.h"
#include <osg/Quat>
#include <osg/ComputeBoundsVisitor>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>

#include <osgGA/StateSetManipulator>
#include <osgGA/GUIEventHandler>
#include <osgGA/TrackballManipulator>

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <osg/NodeVisitor>
#include <osg/Depth>
#include <osg/StencilTwoSided>
#include <osg/PositionAttitudeTransform>
#include <osg/MatrixTransform>

#define ON_AND_PROTECTED  osg::StateAttribute::ON | osg::StateAttribute::PROTECTED
#define OFF_AND_PROTECTED osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED

class geoDataProcess :public osg::NodeVisitor
{
public:
	geoDataProcess() : osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN)
	{ }

	void apply(osg::Geode& geode)
	{
		//for (unsigned int i = 0; i < geode.getNumDrawables() - 1; ++i)
		//{
		//	geode.removeDrawable(i);
		//}
		while (geode.getNumDrawables() > 1)
		{
			geode.removeDrawable(geode.getDrawable(1));
		}
	}
};

class GeologicalDirectoryVisitor
{
public:
	GeologicalDirectoryVisitor::GeologicalDirectoryVisitor(osg::Group* root)
		: _root(root)
	{
		_ca = new osg::Vec4Array;
		_ca->push_back(osg::Vec4(1, 0, 0, 1));
		_ca->push_back(osg::Vec4(1, 1, 0, 1));
		_ca->push_back(osg::Vec4(0, 1, 0, 1));

		_index = 0;
	}

public:
	void GeologicalDirectoryVisitor::traverse(const std::string& path)
	{
		if (osgDB::fileType(path) == osgDB::DIRECTORY)
		{
			if (handleDir(path))
			{
				osgDB::DirectoryContents files = osgDB::getDirectoryContents(path);
				for (osgDB::DirectoryContents::const_iterator f = files.begin(); f != files.end(); ++f)
				{
					if (f->compare(".") == 0 || f->compare("..") == 0)
						continue;

					std::string filepath = osgDB::concatPaths(path, *f);
					traverse(filepath);
				}
			}
		}
		else if (osgDB::fileType(path) == osgDB::REGULAR_FILE)
		{
			handleFile(path);
		}
	}

private:
	void GeologicalDirectoryVisitor::handleFile(const std::string& filename)
	{
		std::string ext = osgDB::getLowerCaseFileExtension(filename);
		if (ext != "ive"&&ext != "osgb") return;

		osg::ref_ptr<osg::Node> node = osgDB::readNodeFile(filename);
		//geoDataProcess gdp;
		//node->accept(gdp);
		if (!node) return;

		//std::string sname = osgDB::getNameLessExtension(filename);
		//osgDB::writeNodeFile(*node, sname + ".IVE");

		if (_root && _root != NULL)
		{
			_root->addChild(node);
		}
	}

	bool GeologicalDirectoryVisitor::handleDir(const std::string& path)
	{
		return true;
	}

private:
	osg::ref_ptr<osg::Group> _root;
	osg::ref_ptr<osg::Vec4Array> _ca;
	unsigned int _index;
};

class GeometryVisitor :public osg::NodeVisitor
{
public:
	GeometryVisitor(const osg::Vec3d& pos)
		:osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN)
		, _pos(pos)
	{}

	virtual void apply(osg::Geode& geode)
	{
		MeshConsolidator::run(geode);
		geode.dirtyBound();

		for (unsigned int i = 0; i < geode.getNumDrawables(); ++i)
		{
			osg::Geometry* geom = dynamic_cast<osg::Geometry*>(geode.getDrawable(i));
			if (!geom) continue;

			osg::Vec3Array* va = dynamic_cast<osg::Vec3Array*>(geom->getVertexArray());
			if (!va) continue;

			for (osg::Vec3Array::iterator it = va->begin(); it != va->end(); ++it)
			{
				*it = *it - _pos;
			}

			geom->dirtyDisplayList();
			geom->dirtyBound();
		}
		geode.dirtyBound();
	}
private:
	osg::Vec3d _pos;
};


class MyGuiEventHandler :public osgGA::GUIEventHandler
{
public:
	MyGuiEventHandler(osg::Node* geoNode, osg::Group* root, const osg::Plane& plane)
		:_geoNode(geoNode), _isVisible(true), _root(root), _plane(plane)
	{}

	bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
	{
		if (ea.getEventType() == osgGA::GUIEventAdapter::KEYUP)
		{
			if (ea.getKey() == 'r')
			{
				if (_geoNode)
				{
					_isVisible = !_isVisible;
					_geoNode->setNodeMask(_isVisible);
				}
			}
			if (ea.getKey() == 'c')
			{
				if (_root&&_geoNode)
				{
					//SliceOperator so;
					//osg::Node* profilenode = so.slice(_geoNode, _plane, _root);
					//if (profilenode)
					//{
					//	_root->addChild(profilenode);
					//}
				}
			}

		}
		return false;
	}
private:
	bool _isVisible;
	osg::Plane _plane;
	osg::ref_ptr<osg::Node> _geoNode;
	osg::ref_ptr<osg::Group> _root;
};

#include "MapNode.h"

int main()
{
	//osg::ref_ptr<osg::Node> testnode = osgDB::readNodeFile("E:/DATA/newGeoData/73_n.dxf");
	//osgDB::writeNodeFile(*testnode, "E:/DATA/newGeoData/73_n.osgb");
	////图层;
	//osg::ref_ptr<GeologicalLayer0> l1 = new GeologicalLayer0("l1", osgDB::readNodeFile("D:/MongoDB/mongodbcxxtest/GeologicalTool/GeologicalTool/test/0_0_0.OSGB"));

	////剖面图层;
	//PlaneProfileOptions planeoptions;
	//planeoptions.addPlane(osg::Plane(osg::Vec3(1, 0, 0), osg::Vec3d(281.07714843750000 + 100.0, 589.49401855468750, -18.289400100708008)));
	//osg::ref_ptr<GeologicalProfileLayer> profileLayer = new GeologicalProfileLayer("test", planeoptions);

	////Map添加图层;
	//osg::ref_ptr<Map> map = new Map;
	//map->addGeologicalLayer(l1);
	//map->addGeologicalProfileLayer(profileLayer);

	////Map Node
	//osg::ref_ptr<MapNode> mapnode = new MapNode(map);

	osg::ref_ptr<osg::Group> root = new osg::Group;
	//root->addChild(mapnode);

	//osg::ComputeBoundsVisitor cbv;
	//root->accept(cbv);

	//osg::BoundingBox bb = cbv.getBoundingBox();

	//osg::Vec3d bbc = bb.center();

	osg::ref_ptr<osg::Group> georoot = new osg::Group;
	root->addChild(georoot);
	{//test
		osg::ref_ptr<osg::PolygonMode> pm = new osg::PolygonMode;
		pm->setMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::POINT);
		//georoot->getOrCreateStateSet()->setAttributeAndModes(pm, osg::StateAttribute::OVERRIDE | osg::StateAttribute::ON);
		//georoot->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	}

	GeologicalDirectoryVisitor gdv(georoot);
	gdv.traverse("E:/DATA/TFXQ_SW_model_V_V1.01/NEW_IVE");
	//gdv.traverse("E:/DATA/newGeoData/tfxq_sw_NW_model_ive");
	//gdv.traverse("D:\\Data\\GeologicalData\\ive");
	//gdv.traverse("E:/DATA/dizhi");
	////gdv.traverse("D:\\GeologicalModel\\ive(1)");
	//gdv.traverse("E:\\DATA\\newGeoData\\tfxq_sw_NW_model");
	////gdv.traverse("E:\\DATA\\geologicalModelTest\\ALL");

	////gdv.traverse("D:/cow");
	////gdv.traverse("D:\\GeologicalModel\\model\\tianfuxinqu\\shuiwen\\ALL");

	//georoot->addChild(osgDB::readNodeFile("D:/MongoDB/mongodbcxxtest/GeologicalTool/GeologicalTool/5_24_4.ive"));
	//georoot->addChild(osgDB::readNodeFile("E:/DATA/newGeoData/73_n.osgb"));
	//georoot->addChild(osgDB::readNodeFile("E:/DATA/newGeoData/73_n.osgb"));
	//georoot->addChild(osgDB::readNodeFile("E:/DATA/TFXQ_SW_model_V_V1.01/IVE/13_J3S.IVE"));
	//georoot->addChild(osgDB::readNodeFile("E:/DATA/TFXQ_SW_model_V_V1.01/NEW_IVE/73_n.osgb"));
	//georoot->addChild(osgDB::readNodeFile("E:/DATA/TFXQ_SW_model_V_V1.01/NEW_IVE/12_k2g.IVE"));
	//georoot->addChild(osgDB::readNodeFile("E:/DATA/TFXQ_SW_model_V_V1.01/NEW_IVE/11_K2g.IVE"));
	//georoot->addChild(osgDB::readNodeFile("E:/DATA/TFXQ_SW_model_V_V1.01/NEW_IVE/10_K2g.IVE"));
	//georoot->addChild(osgDB::readNodeFile("E:/DATA/TFXQ_SW_model_V_V1.01/NEW_IVE/9_K2j.IVE"));
	//georoot->addChild(osgDB::readNodeFile("E:/DATA/TFXQ_SW_model_V_V1.01/NEW_IVE/8_K2j.dxf"));

	osg::ComputeBoundsVisitor cbv;
	georoot->accept(cbv);
	osg::BoundingBox bb = cbv.getBoundingBox();

	//GeometryVisitor gv(bb.center());
	//georoot->accept(gv);

	//georoot->dirtyBound();

	osg::ComputeBoundsVisitor cbv1;
	georoot->accept(cbv1);
	osg::BoundingBox bb1 = cbv1.getBoundingBox();

	osg::Plane plane(osg::Vec3d(1, 1, 0), bb1.center());
	osg::Plane plane1(osg::Vec3d(0, 0, 1), bb1.center() + osg::Vec3d(0, 0, 0.0));//-155.0出问题。
	//osg::Plane plane1(0.0, 0.0, 1.0, -0.245);//-225.0 -245.0出问题。

	SliceOperator so;
	osg::ref_ptr<osg::Node> profilenode = so.slice(georoot, plane1, root);
	if (profilenode)
	{
		osgDB::writeNodeFile(*profilenode, "dogprofile.osgb");
		root->addChild(profilenode);
	}

	//root->addChild(so.slice(geonode, plane1, root));
	//root->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	//georoot->setNodeMask(0);

	{//钻孔;
		WellGenerator wg(false);
		osg::ref_ptr<osg::Node> well = wg(georoot, 10.0, bb.center() + osg::Vec3d(100, 100, (bb.zMax() - bb.zMin())*0.5), 0.5);
		if (well)
		{
			root->addChild(well);
		}
	}

	{//隧道;
		osg::ref_ptr<osg::Vec3dArray> va = new osg::Vec3dArray;
		va->push_back(osg::Vec3d((bb.xMax() + bb.xMin())*0.5, bb.yMax(), (bb.zMax() + bb.zMin())*0.5 - 200.0));
		va->push_back(osg::Vec3d((bb.xMax() + bb.xMin())*0.5, bb.yMin(), (bb.zMax() + bb.zMin())*0.5 - 200.0));
		//va->push_back(osg::Vec3d(bb.xMax(), (bb.yMax() + bb.yMin())*0.5, (bb.zMax() + bb.zMin())*0.5));
		//va->push_back(osg::Vec3d(bb.xMin(), (bb.yMax() + bb.yMin())*0.5, (bb.zMax() + bb.zMin())*0.5));

		float vwidth = 100.0f;

		//SquareTunelGenerator tg(false);
		//root->addChild(tg(georoot, va, vwidth, vwidth*0.5, SquareTunelGenerator::PLANE_TOP));

		CircleTunelGenerator ctg(false);
		root->addChild(ctg(georoot, va, vwidth));
	}

	//float modelSize = vwidth*2.5f;
	//osg::Light* myLight2 = new osg::Light;
	//myLight2->setLightNum(0);
	//myLight2->setPosition(osg::Vec4(bb.center(), 1.0f));
	//myLight2->setAmbient(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
	//myLight2->setDiffuse(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
	//myLight2->setConstantAttenuation(1.0f);
	//myLight2->setLinearAttenuation(1.0f / modelSize);
	//myLight2->setQuadraticAttenuation(0.0f / osg::square(modelSize));

	//{//基坑
	//	double zcenter = (bb.zMax() + bb.zMin())*0.5 - 300.0;
	//	double x0 = bb.xMin() + (bb.xMax() - bb.xMin())*0.45;
	//	double x1 = bb.xMin() + (bb.xMax() - bb.xMin())*0.65;
	//	double y0 = bb.yMin() + (bb.yMax() - bb.yMin())*0.45;
	//	double y1 = bb.yMin() + (bb.yMax() - bb.yMin())*0.65;
	//	osg::ref_ptr<osg::Vec3Array> va1 = new osg::Vec3Array;
	//	va1->push_back(osg::Vec3d(x0, y0, zcenter));
	//	va1->push_back(osg::Vec3d(x1, y0, zcenter));
	//	va1->push_back(osg::Vec3d(x1, y1, zcenter));
	//	va1->push_back(osg::Vec3d(x0, y1, zcenter));

	//	FoundationDitchGenerator fdg;
	//	root->addChild(fdg(georoot, va1));
	//}


	osgViewer::Viewer viewer;
	//viewer.setLight(myLight2);
	//viewer.setLightingMode(osg::View::SKY_LIGHT);
	//osg::Light* light = viewer.getCamera()->getView()->getLight();
	//if (light != NULL)
	//{
	//	light->setAmbient(osg::Vec4(0.5, 0.5, 0.5, 1));
	//	light->setDiffuse(osg::Vec4(1, 1, 1, 1));
	//	light->setSpecular(osg::Vec4(1, 1, 1, 1));
	//	//light->setPosition(osg::Vec4(0.0, 0.0, 0.0, 1.0));
	//	light->setConstantAttenuation(0.0000001);
	//	//light->setLinearAttenuation(0.001);
	//	//light->setQuadraticAttenuation(0.00001);
	//	//light->setQuadraticAttenuation(0.00001);
	//	light->setPosition(osg::Vec4(bb.center(), 1.0));
	//	light->setDirection(osg::Vec3(0, 1, 0));
	//	//light->setSpotCutoff(80.0);
	//	//light->setSpotExponent(100.0);

	//	//osg::ref_ptr<osg::LightSource> ls = new osg::LightSource();
	//	//osg::ref_ptr< osg::Light> lt = new osg::Light;
	//	//lt->setLightNum(0);
	//	//lt->setPosition(osg::Vec4(0.0, -1.0, 0.0, 0));
	//	//lt->setAmbient(osg::Vec4(0.2, 0.2, 0.2, 1.0));
	//	//lt->setDiffuse(osg::Vec4(1.0, 1.0, 1.0, 1.0));
	//	//ls->setLight(light);
	//}

	viewer.addEventHandler(new osgGA::StateSetManipulator(viewer.getCamera()->getOrCreateStateSet()));
	viewer.addEventHandler(new osgViewer::StatsHandler());
	//viewer.addEventHandler(new osgViewer::WindowSizeHandler());
	//viewer.addEventHandler(new osgViewer::ThreadingHandler());
	//viewer.addEventHandler(new osgViewer::LODScaleHandler());
	//viewer.addEventHandler(new osgViewer::RecordCameraPathHandler());
	//viewer.addEventHandler(new osgViewer::ScreenCaptureHandler());
	viewer.addEventHandler(new MyGuiEventHandler(georoot, root, plane1));
	viewer.getDatabasePager()->setTargetMaximumNumberOfPageLOD(1);
	viewer.getCamera()->setSmallFeatureCullingPixelSize(-1.0f);
	viewer.setCameraManipulator(new osgGA::TrackballManipulator());
	viewer.setSceneData(root);
	viewer.realize();

	while (!viewer.done())
	{
		viewer.frame();
	}
	viewer.run();

	return 0;
}