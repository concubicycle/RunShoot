//
// Created by sava on 10/20/19.
//

#ifndef __BEHAVIOR_HPP_
#define __BEHAVIOR_HPP_

#include <vector>

#include <ecs/ecs_types.hpp>
#include <ecs/entity.hpp>
#include <events/event_exchange.hpp>
#include "behavior_context.hpp"

namespace core
{
    class behavior
    {
    public:
        explicit behavior(events::event_exchange &events) : _events(events)
        {
            std::function<void(ecs::entity&)> f = std::bind(&behavior::on_entity_created, this, std::placeholders::_1);
            _listener_id = events.subscribe<ecs::entity&>(events::event_type::entity_created, f);
        }

        virtual ~behavior()
        {
            _events.unsubscribe(events::event_type::entity_created, _listener_id);
        }

        virtual component_bitset required_components() const = 0;


        void update(behavior_context &ctx)
        {
            for (auto& e : _entities)
                update_single(e, ctx);
        }

    protected:
        virtual void update_single(ecs::entity& e, behavior_context &ctx) = 0;

    private:
        events::event_exchange& _events;
        std::vector<std::reference_wrapper<ecs::entity>> _entities;
        listener_id _listener_id;

        void on_entity_created(ecs::entity& e)
        {
            auto req_comps = required_components();
            auto e_comps = e.archetype_id();
            if ((req_comps & e_comps) == req_comps)
            {
                _entities.emplace_back(e);
            }
        }
    };

}

#endif //__BEHAVIOR_HPP_
