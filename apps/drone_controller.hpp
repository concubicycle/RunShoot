//
// Created by sava on 10/29/19.
//

#ifndef __DRONE_CONTROLLER_HPP_
#define __DRONE_CONTROLLER_HPP_

#include <glm/gtc/random.hpp>

#include <ecs/behavior.hpp>
#include "components/drone_controller_component.hpp"
#include "components/player_controller_component.hpp"
#include <ecs/behavior.hpp>

class drone_controller : public ecs::behavior<core::behavior_context>
{
public:
    explicit drone_controller(events::event_exchange &events) : behavior(events) {}

    [[nodiscard]] component_bitset required_components() const override;


protected:

    void update_single(ecs::entity &e, core::behavior_context &ctx) override;

    void thrust_toward_target(ecs::entity &e);

    void friction(ecs::rigid_body_component &rb, drone_controller_component &drone);

    void on_entity_created(ecs::entity& e) override;

private:

    void update_following(ecs::entity &e, core::behavior_context &ctx);
    void update_flashing(ecs::entity &e, core::behavior_context &ctx);

    void set_target(ecs::entity &e);
};

#endif //__DRONE_CONTROLLER_HPP_
