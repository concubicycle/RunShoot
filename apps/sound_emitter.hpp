//
// Created by sava on 11/28/19.
//

#ifndef __SOUND_EMITTER_HPP_
#define __SOUND_EMITTER_HPP_

#include <core/behavior.hpp>

#include "components/sound_emitter_component.hpp"
#include "sound.hpp"

class sound_emitter : public core::behavior
{
public:
    explicit sound_emitter(events::event_exchange& events, string_table& app_strings, sound& game_sound);

    [[nodiscard]] component_bitset required_components()  const override
    {
        return
            sound_emitter_component::archetype_bit;
    }

protected:
    void update_single(ecs::entity& e, core::behavior_context &ctx) override;


private:
    string_table& _app_strings;
    sound& _game_sound;
};


#endif //__SOUND_EMITTER_HPP_
