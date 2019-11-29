//
// Created by sava on 11/28/19.
//

#include "sound_emitter_component.hpp"
#include "runshoot_component.hpp"


void load_sound_emitter(const json& j, ecs::entity& e, string_table& hashes)
{
    auto& emitter = e.get_component<sound_emitter_component>();

    emitter.sound_count = 0;
    for (auto& s : j["sounds"])
        emitter.sounds[emitter.sound_count++] = s.get<std::string>();

    for (std::uint32_t i = 0; i < emitter.sound_count; ++i)
        emitter.sound_path_hashes[i] = hashes.hash_and_store(emitter.sounds[i]);

}

template<> const component_shift ecs::component<sound_emitter_component>::component_bitshift =
    runshoot_component::SoundEmitter;
