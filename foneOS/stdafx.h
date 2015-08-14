// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

// TODO: There's no one place for this TODO, so here it is.
//		 Find all the places using Utils::FoneOSStringToCharArray() and change them to not memory leak on non-Unicode platforms.
//		 (because FoneOSStringToCharArray uses strdup if no Unicode)

#pragma once

#include "FoneOSConfig.h"

#if defined(WIN32) || defined(WIN64) || defined(CYGWIN)
#define WINDOWS
#endif
#if defined(linux) || defined(__linux) || defined(__linux__)
#define LINUX
#endif
#if defined(LINUX) && defined(__i586) && defined(__GNUC__) && defined(__pentium)
#define EDISON // You see, the `i586-poky-linux-gcc` cross-compiler the Edison SDK comes with doesn't have a specific define for Edison.
			   // So, this uses an oddly specific amount of defines in the hopes that there's something unique about that GCC.
			   // It doesn't work very well.
			   // TODO: Replace this with CMake magic.
#endif

// These defines try and sort out if you want simulator or production automagically.

#ifdef WINDOWS
#define SIMULATOR_BUILD
#endif
#ifdef EDISON
#define PRODUCTION_BUILD
#endif

// To override that, comment them out and uncomment one of the following lines.
// #define SIMULATOR_BUILD
// #define PRODUCTION_BUILD

#ifdef WINDOWS
    #include "targetver.h"
#endif

#include <stdio.h>
#include <unistd.h>

#ifdef _MSC_VER
#include <tchar.h>
#endif

#include <sys/types.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

#ifdef WINDOWS
#include "windows.h"
#endif

#define _USRDLL
#define USE_TCL
#define _CRT_SECURE_NO_DEPRECATE
#define THREADSAFE 1
#define TEMP_STORE 1
#define SQLITE_MAX_EXPR_DEPTH 0

#ifdef UNICODE
typedef std::wstring FoneOSString;
#define FoneOSChar wchar_t
#define FoneOSOut std::wcout
#define FoneOSIn std::wcin
#define FoneOSIStringStream std::wistringstream
#define FoneOSOStringStream std::wostringstream
#define FoneOSIFileStream std::wifstream
#define STR(s) L ## s
#else
typedef std::string FoneOSString;
#define FoneOSChar char
#define FoneOSOut std::cout
#define FoneOSIn std::cin
#define FoneOSIStringStream std::istringstream
#define FoneOSOStringStream std::ostringstream
#define FoneOSIFileStream std::ifstream
#define STR(s) s
#endif

#ifdef WINDOWS
#define FoneOSEndL STR("\r\n")
#else
#define FoneOSEndL STR("\n")
#endif

#ifdef SIMULATOR_BUILD
#include "SDL.h"
#endif

#ifdef PRODUCTION_BUILD
#include "mraa.h"
#endif

#include "sqlite3.h"

#include "ft2build.h"
#include FT_FREETYPE_H
#include "ftglyph.h"

#include "pugiconfig.h"
#include "pugixml.h"

#include "lua.hpp"

#include "FoneOSColor.h"
#include "FoneOSPoint.h"

#define COLOR_BLACK FoneOSColor(0, 0, 0, 255)
#define COLOR_GRAY2 FoneOSColor(40, 40, 40, 255) //0x4208
#define COLOR_RED FoneOSColor(255, 0, 0, 255)
#define COLOR_WHITE FoneOSColor(255, 255, 255, 255)

#include "Logging.h"
#include "Utils.h"

#include "Storage.h"
#include "Database.h"
#include "Type.h"

#include "Device.h"

#include "Display.h"
#include "Input.h"
#include "TouchInput.h"
#include "Modem.h"

#include "HardwareManager.h"

#include "Layout.h"

#include "App.h"

#include "Core.h"
