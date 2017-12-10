#include "SliceOperator.h"
#include <osg/Vec3>
#include <osg/Point>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/PolygonMode>
#include <osgUtil/Tessellator>
#include <osgUtil/PlaneIntersector>
#include <osgUtil/LineSegmentIntersector>

#include <iostream>
#include <assert.h>
#include <sstream>

#define GEO_EPSILOND 1.192092896e-05
#define COMPUTEJUNCTION_EPSILON 0.01//计算Junction;
#define PATHREPEAT_EPSILON 0.01//判断BooleanPath是否重合;
#define MIN_JUNCTION_DIS 1.5//求顺时针夹角;

#define PI2 6.28318530717958647692

//问题1: 黏连情况处理不是很好;(解决)
//问题2: 精度控制的不是很好;(解决:Vec3dArray代替Vec3Array，调整TEST_EPSILON和GEO_EPSILOND的值）
//问题3：booleanpath遍历逻辑有点问题（解决）
//问题4：闭合且黏连时候，黏连结点删不干净（首尾）;(解决)
//问题5：黏连的时候，闭环的包围关系计算有误;（解决）
//问题6：还是精度
//问题7：材质填充
//问题8：求重合可优化

namespace
{
	static osg::Vec3d normalize(const osg::Vec3d& v)
	{
		osg::Vec3d n = v; n.normalize();
		return n;
	}

	static bool isVec3dEqual(const osg::Vec3d& p1, const osg::Vec3d& p2, double epsilon)
	{
		if (!osg::equivalent(p1.x(), p2.x(), epsilon)) return false;
		if (!osg::equivalent(p1.y(), p2.y(), epsilon)) return false;
		if (!osg::equivalent(p1.z(), p2.z(), epsilon)) return false;
		return true;
	}

	//得到顺时针方向的夹角;
	static double getClockwiseAngle(const osg::Vec3d& v1, const osg::Vec3d& v2, const osg::Vec3d& n)
	{
		if (isVec3dEqual(v1, osg::Vec3d(), GEO_EPSILOND) ||
			isVec3dEqual(v2, osg::Vec3d(), GEO_EPSILOND))
		{
			return 0.0;
		}

		osg::Vec3d vv1 = normalize(v1);
		osg::Vec3d vv2 = normalize(v2);
		if (isVec3dEqual(vv1, vv2, GEO_EPSILOND))
		{
			return 0.0;
		}
		if (isVec3dEqual(vv1, -vv2, GEO_EPSILOND))
		{
			return osg::PI;
		}

		osg::Quat rot;
		rot.makeRotate_original(v1, v2);

		double angle;
		osg::Vec3d rotAxis;
		rot.getRotate(angle, rotAxis);
		rotAxis.normalize();

		//angle = ((-v1) ^ v2)*n > 0.0 ? angle : PI2 - angle;
		angle = rotAxis*normalize(n) > 0 ? PI2 - angle : angle;

		return angle;
	}


	static double Clamp(double n, double min, double max)
	{
		if (n < min) return min;
		if (n > max) return max;
		return n;
	}

	// from《 Real-Time Collision Detection 》;
#	define EPSILON /*FLT_EPSILON*/1.192092896e-06
	static bool isSegmentsIntersect3(const osg::Vec3d& p1, const osg::Vec3d& q1, const osg::Vec3d& p2, const osg::Vec3d& q2, osg::Vec3d &p)
	{
		double s, t;
		osg::Vec3d c1, c2;
		osg::Vec3d d1 = q1 - p1;
		osg::Vec3d d2 = q2 - p2;
		osg::Vec3d r = p1 - p2;
		double a = d1* d1;
		double e = d2* d2;
		double f = d2* r;
		double res_dis = DBL_MAX;

		if (a <= EPSILON && e <= EPSILON) {
			s = t = 0.0f;
			c1 = p1;
			c2 = p2;
			res_dis = (c1 - c2)*(c1 - c2);
		}
		else {
			if (a <= EPSILON) {
				s = 0.0f;
				t = f / e;
				t = Clamp(t, 0.0f, 1.0f);
			}
			else {
				double c = (d1* r);
				if (e <= EPSILON) {
					t = 0.0f;
					s = Clamp(-c / a, 0.0f, 1.0f);
				}
				else {
					double b = (d1* d2);
					double denom = a*e - b*b;
					if (denom != 0.0f) {
						s = Clamp((b*f - c*e) / denom, 0.0f, 1.0f);
					}
					else s = 0.0f;
					t = (b*s + f) / e;
					if (t < 0.0f) {
						t = 0.0f;
						s = Clamp(-c / a, 0.0f, 1.0f);
					}
					else if (t > 1.0f) {
						t = 1.0f;
						s = Clamp((b - c) / a, 0.0f, 1.0f);
					}
				}
			}
			c1 = p1 + d1 * s;
			c2 = p2 + d2 * t;
			res_dis = (c1 - c2)*(c1 - c2);
		}

		if (res_dis < EPSILON) {
			p = c1;
			return true;
		}
		return false;
	}

	static bool isArrayInClockwiseDirection(const osg::Vec3dArray * va, const osg::Vec3d & planenormal)
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

	static osg::Vec3d getPointInPolygon(const osg::Vec3dArray* va)
	{
		double farest = DBL_MIN;
		unsigned int farestIndex = 0;
		for (unsigned int i = 0; i < va->size(); ++i)
		{
			double r = (va->at(i)).length();
			if (r < farest) continue;
			farest = r;
			farestIndex = i;
		}

		unsigned int nextIndex = farestIndex == va->size() - 1 ? 0 : farestIndex + 1;
		unsigned int prevIndex = farestIndex == 0 ? va->size() - 1 : farestIndex - 1;

		osg::Vec3d dir1 = va->at(nextIndex) - va->at(farestIndex);
		osg::Vec3d dir2 = va->at(prevIndex) - va->at(farestIndex);

		osg::Vec3d point = va->at(farestIndex) + (dir1 + dir2)*0.25;

		return point;
	}

	static osg::Vec3dArray* getSubArray(const osg::Vec3dArray* va, int first, int last)
	{
		if (!va || last < first) return 0L;

		int maxIndex = va->size() - 1;
		int f = Clamp(first, 0, maxIndex);
		int l = Clamp(last, 0, maxIndex);
		osg::ref_ptr<osg::Vec3dArray> subva = new osg::Vec3dArray;
		subva->reserve(l - f + 1);
		for (int j = f; j <= l; ++j)
		{
			subva->push_back(va->at(j));
		}
		return subva.release();
	}

	static osg::Vec3dArray* getSubArrayOfLoop(const osg::Vec3dArray* loopVa, int first, int last)
	{
		int maxIndex = loopVa->size() - 1;
		int f = Clamp(first, 0, maxIndex);
		int l = Clamp(last, 0, maxIndex);
		osg::ref_ptr<osg::Vec3dArray> subva = new osg::Vec3dArray;
		if (f > l) {
			subva->reserve(maxIndex - f + l + 2);
			for (int i = f; i <= maxIndex; ++i)
				subva->push_back(loopVa->at(i));
			for (int j = 0; j <= l; ++j)
				subva->push_back(loopVa->at(j));
		}
		else {
			subva->reserve(l - f + 1);
			for (int i = f; i <= l; ++i)
				subva->push_back(loopVa->at(i));
		}
		return subva.release();
	}

	static osg::Vec3dArray* getReverseArray(const osg::Vec3dArray* va)
	{
		osg::ref_ptr<osg::Vec3dArray> reVa = new osg::Vec3dArray;
		reVa->reserve(va->size());
		for (osg::Vec3dArray::const_reverse_iterator it = va->rbegin(); it != va->rend(); ++it)
		{
			reVa->push_back((*it));
		}
		return reVa.release();
	}

	static bool isAlmostSameArray(const osg::Vec3dArray* va1, const osg::Vec3dArray* va2, double epsilon)
	{
		if (va1->size() != va2->size())
		{
			return false;
		}
		osg::Vec3dArray::const_iterator it1, it2;
		for (it1 = va1->begin(), it2 = va2->begin(); it1 != va1->end(), it2 != va2->end(); ++it1, ++it2)
		{
			if (!isVec3dEqual(*it1, *it2, epsilon))
			{
				return false;
			}
		}
		return true;
	}

	static void removeRepeat(osg::Vec3dArray* va, double epsilon = GEO_EPSILOND)
	{
		osg::Vec3dArray::iterator it, it1;
		for (it = ++va->begin(); it != va->end();)
		{
			bool isRepeat = false;
			for (it1 = va->begin(); it1 != it; ++it1)
			{
				if (isVec3dEqual(*it, *it1, epsilon))
				{
					isRepeat = true;
					break;
				}
			}
			if (isRepeat)
			{
				it = va->erase(it);
			}
			else
			{
				it++;
			}
		}
	}

	static osg::Vec3d getNearestFarerThan(const osg::Vec3dArray* va, const osg::Vec3d& p, double dis)
	{
		double sumDis = 0.0;
		osg::Vec3dArray::const_iterator it;
		for (it = ++va->begin(); it != va->end(); ++it)
		{
			sumDis += ((*it) - (*(it - 1))).length();
			if (sumDis >= dis)
			{
				return *it;
			}
		}
		return va->at(va->size() - 1);
	}
}

//////////////////////////////////////////////////////////////////////////
//
class Junction :public osg::Referenced
{
public:
	Junction(const osg::Vec3d& p) :_position(p) {}

public:
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
	Path(osg::Vec3dArray* va)
		: _va(va)
		, _isClosed(false)
	{
		computeBB();
		_junctions.clear();

		double maxInterval = DBL_MIN;
		for (unsigned int i = 0; i < va->size() - 1; ++i)
		{
			double interval = (va->at(i) - va->at(i + 1)).length();
			if (interval > maxInterval)
			{
				maxInterval = interval;
			}
		}

		//判断闭合的条件放宽些吧;
		if ((va->at(0) - va->at(va->size() - 1)).length() < 2.0*maxInterval)
		{
			_isClosed = true;
		}
	}


	void addJunction(unsigned int i, Junction* junction)
	{
		if (!_junctions.empty())
		{
			const osg::Vec3d& p0 = junction->_position;
			for (JunctionPairVector::iterator it = _junctions.begin(); it != _junctions.end(); ++it)
			{
				if ((*it).first == i)//重合;
				{
					return;
				}
				const osg::Vec3d& p1 = (*it).second->_position;
				if (isVec3dEqual(p0, p1, GEO_EPSILOND))
				{
					return;
				}
			}
		}
		_junctions.push_back(JunctionPair(i, junction));
	}

private:
	COMPUTE_BB_METHOD

public:
	std::string _name;
	bool _isClosed;
	osg::BoundingBox _bb;
	JunctionPairVector _junctions;
	osg::ref_ptr<osg::Vec3dArray> _va;
};

//----------------------------------------------------------------------
class BooleanPath :public osg::Referenced
{
public:
	BooleanPath(osg::Vec3dArray* va)
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

	osg::ref_ptr<osg::Vec3dArray>& pointArray() { return _va; }
	const osg::ref_ptr<osg::Vec3dArray>& pointArray() const { return _va; }

private:
	COMPUTE_BB_METHOD

private:
	bool						  _isClosed;     //是否自闭合;
	osg::BoundingBox			  _bb;			 //矩形包围框;
	osg::ref_ptr<Junction>		  _startJunction;//起点;
	osg::ref_ptr<Junction>		  _endJunction;  //终点;
	osg::ref_ptr<BooleanPath>	  _previous;     //前一路径;
	osg::ref_ptr<BooleanPath>	  _next;         //下一路径;
	osg::ref_ptr<BooleanPath>	  _twin;         //对应的反向路径;
	osg::ref_ptr<osg::Vec3dArray>  _va;			 //对应的参数曲线;
public:
	std::string _name;
};
//////////////////////////////////////////////////////////////////////////

struct DirectedLoop :public osg::Referenced
{
	enum Type { MAX_LOOP, MIN_LOOP, UNDEFINED };

	DirectedLoop() :_type(UNDEFINED), _va(0L)
	{
		if (!_booleanPathVector.empty())
		{
			_booleanPathVector.clear();
		}
	}

	Type& type() { return _type; }
	const Type& type() const { return _type; }

	osg::BoundingBox& boundingBox() { return _bb; }
	const osg::BoundingBox& boundingBox() const { return _bb; }

	BooleanPathVector& booleanPathVector() { return _booleanPathVector; }
	const BooleanPathVector& booleanPathVector() const { return _booleanPathVector; }

	osg::ref_ptr<osg::Vec3dArray>& pointArray() { return _va; }
	const osg::ref_ptr<osg::Vec3dArray>& pointArray() const { return _va; }

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

		//重合?（孤岛通常会出现这种情况）;
		if (_booleanPathVector.size() == loop.booleanPathVector().size())
		{
			bool isRepeat = true;
			BooleanPathVector::const_iterator it0, it1;
			for (it0 = _booleanPathVector.begin(); it0 != _booleanPathVector.end(); ++it0)
			{
				bool isSamePath = false;
				for (it1 = loop.booleanPathVector().begin(); it1 != loop.booleanPathVector().end(); ++it1)
				{
					if ((*it0)->twinPath() == *it1)
					{
						isSamePath = true;
						break;
					}
				}
				if (!isSamePath)
				{
					isRepeat = false;
					break;
				}
			}
			if (isRepeat)
			{
				return false;
			}
		}

		//求交点数量;--判断一个不黏连的点就够了（因为算法可以保证不会出现部分覆盖的情况）。
		osg::Vec3d dir = loop.pointArray()->at(0) - loop.pointArray()->at(1);
		dir.normalize();
		for (unsigned int i = 0; i < loop.pointArray()->size(); ++i)
		{
			//不选择黏连的点;
			const osg::Vec3d& p = loop.pointArray()->at(i);
			osg::Vec3dArray::iterator it = std::find(_va->begin(), _va->end(), p);
			if (it != _va->end())
			{
				continue;
			}
			int hitNum = 0;
			const osg::Vec3d& p1 = loop.pointArray()->at(i);
			osg::Vec3d p2 = p1 + dir*FLT_MAX;
			for (unsigned int j = 0; j < _va->size() - 1; ++j)
			{
				osg::Vec3d p3 = _va->at(j);
				osg::Vec3d p4 = _va->at(j + 1);
				if (isSegmentsIntersect3(p1, p2, p3, p4, osg::Vec3d()))
				{
					hitNum++;
				}
			}
			if (hitNum % 2 == 0)
			{
				return false;
			}
			return true;
		}
		return false;
	}


	void dirty(const osg::Vec3d& clipPlaneNormal)
	{
		unsigned vertexnum = 0;
		BooleanPathVector::const_iterator it0;
		for (it0 = _booleanPathVector.begin(); it0 != _booleanPathVector.end(); ++it0)
		{
			vertexnum += (*it0)->pointArray()->size();
		}
		if (!_va) _va = new osg::Vec3dArray;
		if (!_va->empty())_va->clear();
		_va->reserve(vertexnum);
		BooleanPathVector::const_iterator it;
		for (it = _booleanPathVector.begin(); it != _booleanPathVector.end(); ++it)
		{
			osg::Vec3dArray::const_iterator vi;
			for (vi = (*it)->pointArray()->begin(); vi != (*it)->pointArray()->end(); ++vi)
			{
				_va->push_back(*vi);
			}
		}
		//removeRepeat(_va);
		computeBB();
		type() = isArrayInClockwiseDirection(_va, clipPlaneNormal) ? MAX_LOOP : MIN_LOOP;
	}
private:
	COMPUTE_BB_METHOD

private:
	Type _type;
	osg::BoundingBox _bb;
	BooleanPathVector _booleanPathVector;
	osg::ref_ptr<osg::Vec3dArray> _va;
};

//////////////////////////////////////////////////////////////////////////

namespace
{
	static void removeBooleanPathFromJunction(BooleanPath* fpath)
	{
		if (!fpath) return;
		Junction* sjunction = fpath->startJunction();
		if (!sjunction) return;
		if (sjunction->_paths.empty()) return;

		BooleanPathVector::iterator it;
		it = std::find(sjunction->_paths.begin(), sjunction->_paths.end(), fpath);
		if (it == sjunction->_paths.end())
		{
			return;
		}
		sjunction->_paths.erase(it);
	}

	static BooleanPath* getClockwiseNearstBooleanPath(const BooleanPathVector& paths, const BooleanPath* path, const osg::Vec3d& c, const osg::Vec3d&n)
	{
		if (!path) return 0L;
		if (path->pointArray()->empty()) return 0L;
		if (paths.empty()) return 0L;

		osg::ref_ptr<BooleanPath> resultPath = 0L;
		double minClockwiseAngle = DBL_MAX;

		//为了减少误差，取距离junction较远的点;
		osg::Vec3d v0 = getNearestFarerThan(path->pointArray(), c, MIN_JUNCTION_DIS);
		osg::Vec3d dir0 = normalize(v0 - c);
		for (BooleanPathVector::const_iterator it = paths.begin(); it != paths.end(); ++it)
		{
			if (path == *it) continue;
			osg::Vec3d v1 = getNearestFarerThan((*it)->pointArray(), c, MIN_JUNCTION_DIS);
			osg::Vec3d dir1 = normalize(v1 - c);

			double cloclwiseangle = getClockwiseAngle(dir0, dir1, n);
			if (minClockwiseAngle > cloclwiseangle)
			{
				minClockwiseAngle = cloclwiseangle;
				resultPath = *it;
			}
		}
		return resultPath.release();
	}

	static bool hasBooleanPathDeleted(BooleanPath* path)
	{
		Junction* startJunction = path->startJunction();
		if (startJunction->_paths.empty())
		{
			return true;
		}

		BooleanPathVector::iterator it
			= std::find(startJunction->_paths.begin(), startJunction->_paths.end(), path);

		if (it != startJunction->_paths.end())
		{
			return false;
		}
		return true;
	}

	static Junction* getJunctionAt(const osg::Vec3d& p, const JunctionVector& joints)
	{
		const osg::Vec3d& p1 = p;
		for (JunctionVector::const_iterator it = joints.begin(); it != joints.end(); ++it)
		{
			const osg::Vec3d& p0 = (*it)->_position;
			if (isVec3dEqual(p0, p1, GEO_EPSILOND))
			{
				return (*it);
			}
		}
		return 0L;
	}

	struct GetGeoAttribuate : public osg::NodeVisitor
	{
	public:
		GetGeoAttribuate() : osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN)
		{
			color = osg::Vec4(1.0, 1.0, 1.0, 1.0);
			_texture = 0L;
		}

		void apply(osg::Geode& geode)
		{
			for (unsigned int i = 0; i < geode.getNumDrawables(); ++i)
			{
				osg::Geometry* geom = dynamic_cast<osg::Geometry*>(geode.getDrawable(i));
				if (geom)
				{
					osg::Vec4Array* ca = dynamic_cast<osg::Vec4Array*>(geom->getColorArray());
					if (ca && !ca->empty())
					{
						color = osg::Vec4(ca->at(0));
					}

					osg::StateSet* ss = geom->getStateSet();
					if (ss)
					{
						osg::StateSet::TextureAttributeList& a = ss->getTextureAttributeList();
						for (osg::StateSet::TextureAttributeList::iterator i = a.begin(); i != a.end(); ++i)
						{
							osg::StateSet::AttributeList& b = *i;
							for (osg::StateSet::AttributeList::iterator j = b.begin(); j != b.end(); ++j)
							{
								osg::StateAttribute* sa = j->second.first.get();
								if (sa)
								{
									osg::Texture* tex = dynamic_cast<osg::Texture*>(sa);
									if (tex)
									{
										_texture = dynamic_cast<osg::Texture2D*>(tex);
									}
								}
							}
						}
					}
					return;
				}
			}
			traverse(geode);
		}
		const osg::Vec4& getColor()const { return color; }
		osg::Texture2D* getTexture() { return _texture; }
	private:
		osg::Vec4 color;
		osg::ref_ptr<osg::Texture2D> _texture;
	};
}

//////////////////////////////////////////////////////////////////////////
SliceOperator::SliceOperator(bool isOnEarth) :_isOnEarth(isOnEarth)
{}

SliceOperator::~SliceOperator()
{}

void SliceOperator::generateJunctions(Path* p1, Path* p2, JunctionVector& joints)
{
#if 1
	for (unsigned int i = 0; i < p1->_va->size(); ++i)
	{
		const osg::Vec3d& v1 = p1->_va->at(i);
		for (unsigned int j = 0; j < p2->_va->size(); ++j)
		{
			const osg::Vec3d& v2 = p2->_va->at(j);
			if (isVec3dEqual(v1, v2, COMPUTEJUNCTION_EPSILON))
			{
				osg::Vec3d p = (v1 + v2)*0.5;
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
#else
	for (unsigned int i = 0; i < p1->_va->size() - 1; ++i)
	{
		const osg::Vec3d& s1 = p1->_va->at(i);
		const osg::Vec3d& e1 = p1->_va->at(i + 1);
		for (unsigned int j = 0; j < p2->_va->size() - 1; ++j)
		{
			const osg::Vec3d& s2 = p2->_va->at(j);
			const osg::Vec3d& e2 = p2->_va->at(j + 1);
			osg::Vec3d p;
			if (isSegmentsIntersect3(s1, e1, s2, e2, p))
			{
				if (p.isNaN()) continue;

				osg::ref_ptr<Junction> joint = getJunctionAt(p, joints);
				if (!joint)
				{
					joint = new Junction(p);
					joints.push_back(joint);
				}
				//length
				unsigned int ii = (p - s1).length2() < (p - e1).length2() ? i : i + 1;
				unsigned int jj = (p - s2).length2() < (p - e2).length2() ? j : j + 1;

				p1->addJunction(ii, joint);
				p2->addJunction(jj, joint);
			}
		}
	}
#endif
}

bool compairJunction(Path::JunctionPair& j1, Path::JunctionPair& j2)
{
	return j1.first < j2.first;
}

void SliceOperator::generateJunctions(const Vec3dArrayVector& vas, const osg::Vec3d& planeNormal, JunctionVector& outJoints)
{
	PathVector paths; paths.reserve(vas.size());
	for (Vec3dArrayVector::const_iterator it = vas.begin(); it != vas.end(); ++it)
	{
		osg::ref_ptr<Path> path = new Path((*it));
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
			generateJunctions(path1, path2, joints);
		}
	}

	unsigned int ssi = 0;
	std::vector<std::string> ss;
	ss.push_back("A");
	ss.push_back("B");
	ss.push_back("C");
	ss.push_back("D");
	ss.push_back("E");
	ss.push_back("F");
	ss.push_back("G");
	ss.push_back("H");
	ss.push_back("I");
	ss.push_back("J");
	ss.push_back("K");
	ss.push_back("L");

	//生成关联路径;
	BooleanPathVector bpvector;
	bpvector.reserve(joints.size() * 4);
	joints.clear();

	for (PathVector::const_iterator it = paths.begin(); it != paths.end(); ++it)
	{
		Path* path = *it;
		path->_name = ss[ssi];
		ssi++;

		std::sort(path->_junctions.begin(), path->_junctions.end(), compairJunction);
		if (path->_junctions.size() > 2)
		{
			//删除相邻的Junction（保留首尾），避免黏连情况;
			//Path::JunctionPairVector junctionsToRemoved;
			Path::JunctionPairVector::iterator jit0 = path->_junctions.begin();
			Path::JunctionPairVector::iterator jit1 = jit0 + 1;
			Path::JunctionPairVector::iterator jit2 = jit1 + 1;
			for (; jit2 != path->_junctions.end(); /*++jit0, ++jit1, ++jit2*/)
			{
				if ((*jit1).first - (*jit0).first == 1 &&
					(*jit2).first - (*jit1).first == 1)
				{
					jit1 = path->_junctions.erase(jit1);
					jit0 = jit1 - 1;
					jit2 = jit1 + 1;
				}
				else
				{
					jit0++; jit1++; jit2++;
				}
			}
			if (path->_isClosed)
			{
				jit0 = path->_junctions.begin();
				jit1 = path->_junctions.end() - 1;
				jit2 = jit1 - 1;
				if (jit0->first == 0 && jit1->first == path->_va->size() - 1 && jit2->first == path->_va->size() - 2)
				{
					jit1 = path->_junctions.erase(jit1);
				}

				jit0 = path->_junctions.end() - 1;
				jit1 = path->_junctions.begin();
				jit2 = jit1 + 1;
				if (jit0->first == path->_va->size() - 1 && jit1->first == 0 && jit2->first == 1)
				{
					jit1 = path->_junctions.erase(jit1);
				}

				//if (path->_junctions[0].first == 0 &&
				//	path->_junctions[1].first == 1 &&
				//	path->_junctions[path->_junctions.size() - 1].first == path->_va->size() - 1)
				//{
				//	junctionsToRemoved.push_back(path->_junctions[0]);
				//}
				//if (path->_junctions[0].first == 0 &&
				//	path->_junctions[path->_junctions.size() - 2].first == path->_va->size() - 2 &&
				//	path->_junctions[path->_junctions.size() - 1].first == path->_va->size() - 1)
				//{
				//	junctionsToRemoved.push_back(path->_junctions[path->_junctions.size() - 1]);
				//}
			}
		}
		generateBooleanPaths(path, path->_isClosed, bpvector);
	}

	outJoints.reserve(bpvector.size() * 2);
	for (BooleanPathVector::const_iterator it = bpvector.begin(); it != bpvector.end(); ++it)
	{
		outJoints.push_back((*it)->startJunction());
		outJoints.push_back((*it)->endJunction());
	}
}

bool isBooleanPathRepeat(const BooleanPathVector& paths, const BooleanPath* path)
{
	for (BooleanPathVector::const_iterator it = paths.begin(); it != paths.end(); ++it)
	{
		if (path->startJunction() != (*it)->startJunction() &&
			path->endJunction() != (*it)->endJunction())
		{
			continue;
		}
		if (isAlmostSameArray(path->pointArray(), (*it)->pointArray(), PATHREPEAT_EPSILON))
		{
			return true;
		}
	}
	return false;
}

void SliceOperator::generateBooleanPaths(Path* path, bool isClosedPath, BooleanPathVector &bpvector)
{
	if (path->_junctions.size() < 2)
	{
		//自己单独成闭环--如果junction的数量为1，忽略此junction;
		//分别在起点终点添加一个junction;
		osg::ref_ptr<osg::Vec3dArray> va0 = getSubArray(path->_va, 0, path->_va->size() - 1);
		if (va0 && !va0->empty())
		{
			osg::ref_ptr<Junction> sjunction = new Junction(va0->at(0));
			osg::ref_ptr<Junction> ejunction = new Junction(va0->at(va0->size() - 1));

			osg::ref_ptr<BooleanPath> bp0 = new BooleanPath(va0);
			bp0->startJunction() = sjunction;
			bp0->endJunction() = ejunction;
			bp0->isClosed() = true;
			bp0->_name = path->_name + "0";

			osg::ref_ptr<osg::Vec3dArray> va1 = getReverseArray(va0);
			osg::ref_ptr<BooleanPath> bp1 = new BooleanPath(va1);
			bp1->startJunction() = ejunction;
			bp1->endJunction() = sjunction;
			bp1->isClosed() = true;
			bp1->_name = path->_name + "00";

			bp0->twinPath() = bp1;
			bp1->twinPath() = bp0;

			sjunction->_paths.push_back(bp0);
			ejunction->_paths.push_back(bp1);

			bpvector.push_back(bp0);
			bpvector.push_back(bp1);
		}
	}
	else
	{
		for (unsigned int i = 0; i < path->_junctions.size() - 1; ++i)
		{
			Junction* currentJunction = path->_junctions[i].second;
			Junction* nextJunction = path->_junctions[i + 1].second;

			unsigned int currentVertexIndex = path->_junctions[i].first;
			unsigned int nextVertexIndex = path->_junctions[i + 1].first;
			if (nextVertexIndex == currentVertexIndex)
			{
				continue;
			}

			//往后一个取值，确保不出现重复的点;
			int firstindex = currentVertexIndex == path->_va->size() - 1 ? 0 : currentVertexIndex + 1;
			osg::ref_ptr<osg::Vec3dArray> va0 = getSubArray(path->_va, firstindex, nextVertexIndex);
			if (va0 && !va0->empty())
			{
				std::stringstream ss;
				ss << i;

				osg::ref_ptr<BooleanPath> bp0 = new BooleanPath(va0);
				bp0->startJunction() = currentJunction;
				bp0->endJunction() = nextJunction;
				//忽略重合的;
				if (isBooleanPathRepeat(bpvector, bp0))
				{
					continue;
				}
				bp0->_name = path->_name + ss.str();

				int lastindex = nextVertexIndex == 0 ? path->_va->size() - 1 : nextVertexIndex - 1;
				osg::ref_ptr<osg::Vec3dArray> tempva1 = getSubArray(path->_va, currentVertexIndex, lastindex);
				osg::ref_ptr<osg::Vec3dArray> va1 = getReverseArray(tempva1);
				osg::ref_ptr<BooleanPath> bp1 = new BooleanPath(va1);
				bp1->startJunction() = nextJunction;
				bp1->endJunction() = currentJunction;
				bp1->_name = bp0->_name + ss.str();

				bp0->twinPath() = bp1;
				bp1->twinPath() = bp0;

				currentJunction->_paths.push_back(bp0);
				nextJunction->_paths.push_back(bp1);

				bpvector.push_back(bp0);
				bpvector.push_back(bp1);
			}
		}
		if (path->_isClosed)
		{
			Junction* currentJunction = path->_junctions[path->_junctions.size() - 1].second;
			Junction* nextJunction = path->_junctions[0].second;

			unsigned int currendVertexindex = path->_junctions[path->_junctions.size() - 1].first;
			unsigned int nextVertexIndex = path->_junctions[0].first;

			int firstindex = currendVertexindex == path->_va->size() - 1 ? 0 : currendVertexindex + 1;
			osg::ref_ptr<osg::Vec3dArray> va0 = getSubArrayOfLoop(path->_va, firstindex, nextVertexIndex);
			if (va0 && !va0->empty())
			{
				std::stringstream ss;
				ss << path->_junctions.size() - 1;

				osg::ref_ptr<BooleanPath> bp0 = new BooleanPath(va0);
				bp0->startJunction() = currentJunction;
				bp0->endJunction() = nextJunction;
				if (!isBooleanPathRepeat(bpvector, bp0))
				{
					bp0->_name = path->_name + ss.str();

					int lastindex = nextVertexIndex == 0 ? path->_va->size() - 1 : nextVertexIndex - 1;
					osg::ref_ptr<osg::Vec3dArray> tempva1 = getSubArrayOfLoop(path->_va, currendVertexindex, lastindex);
					osg::ref_ptr<osg::Vec3dArray> va1 = getReverseArray(tempva1);
					osg::ref_ptr<BooleanPath> bp1 = new BooleanPath(va1);
					bp1->startJunction() = nextJunction;
					bp1->endJunction() = currentJunction;
					bp1->_name = bp0->_name + ss.str();

					bp0->twinPath() = bp1;
					bp1->twinPath() = bp0;

					currentJunction->_paths.push_back(bp0);
					nextJunction->_paths.push_back(bp1);

					bpvector.push_back(bp0);
					bpvector.push_back(bp1);
				}
			}
		}
	}
}

//
void SliceOperator::traversePath(BooleanPath* currentPath, DirectedLoopVector& mloops, const osg::Vec3d& planeNormal)
{
	if (!currentPath) return;
	////理论上肯定不会出现这种情况的;
	//if (hasBooleanPathDeleted(currentPath))
	//{
	//	return;
	//}

	//自成一环;
	if (currentPath->isClosed())
	{
		osg::ref_ptr<DirectedLoop> maxloop = new DirectedLoop;
		maxloop->booleanPathVector().push_back(currentPath);
		maxloop->dirty(planeNormal);
		mloops.push_back(maxloop);

		currentPath->pointArray()->setName(currentPath->_name);

		//其实这个不删除也行，因为自成一环的肯定不会再次被遍历到;
		removeBooleanPathFromJunction(currentPath);
		return;
	}

	Junction* endjunction = currentPath->endJunction();
	assert(endjunction);

	if (endjunction->_paths.empty())
	{
		BooleanPath* prePath = currentPath->previousPath();
		removeBooleanPathFromJunction(currentPath);
		traversePath(prePath, mloops, planeNormal);
		return;
	}

	BooleanPath* twinOfCurrentPath = currentPath->twinPath();
	assert(twinOfCurrentPath);

	BooleanPath* nextPath = getClockwiseNearstBooleanPath(endjunction->_paths, twinOfCurrentPath, endjunction->_position, planeNormal);
	if (!nextPath)
	{
		//删除相关path;
		BooleanPath* prePath = currentPath->previousPath();
		removeBooleanPathFromJunction(currentPath);
		traversePath(prePath, mloops, planeNormal);
		return;
	}
	if (!currentPath->nextPath())
	{
		currentPath->nextPath() = nextPath;
	}
	if (nextPath->previousPath())
	{
		//构成了一个环; 删除相关path;
		osg::ref_ptr<DirectedLoop> maxloop = new DirectedLoop;
		BooleanPath* fpath = nextPath->nextPath();
		while (fpath != nextPath)
		{
			assert(fpath);
			fpath->pointArray()->setName(fpath->_name);
			maxloop->booleanPathVector().push_back(fpath);
			removeBooleanPathFromJunction(fpath);
			fpath = fpath->nextPath();
		}
		assert(fpath);
		fpath->pointArray()->setName(fpath->_name);
		removeBooleanPathFromJunction(fpath);
		maxloop->booleanPathVector().push_back(fpath);
		maxloop->dirty(planeNormal);
		mloops.push_back(maxloop);
		return;
	}
	nextPath->previousPath() = currentPath;
	traversePath(nextPath, mloops, planeNormal);
}

void SliceOperator::traverseJunction(Junction* junction, DirectedLoopVector& mloops, const osg::Vec3d& planeNormal)
{
	if (junction->_paths.empty()) return;
	BooleanPath* currentPath = *(junction->_paths.begin());
	if (!currentPath) return;
	traversePath(currentPath, mloops, planeNormal);
}

LayerAttribuate * SliceOperator::getLayerAttribuate(osg::Node* geologicalNode, const osg::Vec3d & pos)
{
	osg::Vec3d updir = _isOnEarth ? pos : osg::Vec3d(0, 0, 1); updir.normalize();
	osg::Vec3d start = pos;
	osg::Vec3d end = start + updir*5000.0;

	osg::ref_ptr<osgUtil::LineSegmentIntersector> intersector = new osgUtil::LineSegmentIntersector(start, end);
	osgUtil::IntersectionVisitor intersectVisitor(intersector.get());
	geologicalNode->accept(intersectVisitor);

	if (intersector->containsIntersections())
	{
		osgUtil::LineSegmentIntersector::Intersections& intersections = intersector->getIntersections();
		osgUtil::LineSegmentIntersector::Intersections::iterator hitr = intersections.begin();
		const osg::NodePath& np = hitr->nodePath;
		if (!np.empty())
		{
			osg::ref_ptr<osg::Node> node = dynamic_cast<osg::Node *>(np[1]);
			if (node)
			{
				GetGeoAttribuate gga;
				node->accept(gga);
				osg::ref_ptr<LayerAttribuate> attribuate = new LayerAttribuate("", gga.getColor(), gga.getTexture());
				return attribuate.release();
			}
		}
	}

	return 0L;
}

void SliceOperator::generateDirectedLoops(JunctionVector& joints, const osg::Vec3d& planeNormal, DirectedLoopVector& outLoops)
{
	if (joints.empty()) return;

	for (JunctionVector::iterator it = joints.begin(); it != joints.end(); ++it)
	{
		Junction* junction = *it;
		if (!junction) continue;

		//先设置preious path和next path为空;
		for (JunctionVector::iterator i = joints.begin(); i != joints.end(); ++i)
		{
			for (BooleanPathVector::iterator bi = (*i)->_paths.begin(); bi != (*i)->_paths.end(); ++bi)
			{
				(*bi)->previousPath() = 0L;
				(*bi)->nextPath() = 0L;
			}
		}
		traverseJunction(junction, outLoops, planeNormal);
	}
}

osg::Node* SliceOperator::generateProfileNode(osg::Node * geologicalModel, const DirectedLoopVector & loops, const osg::Vec3d& normal)
{
	if (loops.empty()) return 0L;

	DirectedLoopVector maxLoops, minLoops;
	for (DirectedLoopVector::const_iterator it = loops.begin(); it != loops.end(); ++it)
	{
		if (!(*it)) continue;
		assert((*it)->type() != DirectedLoop::UNDEFINED);
		if ((*it)->type() == DirectedLoop::MAX_LOOP)
			maxLoops.push_back(*it);
		else if ((*it)->type() == DirectedLoop::MIN_LOOP)
			minLoops.push_back(*it);
	}
	//
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	DirectedLoopVector::const_iterator minIt = minLoops.begin();
	for (; minIt != minLoops.end(); ++minIt)
	{
		DirectedLoop* minloop = *minIt;
		if (!minloop) continue;

		Vec3dArrayVector vas;
		vas.push_back(minloop->pointArray());

		//判断孔洞和岛;
		for (DirectedLoopVector::const_iterator maxIt = maxLoops.begin(); maxIt != maxLoops.end(); ++maxIt)
		{
			if (!(*maxIt)) continue;
			if (minloop->isContains(**maxIt))
			{
				vas.push_back((*maxIt)->pointArray());
			}
		}

		osg::Vec3d posInPolygon = getPointInPolygon(minloop->pointArray());
		osg::ref_ptr<LayerAttribuate> attr = getLayerAttribuate(geologicalModel, posInPolygon);
		if (!attr)continue;

		unsigned int pn = 0;
		for (unsigned int i = 0; i < vas.size(); ++i)
		{
			pn += vas[i]->size();
		}

		osg::ref_ptr<osg::Vec3Array> va = new osg::Vec3Array;
		va->reserve(pn);
		osg::ref_ptr<osg::Vec4Array> ca = new osg::Vec4Array;
		ca->reserve(pn);

		osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
		geom->setVertexArray(va);
		geom->setColorArray(ca, osg::Array::BIND_PER_VERTEX);

		unsigned int totalVertexNum = 0;
		for (Vec3dArrayVector::const_iterator it = vas.begin(); it != vas.end(); ++it)
		{
			unsigned currentVertexNum = 0;
			for (osg::Vec3dArray::const_iterator vaIt = (*it)->begin(); vaIt != (*it)->end(); ++vaIt)
			{
				va->push_back(*vaIt);
				ca->push_back(attr->_color);
				totalVertexNum++;
				currentVertexNum++;
			}
			geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP, totalVertexNum - currentVertexNum, currentVertexNum));
		}

		osgUtil::Tessellator tscx;
		tscx.setTessellationNormal(normal);
		tscx.setTessellationType(osgUtil::Tessellator::TESS_TYPE_GEOMETRY);
		tscx.setBoundaryOnly(false);
		tscx.setWindingType(osgUtil::Tessellator::TESS_WINDING_ODD);
		tscx.retessellatePolygons(*geom);

		geode->addDrawable(geom);
	}


	{//test
		osg::ref_ptr<osg::PolygonMode> pm = new osg::PolygonMode;
		pm->setMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::FILL);
		geode->getOrCreateStateSet()->setAttributeAndModes(pm, osg::StateAttribute::OVERRIDE | osg::StateAttribute::ON);
		geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	}

	return geode.release();
}

namespace
{
	// test
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
}

void SliceOperator::slice(osg::Group * geologicalModel, const osg::Plane& slicePlane, osg::Group* root)
{
	osg::ref_ptr<osgUtil::PlaneIntersector> intersector = new osgUtil::PlaneIntersector(slicePlane);
	intersector->setRecordHeightsAsAttributes(true);

	Vec3dArrayVector vaVector;
	for (unsigned int i = 0; i < geologicalModel->getNumChildren(); ++i)
	{
		osgUtil::IntersectionVisitor intersectionVisitor;
		intersectionVisitor.reset();
		intersectionVisitor.setIntersector(intersector.get());
		geologicalModel->getChild(i)->accept(intersectionVisitor);

		osgUtil::PlaneIntersector::Intersections& intersections = intersector->getIntersections();
		if (intersections.empty())
		{
			continue;
		}

		osgUtil::PlaneIntersector::Intersections::iterator itr;
		for (itr = intersections.begin(); itr != intersections.end(); ++itr)
		{
			osgUtil::PlaneIntersector::Intersection& intersection = *itr;
			if (intersection.matrix.valid())
			{
				osg::ref_ptr<osg::Vec3dArray> vva = new osg::Vec3dArray;
				for (auto pitr = intersection.polyline.begin(); pitr != intersection.polyline.end(); ++pitr)
				{
					osg::Vec3d p = intersection.matrix.valid() ? (*pitr) * (*intersection.matrix) : (*pitr);
					vva->push_back(p);
				}
				intersection.matrix = 0;

				removeRepeat(vva);//模型重复部分需要删除冗余的点;
				if (!vva->empty())
				{
					vaVector.push_back(vva);
				}
			}
		}
	}

	JunctionVector junctions;
	generateJunctions(vaVector, slicePlane.getNormal(), junctions);
	DirectedLoopVector loops;
	generateDirectedLoops(junctions, slicePlane.getNormal(), loops);

	//{//
	//	for (unsigned int i = 0; i < vaVector.size(); ++i)
	//	{
	//		osg::ref_ptr<osg::Vec3Array> va = new osg::Vec3Array;
	//		for (unsigned int j = 0; j < vaVector[i]->size(); ++j)
	//		{
	//			va->push_back(vaVector[i]->at(j));
	//		}
	//		root->addChild(generatePoint(va, osg::Vec4(0, 1, 0, 1), 5.0f));
	//	}
	//}

	{//junction
		osg::ref_ptr<osg::Vec3Array> va = new osg::Vec3Array;
		for (JunctionVector::const_iterator it = junctions.begin(); it != junctions.end(); ++it)
		{
			va->push_back((*it)->_position);
		}
		osg::ref_ptr<osg::Geode> pg = generatePoint(va, osg::Vec4(1, 1, 1, 1), 20.0f);
		root->addChild(pg);
	}

	{
		osg::ref_ptr<osg::Vec3Array> va = new osg::Vec3Array;
		for (unsigned int i = 0; i < loops.size(); ++i)
		{
			for (unsigned int j = 0; j < loops[i]->booleanPathVector().size(); ++j)
			{
				va->push_back(loops[i]->booleanPathVector()[j]->pointArray()->at(0));
			}
		}
		root->addChild(generatePoint(va, osg::Vec4(1, 0, 0, 1), 10.0f));
	}

	{
		for (unsigned int i = 0; i < loops.size(); ++i)
		{
			const BooleanPathVector& vav = loops[i]->booleanPathVector();
			for (unsigned int j = 0; j < vav.size(); ++j)
			{
				osg::Vec3dArray* va = vav[j]->pointArray();
				osg::ref_ptr<osg::Vec3Array> newva = new osg::Vec3Array;
				for (unsigned int k = 0; k < va->size(); ++k)
				{
					newva->push_back(va->at(k));
				}
				osg::Vec4 color;
				//if (
				//	va->getName() == "A0" ||
				//	va->getName() == "A1" ||
				//	va->getName() == "A2" ||
				//	va->getName() == "A3"
				//	)
				//{
				//	color = osg::Vec4(0, 0, 1, 1);
				//	root->addChild(generatePoint(newva, color, 10.0f));
				//}
				//if (
				//	va->getName() == "B00" ||
				//	va->getName() == "B1" ||
				//	va->getName() == "B2" ||
				//	va->getName() == "B3" 
				//	)
				//{
				//	color = osg::Vec4(0, 1, 0, 1);
				//	root->addChild(generatePoint(newva, color, 10.0f));
				//}

				if (
					//va->getName() == "C0" ||
					va->getName() == "C11" /*||
					va->getName() == "C2" ||
					va->getName() == "C3"*/
					)
				{
					color = osg::Vec4(0.8, 0.8, 1, 1);
					root->addChild(generatePoint(newva, color, 10.0f));
				}
				//if (
				//	va->getName() == "D0" ||
				//	va->getName() == "C1" ||
				//	va->getName() == "C2" ||
				//	va->getName() == "C3"
				//	)
				//{
				//	color = osg::Vec4(0, 0.5, 0.5, 1);
				//	root->addChild(generatePoint(newva, color, 10.0f));
				//}
				//if (
				//	va->getName() == "E0" ||
				//	va->getName() == "E1" ||
				//	va->getName() == "E22" ||
				//	va->getName() == "E2" ||
				//	va->getName() == "E33"
				//	)
				//{
				//	color = osg::Vec4(0.1, 1.0, 0.5, 1);
				//	root->addChild(generatePoint(newva, color, 10.0f));
				//}
				if (
					//va->getName() == "F0" ||
					va->getName() == "F11" /*||
					va->getName() == "F22" ||
					va->getName() == "F2" ||
					va->getName() == "F33"*/
					)
				{
					color = osg::Vec4(0.1, 0.5, 0.1, 1);
					root->addChild(generatePoint(newva, color, 10.0f));
				}
				//if (
				//	va->getName() == "G0" ||
				//	va->getName() == "G1" ||
				//	va->getName() == "G22" ||
				//	va->getName() == "G2" ||
				//	va->getName() == "G33"
				//	)
				//{
				//	color = osg::Vec4(0.5, 0.5, 1.0, 1);
				//	root->addChild(generatePoint(newva, color, 10.0f));
				//}
				//if (
				//	va->getName() == "H0" ||
				//	va->getName() == "H1" ||
				//	va->getName() == "H22" ||
				//	va->getName() == "H2" ||
				//	va->getName() == "H33"
				//	)
				//{
				//	color = osg::Vec4(1.0, 0.5, 1.0, 1);
				//	root->addChild(generatePoint(newva, color, 10.0f));
				//}
			}
		}
	}

	if (root)
	{
		osg::ref_ptr<osg::Node> profnode = generateProfileNode(geologicalModel, loops, slicePlane.getNormal());
		if (profnode)
		{
			root->addChild(profnode);
		}
	}
}
