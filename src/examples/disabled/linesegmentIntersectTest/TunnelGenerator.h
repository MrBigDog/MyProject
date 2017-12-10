#ifndef GENERATETUNELOPERATOR_H
#define GENERATETUNELOPERATOR_H 1

#include "SlicePlane.h"
#include <osg/Array>
#include <osg/Group>

//创建方形隧道（拱形顶/平顶）;
class SquareTunelGenerator
{
public:
	enum TopType { ARC_TOP, PLANE_TOP };

	SquareTunelGenerator(bool isOnEarth = false) :_isOnEarth(isOnEarth) {}
	osg::Node* operator()(osg::Node* geologicalGroup, const osg::Vec3dArray* va, double width, double height, TopType topType = ARC_TOP);

	//生成裁剪面;
	static void generateSlicePlanes(const osg::Vec3dArray* va, double width, double height, TopType topType, bool isOnEarth, SlicePLaneVector& out_slice_planes);

private:
	bool _isOnEarth;
};

//创建圆形隧道;
class CircleTunelGenerator
{
public:
	CircleTunelGenerator(bool isOnEarth = false) :_isOnEarth(isOnEarth) {}
	osg::Node* operator()(osg::Node* geologicalGroup, const osg::Vec3dArray* va, double radius);

	//生成裁剪面;
	static void generateSlicePlanes(const osg::Vec3dArray* va, double radius, bool isOnEarth, SlicePLaneVector& out_slice_planes);

private:
	bool _isOnEarth;
};


#endif // GenerateTunelOperator_h__
