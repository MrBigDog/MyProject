/************************************************************************/
/* 程序实现：BigDog                                                      */
/* 参考文章：《参数曲线集复杂区域的全自动识别算法》-谭正华                  ;*/
/************************************************************************/
#ifndef GWGEOLOGICAL_SLICEOPERATOR_H
#define GWGEOLOGICAL_SLICEOPERATOR_H 1

#include <osg/Vec3d>
#include <osg/Array>
#include <osg/Plane>
#include <osg/Group>
#include <osg/Texture2D>
#include <osg/BoundingBox>

#include <vector>
#include <algorithm>

typedef std::vector<osg::ref_ptr<osg::Vec3dArray> > Vec3dArrayVector;
typedef std::vector<osg::ref_ptr<const osg::Vec3dArray> > ConstVec3ArrayVector;

class BooleanPath;
typedef std::vector<osg::ref_ptr<BooleanPath> > BooleanPathVector;

class Junction;
typedef std::vector<osg::ref_ptr<Junction> > JunctionVector;

class Path;
typedef std::vector<osg::ref_ptr<Path> > PathVector;

struct DirectedLoop;
typedef std::vector<osg::ref_ptr<DirectedLoop> > DirectedLoopVector;

struct LayerAttribuate : public osg::Referenced
{
	LayerAttribuate(const std::string& uid, const osg::Vec4& color = osg::Vec4(1, 1, 1, 1), osg::Texture2D* tex = 0L)
		: _uid(uid)
		, _color(color)
		, _texture(tex)
	{}
	osg::Vec4 _color;
	std::string _uid;
	osg::ref_ptr<osg::Texture2D> _texture;
};

struct LayerProfile :public osg::Referenced
{
	DirectedLoopVector _loops;
	osg::ref_ptr<LayerAttribuate> _attribuate;
};

//////////////////////////////////////////////////////////////////////////
class SliceOperator
{
public:
	SliceOperator(bool isOnEarth = false);
	~SliceOperator();

	osg::Node* slice(osg::Node* geologicalModel, const osg::Plane& slicePlane, osg::Group* root = 0L);
	osg::Node* sliceGeom(osg::Geometry* geom, const osg::Plane& sliceplane, const osg::Matrixd& mat, osg::Group* testGroup = 0L);

private:
	//求交点;
	void generateJunctions(const Vec3dArrayVector & vas, const osg::Vec3d& planeNormal, JunctionVector & outJoints);

	//求最大和最小闭环;
	void generateDirectedLoops(JunctionVector & joints, const osg::Vec3d& planeNormal, DirectedLoopVector & outLoops);

	//生成剖面结点;
	osg::Node* generateProfileNode(/*osg::Group * geologicalModel*/osg::Geometry* ogeom, const DirectedLoopVector& loops, const osg::Vec3d& normal);
	//osg::Node* generateProfileNode_closed(osg::Node * geologicalModel, const DirectedLoopVector & loops, const osg::Vec3d& normal);

	void generateJunctions(Path * p1, Path * p2, JunctionVector & joints);

	void removeContinuousRepeatJunctions(Path* path);

	void generateBooleanPaths(Path* path, bool isClosedPath, BooleanPathVector &bpvector);

	void traversePath(BooleanPath * currentPath, DirectedLoopVector & mloops, const osg::Vec3d& planeNormal);

	void traverseJunction(Junction * junction, DirectedLoopVector & mloops, const osg::Vec3d& planeNormal);

	osg::Vec2Array* generateTextureCoord(const osg::Vec3d& normal, osg::ref_ptr<osg::Vec3Array> va);

private:
	bool _isOnEarth;

	double _maxCrackSize;
};


#endif // SliceOperator_h__
