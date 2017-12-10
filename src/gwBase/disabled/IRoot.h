#ifndef GWBASE_IROOT_H
#define GWBASE_IROOT_H 1

#include <osg/ref_ptr>
#include <osg/Referenced>

namespace osg
{
	class Group;
	class Node;
}

namespace gwBase
{
	class IRoot :public osg::Referenced
	{
	public:
		virtual osg::Group* get() const = 0;
		virtual bool addChild(osg::Node* node) = 0;
		virtual bool insertChild(unsigned int index, osg::Node* node) = 0;
		virtual bool removeChild(osg::Node* node) = 0;
		virtual bool replaceChild(osg::Node* oldChild, osg::Node* newChlild) = 0;
		virtual bool containsNode(const osg::Node* node) = 0;
	};
	typedef osg::ref_ptr<IRoot> IRootPtr;
}

#endif // GWBASE_IROOT_H
