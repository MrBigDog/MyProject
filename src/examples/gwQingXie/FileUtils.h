#ifndef GWGEOLOGICALTUIL_FILEUTILS_H
#define GWGEOLOGICALTUIL_FILEUTILS_H

//#include "Common.h"
//#include "DateTime.h"
#include <vector>
#include <string>

namespace gwUtil
{

	/** Make a new directory.  Returns true if directory exists or was created.
	 * Note:  This is adapted from osg's makeDirectory function to ensure that it works with concurrent create attempts.
	*/
	extern /*GWGEOLOGICALUTIL_EXPORT*/ bool makeDirectory(const std::string &directoryPath);

	/** Make a new directory for a given file.
	 * Note:  This is adapted from osg's makeDirectoryForFile to call our custom makeDirectory function.
	*/
	extern /*GWGEOLOGICALUTIL_EXPORT*/ bool makeDirectoryForFile(const std::string &filePath);


}

#endif
