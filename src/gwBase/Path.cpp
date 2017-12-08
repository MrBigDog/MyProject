//#include <gwUtil_config.h>
#include <compiler.h>
#include <gwBase/Stream.h>
#include <gwBase/Path.h>
#include <gwBase/LogStream.h>
#include <gwBase/StringUtils.h>

#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>
#include <fstream>
#include <cstdlib>

#ifdef _WIN32
#  include <direct.h>
#endif

#include <boost/algorithm/string/case_conv.hpp>

using std::string;
using gwBase::strutils::starts_with;

/**
 * define directory path separators
 */

static const char sgDirPathSep = '/';
static const char sgDirPathSepBad = '\\';

#ifdef _WIN32
const char GWPath::pathListSep = ';';
#else
const char GWPath::pathListSep = ':';
#endif

#ifdef _WIN32
#include <ShlObj.h>         // for CSIDL
// TODO: replace this include file with the official <versionhelpers.h> header
// included in the Windows 8.1 SDK
#include <gwBase/PlatformVersionhelpers.h>

#define ENABLE_OLD_PATH_API 1

static GWPath pathForCSIDL(int csidl, const GWPath& def)
{
	typedef BOOL(WINAPI*GetSpecialFolderPath)(HWND, PWSTR, int, BOOL);
	static GetSpecialFolderPath SHGetSpecialFolderPath = NULL;

	// lazy open+resolve of shell32
	if (!SHGetSpecialFolderPath)
	{
		HINSTANCE shellDll = ::LoadLibrary("shell32");
		SHGetSpecialFolderPath = (GetSpecialFolderPath)GetProcAddress(shellDll, "SHGetSpecialFolderPathW");
	}

	if (!SHGetSpecialFolderPath)
	{
		return def;
	}

	wchar_t path[MAX_PATH];
	if (SHGetSpecialFolderPath(0, path, csidl, false))
	{
		return GWPath(std::wstring(path), def.getPermissionChecker());
	}

	return def;
}

static GWPath pathForKnownFolder(REFKNOWNFOLDERID folderId, const GWPath& def)
{
	typedef HRESULT(WINAPI*PSHGKFP)(REFKNOWNFOLDERID, DWORD, HANDLE, PWSTR*);

	HINSTANCE shellDll = LoadLibrary(TEXT("shell32"));
	if (shellDll != NULL)
	{
		PSHGKFP pSHGetKnownFolderPath = (PSHGKFP)GetProcAddress(shellDll, "SHGetKnownFolderPath");
		if (pSHGetKnownFolderPath != NULL)
		{
			// system call will allocate dynamic memory... which we must release when done
			wchar_t* localFolder = 0;

			if (pSHGetKnownFolderPath(folderId, KF_FLAG_DEFAULT_PATH, NULL, &localFolder) == S_OK)
			{
				GWPath folder_path = GWPath(localFolder, def.getPermissionChecker());
				// release dynamic memory
				CoTaskMemFree(static_cast<void*>(localFolder));

				return folder_path;
			}
		}
		FreeLibrary(shellDll);
	}
	return def;
}

#elif __APPLE__

// defined in CocoaHelpers.mm
GWPath appleSpecialFolder(int dirType, int domainMask, const GWPath& def);

#else
static GWPath getXDGDir(const std::string& name,
	const GWPath& def,
	const std::string& fallback)
{
	// http://standards.freedesktop.org/basedir-spec/basedir-spec-latest.html

	// $XDG_CONFIG_HOME defines the base directory relative to which user specific
	// configuration files should be stored. If $XDG_CONFIG_HOME is either not set
	// or empty, a default equal to $HOME/.config should be used.
	const GWPath user_dirs = GWPath::fromEnv("XDG_CONFIG_HOME",
		GWPath::home() / ".config")
		/ "user-dirs.dirs";

	// Format is XDG_xxx_DIR="$HOME/yyy", where yyy is a shell-escaped
	// homedir-relative path, or XDG_xxx_DIR="/yyy", where /yyy is an absolute
	// path. No other format is supported.
	const std::string XDG_ID = "XDG_" + name + "_DIR=\"";

	gw_ifstream user_dirs_file(user_dirs);
	std::string line;
	while (std::getline(user_dirs_file, line).good())
	{
		if (!starts_with(line, XDG_ID) || *line.rbegin() != '"')
			continue;

		// Extract dir from XDG_<name>_DIR="<dir>"
		line = line.substr(XDG_ID.length(), line.length() - XDG_ID.length() - 1);

		const std::string HOME = "$HOME";
		if (starts_with(line, HOME))
			return GWPath::home(def)
			/ gwBase::strutils::unescape(line.substr(HOME.length()));

		return GWPath(line, def.getPermissionChecker());
	}

	if (def.isNull())
		return GWPath::home(def) / fallback;

	return def;
}
#endif

// For windows, replace "\" by "/".
void GWPath::fix()
{
	string::size_type sz = path.size();
	for (string::size_type i = 0; i < sz; ++i)
	{
		if (path[i] == sgDirPathSepBad)
		{
			path[i] = sgDirPathSep;
		}
	}
	// drop trailing "/"
	while ((sz > 1) && (path[sz - 1] == sgDirPathSep))
	{
		path.resize(--sz);
	}
}


// default constructor
GWPath::GWPath(PermissionChecker validator)
	: path("")
	, _permission_checker(validator)
	, _cached(false)
	, _rwCached(false)
	, _cacheEnabled(true)
{
}


// create a path based on "path"
GWPath::GWPath(const std::string& p, PermissionChecker validator)
	: path(p)
	, _permission_checker(validator)
	, _cached(false)
	, _rwCached(false)
	, _cacheEnabled(true)
{
	fix();
}

// create a path based on "path"
GWPath::GWPath(const std::wstring& p, PermissionChecker validator)
	: _permission_checker(validator)
	, _cached(false)
	, _rwCached(false)
	, _cacheEnabled(true)
{
	path = gwBase::strutils::convertWStringToUtf8(p);
	fix();
}


// create a path based on "path" and a "subpath"
GWPath::GWPath(const GWPath& p, const std::string& r, PermissionChecker validator)
	: path(p.path)
	, _permission_checker(validator)
	, _cached(false)
	, _rwCached(false)
	, _cacheEnabled(p._cacheEnabled)
{
	append(r);
	fix();
}

GWPath GWPath::fromLocal8Bit(const char *name)
{
	return GWPath(gwBase::strutils::convertWindowsLocal8BitToUtf8(name));
}

GWPath GWPath::fromUtf8(const std::string& bytes, PermissionChecker p)
{
	return GWPath(bytes, p);
}


GWPath::GWPath(const GWPath& p)
	: path(p.path)
	, _permission_checker(p._permission_checker)
	, _cached(p._cached)
	, _rwCached(p._rwCached)
	, _cacheEnabled(p._cacheEnabled)
	, _canRead(p._canRead)
	, _canWrite(p._canWrite)
	, _exists(p._exists)
	, _isDir(p._isDir)
	, _isFile(p._isFile)
	, _modTime(p._modTime)
	, _size(p._size)
{ }

GWPath& GWPath::operator=(const GWPath& p)
{
	path = p.path;
	_permission_checker = p._permission_checker;
	_cached = p._cached;
	_rwCached = p._rwCached;
	_cacheEnabled = p._cacheEnabled;
	_canRead = p._canRead;
	_canWrite = p._canWrite;
	_exists = p._exists;
	_isDir = p._isDir;
	_isFile = p._isFile;
	_modTime = p._modTime;
	_size = p._size;
	return *this;
}

GWPath::~GWPath()
{
}

#if defined(ENABLE_OLD_PATH_API)
// set path
void GWPath::set(const string& p)
{
	path = p;
	fix();
	_cached = false;
	_rwCached = false;
}
#endif

//------------------------------------------------------------------------------
void GWPath::setPermissionChecker(PermissionChecker validator)
{
	_permission_checker = validator;
	_rwCached = false;
}

//------------------------------------------------------------------------------
GWPath::PermissionChecker GWPath::getPermissionChecker() const
{
	return _permission_checker;
}

//------------------------------------------------------------------------------
void GWPath::set_cached(bool cached)
{
	_cacheEnabled = cached;
	_cached = false;
}

// append another piece to the existing path
void GWPath::append(const string& p)
{
	if (path.empty())
	{
		path = p;
	}
	else
	{
		if (p[0] != sgDirPathSep)
		{
			path += sgDirPathSep;
		}
		path += p;
	}
	fix();
	_cached = false;
	_rwCached = false;
}

//------------------------------------------------------------------------------
GWPath GWPath::operator/(const std::string& p) const
{
	GWPath ret = *this;
	ret.append(p);
	return ret;
}

#if defined(ENABLE_OLD_PATH_API)
//add a new path component to the existing path string
void GWPath::add(const string& p)
{
	append(GWPath::pathListSep + p);
}
#endif

// concatenate a string to the end of the path without inserting a
// path separator
void GWPath::concat(const string& p)
{
	if (path.empty())
	{
		path = p;
	}
	else
	{
		path += p;
	}
	fix();
	_cached = false;
	_rwCached = false;
}

std::string GWPath::local8BitStr() const
{
	return gwBase::strutils::convertUtf8ToWindowsLocal8Bit(path);
}

// Get the file part of the path (everything after the last path sep)
string GWPath::file() const
{
	string::size_type index = path.rfind(sgDirPathSep);
	if (index != string::npos)
	{
		return path.substr(index + 1);
	}
	else
	{
		return path;
	}
}

// get the directory part of the path.
string GWPath::dir() const
{
	int index = path.rfind(sgDirPathSep);
	if (index >= 0)
	{
		return path.substr(0, index);
	}
	else
	{
		return "";
	}
}

GWPath GWPath::dirPath() const
{
	return GWPath::fromUtf8(dir());
}

// get the base part of the path (everything but the extension.)
string GWPath::base() const
{
	string::size_type index = path.rfind(".");
	string::size_type lastSep = path.rfind(sgDirPathSep);

	// tolerate dots inside directory names
	if ((lastSep != string::npos) && (index < lastSep))
	{
		return path;
	}

	if (index != string::npos)
	{
		return path.substr(0, index);
	}
	else
	{
		return path;
	}
}

string GWPath::file_base() const
{
	string::size_type index = path.rfind(sgDirPathSep);
	if (index == string::npos)
	{
		index = 0; // no separator in the name
	}
	else
	{
		++index; // skip past the separator
	}

	string::size_type firstDot = path.find(".", index);
	if (firstDot == string::npos)
	{
		return path.substr(index); // no extensions
	}

	return path.substr(index, firstDot - index);
}

// get the extension (everything after the final ".")
// but make sure no "/" follows the "." character (otherwise it
// is has to be a directory name containing a ".").
string GWPath::extension() const
{
	int index = path.rfind(".");
	if ((index >= 0) && (path.find("/", index) == string::npos))
	{
		return path.substr(index + 1);
	}
	else
	{
		return "";
	}
}

string GWPath::lower_extension() const
{
	return boost::to_lower_copy(extension());
}

string GWPath::complete_lower_extension() const
{
	string::size_type index = path.rfind(sgDirPathSep);
	if (index == string::npos)
	{
		index = 0; // no separator in the name
	}
	else
	{
		++index; // skip past the separator
	}

	string::size_type firstDot = path.find(".", index);
	if ((firstDot != string::npos) && (path.find(sgDirPathSep, firstDot) == string::npos))
	{
		return boost::to_lower_copy(path.substr(firstDot + 1));
	}
	else
	{
		return "";
	}
}

//------------------------------------------------------------------------------
void GWPath::validate() const
{
	if (_cached && _cacheEnabled)
	{
		return;
	}

	if (path.empty())
	{
		_exists = false;
		_canWrite = _canRead = false;
		return;
	}

#if defined(GW_WINDOWS)
	struct _stat buf;
	bool remove_trailing = false;
	std::wstring statPath(wstr());
	if ((path.length() > 1) && (path.back() == '/'))
	{
		statPath.pop_back();
	}

	if (_wstat(statPath.c_str(), &buf) < 0)
	{
		_exists = false;
		_canRead = false;

		// check parent directory for write-ability
		std::wstring parentPath = gwBase::strutils::convertUtf8ToWString(dir());
		struct _stat parentBuf;
		if (_wstat(parentPath.c_str(), &parentBuf) >= 0)
		{
			_canWrite = parentBuf.st_mode & _S_IWRITE;
		}
		else
		{
			_canWrite = false;
		}
	}
	else
	{
		_exists = true;
		_isFile = ((S_IFREG & buf.st_mode) != 0);
		_isDir = ((S_IFDIR & buf.st_mode) != 0);
		_modTime = buf.st_mtime;
		_size = buf.st_size;
		_canRead = _S_IREAD & buf.st_mode;
		_canWrite = _S_IWRITE & buf.st_mode;
	}

#else
	struct stat buf;

	if (stat(path.c_str(), &buf) < 0)
	{
		_exists = false;
		_canRead = false;

		// check parent directory for write-ability
		std::string parentPath = dir();
		struct stat parentBuf;
		if (stat(parentPath.c_str(), &parentBuf) >= 0)
		{
			_canWrite = parentBuf.st_mode & S_IWUSR;
		}
		else
		{
			_canWrite = false;
		}
	}
	else
	{
		_exists = true;
		_isFile = ((S_ISREG(buf.st_mode)) != 0);
		_isDir = ((S_ISDIR(buf.st_mode)) != 0);
		_modTime = buf.st_mtime;
		_size = buf.st_size;
		_canRead = S_IRUSR & buf.st_mode;
		_canWrite = S_IWUSR & buf.st_mode;
	}

#endif
	// ensure permissions are no less restrictive than what the
	// permissions checker offers
	if (_permission_checker)
	{
		Permissions p = _permission_checker(*this);
		_canRead &= p.read;
		_canWrite &= p.write;
	}

	_cached = true;
}

//------------------------------------------------------------------------------
void GWPath::checkAccess() const
{
	if (_rwCached && _cacheEnabled)
	{
		return;
	}
	validate();
	_rwCached = true;
}

bool GWPath::exists() const
{
	validate();
	return _exists;
}

//------------------------------------------------------------------------------
bool GWPath::canRead() const
{
	checkAccess();
	return _canRead;
}

//------------------------------------------------------------------------------
bool GWPath::canWrite() const
{
	checkAccess();
	return _canWrite;
}

bool GWPath::isDir() const
{
	validate();
	return _exists && _isDir;
}

bool GWPath::isFile() const
{
	validate();
	return _exists && _isFile;
}

//------------------------------------------------------------------------------
int GWPath::create_dir(mode_t mode)
{
	if (!permissionsAllowsWrite())
	{
		//GW_LOG(GW_IO, GW_WARN, "Error creating directory for '" << *this << "'reason: access denied");
		return -3;
	}

	GWPath dirP = dirPath();
	if (dirP.isNull())
	{
		return -1;
	}
	StringList path_elements = sgPathBranchSplit(dirP.utf8Str());
	bool absolute = dirP.isAbsolute();
	unsigned int i = 1;

#if defined(GW_WINDOWS)
	GWPath dir(path_elements.front(), _permission_checker);
	// exists() does not work for drive letter paths, eg 'C:\'.
	// Detect this case and skip to the next element immediately
	if (absolute && path_elements.size() >= 2)
	{
		dir.append(path_elements[i++]);
	}
#else
	GWPath dir((absolute ? "/" : "") + path_elements.front(), _permission_checker);
#endif
	while (dir.exists() && (i < path_elements.size()))
	{
		dir.append(path_elements[i++]);
	}

	// already exists
	if (dir.exists() && (i == path_elements.size()))
	{
		return 0;
	}

	for (;;)
	{
#if defined (GW_WINDOWS)
		std::wstring ds = dir.wstr();
		if (_wmkdir(ds.c_str()))
#else
		std::string ds = dir.utf8Str();
		if (mkdir(ds.c_str(), mode))
#endif
		{
			//GW_LOG(GW_IO, GW_ALERT, "Error creating directory: (" << dir << "):"<< gwBase::strutils::error_string(errno));
			return errno;
		}
		else
		{
			//GW_LOG(GW_IO, GW_DEBUG, "Directory created: " << dir);
		}
		if (i >= path_elements.size())
		{
			break;
		}
		dir.append(path_elements[i++]);
	}
	_cached = false; // re-stat on next query
	return 0;
}

StringList sgPathBranchSplit(const string &dirpath)
{
	StringList path_elements;
	string element, path = dirpath;
	while (!path.empty())
	{
		size_t p = path.find(sgDirPathSep);
		if (p != string::npos)
		{
			element = path.substr(0, p);
			path.erase(0, p + 1);
		}
		else
		{
			element = path;
			path = "";
		}
		if (!element.empty())
		{
			path_elements.push_back(element);
		}
	}
	return path_elements;
}

StringList sgPathSplit(const string &search_path)
{
	string tmp = search_path;
	StringList result;
	result.clear();

	bool done = false;
	while (!done)
	{
		int index = tmp.find(GWPath::pathListSep);
		if (index >= 0)
		{
			result.push_back(tmp.substr(0, index));
			tmp = tmp.substr(index + 1);
		}
		else
		{
			if (!tmp.empty())
			{
				result.push_back(tmp);
			}
			done = true;
		}
	}

	return result;
}

bool GWPath::isAbsolute() const
{
	if (path.empty())
	{
		return false;
	}

#ifdef _WIN32
	// detect '[A-Za-z]:/'
	if (path.size() > 2)
	{
		if (isalpha(path[0]) && (path[1] == ':') && (path[2] == sgDirPathSep))
		{
			return true;
		}
	}
#endif
	return (path[0] == sgDirPathSep);
}

bool GWPath::isNull() const
{
	return path.empty();
}

#if defined(ENABLE_OLD_PATH_API)
std::string GWPath::str_native() const
{
#ifdef _WIN32
	std::string s = local8BitStr();
	std::string::size_type pos;
	std::string nativeSeparator;
	nativeSeparator = sgDirPathSepBad;
	while ((pos = s.find(sgDirPathSep)) != std::string::npos)
	{
		s.replace(pos, 1, nativeSeparator);
	}
	return s;
#else
	return utf8Str();
#endif
}
#endif

//------------------------------------------------------------------------------
bool GWPath::remove()
{
	if (!permissionsAllowsWrite())
	{
		//GW_LOG(GW_IO, GW_WARN, "file remove failed: (" << *this << ") reason: access denied");
		return false;
	}

#if defined(GW_WINDOWS)
	std::wstring ps = wstr();

	// windows forbids removing a read-only file, let's try to deal
	// with that case
	int err = _wchmod(ps.c_str(), _S_IWRITE | _S_IREAD);
	if (err != 0)
	{
		//GW_LOG(GW_IO, GW_WARN, "failed to make file writeable prior to remove:" << *this);
	}
	else
	{
		err = _wunlink(ps.c_str());
	}
#else
	std::string ps = local8BitStr();
	int err = ::unlink(ps.c_str());
#endif
	if (err)
	{
		//GW_LOG(GW_IO, GW_WARN, "file remove failed: (" << *this << ") reason: " << strerror(errno));
		// TODO check if failed unlink can really change any of the cached values
	}
	_cached = false; // stat again if required
	_rwCached = false;
	return (err == 0);
}

time_t GWPath::modTime() const
{
	validate();
	return _modTime;
}

size_t GWPath::sizeInBytes() const
{
	validate();
	return _size;
}

bool GWPath::operator==(const GWPath& other) const
{
	return (path == other.path);
}

bool GWPath::operator!=(const GWPath& other) const
{
	return (path != other.path);
}

//------------------------------------------------------------------------------
bool GWPath::rename(const GWPath& newName)
{
	if (!canRead() || !canWrite() || !newName.canWrite())
	{
		//GW_LOG(GW_IO, GW_WARN, "rename failed: from " << *this <<
		//	" to " << newName <<
		//	" reason: access denied");
		return false;
	}

#ifdef GW_WINDOWS
	if (newName.exists())
	{
		GWPath r(newName);
		if (!r.remove())
		{
			return false;
		}
	}
#endif

#if defined(GW_WINDOWS)
	std::wstring p = wstr();
	std::wstring np = newName.wstr();
	if (_wrename(p.c_str(), np.c_str()) != 0)
#else
	std::string p = local8BitStr();
	std::string np = newName.local8BitStr();
	if (::rename(p.c_str(), np.c_str()) != 0)
#endif

	{
		//GW_LOG(GW_IO, GW_WARN, "rename failed: from " << *this <<
		//	" to " << newName <<
		//	" reason: " << strerror(errno));
		return false;
	}

	path = newName.path;

	// Do not remove permission checker (could happen for example if just using
	// a std::string as new name)
	if (newName._permission_checker)
	{
		_permission_checker = newName._permission_checker;
	}
	_cached = false;
	_rwCached = false;

	return true;
}

//------------------------------------------------------------------------------
GWPath GWPath::standardLocation(StandardLocation type, const GWPath& def)
{
	switch (type)
	{
	case HOME:
		return home(def);

#ifdef _WIN32
	case DESKTOP:
		if (IsWindowsVistaOrGreater())
			return pathForKnownFolder(FOLDERID_Desktop, def);

		return pathForCSIDL(CSIDL_DESKTOPDIRECTORY, def);

	case DOWNLOADS:
		if (IsWindowsVistaOrGreater())
			return pathForKnownFolder(FOLDERID_Downloads, def);

		if (!def.isNull())
			return def;

		return pathForCSIDL(CSIDL_DESKTOPDIRECTORY, def);

	case DOCUMENTS:
		if (IsWindowsVistaOrGreater())
			return pathForKnownFolder(FOLDERID_Documents, def);

		return pathForCSIDL(CSIDL_MYDOCUMENTS, def);

	case PICTURES:
		if (IsWindowsVistaOrGreater())
			return pathForKnownFolder(FOLDERID_Pictures, def);

		return pathForCSIDL(CSIDL_MYPICTURES, def);

#elif __APPLE__
		// since this is C++, we can't include NSPathUtilities.h to access the enum
		// values, so hard-coding them here (they are stable, don't worry)
	case DOWNLOADS:
		return appleSpecialFolder(15, 1, def);
	case DESKTOP:
		return appleSpecialFolder(12, 1, def);
	case DOCUMENTS:
		return appleSpecialFolder(9, 1, def);
	case PICTURES:
		return appleSpecialFolder(19, 1, def);
#else
	case DESKTOP:
		return getXDGDir("DESKTOP", def, "Desktop");
	case DOWNLOADS:
		return getXDGDir("DOWNLOADS", def, "Downloads");
	case DOCUMENTS:
		return getXDGDir("DOCUMENTS", def, "Documents");
	case PICTURES:
		return getXDGDir("PICTURES", def, "Pictures");
#endif
	default:
		//GW_LOG(GW_GENERAL,
		//	GW_WARN,
		//	"GWPath::standardLocation() unhandled type: " << type);
		return def;
	}
}

//------------------------------------------------------------------------------
GWPath GWPath::fromEnv(const char* name, const GWPath& def)
{
#if defined(GW_WINDOWS)
	std::wstring wname = gwBase::strutils::convertUtf8ToWString(name);
	const wchar_t* val = _wgetenv(wname.c_str());
	if (val && val[0])
		return GWPath(val, def._permission_checker);
#else
	const char* val = getenv(name);
	if (val && val[0])
		return GWPath(val, def._permission_checker);
#endif
	return def;
}

//------------------------------------------------------------------------------

std::vector<GWPath> GWPath::pathsFromEnv(const char *name)
{
	std::vector<GWPath> r;
#if defined(GW_WINDOWS)
	std::wstring wname = gwBase::strutils::convertUtf8ToWString(name);
	const wchar_t* val = _wgetenv(wname.c_str());
#else
	const char* val = getenv(name);
#endif
	if (!val)
	{
		return r;
	}

#if defined(GW_WINDOWS)
	return pathsFromUtf8(gwBase::strutils::convertWStringToUtf8(val));
#else
	return pathsFromUtf8(val);
#endif
}

//------------------------------------------------------------------------------

std::vector<GWPath> GWPath::pathsFromUtf8(const std::string& paths)
{
	std::vector<GWPath> r;
	StringList items = sgPathSplit(paths);
	StringListIterator it;
	for (it = items.begin(); it != items.end(); ++it)
	{
		r.push_back(GWPath::fromUtf8(it->c_str()));
	}

	return r;
}

//------------------------------------------------------------------------------

std::vector<GWPath> GWPath::pathsFromLocal8Bit(const std::string& paths)
{
	std::vector<GWPath> r;
	StringList items = sgPathSplit(paths);
	StringListIterator it;
	for (it = items.begin(); it != items.end(); ++it)
	{
		r.push_back(GWPath::fromLocal8Bit(it->c_str()));
	}

	return r;
}

//------------------------------------------------------------------------------
GWPath GWPath::home(const GWPath& def)
{
#ifdef _WIN32
	return fromEnv("USERPROFILE", def);
#else
	return fromEnv("HOME", def);
#endif
}

//------------------------------------------------------------------------------
GWPath GWPath::desktop(const GWPath& def)
{
	return standardLocation(DESKTOP, def);
}

//------------------------------------------------------------------------------
GWPath GWPath::documents(const GWPath& def)
{
	return standardLocation(DOCUMENTS, def);
}

//------------------------------------------------------------------------------
GWPath GWPath::realpath() const
{
#if defined(GW_WINDOWS)
	// with absPath NULL, will allocate, and ignore length
	std::wstring ws = wstr();
	wchar_t *buf = _wfullpath(NULL, ws.c_str(), _MAX_PATH);
#else
	// POSIX
	char* buf = ::realpath(path.c_str(), NULL);
#endif
	if (!buf) // File does not exist: return the realpath it would have if created now
			  // (needed for fgValidatePath security)
	{
		if (path.empty())
		{
			return GWPath(".").realpath(); // current directory
		}
		std::string this_dir = dir();
		if (isAbsolute() && this_dir.empty())
		{ // top level
			this_dir = "/";
		}
		if (file() == "..")
		{
			this_dir = GWPath(this_dir).realpath().dir();
			if (this_dir.empty())
			{ // invalid path: .. above root
				return GWPath();
			}
			return GWPath(this_dir).realpath(); // use native path separator,
						// and handle 'existing/nonexisting/../symlink' paths
		}
		return GWPath(this_dir).realpath() / file();
	}

#if defined(GW_WINDOWS)
	GWPath p = GWPath(std::wstring(buf), NULL);
#else
	GWPath p(GWPath::fromLocal8Bit(buf));
#endif
	free(buf);
	return p;

}

//------------------------------------------------------------------------------

std::string GWPath::join(const std::vector<GWPath>& paths, const std::string& joinWith)
{
	std::string r;
	if (paths.empty())
	{
		return r;
	}

	r = paths[0].utf8Str();
	for (size_t i = 1; i < paths.size(); ++i)
	{
		r += joinWith + paths[i].utf8Str();
	}

	return r;
}

//------------------------------------------------------------------------------
std::wstring GWPath::wstr() const
{
	return gwBase::strutils::convertUtf8ToWString(path);
}

//------------------------------------------------------------------------------
bool GWPath::permissionsAllowsWrite() const
{
	return _permission_checker ? _permission_checker(*this).write : true;
}