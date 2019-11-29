//
// Created by sava on 11/27/19.
//

#ifndef __MUSIC_PLAYER_COMPONENT_HPP_
#define __MUSIC_PLAYER_COMPONENT_HPP_

#include <string>

#include <nlohmann/json.hpp>
using nlohmann::json;

#include <ecs/entity.hpp>
#include <ecs/component.hpp>

#define MAX_NUM_TRACKS 16

struct music_player_component : ecs::component<music_player_component>
{
    size_t track_hashes[MAX_NUM_TRACKS];
    std::string tracks[MAX_NUM_TRACKS];
    std::uint16_t track_count;
};

void load_music_player(const json& j, ecs::entity& e, string_table& hashes);

#endif //__MUSIC_PLAYER_COMPONENT_HPP_
