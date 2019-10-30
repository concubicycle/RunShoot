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

#include "character_controller_component.hpp"

// add own custom components to static variables from ecs
void add_custom_components()
{
    ecs::component_meta::bit_metas.insert(std::make_pair(
        character_controller_component::component_bitshift,
        ecs::component_meta::of<character_controller_component>()));

    asset::component_loader::loader_functions.insert(
        std::make_pair(character_controller_component::component_bitshift, load_character_controller));
}

#endif //__ADD_CUSTOM_COMPONENTS_HPP_
