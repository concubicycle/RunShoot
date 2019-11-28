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
#include "segment_spawner_component.hpp"
#include "drone_spawner_component.hpp"
#include "music_player_component.hpp"

template <typename TComponent>
void add_component(std::function<void(const json &, ecs::entity &, string_table &)> load_func)
{
    ecs::component_meta::bit_metas.insert(std::make_pair(
        TComponent::component_bitshift,
        ecs::component_meta::of<TComponent>()));

    asset::component_loader::loader_functions.insert(
        std::make_pair(TComponent::component_bitshift, load_func));
}

// add own custom components to static variables from ecs
void add_custom_components()
{
    add_component<freefly_controller_component>(load_character_controller);
    add_component<drone_controller_component>(load_drone_controller);
    add_component<player_controller_component>(load_player_controller);
    add_component<segment_component>(load_segment_component);
    add_component<turn_trigger_component>(load_turn_trigger);
    add_component<segment_spawner_component>(load_segment_spawner);
    add_component<drone_spawner_component>(load_drone_spawner);
    add_component<music_player_component>(load_music_player);

}

#endif //__ADD_CUSTOM_COMPONENTS_HPP_
