//
// Created by sava on 12/3/19.
//

#ifndef __ENTITY_GRABBER_HPP_
#define __ENTITY_GRABBER_HPP_

#include <vector>
#include <events/event_exchange.hpp>
#include "ecs/entity.hpp"

namespace core
{

    class entity_grabber
    {
    public:
        explicit entity_grabber(
            events::event_exchange& events,
            std::function<bool(ecs::entity&)> _predicate);

        ~entity_grabber();

        std::optional<std::reference_wrapper<ecs::entity>> get_single();

    private:
        std::function<bool(ecs::entity&)> _predicate;
        events::event_exchange& _events;
        std::vector<std::reference_wrapper<ecs::entity>> _entities;

        listener_id _grab_listener_id;
        listener_id _forget_listener_id;

        void grab_entity (ecs::entity &e);
        void forget_entity (ecs::entity &e);
    };


}


#endif //__ENTITY_GRABBER_HPP_
