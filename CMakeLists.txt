cmake_minimum_required (VERSION 3.1)
project(RunShoot C CXX)

set (CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake_modules")
set (CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}/install/")
set (CMAKE_BINARY_DIR "${PROJECT_SOURCE_DIR}/build")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

#TODO: Wrap in environment
#set(CMAKE_BUILD_TYPE Debug)

set (EXTERNAL_INSTALL_LOCATION "${PROJECT_SOURCE_DIR}/external")
set (OpenGL_GL_PREFERENCE "GLVND")
set (RunShoot_VERSION_MAJOR 0)
set (RunShoot_VERSION_MINOR 1)

add_executable(RunShoot apps/main.cpp)


include(FindOpenGL)
include(add_glfw)
include(add_nlohmann_json)
add_dependencies(RunShoot glfw nlohmann_json)

include_directories(RunShoot 
    ${GLFW_INCLUDE} 
    ${OPENGL_INCLUDE_DIRS} 
    ${JSON_INCLUDE})

target_link_libraries(RunShoot ${GLFW_LIB} ${OPENGL_LIBRARIES} core)

if (UNIX)
    target_link_libraries(RunShoot X11)
endif (UNIX)

if(CMAKE_DL_LIBS)
    target_link_libraries(RunShoot ${CMAKE_DL_LIBS})
endif()


add_subdirectory(src)
target_link_libraries (RunShoot core)



