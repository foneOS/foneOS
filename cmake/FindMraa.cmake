# - find MRAA
# MRAA_INCLUDE_DIR - Where to find MRAA header files (directory)
# MRAA_LIBRARIES - MRAA libraries
# MRAA_LIBRARY_RELEASE - Where the release library is
# MRAA_LIBRARY_DEBUG - Where the debug library is
# MRAA_FOUND - Set to TRUE if we found everything (library, includes and executable)

# Copyright (c) 2015  , < >
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#
# Generated by CModuler, a CMake Module Generator - http://gitorious.org/cmoduler

IF( MRAA_INCLUDE_DIR AND MRAA_LIBRARY_RELEASE AND MRAA_LIBRARY_DEBUG )
    SET(MRAA_FIND_QUIETLY TRUE)
ENDIF( MRAA_INCLUDE_DIR AND MRAA_LIBRARY_RELEASE AND MRAA_LIBRARY_DEBUG )

FIND_PATH( MRAA_INCLUDE_DIR mraa.h  )

FIND_LIBRARY(MRAA_LIBRARY_RELEASE NAMES mraa HINTS /usr/lib/)

FIND_LIBRARY(MRAA_LIBRARY_DEBUG NAMES mraa  HINTS /usr/lib/debug/usr/lib/ )

IF( MRAA_LIBRARY_RELEASE OR MRAA_LIBRARY_DEBUG AND MRAA_INCLUDE_DIR )
	SET( MRAA_FOUND TRUE )
ENDIF( MRAA_LIBRARY_RELEASE OR MRAA_LIBRARY_DEBUG AND MRAA_INCLUDE_DIR )

IF( MRAA_LIBRARY_DEBUG AND MRAA_LIBRARY_RELEASE )
	# if the generator supports configuration types then set
	# optimized and debug libraries, or if the CMAKE_BUILD_TYPE has a value
	IF( CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE )
		SET( MRAA_LIBRARIES optimized ${MRAA_LIBRARY_RELEASE} debug ${MRAA_LIBRARY_DEBUG} )
	ELSE( CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE )
    # if there are no configuration types and CMAKE_BUILD_TYPE has no value
    # then just use the release libraries
		SET( MRAA_LIBRARIES ${MRAA_LIBRARY_RELEASE} )
	ENDIF( CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE )
ELSEIF( MRAA_LIBRARY_RELEASE )
	SET( MRAA_LIBRARIES ${MRAA_LIBRARY_RELEASE} )
ELSE( MRAA_LIBRARY_DEBUG AND MRAA_LIBRARY_RELEASE )
	SET( MRAA_LIBRARIES ${MRAA_LIBRARY_DEBUG} )
ENDIF( MRAA_LIBRARY_DEBUG AND MRAA_LIBRARY_RELEASE )

IF( MRAA_FOUND )
	MESSAGE( STATUS "Found MRAA header file in ${MRAA_INCLUDE_DIR}")
	MESSAGE( STATUS "Found MRAA libraries: ${MRAA_LIBRARIES}")
ELSE(MRAA_FOUND)
	MESSAGE( FATAL_ERROR "Could not find MRAA" )
ENDIF(MRAA_FOUND)
