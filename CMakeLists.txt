cmake_minimum_required (VERSION 3.1)
project(RunShoot C CXX)

set (CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake_modules")
set (CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}/install/")
set (CMAKE_BINARY_DIR "${PROJECT_SOURCE_DIR}/build")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

set (TARGET_NAME "RunShoot")
set (CORELIB_NAME "core")
set (PLATFORMLIB_NAME "platform")
set (GLAD "glad")
set (RENDERER "renderer")
set (GLAD_INCLUDE "GLAD/include")
set (ASSETLIB_NAME "asset")
set (OGLLIB_NAME "ogllib")
set (THIRD_PARTY "thirdparty")
set (GLM "glm")

# change for release
set (CMAKE_BUILD_TYPE Debug)
#

set (EXTERNAL_INSTALL_LOCATION "${PROJECT_SOURCE_DIR}/external")
set (OpenGL_GL_PREFERENCE "GLVND")
set (RunShoot_VERSION_MAJOR 0)
set (RunShoot_VERSION_MINOR 1)

add_executable(${TARGET_NAME} apps/main.cpp)

include (FindOpenGL)
include (add_glfw)
include (add_nlohmann_json)
include (add_warnings)
include (add_glad)
include (add_glm)
include (add_ogllib)

include_directories(${TARGET_NAME} 
    ${GLFW_INCLUDE} 
    ${OPENGL_INCLUDE_DIRS} 
    ${JSON_INCLUDE}
    ${GLAD_INCLUDE}
    ${GLM_INCLUDE}
    ${OGLLIB_INCLUDE})
	
add_subdirectory(src)

add_dependencies(${CORELIB_NAME} ${PLATFORMLIB_NAME} nlohmann_json)
add_dependencies(${OGLLIB_NAME} ${GLAD} ${GLM})
add_dependencies(${RENDERER} ${GLAD} ${CORELIB_NAME} ${ASSETLIB_NAME} ${OGLLIB_NAME} ${GLM} glfw)
add_dependencies(${TARGET_NAME} ${CORELIB_NAME} glfw platform renderer)
add_dependencies(${ASSETLIB_NAME} nlohmann_json)

target_link_libraries(${TARGET_NAME} 
    ${GLFW_LIB}
    ${OPENGL_LIBRARIES}
    ${CORELIB_NAME}
    ${PLATFORMLIB_NAME}
    ${GLAD}
    ${ASSETLIB_NAME}
    ${RENDERER})


if (UNIX)
    target_link_libraries(${TARGET_NAME} X11)
endif (UNIX)

if(CMAKE_DL_LIBS)
    target_link_libraries(${TARGET_NAME} ${CMAKE_DL_LIBS})
endif()






