cmake_minimum_required(VERSION 3.1)
project(RunShoot C CXX)

option(BUILD_OFFLINE "Build offline" ON)



set(RunShoot_VERSION_MAJOR 0)
set(RunShoot_VERSION_MINOR 1)

##### change for release #####
set(CMAKE_BUILD_TYPE Debug)
##############################


if (${BUILD_OFFLINE})
    set_property(DIRECTORY ${PROJECT_SOURCE_DIR} PROPERTY EP_UPDATE_DISCONNECTED ON)
endif()

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake_modules")
set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}/install/")
set(CMAKE_BINARY_DIR "${PROJECT_SOURCE_DIR}/build")

if (UNIX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread")
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
set(PHYSICS_LIB "physics")
set(PHYSICS_MODELS_LIB "physics_models")
set(SOUND_LIB "sound")


# WARNING: Doesn't work when build directory isn't /build for some reason (with CLion)
file(COPY ${ASSETS_DIR} DESTINATION ${CMAKE_BINARY_DIR})


add_executable(${TARGET_NAME}
               apps/main.cpp
               apps/runshoot.hpp
               apps/milestone1.hpp
               apps/milestone1.cpp
               apps/runshoot.cpp
               apps/freefly_controller.hpp
               apps/components/freefly_controller_component.cpp
               apps/components/freefly_controller_component.hpp
               apps/components/drone_controller_component.cpp
               apps/components/drone_controller_component.hpp
               apps/drone_controller.hpp
               apps/player_controller.cpp
               apps/player_controller.hpp
               apps/components/player_controller_component.hpp
               apps/components/player_controller_component.cpp
               apps/components/runshoot_component.hpp
               apps/components/segment_component.hpp
               apps/components/segment_component.cpp
               apps/components/turn_trigger_component.cpp
               apps/components/turn_trigger_component.hpp
               apps/components/turn_direction.hpp
               apps/runshoot_event.hpp
               apps/segment_spawner.hpp
               apps/components/segment_spawner_component.cpp
               apps/components/segment_spawner_component.hpp
               apps/segment_spawner.cpp
               apps/components/drone_spawner_component.cpp
               apps/components/drone_spawner_component.hpp
               apps/drone_spawner.cpp
               apps/drone_spawner.hpp
               apps/drone_controller.cpp
               apps/components/music_player_component.cpp
               apps/components/music_player_component.hpp
               apps/music_player.cpp
               apps/music_player.hpp)

add_executable(generate_colliders apps/generate_colliders.cpp)

add_executable(tests tests/main.cpp)


include(add_glfw)
include(add_nlohmann_json)
#include (add_warnings) # will break windows build due to warnings in thirdparty libs
include(add_glm)
include(add_spdlog)
include(add_expected)
include(add_assimp) # causes weird build issue on windows - can't close zlib.lib
# include(add_tinygltf)
include(add_glbinding)
include(add_fmod)


# Prevent GLFW from including gl.h, as this will break glbinding
target_compile_definitions(${TARGET_NAME}
                           PRIVATE
                           ${DEFAULT_COMPILE_DEFINITIONS}
                           GLFW_INCLUDE_NONE)

target_include_directories(${TARGET_NAME} PRIVATE
                           ${GLFW_INCLUDE}
                           ${JSON_INCLUDE}
                           ${GLM_INCLUDE}
                           ${SPDLOG_INCLUDE}
                           ${EXPECTED_INCLUDE}
                           ${GLBINDING_INCLUDE})



include_directories(${TARGET_NAME}
                    ${JSON_INCLUDE}
                    ${GLM_INCLUDE}
                    ${GLBINDING_INCLUDE}
                    ${EXPECTED_INCLUDE}
                    ${SPDLOG_INCLUDE}
                    ${ASSIMP_INCLUDE}
                    thirdparty/opengllib/include
                    #${TINYGLTF_INCLUDE}
                    ${FMOD_INCLUDE_DIRS}
                    )


add_subdirectory(src)
add_subdirectory(thirdparty/opengllib)

add_dependencies(${CORELIB_NAME}
                 expected
                 ${PLATFORMLIB_NAME}
                 ${ASSETLIB_NAME}
                 nlohmann_json
                 glfw
                 ${GLM})

add_dependencies(ogllib
                 glbinding
                 ${GLM}
                 ${SPDLOG})

add_dependencies(${ECS_LIB}
                 ogllib
				 nlohmann_json
                 scene
                 ${MEM_LIB}
                 ${GLM}
                 ${PHYSICS_MODELS_LIB})

add_dependencies(${RENDERER}
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
                 ${ASSETLIB_NAME}
                 ${CORELIB_NAME}
                 ${PLATFORMLIB_NAME}
                 ${RENDERER}
                 ${PHYSICS_LIB}
                 ${SOUND_LIB}
                 scene
                 glfw
                 glbinding
                 assimp
                 expected)

add_dependencies(${ASSETLIB_NAME}
                 #tinygltf
                 scene
                 ${ECS_LIB}
                 expected
                 ${OGLLIB_NAME}
                 nlohmann_json
                 glbinding
                 assimp
                 )

add_dependencies(generate_colliders
                 expected
                 ${OGLLIB_NAME}
                 nlohmann_json
                 assimp
                 ${ASSETLIB_NAME}
                 ogllib)

add_dependencies(${PHYSICS_LIB}
                 ${ECS_LIB}
                 ${PHYSICS_MODELS_LIB}
				 nlohmann_json)
				 
add_dependencies(${PHYSICS_MODELS_LIB}
				glm)

add_dependencies(${SOUND_LIB}
                 ${ECS_LIB}
                 glm)



target_link_libraries(${TARGET_NAME} PRIVATE
                      ${GLFW_LIB}
                      ${CORELIB_NAME}
                      ${PLATFORMLIB_NAME}
                      ${RENDERER}
                      ${OGLLIB_NAME}
                      ${ASSETLIB_NAME}
                      ${ECS_LIB}
                      ${MEM_LIB}
                      ${ASSIMP_LIB}
                      ${GLBINDING_AUX_LIB}
                      ${GLBINDING_LIB}
                      ${PHYSICS_LIB}
                      ${PHYSICS_MODELS_LIB}
                      ${FMOD_LIBRARIES}
                      ${SOUND_LIB})


target_link_libraries(generate_colliders PRIVATE
                      ${OGLLIB_NAME}
                      ${ASSETLIB_NAME}
                      ${ASSIMP_LIB})

target_link_libraries(tests
                      ${MEM_LIB})


if (UNIX)
    target_link_libraries(${TARGET_NAME} PRIVATE X11)
endif (UNIX)

if (CMAKE_DL_LIBS)
    target_link_libraries(${TARGET_NAME} PRIVATE ${CMAKE_DL_LIBS})
endif ()


set (CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/install)
install(TARGETS ${TARGET_NAME} DESTINATION bin)