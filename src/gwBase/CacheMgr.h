#ifndef GWBASE_CACHEMANAGER_H
#define GWBASE_CACHEMANAGER_H 1

#include <compiler.h>
#include <gwBase/Export.h>
#include <gwBase/SubsystemMgr.h>

#include <osgEarth/MemCache>

using namespace osgEarth;

namespace gwBase
{
	//class MemeryCache :public osgEarth::MemCache
	//{
	//public:
	//	MemeryCache(const std::string& cacheId, unsigned int maxBinSize)
	//		:osgEarth::MemCache(maxBinSize)
	//		, _cacheId(cacheId)
	//	{ }

	//	std::string Id() { return _cacheId; }

	//private:
	//	std::string _cacheId;
	//};

	class GW_BASE_EXPORT CacheMgr : public Subsystem
	{
	public:
		CacheMgr();
		~CacheMgr() {}

		void setDefaultMemCache(MemCache* memcache);
		MemCache* getOrCreateDefaultMemCache();

		MemCache* addMemCache(const std::string& cacheId, MemCache* memcache, unsigned int cacheBinSize = 1000);
		MemCache* getOrcreateMemCache(const std::string& cacheId);

		virtual void update(double delta_time_sec);

	private:
		typedef osgEarth::PerObjectRefMap<std::string, MemCache> ThreadSafeMemCacheMap;

		ThreadSafeMemCacheMap _memCaches;
		osg::ref_ptr<osgEarth::MemCache> _defaultMemCache;

		unsigned int _defaultMemCacheBinSize;

	};
}


#endif // CacheManager_h__
