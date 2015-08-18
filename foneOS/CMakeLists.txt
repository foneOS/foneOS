include_directories("${PROJECT_SOURCE_DIR}")
link_directories(${PROJECT_BINARY_DIR})

set(CORELIBS ${FREETYPE_LIBRARY} ${SQLITE3_LIBRARY} ${SDL2_LIBRARY} ${MRAA_LIBRARY} )
 
# configure a header file to pass some of the CMake settings
# to the source code
#configure_file (
#  "${PROJECT_SOURCE_DIR}/foneOS/FoneOSConfig.h.in"
#  "${PROJECT_BINARY_DIR}/foneOS/FoneOSConfig.h"
#  )
  
file(GLOB foneOS_src
    "*.h"
    "*.cpp"
    "*.hpp"
)

add_executable(foneOS ${foneOS_src})

target_link_libraries(foneOS
	PRIVATE "${SDL2_LIBRARY_RELEASE}"
	PRIVATE "${MRAA_LIBRARY_RELEASE}"
	PRIVATE "${SQLITE3_LIBRARY_RELEASE}"
	PRIVATE "${FREETYPE_LIBRARY_RELEASE}"
	PRIVATE "${LUAJIT_LIBRARY_RELEASE}"
)

include_directories("${SDL2_INCLUDE_DIR};${MRAA_INCLUDE_DIR};${SQLITE3_INCLUDE_DIR};${FREETYPE_INCLUDE_DIR};${LUAJIT_INCLUDE_DIR};")
# target_compile_options(foneOS "${SDL2_CFLAGS_OTHERS} ${SQLITE3_CFLAGS_OTHERS} ${FREETYPE_CFLAGS_OTHERS}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

if (EDISON_BUILD)
file(RELATIVE_PATH relativeDir ${PROJECT_SOURCE_DIR} ${PROJECT_BINARY_DIR}/foneOS)
add_deployment_file(${relativeDir} ${deploymentFolderAbsolutPath})
endif(EDISON_BUILD)
