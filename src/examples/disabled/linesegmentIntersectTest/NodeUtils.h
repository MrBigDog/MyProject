#ifndef NodeUtils_h__
#define NodeUtils_h__

#include "ThreadingUtils.h"

#include <osg/View>
#include <osg/PagedLOD>
#include <osgGA/GUIActionAdapter>
#include <osgUtil/LineSegmentIntersector>
#include <set>
#include <vector>

/**
 * General purpose operation for doing something to a node.
 */
struct NodeOperation : public osg::Referenced
{
	virtual void operator()(osg::Node* node) = 0;
};
typedef std::vector< osg::ref_ptr<NodeOperation> > NodeOperationVector;

struct RefNodeOperationVector : public osg::Referenced, public NodeOperationVector
{
public:
	ReadWriteMutex& mutex() const { return _mutex; }
private:
	mutable ReadWriteMutex _mutex;
};

/**
 * A PagedLOD that will fire node operation callbacks when it merges
 * new nodes into the graph.
 */
class PagedLODWithNodeOperations : public osg::PagedLOD
{
public:
	PagedLODWithNodeOperations(RefNodeOperationVector* postMergeOps);

public: // osg::Group
	virtual bool addChild(osg::Node* child);
	virtual bool insertChild(unsigned index, Node* child);
	virtual bool replaceChild(Node* origChild, Node* newChild);

protected:
	osg::observer_ptr<RefNodeOperationVector> _postMergeOps;
	void runPostMerge(osg::Node* node);
};

/**
 * Visitor that looks for empty group nodes and removes them from the
 * scene graph. (Note, if the entry node is an empty group, it will
 * not be affected.)
 */
class RemoveEmptyGroupsVisitor : public osg::NodeVisitor
{
public:
	static void run(osg::Node* entry)
	{
		if (entry)
		{
			RemoveEmptyGroupsVisitor v;
			entry->accept(v);
		}
	}

public:
	RemoveEmptyGroupsVisitor();
	void apply(osg::Group& group); //override
};

/**
 * Visitor that counts the number of point, line, and polygon primitive sets
 * in a scene graph.
 */
class PrimitiveSetTypeCounter : public osg::NodeVisitor
{
public:
	PrimitiveSetTypeCounter();
	virtual ~PrimitiveSetTypeCounter() { }

	void apply(class osg::Geode&);

	unsigned _point;
	unsigned _line;
	unsigned _polygon;
};

/**
 * Visitor that finds all the parental Camera Views, and calls an operator
 * on each one.
 */
template<typename T>
class ViewVisitor : public osg::NodeVisitor, public T
{
public:
	ViewVisitor() : osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_PARENTS) { }
	virtual ~ViewVisitor() { }
	void apply(osg::Camera& cam)
	{
		osg::View* view = cam.getView();
		if (view) this->operator()(view);
		traverse(cam);
	}
};

/**
 * Functor (for use with ViewVisitor) that notifies a view that it needs to
 * redraw the scene because something has changed
 * Usage: ViewVisitor<RequestRedraw> vis; node->accept(vis);
 */
struct RequestRedraw
{
	void operator()(osg::View* view)
	{
		osgGA::GUIActionAdapter* aa = dynamic_cast<osgGA::GUIActionAdapter*>(view);
		if (aa) aa->requestRedraw();
	}
};

/**
 * Visitor that locates a node by its type
 */
template<typename T>
class FindTopMostNodeOfTypeVisitor : public osg::NodeVisitor
{
public:
	FindTopMostNodeOfTypeVisitor()
		: osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN)
		, _foundNode(0)
	{}

	void apply(osg::Node& node)
	{
		T* result = dynamic_cast<T*>(&node);
		if (result)_foundNode = result;
		else traverse(node);
	}

	T* _foundNode;
};

/**
 * Collects all the nodes of type "T"
 */
template<typename T>
struct FindNodesVisitor : public osg::NodeVisitor
{
	FindNodesVisitor() : osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN) { }

	void apply(osg::Node& node)
	{
		T* result = dynamic_cast<T*>(&node);
		if (result)
			_results.push_back(result);
		traverse(node);
	}

	std::vector<T*> _results;
};

/**
 * Searchs the scene graph downward starting at [node] and returns the first node found
 * that matches the template parameter type.
 */
template<typename T>
T* findTopMostNodeOfType(osg::Node* node, unsigned traversalMask = ~0)
{
	if (!node) return 0;

	FindTopMostNodeOfTypeVisitor<T> fnotv;
	fnotv.setTraversalMode(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN);
	fnotv.setTraversalMask(traversalMask);
	node->accept(fnotv);

	return fnotv._foundNode;
}

/**
 * Searchs the scene graph upward starting at [node] and returns the first node found
 * that matches the template parameter type.
 */
template<typename T>
T* findFirstParentOfType(osg::Node* node, unsigned traversalMask = ~0)
{
	if (!node) return 0;

	FindTopMostNodeOfTypeVisitor<T> fnotv;
	fnotv.setTraversalMode(osg::NodeVisitor::TRAVERSE_PARENTS);
	fnotv.setTraversalMask(traversalMask);
	node->accept(fnotv);

	return fnotv._foundNode;
}

/**
 * Searchs the scene graph starting at [node] and returns the first node found
 * that matches the template parameter type. First searched upward, then downward.
 */
template<typename T>
T* findRelativeNodeOfType(osg::Node* node, unsigned traversalMask = ~0)
{
	if (!node) return 0;
	T* result = findFirstParentOfType<T>(node, traversalMask);
	if (!result)
		result = findTopMostNodeOfType<T>(node, traversalMask);
	return result;
}

/** Find the top of the scene graph through parent 0 */
inline osg::Node* findTopOfGraph(osg::Node* node)
{
	return node && node->getNumParents() > 0 ? findTopOfGraph(node->getParent(0)) : node;
}

/**
 * Replace one group with another
 */
inline void replaceGroup(osg::Group* oldGroup, osg::Group* newGroup)
{
	if (oldGroup && newGroup && oldGroup->getNumParents() > 0)
	{
		for (unsigned i = 0; i < oldGroup->getNumChildren(); ++i)
		{
			newGroup->addChild(oldGroup->getChild(i));
		}

		osg::Node::ParentList parents = oldGroup->getParents();
		for (osg::Node::ParentList::iterator i = parents.begin(); i != parents.end(); ++i)
		{
			(*i)->replaceChild(oldGroup, newGroup);
		}
	}
}

/** Insert a group between a parent and its children. */
inline void insertGroup(osg::Group* newGroup, osg::Group* parent)
{
	if (parent && newGroup)
	{
		for (unsigned i = 0; i < parent->getNumChildren(); ++i)
		{
			newGroup->addChild(parent->getChild(i));
		}
		parent->removeChildren(0, parent->getNumChildren());
		parent->addChild(newGroup);
	}
}

/** Insert a group above a node. */
inline void insertParent(osg::Group* newParent, osg::Node* existingChild)
{
	if (newParent && existingChild)
	{
		newParent->addChild(existingChild);

		for (unsigned i = 0; i < existingChild->getNumParents(); ++i)
		{
			osg::Group* parent = existingChild->getParent(i);
			if (parent != newParent)
			{
				parent->removeChild(existingChild);
				parent->addChild(newParent);
			}
		}
	}
}

inline void clearChildren(osg::Group* group)
{
	if (group) group->removeChildren(0, group->getNumChildren());
}

/**
 * OSG Group that keeps its children as observer_ptrs instead of ref_ptrs, and
 * removes them when they deref.
 */
class ObserverGroup : public osg::Group
{
public:
	ObserverGroup();
	virtual void traverse(osg::NodeVisitor& nv);
	std::set<osg::Node*> _orphans;
};


/**
 * Group that acts like a normal group but also notifies another
 * object when a change occurs.
 */
template<typename T>
class NotifierGroup : public osg::Group
{
public:
	NotifierGroup(T* listener) : _listener(listener) { }

	virtual bool addChild(osg::Node* child) {
		bool ok = osg::Group::addChild(child);
		if (ok && _listener.valid()) _listener->onGroupChanged(this);
		return ok;
	}
	virtual bool insertChild(unsigned index, osg::Node* child) {
		bool ok = osg::Group::insertChild(index, child);
		if (ok && _listener.valid()) _listener->onGroupChanged(this);
		return ok;
	}
	virtual bool removeChild(osg::Node* child) {
		bool ok = osg::Group::removeChild(child);
		if (ok && _listener.valid()) _listener->onGroupChanged(this);
		return ok;
	}
	virtual bool replaceChild(osg::Node* origChild, osg::Node* newChild) {
		bool ok = osg::Group::replaceChild(origChild, newChild);
		if (ok && _listener.valid()) _listener->onGroupChanged(this);
		return ok;
	}

protected:
	virtual ~NotifierGroup() { }
	osg::observer_ptr<T> _listener;
};


/**
 * Adjusts a node's update traversal count by a delta.
 * Only safe to call from the UPDATE thread
 */
#define ADJUST_UPDATE_TRAV_COUNT( NODE, DELTA ) \
    { \
        int oldCount = NODE ->getNumChildrenRequiringUpdateTraversal(); \
        if ( oldCount + DELTA >= 0 ) \
            NODE ->setNumChildrenRequiringUpdateTraversal( (unsigned int)(oldCount + DELTA ) ); \
    }

 /**
  * Adjusts a node's event traversal count by a delta.
  * Only safe to call from the main/event/update threads
  */
#define ADJUST_EVENT_TRAV_COUNT( NODE, DELTA ) \
    { \
        int oldCount = NODE ->getNumChildrenRequiringEventTraversal(); \
        if ( oldCount + DELTA >= 0 ) \
            NODE ->setNumChildrenRequiringEventTraversal( (unsigned int)(oldCount + DELTA ) ); \
    }


#endif // NodeUtils_h__