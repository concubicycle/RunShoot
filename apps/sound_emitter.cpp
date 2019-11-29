//
// Created by sava on 11/28/19.
//

#include "sound_emitter.hpp"

sound_emitter::sound_emitter(events::event_exchange &events, string_table &app_strings, sound &game_sound)
    : behavior(events), _app_strings(app_strings), _game_sound(game_sound) {}


void sound_emitter::update_single(ecs::entity &e, core::behavior_context &ctx)
{
    auto& emitter = e.get_component<sound_emitter_component>();

    for (int i = 0; i < emitter.sounds_to_play_count; ++i)
        _game_sound.play_sound(emitter.sounds_to_play[i], false);

    emitter.sounds_to_play_count = 0;
}
