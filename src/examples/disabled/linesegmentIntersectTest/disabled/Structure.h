#include <gwUtil/Math.h>
#include <osg/Vec3d>
#include <osg/Array>
#include <osg/ref_ptr>
#include <osg/Referenced>
#include <osg/BoundingBox>

#include <vector>
#include <algorithm>

using namespace gwUtil;

typedef std::vector<osg::ref_ptr<osg::Vec3Array> > VaVector;
typedef std::vector<osg::ref_ptr<const osg::Vec3Array> > ConstVaVector;

bool isPointInLineSegment(const osg::Vec3d& s, const osg::Vec3d& e, const osg::Vec3d& p)
{
	if (isVec3Equal(s, p) || isVec3Equal(e, p))
	{
		return true;
	}
	double disse = (e - s).length();
	if ((p - s).length() > disse || (p - e).length() > disse)
	{
		return false;
	}

	return true;
}

double getSkewLinesDistance(const osg::Vec3d &r1, const osg::Vec3d &r2, const osg::Vec3d &v1, const osg::Vec3d &v2)
{
	osg::Vec3d u1 = r2 - r1;
	osg::Vec3d u2 = v2 - v1;
	osg::Vec3d u3 = u1^u2;
	osg::Vec3d dir = r1 - v2;
	if (u3.length() == 0)
	{
		return 1;
	}

	return std::fabs((dir*u3) / u3.length());
}


//double t;
//osg::Vec3d p = s1 + v1*t;
//osg::Vec3d ps2 = p - s2;
//osg::Vec3d res = ps2 ^ v2;

//(v3 + v1*t) ^ v2;
//v1^v2*t + v3^v2;

//(v3[1] + v1[1] * t)*v2[2] - (v3[2] + v1[2] * t)*v2[1];
//求两条线段的交点;
bool isLinesegmentIntersect(const osg::Vec3d& s1, const osg::Vec3d&e1, const osg::Vec3d& s2, const osg::Vec3d&e2, osg::Vec3d& hit)
{
	if (!osg::equivalent(osg::computeVolume(s1, e1, s2, e2), 0.0f))
	{
		return false;
	}

	osg::Vec3d v1 = e1 - s1;
	osg::Vec3d v2 = e2 - s2;
	osg::Vec3d v3 = s1 - s2;
	osg::Vec3d v4 = -(v3^v2);
	osg::Vec3d v5 = (v1^v2);

	//有v5*t=v4
	double t;
	if (!osg::equivalent(v5[0], 0.0))
	{
		t = v4[0] / v5[0];
	}
	else if (!osg::equivalent(v5[1], 0.0))
	{
		t = v4[1] / v5[1];
	}
	else if (!osg::equivalent(v5[2], 0.0))
	{
		t = v4[2] / v5[2];
	}
	else//平行,（黏连或者不相交）;
	{
		//if (isPointInLineSegment(s1, e1, s2))
		//{
		//	hit = s2;
		//	return true;
		//}
		//else if (isPointInLineSegment(s1, e1, e2))
		//{
		//	hit = e2;
		//	return true;
		//}
		return false;
	}

	if (t > 1.0 || t < 0.0)
		return false;

	osg::Vec3d p = s1 + v1*t;
	if (!isPointInLineSegment(s2, e2, p))
	{
		return false;
	}

	hit = p;
	return true;
}

inline double norm2(const osg::Vec3d v)
{
	return v.x() * v.x() + v.y() * v.y() + v.z() * v.z();
}

inline bool isLinesegmentIntersect1(const osg::Vec3d& s1, const osg::Vec3d&e1, const osg::Vec3d& s2, const osg::Vec3d&e2, osg::Vec3d& hit)
{
	osg::Vec3d da = e1 - s1;
	osg::Vec3d db = e2 - s2;
	osg::Vec3d dc = s2 - s1;

	if (dc* (da^ db) != 0.0) //不共面;
		return false;

	double n2 = norm2(da^db);
	if (osg::equivalent(n2, 0.0))//平行,（黏连或者不相交）;
	{
		//if (isPointInLineSegment(s1, e1, s2))
		//{
		//	hit = s2;
		//	return true;
		//}
		//else if (isPointInLineSegment(s1, e1, e2))
		//{
		//	hit = e2;
		//	return true;
		//}
		return false;
	}

	double s = ((dc^db)*(da^db)) / n2;
	if (s >= 0.0 && s <= 1.0)
	{
		hit = s1 + da * s;
		return true;
	}

	return false;
}

osg::Vec3Array* getSubArray(const osg::Vec3Array* va, unsigned int first, unsigned int last, bool isReverse)
{
	if (last < first) return NULL;

	unsigned int maxIndex = va->size() - 1;
	if (last > maxIndex) last = maxIndex;

	osg::ref_ptr<osg::Vec3Array> subva = new osg::Vec3Array;
	if (!isReverse)
	{
		for (unsigned int j = first; j <= last; ++j)
			subva->push_back(va->at(j));
	}
	else
	{
		for (unsigned int j = last; j >= first; --j)
			subva->push_back(va->at(j));
	}

	return subva;
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

class BooleanPath;
typedef std::vector<osg::ref_ptr<BooleanPath> > BooleanPathVector;

class Junction :public osg::Referenced
{
public:
	Junction(const osg::Vec3d& p) :_position(p) {}
	void setPosition(const osg::Vec3d& p) { _position = p; }
	void addPath(BooleanPath*path) { _paths.push_back(path); }

	//private:
	osg::Vec3d        _position; //交点的位置;
	BooleanPathVector _paths;    //以_position为起点的关联路径链表
};
typedef std::vector<osg::ref_ptr<Junction> > JunctionVector;

class BooleanPath :public osg::Referenced
{
public:
	BooleanPath(osg::Vec3Array* va) :_va(va) {}
	void setClosed(bool isClosed) { _isClosed = isClosed; }
	void computeBB()
	{
		_bb.init();
		for (unsigned int i = 0; i < _va->size(); ++i)
			_bb.expandBy(_va->at(i));
	}
	void setStartJunction(Junction* s) { _startJunction = s; }
	void setEndJunction(Junction* e) { _endJunction = e; }
	void setPreviousPath(BooleanPath* path) { _previous = path; }
	void setNextPath(BooleanPath*path) { _next = path; }
	void setTwinPath(BooleanPath*path) { _twin = path; }
	void setPointArray(osg::Vec3Array* va) { _va = va; }

public:
	bool						 _isClosed;     //是否自闭合;
	osg::BoundingBox			 _bb;			//矩形包围框;
	osg::ref_ptr<Junction>		 _startJunction;//起点;
	osg::ref_ptr<Junction>		 _endJunction;  //终点;
	osg::ref_ptr<BooleanPath>	 _previous;     //前一路径;
	osg::ref_ptr<BooleanPath>	 _next;         //下一路径;
	osg::ref_ptr<BooleanPath>	 _twin;         //对应的反向路径;
	osg::ref_ptr<osg::Vec3Array> _va;			//对应的参数曲线;
};
typedef std::vector<osg::ref_ptr<BooleanPath> > BooleanPathVector;

class Path : public osg::Referenced
{
public:
	typedef std::pair<unsigned int, osg::ref_ptr<Junction> > JunctionPair;
	typedef std::vector<JunctionPair> JunctionPairVector;

public:
	Path(const osg::Vec3Array* va) :_va(va)
	{
		initBB(va);
	}

	void addJunction(unsigned int i, Junction* junction)
	{
		_junctions.push_back(JunctionPair(i, junction));
	}

private:
	void initBB(const osg::Vec3Array*va)
	{
		_bb.init();
		for (unsigned int i = 0; i < va->size(); ++i)
		{
			_bb.expandBy(va->at(i));
		}
	}

public:
	osg::BoundingBox _bb;
	JunctionPairVector _junctions;
	osg::ref_ptr<const osg::Vec3Array> _va;
};
typedef std::vector<osg::ref_ptr<Path> > PathVector;

void getJunctions(Path* p1, Path* p2, JunctionVector& joints)
{
	for (unsigned int i = 0; i < p1->_va->size() - 1; ++i)
	{
		for (unsigned int j = 0; j < p2->_va->size() - 1; ++j)
		{
			osg::Vec3d p;
			if (isLinesegmentIntersect1(p1->_va->at(i), p1->_va->at(i + 1), p2->_va->at(i), p2->_va->at(i + 1), p))
			{
				osg::ref_ptr<Junction> newjoint = new Junction(p);
				joints.push_back(newjoint);
				p1->addJunction(i, newjoint);
				p2->addJunction(j, newjoint);
			}
		}
	}
}

bool compairJunction(Path::JunctionPair& j1, Path::JunctionPair& j2)
{
	return j1.first < j2.first;
}

void getJunctions(const VaVector& vas, JunctionVector& joints)
{
	PathVector paths;
	for (VaVector::const_iterator it = vas.begin(); it != vas.end(); ++it)
	{
		osg::ref_ptr<Path> path = new Path(*it);
		paths.push_back(path);
	}

	//可能有位置重复的junction;
	for (unsigned int i = 0, f = 1; i < paths.size() - 1, f < paths.size() - 1; ++i, ++f)
	{
		Path* path1 = paths[i];
		for (unsigned int j = f; j < paths.size() - 1; ++j)
		{
			Path* path2 = paths[j];
			if (!path1->_bb.intersects(path2->_bb))
			{
				continue;
			}
			getJunctions(path1, path2, joints);
		}
	}

	//生成关联路径;
	for (PathVector::const_iterator it = paths.begin(); it != paths.end(); ++it)
	{
		Path* path = *it;
		std::sort(path->_junctions.begin(), path->_junctions.end(), compairJunction);

		for (unsigned int i = 0; i < path->_junctions.size(); ++i)
		{
			unsigned int currentIndex = path->_junctions[i].first;
			unsigned int previousIndex = i == 0 ? 0 : path->_junctions[i - 1].first;
			unsigned int nextIndex = i == path->_junctions.size() - 1 ? path->_va->size() - 1 : path->_junctions[i + 1].first;

			Junction* preJunction = i == 0 ? NULL : path->_junctions[i - 1].second;
			Junction* curJunction = path->_junctions[i].second;
			Junction* nexJunction = i == path->_junctions.size() - 1 ? NULL : path->_junctions[i + 1].second;

			osg::ref_ptr<osg::Vec3Array> val = getSubArray(path->_va, previousIndex, currentIndex, true);
			if (val != NULL && !val->empty())
			{
				osg::ref_ptr<BooleanPath> bpl = new BooleanPath(val);
				bpl->setStartJunction(curJunction);
				bpl->setEndJunction(preJunction);
				path->_junctions[i].second->addPath(bpl);
			}
			osg::ref_ptr<osg::Vec3Array> var = getSubArray(path->_va, currentIndex, nextIndex, false);
			if (var != NULL && !var->empty())
			{
				osg::ref_ptr<BooleanPath> bpr = new BooleanPath(var);
				bpr->setStartJunction(curJunction);
				bpr->setEndJunction(nexJunction);
				path->_junctions[i].second->addPath(bpr);
			}
		}
	}
	paths.clear();
}


//osg::Vec3Array 

//类似于深度优先遍历;
void traverse(Junction* junction)
{
	//虽然junction->_paths不可能为空，但是还是判断下吧;
	if (junction->_paths.empty())
	{
		return;
	}

	BooleanPathVector::const_iterator pathIt;
	for (pathIt = junction->_paths.begin(); pathIt != junction->_paths.end(); ++pathIt)
	{
		BooleanPath* path = *pathIt;
		//处理path;
		if (path->_previous&&path->_previous != NULL)
		{
			//构成一个环;
			continue;
		}

		Junction* nexJunction = path->_endJunction;
		if (!nexJunction)
		{
			continue;
		}

		BooleanPath* nextPath;

		path->setNextPath(nextPath);
		nextPath->setPreviousPath(path);

		traverse(nexJunction);
	}
}

//遍历路径;
void generateLoop(const JunctionVector& joints)
{
	if (joints.empty()) return;

	Junction* junction = (*joints.begin());
	traverse(junction);
}