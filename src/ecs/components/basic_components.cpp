//
// Created by sava on 10/13/19.
//

#include <ecs/ecs_types.hpp>
#include <ecs/component.hpp>
#include <ecs/components/basic_components.hpp>



// ADD NEW COMPONENT BIT SHIFTS HERE
template<> const component_shift ecs::component<ecs::transform_component>::component_bitshift = 0;
template<> const component_shift ecs::component<ecs::camera_component>::component_bitshift = 1;
template<> const component_shift ecs::component<ecs::render_component_ogl>::component_bitshift = 2;
template<> const component_shift ecs::component<ecs::punctual_light_component>::component_bitshift = 3;
template<> const component_shift ecs::component<ecs::aabb_component>::component_bitshift = 4;
template<> const component_shift ecs::component<ecs::rigid_body_component>::component_bitshift = 5;