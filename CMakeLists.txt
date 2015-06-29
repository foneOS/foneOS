cmake_minimum_required (VERSION 2.8.4)
project (foneOS)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}")

find_package(PkgConfig REQUIRED)
pkg_search_module(SDL2 REQUIRED sdl2)

find_package(freetype REQUIRED)
find_package(Sqlite3 REQUIRED)

# The version number.
set (FONEOSVERSION "v0.1.2d")

add_subdirectory(foneOS)