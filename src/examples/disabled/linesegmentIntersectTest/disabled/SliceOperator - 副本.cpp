#include "SliceOperator.h"
#include <osg/Vec3>
#include <iostream>
#include <gwUtil/Math.h>

//#include "wykobi/wykobi.hpp"
//#include "wykobi/wykobi_algorithm.hpp"

#include <assert.h>

using namespace gwUtil;

////不准确，邪门;
//bool isLinesegmentIntersect2(const osg::Vec3d& s1, const osg::Vec3d&e1, const osg::Vec3d& s2, const osg::Vec3d&e2, osg::Vec3d& hit)
//{
//	wykobi::point3d<wykobi::Float> p1;
//	p1(0) = s1[0]; p1(1) = s1[1]; p1(2) = s1[2];
//	wykobi::point3d<wykobi::Float> p2;
//	p2(0) = e1[0]; p2(1) = e1[1]; p2(2) = e1[2];
//	wykobi::segment3d seg1;
//	seg1[0] = p1;
//	seg1[1] = p2;
//
//	wykobi::point3d<wykobi::Float> p3;
//	p3(0) = s2[0]; p3(1) = s2[1]; p3(2) = s2[2];
//	wykobi::point3d<wykobi::Float> p4;
//	p4(0) = e2[0]; p4(1) = e2[1]; p4(2) = e2[2];
//	wykobi::segment3d seg2;
//	seg2[0] = p3;
//	seg2[1] = p4;
//
//	std::vector<wykobi::segment3d> segment_list;
//	segment_list.push_back(seg1);
//	segment_list.push_back(seg2);
//
//	std::vector<wykobi::point3d<wykobi::Float> > intersection_list;
//
//	wykobi::algorithm::naive_group_intersections<wykobi::segment3d >
//		(
//			segment_list.begin(),
//			segment_list.end(),
//			std::back_inserter(intersection_list)
//			);
//	if (intersection_list.empty())
//	{
//		return false;
//	}
//
//	wykobi::point3d<wykobi::Float> res = intersection_list[0];
//	if (std::isinf(res.x) || std::isinf(res.x) || std::isinf(res.x))
//	{
//		return false;
//	}
//
//	hit = osg::Vec3d(intersection_list[0].x, intersection_list[0].y, intersection_list[0].z);
//	return true;
//}
////////////////////////////////////////////////////////////////////////////
//#define SMALL_NUM   DBL_EPSILON // anything that avoids division overflow
//// dot product (3D) which allows vector operations in arguments
//#define dot(u,v)   ((u).x() * (v).x() + (u).y() * (v).y() + (u).z() * (v).z())
//#define norm(v)    sqrt(dot(v,v))  // norm = length of  vector
//#define d(u,v)     norm(u-v)        // distance = norm of difference
//#define abs(x)     ((x) >= 0 ? (x) : -(x))   //  absolute value
//
//struct Segment
//{
//	Segment(const osg::Vec3d& v0, const osg::Vec3d& v1)
//		:P0(v0), P1(v1)
//	{}
//
//	osg::Vec3d P0, P1;
//};
//
//
//bool isSegmentsIntersect(Segment S1, Segment S2, osg::Vec3d& intersectPoint)
//{
//	osg::Vec3d   u = S1.P1 - S1.P0;
//	osg::Vec3d   v = S2.P1 - S2.P0;
//	osg::Vec3d   w = S1.P0 - S2.P0;
//	float    a = dot(u, u);         // always >= 0
//	float    b = dot(u, v);
//	float    c = dot(v, v);         // always >= 0
//	float    d = dot(u, w);
//	float    e = dot(v, w);
//	float    D = a*c - b*b;        // always >= 0
//	float    sc, sN, sD = D;       // sc = sN / sD, default sD = D >= 0
//	float    tc, tN, tD = D;       // tc = tN / tD, default tD = D >= 0
//
//								   // compute the line parameters of the two closest points
//	if (D < SMALL_NUM) { // the lines are almost parallel
//		sN = 0.0;         // force using point P0 on segment S1
//		sD = 1.0;         // to prevent possible division by 0.0 later
//		tN = e;
//		tD = c;
//	}
//	else {                 // get the closest points on the infinite lines
//		sN = (b*e - c*d);
//		tN = (a*e - b*d);
//		if (sN < 0.0) {        // sc < 0 => the s=0 edge is visible
//			sN = 0.0;
//			tN = e;
//			tD = c;
//		}
//		else if (sN > sD) {  // sc > 1  => the s=1 edge is visible
//			sN = sD;
//			tN = e + b;
//			tD = c;
//		}
//	}
//
//	if (tN < 0.0) {            // tc < 0 => the t=0 edge is visible
//		tN = 0.0;
//		// recompute sc for this edge
//		if (-d < 0.0)
//			sN = 0.0;
//		else if (-d > a)
//			sN = sD;
//		else {
//			sN = -d;
//			sD = a;
//		}
//	}
//	else if (tN > tD) {      // tc > 1  => the t=1 edge is visible
//		tN = tD;
//		// recompute sc for this edge
//		if ((-d + b) < 0.0)
//			sN = 0;
//		else if ((-d + b) > a)
//			sN = sD;
//		else {
//			sN = (-d + b);
//			sD = a;
//		}
//	}
//	// finally do the division to get sc and tc
//	sc = (abs(sN) < SMALL_NUM ? 0.0 : sN / sD);
//	tc = (abs(tN) < SMALL_NUM ? 0.0 : tN / tD);
//
//	// get the difference of the two closest points
//	osg::Vec3d dP = w + (u*sc) - (v*tc);  // =  S1(sc) - S2(tc)
//	if (norm(dP) < SMALL_NUM)// return the closest distance
//	{
//		intersectPoint = (u*sc);
//		return true;
//	}
//
//	return false;
//}
////////////////////////////////////////////////////////////////////////////


//bool isPointInLineSegment(const osg::Vec3d& s, const osg::Vec3d& e, const osg::Vec3d& p)
//{
//	if ((osg::equivalent(s[0], p[0]) && osg::equivalent(s[1], p[1]) && osg::equivalent(s[2], p[2])) ||
//		(!osg::equivalent(e[0], p[0]) && !osg::equivalent(e[1], p[1]) && !osg::equivalent(e[2], p[2])))
//	{
//		return true;
//	}
//	double disse = (e - s).length();
//	if ((p - s).length() > disse || (p - e).length() > disse)
//	{
//		return false;
//	}
//	return true;
//}


////double t;
////osg::Vec3d p = s1 + v1*t;
////osg::Vec3d ps2 = p - s2;
////osg::Vec3d res = ps2 ^ v2;
//
////(v3 + v1*t) ^ v2;
////v1^v2*t + v3^v2;
//
////(v3[1] + v1[1] * t)*v2[2] - (v3[2] + v1[2] * t)*v2[1];
////求两条线段的交点;
//bool isLinesegmentIntersect(const osg::Vec3d& s1, const osg::Vec3d&e1, const osg::Vec3d& s2, const osg::Vec3d&e2, osg::Vec3d& hit)
//{
//	if ((osg::equivalent(s1[0], s2[0]) && osg::equivalent(s1[1], s2[1]) && osg::equivalent(s1[2], s2[2])) ||
//		(osg::equivalent(s1[0], e2[0]) && osg::equivalent(s1[1], e2[1]) && osg::equivalent(s1[2], e2[2])))
//	{
//		hit = s1;
//		return true;
//	}
//	if ((osg::equivalent(e1[0], s2[0]) && osg::equivalent(e1[1], s2[1]) && osg::equivalent(e1[2], s2[2])) ||
//		(osg::equivalent(e1[0], e2[0]) && osg::equivalent(e1[1], e2[1]) && osg::equivalent(e1[2], e2[2])))
//	{
//		hit = e1;
//		return true;
//	}
//
//	if (!osg::equivalent(osg::computeVolume(s1, e1, s2, e2), 0.0f))
//	{
//		return false;
//	}
//
//	osg::Vec3d v1 = e1 - s1;
//	osg::Vec3d v2 = e2 - s2;
//	osg::Vec3d v3 = s1 - s2;
//	osg::Vec3d v4 = -(v3^v2);
//	osg::Vec3d v5 = (v1^v2);
//
//	//有v5*t=v4
//	double t;
//	if (!osg::equivalent(v5[0], 0.0))
//	{
//		t = v4[0] / v5[0];
//	}
//	else if (!osg::equivalent(v5[1], 0.0))
//	{
//		t = v4[1] / v5[1];
//	}
//	else if (!osg::equivalent(v5[2], 0.0))
//	{
//		t = v4[2] / v5[2];
//	}
//	else//平行,（黏连或者不相交）;
//	{
//		if (isPointInLineSegment(s1, e1, s2))
//		{
//			hit = s2;
//			return true;
//		}
//		else if (isPointInLineSegment(s1, e1, e2))
//		{
//			hit = e2;
//			return true;
//		}
//		return false;
//	}
//
//	if (t > 1.0 || t < 0.0)
//		return false;
//
//	osg::Vec3d p = s1 + v1*t;
//	if (!isPointInLineSegment(s2, e2, p))
//	{
//		return false;
//	}
//
//	hit = p;
//	return true;
//}

//////////////////////////////////////////////////////////////////////////
// from 《 Real-Time Collision Detection 》;

#define EPSILON FLT_EPSILON

float Clamp(float n, float min, float max)
{
	if (n < min) return min;
	if (n > max) return max;
	return n;
}
// Computes closest points C1 and C2 of S1(s)=P1+s*(Q1-P1) and
// S2(t)=P2+t*(Q2-P2), returning s and t. Function result is squared
// distance between between S1(s) and S2(t)
bool isSegmentsIntersect3(const osg::Vec3d& p1, const osg::Vec3d& q1, const osg::Vec3d& p2, const osg::Vec3d& q2, osg::Vec3d &p)
{
	float s, t;
	osg::Vec3d c1, c2;

	osg::Vec3d d1 = q1 - p1; // Direction vector of segment S1
	osg::Vec3d d2 = q2 - p2; // Direction vector of segment S2
	osg::Vec3d r = p1 - p2;
	float a = d1* d1; // Squared length of segment S1, always nonnegative
	float e = d2* d2; // Squared length of segment S2, always nonnegative
	float f = d2* r;
	float res_dis = FLT_MAX;
	// Check if either or both segments degenerate into points
	if (a <= EPSILON && e <= EPSILON)
	{
		// Both segments degenerate into points
		s = t = 0.0f;
		c1 = p1;
		c2 = p2;
		res_dis = (c1 - c2)*(c1 - c2);
	}
	else
	{
		if (a <= EPSILON) 
		{
			// First segment degenerates into a point
			s = 0.0f;
			t = f / e; // s = 0 => t = (b*s + f) / e = f / e
			t = Clamp(t, 0.0f, 1.0f);
		}
		else 
		{
			float c = (d1* r);
			if (e <= EPSILON) 
			{
				// Second segment degenerates into a point
				t = 0.0f;
				s = Clamp(-c / a, 0.0f, 1.0f); // t = 0 => s = (b*t - c) / a = -c / a
			}
			else 
			{
				// The general nondegenerate case starts here
				float b = (d1* d2);
				float denom = a*e - b*b; // Always nonnegative
										 // If segments not parallel, compute closest point on L1 to L2 and
										 // clamp to segment S1. Else pick arbitrary s (here 0)
				if (denom != 0.0f)
				{
					s = Clamp((b*f - c*e) / denom, 0.0f, 1.0f);
				}
				else s = 0.0f;
				// Compute point on L2 closest to S1(s) using
				// t = Dot((P1 + D1*s) - P2,D2) / Dot(D2,D2) = (b*s + f) / e
				t = (b*s + f) / e;
				// If t in [0,1] done. Else clamp t, recompute s for the new value
				// of t using s = Dot((P2 + D2*t) - P1,D1) / Dot(D1,D1)= (t*b - c) / a
				// and clamp s to [0, 1]
				if (t < 0.0f) 
				{
					t = 0.0f;
					s = Clamp(-c / a, 0.0f, 1.0f);
				}
				else if (t > 1.0f) 
				{
					t = 1.0f;
					s = Clamp((b - c) / a, 0.0f, 1.0f);
				}
			}
		}
		c1 = p1 + d1 * s;
		c2 = p2 + d2 * t;
		res_dis = (c1 - c2)*(c1 - c2);
	}

	if (res_dis < EPSILON)
	{
		p = c1;
		return true;
	}

	return false;
}
//////////////////////////////////////////////////////////////////////////

osg::Vec3Array* getSubArray(const osg::Vec3Array* va, unsigned int first, unsigned int last, bool isReverse)
{
	if (last < first) return NULL;

	unsigned int maxIndex = va->size() - 1;
	if (last > maxIndex) last = maxIndex;

	osg::ref_ptr<osg::Vec3Array> subva = new osg::Vec3Array;
	if (!isReverse)
	{
		for (unsigned int j = first; j < last; ++j)
		{
			subva->push_back(va->at(j));
		}
	}
	else
	{
		for (unsigned int j = last; j > first; --j)
		{
			subva->push_back(va->at(j));
		}
	}

	return subva.release();
}

osg::Vec3Array* getReverseArray(const osg::Vec3Array* va)
{
	osg::ref_ptr<osg::Vec3Array> reVa = new osg::Vec3Array;
	for (unsigned int i = va->size() - 1; i >= 0; i--)
	{
		reVa->push_back(va->at(i));
	}
	return reVa;
}

//////////////////////////////////////////////////////////////////////////
SliceOperator::SliceOperator()
{}

SliceOperator::~SliceOperator()
{}

bool compairJunction(Path::JunctionPair& j1, Path::JunctionPair& j2)
{
	return j1.first < j2.first;
}

Junction* getJunctionAt(const osg::Vec3d& p, const JunctionVector& joints)
{
	for (JunctionVector::const_iterator it = joints.begin(); it != joints.end(); ++it)
	{
		const osg::Vec3 p0 = (*it)->_position;
		const osg::Vec3 p1 = p;
		if (osg::equivalent(p0[0], p1[0]) && osg::equivalent(p0[1], p1[1]) && osg::equivalent(p0[2], p1[2]))
		{
			return (*it);
		}
	}
	return NULL;
}

void SliceOperator::getJunctions(Path* p1, Path* p2, JunctionVector& joints)
{
	for (unsigned int i = 0; i < p1->_va->size() - 1; ++i)
	{
		osg::Vec3d s1 = p1->_va->at(i);
		osg::Vec3d e1 = p1->_va->at(i + 1);
		for (unsigned int j = 0; j < p2->_va->size() - 1; ++j)
		{
			osg::Vec3d s2 = p2->_va->at(j);
			osg::Vec3d e2 = p2->_va->at(j + 1);
			osg::Vec3d p;
			if (isSegmentsIntersect3(s1, e1, s2, e2, p))
			{
				if (p.isNaN())
					continue;

				//查找在p点的junction，如果存在，直接用，不存在新建;
				osg::ref_ptr<Junction> joint = getJunctionAt(p, joints);
				if (joint == NULL)
				{
					joint = new Junction(p);
					joints.push_back(joint);
				}
				p1->addJunction(i, joint);
				p2->addJunction(j, joint);
			}
		}
	}
}

void SliceOperator::getJunctions(const VaVector& vas, JunctionVector& out_joints, const osg::Vec3d& planeNormal)
{
	PathVector paths;
	for (VaVector::const_iterator it = vas.begin(); it != vas.end(); ++it)
	{
		osg::ref_ptr<Path> path = new Path(*it);
		paths.push_back(path);
	}

	JunctionVector joints;
	for (unsigned int i = 0; i < paths.size(); ++i)
	{
		Path* path1 = paths[i];
		for (unsigned int j = i + 1; j < paths.size(); ++j)
		{
			Path* path2 = paths[j];
			if (!path1->_bb.intersects(path2->_bb))
			{
				continue;
			}
			getJunctions(path1, path2, joints);
		}
	}

	//path中的junction为空的时候说明是闭环;
	for (PathVector::iterator it = paths.begin(); it != paths.end(); ++it)
	{
		Path* path = *it;
		if (!path->_junctions.empty()) continue;

		osg::ref_ptr<Junction> junction0 = new Junction(path->_va->at(0));
		osg::ref_ptr<Junction> junction1 = new Junction(path->_va->at(path->_va->size() - 1));
		path->addJunction(0, junction0);
		path->addJunction(path->_va->size() - 1, junction1);
		joints.push_back(junction0);
		joints.push_back(junction1);
	}

	//生成关联路径;
	BooleanPathVector bpvector;
	for (PathVector::const_iterator it = paths.begin(); it != paths.end(); ++it)
	{
		Path* path = *it;
		std::sort(path->_junctions.begin(), path->_junctions.end(), compairJunction);

		if (path->_junctions.size() < 2)
		{
			continue;
		}

		for (unsigned int i = 0; i < path->_junctions.size() - 1; ++i)
		{
			Junction* currentJunction = path->_junctions[i].second;
			Junction* nextJunction = path->_junctions[i + 1].second;

			unsigned int currentVertexIndex = path->_junctions[i].first;
			unsigned int nextVertexIndex = path->_junctions[i + 1].first;

			if (currentVertexIndex == nextVertexIndex)
			{
				continue;
			}

			osg::ref_ptr<osg::Vec3Array> va0 = getSubArray(path->_va, currentVertexIndex, nextVertexIndex, false);
			osg::ref_ptr<osg::Vec3Array> va1 = getSubArray(path->_va, currentVertexIndex, nextVertexIndex, true);

			if (va0 == NULL || va0->empty() || va1 == NULL || va1->empty())
			{
				continue;
			}

			osg::ref_ptr<BooleanPath> bp0 = new BooleanPath(va0);
			bp0->setStartJunction(currentJunction);
			bp0->setEndJunction(nextJunction);
			osg::ref_ptr<BooleanPath> bp1 = new BooleanPath(va1);
			bp1->setStartJunction(nextJunction);
			bp1->setEndJunction(currentJunction);
			bp0->setTwinPath(bp1);
			bp1->setTwinPath(bp0);

			currentJunction->addPath(bp0);
			nextJunction->addPath(bp1);

			bpvector.push_back(bp0);
			bpvector.push_back(bp1);
		}
	}

	for (BooleanPathVector::const_iterator it = bpvector.begin(); it != bpvector.end(); ++it)
	{
		if ((*it)->_startJunction)
		{
			out_joints.push_back((*it)->_startJunction);
		}
		if ((*it)->_endJunction)
		{
			out_joints.push_back((*it)->_endJunction);
		}
	}
}

void removeBooleanPathFromJunction(BooleanPath* fpath)
{
	Junction* sjunction = fpath->_startJunction;

	BooleanPathVector::iterator it =
		std::find(sjunction->_paths.begin(), sjunction->_paths.end(), fpath);
	if (it == sjunction->_paths.end())
	{
		return;
	}
	sjunction->_paths.erase(it);
}

double getClockwiseAngle(const osg::Vec3d& v1, const osg::Vec3d& v2, const osg::Vec3d& n)
{
	osg::Vec3d nv1 = v1; nv1.normalize();
	osg::Vec3d nv2 = v2; nv2.normalize();

	double f = (nv1^nv2)*n;
	if (f <= 0)
	{
		return acos(nv1*nv2);
	}
	else
	{
		return 2.0*osg::PI - acos(nv1*nv2);
	}
}

BooleanPath* getClockwiseNearstBooleanPath(const BooleanPathVector& paths, const BooleanPath* path, const osg::Vec3d& c, const osg::Vec3d&n)
{
	BooleanPath* resultPath = 0L;
	double minClockwiseAngle = FLT_MAX;
	osg::Vec3d dir0 = path->_va->at(0) - c; dir0.normalize();
	for (BooleanPathVector::const_iterator it = paths.begin(); it != paths.end(); ++it)
	{
		if (path == *it) continue;

		osg::Vec3d dir1 = (*it)->_va->at(0) - c; dir1.normalize();
		double cloclwiseangle = getClockwiseAngle(dir0, dir1, n);
		if (minClockwiseAngle > cloclwiseangle)
		{
			minClockwiseAngle = cloclwiseangle;
			resultPath = *it;
		}
	}
	return resultPath;
}

void SliceOperator::traversePath(BooleanPath* currentPath, MaxLoopVector& mloops, const osg::Vec3d& planeNormal)
{
	if (!currentPath)
	{
		return;
	}

	Junction* endjunction = currentPath->_endJunction;
	if (!endjunction)
	{
		//error
		return;
	}

	if (endjunction->_paths.empty())
	{
		//删除相关path;
		BooleanPath* prePath = currentPath->_previous;
		removeBooleanPathFromJunction(currentPath);
		traversePath(prePath, mloops, planeNormal);

		return;
	}

	BooleanPath* twinOfCurrentPath = currentPath->_twin;
	if (!twinOfCurrentPath)
	{
		//error
		return;
	}

	BooleanPath* nextPath = getClockwiseNearstBooleanPath(endjunction->_paths, twinOfCurrentPath, endjunction->_position, planeNormal);
	if (!nextPath)
	{
		//删除相关path;
		BooleanPath* prePath = currentPath->_previous;
		removeBooleanPathFromJunction(currentPath);
		traversePath(prePath, mloops, planeNormal);
		return;
	}

	if (nextPath->_previous && nextPath->_previous != NULL)
	{
		//构成了一个环;
		//删除相关path;
		osg::ref_ptr<MaxLoop> maxloop = new MaxLoop;

		BooleanPath* fpath = nextPath;
		maxloop->_paths.push_back(nextPath);

		while (fpath != nextPath->_previous)
		{
			maxloop->_paths.push_back(fpath->_next);
			removeBooleanPathFromJunction(fpath);
			fpath = fpath->_next;
		}
		removeBooleanPathFromJunction(fpath->_next);

		mloops.push_back(maxloop);

		return;
	}

	nextPath->setPreviousPath(currentPath);
	currentPath->setNextPath(nextPath);
	traversePath(nextPath, mloops, planeNormal);
}

void SliceOperator::traverseJunction(Junction* junction, MaxLoopVector& mloops, const osg::Vec3d& planeNormal)
{
	if (junction->_paths.empty())
	{
		return;
	}

	BooleanPath* currentPath = *(junction->_paths.begin());
	if (!currentPath)
	{
		return;
	}
	traversePath(currentPath, mloops, planeNormal);
}

void SliceOperator::traverseJunctions(JunctionVector& joints, MaxLoopVector& mloops, const osg::Vec3d& planeNormal)
{
	for (JunctionVector::iterator it = joints.begin(); it != joints.end(); ++it)
	{
		Junction* junction = *it;
		if (!junction)
		{
			continue;
		}
		traverseJunction(junction, mloops, planeNormal);
	}
}
