include_directories("${PROJECT_SOURCE_DIR}/foneOS")
link_directories(${PROJECT_BINARY_DIR})

set(CORELIBS ${FREETYPE_LIBRARY} ${SQLITE3_LIBRARY} ${SDL2_LIBRARY})
 
# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/foneOS/FoneOSConfig.h.in"
  "${PROJECT_BINARY_DIR}/foneOS/FoneOSConfig.h"
  )
  
file(GLOB foneOS_src
    "*.h"
    "*.cpp"
)

add_executable(foneOS ${foneOS_src})

target_link_libraries(foneOS
	#PRIVATE "${PROJECT_NAME}"
	PRIVATE "${SDL2_LIBRARIES}"
	PRIVATE "sqlite3" # TODO: findsqlite3
	PRIVATE "${FREETYPE_LIBRARIES}"
)
include_directories(foneOS "${SDL2_INCLUDE_DIRS};${SQLITE3_INCLUDE_DIRS};${FREETYPE_INCLUDE_DIRS};")
# target_compile_options(foneOS "${SDL2_CFLAGS_OTHERS} ${SQLITE3_CFLAGS_OTHERS} ${FREETYPE_CFLAGS_OTHERS}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY LINK_DIRECTORIES)
message("dirs = ${dirs}")