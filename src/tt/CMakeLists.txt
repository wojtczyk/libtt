PROJECT(tt CXX C)

#MESSAGE(STATUS "Configuring: ${PROJECT_NAME}")

# make sure the common library checks are included
#IF(NOT LIBRARYCOMMON)
	#MESSAGE(FATAL_ERROR "Please use the toplevel CMakeLists.txt, exiting...")
#ENDIF(NOT LIBRARYCOMMON)

# check the availability of required 3rd party dependecies
FIND_PACKAGE(opencv)

# add here global include directories
INCLUDE_DIRECTORIES(
#	${SRC_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/..
	${OPENCV_INCLUDES}
)

# define shortcuts for the directories (used in each specific section below)
SET(DS_SUB_DIR ds)
SET(INPUT_SUB_DIR input)
SET(OUTPUT_SUB_DIR output)

###############################################################################
# Platform specific extensions
###############################################################################
IF(WIN32) # platform defines for Windows 32bit Systems
	# define math constants like M_PI on Windows
	ADD_DEFINITIONS(-D_USE_MATH_DEFINES)
	# use the winGSL library
	ADD_DEFINITIONS(-DGSL_DLL)

ELSEIF(APPLE) # platform defines for Mac OS X Systems
	IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
		ADD_DEFINITIONS(-DPOSIX -DAPPLE)
	ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

ELSE(WIN32)	# assumes linux/UNIX # platform defines for Linux Systems
	ADD_DEFINITIONS(-DPOSIX -DLINUX)
ENDIF(WIN32)

################################################################################
## specific to namespace ds
################################################################################

SET(DS_HDRS
	${DS_SUB_DIR}/Image.h
)

SET(DS_SRCS
	${DS_SUB_DIR}/Image.cpp 
)

INSTALL(FILES ${DS_HDRS} DESTINATION include/tt/${DS_SUB_DIR})

###############################################################################
# specific to namespace input
###############################################################################
IF (WIN32)
	# find CMU driver libraries
	FIND_PACKAGE(CMU1394)

	INCLUDE_DIRECTORIES(
		${CMU1394_INCLUDES}
	)
ELSE (WIN32)
	IF (APPLE)
		# no apple specific camera support yet
	ELSE (APPLE)
		# Assume Linux and find libdc1394
		FIND_PACKAGE(libdc1394)
		FIND_PACKAGE(libraw1394)
		
		INCLUDE_DIRECTORIES(
			${LIBDC1394_INCLUDE_DIR}
		)
	ENDIF (APPLE)
ENDIF (WIN32)

SET(INPUT_HDRS
	${INPUT_SUB_DIR}/InputDevice.h	
	${INPUT_SUB_DIR}/ImageDevice.h	
	${INPUT_SUB_DIR}/FirewireCamera.h
	${INPUT_SUB_DIR}/LinuxDC1394Camera.h
	${INPUT_SUB_DIR}/WindowsCMU1394Camera.h
	${INPUT_SUB_DIR}/MoviePlayer.h
	${INPUT_SUB_DIR}/OpenCVCamera.h
)

SET(INPUT_SRCS
	${INPUT_SUB_DIR}/InputDevice.cpp	
	${INPUT_SUB_DIR}/ImageDevice.cpp	
	${INPUT_SUB_DIR}/FirewireCamera.cpp
	${INPUT_SUB_DIR}/LinuxDC1394Camera.cpp
	${INPUT_SUB_DIR}/WindowsCMU1394Camera.cpp
	${INPUT_SUB_DIR}/MoviePlayer.cpp	
	${INPUT_SUB_DIR}/OpenCVCamera.cpp
)

INSTALL(FILES ${INPUT_HDRS} DESTINATION include/tt/${INPUT_SUB_DIR})

################################################################################
## specific to namespace output
################################################################################

SET(OUTPUT_HDRS
	${OUTPUT_SUB_DIR}/OutputDevice.h
	${OUTPUT_SUB_DIR}/MovieRecorder.h
)

SET(OUTPUT_SRCS
	${OUTPUT_SUB_DIR}/OutputDevice.cpp
	${OUTPUT_SUB_DIR}/MovieRecorder.cpp
)

INSTALL(FILES ${OUTPUT_HDRS} DESTINATION include/tt/${OUTPUT_SUB_DIR})

################################################################################
## tt library
################################################################################

SET(HDRS
	${DS_HDRS}
	${INPUT_HDRS}
	${OUTPUT_HDRS}
)

SET(SRCS
	${DS_SRCS}
	${INPUT_SRCS}
	${OUTPUT_SRCS}
)	

IF (WIN32)
	ADD_LIBRARY(tt STATIC ${HDRS} ${SRCS})
	TARGET_LINK_LIBRARIES(tt 
		${OPENCV_LIBRARIES} 
		${CMU1394_LIBRARIES} 
	)
ELSE (WIN32)
	ADD_LIBRARY(tt SHARED ${HDRS} ${SRCS})
	TARGET_LINK_LIBRARIES(tt 
		${OPENCV_LIBRARIES} 
		${LIBDC1394_LIBRARY} 
		${LIBRAW1394_LIBRARY} 
	)
ENDIF (WIN32)

SET_TARGET_PROPERTIES(tt PROPERTIES VERSION ${VERSION})

INSTALL(TARGETS tt
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib
)
