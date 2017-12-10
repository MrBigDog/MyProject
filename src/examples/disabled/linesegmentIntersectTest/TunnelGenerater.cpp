#include "TunnelGenerator.h"
#include "GeneratorHelp.h"
#include "SliceOperator.h"
#include "Cutter.h"

#include <osg/Plane>
#include <osg/PolygonMode>

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
		enum TopType { ARCH_TOPE, PLANE_TOP, NO_TYPE };
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
	osg::Node* optimizePlaneGeom(osg::Node*node, const osg::Vec3d& posOnPlane, const osg::Vec3d& normal, const osg::Vec3d& side)
	{
		PlaneGeometryOptimizer pgo(posOnPlane, normal, side);
		node->accept(pgo);
		return pgo.optimize();
	}

	osg::Node* generateColumGeometry(osg::Group* geologicalGroup,
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

			osg::ref_ptr<osg::Node> profilenode = slicer.slice_closed(geologicalGroup, plane);
			osg::Polytope pt;
			pt.add(sp);
			pt.add(ep);
			pt.add(cutPlane1);
			pt.add(cutPlane2);

			Cutter cutter(pt);
			profilenode->accept(cutter);
			arcGroup->addChild(cutter.getResult());
			currentDir = nextDir;
		}
		ColumnGeometryOptimizer cgo(s, frontDir, side, r/*halfWidth*/, false);
		arcGroup->accept(cgo);

		return cgo.optimize();
		//resultRoot->addChild(cgo.optimize());
	}

	osg::Node* generateCircleTunnelSegment(osg::Group * geologicalGroup, const CircleTunnelSegmentPara & tunnelPara)
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

		SliceOperator slicer;
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

	osg::Node * generateSquareTunnelSegment(osg::Group * geologicalGroup, const SquareTunnelSegmentPara & tunnelPara)
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
		SliceOperator slicer;

		osg::Plane leftPlane(sideDir, startPos - sideDir*halfWidth);
		osg::Plane rightPlane(-sideDir, startPos + sideDir*halfWidth);
		osg::Plane topPlane(-upDir, startPos + upDir*halfHeight);
		osg::Plane bottomPlane(upDir, startPos - upDir*halfHeight);

		osg::ref_ptr<osg::Node> leftProfileNode = slicer.slice_closed(geologicalGroup, leftPlane);
		osg::ref_ptr<osg::Node> rightProfileNode = slicer.slice_closed(geologicalGroup, rightPlane);
		osg::ref_ptr<osg::Node> bottomProfileNode = slicer.slice_closed(geologicalGroup, bottomPlane);

		//left and right
		osg::Polytope sidePolytope;
		sidePolytope.add(startPlane);
		sidePolytope.add(endPlane);
		sidePolytope.add(topPlane);
		sidePolytope.add(bottomPlane);

		Cutter leftCutter(sidePolytope);
		leftProfileNode->accept(leftCutter);
		resultRoot->addChild(optimizePlaneGeom(leftCutter.getResult(), startPos - sideDir*halfWidth, sideDir, frontDir));

		Cutter rightCutter(sidePolytope);
		rightProfileNode->accept(rightCutter);
		resultRoot->addChild(optimizePlaneGeom(rightCutter.getResult(), startPos + sideDir*halfWidth, -sideDir, frontDir));

		//top and bottom
		osg::Polytope updownPolytope;
		updownPolytope.add(startPlane);
		updownPolytope.add(endPlane);
		updownPolytope.add(leftPlane);
		updownPolytope.add(rightPlane);

		Cutter bottomCutter(updownPolytope);
		bottomProfileNode->accept(bottomCutter);
		resultRoot->addChild(optimizePlaneGeom(bottomCutter.getResult(), startPos - upDir*halfHeight, upDir, frontDir));

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
			osg::ref_ptr<osg::Node> topProfileNode = slicer.slice_closed(geologicalGroup, topPlane);
			Cutter topCutter(updownPolytope);
			topProfileNode->accept(topCutter);
			resultRoot->addChild(optimizePlaneGeom(topCutter.getResult(), startPos + upDir*halfHeight, upDir, frontDir));
		}
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
SquareTunelGenerator::SquareTunelGenerator(bool isOnEarth)
{ }

osg::Node * SquareTunelGenerator::operator()(osg::Group * geologicalGroup, const osg::Vec3dArray* va, double width, double height, TopeType topType)
{
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
		osg::ref_ptr<osg::Node> tunnelSegNode = generateSquareTunnelSegment(geologicalGroup, *it);
		if (tunnelSegNode)
		{
			tunelRoot->addChild(tunnelSegNode);
		}
	}
	tunnels.clear();

	{//test
		osg::ref_ptr<osg::PolygonMode> pm = new osg::PolygonMode;
		pm->setMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::FILL);
		tunelRoot->getOrCreateStateSet()->setAttributeAndModes(pm, osg::StateAttribute::OVERRIDE | osg::StateAttribute::ON);
		//tunelRoot->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	}

	return tunelRoot.release();
}
//----------------------------------------------------------------------------------------------------------------------------

osg::Node * CircleTunelGenerator::operator()(osg::Group * geologicalGroup, const osg::Vec3dArray * va, double radius)
{
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
		osg::ref_ptr<osg::Node> tunnelSegNode = generateCircleTunnelSegment(geologicalGroup, *it);
		if (tunnelSegNode)
		{
			tunelRoot->addChild(tunnelSegNode);
		}
	}
	tunnels.clear();

	{//test
		osg::ref_ptr<osg::PolygonMode> pm = new osg::PolygonMode;
		pm->setMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::FILL);
		tunelRoot->getOrCreateStateSet()->setAttributeAndModes(pm, osg::StateAttribute::OVERRIDE | osg::StateAttribute::ON);
		//tunelRoot->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	}

	return tunelRoot.release();
}