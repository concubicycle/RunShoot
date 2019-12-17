include(ExternalProject)

ExternalProject_Add(glbinding
    PREFIX glbinding
    GIT_REPOSITORY https://github.com/cginternals/glbinding.git
    GIT_TAG v3.1.0
	GIT_SHALLOW TRUE
    CMAKE_ARGS  -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}/glbinding
				-DCMAKE_RUNTIME_OUTPUT_DIRECTORY=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
                -DOPTION_BUILD_EXAMPLES=OFF
                -DBUILD_SHARED_LIBS=OFF
)

if (NOT ${CMAKE_BUILD_TYPE} MATCHES "Release")
	set (WIN_LIB_SUFFIX "d")
endif()


if (WIN32)
    set (GLBINDING_LIB ${EXTERNAL_INSTALL_LOCATION}/glbinding/lib/glbinding${WIN_LIB_SUFFIX}${CMAKE_STATIC_LIBRARY_SUFFIX})
	set (GLBINDING_AUX_LIB ${EXTERNAL_INSTALL_LOCATION}/glbinding/lib/glbinding-aux${WIN_LIB_SUFFIX}${CMAKE_STATIC_LIBRARY_SUFFIX})
	set (GLBINDING_INCLUDE ${EXTERNAL_INSTALL_LOCATION}/glbinding/include)
else()
    set (GLBINDING_LIB ${EXTERNAL_INSTALL_LOCATION}/glbinding/lib/libglbinding${CMAKE_STATIC_LIBRARY_SUFFIX})
	set (GLBINDING_AUX_LIB ${EXTERNAL_INSTALL_LOCATION}/glbinding/lib/libglbinding-aux${CMAKE_STATIC_LIBRARY_SUFFIX})
	set (GLBINDING_INCLUDE ${EXTERNAL_INSTALL_LOCATION}/glbinding/include)
endif ()



if(WIN32 AND MSVC)
add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${EXTERNAL_INSTALL_LOCATION}/glbinding/glbinding.dll
        $<TARGET_FILE_DIR:${TARGET_NAME}>)                 
		
		
add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${EXTERNAL_INSTALL_LOCATION}/glbinding/glbinding-aux.dll
        $<TARGET_FILE_DIR:${TARGET_NAME}>)                 
endif()
