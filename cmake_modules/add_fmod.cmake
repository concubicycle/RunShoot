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


if (WIN32)
    set(FMOD_DIR ${PROJECT_SOURCE_DIR}/thirdparty/fmod/windows)
	
	set(FMOD_CORE_LIBS
        fmod_vc)

	set(FMOD_CORE_DEBUG_LIBS
        fmodL_vc)
		
	set(FMOD_STUDIO_LIBS
        fmodstudio_vc)

    set(FMOD_STUDIO_DEBUG_LIBS
        fmodstudioL_vc)
		
	set(FMOD_ARCH Win64)
	
	if (NOT ${CMAKE_BUILD_TYPE} MATCHES "Release")
		set (FMOD_DLL_SUFFIX L)
	ENDIF()
	
	add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${FMOD_DIR}/${FMOD_ARCH}/fmod${FMOD_DLL_SUFFIX}.dll
        $<TARGET_FILE_DIR:${TARGET_NAME}>)                 
		
	add_custom_command(TARGET ${TARGET_NAME} POST_BUILD        
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  
        ${FMOD_DIR}/${FMOD_ARCH}/fmodstudio${FMOD_DLL_SUFFIX}.dll	
        $<TARGET_FILE_DIR:${TARGET_NAME}>)                 
	
elseif (UNIX)
    set(FMOD_DIR ${PROJECT_SOURCE_DIR}/thirdparty/fmod/linux)

    set(FMOD_CORE_LIBS
        fmod)

    set(FMOD_CORE_DEBUG_LIBS
        fmodL)

    set(FMOD_STUDIO_LIBS
        fmodstudio)

    set(FMOD_STUDIO_DEBUG_LIBS
        fmodstudioL)

    set(FMOD_FSBANK_LIB
        fsbank
        fsbvorbis)

    set(FMOD_FSBANK_DEBUG_LIBS
        fsbankL)
		
	set(FMOD_ARCH x86_64)

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
		  ${FMOD_DIR}/FMOD
          )

FIND_LIBRARY(FMOD_LIBRARY
            NAMES ${FMOD_CORE_LIBS}
            HINTS
            ${FMOD_DIR}
            $ENV{FMOD_DIR}
            $ENV{FMODDIR}
            PATH_SUFFIXES api/core/lib
            PATHS
            ${FMOD_DIR}/api/core/lib/${FMOD_ARCH}/
			${FMOD_DIR}/${FMOD_ARCH}/
             )

FIND_LIBRARY(FMOD_LIBRARY_DEBUG
            NAMES ${FMOD_CORE_DEBUG_LIBS}
            HINTS
            $ENV{FMOD_DIR}
            $ENV{FMODDIR}
            PATH_SUFFIXES api/lib
            PATHS
            ${FMOD_DIR}/api/core/lib
			${FMOD_DIR}/${FMOD_ARCH}/)


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