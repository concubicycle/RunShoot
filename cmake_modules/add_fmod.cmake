# - Try to find FMOD EX
#
#  This module defines the following variables
#
#  FMOD_FOUND - Was FMOD found
#  FMOD_INCLUDE_DIRS - the FMOD include directories
#  FMOD_LIBRARIES - Link to this, by default it includes
#                     all FMOD components
#
#  This module accepts the following variables
#
#  FMOD_DIR - Can be set to FMOD install path or Windows build path
#

# Copyright (c) 2013, Johannes Deselaers <deselaers (dot) johannes (at) gmail (dot) com>
#
# Feel free to share and edit
# based on https://github.com/philippotto/Tron3D

set(FMOD_ARCH x86_64)

if (WIN32)
    set(FMOD_DIR ${PROJECT_SOURCE_DIR}/thirdparty/fmod/windows)
    #
    # TODO
    #
elseif (UNIX)
    set(FMOD_DIR ${PROJECT_SOURCE_DIR}/thirdparty/fmod/linux)

    set(FMOD_CORE_LIBS
        libfmod)

    set(FMOD_CORE_DEBUG_LIBS
        libfmodL)

    set(FMOD_STUDIO_LIBS
        libfmodstudio)

    set(FMOD_STUDIO_DEBUG_LIBS
        libfmodstudioL)

    set(FMOD_FSBANK_LIB
        libfsbank
        libfsbvorbis)

    set(FMOD_FSBANK_DEBUG_LIBS
        libfsbankL)
endif ()

if (!FMOD_DIR)
    set(FMOD_DIR $ENV{FMOD_DIR})
endif (!FMOD_DIR)


# Find FMOD Ex first
FIND_PATH(FMOD_INCLUDE_DIR fmod.h
          HINTS
          ${FMOD_DIR}
          $ENV{FMOD_DIR}
          $ENV{FMODDIR}
          PATH_SUFFIXES /api/core/inc/
          PATHS
          ${FMOD_DIR}/api/core/inc
          )

FIND_LIBRARY(FMOD_LIBRARY
             NAMES fmod
             HINTS
             ${FMOD_DIR}
             $ENV{FMOD_DIR}
             $ENV{FMODDIR}
             PATH_SUFFIXES api/core/lib
             PATHS
             ${FMOD_DIR}/api/core/lib/${FMOD_ARCH}/
             )

FIND_LIBRARY(FMOD_LIBRARY_DEBUG
             NAMES ${FMOD_CORE_DEBUG_LIBS}
             HINTS
             $ENV{FMOD_DIR}
             $ENV{FMODDIR}
             PATH_SUFFIXES api/lib
             PATHS
             ${FMOD_DIR}/api/core/lib
             )


# handle the QUIETLY and REQUIRED arguments and set FMOD_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(FMOD DEFAULT_MSG
                                  FMOD_LIBRARY
                                  FMOD_INCLUDE_DIR)

IF (FMOD_FOUND)
    MESSAGE(STATUS "FMOD found!")
ELSE (FMOD_FOUND)
    MESSAGE(ERROR "FMOD not (entirely) found!")
ENDIF (FMOD_FOUND)

set(FMOD_INCLUDE_DIRS
    ${FMOD_INCLUDE_DIR})

macro(_FMOD_APPEND_LIBRARIES _list _release)
    set(_debug ${_release}_DEBUG)
    if (${_debug})
        set(${_list} ${${_list}} optimized ${${_release}} debug ${${_debug}})
    else ()
        set(${_list} ${${_list}} ${${_release}})
    endif ()
endmacro()

if (FMOD_FOUND)
    _FMOD_APPEND_LIBRARIES(FMOD_LIBRARIES FMOD_LIBRARY)
endif ()