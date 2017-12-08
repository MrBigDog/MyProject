#include <gwBase/CacheMgr.h>

namespace gwBase
{
	CacheMgr::CacheMgr()
		: _defaultMemCacheBinSize(1000)
	{ }

	void CacheMgr::setDefaultMemCache(osgEarth::MemCache* memcache)
	{
		_defaultMemCache = memcache;
	}

	osgEarth::MemCache* CacheMgr::getOrCreateDefaultMemCache()
	{
		if (!_defaultMemCache.valid())
		{
			_defaultMemCache = new osgEarth::MemCache(_defaultMemCacheBinSize);
		}
		return _defaultMemCache;
	}

	MemCache* CacheMgr::addMemCache(const std::string& cacheId, MemCache* memcache, unsigned int cacheBinSize)
	{
		return _memCaches.getOrCreate(cacheId, memcache);
	}

	osgEarth::MemCache* CacheMgr::getOrcreateMemCache(const std::string& cacheId)
	{
		osgEarth::MemCache* memcache = _memCaches.get(cacheId);
		if (!memcache)
		{
			memcache = addMemCache(cacheId, new osgEarth::MemCache(_defaultMemCacheBinSize));
		}
		return memcache;
	}

	void CacheMgr::update(double delta_time_sec)
	{
		//
	}
}