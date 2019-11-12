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

#define GRAVITY_ACCELERATION -0.5f

class player_controller : public core::behavior
{
    static const std::uint32_t PlayerHeight = 12;


public:
    player_controller(events::event_exchange& events)  : behavior(events)
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
    void update_single(ecs::entity& e, core::behavior_context &ctx) override
    {
        auto& player = e.get_component<player_controller_component>();

        switch (player.state)
        {
            case running:
                update_running(e, player, ctx.time.smoothed_delta_secs());
                break;
        }
    }

    virtual void on_entity_created(ecs::entity& e) override
    {
        auto& player = e.get_component<player_controller_component>();
        auto& c = e.get_component<ecs::camera_component>();
        auto& t = e.get_component<ecs::transform_component>();
        auto& rb = e.get_component<ecs::rigid_body_component>();

        player.to_camera = c.position - t.pos;
        rb.velocity = player.direction * player.run_speed;
    }

private:
    listener_id _collision_listener_id;

    static void update_running(ecs::entity& e, player_controller_component& comp, float frame_time)
    {
        auto& rb = e.get_component<ecs::rigid_body_component>();
        auto& t = e.get_component<ecs::transform_component>();


        if (comp.time_since_collision > 0.25f)
        {
            comp.is_grounded = false;
        }

        // integrate
        auto fa = rb.force * rb.mass_inverse();

        rb.force += rb.gravity;

        rb.acceleration = rb.force;
        rb.velocity += rb.acceleration * frame_time;
        rb.force = glm::vec3(0);

        move_component_positions(e, rb.velocity * frame_time);

        std::cout << glm::to_string(t.pos) << std::endl;
        comp.time_since_collision += frame_time;
    }

    static void on_collision(const physics::entity_contact& collision, float dt)
    {
        auto one_comp = collision.one().get_component_opt<player_controller_component>();
        auto two_comp = collision.two().get_component_opt<player_controller_component>();
        if (one_comp) resolve_collision(collision, collision.one(), *one_comp, dt);
        if (two_comp) resolve_collision(collision, collision.two(), *two_comp, dt);
    }

    static void resolve_collision(
        const physics::entity_contact& collision,
        ecs::entity& e,
        player_controller_component& component,
        float dt)
    {
        auto& rb = e.get_component<ecs::rigid_body_component>();
        auto& n = collision.contact().collision_axis();

        component.is_grounded = true;

        auto intersecting = collision.contact().time() == physics_models::contact::Intersecting;

        if (intersecting)
        {
            auto move = n * -1.01f;
            rb.velocity -= n * glm::dot(n, rb.velocity);
            move_component_positions(e, move);
        } else
        {
            auto move = rb.velocity * (dt - 0.001f);
            auto n = glm::normalize(collision.collision_axis());
            rb.velocity -= n * glm::dot(n, rb.velocity);
            rb.force.x = rb.force.y = rb.force.z = 0.f;
        }

        component.time_since_collision = 0;
    }

    // ugh...
    static void move_component_positions(ecs::entity& e, glm::vec3 displacement)
    {
        auto& player = e.get_component<player_controller_component>();
        auto& rb = e.get_component<ecs::rigid_body_component>();
        auto& t = e.get_component<ecs::transform_component>();
        auto& c = e.get_component<ecs::camera_component>();
        t.pos += displacement;
        rb.position = t.pos;
        c.position = t.pos + player.to_camera;

        physics::collider_iterator it(e);
        physics_models::collider *cursor;
        while (it.end() != (cursor = it.get_next()))
            cursor->set_position(rb.position);
    }

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
