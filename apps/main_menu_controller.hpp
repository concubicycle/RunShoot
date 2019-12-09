//
// Created by sava on 12/8/19.
//

#ifndef __MAIN_MENU_CONTROLLER_HPP_
#define __MAIN_MENU_CONTROLLER_HPP_

#include <string>
#include <ecs/ecs_types.hpp>
#include <core/behavior.hpp>
#include <events/event_exchange.hpp>
#include "components/main_menu_controller_component.hpp"


class main_menu_controller : public core::behavior
{

public:
    explicit main_menu_controller(events::event_exchange& events)  : behavior(events) {}

    [[nodiscard]] component_bitset required_components()  const override
    {
        return main_menu_controller_component::archetype_bit;
    }

protected:

    void update_single(ecs::entity& e, core::behavior_context &ctx) override
    {
        if (ctx.input.was_key_pressed(GLFW_KEY_SPACE))
        {
            _events.invoke<const std::string&>(events::scene_change, "./assets/scenes/runshoot_gameplay.json");
        }
    }
};

#endif //__MAIN_MENU_CONTROLLER_HPP_
