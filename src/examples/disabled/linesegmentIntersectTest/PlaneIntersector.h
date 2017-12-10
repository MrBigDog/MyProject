/***************************************************************************/
//  [9/7/2017 BigDog] 功能与osgUtil::PlaneIntersector相同，修复了其中的一些BUG.
/***************************************************************************/
#ifndef BIGDOG_PLANEINTERSECTOR_H
#define BIGDOG_PLANEINTERSECTOR_H 1

#include <osgUtil/IntersectionVisitor>
#include <osg/CoordinateSystemNode>

class PlaneIntersector : public osgUtil::Intersector
{
public:
	PlaneIntersector(const osg::Plane& plane, const osg::Polytope& boundingPolytope = osg::Polytope());
	PlaneIntersector(CoordinateFrame cf, const osg::Plane& plane, const osg::Polytope& boundingPolytope = osg::Polytope());

	struct Intersection
	{
		Intersection() {}
		bool operator < (const Intersection& rhs) const
		{
			if (polyline < rhs.polyline) return true;
			if (rhs.polyline < polyline) return false;

			if (nodePath < rhs.nodePath) return true;
			if (rhs.nodePath < nodePath) return false;

			return (drawable < rhs.drawable);
		}

		typedef std::vector<osg::Vec3d> Polyline;
		typedef std::vector<double> Attributes;

		osg::NodePath                   nodePath;
		osg::ref_ptr<osg::RefMatrix>    matrix;
		osg::ref_ptr<osg::Drawable>     drawable;
		Polyline                        polyline;
		Attributes                      attributes;
	};

	typedef std::vector<Intersection> Intersections;

	inline void insertIntersection(const Intersection& intersection) { getIntersections().push_back(intersection); }

	inline Intersections& getIntersections() { return _parent ? _parent->_intersections : _intersections; }

	void setRecordHeightsAsAttributes(bool flag) { _recordHeightsAsAttributes = flag; }

	bool getRecordHeightsAsAttributes() const { return _recordHeightsAsAttributes; }

	void setEllipsoidModel(osg::EllipsoidModel* em) { _em = em; }

	const osg::EllipsoidModel* getEllipsoidModel() const { return _em.get(); }

public:
	virtual Intersector* clone(osgUtil::IntersectionVisitor& iv);

	virtual bool enter(const osg::Node& node);

	virtual void leave();

	virtual void intersect(osgUtil::IntersectionVisitor& iv, osg::Drawable* drawable);

	virtual void reset();

	virtual bool containsIntersections() { return !getIntersections().empty(); }

protected:
	PlaneIntersector*                   _parent;

	bool                                _recordHeightsAsAttributes;
	osg::ref_ptr<osg::EllipsoidModel>   _em;

	osg::Plane                          _plane;
	osg::Polytope                       _polytope;

	Intersections                       _intersections;
};

#endif // BIGDOG_PLANEINTERSECTOR_H



