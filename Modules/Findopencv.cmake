###############################################################################
# File:    Findopencv.cmake
# Author:  Martin Wojtczyk <wojtczyk@in.tum.de>
# Purpose: Find the OpenCV library on Windows/Linux/Mac systems
###############################################################################
# On success this script defines
# OPENCV_INCLUDES              location of cv.h cvaux.h cxcore.h highgui.h
# OPENCV_LIBRARIES             full path to cv cvaux cxcore highgui libraries
#
# OPENCV_CV_INCLUDE_DIR        location of cv.h
# OPENCV_CV_LIBRARY            location of cv library
# OPENCV_CXCORE_INCLUDE_DIR    location of cxcore.h
# OPENCV_CXCORE_LIBRARY        location of cxcore library
# OPENCV_CVAUX_INCLUDE_DIR     location of cvaux.h
# OPENCV_CVAUX_LIBRARY         location of cvaux library
# OPENCV_HIGHGUI_INCLUDE_DIR   location of highgui.h
# OPENCV_HIGHGUI_LIBRARY       location of highgui library

FIND_PATH(OPENCV_CV_INCLUDE_DIR cv.h
	${CMAKE_INCLUDE_PATH}
	$ENV{HOME}/include/opencv
	/usr/local/include/opencv
	/usr/include/opencv
	/opt/local/include/opencv
	$ENV{OPENCVDIR}/cv/include
	$ENV{ProgramFiles}/OpenCV/cv/include
)

FIND_PATH(OPENCV_CVAUX_INCLUDE_DIR cvaux.h
	${CMAKE_INCLUDE_PATH}
	$ENV{HOME}/include/opencv
	/usr/local/include/opencv
	/usr/include/opencv
	/opt/local/include/opencv
	$ENV{OPENCVDIR}/cvaux/include
	$ENV{ProgramFiles}/OpenCV/cvaux/include
)

FIND_PATH(OPENCV_CXCORE_INCLUDE_DIR cxcore.h
	${CMAKE_INCLUDE_PATH}
	$ENV{HOME}/include/opencv
	/usr/local/include/opencv
	/usr/include/opencv
	/opt/local/include/opencv
	$ENV{OPENCVDIR}/cxcore/include
	$ENV{ProgramFiles}/OpenCV/cxcore/include
)

FIND_PATH(OPENCV_HIGHGUI_INCLUDE_DIR highgui.h
	${CMAKE_INCLUDE_PATH}
	$ENV{HOME}/include/opencv
	/usr/local/include/opencv
	/usr/include/opencv
	/opt/local/include/opencv
	$ENV{OPENCVDIR}/otherlibs/highgui
	$ENV{ProgramFiles}/OpenCV/otherlibs/highgui
)

FIND_LIBRARY(OPENCV_CV_LIBRARY
	NAMES
	cv
	cv0.9.7
	PATHS
	${CMAKE_LIBRARY_PATH}
	$ENV{HOME}/lib
	/usr/local/opencv/lib
	/usr/local/lib
	/usr/lib
	/opt/local/lib
	$ENV{OPENCVDIR}/lib
	$ENV{ProgramFiles}/OpenCV/lib
)

IF (OPENCV_CV_LIBRARY)
	SET(OPENCV_CV_FOUND TRUE)
ENDIF (OPENCV_CV_LIBRARY)

FIND_LIBRARY(OPENCV_CVAUX_LIBRARY
	NAMES
	cvaux
	cvaux0.9.7
	PATHS
	${CMAKE_LIBRARY_PATH}
	$ENV{HOME}/lib
	/usr/local/opencv/lib
	/usr/local/lib
	/usr/lib
	/opt/local/lib
	$ENV{OPENCVDIR}/lib
	$ENV{ProgramFiles}/OpenCV/lib
)

IF (OPENCV_CVAUX_LIBRARY)
	SET(OPENCV_CVAUX_FOUND TRUE)
ENDIF (OPENCV_CVAUX_LIBRARY)

FIND_LIBRARY(OPENCV_CXCORE_LIBRARY
	NAMES
	cxcore
	cxcore0.9.7
	PATHS
	${CMAKE_LIBRARY_PATH}
	$ENV{HOME}/lib
	/usr/local/opencv/lib
	/usr/local/lib
	/usr/lib
	/opt/local/lib
	$ENV{OPENCVDIR}/lib
	$ENV{ProgramFiles}/OpenCV/lib
)

IF (OPENCV_CXCORE_LIBRARY)
	SET(OPENCV_CXCORE_FOUND TRUE)
ENDIF (OPENCV_CXCORE_LIBRARY)

FIND_LIBRARY(OPENCV_HIGHGUI_LIBRARY
	NAMES
	highgui
	highgui0.9.7
	PATHS
	${CMAKE_LIBRARY_PATH}
	$ENV{HOME}/lib
	/usr/local/opencv/lib
	/usr/local/lib
	/usr/lib
	/opt/local/lib
	$ENV{OPENCVDIR}/lib
	$ENV{ProgramFiles}/OpenCV/lib
)

IF (OPENCV_HIGHGUI_LIBRARY)
	SET(OPENCV_HIGHGUI_FOUND TRUE)
ENDIF (OPENCV_HIGHGUI_LIBRARY)

SET(
	OPENCV_INCLUDES
	${OPENCV_CV_INCLUDE_DIR}
	${OPENCV_CVAUX_INCLUDE_DIR}
	${OPENCV_CXCORE_INCLUDE_DIR}
	${OPENCV_HIGHGUI_INCLUDE_DIR}
)

SET(
	OPENCV_LIBRARIES
	${OPENCV_CV_LIBRARY}
	${OPENCV_CVAUX_LIBRARY}
	${OPENCV_CXCORE_LIBRARY}
	${OPENCV_HIGHGUI_LIBRARY}
)

IF (OPENCV_CV_INCLUDE_DIR AND OPENCV_CVAUX_INCLUDE_DIR AND OPENCV_CV_LIBRARY AND OPENCV_CVAUX_LIBRARY)
	SET(OPENCV_FOUND TRUE)
ENDIF (OPENCV_CV_INCLUDE_DIR AND OPENCV_CVAUX_INCLUDE_DIR AND OPENCV_CV_LIBRARY AND OPENCV_CVAUX_LIBRARY)

MARK_AS_ADVANCED(
	OPENCV_FOUND
	OPENCV_INCLUDES
	OPENCV_LIBRARIES
	OPENCV_CV_INCLUDE_DIR
	OPENCV_CV_LIBRARY
	OPENCV_CXCORE_INCLUDE_DIR
	OPENCV_CXCORE_LIBRARY
	OPENCV_CVAUX_INCLUDE_DIR
	OPENCV_CVAUX_LIBRARY
	OPENCV_HIGHGUI_INCLUDE_DIR
	OPENCV_HIGHGUI_LIBRARY
) 
