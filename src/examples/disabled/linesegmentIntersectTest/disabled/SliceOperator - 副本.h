#ifndef GWGEOLOGICAL_SLICEOPERATOR_H
#define GWGEOLOGICAL_SLICEOPERATOR_H 1

#include "Tree/st_tree.h"

#include <osg/Vec3d>
#include <osg/Array>
#include <osg/Plane>
#include <osg/Group>
#include <osg/ref_ptr>
#include <osg/Referenced>
#include <osg/BoundingBox>

#include <vector>
#include <algorithm>

typedef std::vector<osg::ref_ptr<osg::Vec3Array> > Vec3ArrayVector;
typedef std::vector<osg::ref_ptr<const osg::Vec3Array> > ConstVec3ArrayVector;

class BooleanPath;
typedef std::vector<osg::ref_ptr<BooleanPath> > BooleanPathVector;

class Junction;
typedef std::vector<osg::ref_ptr<Junction> > JunctionVector;

class Path;
typedef std::vector<osg::ref_ptr<Path> > PathVector;

struct DirectedLoop;
typedef std::vector<osg::ref_ptr<DirectedLoop> > DirectedLoopVector;


//typedef st_tree::tree<osg::ref_ptr<DirectedLoop> > DirectedLoopTree;
//typedef st_tree::tree<osg::ref_ptr<DirectedLoop> >::iterator TreeIterator;
//typedef st_tree::tree<osg::ref_ptr<DirectedLoop> >::node_type TreeNode;
//typedef TreeNode::iterator TreeNodeIterator;

//////////////////////////////////////////////////////////////////////////
class SliceOperator
{
public:
	SliceOperator();
	~SliceOperator();

	//求交点;
	void generateJunctions(const Vec3ArrayVector & vas, const osg::Vec3d& planeNormal, JunctionVector & outJoints);

	//求最大-最小闭环;
	void generateDirectedLoops(JunctionVector & joints, const osg::Vec3d& planeNormal, DirectedLoopVector & outLoops);


	//void generateDirectedLoopTree(const DirectedLoopVector& loops, DirectedLoopTree& tree);

	//生成剖面结点;
	osg::Node* generateProfileNode(const DirectedLoopVector& loops);


	void slice(osg::Group* geologicalModel, const osg::Plane& slicePlane);


	//for test
	osg::Node* generateJunctionPointNode(const Vec3ArrayVector& vaVector);




private:
	void generateJunctions(Path * p1, Path * p2, JunctionVector & joints);

	void generateBooleanPaths(Path* path, bool isClosedPath, BooleanPathVector &bpvector);

	void traversePath(BooleanPath * currentPath, DirectedLoopVector & mloops, const osg::Vec3d& planeNormal);

	void traverseJunction(Junction * junction, DirectedLoopVector & mloops, const osg::Vec3d& planeNormal);

private:
	osg::Vec3d _clipPlaneNormal;
};


#endif // SliceOperator_h__
