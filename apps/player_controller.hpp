//
// Created by sava on 10/30/19.
//

#ifndef __PLAYER_CONTROLLER_HPP_
#define __PLAYER_CONTROLLER_HPP_


#include <core/behavior.hpp>
#include <physics/entity_contact.hpp>
#include <physics/collider_iterator.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include "components/player_controller_component.hpp"


class player_controller : public core::behavior
{
public:
    player_controller(events::event_exchange& events) : behavior(events)
    {
        _collision_listener_id = _events.subscribe<const physics::entity_contact&, float>(
            events::collision,
            std::function<void(const physics::entity_contact&, float)>(on_collision));
    }

    ~player_controller()
    {
        _events.unsubscribe<const physics::entity_contact&>(events::collision, _collision_listener_id);
    }

    component_bitset required_components()  const override { return player_components(); }

protected:
    void update_single(ecs::entity& e, core::behavior_context &ctx) override;
    virtual void on_entity_created(ecs::entity& e) override;


private:
    listener_id _collision_listener_id;

    static void update_running(ecs::entity& e, player_controller_component& comp, float frame_time);
    static void update_airborne(ecs::entity& e, player_controller_component& comp, float frame_time);
    static void on_collision(const physics::entity_contact& collision, float dt);
    static void move_component_positions(ecs::entity& e, glm::vec3 displacement);


    static void resolve_collision(const physics::entity_contact& collision,
                                  ecs::entity& e,
                                  player_controller_component& component,
                                  float dt);

    static component_bitset player_components()
    {
        return
            ecs::transform_component::archetype_bit |
            player_controller_component::archetype_bit |
            ecs::transform_component::archetype_bit |
            ecs::camera_component::archetype_bit;
    }
};


#endif //__PLAYER_CONTROLLER_HPP_
