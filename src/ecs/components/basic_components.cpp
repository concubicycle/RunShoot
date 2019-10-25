//
// Created by sava on 10/13/19.
//

#include <cstdint>
#include <ecs/component.hpp>
#include <ecs/components/basic_components.hpp>


// ADD NEW COMPONENT BIT SHIFTS HERE
template<> const std::uint8_t ecs::component<ecs::matrix_component>::component_bitshift = MAX_BASIC_COMPONENTS-1; // dummy component for tests

template<> const std::uint8_t ecs::component<ecs::transform_component>::component_bitshift = 0;
template<> const std::uint8_t ecs::component<ecs::render_component_old>::component_bitshift = 1;
template<> const std::uint8_t ecs::component<ecs::camera_component>::component_bitshift = 2;
template<> const std::uint8_t ecs::component<ecs::render_component_ogl>::component_bitshift = 3;