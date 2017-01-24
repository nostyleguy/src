// ======================================================================
//
// FoundationTypes.cpp
// copyright (c) 2001 Sony Online Entertainment
//
// ======================================================================

#ifndef INCLUDED_FoundationTypes_H
#define INCLUDED_FoundationTypes_H

// ======================================================================

#ifdef WIN32
#include "../win32/FoundationTypesWin32.h"
#elif  LINUX
#include "../linux/FoundationTypesLinux.h"
#else
#error unsupported platform
#endif

// ======================================================================

typedef unsigned char          byte;
typedef uint32_t           uint;
typedef uint16_t         uint16_t;

// ======================================================================

#endif

