//
// Created by sava on 11/28/19.
//

#include "music_player.hpp"
#include <ecs/behavior.hpp>

music_player::music_player(events::event_exchange &events, string_table& app_strings, sound::sound_system& sound)
    : behavior(events)
    , _app_strings(app_strings)
    , _sound(sound)
{

}

void music_player::update_single(ecs::entity &e, core::behavior_context &ctx)
{

}


void music_player::on_entity_created(ecs::entity &e)
{
    auto& music_player = e.get_component<music_player_component>();

    for (int i = 0; i < music_player.track_count; ++i)
        music_player.track_hashes[i] = _app_strings.hash_and_store(music_player.tracks[i]);

    if (music_player.track_count > 0)
        _sound.play_sound_one_off(music_player.track_hashes[0], true);
}