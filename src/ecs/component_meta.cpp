//
// Created by sava on 10/13/19.
//

#include <ecs/ecs_types.hpp>
#include <ecs/component_meta.hpp>
#include <map>

std::map<component_shift, ecs::component_meta> ecs::component_meta::bit_metas
    {
        {ecs::transform_component::component_bitshift, of<ecs::transform_component>()},
        {ecs::camera_component::component_bitshift, of<ecs::camera_component>()},
        {ecs::render_component_ogl::component_bitshift, of<ecs::render_component_ogl>()},
        {ecs::punctual_light_component::component_bitshift, of<ecs::punctual_light_component>()},
        {ecs::aabb_component::component_bitshift, of<ecs::aabb_component>()},
        {ecs::rigid_body_component::component_bitshift, of<ecs::rigid_body_component>()}
    };
