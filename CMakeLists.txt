cmake_minimum_required (VERSION 2.8.4)
project (foneOS)

find_package(PkgConfig REQUIRED)
pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(sqlite3 REQUIRED sqlite3)

find_package(freetype REQUIRED)

# The version number.
set (FONEOSVERSION "v0.1.2d")

add_subdirectory(foneOS)