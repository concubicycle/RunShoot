# Note that headers are optional, and do not affect add_library, but they will not
# show up in IDEs unless they are listed in add_library.

set(CORE_SOURCE_LIST
${PROJECT_SOURCE_DIR}/src/frame_limiter.cpp
${PROJECT_SOURCE_DIR}/src/frame_timer.cpp
${PROJECT_SOURCE_DIR}/src/framerate.cpp
${PROJECT_SOURCE_DIR}/src/startup_config.cpp
${PROJECT_SOURCE_DIR}/src/startup.cpp
)

set(RENDERER_SOURCE_LIST
${PROJECT_SOURCE_DIR}/src/renderer.cpp
)

set(ASSET_SOURCE_LIST
${PROJECT_SOURCE_DIR}/src/mesh.cpp
${PROJECT_SOURCE_DIR}/src/basic_mesh_reader.cpp

)

set(PLATFORM_SOURCE_LIST
${PROJECT_SOURCE_DIR}/src/sleep.cpp
)

set(UTIL_SOURCE_LIST
)

set(EVENTS_SOURCE_LIST
${PROJECT_SOURCE_DIR}/src/event_dispatcher.cpp
)

set(MEMORY_SOURCE_LIST
${PROJECT_SOURCE_DIR}/src/typed_pool_allocator.cpp
${PROJECT_SOURCE_DIR}/src/pool_allocator.cpp
)

set(ECS_SOURCE_LIST
${PROJECT_SOURCE_DIR}/src/game_obj_component_pools.cpp
${PROJECT_SOURCE_DIR}/src/behavior_component.cpp
${PROJECT_SOURCE_DIR}/src/component_store.cpp
${PROJECT_SOURCE_DIR}/src/game_object.cpp
${PROJECT_SOURCE_DIR}/src/archetype_store.cpp

)

set(HEADER_LIST
${PROJECT_SOURCE_DIR}/include/ecs/game_obj_component_pools.hpp
${PROJECT_SOURCE_DIR}/include/ecs/behavior_component.hpp
${PROJECT_SOURCE_DIR}/include/ecs/component_store.hpp
${PROJECT_SOURCE_DIR}/include/ecs/archetype_store.hpp
${PROJECT_SOURCE_DIR}/include/ecs/component.hpp
${PROJECT_SOURCE_DIR}/include/ecs/game_object.hpp
${PROJECT_SOURCE_DIR}/include/ecs/game_object_factory.hpp
${PROJECT_SOURCE_DIR}/include/ecs/ecs_types.hpp
${PROJECT_SOURCE_DIR}/include/asset/textured_mesh.hpp
${PROJECT_SOURCE_DIR}/include/asset/mesh_read_error.hpp
${PROJECT_SOURCE_DIR}/include/memory/typed_pool_allocator.hpp
${PROJECT_SOURCE_DIR}/include/memory/pool_allocator.hpp
${PROJECT_SOURCE_DIR}/include/events/event_dispatcher.hpp
${PROJECT_SOURCE_DIR}/include/asset/mesh.hpp
${PROJECT_SOURCE_DIR}/include/asset/basic_mesh_reader.hpp
${PROJECT_SOURCE_DIR}/include/renderer/renderer.hpp
${PROJECT_SOURCE_DIR}/include/platform/sleep.hpp
${PROJECT_SOURCE_DIR}/include/core/frame_limiter.hpp
${PROJECT_SOURCE_DIR}/include/core/frame_timer.hpp
${PROJECT_SOURCE_DIR}/include/core/framerate.hpp
${PROJECT_SOURCE_DIR}/include/core/startup_config.hpp
${PROJECT_SOURCE_DIR}/include/core/startup.hpp
${PROJECT_SOURCE_DIR}/include/util/running_average.hpp
)

# Make an automatic library - will be static or dynamic based on user setting
add_library(${CORELIB_NAME} STATIC ${CORE_SOURCE_LIST} ${HEADER_LIST})
add_library(${PLATFORMLIB_NAME} STATIC ${PLATFORM_SOURCE_LIST} ${HEADER_LIST})
add_library(${RENDERER} STATIC ${RENDERER_SOURCE_LIST} ${HEADER_LIST})
add_library(${ASSETLIB_NAME} STATIC ${ASSET_SOURCE_LIST} ${HEADER_LIST})
add_library(${MEM_LIB} STATIC ${MEMORY_SOURCE_LIST} ${HEADER_LIST})


# We need this directory, and users of our library will need it too
target_include_directories(${CORELIB_NAME} PUBLIC ../include)
target_include_directories(${PLATFORMLIB_NAME} PUBLIC ../include)
target_include_directories(${RENDERER} PUBLIC ../include)
target_include_directories(${ASSETLIB_NAME} PUBLIC ../include)
target_include_directories(${MEM_LIB} PUBLIC ../include)



# All users of this library will need at least C++11
target_compile_features(${CORELIB_NAME} PUBLIC cxx_std_11)
target_compile_features(${PLATFORMLIB_NAME} PUBLIC cxx_std_11)
target_compile_features(${RENDERER} PUBLIC cxx_std_11)
target_compile_features(${ASSETLIB_NAME} PUBLIC cxx_std_11)
target_compile_features(${MEM_LIB} PUBLIC cxx_std_11)

