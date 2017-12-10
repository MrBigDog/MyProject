#include "TunnelGenerator.h"
#include "GeneratorHelp.h"
#include "SliceOperator.h"
#include "Cutter.h"

#include <osg/Plane>
#include <osg/Depth>
#include <osg/PolygonMode>
#include <osg/StencilTwoSided>
#include <osg/LightModel>

#include <osgDB/WriteFile>

#include <iostream>

#define ON_AND_PROTECTED  osg::StateAttribute::ON | osg::StateAttribute::PROTECTED
#define OFF_AND_PROTECTED osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED

namespace
{
	struct TunnelSegmentParaBase
	{
		TunnelSegmentParaBase(const osg::Plane& sPlane, const osg::Plane& ePlane,
			const osg::Vec3d& sPosition, const osg::Vec3d& ePosition)
			: _startPlane(sPlane)
			, _endPlane(ePlane)
			, _startPostion(sPosition)
			, _endPosition(ePosition)
		{}
		bool isValid() const
		{
			if (osg::equivalent((_startPostion - _endPosition).length2(), 0.0))
			{
				return false;
			}
			double sd1 = _startPlane.distance(_startPostion);
			double sd2 = _startPlane.distance(_endPosition);
			if (osg::equivalent(sd1, 0.0) && osg::equivalent(sd2, 0.0))
			{
				return false;
			}
			double ed1 = _endPlane.distance(_startPostion);
			double ed2 = _endPlane.distance(_endPosition);
			if (osg::equivalent(ed1, 0.0) && osg::equivalent(ed2, 0.0))
			{
				return false;
			}
			return true;
		}
		osg::Plane _startPlane, _endPlane;
		osg::Vec3d _startPostion, _endPosition;
	};

	struct SquareTunnelSegmentPara :public TunnelSegmentParaBase
	{
		enum TopType { ARCH_TOPE, PLANE_TOP };
		SquareTunnelSegmentPara(const osg::Plane& sPlane, const osg::Plane& ePlane,
			const osg::Vec3d& sPosition, const osg::Vec3d& ePosition, double width, double height, TopType type = ARCH_TOPE)
			: TunnelSegmentParaBase(sPlane, ePlane, sPosition, ePosition)
			, _width(width)
			, _height(height)
			, _topeType(type)
		{}
		TopType _topeType;
		double _width, _height;
	};
	typedef std::vector<SquareTunnelSegmentPara> SquareTunnelSegmentParaVec;

	struct CircleTunnelSegmentPara :public TunnelSegmentParaBase
	{
		CircleTunnelSegmentPara(const osg::Plane& sPlane, const osg::Plane& ePlane,
			const osg::Vec3d& sPosition, const osg::Vec3d& ePosition, double radius)
			: TunnelSegmentParaBase(sPlane, ePlane, sPosition, ePosition)
			, _radius(radius)
		{}
		double _radius;
	};
	typedef std::vector<CircleTunnelSegmentPara> CircleTunnelSegmentParaVec;

	//-------------------------------------------------------------------------------------------------------------------------

	struct RemoveEmptyGeometries : public osg::NodeVisitor
	{
		RemoveEmptyGeometries()
		{
			setTraversalMode(TRAVERSE_ALL_CHILDREN);
			setNodeMaskOverride(~0);
		}
		void apply(osg::Geode& geode)
		{
			std::vector<osg::Drawable*> empties;
			for (unsigned i = 0; i < geode.getNumDrawables(); ++i)
			{
				osg::Geometry* g = geode.getDrawable(i)->asGeometry();
				if (g)
				{
					if (g->getVertexArray() == 0L || g->getNumPrimitiveSets() == 0)
						empties.push_back(g);
					else
					{
						osg::Vec3Array* va = dynamic_cast<osg::Vec3Array*>(g->getVertexArray());
						if (!va)
							empties.push_back(g);
						else if (va->empty())
						{
							empties.push_back(g);
						}
					}

				}
			}
			for (unsigned i = 0; i < empties.size(); ++i)
			{
				geode.removeDrawable(empties[i]);
			}
		}
	};

	osg::Node* optimizePlaneGeom(osg::Node*node, const osg::Vec3d& posOnPlane, const osg::Vec3d& normal, const osg::Vec3d& side)
	{
		if (!node) return 0L;
		PlaneGeometryOptimizer pgo(posOnPlane, normal, side);
		node->accept(pgo);
		return node;
	}

	void generateColumSlicePlanes(
		const osg::Vec3d& s,
		const osg::Vec3d& e,
		const osg::Plane& sp,
		const osg::Plane& ep,
		const osg::Vec3d& side,
		double r,
		double angle,
		SlicePLaneVector& out_slice_planes,
		unsigned int segmentNum = 16)
	{
		osg::ref_ptr<osg::Group> arcGroup = new osg::Group;
		const osg::Vec3d& center = s;
		double deltaAngle = angle / (double)segmentNum;

		osg::Vec3d frontDir = normalize(e - s);

		osg::Quat deltaRotQuat(deltaAngle, frontDir);
		osg::Vec3d currentDir = side;
		for (unsigned int i = 0; i < segmentNum; ++i)
		{
			osg::Vec3d nextDir = deltaRotQuat*currentDir;
			nextDir.normalize();

			osg::Vec3d p1 = center + currentDir*r/*halfWidth*/;
			osg::Vec3d p2 = center + nextDir*r/*halfWidth*/;

			osg::Vec3d d1 = p1 - center; d1.normalize();
			osg::Vec3d d2 = p2 - center; d2.normalize();
			osg::Vec3d vnormal = -(d1 + d2); vnormal.normalize();
			osg::Plane plane(vnormal, p1);

			osg::Vec3d vnormalL = p2 - p1; vnormalL.normalize();
			osg::Vec3d vnormalR = -vnormalL; vnormalR.normalize();
			osg::Plane cutPlane1(vnormalL, p1);
			osg::Plane cutPlane2(vnormalR, p2);

			osg::Polytope pt;
			pt.add(sp);
			pt.add(ep);
			pt.add(cutPlane1);
			pt.add(cutPlane2);

			SlicePlane slicePlane(plane, pt);
			out_slice_planes.push_back(slicePlane);

			currentDir = nextDir;
		}
		//ColumnGeometryOptimizer cgo(s, frontDir, side, r/*halfWidth*/, false);
		//arcGroup->accept(cgo);
		//return cgo.optimize();
		//resultRoot->addChild(cgo.optimize());
	}

	osg::Node* generateColumGeometry(osg::Node* geologicalGroup,
		const osg::Vec3d& s,
		const osg::Vec3d& e,
		const osg::Plane& sp,
		const osg::Plane& ep,
		const osg::Vec3d& side,
		/*const*/ SliceOperator& slicer,
		double r, double angle, unsigned int segmentNum = 16)
	{
		osg::ref_ptr<osg::Group> arcGroup = new osg::Group;
		const osg::Vec3d& center = s;
		double deltaAngle = angle / (double)segmentNum;

		osg::Vec3d frontDir = normalize(e - s);

		osg::Quat deltaRotQuat(deltaAngle, frontDir);
		osg::Vec3d currentDir = side;
		for (unsigned int i = 0; i < segmentNum; ++i)
		{
			osg::Vec3d nextDir = deltaRotQuat*currentDir;
			nextDir.normalize();

			osg::Vec3d p1 = center + currentDir*r/*halfWidth*/;
			osg::Vec3d p2 = center + nextDir*r/*halfWidth*/;

			osg::Vec3d d1 = p1 - center; d1.normalize();
			osg::Vec3d d2 = p2 - center; d2.normalize();
			osg::Vec3d vnormal = -(d1 + d2); vnormal.normalize();
			osg::Plane plane(vnormal, p1);

			osg::Vec3d vnormalL = p2 - p1; vnormalL.normalize();
			osg::Vec3d vnormalR = -vnormalL; vnormalR.normalize();
			osg::Plane cutPlane1(vnormalL, p1);
			osg::Plane cutPlane2(vnormalR, p2);

			osg::ref_ptr<osg::Node> profilenode = slicer.slice(geologicalGroup, plane);

			osg::Polytope pt;
			pt.add(sp);
			pt.add(ep);
			pt.add(cutPlane1);
			pt.add(cutPlane2);

			Cutter cutter(pt);
			profilenode->accept(cutter);

			arcGroup->addChild(profilenode);
			currentDir = nextDir;
		}

		//return arcGroup.release();

		ColumnGeometryOptimizer cgo(s, frontDir, side, r/*halfWidth*/, false);
		arcGroup->accept(cgo);

		return arcGroup.release();
		//resultRoot->addChild(cgo.optimize());
	}

	void generateCircleTunnelSlicePlanes(const CircleTunnelSegmentPara & tunnelPara, bool isOnEarth, SlicePLaneVector& out_slice_planes)
	{
		if (!tunnelPara.isValid()) return;

		osg::Vec3d startPos = tunnelPara._startPostion;
		osg::Vec3d endPos = tunnelPara._endPosition;
		double radius = tunnelPara._radius;

		osg::Vec3d frontDir = normalize(endPos - startPos);
		osg::Vec3d worldUpDir = osg::Vec3d(0, 0, 1);
		osg::Vec3d sideDir = normalize(worldUpDir^frontDir);
		osg::Vec3d upDir = normalize(frontDir^sideDir);

		osg::Plane startPlane(tunnelPara._startPlane);
		osg::Plane endPlane(tunnelPara._endPlane);

		generateColumSlicePlanes(
			startPos,
			endPos,
			startPlane,
			endPlane,
			sideDir,
			radius,
			PI2,
			out_slice_planes);
	}

	osg::Node* generateCircleTunnelSegment(osg::Node * geologicalGroup, const CircleTunnelSegmentPara & tunnelPara, bool isOnEarth)
	{
		if (!tunnelPara.isValid()) return 0L;
		if (!geologicalGroup) return 0L;

		osg::Vec3d startPos = tunnelPara._startPostion;
		osg::Vec3d endPos = tunnelPara._endPosition;
		double radius = tunnelPara._radius;

		osg::Vec3d frontDir = normalize(endPos - startPos);
		osg::Vec3d worldUpDir = osg::Vec3d(0, 0, 1);
		osg::Vec3d sideDir = normalize(worldUpDir^frontDir);
		osg::Vec3d upDir = normalize(frontDir^sideDir);

		osg::Plane startPlane(tunnelPara._startPlane);
		osg::Plane endPlane(tunnelPara._endPlane);

		SliceOperator slicer(isOnEarth);
		osg::ref_ptr<osg::Node> arcnode = generateColumGeometry(geologicalGroup,
			startPos,
			endPos,
			startPlane,
			endPlane,
			sideDir,
			slicer,
			radius,
			PI2);
		return arcnode.release();
	}

	void generateSquareTunnelSegmentSlicePlanes(const SquareTunnelSegmentPara & tunnelPara, bool isOnEarth, SlicePLaneVector& out_slice_planes)
	{
		if (!tunnelPara.isValid()) return;

		osg::Vec3d startPos = tunnelPara._startPostion;
		osg::Vec3d endPos = tunnelPara._endPosition;
		double width = tunnelPara._width;
		double height = tunnelPara._height;

		osg::Vec3d frontDir = normalize(endPos - startPos);
		osg::Vec3d worldUpDir = osg::Vec3d(0, 0, 1);
		osg::Vec3d sideDir = normalize(worldUpDir^frontDir);
		osg::Vec3d upDir = normalize(frontDir^sideDir);

		double halfWidth = width*0.5;
		double halfHeight = height*0.5;

		osg::Plane startPlane(tunnelPara._startPlane);
		osg::Plane endPlane(tunnelPara._endPlane);

		osg::Plane leftPlane(sideDir, startPos - sideDir*halfWidth);
		osg::Plane rightPlane(-sideDir, startPos + sideDir*halfWidth);
		osg::Plane topPlane(-upDir, startPos + upDir*halfHeight);
		osg::Plane bottomPlane(upDir, startPos - upDir*halfHeight);

		//left and right
		osg::Polytope sidePolytope;
		sidePolytope.add(startPlane);
		sidePolytope.add(endPlane);
		sidePolytope.add(topPlane);
		sidePolytope.add(bottomPlane);

		//top and bottom
		osg::Polytope updownPolytope;
		updownPolytope.add(startPlane);
		updownPolytope.add(endPlane);
		updownPolytope.add(leftPlane);
		updownPolytope.add(rightPlane);

		SlicePlane leftSlicePlane(leftPlane, sidePolytope);
		SlicePlane rightSlicePlane(rightPlane, sidePolytope);
		SlicePlane bottomSlicePlane(bottomPlane, updownPolytope);

		out_slice_planes.push_back(leftSlicePlane);
		out_slice_planes.push_back(rightSlicePlane);
		out_slice_planes.push_back(bottomSlicePlane);

		if (tunnelPara._topeType == SquareTunnelSegmentPara::ARCH_TOPE)
		{
			generateColumSlicePlanes(
				startPos + upDir*halfHeight,
				endPos + upDir*halfHeight,
				startPlane,
				endPlane,
				sideDir,
				halfWidth,
				osg::PI,
				out_slice_planes);
		}
		else if (tunnelPara._topeType == SquareTunnelSegmentPara::PLANE_TOP)
		{
			SlicePlane topSlicePlane(topPlane, updownPolytope);
			out_slice_planes.push_back(topSlicePlane);
		}
	}

	osg::Node * generateSquareTunnelSegment(osg::Node * geologicalGroup, const SquareTunnelSegmentPara & tunnelPara, bool isOnEarth)
	{
		if (!tunnelPara.isValid()) return 0L;
		if (!geologicalGroup) return 0L;

		osg::Vec3d startPos = tunnelPara._startPostion;
		osg::Vec3d endPos = tunnelPara._endPosition;
		double width = tunnelPara._width;
		double height = tunnelPara._height;

		osg::Vec3d frontDir = normalize(endPos - startPos);
		osg::Vec3d worldUpDir = osg::Vec3d(0, 0, 1);
		osg::Vec3d sideDir = normalize(worldUpDir^frontDir);
		osg::Vec3d upDir = normalize(frontDir^sideDir);

		double halfWidth = width*0.5;
		double halfHeight = height*0.5;

		osg::Plane startPlane(tunnelPara._startPlane);
		osg::Plane endPlane(tunnelPara._endPlane);

		osg::ref_ptr<osg::Group> resultRoot = new osg::Group;
		SliceOperator slicer(isOnEarth);

		osg::Plane leftPlane(sideDir, startPos - sideDir*halfWidth);
		osg::Plane rightPlane(-sideDir, startPos + sideDir*halfWidth);
		osg::Plane topPlane(-upDir, startPos + upDir*halfHeight);
		osg::Plane bottomPlane(upDir, startPos - upDir*halfHeight);

		//osg::ref_ptr<osg::Node> leftProfileNode = SLICE_CLOSED ?
		//	slicer.slice_closed(geologicalGroup, leftPlane) : slicer.slice(geologicalGroup, leftPlane);
		//osg::ref_ptr<osg::Node> rightProfileNode = SLICE_CLOSED ?
		//	slicer.slice_closed(geologicalGroup, rightPlane) : slicer.slice(geologicalGroup, rightPlane);
		//osg::ref_ptr<osg::Node> bottomProfileNode = SLICE_CLOSED ?
		//	slicer.slice_closed(geologicalGroup, bottomPlane) : slicer.slice(geologicalGroup, bottomPlane);
		osg::ref_ptr<osg::Node> leftProfileNode = slicer.slice(geologicalGroup, leftPlane);
		osg::ref_ptr<osg::Node> rightProfileNode = slicer.slice(geologicalGroup, rightPlane);
		osg::ref_ptr<osg::Node> bottomProfileNode = slicer.slice(geologicalGroup, bottomPlane);
		//left and right
		osg::Polytope sidePolytope;
		sidePolytope.add(startPlane);
		sidePolytope.add(endPlane);
		sidePolytope.add(topPlane);
		sidePolytope.add(bottomPlane);


		Cutter leftCutter(sidePolytope);
		leftProfileNode->accept(leftCutter);
		resultRoot->addChild(leftProfileNode/**/);

		Cutter rightCutter(sidePolytope);
		rightProfileNode->accept(rightCutter);
		resultRoot->addChild(rightProfileNode);

		//top and bottom
		osg::Polytope updownPolytope;
		updownPolytope.add(startPlane);
		updownPolytope.add(endPlane);
		updownPolytope.add(leftPlane);
		updownPolytope.add(rightPlane);

		Cutter bottomCutter(updownPolytope);
		bottomProfileNode->accept(bottomCutter);
		resultRoot->addChild(bottomProfileNode);

		if (tunnelPara._topeType == SquareTunnelSegmentPara::ARCH_TOPE)
		{
			osg::Node* arcnode = generateColumGeometry(geologicalGroup,
				startPos + upDir*halfHeight,
				endPos + upDir*halfHeight,
				startPlane,
				endPlane,
				sideDir,
				slicer,
				halfWidth,
				osg::PI);
			resultRoot->addChild(arcnode);
		}
		else if (tunnelPara._topeType == SquareTunnelSegmentPara::PLANE_TOP)
		{
			osg::ref_ptr<osg::Node> topProfileNode = slicer.slice(geologicalGroup, topPlane);
			Cutter topCutter(updownPolytope);
			topProfileNode->accept(topCutter);
			resultRoot->addChild(topProfileNode/*toppn*/);
		}

		RemoveEmptyGeometries reg;
		resultRoot->accept(reg);

		return resultRoot.release();
	}

	bool generateSplitPlanes(const osg::Vec3dArray* va, std::vector<osg::Plane>& planeVector)
	{
		if (va->size() < 2) return false;

		osg::Vec3d plane0Normal = va->at(1) - va->at(0); plane0Normal.normalize();
		osg::Plane plane0(plane0Normal, va->at(0));
		planeVector.push_back(plane0);
		for (osg::Vec3dArray::const_iterator it = va->begin() + 1; it != va->end() - 1; ++it)
		{
			osg::Vec3d line1 = normalize(*(it - 1) - *it);
			osg::Vec3d line2 = normalize(*(it + 1) - *it);
			osg::Vec3d planenormal;
			if (!isVec3dEqual(line1, line2, FLT_EPSILON))
			{
				planenormal = normalize(normalize(line2^line1) ^ normalize(line1 + line2));
			}
			else
			{
				planenormal = normalize(line2);
			}
			planeVector.push_back(osg::Plane(planenormal, *it));
		}
		osg::Vec3d planenNormal = normalize(*(va->end() - 2) - *(va->end() - 1));
		planeVector.push_back(osg::Plane(planenNormal, va->back()));

		return planeVector.size() > 1;
	}
}
//-----------------------------------------------------------------------------------------------------------------------

osg::Node * SquareTunelGenerator::operator()(osg::Node * geologicalGroup, const osg::Vec3dArray* va, double width, double height, TopType topType)
{
	if (va->size() < 2 || !geologicalGroup) return 0L;
	//if (geologicalGroup->getNumChildren() < 1) return 0L;

	std::vector<osg::Plane> planeVector;
	if (!generateSplitPlanes(va, planeVector))
	{
		return 0L;
	}

	SquareTunnelSegmentParaVec tunnels;
	for (unsigned int i = 0; i < planeVector.size() - 1; ++i)
	{
		osg::Plane sPlane = planeVector[i];
		if (i != 0) sPlane.flip();
		osg::Plane ePlane = planeVector[i + 1];

		SquareTunnelSegmentPara para = topType == ARC_TOP ?
			SquareTunnelSegmentPara(sPlane, ePlane, va->at(i), va->at(i + 1), width, height, SquareTunnelSegmentPara::ARCH_TOPE) :
			SquareTunnelSegmentPara(sPlane, ePlane, va->at(i), va->at(i + 1), width, height, SquareTunnelSegmentPara::PLANE_TOP);

		if (para.isValid())
		{
			tunnels.push_back(para);
		}
	}
	osg::ref_ptr<osg::Group> tunelRoot = new osg::Group;
	for (SquareTunnelSegmentParaVec::const_iterator it = tunnels.begin(); it != tunnels.end(); ++it)
	{
		osg::ref_ptr<osg::Node> tunnelSegNode = generateSquareTunnelSegment(geologicalGroup, *it, _isOnEarth);
		if (tunnelSegNode)
		{
			tunelRoot->addChild(tunnelSegNode);
			//osgDB::writeNodeFile(*tunnelSegNode, "bigdogtunneltest.osgb");
		}
	}
	tunnels.clear();
	tunelRoot->getOrCreateStateSet()->setMode(GL_NORMALIZE, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE | osg::StateAttribute::PROTECTED);

	//addLightEffect(tunelRoot->getOrCreateStateSet());

	//{//test
	//	osg::ref_ptr<osg::PolygonMode> pm = new osg::PolygonMode;
	//	pm->setMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::FILL);
	//	tunelRoot->getOrCreateStateSet()->setAttributeAndModes(pm, osg::StateAttribute::OVERRIDE | osg::StateAttribute::ON);
	//	//tunelRoot->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	//}
	return tunelRoot.release();
}
void SquareTunelGenerator::generateSlicePlanes(const osg::Vec3dArray * va, double width, double height, TopType topType, bool isOnEarth, SlicePLaneVector& out_slice_planes)
{
	if (!va) return;
	if (va->size() < 2) return;
	std::vector<osg::Plane> planeVector;
	if (!generateSplitPlanes(va, planeVector))
	{
		return;
	}

	SquareTunnelSegmentParaVec tunnels;
	for (unsigned int i = 0; i < planeVector.size() - 1; ++i)
	{
		osg::Plane sPlane = planeVector[i];
		if (i != 0) sPlane.flip();
		osg::Plane ePlane = planeVector[i + 1];

		SquareTunnelSegmentPara para = topType == ARC_TOP ?
			SquareTunnelSegmentPara(sPlane, ePlane, va->at(i), va->at(i + 1), width, height, SquareTunnelSegmentPara::ARCH_TOPE) :
			SquareTunnelSegmentPara(sPlane, ePlane, va->at(i), va->at(i + 1), width, height, SquareTunnelSegmentPara::PLANE_TOP);

		if (para.isValid())
		{
			tunnels.push_back(para);
		}
	}
	osg::ref_ptr<osg::Group> tunelRoot = new osg::Group;
	for (SquareTunnelSegmentParaVec::const_iterator it = tunnels.begin(); it != tunnels.end(); ++it)
	{
		generateSquareTunnelSegmentSlicePlanes(*it, isOnEarth, out_slice_planes);
	}
	tunnels.clear();
}
//----------------------------------------------------------------------------------------------------------------------------

osg::Node * CircleTunelGenerator::operator()(osg::Node * geologicalGroup, const osg::Vec3dArray * va, double radius)
{
	if (va->size() < 2 || !geologicalGroup) return 0L;
	//if (geologicalGroup->getNumChildren() < 1) return 0L;

	std::vector<osg::Plane> planeVector;
	if (!generateSplitPlanes(va, planeVector))
	{
		return 0L;
	}

	CircleTunnelSegmentParaVec tunnels;
	for (unsigned int i = 0; i < planeVector.size() - 1; ++i)
	{
		osg::Plane sPlane = planeVector[i];
		if (i != 0) sPlane.flip();
		osg::Plane ePlane = planeVector[i + 1];

		CircleTunnelSegmentPara para(sPlane, ePlane, va->at(i), va->at(i + 1), radius);
		if (para.isValid())
		{
			tunnels.push_back(para);
		}
	}
	osg::ref_ptr<osg::Group> tunelRoot = new osg::Group;
	for (CircleTunnelSegmentParaVec::const_iterator it = tunnels.begin(); it != tunnels.end(); ++it)
	{
		osg::ref_ptr<osg::Node> tunnelSegNode = generateCircleTunnelSegment(geologicalGroup, *it, _isOnEarth);
		if (tunnelSegNode)
		{
			tunelRoot->addChild(tunnelSegNode);
		}
	}
	tunnels.clear();

	tunelRoot->getOrCreateStateSet()->setMode(GL_NORMALIZE, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE | osg::StateAttribute::PROTECTED);

	//addLightEffect(tunelRoot->getOrCreateStateSet());
	//osg::LightModel* lm = new osg::LightModel();
	//lm->setTwoSided(true);
	//tunelRoot->getOrCreateStateSet()->setAttributeAndModes(lm);
	//{//test
	//	osg::ref_ptr<osg::PolygonMode> pm = new osg::PolygonMode;
	//	pm->setMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::FILL);
	//	tunelRoot->getOrCreateStateSet()->setAttributeAndModes(pm, osg::StateAttribute::OVERRIDE | osg::StateAttribute::ON);
	//	//tunelRoot->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	//}

	return tunelRoot.release();
}

void CircleTunelGenerator::generateSlicePlanes(const osg::Vec3dArray * va, double radius, bool isOnEarth, SlicePLaneVector& out_slice_planes)
{
	if (!va) return;
	if (va->size() < 2) return;

	std::vector<osg::Plane> planeVector;
	if (!generateSplitPlanes(va, planeVector))
	{
		return;
	}

	CircleTunnelSegmentParaVec tunnels;
	for (unsigned int i = 0; i < planeVector.size() - 1; ++i)
	{
		osg::Plane sPlane = planeVector[i];
		if (i != 0) sPlane.flip();
		osg::Plane ePlane = planeVector[i + 1];

		CircleTunnelSegmentPara para(sPlane, ePlane, va->at(i), va->at(i + 1), radius);
		if (para.isValid())
		{
			tunnels.push_back(para);
		}
	}
	osg::ref_ptr<osg::Group> tunelRoot = new osg::Group;
	for (CircleTunnelSegmentParaVec::const_iterator it = tunnels.begin(); it != tunnels.end(); ++it)
	{
		generateCircleTunnelSlicePlanes(*it, isOnEarth, out_slice_planes);
	}
	tunnels.clear();
}