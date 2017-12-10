#ifndef GENERATETUNELOPERATOR_H
#define GENERATETUNELOPERATOR_H 1

#include "SlicePlane.h"
#include <osg/Array>
#include <osg/Group>

//����������������ζ�/ƽ����;
class SquareTunelGenerator
{
public:
	enum TopType { ARC_TOP, PLANE_TOP };

	SquareTunelGenerator(bool isOnEarth = false) :_isOnEarth(isOnEarth) {}
	osg::Node* operator()(osg::Node* geologicalGroup, const osg::Vec3dArray* va, double width, double height, TopType topType = ARC_TOP);

	//���ɲü���;
	static void generateSlicePlanes(const osg::Vec3dArray* va, double width, double height, TopType topType, bool isOnEarth, SlicePLaneVector& out_slice_planes);

private:
	bool _isOnEarth;
};

//����Բ�����;
class CircleTunelGenerator
{
public:
	CircleTunelGenerator(bool isOnEarth = false) :_isOnEarth(isOnEarth) {}
	osg::Node* operator()(osg::Node* geologicalGroup, const osg::Vec3dArray* va, double radius);

	//���ɲü���;
	static void generateSlicePlanes(const osg::Vec3dArray* va, double radius, bool isOnEarth, SlicePLaneVector& out_slice_planes);

private:
	bool _isOnEarth;
};


#endif // GenerateTunelOperator_h__
