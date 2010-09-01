###############################################################################
# File:    Findlibdc1394.cmake
# Author:  Martin Wojtczyk <wojtczyk@in.tum.de>
# Purpose: Find the dc1394 library on Linux systems
###############################################################################
# On success this script defines
# LIBDC1394_INCLUDE_DIR            location of libdc1394/dc1394_control.h
# LIBDC1394_LIBRARY                full path to dc1394_control library

FIND_PATH(LIBDC1394_INCLUDE_DIR libdc1394/dc1394_control.h
	${CMAKE_INCLUDE_PATH}
	$ENV{LIBDC1394DIR}/include
	/usr/local/libdc1394/include
	/usr/local/include
	/usr/include
)

IF (NOT LIBDC1394_INCLUDE_DIR_FOUND)
	IF (LIBDC1394_INCLUDE_DIR)
		MESSAGE(STATUS "Looking for libdc1394 headers -- found " ${LIBDC1394_INCLUDE_DIR}/libdc1394/dc1394_control.h)
		SET(LIBDC1394_INCLUDE_DIR_FOUND 1 CACHE INTERNAL "libdc1394 headers found")
	ELSE (LIBDC1394_INCLUDE_DIR)
		MESSAGE(FATAL_ERROR 
			"-- Looking for libdc1394 headers -- not found\n"
			"Please install libdc1394 http://sourceforge.net/projects/libdc1394/ or adjust CMAKE_INCLUDE_PATH\n"
			"e.g. cmake -DCMAKE_INCLUDE_PATH=/path-to-libdc1394/include ..."
		)
	ENDIF (LIBDC1394_INCLUDE_DIR)
ENDIF (NOT LIBDC1394_INCLUDE_DIR_FOUND)

FIND_LIBRARY(LIBDC1394_LIBRARY
	NAMES dc1394_control
	PATHS
	${CMAKE_LIBRARY_PATH}
	$ENV{LIBDC1394DIR}/lib
	/usr/local/libdc1394/lib
	/usr/local/lib
	/usr/lib
)

IF (NOT LIBDC1394_LIBRARY_FOUND)
	IF (LIBDC1394_LIBRARY)
		MESSAGE(STATUS "Looking for libdc1394 library -- found " ${LIBDC1394_LIBRARY})
		SET(LIBDC1394_LIBRARY_FOUND 1 CACHE INTERNAL "libdc1394 library found")
	ELSE (LIBDC1394_LIBRARY)
		MESSAGE(FATAL_ERROR 
			"-- Looking for libdc1394 library -- not found\n"
			"Please install libdc1394 http://sourceforge.net/projects/libdc1394/ or adjust CMAKE_LIBRARY_PATH\n"
			"e.g. cmake -DCMAKE_LIBRARY_PATH=/path-to-libdc1394/lib ..."
		)
	ENDIF (LIBDC1394_LIBRARY)
ENDIF (NOT LIBDC1394_LIBRARY_FOUND)

MARK_AS_ADVANCED(
	LIBDC1394_INCLUDE_DIR
	LIBDC1394_LIBRARY
) 
