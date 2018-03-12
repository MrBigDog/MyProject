#include "FileUtils.h"
#include "StringUtils.h"
#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>
#include <osgDB/Registry>
#include <osg/Notify>
#include <stack>
#include <errno.h>

#ifdef WIN32
#  include <windows.h>
#else
#  include <stdio.h>
#  include <stdlib.h>
#endif

#include <sys/types.h>

#ifdef WIN32
#  include <sys/utime.h>
#else
#  include <utime.h>
#endif

#include <sys/stat.h>
#include <time.h>

#include <list>
#include <sstream>

// currently this impl is for _all_ platforms, except as defined.
// the mac version will change soon to reflect the path scheme under osx, but
// for now, the above include is commented out, and the below code takes precedence.

#if defined(WIN32) && !defined(__CYGWIN__)
#include <io.h>
#define WINBASE_DECLARE_GET_MODULE_HANDLE_EX
#include <windows.h>
#include <winbase.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <direct.h> // for _mkdir

#define mkdir(x,y) _mkdir((x))
#define stat64 _stati64

// set up for windows so acts just like unix access().
#ifndef F_OK
#define F_OK 4
#endif

#else // unix

#if defined( __APPLE__ )
	// I'm not sure how we would handle this in raw Darwin
	// without the AvailablilityMacros.
#include <AvailabilityMacros.h>

//>OSG_IOS
//IOS includes
#include "TargetConditionals.h"

#if (TARGET_OS_IPHONE)
#include <Availability.h>
// workaround a bug which appears when compiling for SDK < 4.0 and for the simulator
#if defined(__IPHONE_4_0) && (__IPHONE_OS_VERSION_MIN_REQUIRED >= __IPHONE_4_0)
#define stat64 stat
#else
#if !TARGET_IPHONE_SIMULATOR
#define stat64 stat
#endif
#endif
#endif
//<OSG_IPHONE

// 10.5 defines stat64 so we can't use this #define
// By default, MAC_OS_X_VERSION_MAX_ALLOWED is set to the latest
// system the headers know about. So I will use this as the control
// variable. (MIN_ALLOWED is set low by default so it is
// unhelpful in this case.)
// Unfortunately, we can't use the label MAC_OS_X_VERSION_10_4
// for older OS's like Jaguar, Panther since they are not defined,
// so I am going to hardcode the number.
#if (MAC_OS_X_VERSION_MAX_ALLOWED <= 1040)
#define stat64 stat
#endif
#elif defined(__CYGWIN__) || defined(__FreeBSD__) || (defined(__hpux) && !defined(_LARGEFILE64_SOURCE))
#define stat64 stat
#endif

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif

	// set up _S_ISDIR()
#if !defined(S_ISDIR)
#  if defined( _S_IFDIR) && !defined( __S_IFDIR)
#    define __S_IFDIR _S_IFDIR
#  endif
#  define S_ISDIR(mode)    (mode&__S_IFDIR)
#endif


#define LC "[FileUtils] "


using namespace gwUtil;





bool gwUtil::makeDirectory(const std::string &path)
{
	if (path.empty())
	{
		OSG_DEBUG << "osgDB::makeDirectory(): cannot create an empty directory" << std::endl;
		return false;
	}

	struct stat64 stbuf;
#ifdef OSG_USE_UTF8_FILENAME
	if (_wstat64(OSGDB_STRING_TO_FILENAME(path).c_str(), &stbuf) == 0)
#else
	if (stat64(path.c_str(), &stbuf) == 0)
#endif
	{
		if (S_ISDIR(stbuf.st_mode))
			return true;
		else
		{
			OSG_DEBUG << "osgDB::makeDirectory(): " <<
				path << " already exists and is not a directory!" << std::endl;
			return false;
		}
	}

	std::string dir = path;
	std::stack<std::string> paths;
	while (true)
	{
		if (dir.empty())
			break;

#ifdef OSG_USE_UTF8_FILENAME
		if (_wstat64(OSGDB_STRING_TO_FILENAME(dir).c_str(), &stbuf) < 0)
#else
		if (stat64(dir.c_str(), &stbuf) < 0)
#endif
		{
			switch (errno)
			{
			case ENOENT:
			case ENOTDIR:
				paths.push(dir);
				break;

			default:
				OSG_DEBUG << "osgDB::makeDirectory(): " << strerror(errno) << std::endl;
				return false;
			}
		}
		dir = osgDB::getFilePath(std::string(dir));
	}

	while (!paths.empty())
	{
		std::string dir = paths.top();

#if defined(WIN32)
		//catch drive name
		if (dir.size() == 2 && dir.c_str()[1] == ':') {
			paths.pop();
			continue;
		}
#endif

#ifdef OSG_USE_UTF8_FILENAME
		if (_wmkdir(OSGDB_STRING_TO_FILENAME(dir).c_str()) < 0)
#else
		if (mkdir(dir.c_str(), 0755) < 0)
#endif
		{
			if (osgDB::fileExists(dir))
			{
				OSG_DEBUG << "Attempt to create directory that already exists " << dir << std::endl;
			}
			else
			{
				OSG_DEBUG << "osgDB::makeDirectory(): " << strerror(errno) << std::endl;
				return false;
			}
		}
		paths.pop();
	}
	return true;
}

bool gwUtil::makeDirectoryForFile(const std::string &path)
{
	return gwUtil::makeDirectory(osgDB::getFilePath(path));
}

