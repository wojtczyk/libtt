###############################################################################
# File:    FindCMU1394.cmake
# Author:  Martin Wojtczyk <wojtczyk@in.tum.de>
# Purpose: Find the CMU1394 library on Windows systems
###############################################################################
# On success this script defines
# CMU1394_INCLUDES               location of 1394Camera.h
# CMU1394_LIBRARIES              full path to 1394camera library

FIND_PATH(CMU1394_INCLUDES 1394Camera.h
	${CMAKE_INCLUDE_PATH}
	$ENV{CMU1394DIR}/include
	$ENV{ProgramFiles}/CMU/1394Camera/include
)

FIND_LIBRARY(CMU1394_LIBRARIES
	NAMES 1394camera
	PATHS
	${CMAKE_LIBRARY_PATH}
	$ENV{CMU1394DIR}/include
	$ENV{HOME}/lib
	$ENV{ProgramFiles}/CMU/1394Camera/lib
)

MARK_AS_ADVANCED(
	CMU1394_INCLUDES
	CMU1394_LIBRARIES
)
