#define VOXELIZER_DEBUG
#define VOXELIZER_IMPLEMENTATION
#include "voxelizer.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include <iostream>
#include <sstream>
#include <fstream>

#include <osg/Group>
#include <osg/TriangleIndexFunctor>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgDB/FileNameUtils>
#include <osgViewer/Viewer>

#define TRIANGULATE
//#define POINT_CLOUD

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
	{
		_ia = new osg::UIntArray;
	}
	inline void operator()(unsigned int p1, unsigned int p2, unsigned int p3)
	{
		_ia->push_back(p1);
		_ia->push_back(p2);
		_ia->push_back(p3);
	}
	osg::UIntArray* getIndexArray() const { return _ia; }
private:
	osg::ref_ptr<osg::UIntArray> _ia;
};

struct MeshInf
{
	MeshInf(vx_mesh* mesh, const osg::Vec4& color)
		:_mesh(mesh)
		, _color(color) {}
	vx_mesh* _mesh;
	osg::Vec4 _color;
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

			osg::TriangleIndexFunctor<TrangelTraverser> tt;
			geom->accept(tt);

			osg::UIntArray* ia = tt.getIndexArray();

			vx_mesh_t* mesh = vx_mesh_alloc(va->size(), ia->size());
			for (unsigned int j = 0; j < va->size(); ++j)
			{
				mesh->vertices[j].x = va->at(j).x();
				mesh->vertices[j].y = va->at(j).y();
				mesh->vertices[j].z = va->at(j).z();
			}
			for (unsigned int k = 0; k < ia->size(); ++k)
			{
				mesh->indices[k] = ia->at(k);
			}

			osg::Vec4 color = getGeomColor(geom);

			meshList.push_back(MeshInf(mesh, color));
		}
	}

	const std::vector<MeshInf>& getMeshList()const { return meshList; }

private:
	std::vector<MeshInf> meshList;
};

static osg::Node* makeNodeFromMesh(vx_mesh* mesh, const osg::Vec4& color)
{
	if (!mesh) return 0L;
	if (!mesh->vertices) return 0L;
	if (!mesh->indices) return 0L;
	assert(mesh->nindices % 36 == 0);
	assert(mesh->nnormals == 6);

	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	for (int i = 0; i < mesh->nindices; ++i)
	{
		if ((i + 1) % 36 == 0)
		{
			osg::ref_ptr<osg::Vec3Array> subVa = new osg::Vec3Array; subVa->reserve(36);
			osg::ref_ptr<osg::Vec3Array> subNa = new osg::Vec3Array; subNa->reserve(36);
			for (int subi = 35; subi >= 0; --subi)
			{
				int ii = i - subi;

				int vi = mesh->indices[ii];
				osg::Vec3 v(mesh->vertices[vi].x, mesh->vertices[vi].y, mesh->vertices[vi].z);
				subVa->push_back(v);

				int ni = (35 - subi) / 6;
				osg::Vec3 normal(mesh->normals[ni].x, mesh->normals[ni].y, mesh->normals[ni].z);
				subNa->push_back(normal);
			}

			osg::ref_ptr<osg::Vec4Array> subCa = new osg::Vec4Array;
			subCa->push_back(color);

			osg::ref_ptr<osg::Geometry> subGeom = new osg::Geometry;
			subGeom->setVertexArray(subVa);
			subGeom->setColorArray(subCa, osg::Array::BIND_OVERALL);
			subGeom->setNormalArray(subNa, osg::Array::BIND_PER_VERTEX);
			subGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES, 0, subVa->size()));

			geode->addDrawable(subGeom);
		}
	}

	return geode.release();
}

int main(int argc, char** argv)
{
	float res = 1.0;
	float precision = 0.01;

	std::string filePath = "E:/DATA/GeoData/ALLmodel/5dxq_171110_dxf/dxq_dxf_171110/5dxq_171110_osgb/1.osgb";

	osg::ref_ptr<osg::Node> onode = osgDB::readNodeFile(filePath);
	if (!onode) return EXIT_FAILURE;

	GeometryVisitor gv;
	onode->accept(gv);

	osg::ref_ptr<osg::Group> root = new osg::Group;

	std::vector<MeshInf> meshlist = gv.getMeshList();
	for (unsigned int i = 0; i < meshlist.size(); ++i)
	{
		vx_mesh* mesh = meshlist[i]._mesh;
		vx_mesh_t* result = vx_voxelize(mesh, res, res, res, precision);

		osg::ref_ptr<osg::Node> subnode = makeNodeFromMesh(result, meshlist[i]._color);
		if (!subnode.valid()) continue;

		root->addChild(subnode);

		vx_mesh_free(result);
		vx_mesh_free(mesh);
	}

	std::string fpath = osgDB::getFilePath(filePath);
	std::string sname = osgDB::getSimpleFileName(filePath);
	std::string ssname = osgDB::getNameLessExtension(sname);
	std::string newname = ssname + "_voxel.osgb";
	std::string newfilepath = osgDB::concatPaths(fpath, newname);

	osgDB::writeNodeFile(*root, newfilepath/*, new osgDB::Options("WriteImageHint=WriteOut Compressor=zlib")*/);

	osgViewer::Viewer viewer;
	viewer.setSceneData(root);
	viewer.realize();
	viewer.run();

	return EXIT_SUCCESS;
}
