//
// Created by sava on 10/28/19.
//

#ifndef __PHYSICS_WORLD_HPP_
#define __PHYSICS_WORLD_HPP_

#include <vector>
#include <events/event_exchange.hpp>
#include <ecs/entity.hpp>


namespace physics
{

    class physics_world
    {
    public:
        explicit physics_world(events::event_exchange &events);
        ~physics_world();

        void update();

    private:
        std::vector<std::reference_wrapper<ecs::entity>> _physical_entities;
        std::vector<std::reference_wrapper<ecs::entity>> _collision_entities;


        events::event_exchange &_events;
        listener_id _entity_create_listener_id;
        listener_id _entity_destroy_listener_id;

    };

}

#endif //__PHYSICS_WORLD_HPP_
