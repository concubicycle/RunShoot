//
// Created by sava on 11/23/19.
//

#ifndef __DRONE_SPAWNER_HPP_
#define __DRONE_SPAWNER_HPP_

#include <ctime>

#include <core/behavior.hpp>

#include "components/drone_spawner_component.hpp"
#include "components/player_controller_component.hpp"
#include "components/drone_controller_component.hpp"

class drone_spawner : public core::behavior
{
public:
    drone_spawner(events::event_exchange& events) : behavior(events)
    {
        auto grab_cam_fn = std::function([this](ecs::entity &e) { try_grab_player(e); });
        auto forget_cam_fn = std::function([this](ecs::entity &e) { try_forget_player(e); });
        _entity_create_listener_id = _events.subscribe<ecs::entity &>(events::entity_created, grab_cam_fn);
        _entity_destroy_listener_id = _events.subscribe(events::entity_destroyed, forget_cam_fn);

    }

    ~drone_spawner()
    {
        _events.unsubscribe(events::entity_created, _entity_create_listener_id);
        _events.unsubscribe(events::entity_destroyed, _entity_destroy_listener_id);
    }


    component_bitset required_components()  const override
    {
        return
            drone_spawner_component::archetype_bit;
    }

    void update_single(ecs::entity& e, core::behavior_context &ctx) override
    {
        if (!_player_ptr) return;

        auto& spawner = e.get_component<drone_spawner_component>();
        spawner.time_to_spawn -= ctx.time.smoothed_delta_secs();

        if (spawner.drone_spawn_interval > spawner.min_drone_spawn_interval)
             spawner.drone_spawn_interval -= ctx.time.smoothed_delta_secs() * 0.1f;

        if (spawner.time_to_spawn < 0)
        {
            spawner.time_to_spawn = spawner.drone_spawn_interval;

            auto& drone_e = ctx.current_scene.load_prototype("./assets/prototypes/drone.json");
            auto& drone_t = drone_e.get_component<ecs::transform_component>();
            auto& drone_rb = drone_e.get_component<ecs::rigid_body_component>();
            auto& drone = drone_e.get_component<drone_controller_component>();
            drone.player_ptr = _player_ptr;

            auto& player_t = _player_ptr->get_component<ecs::transform_component>();
            auto& player = _player_ptr->get_component<player_controller_component>();
            auto& player_rb = _player_ptr->get_component<ecs::rigid_body_component>();

            drone_t.pos = player_t.pos +
                glm::vec3(spawner.horizontal_spawn_offset, spawner.vertical_spawn_offset, 0) +
                player.direction * 20.f;

            drone.horizontal_offset = rand() % 40 - 20;
            drone.vertical_offset = rand() % 13 - 5;
            drone.z_offset = rand() % 30 - 15;
        }
    }


private:
    ecs::entity* _player_ptr;

    listener_id _entity_create_listener_id;
    listener_id _entity_destroy_listener_id;


    void try_grab_player(ecs::entity &e)
    {
        if (e.has<player_controller_component>())
            _player_ptr = &e;
    }

    void try_forget_player(ecs::entity &e)
    {
        if (e.has<player_controller_component>())
            _player_ptr = nullptr;
    }
};


#endif //__DRONE_SPAWNER_HPP_
