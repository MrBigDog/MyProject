#ifndef LineSegmentIntersector_h__
#define LineSegmentIntersector_h__

#include <osgUtil/IntersectionVisitor>

class LineSegmentIntersector : public osgUtil::Intersector
{
public:
	/** Construct a LineSegmentIntersector that runs between the specified start and end points in MODEL coordinates. */
	LineSegmentIntersector(const osg::Vec3d& start, const osg::Vec3d& end);

	/** Construct a LineSegmentIntersector that runs between the specified start and end points in the specified coordinate frame. */
	LineSegmentIntersector(CoordinateFrame cf, const osg::Vec3d& start, const osg::Vec3d& end, LineSegmentIntersector* parent = NULL,
		osgUtil::Intersector::IntersectionLimit intersectionLimit = osgUtil::Intersector::NO_LIMIT);

	/** Convenience constructor for supporting picking in WINDOW, or PROJECTION coordinates
	  * In WINDOW coordinates creates a start value of (x,y,0) and end value of (x,y,1).
	  * In PROJECTION coordinates (clip space cube) creates a start value of (x,y,-1) and end value of (x,y,1).
	  * In VIEW and MODEL coordinates creates a start value of (x,y,0) and end value of (x,y,1).*/
	LineSegmentIntersector(CoordinateFrame cf, double x, double y);

	struct Intersection
	{
		Intersection() : ratio(-1.0), primitiveIndex(0) {}

		bool operator < (const Intersection& rhs) const { return ratio < rhs.ratio; }
		typedef std::vector<unsigned int>   IndexList;
		typedef std::vector<double>         RatioList;

		double                          ratio;
		osg::NodePath                   nodePath;
		osg::ref_ptr<osg::Drawable>     drawable;
		osg::ref_ptr<osg::RefMatrix>    matrix;
		osg::Vec3d                      localIntersectionPoint;
		osg::Vec3                       localIntersectionNormal;
		IndexList                       indexList;
		RatioList                       ratioList;
		unsigned int                    primitiveIndex;

		const osg::Vec3d& getLocalIntersectPoint() const { return localIntersectionPoint; }
		osg::Vec3d getWorldIntersectPoint() const { return matrix.valid() ? localIntersectionPoint * (*matrix) : localIntersectionPoint; }

		const osg::Vec3& getLocalIntersectNormal() const { return localIntersectionNormal; }
		osg::Vec3 getWorldIntersectNormal() const { return matrix.valid() ? osg::Matrix::transform3x3(osg::Matrix::inverse(*matrix), localIntersectionNormal) : localIntersectionNormal; }

		/** Convenience function for mapping the intersection point to any textures assigned to the objects intersected.
		 *  Returns the Texture pointer and texture coords of object hit when a texture is available on the object, returns NULL otherwise.*/
		osg::Texture* getTextureLookUp(osg::Vec3& tc) const;
	};

	typedef std::multiset<Intersection> Intersections;

	inline void insertIntersection(const Intersection& intersection) { getIntersections().insert(intersection); }

	inline Intersections& getIntersections() { return _parent ? _parent->_intersections : _intersections; }

	inline Intersection getFirstIntersection() { Intersections& intersections = getIntersections(); return intersections.empty() ? Intersection() : *(intersections.begin()); }

	inline void setStart(const osg::Vec3d& start) { _start = start; }
	inline const osg::Vec3d& getStart() const { return _start; }

	inline void setEnd(const osg::Vec3d& end) { _end = end; }
	inline const osg::Vec3d& getEnd() const { return _end; }

public:
	virtual Intersector* clone(osgUtil::IntersectionVisitor& iv);

	virtual bool enter(const osg::Node& node);

	virtual void leave();

	virtual void intersect(osgUtil::IntersectionVisitor& iv, osg::Drawable* drawable);

	virtual void intersect(osgUtil::IntersectionVisitor& iv, osg::Drawable* drawable,
		const osg::Vec3d& s, const osg::Vec3d& e);

	virtual void reset();

	virtual bool containsIntersections() { return !getIntersections().empty(); }

	/** Compute the matrix that transforms the local coordinate system of parent Intersector (usually
		the current intersector) into the child coordinate system of the child Intersector.
		cf parameter indicates the coordinate frame of parent Intersector. */
	static osg::Matrix getTransformation(osgUtil::IntersectionVisitor& iv, CoordinateFrame cf);

protected:
	bool intersects(const osg::BoundingSphere& bs);
	bool intersectAndClip(osg::Vec3d& s, osg::Vec3d& e, const osg::BoundingBox& bb);

	LineSegmentIntersector* _parent;

	osg::Vec3d  _start;
	osg::Vec3d  _end;

	Intersections _intersections;
};

#endif // LineSegmentIntersector_h__


