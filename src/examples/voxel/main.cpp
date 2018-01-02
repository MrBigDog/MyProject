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
	GeometryVisitor() :osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN)
	{}

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

static osg::Geometry* makeGeom(vx_mesh* mesh, const osg::Vec4& color)
{
	if (!mesh) return 0L;
	if (!mesh->vertices) return 0L;
	if (!mesh->indices) return 0L;

	osg::ref_ptr<osg::Vec3Array> va = new osg::Vec3Array;
	va->reserve(mesh->nvertices);
	for (unsigned int i = 0; i < mesh->nvertices; ++i)
	{
		osg::Vec3 v(mesh->vertices[i].x, mesh->vertices[i].y, mesh->vertices[i].z);
		va->push_back(v);
	}

	osg::ref_ptr<osg::Vec4Array> ca = new osg::Vec4Array;
	ca->push_back(color);

	osg::ref_ptr<osg::DrawElementsUInt> de = new osg::DrawElementsUInt(GL_TRIANGLES);
	for (unsigned int i = 0; i < mesh->nindices; ++i)
	{
		de->push_back(mesh->indices[i]);
	}

	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
	geom->setVertexArray(va);
	geom->setColorArray(ca, osg::Array::BIND_OVERALL);
	geom->addPrimitiveSet(de);

	//if (mesh->normals)
	//{
	//	osg::ref_ptr<osg::Vec3Array> na = new osg::Vec3Array;
	//	for (unsigned int i = 0; i < mesh->nnormals; ++i)
	//	{
	//		osg::Vec3 n(mesh->normals[i].x, mesh->normals[i].y, mesh->normals[i].z);
	//		na->push_back(n);
	//	}
	//	geom->setNormalArray(na, osg::Array::BIND_PER_VERTEX);
	//}

	return geom.release();
}

int main(int argc, char** argv)
{
	float res = 1.5;
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

		osg::ref_ptr<osg::Geometry> geom = makeGeom(result, meshlist[i]._color);
		osg::ref_ptr<osg::Geode> geode = new osg::Geode;
		geode->addDrawable(geom);
		root->addChild(geode);

		vx_mesh_free(result);
		vx_mesh_free(mesh);
	}

	std::string fpath = osgDB::getFilePath(filePath);
	std::string sname = osgDB::getSimpleFileName(filePath);
	std::string ssname = osgDB::getNameLessExtension(sname);
	std::string newname = ssname + "_voxel.osg";
	std::string newfilepath = osgDB::concatPaths(fpath, newname);

	osgDB::writeNodeFile(*root, newfilepath);


	osgViewer::Viewer viewer;
	viewer.setSceneData(root);
	viewer.realize();
	viewer.run();

	//std::vector<tinyobj::shape_t> shapes;
	//std::vector<tinyobj::material_t> materials;
	//std::string err;
	//bool ret = tinyobj::LoadObj(shapes, materials, err, filePath.c_str()/*argv[1]*/, NULL);

	//if (!err.empty())
	//{
	//	std::cerr << err << std::endl;
	//}

	//if (!ret)
	//{
	//	return EXIT_FAILURE;
	//}

//
//#ifdef TRIANGULATE
//	std::ofstream file("mesh_voxelized_res1.obj");
//#endif
//
//#ifdef POINT_CLOUD
//	std::ofstream file("mesh_voxelized_res.txt");
//#endif
//
//	size_t voffset = 0;
//	size_t noffset = 0;
//
//	for (size_t i = 0; i < shapes.size(); i++)
//	{
//		vx_mesh_t* mesh = vx_mesh_alloc(shapes[i].mesh.positions.size(), shapes[i].mesh.indices.size());
//		for (size_t f = 0; f < shapes[i].mesh.indices.size(); f++)
//		{
//			mesh->indices[f] = shapes[i].mesh.indices[f];
//		}
//		for (size_t v = 0; v < shapes[i].mesh.positions.size() / 3; v++)
//		{
//			mesh->vertices[v].x = shapes[i].mesh.positions[3 * v + 0];
//			mesh->vertices[v].y = shapes[i].mesh.positions[3 * v + 1];
//			mesh->vertices[v].z = shapes[i].mesh.positions[3 * v + 2];
//		}
//
//		//float res = std::atof(argv[2]);
//		//float precision = std::atof(argv[3]);
//		float res = 0.5/*std::atof(argv[2])*/;
//		float precision = 0.01/*std::atof(argv[3])*/;
//
//#ifdef TRIANGULATE
//		vx_mesh_t* result = vx_voxelize(mesh, res, res, res, precision);
//
//		printf("Number of vertices: %ld\n", result->nvertices);
//		printf("Number of indices: %ld\n", result->nindices);
//
//		if (file.is_open())
//		{
//			file << "\n";
//			file << "o " << i << "\n";
//
//			for (int j = 0; j < result->nvertices; ++j)
//			{
//				file << "v " << result->vertices[j].x << " "
//					<< result->vertices[j].y << " "
//					<< result->vertices[j].z << "\n";
//			}
//
//			for (int j = 0; j < result->nnormals; ++j)
//			{
//				file << "vn " << result->normals[j].x << " "
//					<< result->normals[j].y << " "
//					<< result->normals[j].z << "\n";
//			}
//
//			size_t max = 0;
//
//			for (int j = 0; j < result->nindices; j += 3)
//			{
//				size_t i0 = voffset + result->indices[j + 0] + 1;
//				size_t i1 = voffset + result->indices[j + 1] + 1;
//				size_t i2 = voffset + result->indices[j + 2] + 1;
//
//				max = i0 > max ? i0 : max;
//				max = i1 > max ? i1 : max;
//				max = i2 > max ? i2 : max;
//
//				file << "f ";
//
//				file << i0 << "//" << result->normalindices[j + 0] + noffset + 1 << " ";
//				file << i1 << "//" << result->normalindices[j + 1] + noffset + 1 << " ";
//				file << i2 << "//" << result->normalindices[j + 2] + noffset + 1 << "\n";
//			}
//
//			voffset += max;
//			noffset += 6;
//		}
//
//		vx_mesh_free(result);
//		vx_mesh_free(mesh);
//#endif
//
//#ifdef POINT_CLOUD
//		vx_point_cloud_t* result;
//		result = vx_voxelize_pc(mesh, res, res, res, precision);
//
//		printf("Number of vertices: %ld\n", result->nvertices);
//
//		if (file.is_open()) {
//			file << "\n";
//			for (int j = 0; j < result->nvertices; ++j) {
//				file << result->vertices[j].x << " "
//					<< result->vertices[j].y << " "
//					<< result->vertices[j].z << "\n";
//			}
//		}
//
//		vx_point_cloud_free(result);
//		vx_mesh_free(mesh);
//#endif
//	}
//	file.close();

	//system("pause");

	return EXIT_SUCCESS;
}
