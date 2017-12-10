#ifndef TYPE_DEFINE_H
#define TYPE_DEFINE_H 1

#include <osg/Array>
#include <vector>

typedef std::vector<osg::ref_ptr<osg::Vec3Array> >Vec3ArrayVector;
typedef std::vector<osg::ref_ptr<osg::Vec3dArray> >Vec3dArrayVector;
typedef std::vector<osg::ref_ptr<osg::Node> > NodeVector;

#endif // TypeDefine_h__
