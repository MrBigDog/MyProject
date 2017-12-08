#ifndef GWEARTH_ROOT_H
#define GWEARTH_ROOT_H 1

#include <compiler.h>
#include <gwEarth/Export.h>

#include <osg/Group>
#include <osgDB/Registry>
#include <osgDB/SharedStateManager>

namespace gwEarth
{
	class GWEARTH_EXPORT Root :public osg::Referenced
	{
	public:
		Root()
		{
			_root = new osg::Group;
			_root->setName("ROOT");
			osgDB::SharedStateManager* ssm = osgDB::Registry::instance()->getSharedStateManager();
			if (ssm)
			{
				ssm->share(_root.get());
			}
			//_root->setDataVariance(osg::Object::DYNAMIC);
			osgDB::Registry::instance()->setBuildKdTreesHint(osgDB::Options::BUILD_KDTREES);
			//{
			//	void setMaxTexturePoolSize(unsigned int size);
			//	void setMaxBufferObjectPoolSize(unsigned int size);
			//}
		}

		virtual osg::Group* get() const
		{
			return _root;
		}

		virtual bool addChild(osg::Node* node)
		{
			if (!_root) return false;
			return _root->addChild(node);
		}

		virtual bool insertChild(unsigned int index, osg::Node* node)
		{
			if (!_root) return false;
			return _root->insertChild(index, node);
		}

		virtual bool removeChild(osg::Node* node)
		{
			if (!_root) return false;
			return _root->removeChild(node);
		}

		virtual bool replaceChild(osg::Node* oldChild, osg::Node* newChlild)
		{
			if (!_root) return false;
			return _root->replaceChild(oldChild, newChlild);
		}

		virtual bool containsNode(const osg::Node* node)
		{
			if (!_root) return false;
			return _root->containsNode(node);
		}

	private:
		osg::ref_ptr<osg::Group> _root;
	};
}




#endif // GWBASE_ROOT_H
