# Note that headers are optional, and do not affect add_library, but they will not
# show up in IDEs unless they are listed in add_library.

set(CORE_SOURCE_LIST
    ${PROJECT_SOURCE_DIR}/src/core/frame_limiter.cpp
    ${PROJECT_SOURCE_DIR}/src/core/frame_timer.cpp
    ${PROJECT_SOURCE_DIR}/src/core/framerate.cpp
    ${PROJECT_SOURCE_DIR}/src/core/startup_config.cpp
    ${PROJECT_SOURCE_DIR}/src/core/scene.cpp
    ${PROJECT_SOURCE_DIR}/src/core/input_manager.cpp
    ${PROJECT_SOURCE_DIR}/src/core/system_info.cpp)

set(CORE_HEADER_LIST
    ${PROJECT_SOURCE_DIR}/include/core/frame_limiter.hpp
    ${PROJECT_SOURCE_DIR}/include/core/frame_timer.hpp
    ${PROJECT_SOURCE_DIR}/include/core/framerate.hpp
    ${PROJECT_SOURCE_DIR}/include/core/startup_config.hpp
    ${PROJECT_SOURCE_DIR}/include/core/startup.hpp
    ${PROJECT_SOURCE_DIR}/include/core/input_manager.hpp
    ${PROJECT_SOURCE_DIR}/include/core/system_info.hpp)


set(RENDERER_SOURCE_LIST
    ${PROJECT_SOURCE_DIR}/src/renderer/renderer.cpp
    ../include/renderer/render_data.hpp
    renderer/model_render_loader.cpp)

set(RENDERER_HEADER_LIST
    ${PROJECT_SOURCE_DIR}/include/renderer/renderer.hpp
    ../include/renderer/model_render_loader.hpp
    ../include/renderer/shader_set.hpp)

set(ASSET_SOURCE_LIST
    ${PROJECT_SOURCE_DIR}/src/asset/mesh.cpp
    ${PROJECT_SOURCE_DIR}/src/asset/basic_mesh_reader.cpp
    ${PROJECT_SOURCE_DIR}/src/asset/scene_loader.cpp
    ${PROJECT_SOURCE_DIR}/src/asset/texture_manager.cpp
    asset/assimp_loader.cpp
    asset/tinygltf_loader.cpp
    asset/prototype_loader.cpp
    asset/component_loader.cpp)

set(ASSET_HEADER_LIST
    ${PROJECT_SOURCE_DIR}/include/asset/texture_manager.hpp
    ${PROJECT_SOURCE_DIR}/include/asset/textured_mesh.hpp
    ${PROJECT_SOURCE_DIR}/include/asset/mesh.hpp
    ${PROJECT_SOURCE_DIR}/include/asset/basic_mesh_reader.hpp
    ${PROJECT_SOURCE_DIR}/include/asset/mesh_read_error.hpp
    ${PROJECT_SOURCE_DIR}/include/asset/scene.hpp
    ${PROJECT_SOURCE_DIR}/include/asset/scene_loader.hpp
    ${PROJECT_SOURCE_DIR}/include/asset/assimp_loader.hpp
    ${PROJECT_SOURCE_DIR}/include/asset/tinygltf_loader.hpp
    ../include/asset/mesh_type.hpp
    ../include/asset/asset_types.hpp
    ../include/asset/prototype_loader.hpp
    ../include/asset/component_loader.hpp)

set(PLATFORM_SOURCE_LIST
    ${PROJECT_SOURCE_DIR}/src/platform/sleep.cpp
)

set(PLATFORM_HEADER_LIST
    ${PROJECT_SOURCE_DIR}/include/platform/sleep.hpp
    ../include/platform/symbols.hpp)

set(UTIL_SOURCE_LIST
)

set(EVENTS_SOURCE_LIST

)

set(EVENTS_HEADER_LIST
    ${PROJECT_SOURCE_DIR}/include/events/event.hpp
    ${PROJECT_SOURCE_DIR}/include/events/event_exchange.hpp
    ${PROJECT_SOURCE_DIR}/include/events/event_type.hpp
    ${PROJECT_SOURCE_DIR}/include/events/listener.hpp
    )

set(MEMORY_SOURCE_LIST
    ${PROJECT_SOURCE_DIR}/src/memory/pool_allocator.cpp
    ${PROJECT_SOURCE_DIR}/src/memory/pool_memory_block.cpp
)

set(MEMORY_HEADER_LIST
    ${PROJECT_SOURCE_DIR}/include/memory/pool_allocator.hpp
    ${PROJECT_SOURCE_DIR}/include/memory/pool_memory_block.hpp
    ${PROJECT_SOURCE_DIR}/include/memory/typed_pool_allocator.hpp
)


set (ECS_SOURCE_LIST
     ${PROJECT_SOURCE_DIR}/src/ecs/components/basic_components.cpp
     ${PROJECT_SOURCE_DIR}/src/ecs/component_meta.cpp
     ${PROJECT_SOURCE_DIR}/src/ecs/entity_factory.cpp
     ${PROJECT_SOURCE_DIR}/src/ecs/entity_world.cpp
     ecs/chunk_component_accessor.cpp)

set (ECS_HEADER_LIST
     ${PROJECT_SOURCE_DIR}/include/ecs/components/basic_components.hpp
     ${PROJECT_SOURCE_DIR}/include/ecs/archetype_chunk_component.hpp
     ${PROJECT_SOURCE_DIR}/include/ecs/archetype_id.hpp
     ${PROJECT_SOURCE_DIR}/include/ecs/archetype_pool.hpp
     ${PROJECT_SOURCE_DIR}/include/ecs/chunk_component_accessor.hpp
     ${PROJECT_SOURCE_DIR}/include/ecs/component_meta.hpp
     ${PROJECT_SOURCE_DIR}/include/ecs/component.hpp
     ${PROJECT_SOURCE_DIR}/include/ecs/ecs_types.hpp
     ${PROJECT_SOURCE_DIR}/include/ecs/entity_factory.hpp
     ${PROJECT_SOURCE_DIR}/include/ecs/entity_world.hpp
     ${PROJECT_SOURCE_DIR}/include/ecs/entity.hpp
     ${PROJECT_SOURCE_DIR}/include/ecs/components/mesh_opengl.hpp)


set (PHYSICS_SOURCE_LIST
     physics/physics_world.cpp physics/collisions.cpp)

set (PHYSICS_HEADER_LIST
     ../include/physics/physics_world.hpp
     ../include/physics/collisions.hpp
     ../include/physics/intersection_tests.hpp
     ../include/physics_models/contact.hpp ../include/physics/collider_iterator.hpp ../include/physics/entity_contact.hpp)


set (PHYSICS_MODELS_SOURCE_LIST
     ${PROJECT_SOURCE_DIR}/src/physics_models/contact.cpp
     physics_models/collision_tests.cpp)

set (PHYSICS_MODELS_HEADER_LIST
     ../include/physics_models/aabb.hpp
     ../include/physics_models/sphere.hpp
     ../include/physics_models/collider.hpp
     ../include/physics_models/contact.hpp
     ../include/physics_models/collision_tests.hpp
     ../include/physics_models/aabb_collider.hpp ../include/physics_models/aabb_collider.hpp ../include/physics_models/sphere_collider.hpp)


set(HEADER_LIST
    ${PROJECT_SOURCE_DIR}/include/events/event_dispatcher.hpp
    ${PROJECT_SOURCE_DIR}/include/util/running_average.hpp
)


# Make an automatic library - will be static or dynamic based on user setting
add_library(${CORELIB_NAME} STATIC ${CORE_SOURCE_LIST} ${CORE_HEADER_LIST})
add_library(${PLATFORMLIB_NAME} STATIC ${PLATFORM_SOURCE_LIST} ${PLATFORM_HEADER_LIST})
add_library(${RENDERER} STATIC ${RENDERER_SOURCE_LIST} ${RENDERER_HEADER_LIST})
add_library(${ASSETLIB_NAME} STATIC ${ASSET_SOURCE_LIST} ${ASSET_HEADER_LIST})
add_library(${MEM_LIB} STATIC ${MEMORY_SOURCE_LIST} ${MEMORY_HEADER_LIST})
add_library(${ECS_LIB} STATIC ${ECS_SOURCE_LIST} ${ECS_HEADER_LIST})
add_library(${PHYSICS_LIB} STATIC ${PHYSICS_SOURCE_LIST} ${PHYSICS_HEADER_LIST})
add_library(${PHYSICS_MODELS_LIB} STATIC ${PHYSICS_MODELS_SOURCE_LIST} ${PHYSICS_MODELS_HEADER_LIST})



# We need this directory, and users of our library will need it too
target_include_directories(${CORELIB_NAME} PUBLIC ../include ${GLFW_INCLUDE})
target_include_directories(${PLATFORMLIB_NAME} PUBLIC ../include)
target_include_directories(${RENDERER} PUBLIC ../include ../thirdparty/opengllib/include)
target_include_directories(${ASSETLIB_NAME} PUBLIC ../include ../thirdparty/opengllib/include)
target_include_directories(${MEM_LIB} PUBLIC ../include)
target_include_directories(${ECS_LIB} PUBLIC ../include ../thirdparty/opengllib/include)
target_include_directories(${PHYSICS_LIB} PUBLIC ../include)
target_include_directories(${PHYSICS_MODELS_LIB} PUBLIC ../include)




# All users of this library will need at least C++17
target_compile_features(${CORELIB_NAME} PUBLIC cxx_std_17)
target_compile_features(${PLATFORMLIB_NAME} PUBLIC cxx_std_17)
target_compile_features(${RENDERER} PUBLIC cxx_std_17)
target_compile_features(${ASSETLIB_NAME} PUBLIC cxx_std_17)
target_compile_features(${MEM_LIB} PUBLIC cxx_std_17)
target_compile_features(${ECS_LIB} PUBLIC cxx_std_17)
target_compile_features(${PHYSICS_LIB} PUBLIC cxx_std_17)
target_compile_features(${PHYSICS_MODELS_LIB} PUBLIC cxx_std_17)
