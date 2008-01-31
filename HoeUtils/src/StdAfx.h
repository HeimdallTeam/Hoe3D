
#ifndef _STDAFX_H_
#define _STDAFX_H_

#if defined (_WIN32) //////////////////////////
//#define WINVER 0x0500
//#define _WIN32_WINNT 0x0500
//#define _CRT_SECURE_NO_DEPRECATE
#include <windows.h>
#include <tchar.h>
#include <math.h>
#endif

#if defined (_MACOSX)
#include <Carbon/Carbon.h>
#endif

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <wchar.h>

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif 

#include <HoeCore/hoe_types.h>
#include <HoeCore/hoe_string.h>


#endif // _STDAFX_H_

