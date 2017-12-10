#ifndef PROFILEREGISTER_H
#define PROFILEREGISTER_H 1

#include "TileRegister.h"
#include <osg/Group>
#include <osg/Plane>
#include <osg/Polytope>
#include <osg/Referenced>
#include <osg/ValueObject>

#include <vector>

typedef std::vector<osg::ref_ptr<osg::Node> > NodeVector;
typedef std::map<std::string, osg::ref_ptr<osg::Group> > GroupMap;

struct Profile :public osg::Referenced
{
	Profile(const osg::Plane& plane, const osg::Polytope& ptop)
		:_plane(plane), _polyTope(ptop)
	{}

	const std::string& uid() { return _uid; }
	const osg::Plane& plane() { return _plane; }
	const osg::Polytope& polytope() { return _polyTope; }

private:
	std::string _uid;
	osg::Plane _plane;
	osg::Polytope _polyTope;
};
typedef std::vector<osg::ref_ptr<Profile> > ProfileVector;

struct ProfileStruct :public osg::Referenced
{
	struct Callback :public osg::Referenced
	{
		virtual void onProfileAdded(Profile* profile) = 0;
		virtual void onProfileRemoved(Profile* profile) = 0;
		virtual void onVisibleChanged(bool isVisible) = 0;
	};
	typedef std::vector<osg::ref_ptr<Callback> > CallbackVector;

	ProfileStruct() :_isVisible(true) {}
	void addProfile(Profile* profile);
	void removeProfile(Profile* profile);
	void setVisible(bool isVisible);
	void addCallback(Callback* cb);
	void removeCallback(Callback* cb);

	void setSceneGraph(osg::Node*scene) { _scene = scene; }
	osg::Node* getSceneGraph() { return _scene; }

	std::string uid() { return _uid; }

	const ProfileVector& getProfileVector() const { return _profileVec; }

private:
	bool _isVisible;
	std::string _uid;
	ProfileVector _profileVec;
	CallbackVector _callbacks;
	osg::ref_ptr<osg::Node> _scene;
};
typedef std::vector<osg::ref_ptr< ProfileStruct> > ProfileStructerVector;

struct ProfileStructMgr :public osg::Referenced
{
	struct Callback :public osg::Referenced
	{
		virtual void onProfileStructAdded(ProfileStruct* profileStruct) = 0;
		virtual void onProfileStructRemoved(ProfileStruct* profileStruct) = 0;
	};
	typedef std::vector<osg::ref_ptr<Callback> > CallbackVector;

	ProfileStructMgr() {}

	void addProfileStruct(ProfileStruct* profileStruct);
	void removeProfileStruct(ProfileStruct* profileStruct);
	void addCallback(Callback* cb);
	void removeCallback(Callback* cb);

	const ProfileStructerVector& profileStructVector() { return _profileStructVector; }

private:
	CallbackVector _callbacks;
	ProfileStructerVector _profileStructVector;
};

//////////////////////////////////////////////////////////////////////////
struct ProfileStructNode :public osg::Group
{
	ProfileStructNode(ProfileStruct* pstruct);

	void onVisibleChanged(bool isVisible);

	void onProfileAdded(Profile* profile);
	void onProfileRemoved(Profile* profile);

	void onTileAdded(osg::Node* tileNode);
	void onTileRemoved(osg::Node* tileNode);

	const ProfileStruct* getProfileStruct() const;

	void traverse(osg::NodeVisitor& nv);

private:
	void init();

private:
	bool _isVisible;
	ProfileStruct* _profileStruct;//用普通指针，避免智能指针循环调用;
	GroupMap _profileMap;
};
typedef std::vector<osg::ref_ptr<ProfileStructNode> > ProfileStructNodeVector;


struct ProfileStructMgrNode :public osg::Group
{
	ProfileStructMgrNode(ProfileStructMgr* profileStructMgr, TileRegister* tileRegister);

	void onProfileStructAdded(ProfileStruct* profileStruct);
	void onProfileStructRemoved(ProfileStruct* profileStruct);
	void onTileAdded(osg::Node* tileNode);
	void onTileRemoved(osg::Node* tileNode);

private:
	ProfileStructNodeVector _nodeVector;
	osg::ref_ptr<ProfileStructMgr> _profileStructMgr;
	osg::ref_ptr<TileRegister> _tileRegister;
};

#endif // ProfileRegister_h__