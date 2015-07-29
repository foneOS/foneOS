cmake_minimum_required (VERSION 2.8.4)

set(EDISON_BUILD OFF CACHE BOOL "Controls if the build is targeting the Intel Edison.")

if(EDISON_BUILD)
        include(${CMAKE_SOURCE_DIR}/Edison.cmake)
endif(EDISON_BUILD)

project (foneOS)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

if (!EDISON_BUILD)
	find_package(SDL2 REQUIRED)
else(!EDISON_BUILD)
	set(SDL2_LIBRARY )
	set(SDL2_LIBRARY_RELEASE )
endif(!EDISON_BUILD)
find_package(freetype REQUIRED)
find_package(Sqlite3 REQUIRED)
find_package(LuaJIT REQUIRED)

# The version number.
#set (FONEOSVERSION "v0.1.2d")

add_subdirectory(foneOS)
