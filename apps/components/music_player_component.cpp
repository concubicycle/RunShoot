//
// Created by sava on 11/27/19.
//

#include <util/string_table.hpp>
#include <ecs/component.hpp>
#include <ecs/entity.hpp>
#include "music_player_component.hpp"
#include "runshoot_component.hpp"


template<> const component_shift ecs::component<music_player_component>::component_bitshift =
    runshoot_component::MusicPlayer;


void load_music_player(const json& j, ecs::entity& e, string_table& hashes)
{
    auto& c = e.get_component<music_player_component>();

    c.track_count = 0;
    for (auto& track : j["tracks"])
    {
        c.tracks[c.track_count++] = track.get<std::string>();
    }
}

