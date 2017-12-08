#ifndef GWBASE_PATH_H
#define GWBASE_PATH_H 1

#include <compiler.h>
#include <gwBase/export.h>
#include <gwBase/TypesDef.h>

#include <sys/types.h>
#include <string>
#include <ctime>
#include <vector>

#ifdef _MSC_VER
typedef int mode_t;
#endif

/**
 * A class to hide path separator difference across platforms and assist
 * in managing file system path names.
 *
 * Paths can be input in any platform format and will be converted
 * automatically to the proper format.
 */
class GW_BASE_EXPORT GWPath
{
public:
	// OS-dependent separator used in paths lists
	static const char pathListSep;

	struct Permissions
	{
		bool read : 1;
		bool write : 1;
	};
	typedef Permissions(*PermissionChecker)(const GWPath&);

	explicit GWPath(PermissionChecker validator = NULL);

	GWPath(const GWPath& p);
	GWPath& operator=(const GWPath& p);

	/**
	 * Construct a path based on the starting path provided.
	 * @param p initial path
	 */
	GWPath(const std::string& p, PermissionChecker validator = NULL);

	explicit GWPath(const std::wstring& p, PermissionChecker validator = NULL);

	/**
	 * Construct a path based on the starting path provided and a relative subpath
	 * @param p initial path
	 * @param r relative subpath
	 */
	GWPath(const GWPath& p,	const std::string& r, PermissionChecker validator = NULL);
	~GWPath();

	/**
	 * Set path to a new value
	 * @param p new path
	 */
	void set(const std::string& p);
	GWPath& operator= (const char* p) { this->set(p); return *this; }

	bool operator==(const GWPath& other) const;
	bool operator!=(const GWPath& other) const;

	void setPermissionChecker(PermissionChecker validator);
	PermissionChecker getPermissionChecker() const;

	/**
	 * Set if file information (exists, type, mod-time) is cached or
	 * retrieved each time it is queried. Caching is enabled by default
	 */
	void set_cached(bool cached);

	/**
	 * Append another piece to the existing path.  Inserts a path
	 * separator between the existing component and the new component.
	 * @param p additional path component */
	void append(const std::string& p);

	/**
	 * Get a copy of this path with another piece appended.
	 *
	 * @param p additional path component
	 */
	GWPath operator/(const std::string& p) const;

	/**
	 * Append a new piece to the existing path.  Inserts a search path
	 * separator to the existing path and the new patch component.
	 * @param p additional path component */
	void add(const std::string& p);

	/**
	 * Concatenate a string to the end of the path without inserting a
	 * path separator.
	 * @param p additional path suffix
	 */
	void concat(const std::string& p);

	/**
	 * Returns a path with the absolute pathname that names the same file, whose
	 * resolution does not involve '.', '..', or symbolic links.
	 */
	GWPath realpath() const;

	/**
	 * Get the file part of the path (everything after the last path sep)
	 * @return file string
	 */
	std::string file() const;

	/**
	 * Get the directory part of the path.
	 * @return directory string
	 */
	std::string dir() const;

	/**
	 * Get the base part of the path (everything but the final extension.)
	 * @return the base string
	 */
	std::string base() const;

	/**
	 * Get the base part of the filename (everything before the first '.')
	 * @return the base filename
	 */
	std::string file_base() const;

	/**
	 * Get the extension part of the path (everything after the final ".")
	 * @return the extension string
	 */
	std::string extension() const;

	/**
	 * Get the extension part of the path (everything after the final ".")
	 * converted to lowercase
	 * @return the extension string
	 */
	std::string lower_extension() const;

	/**
	 * Get the complete extension part of the path (everything after the first ".")
	 * this might look like 'tar.gz' or 'txt.Z', or might be identical to 'extension' above
	 * the extension is converted to lowercase.
	 * @return the extension string
	 */
	std::string complete_lower_extension() const;

	/**
	 * Get the path string
	 * @return path string
	 */
	std::string str() const { return path; }
	std::string utf8Str() const { return path; }

	std::string local8BitStr() const;

	std::wstring wstr() const;

	/**
	 * Get the path string
	 * @return path in "C" string (ptr to char array) form.
	 */
	const char* c_str() const { return path.c_str(); }

	/**
	 * Get the path string in OS native form
	 */
	std::string str_native() const;

	/**
	 * Determine if file exists by attempting to fopen it.
	 * @return true if file exists, otherwise returns false.
	 */
	bool exists() const;

	/**
	 * Create the designated directory.
	 *
	 * @param mode Permissions. See:
	 *    http://en.wikipedia.org/wiki/File_system_permissions#Numeric_notation
	 * @return 0 on success, or <0 on failure.
	 */
	int create_dir(mode_t mode = 0755);

	/**
	 * Check if reading file is allowed. Readabilty does not imply the existance
	 * of the file.
	 *
	 * @note By default all files will be marked as readable. No check is made
	 *       if the operating system allows the given file to be read. Derived
	 *       classes may actually implement custom read/write rights.
	 */
	bool canRead() const;
	bool canWrite() const;

	bool isFile() const;
	bool isDir() const;

	/**
	 * Opposite sense to isAbsolute
	 */
	bool isRelative() const { return !isAbsolute(); }

	/**
	 * Is this an absolute path?
	 * I.e starts with a directory seperator, or a single character + colon
	 */
	bool isAbsolute() const;

	/**
	 * check for default constructed path
	 */
	bool isNull() const;

	/**
	 * delete the file, if possible
	 */
	bool remove();

	/**
	 * modification time of the file
	 */
	time_t modTime() const;

	/**
	 *
	 */
	size_t sizeInBytes() const;

	/**
	 * rename the file / directory we point at, to a new name
	 * this may fail if the new location is on a different volume / share,
	 * or if the destination already exists, or is not writeable
	 */
	bool rename(const GWPath& newName);


	/**
	 * return the path of the parent directory of this path.
	 */
	GWPath dirPath() const;

	enum StandardLocation
	{
		HOME,
		DESKTOP,
		DOWNLOADS,
		DOCUMENTS,
		PICTURES
	};

	static GWPath standardLocation(StandardLocation type, const GWPath& def = GWPath());

	/**
	 * Get a path stored in the environment variable with the given \a name.
	 *
	 * @param name  Name of the environment variable
	 * @param def   Default path to return if the environment variable does not
	 *              exist or is empty.
	 */
	static GWPath fromEnv(const char* name, const GWPath& def = GWPath());

	static GWPath fromUtf8(const std::string& bytes, PermissionChecker p = NULL);

	static GWPath fromLocal8Bit(const char* name);

	/**
	 * Get path to user's home directory
	 */
	static GWPath home(const GWPath& def = GWPath());

	/**
	 * Get path to the user's desktop directory
	 */
	static GWPath desktop(const GWPath& def = GWPath());

	/**
	 * Get path to the user's documents directory
	 */
	static GWPath documents(const GWPath& def = GWPath());

	static std::vector<GWPath> pathsFromEnv(const char* name);

	static std::vector<GWPath> pathsFromUtf8(const std::string& paths);

	static std::vector<GWPath> pathsFromLocal8Bit(const std::string& paths);

	static std::string join(const std::vector<GWPath>& paths, const std::string& joinWith);
private:

	void fix();

	void validate() const;
	void checkAccess() const;

	bool permissionsAllowsWrite() const;

	std::string path;
	PermissionChecker _permission_checker;

	mutable bool _cached : 1;
	mutable bool _rwCached : 1;
	bool _cacheEnabled : 1; ///< cacheing can be disbled if required
	mutable bool _canRead : 1;
	mutable bool _canWrite : 1;
	mutable bool _exists : 1;
	mutable bool _isDir : 1;
	mutable bool _isFile : 1;
	mutable time_t _modTime;
	mutable size_t _size;
};

/// Output to an ostream
template<typename char_type, typename traits_type>
inline
std::basic_ostream<char_type, traits_type>&
operator<<(std::basic_ostream<char_type, traits_type>& s, const GWPath& p)
{
	return s << "Path \"" << p.utf8Str() << "\"";
}

/**
 * Split a directory string into a list of it's parent directories.
 */
/*string_list*/StringList GW_BASE_EXPORT sgPathBranchSplit(const std::string &path);

/**
 * Split a directory search path into a vector of individual paths
 */
StringList GW_BASE_EXPORT sgPathSplit(const std::string &search_path);


#endif // GWUTIL_PATH_H


