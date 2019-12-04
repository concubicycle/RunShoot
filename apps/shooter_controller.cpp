//
// Created by sava on 12/3/19.
//

#include <iostream>
#include <sound_wrapper/sound_emitter_component.hpp>

#include "shooter_controller.hpp"
#include "runshoot_event.hpp"

void shooter_controller::update_single(ecs::entity &e, core::behavior_context &ctx)
{
    update_yaw(e);

    auto& shooter = e.get_component<shooter_controller_component>();
    auto& animator = e.get_component<ecs::billboard_animation_component>();

    switch (shooter.state)
    {
        case shooter_controller_component::waiting:
            update_waiting(e, ctx);
            break;
        case shooter_controller_component::shooting:
            update_shooting(e, ctx);
            break;
        case shooter_controller_component::dying:
            update_dying(e, ctx);
            break;
    }
}

void shooter_controller::update_yaw(ecs::entity &e)
{
    auto player = _player_grabber.get_single();
    if (!player) return;

    auto& transform = e.get_component<ecs::transform_component>();
    auto t = e.graph_node->absolute_transform();
    auto pos = t[3];
    auto player_t = player->get().graph_node->absolute_transform();
    auto player_pos = player_t[3];

    glm::vec3 original_xz(0, 0, -1);
    glm::vec3 original_xz_left(-1, 0, 0);
    glm::vec3 to_player = glm::normalize(player_pos - pos);
    to_player.y = 0;

    auto d = glm::dot(to_player, original_xz);
    auto side = glm::dot(to_player, original_xz_left);
    auto theta = std::acos(d);

    if (side < 0)
        theta = glm::pi<float>() - (theta);
    else
        theta = glm::pi<float>() - (-theta);

    transform.yaw = theta;
}

void shooter_controller::update_waiting(ecs::entity &e, core::behavior_context &ctx)
{
    auto& shooter = e.get_component<shooter_controller_component>();
    auto dt = ctx.time.smoothed_delta_secs();
    auto& animator = e.get_component<ecs::billboard_animation_component>();

    animator.current_state = 0;

    shooter.time_to_shoot -= dt;

    if (shooter.time_to_shoot < 0)
    {
        shooter.time_to_shoot = shooter.shoot_interval + std::rand() % 2;
        shooter.state = shooter_controller_component::shooting;
        shooter.did_shoot = false;
    }

    e.graph_node->traverse([&e](ecs::entity& child_e, glm::mat4& transform) {
        if (e.id() == child_e.id()) return;
        child_e.set_active(false);
    });
}

void shooter_controller::update_shooting(ecs::entity &e, core::behavior_context &ctx)
{
    auto player = _player_grabber.get_single();
    if (!player) return;

    auto& shooter = e.get_component<shooter_controller_component>();
    auto& animator = e.get_component<ecs::billboard_animation_component>();
    auto dt = ctx.time.smoothed_delta_secs();

    if (!shooter.did_shoot)
    {
        auto& sound_emitter = e.get_component<sound::sound_emitter_component>();
        sound_emitter.set_sound_state(0, sound::playing);

        animator.current_state = 1;
        shooter.did_shoot = true;

        e.graph_node->traverse([&e](ecs::entity& child_e, glm::mat4& transform) {
            if (e.id() == child_e.id()) return;
            child_e.set_active(true);
        });

        auto& transform = e.get_component<ecs::transform_component>();
        auto t = e.graph_node->absolute_transform();
        auto pos = t[3];
        auto player_t = player->get().graph_node->absolute_transform();
        auto player_pos = player_t[3];
        auto to_player = glm::normalize(player_pos - pos);
        float dist = glm::length(to_player);
        float kill_chance = dist > 100 ? 0 : shooter.kill_chance;
        auto rand = std::rand() % 100;
        auto kill_chance_var = kill_chance * 100;
        auto is_kill = rand < kill_chance_var;

        if (is_kill)
        {
            _events.invoke<runshoot_event, ecs::entity&>(runshoot_event::shooter_hit, e);
        } else
        {
            _events.invoke<runshoot_event, ecs::entity&>(runshoot_event::shooter_miss, e);
        }
    }

    shooter.remaining_shot_flash_time -=dt;

    if (shooter.remaining_shot_flash_time < 0)
    {
        shooter.remaining_shot_flash_time = shooter.shot_flash_time;
        shooter.state = shooter_controller_component::waiting;
    }
}

void shooter_controller::update_dying(ecs::entity &e, core::behavior_context &ctx)
{
    auto& animator = e.get_component<ecs::billboard_animation_component>();
    auto& shooter = e.get_component<shooter_controller_component>();
    auto dt = ctx.time.smoothed_delta_secs();

    animator.current_state = 2;
    shooter.time_to_die -= dt;
    if (shooter.time_to_die < 0)
    {
        remove_entity(e);
    }
}

