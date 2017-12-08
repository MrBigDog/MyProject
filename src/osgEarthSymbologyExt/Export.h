#ifndef OSGEARTHSYMBOLOGYEXT_EXPORT_H
#define OSGEARTHSYMBOLOGYEXT_EXPORT_H 1

// define USE_DEPRECATED_API is used to include in API which is being phased out
// if you can compile your apps with this turned off you are
// well placed for compatibility with future versions.
#define USE_DEPRECATED_API

#if defined(_MSC_VER)
#pragma warning( disable : 4244 )
#pragma warning( disable : 4251 )
#pragma warning( disable : 4267 )
#pragma warning( disable : 4275 )
#pragma warning( disable : 4290 )
#pragma warning( disable : 4786 )
#pragma warning( disable : 4305 )
#pragma warning( disable : 4996 )
#endif

#if defined(_MSC_VER) || defined(__CYGWIN__) || defined(__MINGW32__) || defined( __BCPLUSPLUS__)  || defined( __MWERKS__)
#  if defined( OSGEARTHSYMBOLOGYEXT_LIBRARY_STATIC )
#    define OSGEARTHSYMBOLOGYEXT_EXPORT
#  elif defined( OSGEARTHSYMBOLOGYEXT_LIBRARY )
#    define OSGEARTHSYMBOLOGYEXT_EXPORT   __declspec(dllexport)
#  else
#    define OSGEARTHSYMBOLOGYEXT_EXPORT   __declspec(dllimport)
#  endif
#else
#  define OSGEARTHSYMBOLOGYEXT_EXPORT
#endif  

// set up define for whether member templates are supported by VisualStudio compilers.
#ifdef _MSC_VER
# if (_MSC_VER >= 1300)
#  define __STL_MEMBER_TEMPLATES
# endif
#endif

/* Define NULL pointer value */

#ifndef NULL
#ifdef  __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif

#ifndef OSG_MIN_VERSION_REQUIRED
#define OSG_MIN_VERSION_REQUIRED(MAJOR, MINOR, PATCH) ((OPENSCENEGRAPH_MAJOR_VERSION>MAJOR) || (OPENSCENEGRAPH_MAJOR_VERSION==MAJOR && (OPENSCENEGRAPH_MINOR_VERSION>MINOR || (OPENSCENEGRAPH_MINOR_VERSION==MINOR && OPENSCENEGRAPH_PATCH_VERSION>=PATCH))))
#endif

#ifndef OSG_VERSION_LESS_THAN
#define OSG_VERSION_LESS_THAN(MAJOR, MINOR, PATCH) ((OPENSCENEGRAPH_MAJOR_VERSION<MAJOR) || (OPENSCENEGRAPH_MAJOR_VERSION==MAJOR && (OPENSCENEGRAPH_MINOR_VERSION<MINOR || (OPENSCENEGRAPH_MINOR_VERSION==MINOR && OPENSCENEGRAPH_PATCH_VERSION<PATCH))))
#endif

#ifndef OSG_VERSION_LESS_OR_EQUAL
#define OSG_VERSION_LESS_OR_EQUAL(MAJOR, MINOR, PATCH) ((OPENSCENEGRAPH_MAJOR_VERSION<MAJOR) || (OPENSCENEGRAPH_MAJOR_VERSION==MAJOR && (OPENSCENEGRAPH_MINOR_VERSION<MINOR || (OPENSCENEGRAPH_MINOR_VERSION==MINOR && OPENSCENEGRAPH_PATCH_VERSION<=PATCH))))
#endif

#ifndef OSG_VERSION_GREATER_THAN
#define OSG_VERSION_GREATER_THAN(MAJOR, MINOR, PATCH) ((OPENSCENEGRAPH_MAJOR_VERSION>MAJOR) || (OPENSCENEGRAPH_MAJOR_VERSION==MAJOR && (OPENSCENEGRAPH_MINOR_VERSION>MINOR || (OPENSCENEGRAPH_MINOR_VERSION==MINOR && OPENSCENEGRAPH_PATCH_VERSION>PATCH))))
#endif

#ifndef OSG_VERSION_GREATER_OR_EQUAL
#define OSG_VERSION_GREATER_OR_EQUAL(MAJOR, MINOR, PATCH) ((OPENSCENEGRAPH_MAJOR_VERSION>MAJOR) || (OPENSCENEGRAPH_MAJOR_VERSION==MAJOR && (OPENSCENEGRAPH_MINOR_VERSION>MINOR || (OPENSCENEGRAPH_MINOR_VERSION==MINOR && OPENSCENEGRAPH_PATCH_VERSION>=PATCH))))
#endif

#endif