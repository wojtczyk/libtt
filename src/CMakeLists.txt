PROJECT(SRC)

# require calling cmake from the top level directory
#IF(${CMAKE_PROJECT_NAME} STREQUAL "Project")
        #MESSAGE(FATAL_ERROR "Please use the toplevel CMakeLists.txt, exiting...")
#ENDIF(${CMAKE_PROJECT_NAME} STREQUAL "Project")

# search this modules directory additionally/first
SET(CMAKE_MODULE_PATH "${LIBTRAX_SOURCE_DIR}/Modules")

# include basic cmake checks and definitions
#INCLUDE(LibraryCommonConfig)


# configure a config.h
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)

# configure a Doxygen config file
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)


# tracking library subfolder
ADD_SUBDIRECTORY(tt)
# testing apps/experimental apps/regression tests subfolder
# ADD_SUBDIRECTORY(test)
# demo apps
# ADD_SUBDIRECTORY(demos)

