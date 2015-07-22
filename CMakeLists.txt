cmake_minimum_required (VERSION 2.8.4)
project (foneOS)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

find_package(SDL2 REQUIRED)
find_package(freetype REQUIRED)
find_package(Sqlite3 REQUIRED)
find_package(LuaJIT REQUIRED)

# The version number.
#set (FONEOSVERSION "v0.1.2d")

add_subdirectory(foneOS)