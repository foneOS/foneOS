// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "FoneOSConfig.h"

#include "targetver.h"

#include <stdio.h>

#ifdef _MSVC_VER
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

#include "windows.h"

#define _WINDOWS
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

#define FoneOSEndL STR("\r\n")

#include "SDL.h"

#include "sqlite3.h"

#include "ft2build.h"
#include FT_FREETYPE_H
#include "ftglyph.h"

#include "pugiconfig.h"
#include "pugixml.h"

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
#include "Modem.h"

#include "Layout.h"

#include "Core.h"