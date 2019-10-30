//
// Created by sava on 10/13/19.
//

#include <cstdint>
#include <ecs/component.hpp>
#include <ecs/components/basic_components.hpp>
#include <ecs/ecs_types.hpp>


// ADD NEW COMPONENT BIT SHIFTS HERE
template<> const std::uint8_t ecs::component<ecs::matrix_component>::component_bitshift = MAX_BASIC_COMPONENTS-1; // dummy component for tests

template<> const component_shift ecs::component<ecs::transform_component>::component_bitshift = 0;
template<> const component_shift ecs::component<ecs::render_component_old>::component_bitshift = 1;
template<> const component_shift ecs::component<ecs::camera_component>::component_bitshift = 2;
template<> const component_shift ecs::component<ecs::render_component_ogl>::component_bitshift = 3;
template<> const component_shift ecs::component<ecs::punctual_light_component>::component_bitshift = 4;
template<> const component_shift ecs::component<ecs::aabb_component>::component_bitshift = 5;
template<> const component_shift ecs::component<ecs::physical_properties_component>::component_bitshift = 6;