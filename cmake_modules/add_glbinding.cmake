include(ExternalProject)

ExternalProject_Add(glbinding
    PREFIX glbinding
    GIT_REPOSITORY https://github.com/cginternals/glbinding.git
    GIT_TAG v3.1
    CMAKE_ARGS  -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}/glbinding
                -DOPTION_BUILD_EXAMPLES=OFF
                -DBUILD_SHARED_LIBS=OFF
)


if (WIN32)
    set (GLBINDING_LIB ${EXTERNAL_INSTALL_LOCATION}/glbinding/lib/glbindingd${CMAKE_STATIC_LIBRARY_SUFFIX})
	set (GLBINDING_AUX_LIB ${EXTERNAL_INSTALL_LOCATION}/glbinding/lib/glbinding-auxd${CMAKE_STATIC_LIBRARY_SUFFIX})  
	set (GLBINDING_INCLUDE ${EXTERNAL_INSTALL_LOCATION}/glbinding/include)
else()
    set (GLBINDING_LIB ${EXTERNAL_INSTALL_LOCATION}/glbinding/lib/libglbinding${CMAKE_STATIC_LIBRARY_SUFFIX})
	set (GLBINDING_AUX_LIB ${EXTERNAL_INSTALL_LOCATION}/glbinding/lib/libglbinding-aux${CMAKE_STATIC_LIBRARY_SUFFIX})
	set (GLBINDING_INCLUDE ${EXTERNAL_INSTALL_LOCATION}/glbinding/include)
endif ()
