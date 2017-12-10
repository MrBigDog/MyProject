#include "SliceOperator.h"

#include <osg/Vec3>
#include <osg/Point>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/PolygonMode>
#include <osgUtil/Tessellator>
#include <osgUtil/PlaneIntersector>

#include <iostream>
#include <assert.h>

////不准确，邪门了;可能因为精度问题，过于精确了。
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

#define GEO_EPSILON 0.00001f

namespace
{
	//得到顺时针方向的夹角;
	static double getClockwiseAngle(const osg::Vec3d & v1, const osg::Vec3d & v2, const osg::Vec3d & n)
	{
		if ((osg::equivalent(v1[0], 0.0) && osg::equivalent(v1[1], 0.0) && osg::equivalent(v1[2], 0.0)) ||
			(osg::equivalent(v2[0], 0.0) && osg::equivalent(v2[1], 0.0) && osg::equivalent(v2[2], 0.0)))
		{
			return 0.0;
		}

		osg::Quat rot;
		rot.makeRotate(v1, v2);

		double angle;
		rot.getRotate(angle, osg::Vec3d(n));

		if (std::isnan(angle))
		{
			return 0.0;
		}

		double f = (v1^v2)*n;

		if (f <= 0)
			return angle;
		else
			return 2.0*osg::PI - angle;

		return angle;
	}


	// from 《 Real-Time Collision Detection 》;
#define EPSILON FLT_EPSILON

	static double Clamp(double n, double min, double max)
	{
		if (n < min) return min;
		if (n > max) return max;
		return n;
	}

	static bool isSegmentsIntersect3(const osg::Vec3d& p1, const osg::Vec3d& q1, const osg::Vec3d& p2, const osg::Vec3d& q2, osg::Vec3d &p)
	{
		double s, t;
		osg::Vec3d c1, c2;

		osg::Vec3d d1 = q1 - p1; // Direction vector of segment S1
		osg::Vec3d d2 = q2 - p2; // Direction vector of segment S2
		osg::Vec3d r = p1 - p2;
		double a = d1* d1; // Squared length of segment S1, always nonnegative
		double e = d2* d2; // Squared length of segment S2, always nonnegative
		double f = d2* r;
		double res_dis = DBL_MAX;
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
				double c = (d1* r);
				if (e <= EPSILON)
				{
					// Second segment degenerates into a point
					t = 0.0f;
					s = Clamp(-c / a, 0.0f, 1.0f); // t = 0 => s = (b*t - c) / a = -c / a
				}
				else
				{
					// The general nondegenerate case starts here
					double b = (d1* d2);
					double denom = a*e - b*b; // Always nonnegative
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

	static bool isArrayInClockwiseDirection(const osg::Vec3Array * va, const osg::Vec3d & planenormal)
	{
		//先找个凸点;
		double farest = DBL_MIN;
		unsigned int farestIndex = 0;
		for (unsigned int i = 0; i < va->size(); ++i)
		{
			double r = (va->at(i)).length();
			if (r < farest) continue;
			farest = r;
			farestIndex = i;
		}

		unsigned int pre = farestIndex == 0 ? va->size() - 1 : farestIndex - 1;
		unsigned int nex = farestIndex == va->size() - 1 ? 0 : farestIndex + 1;

		//判断凸点处的方向, 就是多边形的方向;
		osg::Vec3d dir1 = va->at(farestIndex) - va->at(pre); dir1.normalize();
		osg::Vec3d dir2 = va->at(nex) - va->at(farestIndex); dir2.normalize();
		double f = (dir1^dir2)* planenormal;

		return f < 0;
	}

	static osg::Vec3Array* getSubArray(const osg::Vec3Array* va, int first, int last, bool isReverse)
	{
		if (last < first) return 0L;

		int maxIndex = va->size() - 1;
		if (last > maxIndex) return 0L;

		osg::ref_ptr<osg::Vec3Array> subva = new osg::Vec3Array;
		subva->reserve(last - first);
		if (!isReverse)
		{
			for (int j = first; j < last; ++j)
			{
				subva->push_back(va->at(j));
			}
		}
		else
		{
			for (int j = last; j > first; --j)
			{
				subva->push_back(va->at(j));
			}
		}

		return subva.release();
	}

	static osg::Vec3Array* getSubArrayOfLoop(const osg::Vec3Array* loopVa, int first, int last)
	{
		int maxIndex = loopVa->size() - 1;
		if (first > maxIndex || last > maxIndex)
		{
			return 0L;
		}
		osg::ref_ptr<osg::Vec3Array> subva = new osg::Vec3Array;
		if (first > last)
		{
			subva->reserve(maxIndex - first + last);
			for (int i = first; i < maxIndex; ++i)
			{
				subva->push_back(loopVa->at(i));
			}
			for (int i = 0; i < last; ++i)
			{
				subva->push_back(loopVa->at(i));
			}
		}
		else
		{
			subva->reserve(last - first);
			for (int i = first; i < last; ++i)
			{
				subva->push_back(loopVa->at(i));
			}
		}
		return subva.release();
	}

	static osg::Vec3Array* getReverseArray(const osg::Vec3Array* va)
	{
		osg::ref_ptr<osg::Vec3Array> reVa = new osg::Vec3Array;
		reVa->reserve(va->size());
		for (osg::Vec3Array::const_reverse_iterator it = va->rbegin(); it != va->rend(); ++it)
		{
			reVa->push_back((*it));
		}
		return reVa.release();
	}

	static void removeRepeat(osg::Vec3Array* va)
	{
		osg::Vec3Array::iterator it, it1;
		for (it = ++va->begin(); it != va->end();)
		{
			it1 = std::find(va->begin(), it, *it);
			if (it1 != it)
				it = va->erase(it);
			else
				it++;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
class Junction :public osg::Referenced
{
public:
	Junction(const osg::Vec3d& p) :_position(p) {}

	osg::Vec3d& position() { return _position; }
	const osg::Vec3d& position() const { return _position; }

	BooleanPathVector& pathVector() { return _paths; }
	const BooleanPathVector& pathVector() const { return _paths; }

private:
	osg::Vec3d        _position; //交点的位置;
	BooleanPathVector _paths;    //以_position为起点的关联路径链表
};

#define COMPUTE_BB_METHOD void computeBB() \
{\
	_bb.init();\
	for (unsigned int i = 0; i < _va->size(); ++i)\
	{\
		_bb.expandBy(_va->at(i));\
	}\
}

//////////////////////////////////////////////////////////////////////////
class Path : public osg::Referenced
{
public:
	typedef std::pair<unsigned int, osg::ref_ptr<Junction> > JunctionPair;
	typedef std::vector<JunctionPair> JunctionPairVector;

public:
	Path(const osg::Vec3Array* va)
		: _va(va)
		, _isClosed(false)
	{
		computeBB();
		_junctions.clear();

		double maxInterval = DBL_MIN;
		for (unsigned int i = 0; i < va->size() - 1; ++i)
		{
			osg::Vec3d p1 = va->at(i);
			osg::Vec3d p2 = va->at(i + 1);
			double interval = (p2 - p1).length();
			if (interval > maxInterval)
			{
				maxInterval = interval;
			}
		}

		if ((va->at(0) - va->at(va->size() - 1)).length() < maxInterval)
		{
			_isClosed = true;
		}
	}

	bool & isClosed() { return _isClosed; }
	const bool & isClosed() const { return _isClosed; }

	osg::BoundingBox& boundingBox() { return _bb; }
	const osg::BoundingBox& boundingBox() const { return _bb; }

	osg::ref_ptr<const osg::Vec3Array>& pointArray() { return _va; }
	const osg::ref_ptr<const osg::Vec3Array>& pointArray() const { return _va; }

	JunctionPairVector& junctions() { return _junctions; }
	const JunctionPairVector& junctions() const { return _junctions; }

	osg::ref_ptr<osg::Drawable>& drawable() { return _drawable; }
	const osg::ref_ptr<osg::Drawable>& drawable() const { return _drawable; }

	void addJunction(unsigned int i, Junction* junction)
	{
		if (!_junctions.empty())
		{
			const osg::Vec3 p0 = junction->position();
			for (JunctionPairVector::iterator it = _junctions.begin(); it != _junctions.end(); ++it)
			{
				const osg::Vec3 p1 = (*it).second->position();
				if (osg::equivalent(p0[0], p1[0], GEO_EPSILON) &&
					osg::equivalent(p0[1], p1[1], GEO_EPSILON) &&
					osg::equivalent(p0[2], p1[2], GEO_EPSILON))
				{
					if (abs((int)i - (int)it->first) == 1)
					{
						it->first = i;
					}
					return;
				}
			}
		}
		_junctions.push_back(JunctionPair(i, junction));
	}

private:
	COMPUTE_BB_METHOD

private:
	//std::string _name;
	bool _isClosed;
	osg::BoundingBox _bb;
	JunctionPairVector _junctions;
	osg::ref_ptr<osg::Drawable> _drawable;
	osg::ref_ptr<const osg::Vec3Array> _va;
};

//----------------------------------------------------------------------
class BooleanPath :public osg::Referenced
{
public:
	BooleanPath(osg::Vec3Array* va)
		: _isClosed(false)
		, _va(va)
		, _previous(0L)
		, _next(0L)
		, _twin(0L)
		, _startJunction(0L)
		, _endJunction(0L)
	{}

	bool & isClosed() { return _isClosed; }
	const bool& isClosed() const { return _isClosed; }

	osg::BoundingBox& boundingBox() { return _bb; }
	const osg::BoundingBox& boundingBox() const { return _bb; }

	osg::ref_ptr<Junction> &startJunction() { return _startJunction; }
	const osg::ref_ptr<Junction> &startJunction() const { return _startJunction; }

	osg::ref_ptr<Junction> &endJunction() { return _endJunction; }
	const osg::ref_ptr<Junction> &endJunction() const { return _endJunction; }

	osg::ref_ptr<BooleanPath>& previousPath() { return _previous; }
	const osg::ref_ptr<BooleanPath>& previousPath() const { return _previous; }

	osg::ref_ptr<BooleanPath>& nextPath() { return _next; }
	const osg::ref_ptr<BooleanPath>& nextPath() const { return _next; }

	osg::ref_ptr<BooleanPath>& twinPath() { return _twin; }
	const osg::ref_ptr<BooleanPath>& twinPath() const { return _twin; }

	osg::ref_ptr<osg::Vec3Array>& pointArray() { return _va; }
	const osg::ref_ptr<osg::Vec3Array>& pointArray() const { return _va; }

	osg::ref_ptr<osg::Drawable>& drawable() { return _drawable; }
	const osg::ref_ptr<osg::Drawable>& drawable() const { return _drawable; }

private:
	COMPUTE_BB_METHOD

private:
	//std::string _name;
	bool						 _isClosed;     //是否自闭合;
	osg::BoundingBox			 _bb;			//矩形包围框;
	osg::ref_ptr<Junction>		 _startJunction;//起点;
	osg::ref_ptr<Junction>		 _endJunction;  //终点;
	osg::ref_ptr<BooleanPath>	 _previous;     //前一路径;
	osg::ref_ptr<BooleanPath>	 _next;         //下一路径;
	osg::ref_ptr<BooleanPath>	 _twin;         //对应的反向路径;
	osg::ref_ptr<osg::Vec3Array> _va;			//对应的参数曲线;
	osg::ref_ptr<osg::Drawable>  _drawable;
};

//////////////////////////////////////////////////////////////////////////

struct DirectedLoop :public osg::Referenced
{
	enum Type { MAX_LOOP, MIN_LOOP, UNDEFINED };

	DirectedLoop() :_type(UNDEFINED), _va(0L) { _vaVector.clear(); }

	Type& type() { return _type; }
	const Type& type() const { return _type; }

	osg::BoundingBox& boundingBox() { return _bb; }
	const osg::BoundingBox& boundingBox() const { return _bb; }

	Vec3ArrayVector& pointArrayVector() { return _vaVector; }
	const Vec3ArrayVector& pointArrayVector() const { return _vaVector; }

	osg::ref_ptr<osg::Vec3Array>& pointArray() { return _va; }
	const osg::ref_ptr<osg::Vec3Array>& pointArray() const { return _va; }

	osg::ref_ptr<osg::Drawable>& drawable() { return _drawable; }
	const osg::ref_ptr<osg::Drawable>& drawable() const { return _drawable; }

	bool isContains(const DirectedLoop& loop)
	{
		if (_bb.radius() < loop.boundingBox().radius())
		{
			return false;
		}

		if (!_bb.intersects(loop.boundingBox()))
		{
			return false;
		}

		osg::Vec3d dir = loop.pointArray()->at(0) - loop.pointArray()->at(1);
		dir.normalize();

		int hitNum = 0;
		for (unsigned int i = 0; i < _va->size() - 1; ++i)
		{
			osg::Vec3d p1 = _va->at(i);
			osg::Vec3d p2 = _va->at(i + 1);
			for (unsigned int j = 0; j < loop.pointArray()->size(); ++j)
			{
				osg::Vec3d p3 = loop.pointArray()->at(j);
				osg::Vec3d p4 = p3 + dir*FLT_MAX;
				if (isSegmentsIntersect3(p1, p2, p3, p4, osg::Vec3d()))
				{
					hitNum++;
				}
			}
		}
		if (hitNum % 2 == 0)
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	void dirty(const osg::Vec3d& clipPlaneNormal)
	{
		unsigned vertexnum = 0;
		for (Vec3ArrayVector::const_iterator it = _vaVector.begin(); it != _vaVector.end(); ++it)
		{
			vertexnum += (*it)->size();
		}

		_va = new osg::Vec3Array;
		_va->reserve(vertexnum);
		for (Vec3ArrayVector::const_iterator it = _vaVector.begin(); it != _vaVector.end(); ++it)
		{
			for (osg::Vec3Array::const_iterator vi = (*it)->begin(); vi != (*it)->end(); ++vi)
			{
				_va->push_back(*vi);
			}
		}

		computeBB();

		if (isArrayInClockwiseDirection(_va, clipPlaneNormal))
		{
			type() = MAX_LOOP;
		}
		else
		{
			type() = MIN_LOOP;
		}
	}
private:
	COMPUTE_BB_METHOD

private:
	Type _type;
	osg::BoundingBox _bb;
	Vec3ArrayVector _vaVector;
	osg::ref_ptr<osg::Vec3Array> _va;
	osg::ref_ptr<osg::Drawable>  _drawable;
};

//////////////////////////////////////////////////////////////////////////

namespace
{
	static void removeBooleanPathFromJunction(BooleanPath* fpath)
	{
		if (!fpath)
		{
			return;
		}
		Junction* sjunction = fpath->startJunction();
		if (!sjunction)
		{
			return;
		}

		if (sjunction->pathVector().empty())
		{
			return;
		}

		BooleanPathVector::iterator it;
		it = std::find(sjunction->pathVector().begin(), sjunction->pathVector().end(), fpath);

		if (it == sjunction->pathVector().end())
		{
			return;
		}
		sjunction->pathVector().erase(it);
	}

	static bool getDifferentPos(const osg::Vec3Array* va, const osg::Vec3d& c, osg::Vec3d& result)
	{
		if (va->empty())
		{
			return false;
		}
		const osg::Vec3 fc = c;
		for (osg::Vec3Array::const_iterator it = va->begin(); it != va->end(); ++it)
		{
			const osg::Vec3 v = *it;
			if (!osg::equivalent(v[0], fc[0], GEO_EPSILON) || !osg::equivalent(v[1], fc[1], GEO_EPSILON) || !osg::equivalent(v[2], fc[2], GEO_EPSILON))
			{
				result = v;
				return true;
			}
		}
		return false;
	}

	static BooleanPath* getClockwiseNearstBooleanPath(const BooleanPathVector& paths, const BooleanPath* path, const osg::Vec3d& c, const osg::Vec3d&n)
	{
		if (path->pointArray()->empty())
		{
			return 0L;
		}

		BooleanPath* resultPath = 0L;
		double minClockwiseAngle = DBL_MAX;

		osg::Vec3d v0;
		if (!getDifferentPos(path->pointArray(), c, v0))
		{
			return resultPath;
		}
		osg::Vec3d dir0 = v0 - c; dir0.normalize();

		for (BooleanPathVector::const_iterator it = paths.begin(); it != paths.end(); ++it)
		{
			if (path == *it) continue;

			osg::Vec3d v1;
			if (!getDifferentPos((*it)->pointArray(), c, v1))
			{
				continue;
			}
			osg::Vec3d dir1 = v1 - c; dir1.normalize();

			double cloclwiseangle = getClockwiseAngle(dir0, dir1, n);
			//std::cout << (*it)->_name << ": " << osg::RadiansToDegrees(cloclwiseangle) << std::endl;

			if (minClockwiseAngle > cloclwiseangle)
			{
				minClockwiseAngle = cloclwiseangle;
				resultPath = *it;
			}
		}
		//std::cout << resultPath->_name << ": " << osg::RadiansToDegrees(minClockwiseAngle) << std::endl;
		//std::cout << std::endl;
		return resultPath;
	}

	static bool hasBooleanPathDeleted(BooleanPath* path)
	{
		Junction* startJunction = path->startJunction();
		if (startJunction->pathVector().empty())
		{
			return true;
		}

		BooleanPathVector::iterator it
			= std::find(startJunction->pathVector().begin(), startJunction->pathVector().end(), path);

		if (it != startJunction->pathVector().end())
		{
			return false;
		}
		return true;
	}

	static Junction* getJunctionAt(const osg::Vec3d& p, const JunctionVector& joints)
	{
		const osg::Vec3 p1 = p;
		for (JunctionVector::const_iterator it = joints.begin(); it != joints.end(); ++it)
		{
			const osg::Vec3 p0 = (*it)->position();
			if (osg::equivalent(p0[0], p1[0], GEO_EPSILON) &&
				osg::equivalent(p0[1], p1[1], GEO_EPSILON) &&
				osg::equivalent(p0[2], p1[2], GEO_EPSILON))
			{
				return (*it);
			}
		}
		return 0L;
	}

	struct ParamVec3Array : public osg::Referenced
	{
		ParamVec3Array(osg::Vec3Array* va, osg::Drawable* drawable)
			: _va(va)
			, _drawable(drawable)
		{}
		osg::ref_ptr<osg::Vec3Array> _va;
		osg::ref_ptr<osg::Drawable> _drawable;
	};
	typedef std::vector<osg::ref_ptr<ParamVec3Array> > ParamVec3ArrayVector;

	//class GeologicalPlaneIntersectOperator : public osg::NodeVisitor
	//{
	//public:
	//	GeologicalPlaneIntersectOperator(osgUtil::PlaneIntersector* pi, osg::NodeVisitor::TraversalMode tm = TRAVERSE_ALL_CHILDREN)
	//		: osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN)
	//		, _intersector(pi)
	//	{}

	//	virtual void apply(osg::Node& node)
	//	{
	//		traverse(node);
	//	}

	//	void traverse(osg::Node& node)
	//	{
	//		TraversalMode tm = getTraversalMode();
	//		if (tm == TRAVERSE_NONE)
	//		{
	//			return;
	//		}
	//		else if (tm == TRAVERSE_PARENTS)
	//		{
	//			osg::NodeVisitor::traverse(node);
	//			return;
	//		}
	//		osg::Geode* geode = dynamic_cast<osg::Geode*>(&node);
	//		if (geode)
	//		{
	//			unsigned numDrawables = geode->getNumDrawables();
	//			for (unsigned i = 0; i < numDrawables; ++i)
	//			{
	//				apply(*geode->getDrawable(i));
	//			}
	//		}
	//		else
	//		{
	//			osg::NodeVisitor::traverse(node);
	//		}
	//	}


	//	virtual void apply(osg::Drawable& drawable)
	//	{
	//		osgUtil::IntersectionVisitor intersectionVisitor;
	//		intersectionVisitor.reset();
	//		intersectionVisitor.setIntersector(_intersector.get());
	//		drawable.accept(intersectionVisitor);

	//		osgUtil::PlaneIntersector::Intersections& intersections = _intersector->getIntersections();
	//		osgUtil::PlaneIntersector::Intersections::iterator itr;

	//		for (itr = intersections.begin(); itr != intersections.end(); ++itr)
	//		{
	//			osgUtil::PlaneIntersector::Intersection& intersection = *itr;
	//			if (intersection.matrix.valid())
	//			{
	//				osg::ref_ptr<osg::Vec3Array> vva = new osg::Vec3Array;
	//				for (auto pitr = intersection.polyline.begin(); pitr != intersection.polyline.end(); ++pitr)
	//				{
	//					osg::Vec3d p = (*pitr) * (*intersection.matrix);
	//					vva->push_back(p);
	//				}
	//				intersection.matrix = 0;

	//				removeRepeat(vva);//模型重复部分需要删除冗余的点;

	//				if (!vva->empty())
	//				{
	//					osg::ref_ptr<ParamVec3Array> pa = new ParamVec3Array(vva, &drawable);
	//					_paVector.push_back(pa);
	//				}
	//			}
	//		}
	//	}

	//	ParamVec3ArrayVector& paVector() { return _paVector; }
	//	const ParamVec3ArrayVector& paVector() const { return _paVector; }

	//protected:
	//private:
	//	osg::ref_ptr<osgUtil::PlaneIntersector> _intersector;
	//	ParamVec3ArrayVector _paVector;
	//};
}


SliceOperator::SliceOperator()
{}

SliceOperator::~SliceOperator()
{}

bool compairJunction(Path::JunctionPair& j1, Path::JunctionPair& j2)
{
	return j1.first < j2.first;
}

void SliceOperator::generateJunctions(Path* p1, Path* p2, JunctionVector& joints)
{
	for (unsigned int i = 0; i < p1->pointArray()->size() - 1; ++i)
	{
		osg::Vec3d s1 = p1->pointArray()->at(i);
		osg::Vec3d e1 = p1->pointArray()->at(i + 1);
		for (unsigned int j = 0; j < p2->pointArray()->size() - 1; ++j)
		{
			osg::Vec3d s2 = p2->pointArray()->at(j);
			osg::Vec3d e2 = p2->pointArray()->at(j + 1);
			osg::Vec3d p;
			if (isSegmentsIntersect3(s1, e1, s2, e2, p))
			{
				if (p.isNaN())
				{
					continue;
				}

				osg::ref_ptr<Junction> joint = getJunctionAt(p, joints);
				if (!joint)
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

void SliceOperator::generateJunctions(const Vec3ArrayVector& vas, const osg::Vec3d& planeNormal, JunctionVector& outJoints)
{
	//std::vector<std::string> names;
	//names.push_back("A");
	//names.push_back("B");
	//names.push_back("C");

	//unsigned int namei = 0;
	PathVector paths; paths.reserve(vas.size());
	for (Vec3ArrayVector::const_iterator it = vas.begin(); it != vas.end(); ++it)
	{
		osg::ref_ptr<Path> path = new Path(*it);
		paths.push_back(path);
		//path->_name = names[namei];
		//namei++;
	}

	JunctionVector joints;
	for (unsigned int i = 0; i < paths.size(); ++i)
	{
		Path* path1 = paths[i];
		for (unsigned int j = i + 1; j < paths.size(); ++j)
		{
			Path* path2 = paths[j];
			if (!path1->boundingBox().intersects(path2->boundingBox()))
			{
				continue;
			}
			generateJunctions(path1, path2, joints);
		}
	}

	//path中的junction为空的时候说明是闭环;
	for (PathVector::iterator it = paths.begin(); it != paths.end(); ++it)
	{
		Path* path = *it;
		if (!path->junctions().empty()) continue;

		osg::ref_ptr<Junction> junction0 = new Junction(path->pointArray()->at(0));
		osg::ref_ptr<Junction> junction1 = new Junction(path->pointArray()->at(path->pointArray()->size() - 1));
		path->addJunction(0, junction0);
		path->addJunction(path->pointArray()->size() - 1, junction1);
		joints.push_back(junction0);
		joints.push_back(junction1);
	}

	//生成关联路径;
	BooleanPathVector bpvector; bpvector.reserve(joints.size()); joints.clear();
	for (PathVector::const_iterator it = paths.begin(); it != paths.end(); ++it)
	{
		Path* path = *it;
		std::sort(path->junctions().begin(), path->junctions().end(), compairJunction);
		generateBooleanPaths(path, path->isClosed(), bpvector);
	}

	outJoints.reserve(bpvector.size() * 2);
	for (BooleanPathVector::const_iterator it = bpvector.begin(); it != bpvector.end(); ++it)
	{
		if ((*it)->startJunction())
		{
			outJoints.push_back((*it)->startJunction());
		}
		if ((*it)->endJunction())
		{
			outJoints.push_back((*it)->endJunction());
		}
	}
}

void SliceOperator::generateBooleanPaths(Path* path, bool isClosedPath, BooleanPathVector &bpvector)
{
	if (path->junctions().size() < 2)
	{
		return;
	}

	for (unsigned int i = 0; i < path->junctions().size() - 1; ++i)
	{
		Junction* currentJunction = path->junctions()[i].second;
		Junction* nextJunction = path->junctions()[i + 1].second;

		unsigned int currentVertexIndex = path->junctions()[i].first;
		unsigned int nextVertexIndex = path->junctions()[i + 1].first;

		if (currentVertexIndex == nextVertexIndex)
		{
			continue;
		}

		osg::ref_ptr<osg::Vec3Array> va0 = getSubArray(path->pointArray(), currentVertexIndex, nextVertexIndex, false);
		osg::ref_ptr<osg::Vec3Array> va1 = getSubArray(path->pointArray(), currentVertexIndex, nextVertexIndex, true);

		if (!va0 || va0->empty() || !va1 || va1->empty())
		{
			continue;
		}

		va0->push_back(nextJunction->position());
		va1->push_back(currentJunction->position());
		//std::stringstream ss;
		//ss << i;
		//std::string namei = ss.str();

		osg::ref_ptr<BooleanPath> bp0 = new BooleanPath(va0);
		bp0->startJunction() = currentJunction;
		bp0->endJunction() = nextJunction;
		//bp0->_name = path->_name + namei;

		osg::ref_ptr<BooleanPath> bp1 = new BooleanPath(va1);
		bp1->startJunction() = nextJunction;
		bp1->endJunction() = currentJunction;
		//bp1->_name = path->_name + namei + namei;

		bp0->twinPath() = bp1;
		bp1->twinPath() = bp0;

		currentJunction->pathVector().push_back(bp0);
		nextJunction->pathVector().push_back(bp1);

		bpvector.push_back(bp0);
		bpvector.push_back(bp1);
	}
	if (isClosedPath)
	{
		Junction* currentJunction = path->junctions()[path->junctions().size() - 1].second;
		Junction* nextJunction = path->junctions()[0].second;

		unsigned int currendVertexindex = path->junctions()[path->junctions().size() - 1].first;
		unsigned int nextVertexIndex = path->junctions()[0].first;
		if (currendVertexindex == nextVertexIndex)
		{
			return;
		}
		osg::ref_ptr<osg::Vec3Array> va0 = getSubArrayOfLoop(path->pointArray(), currendVertexindex, nextVertexIndex);
		if (!va0 || va0->empty())
		{
			return;
		}
		osg::ref_ptr<osg::Vec3Array> va1 = getReverseArray(va0);

		//std::stringstream ss;
		//ss << 3;
		//std::string namei = ss.str();

		osg::ref_ptr<BooleanPath> bp0 = new BooleanPath(va0);
		bp0->startJunction() = currentJunction;
		bp0->endJunction() = nextJunction;
		//bp0->_name = path->_name + namei;

		osg::ref_ptr<BooleanPath> bp1 = new BooleanPath(va1);
		bp1->startJunction() = nextJunction;
		bp1->endJunction() = currentJunction;
		//bp1->_name = path->_name + namei + namei;
		bp0->twinPath() = bp1;
		bp1->twinPath() = bp0;

		currentJunction->pathVector().push_back(bp0);
		nextJunction->pathVector().push_back(bp1);

		bpvector.push_back(bp0);
		bpvector.push_back(bp1);
	}
}

void SliceOperator::traversePath(BooleanPath* currentPath, DirectedLoopVector& mloops, const osg::Vec3d& planeNormal)
{
	if (!currentPath)
	{
		return;
	}

	if (hasBooleanPathDeleted(currentPath))
	{
		return;
	}

	Junction* endjunction = currentPath->endJunction();
	if (!endjunction)
	{
		return;
	}

	if (endjunction->pathVector().empty())
	{
		BooleanPath* prePath = currentPath->previousPath();
		removeBooleanPathFromJunction(currentPath);
		traversePath(prePath, mloops, planeNormal);

		return;
	}

	BooleanPath* twinOfCurrentPath = currentPath->twinPath();
	if (!twinOfCurrentPath)
	{
		//error
		return;
	}

	BooleanPath* nextPath = getClockwiseNearstBooleanPath(endjunction->pathVector(), twinOfCurrentPath, endjunction->position(), planeNormal);
	if (!nextPath)
	{
		//删除相关path;
		BooleanPath* prePath = currentPath->previousPath();
		removeBooleanPathFromJunction(currentPath);
		traversePath(prePath, mloops, planeNormal);
		return;
	}

	if (nextPath->previousPath())
	{
		//构成了一个环; 删除相关path;
		osg::ref_ptr<DirectedLoop> maxloop = new DirectedLoop;

		BooleanPath* fpath = nextPath;
		while (fpath != nextPath->previousPath())
		{
			if (!fpath)
			{
				continue;
			}
			maxloop->pointArrayVector().push_back(fpath->pointArray());
			removeBooleanPathFromJunction(fpath);
			fpath = fpath->nextPath();
		}
		maxloop->pointArrayVector().push_back(fpath->pointArray());
		removeBooleanPathFromJunction(fpath);
		maxloop->dirty(planeNormal);
		mloops.push_back(maxloop);

		return;
	}

	nextPath->previousPath() = currentPath;
	currentPath->nextPath() = nextPath;
	traversePath(nextPath, mloops, planeNormal);
}

void SliceOperator::traverseJunction(Junction* junction, DirectedLoopVector& mloops, const osg::Vec3d& planeNormal)
{
	if (junction->pathVector().empty())
	{
		return;
	}

	BooleanPath* currentPath = *(junction->pathVector().begin());
	if (!currentPath)
	{
		return;
	}
	traversePath(currentPath, mloops, planeNormal);
}

void SliceOperator::generateDirectedLoops(JunctionVector& joints, const osg::Vec3d& planeNormal, DirectedLoopVector& outLoops)
{
	for (JunctionVector::iterator it = joints.begin(); it != joints.end(); ++it)
	{
		Junction* junction = *it;
		if (!junction)
		{
			continue;
		}

		//先设置preious path和next path为空;
		for (JunctionVector::iterator i = joints.begin(); i != joints.end(); ++i)
		{
			for (BooleanPathVector::iterator bi = (*i)->pathVector().begin(); bi != (*i)->pathVector().end(); ++bi)
			{
				if ((*bi)->previousPath())
				{
					(*bi)->previousPath() = 0L;
				}
				if ((*bi)->nextPath())
				{
					(*bi)->nextPath() = 0L;
				}
			}
		}
		traverseJunction(junction, outLoops, planeNormal);
	}
}

bool compairDirectedLoop(DirectedLoop& j1, DirectedLoop& j2)
{
	return j1.boundingBox().radius() > j2.boundingBox().radius();
}


//void SliceOperator::generateDirectedLoopTree(const DirectedLoopVector & loops, DirectedLoopTree & tree)
//{
//	//if (loops.empty()) return;
//
//	//std::sort(loops.begin(), loops.end(), compairDirectedLoop);
//	//tree.insert(loops[0]);
//
//	//for (unsigned int i = 1; i < loops.size(); ++i)
//	//{
//	//	for (TreeIterator treeit = tree.begin(); treeit != tree.end(); ++treeit)
//	//	{
//	//		if (!treeit->data()->boundingBox().intersects(loops[i]->boundingBox()))
//	//		{
//	//			treeit->parent();
//	//			//treeit->tree()
//	//			//treeit->parent()->erase(treeit);
//	//		}
//	//	}
//	//}
//}

osg::Node* SliceOperator::generateProfileNode(const DirectedLoopVector & loops)
{
	DirectedLoopVector maxLoops, minLoops;
	for (DirectedLoopVector::const_iterator it = loops.begin(); it != loops.end(); ++it)
	{
		if ((*it)->type() == DirectedLoop::MAX_LOOP)
			maxLoops.push_back(*it);
		else if ((*it)->type() == DirectedLoop::MIN_LOOP)
			minLoops.push_back(*it);
		else
		{
			//error;
		}
	}
	//
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	for (DirectedLoopVector::const_iterator minIt = minLoops.begin(); minIt != minLoops.end(); ++minIt)
	{
		DirectedLoop* minloop = *minIt;
		Vec3ArrayVector vas;
		vas.push_back(minloop->pointArray());

		for (DirectedLoopVector::const_iterator maxIt = maxLoops.begin(); maxIt != maxLoops.end(); ++maxIt)
		{
			if (minloop->isContains(**maxIt))
				vas.push_back((*maxIt)->pointArray());
		}

		unsigned int pn = 0;
		for (unsigned int i = 0; i < vas.size(); ++i)
			pn += vas[i]->size();

		osg::ref_ptr<osg::Vec3Array> va = new osg::Vec3Array;
		va->reserve(pn);

		osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
		geom->setVertexArray(va);

		unsigned int totalVertexNum = 0;
		for (Vec3ArrayVector::const_iterator it = vas.begin(); it != vas.end(); ++it)
		{
			unsigned currentVertexNum = 0;
			for (osg::Vec3Array::const_iterator vaIt = (*it)->begin(); vaIt != (*it)->end(); ++vaIt)
			{
				va->push_back(*vaIt);
				totalVertexNum++;
				currentVertexNum++;
			}
			geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP, totalVertexNum - currentVertexNum, currentVertexNum));
		}

		geode->addDrawable(geom);
	}

	return geode.release();
}



void SliceOperator::slice(osg::Group * geologicalModel, const osg::Plane& slicePlane)
{
	osg::ref_ptr<osgUtil::PlaneIntersector> intersector = new osgUtil::PlaneIntersector(slicePlane);
	intersector->setRecordHeightsAsAttributes(true);

	//GeologicalPlaneIntersectOperator gpio(intersector);
	//geologicalModel->accept(gpio);

	for (unsigned int i = 0; i < geologicalModel->getNumChildren(); ++i)
	{
		osgUtil::IntersectionVisitor intersectionVisitor;
		intersectionVisitor.reset();
		intersectionVisitor.setIntersector(intersector.get());
		geologicalModel->getChild(i)->accept(intersectionVisitor);

		osgUtil::PlaneIntersector::Intersections& intersections = intersector->getIntersections();
		osgUtil::PlaneIntersector::Intersections::iterator itr;

		for (itr = intersections.begin(); itr != intersections.end(); ++itr)
		{
			osgUtil::PlaneIntersector::Intersection& intersection = *itr;
			if (intersection.matrix.valid())
			{
				osg::ref_ptr<osg::Vec3Array> vva = new osg::Vec3Array;
				for (auto pitr = intersection.polyline.begin(); pitr != intersection.polyline.end(); ++pitr)
				{
					osg::Vec3d p = (*pitr) * (*intersection.matrix);
					vva->push_back(p);
				}
				intersection.matrix = 0;

				removeRepeat(vva);//模型重复部分需要删除冗余的点;
				if (!vva->empty())
				{
					//osg::ref_ptr<ParamVec3Array> pa = new ParamVec3Array(vva);
					//vaVector.push_back(vva);
				}
			}
		}
	}
}




















//--for test----------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
namespace
{
	osg::Geode* generatePoint(osg::Vec3Array* va, const osg::Vec4& color, float pointsize)
	{
		osg::ref_ptr<osg::Vec4Array> ca = new osg::Vec4Array;
		for (unsigned int i = 0; i < va->size(); ++i)
		{
			ca->push_back(color);
		}

		osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
		geom->setVertexArray(va);
		geom->setColorArray(ca, osg::Array::BIND_PER_VERTEX);
		geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POINTS, 0, va->size()));

		osg::ref_ptr<osg::Point> point = new osg::Point;
		point->setSize(pointsize);
		geom->getOrCreateStateSet()->setAttributeAndModes(point);

		osg::ref_ptr<osg::Geode> geode = new osg::Geode;
		geode->addDrawable(geom);

		geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

		return geode.release();
	}

	osg::Geode* generateLine(osg::Vec3Array* va, const osg::Vec4& color)
	{
		osg::ref_ptr<osg::Vec4Array> ca = new osg::Vec4Array;
		for (unsigned int i = 0; i < va->size(); ++i)
		{
			ca->push_back(color);
		}

		osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
		geom->setVertexArray(va);
		geom->setColorArray(ca, osg::Array::BIND_PER_VERTEX);
		geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP, 0, va->size()/* / 1.5*/));

		osg::ref_ptr<osg::Geode> geode = new osg::Geode;
		geode->addDrawable(geom);

		geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

		return geode.release();
	}

	osg::Geode* generatePolygon(osg::Vec3Array* va, const osg::Vec4& color)
	{
		osg::ref_ptr<osg::Vec4Array> ca = new osg::Vec4Array;
		for (unsigned int i = 0; i < va->size(); ++i)
		{
			ca->push_back(color);
		}

		osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
		geom->setVertexArray(va);
		geom->setColorArray(ca, osg::Array::BIND_PER_VERTEX);
		geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP, 0, va->size()/* / 1.5*/));

		osgUtil::Tessellator tscx;
		tscx.setTessellationType(osgUtil::Tessellator::TESS_TYPE_GEOMETRY);
		tscx.setBoundaryOnly(false);
		tscx.setWindingType(osgUtil::Tessellator::TESS_WINDING_NONZERO);
		tscx.retessellatePolygons(*geom);

		osg::ref_ptr<osg::Geode> geode = new osg::Geode;
		geode->addDrawable(geom);

		geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

		return geode.release();
	}
}


osg::Node * SliceOperator::generateJunctionPointNode(const Vec3ArrayVector & vaVector)
{
	JunctionVector junctions;
	DirectedLoopVector maxLoops;

	generateJunctions(vaVector, osg::Vec3d(1, 0, 0), junctions);
	generateDirectedLoops(junctions, osg::Vec3d(1, 0, 0), maxLoops);

	osg::ref_ptr<osg::Vec3Array> va = new osg::Vec3Array;
	for (JunctionVector::const_iterator it = junctions.begin(); it != junctions.end(); ++it)
	{
		va->push_back((*it)->position());
	}
	osg::ref_ptr<osg::Geode> pg = generatePoint(va, osg::Vec4(1, 1, 1, 1), 20.0f);

	//////////////////////////////////////////////////////////////////////////

	osg::ref_ptr<osg::Vec4Array> ca = new osg::Vec4Array;
	ca->push_back(osg::Vec4(1, 0, 0, 1));
	ca->push_back(osg::Vec4(1, 1, 0, 1));
	ca->push_back(osg::Vec4(0, 1, 0, 1));


	osg::ref_ptr<osg::Group> profileRoot = new osg::Group;
	osg::ref_ptr<osg::PolygonMode> pm = new osg::PolygonMode;
	pm->setMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::FILL);
	profileRoot->getOrCreateStateSet()->setAttributeAndModes(pm, osg::StateAttribute::OVERRIDE | osg::StateAttribute::ON);

	for (unsigned int i = 0; i < maxLoops.size(); ++i)
	{
		//if (i != 0) continue;

		DirectedLoop* maxloop = maxLoops[i];
		osg::Vec4 color;
		if (maxloop->type() == DirectedLoop::MAX_LOOP)
		{
			color = osg::Vec4(1, 0, 0, 1);
		}
		else
		{
			color = osg::Vec4(1, 1, 0, 1);
			profileRoot->addChild(generatePolygon(maxloop->pointArray(), ca->at(i - 1)));
		}
	}

	osg::ref_ptr<osg::Group> root = new osg::Group;
	root->addChild(pg);
	root->addChild(profileRoot);

	return root.release();
}
