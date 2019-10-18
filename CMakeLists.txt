cmake_minimum_required(VERSION 3.1)
project(RunShoot C CXX)

set(RunShoot_VERSION_MAJOR 0)
set(RunShoot_VERSION_MINOR 1)

##### change for release #####
set(CMAKE_BUILD_TYPE Debug)
##############################


set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake_modules")
set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}/install/")
set(CMAKE_BINARY_DIR "${PROJECT_SOURCE_DIR}/build")

if (UNIX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread")
endif (UNIX)

set(EXTERNAL_INSTALL_LOCATION "${PROJECT_SOURCE_DIR}/external")
set(OpenGL_GL_PREFERENCE "GLVND")
set(ASSETS_DIR ${PROJECT_SOURCE_DIR}/assets)

set(TARGET_NAME "RunShoot")
set(CORELIB_NAME "core")
set(PLATFORMLIB_NAME "platform")
set(RENDERER "renderer")
set(ASSETLIB_NAME "asset")
set(OGLLIB_NAME "ogllib")
set(THIRD_PARTY "thirdparty")
set(GLM "glm")
set(SPDLOG "spdlog")
set(MEM_LIB "memory")
set(ECS_LIB "ecs")

# WARNING: Doesn't work when build directory isn't /build for some reason (with CLion)
file(COPY ${ASSETS_DIR} DESTINATION ${CMAKE_BINARY_DIR})


add_executable(${TARGET_NAME} apps/main.cpp apps/runshoot.hpp apps/milestone1.hpp apps/milestone1.cpp)
add_executable(tests tests/main.cpp)


include(add_glfw)
include(add_nlohmann_json)
#include (add_warnings) # will break windows build due to warnings in thirdparty libs
include(add_glm)
include(add_spdlog)
include(add_expected)
# include(add_assimp) # causes weird build issue on windows - can't close zlib.lib
include(add_glbinding)


# Prevent GLFW from including gl.h, as this will break glbinding
target_compile_definitions(${TARGET_NAME}
                           PRIVATE
                           ${DEFAULT_COMPILE_DEFINITIONS}
                           GLFW_INCLUDE_NONE)


target_include_directories(${TARGET_NAME} PRIVATE
                           ${GLFW_INCLUDE}
                           ${JSON_INCLUDE}
                           ${GLM_INCLUDE}
                           ${OGLLIB_INCLUDE}
                           ${SPDLOG_INCLUDE}
                           ${EXPECTED_INCLUDE}
                           #${ASSIMP_INCLUDE}
                           ${GLBINDING_INCLUDE})

include_directories(${TARGET_NAME}
                    ${JSON_INCLUDE}
                    ${GLM_INCLUDE}
                    ${GLBINDING_INCLUDE}
                    ${OGLLIB_INCLUDE}
                    ${EXPECTED_INCLUDE}
                    ${SPDLOG_INCLUDE})


add_subdirectory(src)
add_subdirectory(thirdparty/opengllib)

add_dependencies(${CORELIB_NAME}
                 expected
                 ${PLATFORMLIB_NAME}
                 nlohmann_json
                 glfw)

add_dependencies(ogllib
                 glbinding
                 ${GLM}
                 ${SPDLOG})

add_dependencies(${ECS_LIB}
                 ogllib
				 nlohmann_json
                 ${MEM_LIB}
                 ${GLM})

add_dependencies(${RENDERER}
                 ${ECS_LIB}
                 glbinding
                 expected
                 glfw
                 ${ECS_LIB}
                 ${CORELIB_NAME}
                 ${ASSETLIB_NAME}
                 ${OGLLIB_NAME}
                 ${GLM}
                 ${SPDLOG})

add_dependencies(${TARGET_NAME}
                 glbinding
                 #assimp
                 expected
                 glfw
                 ${ASSETLIB_NAME}
                 ${CORELIB_NAME}
                 ${PLATFORMLIB_NAME}
                 ${RENDERER})

add_dependencies(${ASSETLIB_NAME}
                 ${ECS_LIB}
                 expected
                 ${OGLLIB_NAME}
                 nlohmann_json
                 glbinding
                 )

target_link_libraries(${TARGET_NAME} PRIVATE
                      ${GLFW_LIB}
                      ${CORELIB_NAME}
                      ${PLATFORMLIB_NAME}
                      ${GLBINDING_LIB}
                      ${RENDERER}
                      ${OGLLIB_NAME}
                      #${ASSIMP_LIB}
                      ${GLBINDING_AUX_LIB}
                      ${ECS_LIB}
                      ${ASSETLIB_NAME}
                      ${MEM_LIB})

target_link_libraries(tests
                      ${MEM_LIB})


if (UNIX)
    target_link_libraries(${TARGET_NAME} PRIVATE X11)
endif (UNIX)

if (CMAKE_DL_LIBS)
    target_link_libraries(${TARGET_NAME} PRIVATE ${CMAKE_DL_LIBS})
endif ()






