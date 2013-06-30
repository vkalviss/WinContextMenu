// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


void MyDebugOutput(const TCHAR* szFormat, ...);

#ifdef DEBUG
#define TRACE( format, ...) \
    MyDebugOutput( format, ##__VA_ARGS__)
#else
#define TRACE( format, ...)
#endif

/* #define TRACE( format, ...) \ */
/*     MyDebugOutput( format, ##__VA_ARGS__) */
