/************************************************************************/
//模型切割(Head only) --MrDog 2017.04.19									 ;
/************************************************************************/
#ifndef Cutter_h__
#define Cutter_h__

#include <osg/Polytope>
#include <osg/MatrixTransform>
#include <osg/TriangleIndexFunctor>
#include <osgUtil/MeshOptimizers>
#include <osgUtil/SmoothingVisitor>

#include <osgDB/WriteFile>

struct TriangulateOperator
{
public:
	enum Side { POSITIVE_SIDE, NEGATIVE_SIDE };

public:
	TriangulateOperator()
	{
		init();
	}

	void init()
	{
		vAry_ = 0L; nAry_ = 0L; tAry_ = 0L; cAry_ = 0L;
		geom_ = 0L;
		rcAry_ = 0L; rvAry_ = 0L; rnAry_ = 0L; rtAry_ = 0L;
		lcAry_ = 0L; lvAry_ = 0L; lnAry_ = 0L; ltAry_ = 0L;
	}

	void getGeomProperties(const osg::Geometry& geom)
	{
		geom_ = new osg::Geometry(geom);

		const osg::Vec3Array* va = dynamic_cast<const osg::Vec3Array*>(geom.getVertexArray());
		if (!va) return;
		vAry_ = va;
		rvAry_ = new osg::Vec3Array;
		lvAry_ = new osg::Vec3Array;

		const osg::Vec3Array* na = dynamic_cast<const osg::Vec3Array*>(geom.getNormalArray());
		if (na)
		{
			nAry_ = na;
			rnAry_ = new osg::Vec3Array;
			lnAry_ = new osg::Vec3Array;
		}
		const osg::Vec2Array* ta = dynamic_cast<const osg::Vec2Array*>(geom.getTexCoordArray(0));
		if (ta)
		{
			tAry_ = ta;
			rtAry_ = new osg::Vec2Array;
			ltAry_ = new osg::Vec2Array;
		}
		const osg::Vec4Array* ca = dynamic_cast<const osg::Vec4Array*>(geom.getColorArray());
		if (ca)
		{
			cAry_ = ca;
			rcAry_ = new osg::Vec4Array;
			lcAry_ = new osg::Vec4Array;
		}
	}

	void setCutPlane(const osg::Plane& plane)
	{
		plane_ = plane;
	}

	osg::Geometry* getGeometry(Side side)
	{
		osg::ref_ptr<osg::Vec3Array> va = side == POSITIVE_SIDE ? lvAry_ : rvAry_;
		osg::ref_ptr<osg::Vec3Array> na = side == POSITIVE_SIDE ? lnAry_ : rnAry_;
		osg::ref_ptr<osg::Vec2Array> ta = side == POSITIVE_SIDE ? ltAry_ : rtAry_;
		osg::ref_ptr<osg::Vec4Array> ca = side == POSITIVE_SIDE ? lcAry_ : rcAry_;

		if (!va) return 0L;
		if (va->size() < 3) return 0L;

		osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
		geom->setUseVertexBufferObjects(true);
		geom->setUseDisplayList(false);
		geom->setVertexArray(va);

		int vs = va->size();
		if (na && na->size() == vs)geom->setNormalArray(na, osg::Array::BIND_PER_VERTEX);
		if (ta && ta->size() == vs)geom->setTexCoordArray(0, ta);
		if (ca && ca->size() == vs)geom->setColorArray(ca, osg::Array::BIND_PER_VERTEX);
		else//防止合并Geometry的时候出问题;
		{
			ca = new osg::Vec4Array;
			ca->push_back(osg::Vec4(1, 1, 1, 1));
			geom->setColorArray(ca, osg::Array::BIND_OVERALL);
		}

		geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES, 0, vs));
		geom->setStateSet(geom_->getStateSet());
		geom->setUserData(geom_->getUserData());

		return geom.release();
	}

	//添加顶点的顺序不能变，否则会影响背面裁剪;
	inline void operator()(unsigned int p1, unsigned int p2, unsigned int p3)
	{
		const osg::Vec3d& v1 = (*vAry_)[p1];
		const osg::Vec3d& v2 = (*vAry_)[p2];
		const osg::Vec3d& v3 = (*vAry_)[p3];
		double dis1 = plane_.distance(v1);
		double dis2 = plane_.distance(v2);
		double dis3 = plane_.distance(v3);

		int pointNumOnLeftSide = ((dis1 > 0.0) ? 1 : 0) + ((dis2 > 0.0) ? 1 : 0) + ((dis3 > 0.0) ? 1 : 0);
		if (pointNumOnLeftSide == 3)
		{
			addToVertexArray(v1, v2, v3, POSITIVE_SIDE);
			if (nAry_) addToNormalArray(getNormal(nAry_, p1), getNormal(nAry_, p2), getNormal(nAry_, p3), POSITIVE_SIDE);
			if (cAry_)addToColorArray(getColor(cAry_, p1), getColor(cAry_, p2), getColor(cAry_, p3), POSITIVE_SIDE);
			if (tAry_) addToTextCoordArray(tAry_->at(p1), tAry_->at(p2), tAry_->at(p3), POSITIVE_SIDE);
		}
		else if (pointNumOnLeftSide == 0)
		{
			addToVertexArray(v1, v2, v3, NEGATIVE_SIDE);
			if (nAry_) addToNormalArray(getNormal(nAry_, p1), getNormal(nAry_, p2), getNormal(nAry_, p3), NEGATIVE_SIDE);
			if (cAry_)addToColorArray(getColor(cAry_, p1), getColor(cAry_, p2), getColor(cAry_, p3), NEGATIVE_SIDE);
			if (tAry_) addToTextCoordArray(tAry_->at(p1), tAry_->at(p2), tAry_->at(p3), NEGATIVE_SIDE);
		}
		else if (pointNumOnLeftSide == 1)
		{
			if (dis1 > 0.0)
			{
				double r12 = abs(dis1) / (abs(dis1) + abs(dis2));
				double r13 = abs(dis1) / (abs(dis1) + abs(dis3));
				osg::Vec3d v12 = getLerp(v1, v2, r12);
				osg::Vec3d v13 = getLerp(v1, v3, r13);
				addToVertexArray(v1, v12, v13, POSITIVE_SIDE);
				addToVertexArray(v12, v2, v3, NEGATIVE_SIDE);
				addToVertexArray(v3, v13, v12, NEGATIVE_SIDE);
				if (nAry_)
				{
					osg::Vec3d n12 = getLerp(getNormal(nAry_, p1), getNormal(nAry_, p2), r12);
					osg::Vec3d n13 = getLerp(getNormal(nAry_, p1), getNormal(nAry_, p3), r13);
					addToNormalArray(getNormal(nAry_, p1), n12, n13, POSITIVE_SIDE);
					addToNormalArray(n12, getNormal(nAry_, p2), getNormal(nAry_, p3), NEGATIVE_SIDE);
					addToNormalArray(getNormal(nAry_, p3), n13, n12, NEGATIVE_SIDE);
				}
				if (cAry_)
				{
					osg::Vec4 c12 = getLerp(getColor(cAry_, p1), getColor(cAry_, p2), r12);
					osg::Vec4 c13 = getLerp(getColor(cAry_, p1), getColor(cAry_, p3), r13);
					addToColorArray(getColor(cAry_, p1), c12, c13, POSITIVE_SIDE);
					addToColorArray(c12, getColor(cAry_, p2), getColor(cAry_, p3), NEGATIVE_SIDE);
					addToColorArray(getColor(cAry_, p3), c13, c12, NEGATIVE_SIDE);
				}
				if (tAry_)
				{
					osg::Vec2 t12 = getLerp(tAry_->at(p1), tAry_->at(p2), r12);
					osg::Vec2 t13 = getLerp(tAry_->at(p1), tAry_->at(p3), r13);
					addToTextCoordArray(tAry_->at(p1), t12, t13, POSITIVE_SIDE);
					addToTextCoordArray(t12, tAry_->at(p2), tAry_->at(p3), NEGATIVE_SIDE);
					addToTextCoordArray(tAry_->at(p3), t13, t12, NEGATIVE_SIDE);
				}
			}
			else if (dis2 > 0.0)
			{
				double r12 = abs(dis1) / (abs(dis1) + abs(dis2));
				double r23 = abs(dis2) / (abs(dis2) + abs(dis3));
				osg::Vec3d v12 = getLerp(v1, v2, r12);
				osg::Vec3d v23 = getLerp(v2, v3, r23);
				addToVertexArray(v12, v2, v23, POSITIVE_SIDE);
				addToVertexArray(v1, v12, v23, NEGATIVE_SIDE);
				addToVertexArray(v1, v23, v3, NEGATIVE_SIDE);
				if (nAry_)
				{
					osg::Vec3d n12 = getLerp(getNormal(nAry_, p1), getNormal(nAry_, p2), r12);
					osg::Vec3d n23 = getLerp(getNormal(nAry_, p2), getNormal(nAry_, p3), r23);
					addToNormalArray(n12, getNormal(nAry_, p2), n23, POSITIVE_SIDE);
					addToNormalArray(getNormal(nAry_, p1), n12, n23, NEGATIVE_SIDE);
					addToNormalArray(getNormal(nAry_, p1), n23, getNormal(nAry_, p3), NEGATIVE_SIDE);
				}
				if (cAry_)
				{
					osg::Vec4 c12 = getLerp(getColor(cAry_, p1), getColor(cAry_, p2), r12);
					osg::Vec4 c23 = getLerp(getColor(cAry_, p2), getColor(cAry_, p3), r23);
					addToColorArray(c12, getColor(cAry_, p2), c23, POSITIVE_SIDE);
					addToColorArray(getColor(cAry_, p1), c12, c23, NEGATIVE_SIDE);
					addToColorArray(getColor(cAry_, p1), c23, getColor(cAry_, p3), NEGATIVE_SIDE);
				}
				if (tAry_)
				{
					osg::Vec2 t12 = getLerp(tAry_->at(p1), tAry_->at(p2), r12);
					osg::Vec2 t23 = getLerp(tAry_->at(p2), tAry_->at(p3), r23);
					addToTextCoordArray(t12, tAry_->at(p2), t23, POSITIVE_SIDE);
					addToTextCoordArray(tAry_->at(p1), t12, t23, NEGATIVE_SIDE);
					addToTextCoordArray(tAry_->at(p1), t23, tAry_->at(p3), NEGATIVE_SIDE);
				}
			}
			else if (dis3 > 0.0)
			{
				double r13 = abs(dis1) / (abs(dis1) + abs(dis3));
				double r23 = abs(dis2) / (abs(dis2) + abs(dis3));
				osg::Vec3d v13 = getLerp(v1, v3, r13);
				osg::Vec3d v23 = getLerp(v2, v3, r23);
				addToVertexArray(v3, v13, v23, POSITIVE_SIDE);
				addToVertexArray(v1, v2, v23, NEGATIVE_SIDE);
				addToVertexArray(v1, v23, v13, NEGATIVE_SIDE);
				if (nAry_)
				{
					osg::Vec3d n13 = getLerp(getNormal(nAry_, p1), getNormal(nAry_, p3), r13);
					osg::Vec3d n23 = getLerp(getNormal(nAry_, p2), getNormal(nAry_, p3), r23);
					addToNormalArray(getNormal(nAry_, p3), n13, n23, POSITIVE_SIDE);
					addToNormalArray(getNormal(nAry_, p1), getNormal(nAry_, p2), n23, NEGATIVE_SIDE);
					addToNormalArray(getNormal(nAry_, p1), n23, n13, NEGATIVE_SIDE);
				}
				if (cAry_)
				{
					osg::Vec4 c13 = getLerp(getColor(cAry_, p1), getColor(cAry_, p3), r13);
					osg::Vec4 c23 = getLerp(getColor(cAry_, p2), getColor(cAry_, p3), r23);
					addToColorArray(getColor(cAry_, p3), c13, c23, POSITIVE_SIDE);
					addToColorArray(getColor(cAry_, p1), getColor(cAry_, p2), c23, NEGATIVE_SIDE);
					addToColorArray(getColor(cAry_, p1), c23, c13, NEGATIVE_SIDE);
				}
				if (tAry_)
				{
					osg::Vec2 t13 = getLerp(tAry_->at(p1), tAry_->at(p3), r13);
					osg::Vec2 t23 = getLerp(tAry_->at(p2), tAry_->at(p3), r23);
					addToTextCoordArray(tAry_->at(p3), t13, t23, POSITIVE_SIDE);
					addToTextCoordArray(tAry_->at(p1), tAry_->at(p2), t23, NEGATIVE_SIDE);
					addToTextCoordArray(tAry_->at(p1), t23, t13, NEGATIVE_SIDE);
				}
			}
		}
		else if (pointNumOnLeftSide == 2)
		{
			if (dis1 < 0.0)
			{
				double r12 = abs(dis1) / (abs(dis1) + abs(dis2));
				double r13 = abs(dis1) / (abs(dis1) + abs(dis3));
				osg::Vec3d v12 = getLerp(v1, v2, r12);
				osg::Vec3d v13 = getLerp(v1, v3, r13);
				addToVertexArray(v1, v12, v13, NEGATIVE_SIDE);
				addToVertexArray(v12, v2, v3, POSITIVE_SIDE);
				addToVertexArray(v3, v13, v12, POSITIVE_SIDE);
				if (nAry_)
				{
					osg::Vec3d n12 = getLerp(getNormal(nAry_, p1), getNormal(nAry_, p2), r12);
					osg::Vec3d n13 = getLerp(getNormal(nAry_, p1), getNormal(nAry_, p3), r13);
					addToNormalArray(getNormal(nAry_, p1), n12, n13, NEGATIVE_SIDE);
					addToNormalArray(n12, getNormal(nAry_, p2), getNormal(nAry_, p3), POSITIVE_SIDE);
					addToNormalArray(getNormal(nAry_, p3), n13, n12, POSITIVE_SIDE);
				}
				if (cAry_)
				{
					osg::Vec4 c12 = getLerp(getColor(cAry_, p1), getColor(cAry_, p2), r12);
					osg::Vec4 c13 = getLerp(getColor(cAry_, p1), getColor(cAry_, p3), r13);
					addToColorArray(getColor(cAry_, p1), c12, c13, NEGATIVE_SIDE);
					addToColorArray(c12, getColor(cAry_, p2), getColor(cAry_, p3), POSITIVE_SIDE);
					addToColorArray(getColor(cAry_, p3), c13, c12, POSITIVE_SIDE);
				}
				if (tAry_)
				{
					osg::Vec2 t12 = getLerp(tAry_->at(p1), tAry_->at(p2), r12);
					osg::Vec2 t13 = getLerp(tAry_->at(p1), tAry_->at(p3), r13);
					addToTextCoordArray(tAry_->at(p1), t12, t13, NEGATIVE_SIDE);
					addToTextCoordArray(t12, tAry_->at(p2), tAry_->at(p3), POSITIVE_SIDE);
					addToTextCoordArray(tAry_->at(p3), t13, t12, POSITIVE_SIDE);
				}
			}
			else if (dis2 < 0.0)
			{
				double r12 = abs(dis1) / (abs(dis1) + abs(dis2));
				double r23 = abs(dis2) / (abs(dis2) + abs(dis3));
				osg::Vec3d v12 = getLerp(v1, v2, r12);
				osg::Vec3d v23 = getLerp(v2, v3, r23);
				addToVertexArray(v12, v2, v23, NEGATIVE_SIDE);
				addToVertexArray(v1, v12, v23, POSITIVE_SIDE);
				addToVertexArray(v1, v23, v3, POSITIVE_SIDE);
				if (nAry_)
				{
					osg::Vec3d n12 = getLerp(getNormal(nAry_, p1), getNormal(nAry_, p2), r12);
					osg::Vec3d n23 = getLerp(getNormal(nAry_, p2), getNormal(nAry_, p3), r23);
					addToNormalArray(n12, getNormal(nAry_, p3), n23, NEGATIVE_SIDE);
					addToNormalArray(getNormal(nAry_, p1), n12, n23, POSITIVE_SIDE);
					addToNormalArray(getNormal(nAry_, p1), n23, getNormal(nAry_, p3), POSITIVE_SIDE);
				}
				if (cAry_)
				{
					osg::Vec4 c12 = getLerp(getColor(cAry_, p1), getColor(cAry_, p2), r12);
					osg::Vec4 c23 = getLerp(getColor(cAry_, p2), getColor(cAry_, p3), r23);
					addToColorArray(c12, getColor(cAry_, p2), c23, NEGATIVE_SIDE);
					addToColorArray(getColor(cAry_, p1), c12, c23, POSITIVE_SIDE);
					addToColorArray(getColor(cAry_, p1), c23, getColor(cAry_, p3), POSITIVE_SIDE);
				}
				if (tAry_)
				{
					osg::Vec2 t12 = getLerp(tAry_->at(p1), tAry_->at(p2), r12);
					osg::Vec2 t23 = getLerp(tAry_->at(p2), tAry_->at(p3), r23);
					addToTextCoordArray(t12, tAry_->at(p2), t23, NEGATIVE_SIDE);
					addToTextCoordArray(tAry_->at(p1), t12, t23, POSITIVE_SIDE);
					addToTextCoordArray(tAry_->at(p1), t23, tAry_->at(p3), POSITIVE_SIDE);
				}
			}
			else if (dis3 < 0.0)
			{
				double r13 = abs(dis1) / (abs(dis1) + abs(dis3));
				double r23 = abs(dis2) / (abs(dis2) + abs(dis3));
				osg::Vec3d v13 = getLerp(v1, v3, r13);
				osg::Vec3d v23 = getLerp(v2, v3, r23);
				addToVertexArray(v3, v13, v23, NEGATIVE_SIDE);
				addToVertexArray(v1, v2, v23, POSITIVE_SIDE);
				addToVertexArray(v1, v23, v13, POSITIVE_SIDE);
				if (nAry_)
				{
					osg::Vec3d n13 = getLerp(getNormal(nAry_, p1), getNormal(nAry_, p3), r13);
					osg::Vec3d n23 = getLerp(getNormal(nAry_, p2), getNormal(nAry_, p3), r23);
					addToNormalArray(getNormal(nAry_, p3), n13, n23, NEGATIVE_SIDE);
					addToNormalArray(getNormal(nAry_, p1), getNormal(nAry_, p2), n23, POSITIVE_SIDE);
					addToNormalArray(getNormal(nAry_, p1), n23, n13, POSITIVE_SIDE);
				}
				if (cAry_)
				{
					osg::Vec4 c13 = getLerp(getColor(cAry_, p1), getColor(cAry_, p3), r13);
					osg::Vec4 c23 = getLerp(getColor(cAry_, p2), getColor(cAry_, p3), r23);
					addToColorArray(getColor(cAry_, p3), c13, c23, NEGATIVE_SIDE);
					addToColorArray(getColor(cAry_, p1), getColor(cAry_, p2), c23, POSITIVE_SIDE);
					addToColorArray(getColor(cAry_, p1), c23, c13, POSITIVE_SIDE);
				}
				if (tAry_)
				{
					osg::Vec2 t13 = getLerp(tAry_->at(p1), tAry_->at(p3), r13);
					osg::Vec2 t23 = getLerp(tAry_->at(p2), tAry_->at(p3), r23);
					addToTextCoordArray(tAry_->at(p3), t13, t23, NEGATIVE_SIDE);
					addToTextCoordArray(tAry_->at(p1), tAry_->at(p2), t23, POSITIVE_SIDE);
					addToTextCoordArray(tAry_->at(p1), t23, t13, POSITIVE_SIDE);
				}
			}
		}
	}

private:
	void addToVertexArray(const osg::Vec3d& v1, const osg::Vec3d& v2, const osg::Vec3d& v3, Side side)
	{
		if (side == POSITIVE_SIDE)
		{
			lvAry_->push_back(v1);
			lvAry_->push_back(v2);
			lvAry_->push_back(v3);
		}
		else
		{
			rvAry_->push_back(v1);
			rvAry_->push_back(v2);
			rvAry_->push_back(v3);
		}
	}

	void addToNormalArray(const osg::Vec3d& v1, const osg::Vec3d& v2, const osg::Vec3d& v3, Side side)
	{
		if (side == POSITIVE_SIDE)
		{
			lnAry_->push_back(v1);
			lnAry_->push_back(v2);
			lnAry_->push_back(v3);
		}
		else
		{
			rnAry_->push_back(v1);
			rnAry_->push_back(v2);
			rnAry_->push_back(v3);
		}
	}

	osg::Vec4 getColor(const osg::Vec4Array* ca, unsigned int i)
	{
		if (geom_->getColorBinding() == osg::Geometry::BIND_OVERALL)
			return ca->at(0);
		else if (geom_->getColorBinding() == osg::Geometry::BIND_PER_VERTEX)
			return ca->at(i);
		return osg::Vec4(1, 1, 1, 1);
	}

	osg::Vec3 getNormal(const osg::Vec3Array* na, unsigned int i)
	{
		if (geom_->getNormalBinding() == osg::Geometry::BIND_OVERALL)
			return na->at(0);
		else if (geom_->getNormalBinding() == osg::Geometry::BIND_PER_VERTEX)
			return na->at(i);
		return osg::Vec3();
	}

	void addToColorArray(const osg::Vec4d& v1, const osg::Vec4d& v2, const osg::Vec4d& v3, Side side)
	{
		if (side == POSITIVE_SIDE)
		{
			lcAry_->push_back(v1);
			lcAry_->push_back(v2);
			lcAry_->push_back(v3);
		}
		else
		{
			rcAry_->push_back(v1);
			rcAry_->push_back(v2);
			rcAry_->push_back(v3);
		}
	}

	void addToTextCoordArray(const osg::Vec2d& v1, const osg::Vec2d& v2, const osg::Vec2d& v3, Side side)
	{
		if (side == POSITIVE_SIDE)
		{
			ltAry_->push_back(v1);
			ltAry_->push_back(v2);
			ltAry_->push_back(v3);
		}
		else
		{
			rtAry_->push_back(v1);
			rtAry_->push_back(v2);
			rtAry_->push_back(v3);
		}
	}

	osg::Vec3d getLerp(const osg::Vec3d& ps, const osg::Vec3d& pe, double rate)
	{
		osg::Vec3d dir = pe - ps;
		double len = dir.length();
		dir.normalize();
		return ps + dir*len*rate;
	}

	osg::Vec4 getLerp(const osg::Vec4& ps, const osg::Vec4& pe, double rate)
	{
		osg::Vec4 dir = pe - ps;
		double len = dir.length();
		dir.normalize();
		return ps + dir*len*rate;
	}

	osg::Vec2 getLerp(const osg::Vec2& ps, const osg::Vec2& pe, double rate)
	{
		osg::Vec2 dir = pe - ps;
		double len = dir.length();
		dir.normalize();
		return ps + dir*len*rate;
	}

private:
	osg::ref_ptr<const osg::Vec3Array> vAry_;
	osg::ref_ptr<const osg::Vec3Array> nAry_;
	osg::ref_ptr<const osg::Vec2Array> tAry_;
	osg::ref_ptr<const osg::Vec4Array> cAry_;
	osg::ref_ptr<osg::Geometry> geom_;

	osg::ref_ptr<osg::Vec4Array> rcAry_, lcAry_;
	osg::ref_ptr<osg::Vec3Array> rvAry_, lvAry_;
	osg::ref_ptr<osg::Vec3Array> rnAry_, lnAry_;
	osg::ref_ptr<osg::Vec2Array> rtAry_, ltAry_;
	osg::Plane plane_;
};

class GeometryColl :public osg::NodeVisitor
{
public:
	GeometryColl(const osg::Matrixd& mat)
		:osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN)
		, _mat(mat) {}
	virtual void apply(osg::Geode& geode)
	{
		for (unsigned int i = 0; i < geode.getNumDrawables(); ++i)
		{
			osg::Geometry* geom = dynamic_cast<osg::Geometry*>(geode.getDrawable(i));
			if (!geom) continue;
			osg::Vec3Array* va = dynamic_cast<osg::Vec3Array*>(geom->getVertexArray());
			if (!va) continue;
			for (osg::Vec3Array::iterator it = va->begin(); it != va->end(); ++it)
			{
				*it = *it*_mat;
			}
			geom->dirtyDisplayList();
			geom->dirtyBound();
			_geomVector.push_back(geom);
		}
	}
	typedef std::vector < osg::ref_ptr<osg::Geometry> >GeometryVector;
	const GeometryVector& getGeometrys() const { return _geomVector; }
private:
	GeometryVector _geomVector;
	osg::Matrixd _mat;
};


//支持凸多边形切割/普通切割;
class Cutter : public osg::NodeVisitor
{
public:
	Cutter(const osg::Polytope& polytope)
		: osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN)
		, polytope_(polytope)/*, result_(0L)*/
	{}

	void setPolytope(const osg::Polytope& polytope) { polytope_ = polytope; }

	void apply(osg::Geode& geode)
	{
		osg::ref_ptr<osg::Geode> newgeode = new osg::Geode;

		osg::MatrixList ml = geode.getWorldMatrices();
		osg::Matrixd matrix = getMatrix(ml);
		osg::Matrixd inverseMatrix = osg::Matrixd::inverse(matrix);

		const std::vector<osg::Plane>& planes = polytope_.getPlaneList();

		for (unsigned int i = 0; i < geode.getNumDrawables(); ++i)
		{
			osg::Geometry* geom = geode.getDrawable(i)->asGeometry();
			if (!geom) continue;

			osg::Polytope pt;
			for (std::vector<osg::Plane>::const_iterator it = planes.begin(); it != planes.end(); ++it)
			{
				osg::Plane plane = osg::Plane(*it);
				plane.transform(inverseMatrix);
				pt.add(plane);
			}

			osg::ref_ptr<osg::Geometry> newGeom = new osg::Geometry(*geom, osg::CopyOp::DEEP_COPY_ALL);
			std::vector<osg::Plane> planelist = pt.getPlaneList();
			for (std::vector<osg::Plane>::const_iterator it = planelist.begin(); it != planelist.end(); ++it)
			{
				osg::Plane plane = *it;
				osg::TriangleIndexFunctor<TriangulateOperator> MyTriangleIndexFunctor;
				MyTriangleIndexFunctor.init();
				MyTriangleIndexFunctor.setCutPlane(plane);
				MyTriangleIndexFunctor.getGeomProperties(*newGeom);
				newGeom->accept(MyTriangleIndexFunctor);

				newGeom = MyTriangleIndexFunctor.getGeometry(TriangulateOperator::POSITIVE_SIDE);
				if (!newGeom || newGeom->getNumPrimitiveSets() < 1) break;
			}

			if (newGeom&&newGeom->getNumPrimitiveSets() > 0)
			{
				geode.replaceDrawable(geom, newGeom);
			}
			else
			{
				geode.removeDrawable(geom);
			}
		}
	}

private:
	osg::Matrixd getMatrix(const osg::MatrixList& ml)
	{
		if (!ml.empty())
		{
			return ml[0];
		}
		return osg::Matrixd::identity();
	}

private:
	osg::Polytope polytope_;
	//osg::ref_ptr<osg::Group> result_;
};

#endif // Cutter_h__