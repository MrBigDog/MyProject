#include <osg/io_utils>
#include <osg/ShapeDrawable>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>

#include <osgGA/StateSetManipulator>

#include <osgUtil/Simplifier>

#include <osgViewer/Viewer>
#include <osgViewer/CompositeViewer>
#include <osgViewer/ViewerEventHandlers>

class NodeToUsxmesh :public osg::NodeVisitor
{
public:
	NodeToUsxmesh() :osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN)
	{
	}

	void apply(osg::Geode& geode)
	{
		osg::Geometry* geom = geode.getDrawable(0)->asGeometry();

		osg::Vec3Array* va =dynamic_cast<osg::Vec3Array*>( geom->getVertexArray());


		int size = va->size();
	}
};

int main()
{
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("D:/BYL/ive/ZZZ8CM/Production_3/Data/Tile_+000_+000/Tile_+000_+000.osgb");

	NodeToUsxmesh aa;
	aa.traverse(*node);
	//node->accept(aa);

	std::cout << "start" << std::endl;

	osgUtil::Simplifier simplifier(0.01, 100.0);
	node->accept(simplifier);

	std::cout << "end" << std::endl;

	osgDB::writeNodeFile(*node, "E:/DATA/qxmx/Tile_132102312010300213/Tile_132102312010300213.osgb");

	osgViewer::Viewer viewer;
	viewer.addEventHandler(new osgGA::StateSetManipulator(viewer.getCamera()->getOrCreateStateSet()));
	viewer.addEventHandler(new osgViewer::StatsHandler());
	viewer.setSceneData(node);
	viewer.realize();
	viewer.run();

	return 0;
}

