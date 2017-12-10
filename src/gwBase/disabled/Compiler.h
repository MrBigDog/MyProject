/**************************************************************************
 * Compiler.h -- C++ Compiler Portability Macros
 **************************************************************************/

 /** \file compiler.h
  * A set of defines to encapsulate compiler and platform differences.
  * Please refer to the source code for full documentation on this file.
  *
  * This file is useful to set compiler-specific options in every file - for
  * example, disabling warnings.
  *
  */

#ifndef GWBASE_COMPILER_H
#define GWBASE_COMPILER_H 1

  /*
   * Helper macro GW_STRINGIZE:
   * Converts the parameter X to a string after macro replacement
   * on X has been performed.
   */
#define GW_STRINGIZE(X) GW_DO_STRINGIZE(X)
#define GW_DO_STRINGIZE(X) #X

#ifdef __GNUC__
#  define GW_GCC_VERSION (__GNUC__ * 10000 \
                     + __GNUC_MINOR__ * 100 \
                     + __GNUC_PATCHLEVEL__)
#  define GW_COMPILER_STR "GNU C++ version " GW_STRINGIZE(__GNUC__) "." GW_STRINGIZE(__GNUC_MINOR__)
#endif // __GNUC__

   //
   // Microsoft compilers.
   //
#ifdef _MSC_VER
#  define strcasecmp stricmp

#  if _MSC_VER >= 1200 // msvc++ 6.0 up to MSVC2013
#    if _MSC_VER < 1900
#      define bcopy(from, to, n) memcpy(to, from, n)
#      define snprintf _snprintf
#      define strdup _strdup
#      define copysign _copysign
#    endif

#    pragma warning(disable: 4786) // identifier was truncated to '255' characters
#    pragma warning(disable: 4244) // conversion from double to float
#    pragma warning(disable: 4305) // truncation from larger type to smaller
#    pragma warning(disable: 4267) // conversion from size_t to int / 32-bit type
#    pragma warning(disable: 4996) // don't require _ prefix for standard library functions
#    pragma warning(disable: 4800) // don't warn about int -> bool performance
#    pragma warning(disable: 4819) // 
#  else
#    error What version of MSVC++ is this?
#  endif
#  define GW_COMPILER_STR "Microsoft Visual C++ version " GW_STRINGIZE(_MSC_VER)

#endif // _MSC_VER

//
// Intel C++ Compiler
//
#if defined(__ICC) || defined (__ECC)
#  define GW_COMPILER_STR "Intel C++ version " GW_STRINGIZE(__ICC)
#endif // __ICC

//
// Platform dependent gl.h and glut.h definitions
//

#ifdef __APPLE__
#  define GW_MAC
#  define GW_UNIX
#endif

#if defined (__FreeBSD__)
#  define GW_UNIX
#endif

#if defined (__CYGWIN__)
#  define GW_WINDOWS
#  define GW_UNIX
#  include <ieeefp.h>		// isnan
#endif

// includes both MSVC and mingw compilers
#if defined(_WIN32) || defined(__WIN32__)
#  define GW_WINDOWS
#endif

#if defined(__linux__) || defined(_AIX) || defined ( sgi )
#  define GW_UNIX
#endif

#ifdef __GNUC__
#define GW_DEPRECATED(func) func __attribute__ ((deprecated))
#elif defined(_MSC_VER)
#define GW_DEPRECATED(func) __declspec(deprecated) func
#else
#pragma message("WARNING: You need to implement GW_DEPRECATED for this compiler")
#define GW_DEPRECATED(func) func
#endif

#if defined(__clang__)
#  define GW_NO_RETURN [[noreturn]]
#else
#  define GW_NO_RETURN
#endif

//
// No user modifiable definitions beyond here.
//

#endif // GWBASE_COMPILER_H
