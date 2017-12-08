#ifndef GWBASE_DIR_H_
#define GWBASE_DIR_H_ 1

#include <compiler.h>
#include <gwBase/export.h>
#include <gwBase/Path.h>

#include <sys/types.h>
#include <string>
#include <vector>


#ifdef _MSC_VER
typedef int mode_t;
#endif

namespace gwBase
{
	typedef std::vector<GWPath> PathList;

	class GW_BASE_EXPORT Dir
	{
	public:
		Dir();
		~Dir();

		/**
		 * when this directory object is destroyed, remove the corresponding
		 * diretory (and its contents) from the disk. Often used with temporary
		 * directories to ensure they are cleaned up.
		 */
		void setRemoveOnDestroy();

		static Dir current();

		/**
		 * Create a temporary directory, using the supplied name.
		 * The return value 'd' is such that d.isNull() in case this failed.
		 */
		static Dir tempDir(const std::string& templ);

		Dir(const GWPath& path);
		Dir(const Dir& rel, const GWPath& relPath);

		enum FileTypes
		{
			TYPE_FILE = 1,
			TYPE_DIR = 2,
			NO_DOT_OR_DOTDOT = 1 << 12,
			INCLUDE_HIDDEN = 1 << 13
		};

		PathList children(int types = 0, const std::string& nameGlob = "") const;

		/**
		 * Check if the underlying GWPath is null.
		 *
		 * Note: this is the case for a default-constructed Dir instance.
		 */
		bool isNull() const;

		/**
		 * test if the directory contains no children (except '.' and '..')
		 */
		bool isEmpty() const;

		GWPath file(const std::string& name) const;

		GWPath path() const
		{
			return _path;
		}

		/**
		 * create the directory (and any parents as required) with the
		 * request mode, or return failure
		 */
		bool create(mode_t mode);

		/**
		 * remove the directory.
		 * If recursive is true, contained files and directories are
		 * recursively removed
		 */
		bool remove(bool recursive = false);

		/**
		 * remove our children but not us
		 */
		bool removeChildren() const;


		/**
		 * Check that the directory at the path exists (and is a directory!)
		 */
		bool exists() const;

		/**
		 * parent directory, if one exists
		 */
		Dir parent() const;

	private:
		mutable GWPath _path;
		bool _removeOnDestroy;
	};
} // of namespace gwBase

#endif // _GWUTIL_DIR_H_


