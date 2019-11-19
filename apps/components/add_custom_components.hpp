//
// Created by sava on 10/20/19.
//

#ifndef __ADD_CUSTOM_COMPONENTS_HPP_
#define __ADD_CUSTOM_COMPONENTS_HPP_

#include <functional>
#include <ecs/component_meta.hpp>
#include <ecs/entity.hpp>
#include <asset/scene_loader.hpp>
#include <nlohmann/json.hpp>
using nlohmann::json;

#include "freefly_controller_component.hpp"
#include "drone_controller_component.hpp"
#include "player_controller_component.hpp"
#include "segment_component.hpp"
#include "turn_trigger_component.hpp"

// add own custom components to static variables from ecs
void add_custom_components()
{
    ecs::component_meta::bit_metas.insert(std::make_pair(
        freefly_controller_component::component_bitshift,
        ecs::component_meta::of<freefly_controller_component>()));

    ecs::component_meta::bit_metas.insert(std::make_pair(
        drone_controller_component::component_bitshift,
        ecs::component_meta::of<drone_controller_component>()));

    ecs::component_meta::bit_metas.insert(std::make_pair(
        player_controller_component::component_bitshift,
        ecs::component_meta::of<player_controller_component>()));

    ecs::component_meta::bit_metas.insert(std::make_pair(
        segment_component::component_bitshift,
        ecs::component_meta::of<segment_component>()));

    ecs::component_meta::bit_metas.insert(std::make_pair(
        turn_trigger_component::component_bitshift,
        ecs::component_meta::of<turn_trigger_component>()));


    asset::component_loader::loader_functions.insert(
        std::make_pair(freefly_controller_component::component_bitshift, load_character_controller));

    asset::component_loader::loader_functions.insert(
        std::make_pair(drone_controller_component::component_bitshift, load_drone_controller));

    asset::component_loader::loader_functions.insert(
        std::make_pair(player_controller_component::component_bitshift, load_player_controller));

    asset::component_loader::loader_functions.insert(
        std::make_pair(segment_component::component_bitshift, load_segment_component));

    asset::component_loader::loader_functions.insert(
        std::make_pair(turn_trigger_component::component_bitshift, load_turn_trigger));
}

#endif //__ADD_CUSTOM_COMPONENTS_HPP_
