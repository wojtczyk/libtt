###############################################################################
# File:    Findlibraw1394.cmake
# Author:  Martin Wojtczyk <wojtczyk@in.tum.de>
# Purpose: Find the raw1394 library on Linux systems
###############################################################################
# On success this script defines
# LIBRAW1394_INCLUDE_DIR           location of libraw1394/raw1394.h
# LIBRAW1394_LIBRARY               full path to raw1394 library

FIND_PATH(LIBRAW1394_INCLUDE_DIR libraw1394/raw1394.h
	${CMAKE_INCLUDE_PATH}
	$ENV{LIBRAW1394DIR}/include
	/usr/local/libraw1394/include
	/usr/local/include
	/usr/include
)

IF (NOT LIBRAW1394_INCLUDE_DIR_FOUND)
	IF (LIBRAW1394_INCLUDE_DIR)
		MESSAGE(STATUS "Looking for libraw1394 headers -- found " ${LIBRAW1394_INCLUDE_DIR}/libraw1394/raw1394.h)
		SET(LIBRAW1394_INCLUDE_DIR_FOUND 1 CACHE INTERNAL "libraw1394 headers found")
	ELSE (LIBRAW1394_INCLUDE_DIR)
		MESSAGE(FATAL_ERROR 
			"-- Looking for libraw1394 headers -- not found\n"
			"Please install libraw1394 http://sourceforge.net/projects/libraw1394/ or adjust CMAKE_INCLUDE_PATH\n"
			"e.g. cmake -DCMAKE_INCLUDE_PATH=/path-to-libraw1394/include ..."
		)
	ENDIF (LIBRAW1394_INCLUDE_DIR)
ENDIF (NOT LIBRAW1394_INCLUDE_DIR_FOUND)

FIND_LIBRARY(LIBRAW1394_LIBRARY
	NAMES raw1394
	PATHS
	${CMAKE_LIBRARY_PATH}
	$ENV{LIBRAW1394DIR}/lib
	/usr/local/libraw1394/lib
	/usr/local/lib
	/usr/lib
)

IF (NOT LIBRAW1394_LIBRARY_FOUND)
	IF (LIBRAW1394_LIBRARY)
		MESSAGE(STATUS "Looking for libraw1394 library -- found " ${LIBRAW1394_LIBRARY})
		SET(LIBRAW1394_LIBRARY_FOUND 1 CACHE INTERNAL "libraw1394 library found")
	ELSE (LIBRAW1394_LIBRARY)
		MESSAGE(FATAL_ERROR 
			"-- Looking for libraw1394 library -- not found\n"
			"Please install libraw1394 http://sourceforge.net/projects/libraw1394/ or adjust CMAKE_LIBRARY_PATH\n"
			"e.g. cmake -DCMAKE_LIBRARY_PATH=/path-to-libraw1394/lib ..."
		)
	ENDIF (LIBRAW1394_LIBRARY)
ENDIF (NOT LIBRAW1394_LIBRARY_FOUND)

MARK_AS_ADVANCED(
	LIBRAW1394_INCLUDE_DIR
	LIBRAW1394_LIBRARY
) 
