#ifndef GWBASE_ICACHEMANAGER_H
#define GWBASE_ICACHEMANAGER_H 1

#include <gwApp/IApplication.h>
#include <gwBase/ISubsystemMgr.h>

#include <osgEarth/MemCache>

namespace gwBase
{
	class ICacheMgr : public ISubsystem
	{
	public:
		ICacheMgr() { }
		virtual ~ICacheMgr() {}

		virtual void setDefaultMemCache(osgEarth::MemCache* memcache) = 0;
		virtual osgEarth::MemCache* getOrCreateDefaultMemCache() = 0;

		virtual osgEarth::MemCache* addMemCache(const std::string& cacheId, osgEarth::MemCache* memcache) = 0;
		virtual osgEarth::MemCache* getOrcreateMemCache(const std::string& cacheId) = 0;
	};
}


#endif // ICacheManager_h__
