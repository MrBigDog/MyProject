#include "Voxelizer.h"

#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
#include <osg/ComputeBoundsVisitor>
#include <osg/TriangleFunctor>

#include <osgDB/ReadFile>
#include <osgGA/StateSetManipulator>

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <iostream>
#include <cstdlib>

static osg::Node* makeUnitBox()
{
	osg::ref_ptr<osg::Box> vbox = new osg::Box(osg::Vec3f(0, 0, 0), 1.0f);
	osg::ref_ptr<osg::ShapeDrawable> sd = new osg::ShapeDrawable(vbox);
	sd->setColor(osg::Vec4(1, 1, 1, 1));
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	geode->addDrawable(sd);

	return geode.release();
}

static osg::MatrixTransform* makeMatrixTransform(const osg::Vec3d& pos, float vsize)
{
	osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
	mt->setMatrix(osg::Matrixd::scale(vsize, vsize, vsize)*osg::Matrixd::translate(pos));
	return mt.release();
}

osg::Vec4 getGeomColor(osg::Geometry* geom)
{
	if (!geom) return osg::Vec4(1, 1, 1, 1);
	osg::Vec4Array* ca = dynamic_cast<osg::Vec4Array*>(geom->getColorArray());
	if (!ca) return osg::Vec4(1, 1, 1, 1);
	if (ca->empty()) return osg::Vec4(1, 1, 1, 1);

	return ca->at(0);
}

struct TrangelTraverser
{
public:
	TrangelTraverser()
	{}

	inline void operator () (const osg::Vec3& v1, const osg::Vec3& v2, const osg::Vec3& v3)
	{
		V3 vv1(v1.x(), v1.y(), v1.z());
		V3 vv2(v2.x(), v2.y(), v2.z());
		V3 vv3(v1.x(), v3.y(), v3.z());
		Triangle trangle(vv1, vv2, vv3);
		_trangles.push_back(trangle);
	}
	const std::vector<Triangle>& getTrangles() const { return _trangles; }

private:
	std::vector<Triangle> _trangles;
};

class GeometryVisitor :public osg::NodeVisitor
{
public:
	GeometryVisitor() :osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN) {}

	void apply(osg::Geode& geode)
	{
		for (unsigned int i = 0; i < geode.getNumDrawables(); ++i)
		{
			osg::Geometry* geom = geode.getDrawable(i)->asGeometry();
			if (!geom) continue;

			osg::Vec3Array* va = dynamic_cast<osg::Vec3Array*>(geom->getVertexArray());
			if (!va || va->empty()) continue;

			osg::TriangleFunctor<TrangelTraverser> tt;
			geom->accept(tt);

			const std::vector<Triangle>& trangles = tt.getTrangles();

			_trangles.push_back(trangles);
		}
	}

	const std::vector<std::vector<Triangle> >& getTrangles() const { return _trangles; }

private:
	std::vector<std::vector<Triangle> > _trangles;
};

class MyGeometry :public Geometry
{
public:
	MyGeometry(osg::Node* node)
	{
		traverse(node);
	}

private:
	void traverse(osg::Node* node)
	{
		if (!node) return;

		GeometryVisitor gv;
		node->accept(gv);

		std::vector<std::vector<Triangle> > ts = gv.getTrangles();
		for (unsigned int i = 0; i < ts.size(); ++i)
		{
			for (unsigned int j = 0; j < ts[i].size(); ++j)
			{
				triangles.push_back(ts[i][j]);
			}
		}
	}
};


int main(int argc, char* argv[])
{
	//string fname = "E:/DATA/GeoData/ALLmodel/5dxq_171110_dxf/dxq_dxf_171110/1.stl";
	//string fname = "E:/BYL/Voxelizer-master/sphere.stl";// (argv[1]);
	string fname = "C:/COMMON_LIBRARY/OpenSceneGraph-Data-master/cow.osg";// (argv[1]);
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile(fname);

	osg::ComputeBoundsVisitor cbv;
	node->accept(cbv);
	osg::BoundingBox bb = cbv.getBoundingBox();

	bb.corner(0);

	// import geometry
	//Geometry geo(fname);
	MyGeometry geo(node);
	// The geometry bound, min/max corners coordinates will be reported.
	// These information will be used to determine the appropriate bound the grid
	// For this example we use the sphere.stl file
	// Tese reported information are
	//
	//GEO bound = Bbox(
	//minCorner:    (-1.0, 1.0, 1.0)
	//maxCorner:    (-1.0, 1.0, 1.0)
	//Extend:       ( 2,   2,   2  )
	//)
	// Now we need create the bound of the **grid**. 
	// We set it larger then the bound of the **geometry**.
	// Let's say: 
	// minCorner (-1,-1,-0.5)
	// maxCorner ( 1, 1, 1.5)
	V3 gridMinCorner(bb.corner(0).x(), bb.corner(0).y(), bb.corner(0).z()/*-2.0, -2.0, -2.0*/);
	V3 gridMaxCorner(bb.corner(7).x(), bb.corner(7).y(), bb.corner(7).z()/*2.0, 2.0, 2.0*/);
	V3 gridExtend = gridMaxCorner - gridMinCorner;

	// and we want a 33 voxels along X direction,
	double dx = gridExtend.x / 199;
	// and the grid number in each direction can be determined
	// Why not 32??? The voxillization implemntation is not robust enough.
	// If using 32, then the polar points causes wrong vexillization in the sphere axis

	// set grid bounding corner
	int3 gridNum = int3(gridExtend.x / dx, gridExtend.y / dx, gridExtend.z / dx);

	// create the grid finally
	GridBox grid(gridMinCorner, dx, gridNum);

	// generate voxilzer from geometry and gridbox
	Voxelizer vox(geo, grid);

	// get flag
	const char* flag = vox.get_flag();
	gridNum = grid.get_gridNum();

	float halfsize = dx*0.5;

	osg::Vec3d mincorner(gridMinCorner.x, gridMinCorner.y, gridMinCorner.z);
	osg::Vec3d halfgrid(halfsize, halfsize, halfsize);

	osg::ref_ptr<osg::Node> unitBox = makeUnitBox();

	osg::ref_ptr<osg::Group> root = new osg::Group;
	for (int iz = 1; iz <= gridNum.nz; iz++)
	{
		for (int iy = 1; iy <= gridNum.ny; iy++)
		{
			for (int ix = 0; ix < gridNum.nx; ix++)
			{
				int vindex = (iz - 1) * gridNum.nx * gridNum.ny + (iy - 1) * gridNum.nx + ix;
				if (flag[vindex] == 1)
				{
					osg::Vec3d offset = osg::Vec3d((ix + 1)*dx, iy*dx, iz*dx) - halfgrid;
					osg::Vec3d pos = mincorner + offset;

					osg::ref_ptr<osg::MatrixTransform> mt = makeMatrixTransform(pos, dx);
					mt->addChild(unitBox);
					root->addChild(mt);
				}
			}
		}
	}

	root->addChild(node);
	osgViewer::Viewer viewer;
	viewer.addEventHandler(new osgViewer::StatsHandler());
	viewer.addEventHandler(new osgViewer::WindowSizeHandler());
	viewer.setSceneData(root);
	viewer.realize();
	viewer.run();


	//// count solid flag
	//int count = 0;
	//for (int i = 0; i < gridNum.nx * gridNum.ny * gridNum.nz; i++)
	//{
	//	//if (*(flag+i) == 0) count++;
	//	if (flag[i] == 1) count++;
	//}
	//cout << "count of flag == 1 : " << count << endl;

	//// wirte vtk file of flag, use paraview to view the flag data
	//vox.write_vtk_image();

	//system("pause");

	return 0;
}
