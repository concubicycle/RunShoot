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

    public:
        explicit physics_world(
            events::event_exchange &events,
            collisions &collisions);

        ~physics_world();

        void update(float frame_time);

    private:
        std::vector<physics_models::contact> _contacts;

        events::event_exchange &_events;
        physics::collisions &_collisions;

        std::vector<std::reference_wrapper<ecs::entity>> _physical_entities;
        std::vector<std::reference_wrapper<ecs::entity>> _collision_entities;

        listener_id _entity_create_listener_id;
        listener_id _entity_destroy_listener_id;

        void integrate(ecs::entity& e, float frame_time);


        void grab_entity (ecs::entity &e);
        void forget_entity (ecs::entity &e);
    };

}

#endif //__PHYSICS_WORLD_HPP_
