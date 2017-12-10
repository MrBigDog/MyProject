#include "SliceOperator.h"
#include "GeneratorHelp.h"
#include "PlaneIntersector.h"
#include "clipper.hpp"
#include <osg/Vec3>
#include <osg/Point>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/PolygonMode>
#include <osg/TriangleFunctor>
#include <osg/TriangleIndexFunctor>
#include <osgUtil/Tessellator>
#include <osgUtil/TriStripVisitor>
#include <osgUtil/RayIntersector>
#include <osgUtil/LineSegmentIntersector>

#include <osgUtil/PlaneIntersector>

#include <iostream>
#include <assert.h>
#include <sstream>

//#define GEO_EPSILOND 1.192092896e-05
#define COMPUTEJUNCTION_EPSILON 0.0001//计算Junction;
#define PATHREPEAT_EPSILON 0.0001//判断BooleanPath是否重合;

//#define PI2 6.28318530717958647692

//问题1：取材质可能出错(解决方案：取外边缘点，然后求交，注意求交射线的方向要向外);
namespace
{
	//struct TrangleOptimizer
	//{
	//	void set(osg::Geometry* geom, const osg::Vec3d& normal)
	//	{
	//		_geom = geom;
	//		_trangles = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES);
	//		_va = dynamic_cast<osg::Vec3Array*>(geom->getVertexArray());
	//		_na = dynamic_cast<osg::Vec3Array*>(geom->getNormalArray());
	//		_ca = dynamic_cast<osg::Vec4Array*>(geom->getColorArray());
	//		_normal = normal;
	//	}

	//	inline void operator()(unsigned int p1, unsigned int p2, unsigned int p3)
	//	{
	//		osg::Vec3d dir1 = normalize((*_va)[p2] - (*_va)[p1]);
	//		osg::Vec3d dir2 = normalize((*_va)[p3] - (*_va)[p2]);
	//		double f = (dir1^dir2)* _normal;
	//		if (f < 0)
	//		{
	//			_trangles->push_back(p1);
	//			_trangles->push_back(p2);
	//			_trangles->push_back(p3);
	//		}
	//		else
	//		{
	//			_trangles->push_back(p3);
	//			_trangles->push_back(p2);
	//			_trangles->push_back(p1);
	//		}
	//	}

	//	osg::Geometry* getNewGeometry()
	//	{
	//		osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
	//		geom->setVertexArray(_va);
	//		geom->setNormalArray(_na, osg::Array::BIND_PER_VERTEX);
	//		geom->setColorArray(_ca, osg::Array::BIND_PER_VERTEX);
	//		geom->addPrimitiveSet(_trangles);
	//		geom->setStateSet(_geom->getStateSet());

	//		return geom.release();
	//	}

	//private:
	//	osg::Vec3d _normal;
	//	osg::ref_ptr<osg::Geometry> _geom;
	//	osg::ref_ptr<osg::Vec3Array> _va;
	//	osg::ref_ptr<osg::Vec3Array> _na;
	//	osg::ref_ptr<osg::Vec4Array> _ca;
	//	osg::ref_ptr<osg::DrawElementsUInt> _trangles;
	//};

	// 参考《 Real-Time Collision Detection 》;
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
				t = osg::clampTo(t, 0.0, 1.0);
			}
			else {
				double c = (d1* r);
				if (e <= EPSILON) {
					t = 0.0f;
					s = osg::clampTo(-c / a, 0.0, 1.0);
				}
				else {
					double b = (d1* d2);
					double denom = a*e - b*b;
					if (denom != 0.0f) {
						s = osg::clampTo((b*f - c*e) / denom, 0.0, 1.0);
					}
					else s = 0.0f;
					t = (b*s + f) / e;
					if (t < 0.0f) {
						t = 0.0f;
						s = osg::clampTo(-c / a, 0.0, 1.0);
					}
					else if (t > 1.0f) {
						t = 1.0f;
						s = osg::clampTo((b - c) / a, 0.0, 1.0);
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

	static osg::Vec3dArray* getSubArray(const osg::Vec3dArray* va, int first, int last)
	{
		if (!va || last < first) return 0L;

		int maxIndex = va->size() - 1;
		int f = osg::clampTo(first, 0, maxIndex);
		int l = osg::clampTo(last, 0, maxIndex);
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
		int f = osg::clampTo(first, 0, maxIndex);
		int l = osg::clampTo(last, 0, maxIndex);
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
		for (int i = va->size() - 1; i > -1; --i)
		{
			reVa->push_back(va->at(i));
		}
		//for (osg::Vec3dArray::const_reverse_iterator it = va->rbegin(); it != va->rend(); ++it)
		//{
		//	reVa->push_back((*it));
		//}
		return reVa.release();
	}

	static bool isRepeat(const osg::Vec3dArray*va, const osg::Vec3d& p, double epsilon)
	{
		for (osg::Vec3dArray::const_iterator it = va->begin(); it != va->end(); ++it)
		{
			if (isVec3dEqual(*it, p, epsilon)) return true;
		}
		return false;
	}

	static bool isAlmostSameArray(const osg::Vec3dArray* va1, const osg::Vec3dArray* va2, double epsilon)
	{
		if (va1->size() != va2->size()) return false;
		osg::Vec3dArray::const_iterator it1;
		for (it1 = va1->begin(); it1 != va1->end(); ++it1)
		{
			if (!isRepeat(va2, *it1, epsilon)) return false;
		}
		return true;
		//for (it1 = va1->begin(), it2 = va2->begin(); it1 != va1->end(), it2 != va2->end(); ++it1, ++it2)
		//{
		//	if (!isVec3dEqual(*it1, *it2, epsilon))
		//	{
		//		return false;
		//	}
		//}
		//return true;
	}

	static double getLineStripLength(const osg::Vec3dArray* va)
	{
		double l = 0.0;
		for (osg::Vec3dArray::const_iterator it = va->begin() + 1; it != va->end(); ++it)
		{
			l += (*it - *(it - 1)).length();
		}
		return l;
	}

	template<typename ARRAY_TYPE = osg::Vec3dArray>
	static osg::Vec3d getNearestFarerThan(const ARRAY_TYPE* va, const osg::Vec3d& p, double dis)
	{
		double sumDis = 0.0;
		ARRAY_TYPE::const_iterator it;
		for (it = va->begin() + 1; it != va->end(); ++it)
		{
			sumDis += ((*it) - (*(it - 1))).length();
			if (sumDis >= dis) return *it;
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
	Path(osg::Vec3dArray* va) : _va(va), _isClosed(false)
	{
		computeBB();
		_junctions.clear();
	}

	void addJunction(unsigned int i, Junction* junction)
	{
		if (!_junctions.empty())
		{
			JunctionPairVector::const_iterator it;
			for (it = _junctions.begin(); it != _junctions.end(); ++it)
			{
				if ((*it).first == i)
					return;
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
		_booleanPathVector.clear();
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
		if (_bb.radius() < loop.boundingBox().radius()) return false;
		if (!_bb.intersects(loop.boundingBox())) return false;

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
			if (isRepeat) return false;
		}

		//求交点数量;--判断一个不黏连的点就够了（因为算法可以保证不会出现部分覆盖的情况）。
		osg::Vec3d dir = normalize(loop.pointArray()->at(0) - loop.pointArray()->at(1));
		for (unsigned int i = 0; i < loop.pointArray()->size(); ++i)
		{
			const osg::Vec3d& p1 = loop.pointArray()->at(i);
			if (isRepeat(_va, p1, GEO_EPSILOND))//选择不黏连的点;
			{
				continue;
			}

			int hitNum = 0;
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
		computeBB();
		type() = isArrayInClockwiseDirection<osg::Vec3dArray>(_va, clipPlaneNormal) ? MAX_LOOP : MIN_LOOP;
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
		osg::Vec3d v0 = getNearestFarerThan<osg::Vec3dArray>(path->pointArray(), c, getLineStripLength(path->pointArray())*0.1);
		osg::Vec3d dir0 = normalize(v0 - c);
		for (BooleanPathVector::const_iterator it = paths.begin(); it != paths.end(); ++it)
		{
			if (path == *it) continue;
			osg::Vec3d v1 = getNearestFarerThan<osg::Vec3dArray>((*it)->pointArray(), c, getLineStripLength((*it)->pointArray())*0.1);
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
		if (startJunction->_paths.empty()) return true;

		BooleanPathVector::iterator it;
		it = std::find(startJunction->_paths.begin(), startJunction->_paths.end(), path);
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
			if (geode.getNumDrawables() > 1)
			{
				std::cout << geode.getNumDrawables() << std::endl;
			}
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

	struct LayerAttribuateGetter
	{
		LayerAttribuateGetter() :_isFinish(false), _pos(osg::Vec3d()), _h(DBL_MIN) {}
		//取最高点（水平切的时候仍然会有问题）--方案：取外边缘;
		inline void operator() (const osg::Vec3& v1, const osg::Vec3& v2, const osg::Vec3& v3, bool)
		{
			//面积过小的不考虑（因为如果在边缘的话碰撞检测时候会造成误差）;
			double a = (v2 - v1).length();
			double b = (v3 - v2).length();
			double c = (v3 - v1).length();
			double s = (a + b + c)*0.5;
			double s2 = s*(s - a)*(s - b)*(s - c);
			if (s2 < 0.1) return;

			osg::Vec3d p = (v1 + v2 + v3) / 3.0;//坐标过大的时候可能会有比较大的误差;
			double h = _isOnearth ? p.length2() : p.z();
			if (h > _h)
			{
				_pos = p;
				_h = h;
				if (!_isFinish)
					_isFinish = true;
			}
		}

		LayerAttribuate* getAttribuate(osg::Group* geoModel, bool isonearth)
		{
			if (!geoModel) return 0L;
			if (!_isFinish) { std::cout << "no triangle" << std::endl; return 0L; }

			osg::Vec3d updir = isonearth ? normalize(_pos) : osg::Vec3d(0, 0, 1);
			osg::Node* nearestNode = 0L;
			double nearest = DBL_MAX;
			for (unsigned int i = 0; i < geoModel->getNumChildren(); ++i)
			{
				osg::ref_ptr<osgUtil::RayIntersector> intersector = new osgUtil::RayIntersector(_pos, updir);
				osgUtil::IntersectionVisitor intersectVisitor(intersector.get());
				geoModel->getChild(i)->accept(intersectVisitor);
				if (!intersector->containsIntersections()) continue;
				osgUtil::RayIntersector::Intersections& intersections = intersector->getIntersections();
				if (intersections.size() % 2 == 0) continue;//与闭合区域相交--忽略(不闭合情况下有风险，需要改);
				osg::Vec3d averagePos = intersections.begin()->getWorldIntersectPoint();
				//osgUtil::RayIntersector::Intersections::iterator hitr;
				//for (hitr = intersections.begin(); hitr != intersections.end(); ++hitr)
				//{
				//	osg::Vec3d hitP = hitr->getWorldIntersectPoint();
				//	averagePos += hitP;
				//}
				//averagePos /= (double)intersections.size();
				double length2 = (averagePos - _pos).length2();
				if (length2 < nearest)
				{
					nearest = length2;
					nearestNode = geoModel->getChild(i);
				}
			}
			if (!nearestNode) return 0L;

			GetGeoAttribuate gga;
			nearestNode->accept(gga);
			return new LayerAttribuate("", gga.getColor(), gga.getTexture());
		}

	private:
		bool _isFinish;
		bool _isOnearth;
		double _h;
		osg::Vec3d _pos;
	};
}

//////////////////////////////////////////////////////////////////////////
SliceOperator::SliceOperator(bool isOnEarth) :_isOnEarth(isOnEarth) {}
SliceOperator::~SliceOperator() {}

void SliceOperator::generateJunctions(Path* p1, Path* p2, JunctionVector& joints)
{
#if 1
	for (unsigned int i = 0; i < p1->_va->size(); ++i)
	{
		const osg::Vec3d& v1 = p1->_va->at(i);
		for (unsigned int j = 0; j < p2->_va->size(); ++j)
		{
			const osg::Vec3d& v2 = p2->_va->at(j);
			if (isVec3dEqual(v1, v2, /*COMPUTEJUNCTION_EPSILON*/GEO_EPSILOND))
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
	unsigned pathIndex = 0;
	PathVector paths; paths.reserve(vas.size());
	for (Vec3dArrayVector::const_iterator it = vas.begin(); it != vas.end(); ++it)
	{
		std::stringstream ss; ss << pathIndex << "_";
		osg::ref_ptr<Path> path = new Path((*it));
		path->_name = ss.str();
		paths.push_back(path);
		pathIndex++;
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

	//生成关联路径;
	BooleanPathVector bpvector;
	bpvector.reserve(joints.size() * 4); joints.clear();
	for (PathVector::const_iterator it = paths.begin(); it != paths.end(); ++it)
	{
		Path* path = *it;
		removeContinuousRepeatJunctions(path);
		generateBooleanPaths(path, path->_isClosed, bpvector);
	}
	outJoints.reserve(bpvector.size() * 2);
	for (BooleanPathVector::const_iterator it = bpvector.begin(); it != bpvector.end(); ++it)
	{
		outJoints.push_back((*it)->startJunction());
		outJoints.push_back((*it)->endJunction());
	}
}

//删除相邻的Junction（保留首尾），避免黏连情况;
void SliceOperator::removeContinuousRepeatJunctions(Path* path)
{
	std::sort(path->_junctions.begin(), path->_junctions.end(), compairJunction);
	if (path->_junctions.size() > 2)
	{
		Path::JunctionPairVector junctionsToRemoved;
		Path::JunctionPairVector::iterator jit0 = path->_junctions.begin();
		Path::JunctionPairVector::iterator jit1 = jit0 + 1;
		Path::JunctionPairVector::iterator jit2 = jit1 + 1;
		for (; jit2 != path->_junctions.end(); ++jit0, ++jit1, ++jit2)
		{
			if ((*jit1).first - (*jit0).first == 1 && (*jit2).first - (*jit1).first == 1)
			{
				junctionsToRemoved.push_back(*jit1);
			}
		}
		if (path->_isClosed)
		{
			if (path->_junctions[0].first == 0 &&
				path->_junctions[1].first == 1 &&
				path->_junctions[path->_junctions.size() - 1].first == path->_va->size() - 1)
			{
				junctionsToRemoved.push_back(path->_junctions[0]);
			}
			if (path->_junctions[0].first == 0 &&
				path->_junctions[path->_junctions.size() - 2].first == path->_va->size() - 2 &&
				path->_junctions[path->_junctions.size() - 1].first == path->_va->size() - 1)
			{
				junctionsToRemoved.push_back(path->_junctions[path->_junctions.size() - 1]);
			}
		}
		for (unsigned int i = 0; i < junctionsToRemoved.size(); ++i)
		{
			Path::JunctionPairVector::iterator jit;
			jit = std::find(path->_junctions.begin(), path->_junctions.end(), junctionsToRemoved[i]);
			if (jit != path->_junctions.end())
			{
				path->_junctions.erase(jit);
			}
		}
	}
}

bool isBooleanPathRepeat(const BooleanPathVector& paths, const BooleanPath* path)
{
	for (BooleanPathVector::const_iterator it = paths.begin(); it != paths.end(); ++it)
	{
		if (path->isClosed())
		{
			if (!(*it)->isClosed()) continue;
			if (isAlmostSameArray(path->pointArray(), (*it)->pointArray(), /*PATHREPEAT_EPSILON*/GEO_EPSILOND))
			{
				return true;
			}
		}
		else
		{
			if ((*it)->isClosed()) continue;
			if (path->startJunction() != (*it)->startJunction() &&
				path->endJunction() != (*it)->endJunction())
			{
				continue;
			}
			if (isAlmostSameArray(path->pointArray(), (*it)->pointArray(), /*PATHREPEAT_EPSILON*/GEO_EPSILOND))
			{
				return true;
			}
		}
	}
	return false;
}

void SliceOperator::generateBooleanPaths(Path* path, bool isClosedPath, BooleanPathVector &bpvector)
{
	if (path->_junctions.size() < 2)
	{
		//自己单独成闭环--如果junction的数量为1，忽略此junction;
		osg::ref_ptr<osg::Vec3dArray> va0 = getSubArray(path->_va, 0, path->_va->size() - 1);
		assert(va0); assert(!va0->empty());
		if (va0 && !va0->empty())
		{
			//在任意位置添加两个junction，这里选起点和终点;
			osg::ref_ptr<Junction> sjunction = new Junction(va0->at(0));
			osg::ref_ptr<Junction> ejunction = new Junction(va0->at(va0->size() - 1));

			osg::ref_ptr<BooleanPath> bp0 = new BooleanPath(va0);
			bp0->startJunction() = sjunction;
			bp0->endJunction() = ejunction;
			bp0->isClosed() = true;
			bp0->_name = path->_name + "0";
			if (!isBooleanPathRepeat(bpvector, bp0))
			{
				osg::ref_ptr<osg::Vec3dArray> va1 = getReverseArray(va0);
				osg::ref_ptr<BooleanPath> bp1 = new BooleanPath(va1);
				bp1->startJunction() = ejunction;
				bp1->endJunction() = sjunction;
				bp1->isClosed() = true;
				bp1->_name = path->_name + "00";

				bp0->twinPath() = bp1;
				bp1->twinPath() = bp0;
				bpvector.push_back(bp0);
				bpvector.push_back(bp1);
				sjunction->_paths.push_back(bp0);
				ejunction->_paths.push_back(bp1);
			}
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
			if (nextVertexIndex == currentVertexIndex) continue;

			//往后一个取值，确保不出现重复的点;
			int firstindex = currentVertexIndex == path->_va->size() - 1 ? 0 : currentVertexIndex + 1;
			osg::ref_ptr<osg::Vec3dArray> va0 = getSubArray(path->_va, firstindex, nextVertexIndex);
			assert(va0); assert(!va0->empty());
			if (!va0 || va0->empty()) continue;

			std::stringstream ss; ss << i;
			osg::ref_ptr<BooleanPath> bp0 = new BooleanPath(va0);
			bp0->startJunction() = currentJunction;
			bp0->endJunction() = nextJunction;
			bp0->_name = path->_name + ss.str();
			if (isBooleanPathRepeat(bpvector, bp0))
			{
				continue;
			}

			int lastindex = nextVertexIndex == 0 ? path->_va->size() - 1 : nextVertexIndex - 1;
			osg::ref_ptr<osg::Vec3dArray> tempva1 = getSubArray(path->_va, currentVertexIndex, lastindex);
			osg::ref_ptr<osg::Vec3dArray> va1 = getReverseArray(tempva1);
			osg::ref_ptr<BooleanPath> bp1 = new BooleanPath(va1);
			bp1->startJunction() = nextJunction;
			bp1->endJunction() = currentJunction;
			bp1->_name = bp0->_name + ss.str();

			bp0->twinPath() = bp1;
			bp1->twinPath() = bp0;
			bpvector.push_back(bp0);
			bpvector.push_back(bp1);
			currentJunction->_paths.push_back(bp0);
			nextJunction->_paths.push_back(bp1);
		}
		if (path->_isClosed)
		{
			Junction* currentJunction = path->_junctions[path->_junctions.size() - 1].second;
			Junction* nextJunction = path->_junctions[0].second;

			unsigned int currendVertexindex = path->_junctions[path->_junctions.size() - 1].first;
			unsigned int nextVertexIndex = path->_junctions[0].first;

			int firstindex = currendVertexindex == path->_va->size() - 1 ? 0 : currendVertexindex + 1;
			osg::ref_ptr<osg::Vec3dArray> va0 = getSubArrayOfLoop(path->_va, firstindex, nextVertexIndex);
			assert(va0); assert(!va0->empty());
			if (va0 && !va0->empty())
			{
				std::stringstream ss; ss << path->_junctions.size() - 1;
				osg::ref_ptr<BooleanPath> bp0 = new BooleanPath(va0);
				bp0->startJunction() = currentJunction;
				bp0->endJunction() = nextJunction;
				bp0->_name = path->_name + ss.str();
				if (!isBooleanPathRepeat(bpvector, bp0))
				{
					int lastindex = nextVertexIndex == 0 ? path->_va->size() - 1 : nextVertexIndex - 1;
					osg::ref_ptr<osg::Vec3dArray> tempva1 = getSubArrayOfLoop(path->_va, currendVertexindex, lastindex);
					osg::ref_ptr<osg::Vec3dArray> va1 = getReverseArray(tempva1);
					osg::ref_ptr<BooleanPath> bp1 = new BooleanPath(va1);
					bp1->startJunction() = nextJunction;
					bp1->endJunction() = currentJunction;
					bp1->_name = bp0->_name + ss.str();

					bp0->twinPath() = bp1;
					bp1->twinPath() = bp0;
					bpvector.push_back(bp0);
					bpvector.push_back(bp1);
					currentJunction->_paths.push_back(bp0);
					nextJunction->_paths.push_back(bp1);
				}
			}
		}
	}
}

//递归遍历闭环;
void SliceOperator::traversePath(BooleanPath* currentPath, DirectedLoopVector& mloops, const osg::Vec3d& planeNormal)
{
	if (!currentPath) return;
	assert(!hasBooleanPathDeleted(currentPath));

	if (currentPath->isClosed())//自成一环;
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
	//强制设置nextPath能一定程度增强容错能力;
	currentPath->nextPath() = nextPath;

	if (nextPath->previousPath())
	{
		//构成了一个环，保存并删除相关path;
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

osg::Vec2Array* SliceOperator::generateTextureCoord(const osg::Vec3d& normal, osg::ref_ptr<osg::Vec3Array> va)
{
	osg::Vec3d nn = normalize(normal);
	osg::Vec3d p1 = getNearestFarerThan<osg::Vec3Array>(va, va->at(0), 10.0);

	osg::Vec3d normal_s = normalize(p1 - va->at(0));
	osg::Vec3d normal_t = normalize(nn^normal_s);

	osg::Plane plane_s(normal_s, va->at(0));
	osg::Plane plane_t(normal_t, va->at(0));
	osg::ref_ptr<osg::Vec2Array> ta = new osg::Vec2Array();
	ta->reserve(va->size());
	for (osg::Vec3Array::const_iterator it = va->begin(); it != va->end(); ++it)
	{
		double ts = plane_s.distance(*it);
		double ty = plane_t.distance(*it);
		ta->push_back(osg::Vec2(ts, ty));
	}

	return ta.release();
}

bool loopCompair(osg::ref_ptr<DirectedLoop> l1, osg::ref_ptr<DirectedLoop> l2)
{
	return l1->boundingBox().radius() > l2->boundingBox().radius();
}

osg::Node* SliceOperator::generateProfileNode_closed(osg::Node * geologicalModel, const DirectedLoopVector & loops, const osg::Vec3d& normal)
{
	if (loops.empty()) return 0L;

	DirectedLoopVector maxLoops, minLoops;
	for (DirectedLoopVector::const_iterator it = loops.begin(); it != loops.end(); ++it)
	{
		assert((*it)->type() != DirectedLoop::UNDEFINED);

		if (!(*it)) continue;
		if ((*it)->type() == DirectedLoop::MAX_LOOP)
			maxLoops.push_back(*it);
		else if ((*it)->type() == DirectedLoop::MIN_LOOP)
			minLoops.push_back(*it);
	}

	osg::ref_ptr<osg::Group> root = new osg::Group;
	DirectedLoopVector::const_iterator minIt = minLoops.begin();
	for (; minIt != minLoops.end(); ++minIt)
	{
		DirectedLoop* minloop = *minIt;
		if (!minloop) continue;

		Vec3dArrayVector vas;
		vas.push_back(minloop->pointArray());

		//判断孔洞和岛;
		DirectedLoopVector::const_iterator maxIt;
		for (maxIt = maxLoops.begin(); maxIt != maxLoops.end(); ++maxIt)
		{
			if (!(*maxIt)) continue;
			if (minloop->isContains(**maxIt))
			{
				vas.push_back((*maxIt)->pointArray());
			}
		}
		unsigned int pn = 0;
		for (unsigned int i = 0; i < vas.size(); ++i)
		{
			pn += vas[i]->size();
		}

		osg::ref_ptr<osg::Geode> geode = new osg::Geode;
		osg::ref_ptr<osg::Vec3Array> va = new osg::Vec3Array; va->reserve(pn);
		osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
		geode->addDrawable(geom);
		geom->setVertexArray(va);

		unsigned int totNum = 0;
		for (Vec3dArrayVector::const_iterator it = vas.begin(); it != vas.end(); ++it)
		{
			unsigned int vetNum = 0;
			for (osg::Vec3dArray::const_iterator vaIt = (*it)->begin(); vaIt != (*it)->end(); ++vaIt)
			{
				va->push_back((*vaIt));
				totNum++; vetNum++;
			}
			geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POLYGON, totNum - vetNum, vetNum));
		}

		osg::ref_ptr<osg::Vec3Array> na = new osg::Vec3Array(va->size());
		na->assign(va->size(), normal);
		geom->setNormalArray(na, osg::Array::BIND_PER_VERTEX);

		osgUtil::Tessellator tscx;
		tscx.setTessellationNormal((normal));
		tscx.setTessellationType(osgUtil::Tessellator::TESS_TYPE_GEOMETRY);
		tscx.setBoundaryOnly(false);
		tscx.setWindingType(osgUtil::Tessellator::TESS_WINDING_ODD);
		tscx.retessellatePolygons(*geom);

		GetGeoAttribuate gga;
		geologicalModel->accept(gga);
		osg::ref_ptr<LayerAttribuate>attr = new LayerAttribuate("", gga.getColor(), gga.getTexture());

		osg::ref_ptr<GeologicalMaterial> gm = new GeologicalMaterial(attr->_color, attr->_texture);
		geom->setUserData(gm);

		if (!attr->_texture)//没有纹理的话，只设置颜色;
		{
			osg::ref_ptr<osg::Vec4Array> ca = new osg::Vec4Array;
			ca->reserve(pn);
			for (unsigned int i = 0; i < va->size(); ++i)
			{
				ca->push_back(attr->_color);
			}
			geom->setColorArray(ca, osg::Array::BIND_PER_VERTEX);
		}
		else//设置纹理;
		{
			//PlaneGeometryOptimizer pgo(va->at(0), normal, (va->at(1) - va->at(0)) ^ normal);
			//geode->accept(pgo);
			osg::ref_ptr<osg::Vec2Array> ta = generateTextureCoord(normal, va);
			if (ta)
			{
				geom->setTexCoordArray(0, ta);
				geom->getOrCreateStateSet()->setTextureAttributeAndModes(0, attr->_texture);
			}
		}

		//{
		//	static bool wwww = false;
		//	if (!wwww)
		//	{
		//		convertGeomToShp(geom, "myshapefile.shp");
		//		wwww = true;
		//	}
		//}

		//osg::TriangleIndexFunctor<TrangleOptimizer> to;
		//to.set(geom, normal);
		//geom->accept(to);

		//osg::ref_ptr<osg::Geometry> newgeom = to.getNewGeometry();
		//if (newgeom)
		//{
		//	geode->addDrawable(newgeom);
		//}
		//geode->addDrawable(geom);
		root->addChild(geode);
	}

	{//test
		osg::ref_ptr<osg::PolygonMode> pm = new osg::PolygonMode;
		pm->setMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::FILL);
		root->getOrCreateStateSet()->setAttributeAndModes(pm, osg::StateAttribute::OVERRIDE | osg::StateAttribute::ON);
		root->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	}

	return root.release();
}

osg::Node* SliceOperator::generateProfileNode(osg::Group * geologicalModel, const DirectedLoopVector & loops, const osg::Vec3d& normal)
{
	if (loops.empty()) return 0L;

	DirectedLoopVector maxLoops, minLoops;
	for (DirectedLoopVector::const_iterator it = loops.begin(); it != loops.end(); ++it)
	{
		assert((*it)->type() != DirectedLoop::UNDEFINED);
		if (!(*it)) continue;
		if ((*it)->type() == DirectedLoop::MAX_LOOP)
			maxLoops.push_back(*it);
		else if ((*it)->type() == DirectedLoop::MIN_LOOP)
			minLoops.push_back(*it);
	}

	osg::ref_ptr<osg::Group> root = new osg::Group;
	DirectedLoopVector::const_iterator minIt = minLoops.begin();
	for (; minIt != minLoops.end(); ++minIt)
	{
		DirectedLoop* minloop = *minIt;
		if (!minloop) continue;

		Vec3dArrayVector vas;
		vas.push_back(minloop->pointArray());

		//判断孔洞和岛;
		DirectedLoopVector::const_iterator maxIt;
		for (maxIt = maxLoops.begin(); maxIt != maxLoops.end(); ++maxIt)
		{
			if (!(*maxIt)) continue;
			if (minloop->isContains(**maxIt))
			{
				vas.push_back((*maxIt)->pointArray());
			}
		}
		unsigned int pn = 0;
		for (unsigned int i = 0; i < vas.size(); ++i)
		{
			pn += vas[i]->size();
		}

		osg::ref_ptr<osg::Geode> geode = new osg::Geode;
		osg::ref_ptr<osg::Vec3Array> va = new osg::Vec3Array; va->reserve(pn);
		osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
		geode->addDrawable(geom);
		geom->setVertexArray(va);

		unsigned int totNum = 0;
		for (Vec3dArrayVector::const_iterator it = vas.begin(); it != vas.end(); ++it)
		{
			unsigned int vetNum = 0;
			for (osg::Vec3dArray::const_iterator vaIt = (*it)->begin(); vaIt != (*it)->end(); ++vaIt)
			{
				va->push_back((*vaIt));
				totNum++; vetNum++;
			}
			geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POLYGON, totNum - vetNum, vetNum));
		}

		osgUtil::Tessellator tscx;
		tscx.setTessellationNormal((normal));
		tscx.setTessellationType(osgUtil::Tessellator::TESS_TYPE_GEOMETRY);
		tscx.setBoundaryOnly(false);
		tscx.setWindingType(osgUtil::Tessellator::TESS_WINDING_ODD);
		tscx.retessellatePolygons(*geom);

		osgUtil::TriStripVisitor tsv;
		tsv.stripify(*geom);

		osg::TriangleFunctor<LayerAttribuateGetter> lag;
		geom->accept(lag);
		osg::ref_ptr<LayerAttribuate>attr = lag.getAttribuate(geologicalModel, _isOnEarth);

		if (!attr.valid())
		{
			continue;
		}

		osg::ref_ptr<GeologicalMaterial> gm = new GeologicalMaterial(attr->_color, attr->_texture);
		geom->setUserData(gm);

		if (!attr->_texture)//没有纹理的话，只设置颜色;
		{
			osg::ref_ptr<osg::Vec4Array> ca = new osg::Vec4Array;
			ca->reserve(pn);
			for (unsigned int i = 0; i < va->size(); ++i)
			{
				ca->push_back(attr->_color);
			}
			geom->setColorArray(ca, osg::Array::BIND_PER_VERTEX);
		}
		else//设置纹理;
		{
			osg::ref_ptr<osg::Vec2Array> ta = generateTextureCoord(normal, va);
			if (ta)
			{
				geom->setTexCoordArray(0, ta);
				geom->getOrCreateStateSet()->setTextureAttributeAndModes(0, attr->_texture);
			}
		}
		//geode->addDrawable(geom);
		root->addChild(geode);
	}

	{//test
		osg::ref_ptr<osg::PolygonMode> pm = new osg::PolygonMode;
		pm->setMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::FILL);
		root->getOrCreateStateSet()->setAttributeAndModes(pm, osg::StateAttribute::OVERRIDE | osg::StateAttribute::ON);
		root->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	}

	return root.release();
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

osg::Node* SliceOperator::slice_closed(osg::Group* geologicalModel, const osg::Plane& slicePlane)
{
	const osg::Vec3d& planenormal = slicePlane.getNormal();
	osg::ref_ptr<osgUtil::PlaneIntersector> intersector = new osgUtil::PlaneIntersector(slicePlane);
	osg::ref_ptr<osg::Group> profileRoot = new osg::Group;
	for (unsigned int i = 0; i < geologicalModel->getNumChildren(); ++i)
	{
		//intersector->setRecordHeightsAsAttributes(true);
		osgUtil::IntersectionVisitor intersectionVisitor;
		intersectionVisitor.reset();
		intersectionVisitor.setIntersector(intersector.get());
		geologicalModel->getChild(i)->accept(intersectionVisitor);

		osgUtil::PlaneIntersector::Intersections& intersections = intersector->getIntersections();
		if (intersections.empty()) continue;

		Vec3dArrayVector vaVector;
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
				removeRepeat<osg::Vec3dArray>(vva);
				if (vva->size() > 2)
				{
					vaVector.push_back(vva);
				}
			}
		}
		if (vaVector.empty()) continue;

		DirectedLoopVector loopVector;
		for (Vec3dArrayVector::const_iterator it = vaVector.begin(); it != vaVector.end(); ++it)
		{
			osg::ref_ptr<DirectedLoop> loop = new DirectedLoop();
			loop->booleanPathVector().push_back(new BooleanPath(*it));
			loop->dirty(planenormal);
			loopVector.push_back(loop);
		}
		for (DirectedLoopVector::iterator it0 = loopVector.begin(); it0 != loopVector.end(); ++it0)
		{
			if ((*it0)->type() == DirectedLoop::MAX_LOOP)
			{
				(*it0)->pointArray() = getReverseArray((*it0)->pointArray());
				(*it0)->type() = DirectedLoop::MIN_LOOP;
			}
		}

		std::sort(loopVector.begin(), loopVector.end(), loopCompair);
		for (DirectedLoopVector::iterator it0 = loopVector.begin(); it0 != loopVector.end(); ++it0)
		{
			if ((*it0)->type() == DirectedLoop::MAX_LOOP) continue;
			for (DirectedLoopVector::iterator it1 = loopVector.begin(); it1 != loopVector.end(); ++it1)
			{
				if ((*it0)->isContains(**it1))
				{
					(*it1)->pointArray() = getReverseArray((*it1)->pointArray());
					(*it1)->type() = DirectedLoop::MAX_LOOP;
				}
			}
		}

		osg::ref_ptr<osg::Node> profileNode = generateProfileNode_closed(geologicalModel->getChild(i), loopVector, planenormal);
		if (profileNode)
		{
			profileRoot->addChild(profileNode);
		}
	}
	return profileRoot.release();
}

osg::Node* SliceOperator::slice(osg::Group * geologicalModel, const osg::Plane& slicePlane, osg::Group* root)
{

	Vec3dArrayVector vaVector;
	for (unsigned int i = 0; i < geologicalModel->getNumChildren(); ++i)
	{
		osg::ref_ptr<PlaneIntersector> intersector = new PlaneIntersector(slicePlane);
		intersector->setRecordHeightsAsAttributes(true);

		osgUtil::IntersectionVisitor intersectionVisitor;
		intersectionVisitor.reset();
		intersectionVisitor.setIntersector(intersector.get());
		geologicalModel->getChild(i)->accept(intersectionVisitor);

		PlaneIntersector::Intersections& intersections = intersector->getIntersections();
		if (intersections.empty())
		{
			continue;
		}

		PlaneIntersector::Intersections::iterator itr;
		for (itr = intersections.begin(); itr != intersections.end(); ++itr)
		{
			PlaneIntersector::Intersection& intersection = *itr;
			if (intersection.matrix.valid())
			{
				osg::ref_ptr<osg::Vec3dArray> vva = new osg::Vec3dArray;
				for (auto pitr = intersection.polyline.begin(); pitr != intersection.polyline.end(); ++pitr)
				{
					osg::Vec3d p = intersection.matrix.valid() ? (*pitr) * (*intersection.matrix) : (*pitr);
					vva->push_back(p);
				}
				intersection.matrix = 0;
				//删除重复冗余的点;
				removeRepeat<osg::Vec3dArray>(vva);
				//if (vva->size() > 2)
				{
					vaVector.push_back(vva);
				}
				//else
				//{
				std::cout << "ignor: " << vva->size() << std::endl;
				//}
			}
		}
	}

	//unsigned pathsize = 0;
	//osg::BoundingSphered bs;
	//for (Vec3dArrayVector::const_iterator it = vaVector.begin(); it != vaVector.end(); ++it)
	//{
	//	for (osg::Vec3dArray::const_iterator vit = (*it)->begin(); vit != (*it)->end(); ++vit)
	//	{
	//		bs.expandBy(*vit);
	//		pathsize++;
	//	}
	//}

	//double radius = bs.radius()*0.25;

	//unsigned int circleSegment = 36;
	//ClipperLib::Path backGroundPath;
	//backGroundPath.resize(circleSegment);

	//double deltaAngle = osg::PI*2.0 / (double)circleSegment;
	//osg::Quat rot(deltaAngle, osg::Vec3d(0, 0, 1));
	//osg::Vec3d dir(1, 0, 0);
	//for (unsigned int ci = 0; ci < circleSegment; ++ci)
	//{
	//	dir = rot*dir;
	//	osg::Vec3d p = bs.center() + dir*radius;
	//	backGroundPath[ci].X = p[0];
	//	backGroundPath[ci].Y = p[1];
	//}

	//osg::Vec3dArray* baseva = vaVector[0];
	//ClipperLib::Path backGroundPath;
	//backGroundPath.resize(baseva->size());
	//for (unsigned int i = 0; i < baseva->size(); ++i)
	//{
	//	backGroundPath[i].X = baseva->at(i)[0];// << ClipperLib::IntPoint((*it)[0], (*it)[1]);
	//	backGroundPath[i].Y = baseva->at(i)[1];
	//}

	//ClipperLib::Clipper cliper;
	////cliper.AddPath(backGroundPath, ClipperLib::ptClip, true);
	//for (unsigned int vanum = 0; vanum < vaVector.size(); ++vanum)
	//{
	//	osg::Vec3dArray* va = vaVector[vanum];
	//	if (!va) continue;

	//	ClipperLib::Path path; path.resize(va->size());
	//	for (unsigned int j = 0; j < va->size(); ++j)
	//	{
	//		path[j].X = va->at(j)[0];
	//		path[j].Y = va->at(j)[1];
	//	}
	//	cliper.AddPath(path, ClipperLib::ptSubject, false);
	//}

	//ClipperLib::PolyTree tree;
	//cliper.Execute(ClipperLib::ctIntersection, tree/*sol*/, ClipperLib::pftNonZero, ClipperLib::pftNonZero);

	JunctionVector junctions;
	generateJunctions(vaVector, slicePlane.getNormal(), junctions);
	DirectedLoopVector loops;
	generateDirectedLoops(junctions, slicePlane.getNormal(), loops);

	osg::ref_ptr<osg::Node> profileNode = generateProfileNode(geologicalModel, loops, slicePlane.getNormal());
	//return profileNode.release();
	if (root)
	{
		if (root)
		{
			//osg::ref_ptr<osg::Vec3Array> pathVa = new osg::Vec3Array;
			//for (ClipperLib::PolyNodes::const_iterator it = tree.Childs.begin(); it != tree.Childs.end(); ++it)
			//{
			//	const ClipperLib::Path& path = (*it)->Contour;
			//	for (ClipperLib::Path::const_iterator pit = path.begin(); pit != path.end(); ++pit)
			//	{
			//		osg::Vec3d p = osg::Vec3d((*pit).X, (*pit).Y, -350.0);
			//		pathVa->push_back(p);
			//	}
			//}
			//root->addChild(generatePoint(pathVa, osg::Vec4(0, 1, 1, 1), 10.0f));
			//osg::ref_ptr<osg::Node> profnode = generateProfileNode(geologicalModel, loops, slicePlane.getNormal());
			//if (profnode)
			//{
			//	root->addChild(profnode);
			//}

			//osg::ref_ptr<osg::Vec3Array> va = new osg::Vec3Array;
			//va->push_back(osg::Vec3d(447.57189941406250, 113.00033569335938, -54.368816375732422));
			//va->push_back(osg::Vec3d(498.33602905273438, 711.26470947265625, -54.368816375732422));
			//va->push_back(osg::Vec3d(614.14633178710938, 1057.0754394531250, -54.368816375732422));
			//va->push_back(osg::Vec3d(527.62503051757813, 550.47839355468750, -54.368816375732422));
			//va->push_back(osg::Vec3d(255.19452667236328, 255.22538757324219, -54.368816375732422));
			//va->push_back(osg::Vec3d(420.58874511718750, 1079.5689086914063, -54.368816375732422));
			//root->addChild(generatePoint(va, osg::Vec4(0, 1, 1, 1), 10.0f));
		}

		{//
			for (unsigned int i = 0; i < vaVector.size(); ++i)
			{
				osg::ref_ptr<osg::Vec3Array> va = new osg::Vec3Array;
				for (unsigned int j = 0; j < vaVector[i]->size(); ++j)
				{
					va->push_back(vaVector[i]->at(j));
				}
				root->addChild(generatePoint(va, osg::Vec4(0, 1, 0, 1), 5.0f));
			}
		}

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
			//osg::ref_ptr<osg::Vec3Array> va = new osg::Vec3Array;
			//for (unsigned int i = 0; i < loops.size(); ++i)
			//{
			//	for (unsigned int j = 0; j < loops[i]->booleanPathVector().size(); ++j)
			//	{
			//		osg::Vec3d v0 = getNearestFarerThan(
			//			loops[i]->booleanPathVector()[j]->pointArray(),
			//			loops[i]->booleanPathVector()[j]->pointArray()->at(0),
			//			getLineStripLength(loops[i]->booleanPathVector()[j]->pointArray())*0.1);
			//		va->push_back(v0);
			//		//va->push_back(loops[i]->booleanPathVector()[j]->pointArray()->at(0));
			//	}
			//}
			//root->addChild(generatePoint(va, osg::Vec4(1, 0, 0, 1), 10.0f));
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
					if (
						va->getName() == "9_0" /*||
						va->getName() == "0_1" ||
						va->getName() == "0_2" ||
						va->getName() == "0_3"*/
						)
					{
						color = osg::Vec4(0, 0, 1, 1);
						root->addChild(generatePoint(newva, color, 10.0f));
					}
					if (
						va->getName() == "1_0" /*||
						va->getName() == "1_1" ||
						va->getName() == "1_2" ||
						va->getName() == "1_3"*/
						)
					{
						color = osg::Vec4(0, 0, 1, 1);
						root->addChild(generatePoint(newva, color, 10.0f));
					}
					if (
						va->getName() == "2_0" /*||
						va->getName() == "2_1" ||
						va->getName() == "2_2" ||
						va->getName() == "2_3" */
						)
					{
						color = osg::Vec4(0, 1, 0, 1);
						root->addChild(generatePoint(newva, color, 10.0f));
					}
					if (
						va->getName() == "3_0" /*||
						va->getName() == "3_11" ||
						va->getName() == "3_22" ||
						va->getName() == "3_3"*/
						)
					{
						color = osg::Vec4(0.8, 0.8, 1, 1);
						root->addChild(generatePoint(newva, color, 10.0f));
					}
					if (
						va->getName() == "4_0" /*||
						va->getName() == "4_1" ||
						va->getName() == "4_2" ||
						va->getName() == "4_3"*/
						)
					{
						color = osg::Vec4(0, 0.5, 0.5, 1);
						root->addChild(generatePoint(newva, color, 10.0f));
					}
					if (
						va->getName() == "5_0" /*||
						va->getName() == "5_1" ||
						va->getName() == "5_22" ||
						va->getName() == "5_2" ||
						va->getName() == "5_33"*/)
					{
						color = osg::Vec4(0.1, 1.0, 0.5, 1);
						root->addChild(generatePoint(newva, color, 10.0f));
					}
					if (
						va->getName() == "6_0" /*||
						va->getName() == "6_11" ||
						va->getName() == "6_22" ||
						va->getName() == "6_2" ||
						va->getName() == "6_33"*/)
					{
						color = osg::Vec4(0.1, 0.5, 0.1, 1);
						root->addChild(generatePoint(newva, color, 10.0f));
					}
					if (
						va->getName() == "7_0" /*||
						va->getName() == "7_1" ||
						va->getName() == "7_22" ||
						va->getName() == "7_2" ||
						va->getName() == "7_33"*/)
					{
						color = osg::Vec4(0.5, 0.5, 1.0, 1);
						root->addChild(generatePoint(newva, color, 10.0f));
					}
					if (
						va->getName() == "10_0" /*||
						va->getName() == "8_1" ||
						va->getName() == "8_22" ||
						va->getName() == "8_2" ||
						va->getName() == "8_33"*/)
					{
						color = osg::Vec4(1.0, 0.5, 1.0, 1);
						root->addChild(generatePoint(newva, color, 10.0f));
					}
				}
			}
		}
	}
	//return 0L;
	return profileNode.release();
}
