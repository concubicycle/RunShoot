//
// Created by sava on 12/3/19.
//

#ifndef __SHOOTER_CONTROLLER_HPP_
#define __SHOOTER_CONTROLLER_HPP_


#include <core/behavior.hpp>
#include <core/entity_grabber.hpp>
#include "components/shooter_controller_component.hpp"
#include "components/player_controller_component.hpp"
#include "components/freefly_controller_component.hpp"

class shooter_controller : public core::behavior
{
public:
    explicit shooter_controller(events::event_exchange& events) : behavior(events),
        _player_grabber(events, [](ecs::entity& e) {
            return e.has<player_controller_component>() || e.has<freefly_controller_component>();
        })
    {

    }

    void update_single(ecs::entity& e, core::behavior_context &ctx) override;

    [[nodiscard]] component_bitset required_components()  const override
    {
        return shooter_controller_component::archetype_bit;
    }


private:
    core::entity_grabber _player_grabber;

    std::uint8_t _death_sound = 1;

    void update_yaw(ecs::entity &e);

    static void update_waiting(ecs::entity &e, core::behavior_context &ctx);
    void update_shooting(ecs::entity &e, core::behavior_context &ctx);
    void update_dying(ecs::entity &e, core::behavior_context &ctx);

    void take_shot(ecs::entity &e, core::behavior_context &ctx);
};


#endif //__SHOOTER_CONTROLLER_HPP_
