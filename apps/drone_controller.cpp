//
// Created by sava on 11/26/19.
//

#include <sound_wrapper/sound_emitter_component.hpp>
#include "drone_controller.hpp"

component_bitset drone_controller::required_components() const
{
    return
        drone_controller_component::archetype_bit |
        ecs::transform_component::archetype_bit;
}

void drone_controller::update_single(ecs::entity &e, core::behavior_context &ctx)
{
    auto &rb = e.get_component<ecs::rigid_body_component>();
    auto &component = e.get_component<drone_controller_component>();
    auto &transform = e.get_component<ecs::transform_component>();

    auto frame_time = ctx.time.smoothed_delta_secs();

    if (component.state != drone_controller_component::flashing)
    {
        e.graph_node->traverse([&e](ecs::entity& child_e, glm::mat4& transform) {
            if (e.id() == child_e.id()) return;
            child_e.set_active(false);
        });
    }

    if (glm::length2(rb.velocity) > component.max_speed_sq)
    {
        rb.velocity = glm::normalize(rb.velocity) * component.max_speed;
    }

    switch(component.state)
    {
        case drone_controller_component::flashing:
            update_flashing(e, ctx);
            update_following(e, ctx);
            break;
        case drone_controller_component::following:
            update_following(e, ctx);
            break;
        case drone_controller_component::exploding: {
            auto& l = e.get_component<ecs::punctual_light_component>();
            l.intensity = 0;

            auto& emitter = e.get_component<sound::sound_emitter_component>();
            emitter.set_sound_state(0, sound::stopped);

            component.active_after_destruction -= ctx.time.smoothed_delta_secs();

            if (component.active_after_destruction < 0)
                ctx.current_scene.remove(e);

            break;
        }

    }

    friction(rb, component);
}


void drone_controller::update_following(ecs::entity &e, core::behavior_context &ctx)
{
    auto &drone = e.get_component<drone_controller_component>();

    set_target(e);
    thrust_toward_target(e);

    if (drone.time_to_flash >= 0)
    {
        drone.time_to_flash -= ctx.time.smoothed_delta_secs();
        if (drone.time_to_flash < 0)
        {
            drone.state = drone_controller_component::flashing;
        }
    }
}

void drone_controller::update_flashing(ecs::entity &e, core::behavior_context &ctx)
{
    float dt = ctx.time.smoothed_delta_secs();
    auto &drone = e.get_component<drone_controller_component>();

    drone.time_to_warning_light -= dt;
    if (drone.time_to_warning_light < 0 && !drone.light_on)
    {
        e.graph_node->traverse([&e](ecs::entity& child_e, glm::mat4& transform) {
            if (e.id() == child_e.id()) return;
            child_e.set_active(true);
        });
        drone.time_to_warning_light = drone.warning_light_duration;
        drone.light_on = true;
        drone.flashes_before_blind--;
    }
    else if (drone.time_to_warning_light < 0 && drone.light_on)
    {
        e.graph_node->traverse([&e](ecs::entity& child_e, glm::mat4& transform) {
            if (e.id() == child_e.id()) return;
            child_e.set_active(false);
        });
        drone.time_to_warning_light = drone.warning_light_interval;
        drone.light_on = false;
    }

    drone.time_to_flash -= dt;
    drone.warning_light_interval *= (1.f - dt*0.9f);
    drone.warning_light_duration *= (1.f - dt*0.9f);

    if (drone.flashes_before_blind < 0)
    {
        drone.time_to_flash = drone.initial_time_to_flash;
        drone.warning_light_interval = drone.initial_warning_light_interval;
        drone.warning_light_duration = drone.initial_warning_light_duration;
        drone.flashes_before_blind = drone.initial_flashes_before_blind;

        _events.invoke<std::string, float>(events::camera_prop_set, "color_multiplier", 35.0f);
        drone.state = drone_controller_component::following;
    }
}

void drone_controller::set_target(ecs::entity &e)
{
    auto &rb = e.get_component<ecs::rigid_body_component>();
    auto &drone = e.get_component<drone_controller_component>();
    if (!drone.player_ptr)
    {
        drone.target = glm::vec3(0.f, 15.f, 0.f);
        return;
    }

    auto &player = drone.player_ptr->get_component<player_controller_component>();
    auto &player_t = drone.player_ptr->get_component<ecs::transform_component>();
    drone.target = player_t.pos + player.direction * 50.f;
    drone.target.y = 20.f;

    auto to_player = player_t.pos - rb.position;
    drone.target += glm::normalize(glm::cross(to_player, glm::vec3(0, 1, 0))) * drone.horizontal_offset;
    drone.target += glm::vec3(0, 1, 0) * drone.vertical_offset;
    drone.target += player.direction * drone.z_offset;
}

void drone_controller::thrust_toward_target(ecs::entity &e)
{
    auto &rb = e.get_component<ecs::rigid_body_component>();
    auto &drone = e.get_component<drone_controller_component>();
    auto to_target = drone.target - rb.position;

    if (to_target.y < 0) to_target.y = 0.1f;
    to_target.y *= 2;
    to_target.x *= .65;
    to_target.z *= .65;

    rb.force += to_target * drone.acceleration;
}

void drone_controller::friction(ecs::rigid_body_component &rb, drone_controller_component &drone)
{
    // up the friction when close to target
    auto to_target = drone.target - rb.position;
    auto tangent = glm::normalize(glm::cross(glm::vec3(0, 1, 0), to_target));
    float friction = drone.friction + std::max((1.f - drone.friction) * 0.7f, 1.f / glm::length(to_target));

    rb.force += -(rb.velocity) * friction;

    // cancel off some tangent velocity, try not to orbit the player
    float tan_speed = glm::dot(rb.velocity, tangent);

    if (tan_speed * tan_speed > 40.f)
        rb.force -= 5.0f * tan_speed * tangent;
}

void drone_controller::on_entity_created(ecs::entity &e)
{
    auto& emitter = e.get_component<sound::sound_emitter_component>();
    emitter.set_sound_state(0, sound::playing);
    emitter.emitter_sounds[0].loop = true;
}

