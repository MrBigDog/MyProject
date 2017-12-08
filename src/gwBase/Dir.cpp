//#include <gwUtil_config.h>
#include <compiler.h>
#include <gwBase/Dir.h>
#include <gwBase/Exception.h>
#include <gwBase/StringUtils.h>
#include <gwBase/LogStream.h>

#ifdef _WIN32
#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>
#  include <direct.h>
#  include <Shlwapi.h>
#else
#  include <sys/types.h>
#  include <dirent.h>
#  include <sys/stat.h>
#  include <unistd.h>
#endif

#include <boost/foreach.hpp>

#include <math.h>
#include <stdlib.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cerrno>
#include <iostream>
#include <algorithm> // for std::sort

using std::string;

namespace gwBase
{
	Dir::Dir() : _removeOnDestroy(false)
	{
	}

	Dir::Dir(const GWPath& path)
		: _path(path)
		, _removeOnDestroy(false)
	{
		_path.set_cached(false); // disable caching, so create/remove work
	}

	Dir::Dir(const Dir& rel, const GWPath& relPath)
		:_path(rel.file(relPath.utf8Str()))
		, _removeOnDestroy(false)
	{
		_path.set_cached(false); // disable caching, so create/remove work
	}

	Dir::~Dir()
	{
		if (_removeOnDestroy)
		{
			remove(true);
		}
	}

	void Dir::setRemoveOnDestroy()
	{
		_removeOnDestroy = true;
	}

	Dir Dir::current()
	{
#if defined(GW_WINDOWS)
		wchar_t* buf = _wgetcwd(NULL, 0);
#else
		char *buf = ::getcwd(NULL, 0);
#endif
		if (!buf)
		{
			if (errno == 2) throw gw_exception("The current directory is invalid");
			else throw gw_exception(gwBase::strutils::error_string(errno));
		}

		GWPath p(buf);
		free(buf);
		return Dir(p);
	}

	Dir Dir::tempDir(const std::string& templ)
	{
#ifdef HAVE_MKDTEMP
		char buf[1024];
		const char* tempPath = ::getenv("TMPDIR");
		if (!tempPath) {
			tempPath = "/tmp";
		}
		GWPath p(tempPath);
		p.append(templ);
		// Mac OS-X / BSD manual says any number of 'X's, but GLibc manual
		// says exactly six, so that's what I'm going with
		p.concat("-XXXXXX");
		std::string s = p.local8BitStr();
		::snprintf(buf, 1024, "%s", s.c_str());
		if (!mkdtemp(buf)) {
			GW_LOG(GW_IO, GW_WARN,
				"mkdtemp failed: " << gwBase::strutils::error_string(errno));
			return Dir();
		}

		return Dir(GWPath(buf));
#else
#if defined(GW_WINDOWS)
		std::wstring wideTemplate = gwBase::strutils::convertUtf8ToWString(templ);
		wchar_t* buf = _wtempnam(0, wideTemplate.c_str());
		GWPath p(buf);
		free(buf); // unlike tempnam(), _wtempnam mallocs its result buffer
#else
		GWPath p(tempnam(0, templ.c_str()));
#endif
		Dir t(p);
		if (!t.create(0700))
		{
			GW_LOG(GW_IO, GW_WARN, "failed to create temporary directory at " << p);
			return Dir();
		}

		return t;
#endif
	}

	static bool pathSortPredicate(const GWPath& p1, const GWPath& p2)
	{
		return p1.file() < p2.file();
	}

	PathList Dir::children(int types, const std::string& nameFilter) const
	{
		PathList result;
		if (types == 0)
		{
			types = TYPE_FILE | TYPE_DIR | NO_DOT_OR_DOTDOT;
		}

#if defined(GW_WINDOWS)
		std::wstring search(_path.wstr());
		if (nameFilter.empty())
		{
			search += gwBase::strutils::convertUtf8ToWString("\\*"); // everything
		}
		else
		{
			search += gwBase::strutils::convertUtf8ToWString("\\*" + nameFilter);
		}

		WIN32_FIND_DATAW fData;
		HANDLE find = FindFirstFileW(search.c_str(), &fData);
		if (find == INVALID_HANDLE_VALUE)
		{
			int err = GetLastError();
			if (err != ERROR_FILE_NOT_FOUND)
			{
				GW_LOG(GW_GENERAL, GW_WARN, "Dir::children: FindFirstFile failed:" <<
					_path << " with error:" << err);
			}
			return result;
		}

		bool done = false;
		for (bool done = false; !done; done = (FindNextFileW(find, &fData) == 0))
		{
			if (fData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
			{
				if (!(types & INCLUDE_HIDDEN))
				{
					continue;
				}
			}

			std::string utf8File = gwBase::strutils::convertWStringToUtf8(fData.cFileName);
			if (fData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (types & NO_DOT_OR_DOTDOT)
				{
					if ((utf8File == ".") || (utf8File == ".."))
					{
						continue;
					}
				}

				if (!(types & TYPE_DIR))
				{
					continue;
				}
			}
			else if ((fData.dwFileAttributes & FILE_ATTRIBUTE_DEVICE) ||
				(fData.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT))
			{
				continue; // always ignore device files
			}
			else if (!(types & TYPE_FILE))
			{
				continue;
			}

			result.push_back(file(utf8File));
		}

		FindClose(find);
#else
		std::string ps = _path.local8BitStr();
		DIR* dp = opendir(ps.c_str());
		if (!dp) {
			GW_LOG(GW_GENERAL, GW_WARN, "Dir::children: opendir failed:" << _path);
			return result;
		}

		int filterLen = nameFilter.size();

		while (true) {
			struct dirent* entry = readdir(dp);
			if (!entry) {
				break; // done iteration
			}

			// skip hidden files (names beginning with '.') unless requested
			if (!(types & INCLUDE_HIDDEN) && (entry->d_name[0] == '.') &&
				strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) {
				continue;
			}

			GWPath f = file(entry->d_name);
			if (!f.exists()) {
				continue; // stat() failed
			}

			if (f.isDir()) {
				// directory handling
				if (!(types & TYPE_DIR)) {
					continue;
				}

				if (types & NO_DOT_OR_DOTDOT) {
					if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) {
						continue;
					}
				}
			}
			else if (f.isFile()) {
				// regular file handling
				if (!(types & TYPE_FILE)) {
					continue;
				}
			}
			else {
				// block device /fifo/char file, ignore
				continue;
			}

			if (!nameFilter.empty()) {
				int nameLen = strlen(entry->d_name);
				if (nameLen < filterLen) {
					continue; // name is shorter than the filter
				}

				char* nameSuffix = entry->d_name + (nameLen - filterLen);
				if (strcmp(nameSuffix, nameFilter.c_str())) {
					continue;
				}
			}

			// passed all criteria, add to our result vector
			result.push_back(file(entry->d_name));
		}

		closedir(dp);
#endif

		// File system order is random. Make things deterministic,
		// so it's the same for every user.
		std::sort(result.begin(), result.end(), pathSortPredicate);

		return result;
	}

	bool Dir::isNull() const
	{
		return _path.isNull();
	}

	bool Dir::isEmpty() const
	{
#if defined(GW_WINDOWS)
		std::wstring ps = _path.wstr();
		return PathIsDirectoryEmptyW(ps.c_str());
#else
		std::string ps = _path.local8BitStr();
		DIR* dp = opendir(ps.c_str());
		if (!dp) return true;

		int n = 0;
		dirent* d;
		while ((d = readdir(dp)) != NULL && (n < 4)) n++;
		closedir(dp);

		return (n == 2); // '.' and '..' always exist
#endif
	}

	bool Dir::exists() const
	{
		return _path.isDir();
	}

	GWPath Dir::file(const std::string& name) const
	{
		GWPath childPath = _path;
		childPath.set_cached(true);
		childPath.append(name);
		return childPath;
	}

	bool Dir::create(mode_t mode)
	{
		if (exists())
		{
			return false; // already exists
		}

		// recursively create parent directories
		Dir pr(parent());
		if (!pr.path().isNull() && !pr.exists())
		{
			bool ok = pr.create(mode);
			if (!ok)
			{
				return false;
			}
		}

		// finally, create ourselves
#if defined(GW_WINDOWS)
		std::wstring ps = _path.wstr();
		int err = _wmkdir(ps.c_str());
#else
		std::string ps = _path.local8BitStr();
		int err = mkdir(ps.c_str(), mode);
#endif
		if (err)
		{
			GW_LOG(GW_IO, GW_WARN,
				"directory creation failed for '" << _path.utf8Str() << "': " <<
				gwBase::strutils::error_string(errno));
		}

		return (err == 0);
	}

	bool Dir::removeChildren() const
	{
		if (!exists())
		{
			return true;
		}

		bool ok;
		PathList cs = children(NO_DOT_OR_DOTDOT | INCLUDE_HIDDEN | TYPE_FILE | TYPE_DIR);
		BOOST_FOREACH(GWPath path, cs)
		{
			if (path.isDir())
			{
				Dir childDir(path);
				ok = childDir.remove(true);
			}
			else
			{
				ok = path.remove();
			}

			if (!ok)
			{
				GW_LOG(GW_IO, GW_WARN, "failed to remove:" << path);
				return false;
			}
		} // of child iteration

		return true;
	}

	bool Dir::remove(bool recursive)
	{
		if (!exists())
		{
			GW_LOG(GW_IO, GW_WARN, "attempt to remove non-existant dir:" << _path);
			return false;
		}

		if (recursive)
		{
			if (!removeChildren())
			{
				GW_LOG(GW_IO, GW_WARN, "Dir at:" << _path << " failed to remove children");
				return false;
			}
		} // of recursive deletion

#if defined(GW_WINDOWS)
		std::wstring ps = _path.wstr();
		int err = _wrmdir(ps.c_str());
#else
		std::string ps = _path.local8BitStr();
		int err = rmdir(ps.c_str());
#endif
		if (err)
		{
			GW_LOG(GW_IO, GW_WARN,
				"rmdir failed for '" << _path.utf8Str() << "': " <<
				gwBase::strutils::error_string(errno));
		}
		return (err == 0);
	}

	Dir Dir::parent() const
	{
		return Dir(_path.dir());
	}

} // of namespace gwBase
