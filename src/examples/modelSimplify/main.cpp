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

	void apply(osg::Node& node)
	{
		traverse(node);
	}

	void apply(osg::Geode& geode)
	{
		osg::Geometry* geom = geode.getDrawable(0)->asGeometry();
		osg::Vec3Array* va = dynamic_cast<osg::Vec3Array*>(geom->getVertexArray());
		int size = va->size();

		osg::Vec4Array* ca = dynamic_cast<osg::Vec4Array*>(geom->getColorArray());
	}

	void apply(osg::PagedLOD& pl)
	{
		//std::cout << "find a PagedLod" << std::endl;
		for (unsigned int i = 0; i < pl.getNumFileNames(); ++i)
		{
			if (pl.getFileName(i).empty()) continue;
			std::cout << pl.getFileName(i) << std::endl;
		}
		traverse(pl);
	}
};

int main()
{
	std::string fname = "E:/DATA/GeoData/ALLmodel/5dxq_171110_dxf/dxq_dxf_171110/5dxq_171110_osgb/1.osgb";
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile(fname);
	std::string newname = osgDB::getNameLessExtension(fname) + ".obj";
	osgDB::writeNodeFile(*node, newname);







	//std::string tilename = "D:/BYL/ive/ZZZ8CM/Production_3/Data/Tile_+000_+000/Tile_+000_+000_L22_00010000.osgb";
	//osg::ref_ptr<osg::Node> node = osgDB::readNodeFile(tilename);

	//osg::PagedLOD* pl = dynamic_cast<osg::PagedLOD*>(node.get());
	//if (pl)
	//{
	//	std::cout << "find a PagedLod" << std::endl;
	//}
	//osg::Group* group = dynamic_cast<osg::Group*>(node.get());
	//if (group)
	//{
	//	for (unsigned int i = 0; i < group->getNumChildren(); ++i)
	//	{
	//		osg::PagedLOD* pl = dynamic_cast<osg::PagedLOD*>(group->getChild(i));
	//		if (pl)
	//		{
	//			std::cout << "yeah" << std::endl;
	//		}
	//	}
	//}

	//NodeToUsxmesh aa;
	////aa.traverse(*node);
	//node->accept(aa);

	//std::cout << "start" << std::endl;

	//osgUtil::Simplifier simplifier(0.01, 100.0);
	//node->accept(simplifier);

	//std::cout << "end" << std::endl;

	//osgDB::writeNodeFile(*node, "E:/DATA/qxmx/Tile_132102312010300213/Tile_132102312010300213.osgb");

	//osgViewer::Viewer viewer;
	//viewer.addEventHandler(new osgGA::StateSetManipulator(viewer.getCamera()->getOrCreateStateSet()));
	//viewer.addEventHandler(new osgViewer::StatsHandler());
	//viewer.setSceneData(node);
	//viewer.realize();
	//viewer.run();

	return 0;
}

