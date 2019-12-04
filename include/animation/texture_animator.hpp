//
// Created by sava on 12/3/19.
//

#ifndef __TEXTURE_ANIMATOR_HPP_
#define __TEXTURE_ANIMATOR_HPP_

#include <vector>

#include <events/event_exchange.hpp>

namespace animation
{
    class texture_animator
    {
    public:
        explicit texture_animator(events::event_exchange &events);
        ~texture_animator();

        void update(float dt);

    private:
        events::event_exchange &_events;
        std::vector<std::reference_wrapper<ecs::entity>> _entities;

        listener_id _entity_create_listener_id;
        listener_id _entity_destroy_listener_id;

        void grab_entity(ecs::entity &e);
        void forget_entity(ecs::entity &e);
    };

}

#endif //__TEXTURE_ANIMATOR_HPP_
