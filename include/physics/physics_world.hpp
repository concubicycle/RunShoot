//
// Created by sava on 10/28/19.
//

#ifndef __PHYSICS_WORLD_HPP_
#define __PHYSICS_WORLD_HPP_

#include <cstdint>
#include <vector>
#include <events/event_exchange.hpp>
#include <ecs/entity.hpp>
#include <physics_models/contact.hpp>
#include "collisions.hpp"


namespace physics
{
    class physics_world
    {
        const std::uint32_t NumContactsReserved = 1024;
        const std::uint32_t NumEntitiesReserved = 2048;
        const float ContinuousCollisionResolutionBias = 0.0001f;

    public:
        physics_world(events::event_exchange &events, collisions &collisions);
        ~physics_world();

        void update(float frame_time);

    private:
        events::event_exchange &_events;
        physics::collisions &_collisions;

        std::vector<entity_contact> _contacts;
        std::vector<std::reference_wrapper<ecs::entity>> _physical_entities;
        std::vector<std::reference_wrapper<ecs::entity>> _collision_entities;

        listener_id _entity_create_listener_id;
        listener_id _entity_destroy_listener_id;

        void detect_collisions();
        void resolve_collisions(float frame_time);
        void resolve_collision_continuous(std::vector<physics::entity_contact>::iterator first_col);
        void resolve_collision_discrete(std::vector<physics::entity_contact>::iterator first_col);
        void resolve_collision_trigger(std::vector<physics::entity_contact>::iterator first_col);

        void update_transforms();
        void grab_entity (ecs::entity &e);
        void forget_entity (ecs::entity &e);

        static void resolve_velocity(const physics::entity_contact& collision, ecs::entity& e);
        static void update_collider_positions(ecs::entity& e);
        static void integrate(ecs::entity& e, float frame_time);
        static void integrate_position(ecs::entity& e, float frame_time);
        static void sync_to_transform(ecs::entity& e);
    };

}

#endif //__PHYSICS_WORLD_HPP_
