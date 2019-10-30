//
// Created by sava on 10/29/19.
//

#ifndef __DRONE_CONTROLLER_HPP_
#define __DRONE_CONTROLLER_HPP_

#include <core/behavior.hpp>
#include "components/drone_controller_component.hpp"

class drone_controller : public core::behavior
{
public:
    explicit drone_controller(events::event_exchange& events)  : behavior(events) {}

    component_bitset required_components()  const override
    {
        return
            drone_controller_component::archetype_bit |
            ecs::transform_component::archetype_bit;
    }


protected:

    void update_single(ecs::entity& e, core::behavior_context &ctx) override
    {
        auto& component = e.get_component<drone_controller_component>();
        auto& transform = e.get_component<ecs::transform_component>();

        auto frame_time = ctx.time.smoothed_delta_secs();

        component.time_to_dir_switch -= frame_time;

        if (component.time_to_dir_switch < 0)
        {
            component.dir *= -1;
            component.time_to_dir_switch = component.dir_switch_interval;
        }

        transform.pos.y += frame_time * component.dir;
    }
};

#endif //__DRONE_CONTROLLER_HPP_
