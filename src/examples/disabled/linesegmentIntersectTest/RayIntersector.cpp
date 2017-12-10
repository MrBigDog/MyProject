#include "RayIntersector.h"
#include "LineSegmentIntersector.h"
#include <osg/KdTree>
#include <osg/Notify>
#include <osg/TexMat>
#include <limits>
#include <cmath>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  RayIntersector
//
RayIntersector::RayIntersector(CoordinateFrame cf, RayIntersector* parent,
	Intersector::IntersectionLimit intersectionLimit) :
	Intersector(cf, intersectionLimit),
	_parent(parent)
{
	if (parent) setPrecisionHint(parent->getPrecisionHint());
}

RayIntersector::RayIntersector(const osg::Vec3d& start, const osg::Vec3d& direction) :
	Intersector(),
	_parent(0),
	_start(start),
	_direction(direction)
{
}

RayIntersector::RayIntersector(CoordinateFrame cf, const osg::Vec3d& start, const osg::Vec3d& direction,
	RayIntersector* parent, Intersector::IntersectionLimit intersectionLimit) :
	Intersector(cf, intersectionLimit),
	_parent(parent),
	_start(start),
	_direction(direction)
{
	if (parent) setPrecisionHint(parent->getPrecisionHint());
}

RayIntersector::RayIntersector(CoordinateFrame cf, double x, double y) :
	Intersector(cf),
	_parent(0)
{
	switch (cf)
	{
	case WINDOW:     setStart(osg::Vec3d(x, y, 0.));  setDirection(osg::Vec3d(0., 0., 1.)); break;
	case PROJECTION: setStart(osg::Vec3d(x, y, -1.)); setDirection(osg::Vec3d(0., 0., 1.)); break;
	case VIEW:       setStart(osg::Vec3d(x, y, 0.));  setDirection(osg::Vec3d(0., 0., 1.)); break;
	case MODEL:      setStart(osg::Vec3d(x, y, 0.));  setDirection(osg::Vec3d(0., 0., 1.)); break;
	}
}

osgUtil::Intersector* RayIntersector::clone(osgUtil::IntersectionVisitor& iv)
{
	if (_coordinateFrame == MODEL && iv.getModelMatrix() == 0)
	{
		return new RayIntersector(MODEL, _start, _direction, this, _intersectionLimit);
	}

	osg::Matrix matrix(LineSegmentIntersector::getTransformation(iv, _coordinateFrame));

	osg::Vec3d newStart = _start * matrix;
	osg::Vec4d tmp = osg::Vec4d(_start + _direction, 1.) * matrix;
	osg::Vec3d newEnd = osg::Vec3d(tmp.x(), tmp.y(), tmp.z()) - (newStart * tmp.w());
	return new RayIntersector(MODEL, newStart, newEnd, this, _intersectionLimit);
}

bool RayIntersector::enter(const osg::Node& node)
{
	if (reachedLimit()) return false;
	return !node.isCullingActive() || intersects(node.getBound());
}

void RayIntersector::leave()
{
}

void RayIntersector::reset()
{
	Intersector::reset();
	_intersections.clear();
}

void RayIntersector::intersect(osgUtil::IntersectionVisitor& iv, osg::Drawable* drawable)
{
	// did we reached what we wanted as specified by setIntersectionLimit()?
	if (reachedLimit()) return;

	// clip ray to finite line segment
	osg::Vec3d s(_start), e;
	if (!intersectAndClip(s, _direction, e, drawable->getBoundingBox())) return;

	// dummy traversal
	if (iv.getDoDummyTraversal()) return;

	// get intersections using LineSegmentIntersector
	LineSegmentIntersector lsi(MODEL, s, e, NULL, _intersectionLimit);
	lsi.setPrecisionHint(getPrecisionHint());
	lsi.intersect(iv, drawable, s, e);

	// copy intersections from LineSegmentIntersector
	LineSegmentIntersector::Intersections intersections = lsi.getIntersections();
	if (!intersections.empty())
	{
		double preLength = (s - _start).length();
		double esLength = (e - s).length();

		for (LineSegmentIntersector::Intersections::iterator it = intersections.begin();
			it != intersections.end(); it++)
		{
			Intersection hit;
			hit.distance = preLength + it->ratio * esLength;
			hit.matrix = it->matrix;
			hit.nodePath = it->nodePath;
			hit.drawable = it->drawable;
			hit.primitiveIndex = it->primitiveIndex;

			hit.localIntersectionPoint = it->localIntersectionPoint;
			hit.localIntersectionNormal = it->localIntersectionNormal;

			hit.indexList = it->indexList;
			hit.ratioList = it->ratioList;

			insertIntersection(hit);
		}
	}
}

bool RayIntersector::intersects(const osg::BoundingSphere& bs)
{
	// if bs not valid then return true based on the assumption that an invalid sphere is yet to be defined.
	if (!bs.valid()) return true;

	// test for _start inside the bounding sphere
	osg::Vec3d sm = _start - bs._center;
	double c = sm.length2() - bs._radius * bs._radius;
	if (c < 0.0) return true;

	// solve quadratic equation
	double a = _direction.length2();
	double b = (sm * _direction) * 2.0;
	double d = b * b - 4.0 * a * c;

	// no intersections if d<0
	if (d < 0.0) return false;

	// compute two solutions of quadratic equation
	d = sqrt(d);
	double div = 1.0 / (2.0*a);
	double r1 = (-b - d)*div;
	double r2 = (-b + d)*div;

	// return false if both intersections are before the ray start
	if (r1 <= 0.0 && r2 <= 0.0) return false;

	// if LIMIT_NEAREST and closest point of bounding sphere is further than already found intersection, return false
	if (_intersectionLimit == LIMIT_NEAREST && !getIntersections().empty())
	{
		double minDistance = sm.length() - bs._radius;
		if (minDistance >= getIntersections().begin()->distance) return false;
	}

	// passed all the rejection tests so line must intersect bounding sphere, return true.
	return true;
}

bool RayIntersector::intersectAndClip(osg::Vec3d& s, const osg::Vec3d& d, osg::Vec3d& e, const osg::BoundingBox& bbInput)
{
	// bounding box min and max
	osg::Vec3d bb_min(bbInput._min);
	osg::Vec3d bb_max(bbInput._max);

	// Expand the extents of the bounding box by the epsilon to prevent numerical errors resulting in misses.
	const double epsilon = 1e-6;

	// clip s against all three components of the Min to Max range of bb
	for (int i = 0; i < 3; i++)
	{
		// test direction
		if (d[i] >= 0.)
		{
			// trivial reject of segment wholly outside
			if (s[i] > bb_max[i]) return false;

			if (s[i] < bb_min[i])
			{
				// clip s to xMin
				double t = (bb_min[i] - s[i]) / d[i] - epsilon;
				if (t > 0.0) s = s + d*t;
			}
		}
		else
		{
			// trivial reject of segment wholly outside
			if (s[i] < bb_min[i]) return false;

			if (s[i] > bb_max[i])
			{
				// clip s to xMax
				double t = (bb_max[i] - s[i]) / d[i] - epsilon;
				if (t > 0.0) s = s + d*t;
			}
		}
	}

	// t for ending point of clipped ray
	double end_t = std::numeric_limits<double>::infinity();

	// get end point by clipping the ray by bb
	// note: this can not be done in previous loop as start point s is moving
	for (int i = 0; i < 3; i++)
	{
		// test direction
		if (d[i] >= 0.)
		{
			// compute end_t based on xMax
			double t = (bb_max[i] - s[i]) / d[i] + epsilon;
			if (t < end_t)
				end_t = t;
		}
		else
		{
			// compute end_t based on xMin
			double t = (bb_min[i] - s[i]) / d[i] + epsilon;
			if (t < end_t)
				end_t = t;
		}
	}

	// compute e
	e = s + d*end_t;

	return true;
}

osg::Texture* RayIntersector::Intersection::getTextureLookUp(osg::Vec3& tc) const
{
	osg::Geometry* geometry = drawable.valid() ? drawable->asGeometry() : 0;
	osg::Vec3Array* vertices = geometry ? dynamic_cast<osg::Vec3Array*>(geometry->getVertexArray()) : 0;

	if (vertices)
	{
		if (indexList.size() == 3 && ratioList.size() == 3)
		{
			unsigned int i1 = indexList[0];
			unsigned int i2 = indexList[1];
			unsigned int i3 = indexList[2];

			float r1 = ratioList[0];
			float r2 = ratioList[1];
			float r3 = ratioList[2];

			osg::Array* texcoords = (geometry->getNumTexCoordArrays() > 0) ? geometry->getTexCoordArray(0) : 0;
			osg::FloatArray* texcoords_FloatArray = dynamic_cast<osg::FloatArray*>(texcoords);
			osg::Vec2Array* texcoords_Vec2Array = dynamic_cast<osg::Vec2Array*>(texcoords);
			osg::Vec3Array* texcoords_Vec3Array = dynamic_cast<osg::Vec3Array*>(texcoords);
			if (texcoords_FloatArray)
			{
				// we have tex coord array so now we can compute the final tex coord at the point of intersection.
				float tc1 = (*texcoords_FloatArray)[i1];
				float tc2 = (*texcoords_FloatArray)[i2];
				float tc3 = (*texcoords_FloatArray)[i3];
				tc.x() = tc1*r1 + tc2*r2 + tc3*r3;
			}
			else if (texcoords_Vec2Array)
			{
				// we have tex coord array so now we can compute the final tex coord at the point of intersection.
				const osg::Vec2& tc1 = (*texcoords_Vec2Array)[i1];
				const osg::Vec2& tc2 = (*texcoords_Vec2Array)[i2];
				const osg::Vec2& tc3 = (*texcoords_Vec2Array)[i3];
				tc.x() = tc1.x()*r1 + tc2.x()*r2 + tc3.x()*r3;
				tc.y() = tc1.y()*r1 + tc2.y()*r2 + tc3.y()*r3;
			}
			else if (texcoords_Vec3Array)
			{
				// we have tex coord array so now we can compute the final tex coord at the point of intersection.
				const osg::Vec3& tc1 = (*texcoords_Vec3Array)[i1];
				const osg::Vec3& tc2 = (*texcoords_Vec3Array)[i2];
				const osg::Vec3& tc3 = (*texcoords_Vec3Array)[i3];
				tc.x() = tc1.x()*r1 + tc2.x()*r2 + tc3.x()*r3;
				tc.y() = tc1.y()*r1 + tc2.y()*r2 + tc3.y()*r3;
				tc.z() = tc1.z()*r1 + tc2.z()*r2 + tc3.z()*r3;
			}
			else
			{
				return 0;
			}
		}

		const osg::TexMat* activeTexMat = 0;
		const osg::Texture* activeTexture = 0;

		if (drawable->getStateSet())
		{
			const osg::TexMat* texMat = dynamic_cast<osg::TexMat*>(drawable->getStateSet()->getTextureAttribute(0, osg::StateAttribute::TEXMAT));
			if (texMat) activeTexMat = texMat;

			const osg::Texture* texture = dynamic_cast<osg::Texture*>(drawable->getStateSet()->getTextureAttribute(0, osg::StateAttribute::TEXTURE));
			if (texture) activeTexture = texture;
		}

		for (osg::NodePath::const_reverse_iterator itr = nodePath.rbegin();
			itr != nodePath.rend() && (!activeTexMat || !activeTexture);
			++itr)
		{
			const osg::Node* node = *itr;
			if (node->getStateSet())
			{
				if (!activeTexMat)
				{
					const osg::TexMat* texMat = dynamic_cast<const osg::TexMat*>(node->getStateSet()->getTextureAttribute(0, osg::StateAttribute::TEXMAT));
					if (texMat) activeTexMat = texMat;
				}

				if (!activeTexture)
				{
					const osg::Texture* texture = dynamic_cast<const osg::Texture*>(node->getStateSet()->getTextureAttribute(0, osg::StateAttribute::TEXTURE));
					if (texture) activeTexture = texture;
				}
			}
		}

		if (activeTexMat)
		{
			osg::Vec4 tc_transformed = osg::Vec4(tc.x(), tc.y(), tc.z(), 0.0f) * activeTexMat->getMatrix();
			tc.x() = tc_transformed.x();
			tc.y() = tc_transformed.y();
			tc.z() = tc_transformed.z();

			if (activeTexture && activeTexMat->getScaleByTextureRectangleSize())
			{
				tc.x() *= static_cast<float>(activeTexture->getTextureWidth());
				tc.y() *= static_cast<float>(activeTexture->getTextureHeight());
				tc.z() *= static_cast<float>(activeTexture->getTextureDepth());
			}
		}

		return const_cast<osg::Texture*>(activeTexture);

	}
	return 0;
}
