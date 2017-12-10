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

		osg::Vec3d d1 = q1 - p1;
		osg::Vec3d d2 = q2 - p2;
		osg::Vec3d r = p1 - p2;
		double a = d1* d1;
		double e = d2* d2;
		double f = d2* r;
		double res_dis = DBL_MAX;

		if (a <= EPSILON && e <= EPSILON)
		{
			s = t = 0.0f;
			c1 = p1;
			c2 = p2;
			res_dis = (c1 - c2)*(c1 - c2);
		}
		else
		{
			if (a <= EPSILON)
			{
				s = 0.0f;
				t = f / e;
				t = Clamp(t, 0.0f, 1.0f);
			}
			else
			{
				double c = (d1* r);
				if (e <= EPSILON)
				{
					t = 0.0f;
					s = Clamp(-c / a, 0.0f, 1.0f);
				}
				else
				{
					double b = (d1* d2);
					double denom = a*e - b*b;
					if (denom != 0.0f)
					{
						s = Clamp((b*f - c*e) / denom, 0.0f, 1.0f);
					}
					else s = 0.0f;
					t = (b*s + f) / e;
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

	void addJunction(unsigned int i, Junction* junction)
	{
		if (!_junctions.empty())
		{
			const osg::Vec3 p0 = junction->_position;
			for (JunctionPairVector::iterator it = _junctions.begin(); it != _junctions.end(); ++it)
			{
				const osg::Vec3 p1 = (*it).second->_position;
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

public:
	bool _isClosed;
	osg::BoundingBox _bb;
	JunctionPairVector _junctions;
	//osg::ref_ptr<LayerAttribuate> _attribuate;
	osg::ref_ptr<const osg::Vec3Array> _va;
};

//----------------------------------------------------------------------
class BooleanPath :public osg::Referenced
{
public:
	BooleanPath(osg::Vec3Array* va)
		: _isClosed(false)
		, _isPartOfClosed(false)
		, _va(va)
		, _previous(0L)
		, _next(0L)
		, _twin(0L)
		, _startJunction(0L)
		, _endJunction(0L)
	{}

	bool & isClosed() { return _isClosed; }
	const bool& isClosed() const { return _isClosed; }

	bool & isPartOfClosed() { return _isPartOfClosed; }
	const bool & isPartOfClosed() const { return _isPartOfClosed; }

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

	//osg::ref_ptr<LayerAttribuate>& attribuate() { return _attribuate; }
	//const osg::ref_ptr<LayerAttribuate>& attribuate() const { return _attribuate; }

private:
	COMPUTE_BB_METHOD

private:
	bool						  _isClosed;     //是否自闭合;
	bool						  _isPartOfClosed;
	osg::BoundingBox			  _bb;			//矩形包围框;
	osg::ref_ptr<Junction>		  _startJunction;//起点;
	osg::ref_ptr<Junction>		  _endJunction;  //终点;
	osg::ref_ptr<BooleanPath>	  _previous;     //前一路径;
	osg::ref_ptr<BooleanPath>	  _next;         //下一路径;
	osg::ref_ptr<BooleanPath>	  _twin;         //对应的反向路径;
	osg::ref_ptr<osg::Vec3Array>  _va;			//对应的参数曲线;
	//osg::ref_ptr<LayerAttribuate> _attribuate;
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

	BooleanPathVector& booleanPathVector() { return _booleanPathVector; }
	const BooleanPathVector& booleanPathVector() const { return _booleanPathVector; }

	osg::ref_ptr<osg::Vec3Array>& pointArray() { return _va; }
	const osg::ref_ptr<osg::Vec3Array>& pointArray() const { return _va; }

	//osg::ref_ptr<LayerAttribuate>& attribuate() { return _attribuate; }
	//const osg::ref_ptr<LayerAttribuate>& attribuate() const { return _attribuate; }

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

		//重合?;
		if (_va->size() == loop.pointArray()->size())
		{
			bool isRepeat = true;

			osg::Vec3Array::const_iterator it1;
			osg::Vec3Array::const_reverse_iterator it2;
			for (it1 = _va->begin(), it2 = loop.pointArray()->rbegin();
				it1 != _va->begin(), it2 != loop.pointArray()->rend();
				++it1, ++it2)
			{
				if (!osg::equivalent((*it1)[0], (*it2)[0], GEO_EPSILON) ||
					!osg::equivalent((*it1)[1], (*it2)[1], GEO_EPSILON) ||
					!osg::equivalent((*it1)[2], (*it2)[2], GEO_EPSILON))
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

		//求交点数量;--判断一个点就够了。
		osg::Vec3d dir = loop.pointArray()->at(0) - loop.pointArray()->at(1);
		dir.normalize();
		for (unsigned int i = 0; i < loop.pointArray()->size(); ++i)
		{
			osg::Vec3d p = loop.pointArray()->at(i);
			osg::Vec3Array::iterator it = std::find(_va->begin(), _va->end(), p);
			if (it != _va->end())
			{
				continue;
			}
			int hitNum = 0;
			osg::Vec3d p1 = loop.pointArray()->at(i);
			osg::Vec3d p2 = p1 + dir*FLT_MAX;
			for (unsigned int j = 0; j < _va->size() - 1; ++j)
			{
				osg::Vec3d p3 = _va->at(j);
				osg::Vec3d p4 = _va->at(j + 1);
				if (isSegmentsIntersect3(p1, p2, p3, p4, osg::Vec3d()))
					hitNum++;
			}
			if (hitNum % 2 == 0)
				return false;
			return true;
		}
		return true;
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
		type() = isArrayInClockwiseDirection(_va, clipPlaneNormal) ? MAX_LOOP : MIN_LOOP;
	}
private:
	COMPUTE_BB_METHOD

private:
	Type _type;
	osg::BoundingBox _bb;
	Vec3ArrayVector _vaVector;
	BooleanPathVector _booleanPathVector;
	osg::ref_ptr<osg::Vec3Array> _va;
	//osg::ref_ptr<LayerAttribuate>  _attribuate;
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

		if (sjunction->_paths.empty())
		{
			return;
		}

		BooleanPathVector::iterator it;
		it = std::find(sjunction->_paths.begin(), sjunction->_paths.end(), fpath);

		if (it == sjunction->_paths.end())
		{
			return;
		}
		sjunction->_paths.erase(it);
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
			if (!osg::equivalent(v[0], fc[0], GEO_EPSILON) ||
				!osg::equivalent(v[1], fc[1], GEO_EPSILON) ||
				!osg::equivalent(v[2], fc[2], GEO_EPSILON))
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

			if (minClockwiseAngle > cloclwiseangle)
			{
				minClockwiseAngle = cloclwiseangle;
				resultPath = *it;
			}
		}
		return resultPath;
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
		const osg::Vec3 p1 = p;
		for (JunctionVector::const_iterator it = joints.begin(); it != joints.end(); ++it)
		{
			const osg::Vec3 p0 = (*it)->_position;
			if (osg::equivalent(p0[0], p1[0], GEO_EPSILON) &&
				osg::equivalent(p0[1], p1[1], GEO_EPSILON) &&
				osg::equivalent(p0[2], p1[2], GEO_EPSILON))
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

void SliceOperator::generateJunctions(const SliceTrailVector& vas, const osg::Vec3d& planeNormal, JunctionVector& outJoints)
{
	PathVector paths; paths.reserve(vas.size());
	for (SliceTrailVector::const_iterator it = vas.begin(); it != vas.end(); ++it)
	{
		osg::ref_ptr<Path> path = new Path((*it)->_va);
		path->_attribuate = (*it)->_attribuate;
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

	//path中的junction为空的时候说明是闭环;
	for (PathVector::iterator it = paths.begin(); it != paths.end(); ++it)
	{
		Path* path = *it;
		if (!path->_junctions.empty()) continue;

		osg::ref_ptr<Junction> junction0 = new Junction(path->_va->at(0));
		osg::ref_ptr<Junction> junction1 = new Junction(path->_va->at(path->_va->size()/2));
		path->addJunction(0, junction0);
		path->addJunction(path->_va->size()/2, junction1);
		joints.push_back(junction0);
		joints.push_back(junction1);
		if (!path->_isClosed)
		{
			path->_isClosed = true;
		}
	}

	//生成关联路径;
	BooleanPathVector bpvector; bpvector.reserve(joints.size()); joints.clear();
	for (PathVector::const_iterator it = paths.begin(); it != paths.end(); ++it)
	{
		Path* path = *it;
		std::sort(path->_junctions.begin(), path->_junctions.end(), compairJunction);
		generateBooleanPaths(path, path->_isClosed, bpvector);
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
	if (path->_junctions.size() < 2)
	{
		return;
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

		if (!va0 || va0->empty() || !va1 || va1->empty())
		{
			continue;
		}

		va0->push_back(nextJunction->_position);
		va1->push_back(currentJunction->_position);

		osg::ref_ptr<BooleanPath> bp0 = new BooleanPath(va0);
		bp0->startJunction() = currentJunction;
		bp0->endJunction() = nextJunction;
		bp0->isPartOfClosed() = path->_isClosed;
		bp0->attribuate() = path->_attribuate;

		osg::ref_ptr<BooleanPath> bp1 = new BooleanPath(va1);
		bp1->startJunction() = nextJunction;
		bp1->endJunction() = currentJunction;
		bp1->isPartOfClosed() = path->_isClosed;
		bp1->attribuate() = path->_attribuate;

		bp0->twinPath() = bp1;
		bp1->twinPath() = bp0;

		currentJunction->_paths.push_back(bp0);
		nextJunction->_paths.push_back(bp1);

		bpvector.push_back(bp0);
		bpvector.push_back(bp1);
	}
	if (isClosedPath)
	{
		Junction* currentJunction = path->_junctions[path->_junctions.size() - 1].second;
		Junction* nextJunction = path->_junctions[0].second;

		unsigned int currendVertexindex = path->_junctions[path->_junctions.size() - 1].first;
		unsigned int nextVertexIndex = path->_junctions[0].first;
		if (currendVertexindex == nextVertexIndex)
		{
			return;
		}
		osg::ref_ptr<osg::Vec3Array> va0 = getSubArrayOfLoop(path->_va, currendVertexindex, nextVertexIndex);
		if (!va0 || va0->empty())
		{
			//return;
			std::cout << "closed" << std::endl;
		}

		osg::ref_ptr<BooleanPath> bp0 = new BooleanPath(va0);
		bp0->startJunction() = currentJunction;
		bp0->endJunction() = nextJunction;
		bp0->isPartOfClosed() = path->_isClosed;
		bp0->attribuate() = path->_attribuate;

		osg::ref_ptr<osg::Vec3Array> va1 = getReverseArray(va0);
		osg::ref_ptr<BooleanPath> bp1 = new BooleanPath(va1);
		bp1->startJunction() = nextJunction;
		bp1->endJunction() = currentJunction;
		bp1->isPartOfClosed() = path->_isClosed;
		bp1->attribuate() = path->_attribuate;

		bp0->twinPath() = bp1;
		bp1->twinPath() = bp0;

		currentJunction->_paths.push_back(bp0);
		nextJunction->_paths.push_back(bp1);

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

	if (endjunction->_paths.empty())
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

	BooleanPath* nextPath = getClockwiseNearstBooleanPath(endjunction->_paths, twinOfCurrentPath, endjunction->_position, planeNormal);
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
		maxloop->booleanPathVector().push_back(nextPath);
		while (fpath != nextPath->previousPath())
		{
			if (!fpath)
			{
				break;
			}
			maxloop->pointArrayVector().push_back(fpath->pointArray());
			maxloop->booleanPathVector().push_back(fpath->nextPath());

			removeBooleanPathFromJunction(fpath);
			fpath = fpath->nextPath();
		}
		if (fpath)
		{
			maxloop->pointArrayVector().push_back(fpath->pointArray());
			removeBooleanPathFromJunction(fpath);
		}

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
			for (BooleanPathVector::iterator bi = (*i)->_paths.begin(); bi != (*i)->_paths.end(); ++bi)
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

osg::Node* SliceOperator::generateProfileNode(const DirectedLoopVector & loops)
{
	DirectedLoopVector maxLoops, minLoops;
	for (DirectedLoopVector::const_iterator it = loops.begin(); it != loops.end(); ++it)
	{
		if ((*it)->type() == DirectedLoop::UNDEFINED)
			continue;//error;
		else if ((*it)->type() == DirectedLoop::MAX_LOOP)
			maxLoops.push_back(*it);
		else if ((*it)->type() == DirectedLoop::MIN_LOOP)
			minLoops.push_back(*it);
		else
		{
			continue;//error;
		}
	}
	//
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	for (DirectedLoopVector::const_iterator minIt = minLoops.begin(); minIt != minLoops.end(); ++minIt)
	{
		DirectedLoop* minloop = *minIt;

		Vec3ArrayVector vas;
		vas.push_back(minloop->pointArray());

		BooleanPathVector bps;
		for (unsigned int i = 0; i < minloop->booleanPathVector().size(); ++i)
		{
			bps.push_back(minloop->booleanPathVector()[i]);
		}

		//判断孔洞和岛;
		for (DirectedLoopVector::const_iterator maxIt = maxLoops.begin(); maxIt != maxLoops.end(); ++maxIt)
		{
			if (minloop->isContains(**maxIt))
			{
				vas.push_back((*maxIt)->pointArray());
				for (unsigned int i = 0; i < (*maxIt)->booleanPathVector().size(); ++i)
				{
					bps.push_back((*maxIt)->booleanPathVector()[i]);
				}
			}
		}

		//osg::ref_ptr<LayerAttribuate> attr = getAttribuate(bps);
		//if (!attr)
		//{
		//	continue;
		//}

		unsigned int pn = 0;
		for (unsigned int i = 0; i < vas.size(); ++i)
			pn += vas[i]->size();

		osg::ref_ptr<osg::Vec3Array> va = new osg::Vec3Array;
		va->reserve(pn);
		osg::ref_ptr<osg::Vec4Array> ca = new osg::Vec4Array;
		ca->reserve(pn);

		osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
		geom->setVertexArray(va);
		geom->setColorArray(ca, osg::Array::BIND_PER_VERTEX);

		unsigned int totalVertexNum = 0;
		for (Vec3ArrayVector::const_iterator it = vas.begin(); it != vas.end(); ++it)
		{
			unsigned currentVertexNum = 0;
			for (osg::Vec3Array::const_iterator vaIt = (*it)->begin(); vaIt != (*it)->end(); ++vaIt)
			{
				va->push_back(*vaIt);
				ca->push_back(osg::Vec4(1,1,0,1));
				totalVertexNum++;
				currentVertexNum++;
			}
			geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP, totalVertexNum - currentVertexNum, currentVertexNum));
		}

		osgUtil::Tessellator tscx;
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

namespace {// test
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

	//GeologicalPlaneIntersectOperator gpio(intersector);
	//geologicalModel->accept(gpio);

	SliceTrailVector stVector;
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

		GetGeoAttribuate gga;
		geologicalModel->getChild(i)->accept(gga);
		osg::Vec4 color = gga.getColor();
		osg::ref_ptr<osg::Texture2D> texture = gga.getTexture();

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
					osg::ref_ptr<SliceTrail> sliceTrail = new SliceTrail(vva, new LayerAttribuate("", color, texture));
					stVector.push_back(sliceTrail);
				}
			}
		}
	}

	JunctionVector junctions;
	generateJunctions(/*vaVector*/stVector, slicePlane.getNormal(), junctions);
	DirectedLoopVector loops;
	generateDirectedLoops(junctions, slicePlane.getNormal(), loops);


	{//
		for (unsigned int i = 0; i < stVector.size(); ++i)
		{
			osg::ref_ptr<osg::Vec3Array> va = stVector[i]->_va;
			root->addChild(generatePoint(va, osg::Vec4(1, 0, 0, 1), 10.0f));
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

	if (root)
	{
		root->addChild(generateProfileNode(loops));
	}
}
