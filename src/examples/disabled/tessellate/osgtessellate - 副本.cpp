/************************************************************************/
/* ≤‚ ‘Tornado Server                                                   */
/************************************************************************/

#include <osgEarth/HTTPClient>
#include <osgEarth/Notify>
#include <osgEarth/Config>

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/PagedLOD>
#include <osg/ValueObject>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgGA/TrackballManipulator>
#include <osgGA/StateSetManipulator>

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <ostream>
#include <fstream>

using namespace osgEarth;

class MyNodeVisitor :public osg::NodeVisitor
{
public:
	MyNodeVisitor(/*const osg::Vec4& color*/)
		:osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN)/*, _color(color)*/
	{
		_groupnum = 0;
	}

	//void apply(osg::Geode& geode)
	//{
	//	unsigned int geoNum = geode.getNumDrawables();
	//	for (unsigned int i = 0; i < geoNum; ++i)
	//	{
	//		osg::Geometry* geom = geode.getDrawable(i)->asGeometry();
	//		if (!geom) continue;

	//		osg::Vec3Array* va = dynamic_cast<osg::Vec3Array*>(geom->getVertexArray());
	//		if (!va) continue;

	//		osg::ref_ptr<osg::Vec4Array> ca = new osg::Vec4Array;
	//		//ca->reserve(va->size());
	//		//for (unsigned int j = 0; j < va->size(); ++j)
	//		{
	//			ca->push_back(_color);
	//		}
	//		geom->setColorArray(ca, osg::Array::BIND_OVERALL);
	//		geom->dirtyDisplayList();
	//	}
	//}

	void apply(osg::Group& node)
	{
		////std::cout << () << std::endl;
		//if (dynamic_cast<osg::PagedLOD*>(&node))
		//{
		//	traverse(node);
		//}
		//std::cout << node.getNumChildren() << std::endl;
		if (node.getNumChildren() == 4)
		{
			_groupnum++;
		}
		//
		traverse(node);
	}

	//void apply(osg::PagedLOD& pg)
	//{
	//	std::cout << pg.getNumChildren() << std::endl;

	//	traverse(pg);
	//}

//private:
//	osg::Vec4 _color;
	unsigned int _groupnum;
};

//±È¿˙µ±«∞øÈ;
class GeologicalTileVistor :public osg::NodeVisitor
{
public:
	GeologicalTileVistor() :osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN) {}
	void apply(osg::Node& node)
	{
		std::string tkey;
		node.getUserValue("tile_key", tkey);
		if (!tkey.empty())
		{
			_tiles[tkey] = &node;
			std::cout << tkey << std::endl;
		}

		traverse(node);
	}
	const std::map<std::string, osg::ref_ptr<osg::Node> >& getTiles() const
	{
		return _tiles;
	}

private:
	std::map<std::string, osg::ref_ptr<osg::Node> > _tiles;
};

int main(int argc, char** argv)
{

	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("D:/MongoDB/mongodbcxxtest/GeologicalTool/GeologicalTool/L0_X0_Y0.OSGB");


	//MyNodeVisitor mnv(osg::Vec4(255.0 / 255.0, 210.0 / 255.0, 165.0 / 255.0f, 1.0));
	//node->accept(mnv);

	//osgDB::writeNodeFile(*node, "E:/DATA/newGeoData/tfxq_sw_NW_model_ive/012_k2g.IVE");
	//HTTPClient http;

	//std::string url = "http://localhost:8010/wjh_model/gy1010jz4a0027";

	//HTTPResponse response = http.get(url);

	//std::ofstream outfile("dogtest.osgb", std::ofstream::binary);
	//if (response.isOK())
	//{
	//	unsigned int numpart = response.getNumParts();
	//	for (unsigned int i = 0; i < numpart; ++i)
	//	{
	//		outfile << response.getPartAsString(i);
	//	}
	//	outfile.close();
	//	//OE_NOTICE << content << std::endl;
	//}
	//else
	//{
	//	OE_WARN << "Request failed; error = " << response.getCode() << std::endl;
	//}

	osgViewer::Viewer viewer;
	viewer.setCameraManipulator(new osgGA::TrackballManipulator);
	viewer.addEventHandler(new osgGA::StateSetManipulator(viewer.getCamera()->getOrCreateStateSet()));
	viewer.addEventHandler(new osgViewer::StatsHandler());
	viewer.addEventHandler(new osgViewer::WindowSizeHandler());
	viewer.addEventHandler(new osgViewer::ThreadingHandler());
	viewer.addEventHandler(new osgViewer::LODScaleHandler());
	viewer.addEventHandler(new osgViewer::RecordCameraPathHandler());
	viewer.addEventHandler(new osgViewer::ScreenCaptureHandler());
	viewer.getDatabasePager()->setTargetMaximumNumberOfPageLOD(1);
	viewer.setSceneData(node);
	viewer.realize();
	while (!viewer.done())
	{
		GeologicalTileVistor mnv;
		node->accept(mnv);
		//std::cout << mnv._groupnum << std::endl;
		viewer.frame();
	}
	viewer.run();

	return 0;
}

//http://172.30.17.129:8010/wjh_model/gy1010jz4a0027