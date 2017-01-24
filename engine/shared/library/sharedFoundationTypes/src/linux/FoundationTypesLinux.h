// PRIVATE.  Do not export this header file outside the package.

#ifndef INCLUDED_FoundationTypesLinux_H
#define INCLUDED_FoundationTypesLinux_H

// ======================================================================
// specify what platform we're running on.

#define PLATFORM_UNIX
#define PLATFORM_LINUX

#include <stdint.h>
#include <cstdio>
// ======================================================================
// basic types that we assume to be around

typedef unsigned int           uint;
typedef unsigned char          uint8;
typedef uint16_t               uint16;
typedef uint32_t	       uint32;
typedef signed char            int8;
typedef int16_t                int16;
typedef int32_t                int32;
typedef int64_t		       int64;
typedef uint64_t	       uint64;
typedef float                  real;
typedef FILE*                  FILE_HANDLE;

#endif


