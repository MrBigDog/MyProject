#include "ProfileRegister.h"

void ProfileStruct::addProfile(Profile * profile)
{
	//@todo加锁;
	if (!profile) return;
	_profileVec.push_back(profile);
	for (CallbackVector::const_iterator it = _callbacks.begin(); it != _callbacks.end(); ++it)
	{
		(*it)->onProfileAdded(profile);
	}
}

void ProfileStruct::removeProfile(Profile * profile)
{
	//@todo加锁;
	ProfileVector::iterator it = std::find(_profileVec.begin(), _profileVec.end(), profile);
	if (it != _profileVec.end())
	{
		_profileVec.erase(it);
		for (CallbackVector::const_iterator it = _callbacks.begin(); it != _callbacks.end(); ++it)
		{
			(*it)->onProfileRemoved(profile);
		}
	}
}

void ProfileStruct::setVisible(bool isVisible)
{
	//@todo加锁;
	if (_isVisible != isVisible)
	{
		_isVisible = isVisible;
		for (CallbackVector::const_iterator it = _callbacks.begin(); it != _callbacks.end(); ++it)
		{
			(*it)->onVisibleChanged(isVisible);
		}
	}
}

void ProfileStruct::addCallback(Callback * cb)
{
	//@todo加锁;
	if (!cb) return;
	_callbacks.push_back(cb);
}

void ProfileStruct::removeCallback(Callback * cb)
{
	//@todo加锁;
	CallbackVector::iterator it = std::find(_callbacks.begin(), _callbacks.end(), cb);
	if (it != _callbacks.end())
	{
		_callbacks.erase(it);
	}
}

//--------------------------------------------------------------------------------------------
void ProfileStructMgr::addProfileStruct(ProfileStruct * profileStruct)
{
	//@todo加锁;
	if (!profileStruct) return;
	_profileStructVector.push_back(profileStruct);
	for (CallbackVector::const_iterator it = _callbacks.begin(); it != _callbacks.end(); ++it)
	{
		(*it)->onProfileStructAdded(profileStruct);
	}
}

void ProfileStructMgr::removeProfileStruct(ProfileStruct * profileStruct)
{
	//@todo加锁;
	ProfileStructerVector::iterator it =
		std::find(_profileStructVector.begin(), _profileStructVector.end(), profileStruct);
	if (it != _profileStructVector.end())
	{
		_profileStructVector.erase(it);
		for (CallbackVector::const_iterator it = _callbacks.begin(); it != _callbacks.end(); ++it)
		{
			(*it)->onProfileStructRemoved(profileStruct);
		}
	}
}

void ProfileStructMgr::addCallback(Callback * cb)
{
	//@todo加锁;
	if (!cb) return;
	_callbacks.push_back(cb);
}

void ProfileStructMgr::removeCallback(Callback * cb)
{
	//@todo加锁;
	CallbackVector::iterator it = std::find(_callbacks.begin(), _callbacks.end(), cb);
	if (it != _callbacks.end())
	{
		_callbacks.erase(it);
	}
}

//////////////////////////////////////////////////////////////////////////

namespace
{
	struct ProfileStructUpdate :public ProfileStruct::Callback
	{
		ProfileStructUpdate(ProfileStructNode* node) :_node(node) {}

		virtual void onProfileAdded(Profile* profile)
		{
			if (!_node) return;
			_node->onProfileAdded(profile);
		}

		virtual void onProfileRemoved(Profile* profile)
		{
			if (!_node) return;
			_node->onProfileRemoved(profile);
		}

		virtual void onVisibleChanged(bool isVisible)
		{
			if (!_node) return;
			_node->onVisibleChanged(isVisible);
		}

	private:
		osg::ref_ptr<ProfileStructNode> _node;
	};
}

ProfileStructNode::ProfileStructNode(ProfileStruct * pstruct)
	: _profileStruct(pstruct)
{
	if (_profileStruct)
	{
		init();
		_profileStruct->setSceneGraph(this);
		_profileStruct->addCallback(new ProfileStructUpdate(this));
	}
}

void ProfileStructNode::onVisibleChanged(bool isVisible)
{
	_isVisible = isVisible;
}

void ProfileStructNode::onProfileAdded(Profile * profile)
{
	const std::string& uid = profile->uid();
	if (_profileMap[uid])
	{
		return;//UID已经存在，换个UID
	}
	const osg::Plane& plane = profile->plane();
	const osg::Polytope& polytope = profile->polytope();

	//生成剖面;
	osg::ref_ptr<osg::Group> profilegroup = new osg::Group;

	addChild(profilegroup);
	_profileMap[uid] = profilegroup;
}

void ProfileStructNode::onProfileRemoved(Profile * profile)
{
	if (!profile) return;
	const std::string& uid = profile->uid();

	GroupMap::iterator it = _profileMap.find(uid);
	if (it == _profileMap.end())
	{
		return;
	}
	removeChild(it->second);
	_profileMap.erase(it);
}

void ProfileStructNode::onTileAdded(osg::Node * tileNode)
{
	//@todo加锁;
	if (!_profileStruct) return;
	if (_profileMap.empty()) return;

	std::string tileKey;
	if (!tileNode->getUserValue("tile_key", tileKey)) return;

	const ProfileVector& profiles = _profileStruct->getProfileVector();
	for (ProfileVector::const_iterator it = profiles.begin(); it != profiles.end(); ++it)
	{
		const osg::Plane& plane = (*it)->plane();
		const osg::Polytope& polytope = (*it)->polytope();

		osg::ref_ptr<osg::Node> profile = new osg::Node;//生成一个块的剖面;
		if (!profile) continue;

		osg::Group* g = _profileMap[(*it)->uid()];
		if (g)
		{
			g->addChild(profile);
		}
	}
}

void ProfileStructNode::onTileRemoved(osg::Node * tileNode)
{
	//@todo加锁;
	if (_profileMap.empty()) return;

	std::string tileKey;
	if (!tileNode->getUserValue("tile_key", tileKey)) return;

	for (GroupMap::iterator it = _profileMap.begin(); it != _profileMap.end(); ++it)
	{
		for (unsigned int i = 0; i < it->second->getNumChildren(); ++i)
		{
			std::string quadKey;
			if (!it->second->getChild(i)->getUserValue("tile_key", quadKey)) continue;
			if (tileKey != quadKey) continue;

			it->second->removeChild(it->second->getChild(i));
		}
	}
}

const ProfileStruct * ProfileStructNode::getProfileStruct() const
{
	return  _profileStruct;
}

void ProfileStructNode::traverse(osg::NodeVisitor & nv)
{
	if (_isVisible)
	{
		osg::Group::traverse(nv);
	}
}

void ProfileStructNode::init()
{
	const ProfileVector& profilevec = _profileStruct->getProfileVector();
	for (ProfileVector::const_iterator it = profilevec.begin(); it != profilevec.end(); ++it)
	{
		const std::string& uid = (*it)->uid();
		if (_profileMap[uid])
		{
			//UID不能相同
			return;
		}

		const osg::Plane& plane = (*it)->plane();
		const osg::Polytope& polytope = (*it)->polytope();

		osg::ref_ptr<osg::Group> profile = new osg::Group;
		addChild(profile);
		_profileMap[uid] = profile;
	}
}

//////////////////////////////////////////////////////////////////////////
namespace
{
	struct UpdateStructCallback :public ProfileStructMgr::Callback
	{
		UpdateStructCallback(ProfileStructMgrNode* node) :_node(node) {}

		virtual void onProfileStructAdded(ProfileStruct* profileStruct)
		{
			_node->onProfileStructAdded(profileStruct);
		}
		virtual void onProfileStructRemoved(ProfileStruct* profileStruct)
		{
			_node->onProfileStructRemoved(profileStruct);
		}

	private:
		osg::ref_ptr<ProfileStructMgrNode> _node;
	};

	struct UpdateTileCallback :public TileRegister::Callback
	{
		UpdateTileCallback(ProfileStructMgrNode*node) :_node(node) {}
		virtual void onTileAdded(TileNode* tileNode)
		{
			_node->onTileAdded(tileNode);
		}
		virtual void onTileRemoved(TileNode* tileNode)
		{
			_node->onTileRemoved(tileNode);
		}
	private:
		osg::ref_ptr<ProfileStructMgrNode> _node;
	};
}

ProfileStructMgrNode::ProfileStructMgrNode(ProfileStructMgr * profileStructMgr, TileRegister* tileRegister)
	: _profileStructMgr(profileStructMgr)
	, _tileRegister(tileRegister)
{
	if (_profileStructMgr)
	{
		_profileStructMgr->addCallback(new UpdateStructCallback(this));
	}
	if (_tileRegister)
	{
		_tileRegister->addCallback(new UpdateTileCallback(this));
	}
}

void ProfileStructMgrNode::onProfileStructAdded(ProfileStruct * profileStruct)
{
	//@todo加锁;
	osg::ref_ptr<ProfileStructNode> node = new ProfileStructNode(profileStruct);
	addChild(node);
	_nodeVector.push_back(new ProfileStructNode(profileStruct));
}

void ProfileStructMgrNode::onProfileStructRemoved(ProfileStruct * profileStruct)
{
	//@todo加锁;
	ProfileStructNodeVector::iterator it;
	for (it = _nodeVector.begin(); it != _nodeVector.end(); ++it)
	{
		if ((*it)->getProfileStruct() == profileStruct)
		{
			removeChild(*it);
			_nodeVector.erase(it);
			break;
		}
	}
}

void ProfileStructMgrNode::onTileAdded(osg::Node * tileNode)
{
	//@todo加锁;
	for (ProfileStructNodeVector::const_iterator it = _nodeVector.begin(); it != _nodeVector.end(); ++it)
	{
		(*it)->onTileAdded(tileNode);
	}
}

void ProfileStructMgrNode::onTileRemoved(osg::Node * tileNode)
{
	//@todo加锁;
	for (ProfileStructNodeVector::const_iterator it = _nodeVector.begin(); it != _nodeVector.end(); ++it)
	{
		(*it)->onTileRemoved(tileNode);
	}
}
