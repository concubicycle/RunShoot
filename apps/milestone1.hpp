//
// Created by sava on 10/18/19.
//

#ifndef __MILESTONE1_HPP_
#define __MILESTONE1_HPP_

#include <core/input_manager.hpp>
#include <ecs/components/basic_components.hpp>
#include <ecs/entity.hpp>


void handle_input(
    ecs::entity &entity,
    core::input_manager &input,
    float frame_time);

void spin_quad(ecs::entity &entity,
    float frame_time);

#endif //__MILESTONE1_HPP_
