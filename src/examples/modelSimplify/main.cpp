#include <osgGA/StateSetManipulator>
#include <osgViewer/Viewer>
#include <osgViewer/CompositeViewer>
#include <osgViewer/ViewerEventHandlers>

#include <osgUtil/Simplifier>

#include <osg/io_utils>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>

#include <osg/ShapeDrawable>


int main()
{
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("E:/DATA/qxmx/Tile_132102312010300213/Tile_132102312010300213.obj");

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

