//
// Created by sava on 11/28/19.
//

#include <sound_wrapper/sound_listener_component.hpp>




void sound::load_sound_listener(const json &j, ecs::entity &e, string_table &hashes) {}

template<> const component_shift ecs::component<sound::sound_listener_component>::component_bitshift =
    62; // i don't now, just.. whatever. this system needs a code generation solution.
